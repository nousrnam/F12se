/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef SAVFILE_H
#define SAVFILE_H

#include <string>
#include <vector>

#include "Defines.h"
#include "MemBuffer.h"

typedef struct
   {
   unsigned SID;
   int startIndex; // index in array of local variables. Real offset(from begining of file) for first local variable of this script = array offset + startIndex * sizeof(int32)
   int lvarCount;
   char scrType;
   } SavScriptRec;

class SAVfile
    {
public:
    SAVfile() : svarNum(0), svarOffset(0), mvarNum(0), gziped(true) {}
    bool LoadFile(const std::string& path);
    bool SaveFile() {return SaveFile(filepath);}
    bool SaveFile(const std::string& path);
    int32    GetInv32(unsigned offset) const {return buf.GetInv32(offset);}
    bool     SetInv32(unsigned offset, uint32 val) {return buf.SetInv32(offset, val);}
    unsigned GetTilesSize() const;
    unsigned GetScriptsOffset() const {return svarOffset + svarNum * sizeof(int32) + GetTilesSize();}
    unsigned GetSvarNum() const {return svarNum;}
    int32    GetSvar(unsigned num) const {return GetInv32(svarOffset + num * sizeof(int32));}
    bool     SetSvar(unsigned num, int32 val)
        {
        if (num >= svarNum)
            return false;
        return SetInv32(svarOffset + num * sizeof(int32), val);
        }
    unsigned GetMvarNum() const {return mvarNum;}
    int32    GetMvar(unsigned num) const {return GetInv32(SAV_MVAR_OFFSET + num * sizeof(int32));}
    bool     SetMvar(unsigned num, int32 val)
        {
        if (num >= mvarNum)
            return false;
        return SetInv32(SAV_MVAR_OFFSET + num * sizeof(int32), val);
        }
    std::vector<SavScriptRec> GetScripts() const;
    int GetSvarOffset() const {return svarOffset;}
private:
    std::string filepath;
    MemBuffer buf;
    int LoadScriptsGroup(unsigned offset, std::vector<SavScriptRec> &result) const;
    //int8 header[10];
    unsigned svarNum;
    int svarOffset;
    unsigned mvarNum;
    bool gziped;
    };

#endif // SAVFILE_H
