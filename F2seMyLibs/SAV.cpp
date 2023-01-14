/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "SAV.h"
#include "fallout.h"
#include "ntst_loggingpp.hpp"
#include "zlib/zlib.h"
#include "MemBufferEX.h"

enum SavFormat
    {
    FORMAT_PACKED,
    FORMAT_UNPACKED,
    FORMAT_INVALID
    };

SavFormat GetFileFormat(const std::string& path)
    {
    FILE *f = fopen(path.c_str(), "rb");
    if (f == NULL)
        {
        ntst_log_error("Can't open file: " + path);
        return FORMAT_INVALID;
        }
    int32 head;
    if (fread(&head, 4, 1, f) != 1)
        {
        fclose(f);
        ntst_log_error("Can't read header: " + path);
        return FORMAT_INVALID;
        }
    fclose(f);
    if ((head & 0xFFFF) == 0x8b1f)
        return FORMAT_PACKED;
    if (head == 0x13000000 || head == 0x14000000)
        return FORMAT_UNPACKED;
    return FORMAT_INVALID;
    }

bool SAVfile::LoadFile(const std::string& path)
    {
    filepath.clear();
    SavFormat format = GetFileFormat(path);
    switch(format)
        {
        case FORMAT_PACKED:
            gziped = true;
            if (!LoadGZipFileInMemBuffer(path, buf))
                return false;
            break;
        case FORMAT_UNPACKED:
            gziped = false;
            if (!buf.LoadFile(path))
                return false;
            break;
        default:
            ntst_log_error("Invalid file: " + path);
            return false;
        }
    svarNum = buf.GetInv32(SAV_SVARNUM_OFFSET);
    mvarNum = buf.GetInv32(SAV_MVARNUM_OFFSET);
    svarOffset = mvarNum * sizeof(int32) + SAV_MVAR_OFFSET;
    filepath = path;
    return true;
    }

bool SAVfile::SaveFile(const std::string& path)
    {
    if (gziped)
        return MemBufferSaveInGZipFile(path, buf);
    return buf.SaveInFile(path);
    }

unsigned SAVfile::GetTilesSize() const
    {
    unsigned tilesSize = 0;
    int32 elevFlag = buf.GetInv32(SAV_ELEVATION_FLAGS_OFFSET);
    if ((elevFlag & 0x2) == 0)
        tilesSize += 40000;
    if ((elevFlag & 0x4) == 0)
        tilesSize += 40000;
    if ((elevFlag & 0x8) == 0)
        tilesSize += 40000;
    return tilesSize;
    }

#define SCRIPT_IN_BLOCK_COUNT 16

int ReadScriptDesc(const MemBuffer& buf, unsigned offset, SavScriptRec *scriptRec)
    {
    unsigned scrType = buf.GetInv32(offset) >> 24;
    int scrDescSize;
    if (!GetScriptDescSize(scrType, &scrDescSize))
        {
        ntst_log_error("Unknown script type = " + ntst::to_string(scrType));
        return -1;
        }
    if (buf.GetSize() < offset + scrDescSize)
        {
        ntst_log_error("Reading script descriptor out of buffer size.");
        return -1;
        }
    if (scriptRec != NULL)
        {
        scriptRec->SID = buf.GetInv32(offset + scrDescSize - 0x34);
        scriptRec->startIndex = buf.GetInv32(offset + scrDescSize - 0x28);
        scriptRec->lvarCount = buf.GetInv32(offset + scrDescSize - 0x24);
        scriptRec->scrType = scrType;
        }
    return scrDescSize;
    }

int ReadScriptDescJunk(const MemBuffer& buf, unsigned offset)
    {
    unsigned scrType = buf.GetInv32(offset) >> 24;
    int scrDescSize = GetScriptDescSizeForJunk(scrType);
    // TeamX: "Неиспользованные" описатели чаще всего имеют тип "01" (тогда они имеют длину 72 байта, т.е. 18 DWORD), либо имеют тип отличный от "01" (например, "03" или даже "EC") - тогда такие описатели имеют длину 64 байта, реально они заполнены "мусором", в независимости от типа группы.
    return scrDescSize;
    }

int ReadScriptDescBlock(const MemBuffer& buf, unsigned offset, unsigned usedCount, unsigned unusedCount, std::vector<SavScriptRec> &result)
    {
    assert(usedCount + unusedCount == SCRIPT_IN_BLOCK_COUNT);

    unsigned curOffset = offset;
    for (unsigned i = 0; i < usedCount; i++)
        {
        SavScriptRec scriptRec;
        int scriptDescSize = ReadScriptDesc(buf, curOffset, &scriptRec);
        if (scriptDescSize <= 0)
            return -1;
        result.push_back(scriptRec);
        curOffset += scriptDescSize;
        }
    for (unsigned i = 0; i < unusedCount; i++)
        curOffset += ReadScriptDescJunk(buf, curOffset);

    unsigned used = buf.GetInv32(curOffset);
    if (usedCount != used)
        {
        ntst_log_warning("The number of script descriptors used do not match. " + ntst::to_string(usedCount) + " vs " + ntst::to_string(used));
        }
    curOffset += 4; //счетчик использованных
    curOffset += 4; //неизвестно что. У TeamX написано, что возможно CRC
    return curOffset - offset;
    }

int SAVfile::LoadScriptsGroup(unsigned offset, std::vector<SavScriptRec> &result) const
    {
    unsigned curOffset = offset + sizeof(int32);
    if (buf.GetSize() < curOffset)
        {
        ntst_log_error("Reading script descriptor group out of buffer size.");
        return -1;
        }

    int32 scriptsInGroup = buf.GetInv32(offset);
    if (scriptsInGroup == 0)
        return sizeof(int32);

    while (scriptsInGroup > SCRIPT_IN_BLOCK_COUNT)
        {
        int blockSize = ReadScriptDescBlock(buf, curOffset, SCRIPT_IN_BLOCK_COUNT, 0, result);
        if (blockSize <= 0)
            {
            ntst_log_error("Invalid script descriptors block. Loading aborted.");
            return -1;
            }
        scriptsInGroup -= SCRIPT_IN_BLOCK_COUNT;
        curOffset += blockSize;
        }
    int blockSize = ReadScriptDescBlock(buf, curOffset, scriptsInGroup, SCRIPT_IN_BLOCK_COUNT - scriptsInGroup, result);
    curOffset += blockSize;

    //если нужно прочитаем Too Many Items бажные блоки
    int32 tmpInt = buf.GetInv32(curOffset);
    while (tmpInt >= 0x01000000)
        {
        ntst_log_warning("Reading block at offset " + ntst::to_string_hex_pref(curOffset) + " affected by \"Too Many Items bug\".");
        blockSize = ReadScriptDescBlock(buf, curOffset, 0, 16, result);
        if (blockSize <= 0)
            {
            ntst_log_error("Invalid script descriptors block. Loading aborted.");
            return -1;
            }
        curOffset += blockSize;
        tmpInt = buf.GetInv32(curOffset);
        }
    return (curOffset - offset);
    }

std::vector<SavScriptRec> SAVfile::GetScripts() const
    {
    std::vector<SavScriptRec> result;
    unsigned curOffset = GetScriptsOffset();
    for (int seqNum = 0; seqNum < 5; seqNum++)
        {
        int groupSize = LoadScriptsGroup(curOffset, result);
        if (groupSize <= 0)
            {
            ntst_log_error("Failed to load script group " + ntst::to_string(seqNum));
            return result;
            }
        curOffset += groupSize;
        }
    //TODO check all variables are inside array
    /*for (i = 0; i < (int)scripts.size(); i++)
        {
        if (scripts[i].offset + scripts[i].lvarCount > cntSVAR)
            ShowMessage("d>_<b");
        }*/
    return result;
    }
