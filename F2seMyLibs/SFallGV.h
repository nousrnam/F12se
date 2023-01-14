#ifndef SFALLGV_H
#define SFALLGV_H

#include <vector>
#include <map>
#include <string>

#include "commontypes.h"
#include "misc.h"
#include "ntst_base.h"
#include "ntst_stringpp.hpp"

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

struct ArrayVar
    {
    uint32 id;
	std::vector<uint32> types;
	std::vector<char> data;
    };

class SFallGV
    {
public:
    SFallGV() : unused(0), gainStatFix(0) {}
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
    std::vector<ArrayVar> arrays;
    };

#endif // SFALLGV_H
