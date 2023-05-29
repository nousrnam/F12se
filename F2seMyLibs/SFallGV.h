#ifndef SFALLGV_H
#define SFALLGV_H

#include <assert.h>
#include <vector>
#include <map>
#include <string>

#include "commontypes.h"
#include "misc.h"
#include "ntst_base.h"
#include "ntst_stringpp.hpp"
#include "ntst_loggingpp.hpp"

enum SFallDataType : uint32
{
    SFDT_NONE  = 0,
    SFDT_INT   = 1,
    SFDT_FLOAT = 2,
    SFDT_STR   = 3,
    SFDT_END // not sfall type, just to check that type is "< SFDT_END"
};

struct ArrayVarNew
{
    ArrayVarNew(SFallDataType t = SFDT_NONE) : type(t) {}
    ArrayVarNew(FILE* f, bool& ok)
    {
        uint32 t;
        ok = fread(&t, sizeof(t), 1, f) == 1;
        if (!ok)
            return;
        ok = (t < SFDT_END);
        if (!ok)
        {
            ntst_log_warning("Invalid array element type " + ntst::to_string(t));
            return;
        }
        type = (SFallDataType)t;
        if (type == SFDT_STR)
        {
            uint32 len;
            ok = fread(&len, sizeof(len), 1, f) == 1;
            if (!ok)
                return;
            data.resize(len);
            ok = fread(data.data(), len, 1, f) == 1;
            if (!ok)
                return;
        }
        else
        { // sfall always use 4 bytes for other types
            data.resize(sizeof(int32));
            ok = fread(data.data(), sizeof(int32), 1, f) == 1;
            if (!ok)
                return;
        }
    }
    SFallDataType type;
    std::vector<char> data; // string are stored with '\0'
    int32 GetInt() const
    {
        assert(type == SFDT_INT);
        //assert(data.size() == sizeof(int32));
        if (data.size() >= sizeof(int32))
            return *(int32*)data.data();
        return 0;
    }
    void SetInt(int32 val)
    {
        data.resize(sizeof(val));
        type = SFDT_INT;
        *(int32*)data.data() = val;
    }
    float GetFloat() const
    {
        assert(type == SFDT_FLOAT);
        //assert(data.size() == sizeof(float));
        if (data.size() >= sizeof(float))
            return *(float*)data.data();
        return 0;
    }
    void SetFloat(float val)
    {
        data.resize(sizeof(val));
        type = SFDT_FLOAT;
        *(float*)data.data() = val;
    }
    std::string GetString() const
    {
        switch (type)
        {
        case SFDT_NONE: return "NONE";
        case SFDT_INT: return ntst::to_string(GetInt());
        case SFDT_FLOAT: return ntst::to_string(GetFloat());
        default: return data.size() > 0 ? std::string(data.data(), data.size() - 1) : std::string(); // -1 for '\0'
        }
    }
    void SetString(const std::string& val)
    {
        unsigned toCopy = val.size() + 1; // +1 for '\0'
        data.resize(toCopy);
        type = SFDT_STR;
        memcpy(data.data(), val.c_str(), toCopy);
    }
    void SetType(SFallDataType t)
    {
        type = t;
    }
    bool Save(FILE* f) const
    {
        uint32 val = type;
        if (fwrite(&val, sizeof(val), 1, f) != 1)
            return false;
        if (type == SFDT_STR)
        {
            val = data.size();
            if (fwrite(&val, sizeof(val), 1, f) != 1)
                return false;
            if (fwrite(data.data(), data.size(), 1, f) != 1)
                return false;
        }
        else
        { // SFDT_NONE saves 4 bytes for value too
            val = 0;
            if (data.size() >= sizeof(val))
                memcpy(&val, data.data(), sizeof(val));
            else if (type != SFDT_NONE)
                ntst_log_warning("Unexpected element size in sfall array!");
            if (fwrite(&val, sizeof(val), 1, f) != 1)
                return false;

        }
        return true;
    }
};

struct ArrayNew
{
    enum
    {
        FLAG_ASSOC = 1
    };
    typedef std::pair<ArrayVarNew, ArrayVarNew> KeyValPair;
    ArrayNew(FILE* f, bool& ok) : key(f, ok)
    {
        // key
        if (!ok)
            return;
        // flags
        ok = fread(&flags, sizeof(flags), 1, f) == 1;
        if (!ok)
            return;
        uint32 count;
        ok = fread(&count, sizeof(count), 1, f) == 1;
        if (!ok)
            return;
        if (IsAssoc())
        { // count / 2 because keys and values
            ok = ((count % 2) == 0);
            if (!ok)
                return;
            count /= 2;
            // elements
            for (unsigned i = 0; i < count; ++i)
            {
                ArrayVarNew key(f, ok);
                if (!ok)
                    return;
                ArrayVarNew val(f, ok);
                if (!ok)
                    return;
                data.push_back(KeyValPair(key, val));
            }
        }
        else
        {
            ArrayVarNew key(SFDT_INT);
            key.data.resize(sizeof(int32));
            int32* keyPtr = (int32*)key.data.data();
            for (int32 i = 0; i < (int32)count; ++i)
            {
                *keyPtr = i;
                ArrayVarNew val(f, ok);
                if (!ok)
                    return;
                data.push_back(KeyValPair(key, val));
            }
        }
    }
    bool IsAssoc() const {return flags & FLAG_ASSOC;}
    bool Save(FILE* f) const
    {
        if (!key.Save(f))
            return false;
        if (fwrite(&flags, sizeof(flags), 1, f) != 1)
            return false;
        uint32 val = data.size();
        const bool assoc = IsAssoc();
        if (assoc)
            val *= 2;
        if (fwrite(&val, sizeof(val), 1, f) != 1)
            return false;
        for (std::vector<KeyValPair>::const_iterator it = data.begin(); it != data.end(); ++it)
        {
            if (assoc)
            { // save key for assoc arrays
                if (!it->first.Save(f))
                    return false;
            }
            if (!it->second.Save(f))
                return false;
        }
        return true;
    }
    ArrayVarNew key;
    std::vector<KeyValPair> data;
    uint32 flags;
};

struct FakePerk
    {
	int32 Level;
	int32 Image;
	char Name[64];
	char Desc[1024];
	std::string GetName() const {return std::string(Name);} // TODO unsafe
	std::string GetDesc() const {return std::string(Desc);} // TODO unsafe
	std::string GetString() const {return GetName() + '=' + ntst::to_string(Level);}
    };

#define MY_STATIC_ASSERT(condition, name) typedef char name[(condition) ? 1 : -1]
MY_STATIC_ASSERT(sizeof(FakePerk) == 8+64+1024, bad_size);

struct GlobalVar
    {
    GlobalVar() : id(0) {paddedVal.valWithPadding = 0;}
    GlobalVar(int64 i, int64 v) : id(i) {paddedVal.valWithPadding = v;}
	int64 id; // size in file 64
	//int32 val; // size in file 64
	union
        {
        int32 val;
        int64 valWithPadding;
        } paddedVal;
    std::string GetName() const;
    std::string GetString() const {return GetName() + '=' + ntst::to_string(paddedVal.val);}
    };

MY_STATIC_ASSERT(sizeof(GlobalVar) == 16, bad_size);

struct ArrayVarOld
{
    uint32 id;
    uint32 dataSize;
    std::vector<uint32> types;
    std::vector<char> data;
};

class SFallGV
    {
public:
    SFallGV() : unused(0), gainStatFix(0), mIsValid(false) {}
    void Load(const std::string& path);
    void Save(const std::string& path) const;
    void Clear();

    typedef std::vector<GlobalVar> GlobalVars;
    GlobalVars globalVars;
    uint32 unused;
    uint32 gainStatFix;
    typedef std::vector<FakePerk> FakePerks;
    FakePerks fakeTraits;
    FakePerks fakePerks;
    FakePerks fakeSelectablePerks;
    std::vector<ArrayVarOld> arraysOld;
    std::vector<ArrayNew> arraysNew;
    std::vector<uint32> drugFixes;
    bool mIsValid;
    };

#endif // SFALLGV_H
