/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef SaveDatH
#define SaveDatH

#include <string>
#include <stdint.h>

#include "MemBufferEX.h"
#include "commontypes.h"
#include "misc.h"
#include "GAM.h"
#include "PRO.h"
#include "IniFile.h"

bool Crc32FromFile(const std::string& filepath, uint32 &crc);

class SaveDatFile
    {
public:
    SaveDatFile();
    bool Load(const char* filepath, bool fallout1, GamFile *gvars, ProWorker *itemsPro, IniFile *aiTxt, int partySize = 26);
    bool Save(const std::string& filepath, std::string& errMsg);
    bool Save(std::string& errMsg){return Save(path, errMsg);}
    const std::string& GetPath() const {return path;}
    std::string GetDir() const;
    std::string GetCrittersDir() const {return GetDir() + "\\proto\\critters";}
    std::string ReadStr (int sectionNum, uint offset){return (char*)((uint)fileSections[sectionNum].GetAddress() + offset);}
    /*void SetGam(GamFile *gam){gvars = gam;};
    void SetItems(ProWorker *items){itemsPro = items;};
    void SetAiTxt(IniFile *ai){aiTxt = ai;};*/
    size_t GetFunctionOffset(size_t num);
    static const unsigned FUNCTIONS_NUM = 29; // Valid [0]-[27]. [28] - For crashed loading.
    MemBufferEX fileSections[FUNCTIONS_NUM];
    void Close() {path.clear(); ClearBuffers();}
    void ClearBuffers() {for (unsigned i = 0; i < FUNCTIONS_NUM; ++i) fileSections[i].Clear();}
    bool FileCrcChanged()
        {
        uint32 crc;
        if (Crc32FromFile(GetPath(), crc) && crc != originalcrc)
            return true;
        return false;
        }
    unsigned GetGvarNum() const {return fileSections[3].GetSize() / 4;}
    int32_t GetGvarVal(unsigned num) const {return fileSections[3].GetInv32(num * sizeof(int32_t));}
    void SetGvarVal(unsigned num, int32_t val)
        {
        const size_t offset = num * sizeof(int32_t);
        fileSections[3].SetInv32(offset, val);
        fileSections[5].SetInv32(offset, val);
        }
private:
    long int GetTotalSize()
        {
        long int loadedSize = 0;
        for (unsigned i = 0; i < FUNCTIONS_NUM; i++)
            loadedSize += fileSections[i].GetSize();
        return loadedSize;
        }
    void ReadRestOfFileAsFileSection28(FILE *f, long int fileSize);
    /*GamFile *gvars;
    ProWorker *itemsPro;
    IniFile *aiTxt;*/
    std::string path;
    uint32 originalcrc;
    };

#endif
