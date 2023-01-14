/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef falloutH
#define falloutH

#include <vector>
#include <algorithm>
#include "FileFinder.h"
#include "msg.h"
#include "gam.h"
#include "commontypes.h"
#include "VecOfStr.h"
#include "ntst_loggingpp.hpp"
#include "ntst_stringpp.hpp"

#define ITEMS 0x0
#define CRITTERS 0x1
#define SCENERY 0x2
#define WALLS 0x3
#define TILES 0x4
#define MISC 0x5

/*#define PRO_PID 0x0000
#define PRO_DID 0x0004
#define PRO_FID 0x0008*/
#define ITM_LDIST 0x000C
#define ITM_LINTEN 0x0010
#define ITM_FLAGS 0x0014
#define ITM_EXTFLAGS 0x0018
#define ITM_SID 0x001C
#define ITM_OBJSUBTYPE 0x0020
#define ITM_MATERIALID 0x0024
#define ITM_SIZE 0x0028
#define ITM_WEIGHT 0x002C
#define ITM_COST 0x0030
#define ITM_INVFID 0x0034
#define ITM_SNDID 0x0038

#define ITMT_ARMOR 0x0
#define ITMT_CONTAINER 0x1
#define ITMT_DRUG 0x2
#define ITMT_WEAPON 0x3
#define ITMT_AMMO 0x4
#define ITMT_MISCITEM 0x5
#define ITMT_KEY 0x6

class MemBuffer;

/*
Format like:
#####
# Arroyo Quests
#####
# Kill the Evil Plants
# Arroyo, GVAR_KILL_EVIL_PLANTS, Quest_Accepted, Quest_Completed
1500, 100, 9, 2, 6
*/
template <size_t colCount>
class DataTxtFile
    {
public:
    DataTxtFile() {;}
    void LoadFromMem(const MemBuffer& buf);
    void Sort(size_t colNum);
    void Clear() {rows.clear();}
    size_t Rows() const {return rows.size();}
    size_t Cols() const {return colCount;}
    size_t Get(size_t row, size_t col) const
        {
        if (row >= rows.size())
            {
            ntst_log_error("Row is out of range.");
            return 0;
            }
        if (col >= colCount)
            {
            ntst_log_error("Column is out of range.");
            return 0;
            }
        return rows[row].cols[col];
        }
private:
    struct Row
        {
        int cols[colCount];
        };
    struct Compare
        {
        Compare(size_t _col) : col(_col) {;}
        bool operator() (const Row& r1, const Row& r2) {return r1.cols[col] < r2.cols[col];}
        size_t col;
        };
    bool LoadRow(const MemBuffer& buf, const std::string& line, Row& row);
    std::vector<Row> rows;
    };

template <size_t colCount>
void DataTxtFile<colCount>::LoadFromMem(const MemBuffer& buf)
    {
    VectorOfStrings tmpList;

    Clear();
    tmpList.LoadFromMemory(buf);
    for (size_t i = 0; i < tmpList.size(); ++i)
        {
        Row row;
        if (LoadRow(buf, tmpList[i], row))
            rows.push_back(row);
        }
   }

template <size_t colCount>
void DataTxtFile<colCount>::Sort(size_t colNum)
    {
    Compare comp(colNum);
    std::sort(rows.begin(), rows.end(), comp);
    }


typedef struct
   {
   int start;//строка откуда начинается
   int length;//сколько продолжается
   } QuestRec;

class QuestsTxt: public DataTxtFile<5>
   {
public:
   void LoadFromMem(const MemBuffer& buf);
   std::vector<QuestRec> records;
private:
   };

struct ScriptRec
   {
   std::string filename;
   std::string comment;
   int varCount;
   };

class ScriptList : public std::vector<ScriptRec>
    {
public:
    void LoadFromMem(const MemBuffer& buf);
    unsigned GetIndexBySid(uint32 sid)
        {
        return sid & 0xFFFF;
        }
    std::string GetCommentBySid(uint32 sid)
        {
        const unsigned num = GetIndexBySid(sid);
        if (num < size())
            return operator[](num).comment;
        std::string result = "Requested script(sid=" + ntst::to_string(sid) + ") is outside of the scripts list(size=" + ntst::to_string(size()) + ")";
        ntst_log_warning(result);
        return result;
        }
    std::string GetScriptFilenameBySid(uint32 sid)
        {
        const unsigned num = GetIndexBySid(sid);
        if (num < size())
            return operator[](num).filename;
        return "Name not found, SID=" + ntst::to_string(sid);
        }
   };

bool GetScriptDescSize(unsigned scriptType/* == PID >> 24 */, int *size);
unsigned GetScriptDescSizeForJunk(unsigned scriptType);

bool GetItemSizeFromType(unsigned type, unsigned *size);
#endif
