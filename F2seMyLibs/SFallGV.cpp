#include <assert.h>
#include <stdio.h>

#include "SFallGV.h"
#include "ntst_loggingpp.hpp"

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
void SaveVector(FILE* f, const std::vector<T>& vec)
    {
    uint32 elementCount = vec.size();
    fwrite(&elementCount, sizeof(elementCount), 1, f);
    for (typename std::vector<T>::const_iterator it = vec.begin(); it != vec.end(); ++it)
        {
        fwrite(&(*it), sizeof(*it), 1, f);
        }
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

void SaveGlobalVars(FILE* f, const SFallGV::GlobalVars& vec)
    {
    uint32 elementCount = vec.size();
    fwrite(&elementCount, sizeof(elementCount), 1, f);
    for (SFallGV::GlobalVars::const_iterator it = vec.begin(); it != vec.end(); ++it)
        {
        fwrite(&it->id, sizeof(it->id), 1, f);
        fwrite(&it->paddedVal.valWithPadding, sizeof(it->paddedVal.valWithPadding), 1, f);
        }
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
    FILE* f = fopen(path.c_str(), "wb");
    if (f == NULL)
        {
        ntst_log_error("Can't open file: " + path);
        return;
        }
    // Global variables
    SaveGlobalVars(f, globalVars);
    // Unused
    fwrite(&unused, sizeof(int32), 1, f);
    // GainStatFix
    fwrite(&gainStatFix, sizeof(gainStatFix), 1, f);
    // Perks
    SaveVector(f, fakeTraits);
    SaveVector(f, fakePerks);
    SaveVector(f, fakeSelectablePerks);
    // Arrays
    uint32 arraysCount = arrays.size();
    fwrite(&arraysCount, sizeof(arraysCount), 1, f);
    for (std::vector<ArrayVar>::const_iterator it = arrays.begin(); it != arrays.end(); ++it)
        {
        const ArrayVar& var = *it;
        fwrite(&var.id, sizeof(var.id), 1, f);
        uint32 typessize = var.types.size();
        fwrite(&typessize, sizeof(typessize), 1, f);
        uint32 datasize;
        if (typessize != 0)
            datasize = var.data.size() / typessize;
        else
            datasize = 0;
        fwrite(&datasize, sizeof(datasize), 1, f);
        fwrite(var.types.data(), 4, typessize, f);
        fwrite(var.data.data(), 1, var.data.size(), f);
        }
    fclose(f);
    }

void SFallGV::Clear()
    {
    globalVars.clear();
    gainStatFix = 0;
    fakeTraits.clear();
    fakePerks.clear();
    fakeSelectablePerks.clear();
    arrays.clear();
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
    ArrayVar var;
    bool allRead = false;
    // Global variables
    if (!LoadGlobalVars(f, globalVars))
        goto QUIT_LOAD;
    ntst_log_info("sfallgv loaded global variables = " + ntst::to_string(globalVars.size()));
    // Unused
    if (fread(&unused, sizeof(unused), 1, f) != 1)
        goto QUIT_LOAD;
    // GainStatFix
    if (fread(&gainStatFix, sizeof(gainStatFix), 1, f) != 1)
        goto QUIT_LOAD;
    ntst_log_info("sfallgv GainStatFix = " + ntst::to_string(gainStatFix));
    // Perks
    if (!LoadVector(f, fakeTraits))
        goto QUIT_LOAD;
    ntst_log_info("sfallgv loaded fake traits = " + ntst::to_string(fakeTraits.size()));
    if (!LoadVector(f, fakePerks))
        goto QUIT_LOAD;
    ntst_log_info("sfallgv loaded fake perks = " + ntst::to_string(fakePerks.size()));
    if (!LoadVector(f, fakeSelectablePerks))
        goto QUIT_LOAD;
    ntst_log_info("sfallgv loaded fake selectable perks = " + ntst::to_string(fakeSelectablePerks.size()));
    // Arrays
    uint32 arraysCount;
    if (fread(&arraysCount, sizeof(arraysCount), 1, f) != 1)
        goto QUIT_LOAD;
    ntst_log_info("sfallgv arrays count(old format) = " + ntst::to_string(arraysCount));
    for (uint32 i = 0; i < arraysCount; ++i)
        {
        if (fread(&var.id, sizeof(var.id), 1, f) != 1)
            goto QUIT_LOAD;
        uint32 typessize;
        if (fread(&typessize, sizeof(typessize), 1, f) != 1)
            goto QUIT_LOAD;
        uint32 datasize;
        if (fread(&datasize, sizeof(datasize), 1, f) != 1)
            goto QUIT_LOAD;
        var.types.reserve(typessize);
        for (uint32 j = 0; j < typessize; ++j)
            {
            uint32 type;
            if (fread(&type, sizeof(type), 1, f) != 1)
                goto QUIT_LOAD;
            var.types.push_back(type);
            }
        uint32 datafullsize = typessize * datasize;
        var.data.reserve(datafullsize);
        for (uint32 j = 0; j < datafullsize; ++j)
            {
            char data;
            if (fread(&data, sizeof(data), 1, f) != 1)
                goto QUIT_LOAD;
            var.data.push_back(data);
            }
        arrays.push_back(var);
        var.types.clear();
        var.data.clear();
        }
    if (arraysCount != 0)
        ntst_log_info("sfallgv loaded arrays(old format) = " + ntst::to_string(arrays.size()));
    uint32 newArraysCount;
    if (fread(&newArraysCount, sizeof(newArraysCount), 1, f) == 1)
        {
        ntst_log_info("sfallgv arrays count(new format) = " + ntst::to_string(newArraysCount));
        if (newArraysCount != 0)
            {
            ntst_log_warning("sfallgv.sav has new format arrays, they are unsupported by editor! Do not change this file from editor!");
            goto QUIT_LOAD;
            }
        }
    allRead = true;
QUIT_LOAD:
    const long int curPos = ftell(f);
    fseek(f,0,SEEK_END);
    const long int endPos = ftell(f);
    if (!allRead || (curPos != endPos))
        {
        ntst_log_warning("File: " + path + " was loaded with errors. Position " + ntst::to_string(curPos) + '/' + ntst::to_string(endPos));
        }
    fclose(f);
    }
