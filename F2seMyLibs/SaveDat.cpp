/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include <stdio.h>
#include "Defines.h"
#include "SaveDat.h"
#include "misc.h"
#include "fallout.h"

#include "zlib/zlib.h"

#define HEADER_SIZE 0x7563
#define FUNCTION1_SIZE 0x4
#define FUNCTION8_SIZE 0x10
#define FUNCTION13_SIZE 0x14
#define FUNCTION15_SIZE 0x8
#define FUNCTION16_SIZE 0x4
#define FUNCTION17_SIZE 0x50
#define FUNCTION18_SIZE 0x5
#define FUNCTION21_SIZE 0x11
#define FUNCTION22_SIZE 0xD8
#define FUNCTION25_SIZE 0x10

#define BUF_SIZE 1024
static bool Crc32FromFile(FILE *f, uint32 &crc)
    {
    unsigned char buf[BUF_SIZE];
    long int curPos = ftell(f);
    int bytesRead;
    fseek(f, 0, SEEK_SET);

    crc = crc32(0, Z_NULL, 0);
    while((bytesRead = fread(buf, 1, BUF_SIZE, f)) > 0)
        {
        crc = crc32(crc, buf, bytesRead);
        }

    bool result = (ferror(f) == 0);

	fseek(f, curPos, SEEK_SET);
	return result;
    }

bool Crc32FromFile(const std::string& filepath, uint32 &crc)
    {
    FILE * f = fopen(filepath.c_str(), "rb");
    if (f == NULL)
        {
        ntst_log_error(std::string("Can't open file to get crc32: ") + filepath);
        return false;
        }
    bool result = Crc32FromFile(f, crc);
    fclose(f);
    return result;
    }

SaveDatFile::SaveDatFile()
    {
    for (unsigned i = 0; i < FUNCTIONS_NUM; i++)
        fileSections[i].SetName("save.dat block#" + ntst::to_string(i));
    }

bool SaveDatFile::Save(const std::string& filepath, std::string& errMsg)
    {
    if (filepath.empty())
        {
        errMsg = "Path to the file is empty.";
        ntst_log_error(errMsg);
        return false;
        }
    const std::string filepathBackup = filepath + ".backup";
    remove(filepathBackup.c_str());
    if (rename(filepath.c_str(), filepathBackup.c_str()) != 0)
        {
        const std::string strerr = strerror(errno);
        errMsg = "Can't rename original file \"" + filepath + "\" to \"" + filepathBackup + "\" error: " + strerr;
        ntst_log_error(errMsg);
        return false;
        }
    ntst_log_info("Original file renamed to: " + filepathBackup);
    FILE* f = fopen(filepath.c_str(), "wb");
    if (f == NULL)
        {
        const std::string strerr = strerror(errno);
        errMsg = std::string("Can't open file for writing: \"") + filepath + "\" error: " + strerr;
        goto FAILURE;
        }
    for (int i = 0; i <= 27; i++)
        {
        const size_t toWrite = fileSections[i].GetSize();
        if (toWrite > 0)
            {
            const size_t written = fwrite(fileSections[i].GetAddress(), toWrite, 1, f);
            if (written != 1)
                {
                const std::string strerr = strerror(errno);
                errMsg = std::string("Can't write in file: \"") + filepath + "\" errno:" + strerr;
                goto FAILURE;
                }
            }
        }
    if (ftell(f) != GetTotalSize())
        {
        errMsg = "The size of the saved data is not equal to the size in memory. File: " + filepath;
        goto FAILURE;
        }
    fclose(f);
    ntst_log_info(filepath + std::string(" succesfully saved."));
    if (filepath == path) // overwriting opened file
        Crc32FromFile(filepath, originalcrc); // update crc
    if (remove(filepathBackup.c_str()) != 0)
        {
        const std::string strerr = strerror(errno);
        ntst_log_warning("Can't delete backup file \"" + filepathBackup + "\" error: " + strerr);
        }
    else
        ntst_log_info("Original file deleted.");
    return true;
FAILURE:
    ntst_log_error(errMsg);
    if (f != NULL)
        fclose(f);
    remove(filepath.c_str()); // rename may fail if new name already exist.
    if (rename(filepathBackup.c_str(), filepath.c_str()) != 0)
        {
        const std::string strerr = strerror(errno);
        ntst_log_warning("Can't restore original file from \"" + filepathBackup + "\" error: " + strerr);
        }
    return false;
    }

std::string SaveDatFile::GetDir() const
    {
    std::string dir, file;
    if (SplitPath(path, dir, file))
        return dir;
    return std::string();
    }

static int32 preferences[13][2] = {{0,2},{0,2},{0,3},{0,2},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,50},{0,1}};
/**
 * Finds preferences offset in file starting with *offset, matches read values with preferences arary.
 * return true if found, and offset in *offset.
 */
bool FindPreferencesOffset(FILE *f, int *offset)
    {
    long pos = ftell(f);
    do
        {
        fseek(f, *offset, SEEK_SET);
        bool success = true;
        for (int i = 0; i < 13; ++i)
            {
            int32 val;
            if (fread(&val, sizeof(val), 1, f) != 1)
                {
                ntst_log_error("File reading failed, preferences offset not found.");
                return false;
                }
            val = ntst_bytes_swap32(val);
            if (val < preferences[i][0] || val > preferences[i][1])
                {
                success = false;
                break;
                }
            }
        if (success)
            {
            ntst_log_info("Preferences offset found " + ntst::to_string_hex_pref(*offset) + '.');
            fseek(f, *offset, SEEK_SET);
            return true;
            }
        *offset += 1;
        }
    while (!feof(f) && !ferror(f));
    fseek(f, pos, SEEK_SET);
    return false;
    }

bool SaveDatFile::Load(const char* filepath, bool fallout1, GamFile *gvars, ProWorker *itemsPro, IniFile *aiTxt, int partySize)
    {
    FILE *f;
    int i,
       itemOffset;
    int32 specialEncountersCount,
         tmp4B,
         tmp4B2,
         tmp4B3,
         packet_num, max_dist, min_to_hit;
    uint32 itemPID;
    int8 tmp1B;

    path = filepath;

    ClearBuffers();

    f = fopen(filepath, "rb");
    if (f == NULL)
        {
        ntst_log_error("Can't open file: " + path);
        return false;
        }
    Crc32FromFile(f, originalcrc);
    ntst_log_info("Crc32 = " + ntst::to_string_hex_pref(originalcrc));
	long int fileSize = GetFileSize(f);

#define CRITICAL_FAILURE \
    ReadRestOfFileAsFileSection28(f, fileSize); \
    fclose(f); \
    return false;

#define READ_FUNCTION(num, size) \
    if (fileSections[num].WriteFile(f, size) != size) \
        { \
        CRITICAL_FAILURE; \
        }

    //HEADER
    ntst_log_info("Loading header.");
    READ_FUNCTION(0, HEADER_SIZE);

    //FUNCTION0
    //FUNCTION1
    ntst_log_info("Loading function 1.");
    READ_FUNCTION(2, FUNCTION1_SIZE);

    //FUNCTION2
    ntst_log_info("Loading array of global variables.");
    size_t function2Size = gvars->size() * sizeof(int32);
    if (fallout1)
        function2Size++; // Почему в Фаллауте1 2 и 4 функции на один байт больше чем должны быть??!
    READ_FUNCTION(3, function2Size);

    //FUNCTION3
    ntst_log_info("Loading open maps.");
    long int f3begin = ftell(f);
    int32 openedMapCount;
    fread(&openedMapCount, sizeof(int32), 1, f);
    openedMapCount = ntst_bytes_swap32(openedMapCount);
    while (openedMapCount)
        {
        if (fread(&tmp1B, sizeof(int8), 1, f) != 1)
            {
            ntst_log_error("File reading failed.");
            CRITICAL_FAILURE
            }
        if (tmp1B == 0)
            openedMapCount--;
        }
    size_t f3Size = ftell(f) - f3begin + 4;
    fseek(f, f3begin, SEEK_SET);
    READ_FUNCTION(4, f3Size);

    //FUNCTION4
    ntst_log_info("Loading duplicate array of global variables.");
    READ_FUNCTION(5, function2Size);
    if (fileSections[5] != fileSections[3])
        ntst_log_warning("array of GVARs != duplicate array. Expect the crash.");

    //FUNCTION5
    ntst_log_info("Loading player and inventory.");
    long int f5begin = ftell(f);
    fread(&tmp4B, sizeof(int32), 1, f);
    if (tmp4B != 0x50460000)
        ntst_log_warning("Function 5 doesn't start with 0x0,0x0,'FP'. We are definitely going to crash.");
    fseek(f, f5begin + 0x48, SEEK_SET);
    uint32 itemsCount;
    fread(&itemsCount, sizeof(itemsCount), 1, f);
    itemsCount = ntst_bytes_swap32(itemsCount);
    ntst_log_info("Number of items in inventory: " + ntst::to_string(itemsCount));
    if (itemsCount > itemsPro->GetProQuantity())
        ntst_log_warning("There are more items in the inventory than in the entire game. We are going to crash in the next loop.");
    itemOffset = f5begin + FIRST_ITEM_OFFSET;
    fseek(f, itemOffset, SEEK_SET); //Эта строка нужна чтобы не было тупого бага, когда в инвентаре 0 предметов. Иначе ftell(f) выдаст не то что ожидается.
    while (itemsCount)
        {
        fseek(f, itemOffset + INVEN_ITEM_PID, SEEK_SET);
        if (fread(&itemPID, sizeof(int32), 1, f) != 1)
            {
            ntst_log_error("File reading failed.");
            CRITICAL_FAILURE
            }
        itemPID = ntst_bytes_swap32(itemPID);
        ntst_log_info("Loading item with PID " + ntst::to_string_hex_pref(itemPID) + '.');
        int32 itemType;
        if (!itemsPro->Get4b(itemPID, 0x20, &itemType))
            {
            ntst_log_error("Can't get type for item in inventory.");
            CRITICAL_FAILURE
            }
        //если встретили контейнер то к общему числу прибавим число в контейнере
        if (itemType == ITMT_CONTAINER)
            {
            fseek(f, itemOffset + 0x4C, SEEK_SET);
            if (fread(&tmp4B, sizeof(int32), 1, f) != 1)
                {
                ntst_log_error("File reading failed.");
                CRITICAL_FAILURE
                }
            tmp4B = ntst_bytes_swap32(tmp4B);
            ntst_log_info("Item is container and has " + ntst::to_string(tmp4B) + " items inside.");
            itemsCount = itemsCount + tmp4B;
            }
        unsigned itemSize;
        if (!GetItemSizeFromType(itemType, &itemSize))
            {
            ntst_log_error("Invalid item type: " + ntst::to_string(itemType) + '.');
            CRITICAL_FAILURE
            }
        itemOffset = itemOffset + itemSize;
        fseek(f, itemOffset, SEEK_SET);
        itemsCount--;
        }
    size_t f5Size = ftell(f) - f5begin + 4; // +4 byte = camera position after all items
    fseek(f, f5begin, SEEK_SET);
    READ_FUNCTION(6, f5Size);

    size_t FUNCTION6_SIZE, FUNCTION7_SIZE;
    if (fallout1)
        {
        FUNCTION6_SIZE = 0x174;
        FUNCTION7_SIZE = 0x3C;
        }
    else
        {
        FUNCTION6_SIZE = 0x178;
        FUNCTION7_SIZE = 0x4C;
        }

    //FUNCTION6
    ntst_log_info("Loading player stats.");
    READ_FUNCTION(7, FUNCTION6_SIZE);

    //FUNCTION7
    ntst_log_info("Loading kill counts.");
    READ_FUNCTION(8, FUNCTION7_SIZE);

    //FUNCTION8
    ntst_log_info("Loading tag skills.");
    READ_FUNCTION(9, FUNCTION8_SIZE);

    //FUNCTION9
    //FUNCTION10
    ntst_log_info("Loading perks.");
    int PERK_CNT;
    if (fallout1)
        PERK_CNT = F1_PERKS_CNT;
    else
        PERK_CNT = F2_PERKS_CNT * partySize;
    size_t f10Size = sizeof(int32) * PERK_CNT;
    READ_FUNCTION(11, f10Size);

    //FUNCTION11
    ntst_log_info("Loading combat.");
    long int f11begin = ftell(f);
    size_t f11Size;
    fread(&tmp4B, sizeof(int32), 1, f);
    tmp4B = ntst_bytes_swap32(tmp4B);
    if (tmp4B == 2)
        f11Size = 4;
    else
        {
        fseek(f, f11begin + 0x18, SEEK_SET);
        fread(&tmp4B, sizeof(int32), 1, f);
        tmp4B = ntst_bytes_swap32(tmp4B);
        f11Size = 0x20 + 0x4 * tmp4B;
        if (!fallout1)
            f11Size += 0x10 * tmp4B;
        }
    fseek(f, f11begin, SEEK_SET);
    READ_FUNCTION(12, f11Size);

    //FUNCTION12
    //У меня в сейве FALLOUT1 не было 12 функции, но у меня не было и партийцев
    if (!fallout1)
        {
        ntst_log_info("Loading party AI.");
        if (aiTxt->Empty())
            {
            ntst_log_error("ai.txt is empty loading aborted.");
            CRITICAL_FAILURE
            }
        size_t f12Size = 0;
        long int f12begin = ftell(f);
        while (1)
            {
            fseek(f, f12begin + f12Size, SEEK_SET);
            fread(&tmp4B, sizeof(int32), 1, f);
            fread(&tmp4B2, sizeof(int32), 1, f);
            if (fread(&tmp4B3, sizeof(int32), 1, f) != 1)
                {
                ntst_log_error("File reading failed.");
                CRITICAL_FAILURE
                }
            tmp4B = ntst_bytes_swap32(tmp4B);
            tmp4B2 = ntst_bytes_swap32(tmp4B2);
            tmp4B3 = ntst_bytes_swap32(tmp4B3);
            bool isFinded = false;
            for (IniFile::const_iterator it = aiTxt->begin(); it != aiTxt->end(); ++it)
                {
                const IniSection& curSection = it->second;
                const std::string* tmpstr = curSection.FindValue("packet_num");
                if (tmpstr == NULL)
                    continue;
                packet_num = atoi(tmpstr->c_str());

                tmpstr = curSection.FindValue("max_dist");
                if (tmpstr == NULL)
                    continue;
                max_dist = atoi(tmpstr->c_str());

                tmpstr = curSection.FindValue("min_to_hit");
                if (tmpstr == NULL)
                    continue;
                min_to_hit = atoi(tmpstr->c_str());

                if (tmp4B  == packet_num &&
                    tmp4B2 == max_dist   &&
                    tmp4B3 == min_to_hit)
                    {
                    isFinded = true;
                    break;
                    }
                }
            if (isFinded)
                f12Size += 0xB4;
            else
                break;
            }
        fseek(f, f12begin, SEEK_SET);
        READ_FUNCTION(13, f12Size);
        }

    //FUNCTION13
    ntst_log_info("Loading experience and level.");
    READ_FUNCTION(14, FUNCTION13_SIZE);

    if (fallout1)
        {
        //FUNCTION14 FALLOUT1
        long int f14begin = ftell(f);
        long int f15begin;
        // Have same unknown format as Function 24 - Event queue in Fallout2
        int offset = f14begin;
        if (!FindPreferencesOffset(f, &offset))
            {
            CRITICAL_FAILURE
            }
        f15begin = offset - FUNCTION15_SIZE - FUNCTION16_SIZE; // trait begin = PreferencesOffset - trait size(8) - unknown(4)
        size_t f14Size = f15begin - f14begin;
        fseek(f, f14begin, SEEK_SET);

        ntst_log_info("Loading event queue.");
        READ_FUNCTION(15, f14Size);
        //FUNCTION15 FALLOUT1
        ntst_log_info("Loading traits.");
        READ_FUNCTION(16, FUNCTION15_SIZE);

        //FUNCTION16 FALLOUT1
        ntst_log_info("Loading function 16.");
        READ_FUNCTION(17, FUNCTION16_SIZE);

        //FUNCTION17 FALLOUT1
        ntst_log_info("Loading preferences.");
        READ_FUNCTION(18, FUNCTION17_SIZE);

        //FUNCTION18 FALLOUT1
        ntst_log_info("Loading character window.");
        READ_FUNCTION(19, FUNCTION18_SIZE);

        size_t bytesLeft = fileSize - ftell(f);
        // UNKNOWN
        ntst_log_info("Loading worldmap + unknown data.");
        READ_FUNCTION(21, bytesLeft);

        //END FALLOUT1
        fclose(f);
        return true;
        }

    //FUNCTION14
    //FUNCTION15
    ntst_log_info("Loading traits.");
    READ_FUNCTION(16, FUNCTION15_SIZE);

    //FUNCTION16
    ntst_log_info("Loading function 16.");
    READ_FUNCTION(17, FUNCTION16_SIZE);

    //FUNCTION17
    ntst_log_info("Loading preferences.");
    READ_FUNCTION(18, FUNCTION17_SIZE);

    //FUNCTION18
    ntst_log_info("Loading character window.");
    READ_FUNCTION(19, FUNCTION18_SIZE);

    //FUNCTION19
    ntst_log_info("Loading world map.");
    long int f19begin = ftell(f);
    fseek(f, f19begin + 0x2C, SEEK_SET);
    int32 citiesCount;
    if (fread(&citiesCount, sizeof(citiesCount), 1, f) != 1)
        {
        ntst_log_error("File reading failed.");
        CRITICAL_FAILURE
        }
    citiesCount = ntst_bytes_swap32(citiesCount);
    ntst_log_info("Number of cities is " + ntst::to_string(citiesCount));
    size_t f19Size = 0x30;
    for (i = 0; i < citiesCount; i++)
        {
        //считаем число входов
        fseek(f, f19begin + f19Size + 0x10, SEEK_SET);
        if (fread(&tmp4B, sizeof(int32), 1, f) != 1)
            {
            ntst_log_error("File reading failed.");
            CRITICAL_FAILURE
            }
        tmp4B = ntst_bytes_swap32(tmp4B);
        //увеличим размер функции на размер записи о городе
        f19Size += 0x14 + (tmp4B * 4);
        }
    fseek(f, f19begin + f19Size, SEEK_SET);
    int32 tilesCount;
    if (fread(&tilesCount, sizeof(int32), 1, f) != 1)
        {
        ntst_log_error("File reading failed.");
        CRITICAL_FAILURE
        }
    tilesCount = ntst_bytes_swap32(tilesCount);
    ntst_log_info("Number of tiles is " + ntst::to_string(tilesCount));
    //посчитаем смещение после тумана
    //до тумана 2 по 4 байта а дальше уже сам туман
    f19Size += 2 * 0x4 + 0x4 * tilesCount * 7 * 6; // each tile is 7x6 squares
    fseek(f, f19begin + f19Size, SEEK_SET);
    fread(&specialEncountersCount, sizeof(int32), 1, f);
    specialEncountersCount = ntst_bytes_swap32(specialEncountersCount);
    ntst_log_info("Number of special encounters is " + ntst::to_string(specialEncountersCount));
    //сам счетчик встреч + число записей на их размер
    f19Size += 0x4 + specialEncountersCount * 0xC;
    fseek(f, f19begin, SEEK_SET);
    READ_FUNCTION(20, f19Size);

    //FUNCTION20
    //FUNCTION21
    ntst_log_info("Loading video archives.");
    READ_FUNCTION(22, FUNCTION21_SIZE);

    //FUNCTION22
    ntst_log_info("Loading skill use.");
    READ_FUNCTION(23, FUNCTION22_SIZE);

    //FUNCTION23
    ntst_log_info("Loading party.");
    long int f23begin = ftell(f);
    if (fread(&tmp4B, sizeof(int32), 1, f) != 1)
        {
        ntst_log_error("File reading failed.");
        CRITICAL_FAILURE
        }
    tmp4B = ntst_bytes_swap32(tmp4B);
    size_t f23Size = 0x4 + (0x4 * tmp4B) + (0xC * (partySize - 1));
    fseek(f, f23begin, SEEK_SET);
    READ_FUNCTION(24, f23Size);

    //FUNCTION24
    ntst_log_info("Loading event queue.");
    long int f24begin = ftell(f);
    size_t f24Size = fileSize - f24begin - FUNCTION25_SIZE;
    READ_FUNCTION(25, f24Size);

    //FUNCTION25
    ntst_log_info("Loading interface.");
    READ_FUNCTION(26, FUNCTION25_SIZE);

    //END FALLOUT2
    fclose(f);
    return true;
    }

size_t SaveDatFile::GetFunctionOffset(size_t num)
    {
    size_t result = 0;
    if (num >= FUNCTIONS_NUM)
        return result;
    for (size_t i = 0; i < num; i++)
        result += fileSections[i].GetSize();
    return result;
    }

void SaveDatFile::ReadRestOfFileAsFileSection28(FILE *f, long int fileSize)
    {
    long int loadedSize = GetTotalSize();
    if (loadedSize < fileSize)
        {
        fseek(f, loadedSize, SEEK_SET);
        fileSections[FUNCTIONS_NUM - 1].WriteFile(f, fileSize - loadedSize);
        loadedSize = GetTotalSize();
        if (loadedSize != fileSize)
            ntst_log_error("Can't load rest of the file. There is error somewhere.n");
        }
    else if (loadedSize > fileSize)
        ntst_log_error("Size of loaded data from save.dat greater than filesize. There is logical error somewhere.");
    }
