#include <assert.h>
#include <stdio.h>

#include "SFallGV.h"

unsigned Int64GetTextLen(int64 val)
    {
    unsigned len = 0;
    unsigned char* c = (unsigned char*)(&val);
    for (unsigned i = 0; i < 8; ++i, ++c)
        {
        if (*c == 0)
            break;
        if (*c < 32 || *c > 126) // not ASCII printable character
            {
            len = 0;
            break;
            }
        ++len;
        }
    return len;
    }

std::string GlobalVar::GetName() const
    {
    assert(sizeof(id) == sizeof(long long));
    unsigned len = Int64GetTextLen(id);
    if (len == 0)
        return '[' + ntst::to_string_hex_pref(id) + ']';
    assert(len <= 8);
    return std::string((char*)(&id), len);
    }

template<class T>
bool SaveVector(FILE* f, const std::vector<T>& vec)
{
    uint32 elementCount = vec.size();
    if (fwrite(&elementCount, sizeof(elementCount), 1, f) != 1)
        return false;
    for (typename std::vector<T>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (fwrite(&(*it), sizeof(*it), 1, f) != 1)
            return false;
    }
    return true;
}

template<class T>
bool LoadVector(FILE* f, std::vector<T>& vec)
    {
    uint32 elementCount;
    if (fread(&elementCount, sizeof(elementCount), 1, f) != 1)
        return false;
    //ntst_log_info("elementCount = " + ntst::to_string(elementCount));
    vec.reserve(elementCount);
    for (uint32 i = 0; i < elementCount; ++i)
        {
        T element;
        if (fread(&element, sizeof(T), 1, f) != 1)
            return false;
        vec.push_back(element);
        }
    return true;
    }

static bool SaveGlobalVars(FILE* f, const SFallGV::GlobalVars& vec)
{
    uint32 elementCount = vec.size();
    if (fwrite(&elementCount, sizeof(elementCount), 1, f) != 1)
        return false;
    for (SFallGV::GlobalVars::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (fwrite(&it->id, sizeof(it->id), 1, f) != 1)
            return false;
        if (fwrite(&it->paddedVal.valWithPadding, sizeof(it->paddedVal.valWithPadding), 1, f) != 1)
            return false;
    }
    return true;
}

bool LoadGlobalVars(FILE* f, SFallGV::GlobalVars& vec)
    {
    uint32 elementCount;
    if (fread(&elementCount, sizeof(elementCount), 1, f) != 1)
        return false;
    for (uint32 i = 0; i < elementCount; ++i)
        {
        int64 id;
        if (fread(&id, sizeof(id), 1, f) != 1)
            return false;
        int64 val;
        if (fread(&val, sizeof(val), 1, f) != 1)
            return false;
        vec.push_back(GlobalVar(id, val));
        }
    return true;
    }

void SFallGV::Save(const std::string& path) const
{
    const std::string filepathBackup = path + ".backup";
    remove(filepathBackup.c_str());
    if (rename(path.c_str(), filepathBackup.c_str()) != 0)
    {
        ntst_log_error("Can't rename original file \"" + path + "\" to \"" + filepathBackup + "\" error: " + strerror(errno));
        return;
    }
    FILE* f = fopen(path.c_str(), "wb");
    if (f == NULL)
    {
        ntst_log_error("Can't open file: \"" + path + "\" error: " + strerror(errno));
        return;
    }
    // Global variables
    bool ok = SaveGlobalVars(f, globalVars);
    // Unused
    ok = ok && (fwrite(&unused, sizeof(int32), 1, f) == 1);
    // GainStatFix
    ok = ok && (fwrite(&gainStatFix, sizeof(gainStatFix), 1, f) == 1);
    // Perks
    ok = ok && SaveVector(f, fakeTraits);
    ok = ok && SaveVector(f, fakePerks);
    ok = ok && SaveVector(f, fakeSelectablePerks);
    // Arrays Old
    uint32 arraysCountOld = arraysOld.size();
    ok = ok && (fwrite(&arraysCountOld, sizeof(arraysCountOld), 1, f) == 1);
    for (std::vector<ArrayVarOld>::const_iterator it = arraysOld.begin(); it != arraysOld.end(); ++it)
    {
        const ArrayVarOld& var = *it;
        ok = ok && (fwrite(&var.id, sizeof(var.id), 1, f) == 1);
        uint32 typessize = var.types.size();
        ok = ok && (fwrite(&typessize, sizeof(typessize), 1, f) == 1);
        ok = ok && (fwrite(&var.dataSize, sizeof(var.dataSize), 1, f) == 1);
        const size_t ret = fwrite(var.types.data(), sizeof(uint32), typessize, f);
        ok = ok && (ret == typessize);
        ok = ok && (fwrite(var.data.data(), 1, var.data.size(), f) == var.data.size());
    }
    // Arrays New
    uint32 arraysCountNew = arraysNew.size();
    ok = ok && (fwrite(&arraysCountNew, sizeof(arraysCountNew), 1, f) == 1);
    for (std::vector<ArrayNew>::const_iterator it = arraysNew.begin(); it != arraysNew.end(); ++it)
    {
        ok = ok && it->Save(f);
    }
    // BugFixes::DrugsSaveFix
    uint32 drugPidCount = drugFixes.size();
    if (drugPidCount > 0)
    {
        ok = ok && (fwrite(&drugPidCount, sizeof(drugPidCount), 1, f) == 1);
        for (uint32 i = 0; i < drugPidCount; ++i)
        {
            const uint32 pid = drugFixes[i];
            ok = ok && (fwrite(&pid, sizeof(pid), 1, f) == 1);
        }
    }
    fclose(f);
    if (ok)
    { // Deleting backup
        if (remove(filepathBackup.c_str()) != 0)
        {
            const std::string strerr = strerror(errno);
            ntst_log_warning("Can't delete backup file \"" + filepathBackup + "\" error: " + strerr);
        }
        ntst_log_info(path + " succesfully saved.");
    }
    else
    { // Restoring backup
        ntst_log_warning("Can't save file somehow. Restoring backup.");
        remove(path.c_str()); // rename may fail if new name already exist.
        if (rename(filepathBackup.c_str(), path.c_str()) != 0)
        {
            ntst_log_error("Can't restore original file from \"" + filepathBackup + "\" error: " + strerror(errno));
        }
    }
}

void SFallGV::Clear()
    {
    globalVars.clear();
    gainStatFix = 0;
    fakeTraits.clear();
    fakePerks.clear();
    fakeSelectablePerks.clear();
    arraysOld.clear();
    arraysNew.clear();
    drugFixes.clear();
    mIsValid = false;
    }

void SFallGV::Load(const std::string& path)
{
    Clear();
    FILE* f = fopen(path.c_str(), "rb");
    if (f == NULL)
    {
        ntst_log_error("Can't open file: " + path);
        return;
    }
    ArrayVarOld var;
    // Global variables
    bool ok = LoadGlobalVars(f, globalVars);
    if (!ok)
        goto QUIT_LOAD;
    ntst_log_info("sfallgv loaded global variables = " + ntst::to_string(globalVars.size()));
    // Unused
    ok = fread(&unused, sizeof(unused), 1, f) == 1;
    if (!ok)
        goto QUIT_LOAD;
    // GainStatFix
    ok = fread(&gainStatFix, sizeof(gainStatFix), 1, f) == 1;
    if (!ok)
        goto QUIT_LOAD;
    ntst_log_info("sfallgv GainStatFix = " + ntst::to_string(gainStatFix));
    // Perks
    ok = LoadVector(f, fakeTraits);
    if (!ok)
        goto QUIT_LOAD;
    ntst_log_info("sfallgv loaded fake traits = " + ntst::to_string(fakeTraits.size()));
    ok = LoadVector(f, fakePerks);
    if (!ok)
        goto QUIT_LOAD;
    ntst_log_info("sfallgv loaded fake perks = " + ntst::to_string(fakePerks.size()));
    ok = LoadVector(f, fakeSelectablePerks);
    if (!ok)
        goto QUIT_LOAD;
    ntst_log_info("sfallgv loaded fake selectable perks = " + ntst::to_string(fakeSelectablePerks.size()));
    // Arrays
    uint32 arraysCount;
    ok = fread(&arraysCount, sizeof(arraysCount), 1, f) == 1;
    if (!ok)
        goto QUIT_LOAD;
    ntst_log_info("sfallgv arrays count(old format) = " + ntst::to_string(arraysCount));
    for (uint32 i = 0; i < arraysCount; ++i)
    {
        ok = fread(&var.id, sizeof(var.id), 1, f) == 1;
        if (!ok)
            goto QUIT_LOAD;
        uint32 typessize;
        ok = fread(&typessize, sizeof(typessize), 1, f) == 1;
        if (!ok)
            goto QUIT_LOAD;
        ok = fread(&var.dataSize, sizeof(var.dataSize), 1, f) == 1;
        if (!ok)
            goto QUIT_LOAD;
        for (uint32 j = 0; j < typessize; ++j)
        {
            uint32 type;
            ok = fread(&type, sizeof(type), 1, f) == 1;
            if (!ok)
                goto QUIT_LOAD;
            var.types.push_back(type);
        }
        uint32 datafullsize = typessize * var.dataSize;
        for (uint32 j = 0; j < datafullsize; ++j)
        {
            char data;
            ok = fread(&data, sizeof(data), 1, f) == 1;
            if (!ok)
                goto QUIT_LOAD;
            var.data.push_back(data);
        }
        arraysOld.push_back(var);
        var.types.clear();
        var.data.clear();
    }
    if (arraysCount != 0)
        ntst_log_info("sfallgv loaded arrays(old format) = " + ntst::to_string(arraysOld.size()));
    // Arrays new
    uint32 newArraysCount;
    ok = fread(&newArraysCount, sizeof(newArraysCount), 1, f) == 1;
    if (!ok)
    {
        ok = feof(f);
        goto QUIT_LOAD;
    }
    ntst_log_info("sfallgv arrays count(new format) = " + ntst::to_string(newArraysCount));
    for (uint32 i = 0; i < newArraysCount; ++i)
    {
        ArrayNew arr(f, ok);
        if (!ok)
            goto QUIT_LOAD;
        arraysNew.push_back(arr);
    }
    // BugFixes::DrugsSaveFix
    uint32 drugPidCount;
    ok = fread(&drugPidCount, sizeof(drugPidCount), 1, f) == 1;
    if (!ok)
    {
        ok = feof(f);
        goto QUIT_LOAD;
    }
    ntst_log_info("sfallgv drugs fixes count = " + ntst::to_string(drugPidCount));
    for (uint32 i = 0; i < drugPidCount; ++i)
    {
        uint32 pid;
        ok = fread(&pid, sizeof(pid), 1, f) == 1;
        if (!ok)
            goto QUIT_LOAD;
        drugFixes.push_back(pid);
    }
QUIT_LOAD:
    mIsValid = ok && !ferror(f) && feof(f);
    const long int curPos = ftell(f);
    fseek(f,0,SEEK_END);
    const long int endPos = ftell(f);
    mIsValid = mIsValid && (curPos == endPos);
    if (!mIsValid)
        ntst_log_warning("File: " + path + " was loaded with errors. Position " + ntst::to_string(curPos) + '/' + ntst::to_string(endPos));
    fclose(f);
}
