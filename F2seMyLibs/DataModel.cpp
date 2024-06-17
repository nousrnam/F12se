/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include <stdio.h>
#include <assert.h>
#include <fstream>
#include <iostream>
#ifdef _WIN32
#include <Shlobj.h>
#endif

#include "DataModel.h"
#include "WinFunc.h"
#include "IniFile.h"
#include "misc.h"
#include "ntst_stringpp.hpp"
#include "ntst_loggingpp.hpp"

#define MSG_RUS_CHECK_LEN 4
static unsigned char monthsFromRussianVersion[12][MSG_RUS_CHECK_LEN] =
    {
    {0x7B, 0x9F, 0x8D, 0x82}, // {ЯНВ
    {0x7B, 0x94, 0x85, 0x82}, // {ФЕВ
    {0x7B, 0x8C, 0x80, 0x90}, // {МАР
    {0x7B, 0x80, 0x8F, 0x90}, // {АПР
    {0x7B, 0x8C, 0x80, 0x89}, // {МАЙ
    {0x7B, 0x88, 0x9E, 0x8D}, // {ИЮН
    {0x7B, 0x88, 0x9E, 0x8B}, // {ИЮЛ
    {0x7B, 0x80, 0x82, 0x83}, // {АВГ
    {0x7B, 0x91, 0x85, 0x8D}, // {СЕН
    {0x7B, 0x8E, 0x8A, 0x92}, // {ОКТ
    {0x7B, 0x8D, 0x8E, 0x9F}, // {НОЯ
    {0x7B, 0x84, 0x85, 0x8A}, // {ДЕК
    };

static unsigned char levCorpRussianTest[MSG_RUS_CHECK_LEN] = {0x7B, 0x93, 0xB0, 0xAE}; // {Уро

DataModel::DataModel() : view(NULL), appDir(GetApplicationDir()), perksDumpState(PERKS_DUMP_INVALID), gameCodePage(GCP_SYSTEM_DEFAULT)
    {
    iniFilePath = appDir + DIR_SEPARATOR INI_FILENAME;
    if (settings.Load(iniFilePath))
        ntst_log_set_mode(settings.logMode);
    else
        ntst_log_set_mode(0);
    ntst_log_info("Application directory is: " + appDir);
    ntst_log_info("Path to settings(" INI_FILENAME ") is: " + iniFilePath);
    }

DataModel::~DataModel()
    {
    settings.Save(iniFilePath);
    }

void DataModel::LoadSaveDat(const std::string& path)
    {
    bool result;
    ntst_log_info("Loading: " + path);
    if (IsFallout1())
        result = curSave.Load(path.c_str(), true, &vault13gam, &itemsPro, &aiTxt, 1);
    else
        result = curSave.Load(path.c_str(), false, &vault13gam, &itemsPro, &aiTxt, partyTxt.GetIniSectionCount());
    if (!result)
        ntst_log_error("Failed to load: " + path);
    }

void DataModel::LoadSaveDat(size_t index)
    {
    if (index < slots.size())
        LoadSaveDat(GetSaveDir() + DIR_SEPARATOR + slots[index].GetDir() + DIR_SEPARATOR SAVE_MAIN_FILE_NAME);
    }

static bool IsPathRelative(const std::string& path)
    {
    return path.size() < 2 || path[1] != ':';
    }

static std::string MakeFullFalloutPath(const std::string& path, const std::string& falloutDir)
    {
    //TODO I disliked something here
    // make full path if it isn't not. Because working directory can be changed.
    if (IsPathRelative(path))
        return falloutDir + DIR_SEPARATOR + path;
    return path;
    }

void AddPathToFF(std::string path, FileFinder &ff, const std::string& falloutDir)
    {
    ff.AddSource(MakeFullFalloutPath(path, falloutDir));
    }

void AddPathFromCfg(FileFinder &ff, IniFile& falloutcfg, const std::string& key, const std::string& defVal, const std::string& falloutDir)
    {
    AddPathToFF(falloutcfg.GetValue("system", key, defVal), ff, falloutDir);
    }

static bool GetSlotNumber(const std::string& str, unsigned& result)
    {
    if (str.size() > 4
            && (str[0] | 32) == 's'
            && (str[1] | 32) == 'l'
            && (str[2] | 32) == 'o'
            && (str[3] | 32) == 't')
        {
        return ntst_str10tou(str.c_str() + 4, &result) > 0;
        }
    return false;
    }

static bool MyStrComp(const std::string& l, const std::string& r)
    {
    unsigned ln, rn;
    if (GetSlotNumber(l, ln)
            && GetSlotNumber(r, rn))
        return ln < rn;
    return l < r;
    }

void DataModel::RefreshSaveSlots()
    {
    slots.clear();
    const std::string SaveDir = GetSaveDir();
    std::vector<std::string> subDirs = GetSubDirsVec(SaveDir);
    std::sort(subDirs.begin(), subDirs.end(), MyStrComp);
    std::string tmpStr;
    ntst_log_info("Number of subdirectories for \"" + SaveDir + "\" is " + ntst::to_string(subDirs.size()));
    for (size_t i = 0; i < subDirs.size(); ++i)
        {
        tmpStr = SaveDir + DIR_SEPARATOR + subDirs[i] + DIR_SEPARATOR SAVE_MAIN_FILE_NAME;
        FILE *f = fopen(tmpStr.c_str(), "rb");
        if (f != NULL)
            {
            fseek(f, 0x3D, SEEK_SET);
            tmpStr.clear();
            int chr = getc(f);
            while (chr != 0 && chr != EOF)
                {
                tmpStr += chr;
                chr = getc(f);
                }
            fclose(f);
            ntst_log_info("New save(" + tmpStr + ") was found in directory: " + subDirs[i]);
            slots.push_back(Slot(subDirs[i], tmpStr));
            }
        else
            {
            const int err = errno;
            std::string msg = "Can't open file: " + tmpStr + " error:" + strerror(err);
            if (err == ENOENT)
                ntst_log_info(msg);
            else
                ntst_log_warning(msg);
            }
        }
    if (view != NULL)
        view->RefreshSaveSlots(slots);
    }

struct StartDate
    {
    uint month;
    uint day;
    uint daysLeft;
    };

//число дней в фаллаутовских месяцах. А високосных лет в фаллауте нет.
static uint8 daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

#define F1_START_DAYS_LEFT ((unsigned)(daysInMonth[F1_START_MONTH] - F1_START_DAY + 1))
#define F2_START_DAYS_LEFT ((unsigned)(daysInMonth[F2_START_MONTH] - F2_START_DAY + 1))
StartDate GetStartDate(bool isFallout1)
    {
    StartDate result;
    if (isFallout1)
        {
        result.day = F1_START_DAY;
        result.daysLeft = F1_START_DAYS_LEFT;
        result.month = F1_START_MONTH;
        }
    else
        {
        result.day = F2_START_DAY;
        result.daysLeft = F2_START_DAYS_LEFT;
        result.month = F2_START_MONTH;
        }
    return result;
    }

void TicksToDate(uint32 Ticks, const StartDate& startDate, uint &year, uint &month, uint &day, uint &hour, uint &minute, uint &sec, uint &tick)
    {
    uint daysincurm;

    tick = Ticks % 10;
    Ticks = Ticks / 10;
    sec = Ticks % 60;
    Ticks = Ticks / 60;
    minute = Ticks % 60;
    Ticks = Ticks / 60;
    hour = Ticks % 24;
    Ticks = Ticks / 24;
    year = 0;
    month = startDate.month;
    day = startDate.day;
    daysincurm = startDate.daysLeft;
    if (Ticks > daysincurm) day = 1;
    while (Ticks>=daysincurm)
        {
        month++;
        if (month > 11)
            {
            month = 0;
            year++;
            }
        Ticks = Ticks - daysincurm;
        daysincurm = daysInMonth[month];
        }
    day = day + Ticks;
    }

uint32 DateToTicks(const StartDate& startDate, uint year, uint month, uint day, uint hour, uint minute, uint sec, uint tick)
    {
    uint daysincurm;
    uint32 Ticks;

    assert(month < NTST_ARRAY_LEN(daysInMonth));
    if (month >= NTST_ARRAY_LEN(daysInMonth))
        {
        return 0;
        }
    //Ticks:=Ticks+year*365*24*60*60*10;
    Ticks = tick+10*(sec+60*(minute+60*hour));
    if (year > 0)
        {
        Ticks = Ticks + (year-1)*365*24*60*60*10;//т.к. последний год не полный
        for (uint i = 0; i < month; i++)
            Ticks = Ticks + daysInMonth[i]*24*60*60*10;
        daysincurm = daysInMonth[month] - startDate.day + 1;
        for (uint i = startDate.month; i < 12; i++)//первый неполный год
            {
            Ticks = Ticks + daysincurm*24*60*60*10;
            daysincurm = daysInMonth[i];
            }
        Ticks = Ticks + (day-1)*24*60*60*10;
        }
    else//если начальный год еще не прошел
        {
        daysincurm = startDate.daysLeft;
        for (uint i = startDate.month; i < month; i++)
            {
            Ticks = Ticks + daysincurm*24*60*60*10;
            daysincurm = daysInMonth[i];
            }
        if (daysincurm == startDate.daysLeft)
            Ticks = Ticks + (day-startDate.day)*24*60*60*10;//если еще самый первый месяц
        else
            Ticks = Ticks + (day-1)*24*60*60*10;
        }
    return Ticks;
    }

void DataModel::TicksToDate(uint32 Ticks, uint &year, uint &month, uint &day, uint &hour, uint &minute, uint &sec, uint &tick) const
    {
    ::TicksToDate(Ticks, GetStartDate(IsFallout1()), year, month, day, hour, minute, sec, tick);
    }

uint32 DataModel::DateToTicks(uint year, uint month, uint day, uint hour, uint minute, uint sec, uint tick) const
    {
    return ::DateToTicks(GetStartDate(IsFallout1()), year, month, day, hour, minute, sec, tick);
    }

std::string DataModel::TicksToDateStr(uint32 ticks) const
    {
    char datebuf[20];
    uint year, month, day, hour, minute, sec, tick;
    TicksToDate(ticks, year, month, day, hour, minute, sec, tick);
    year += GetStartYear();
    month++;
#if _MSC_VER
    _snprintf_s(datebuf, sizeof(datebuf), sizeof(datebuf), "%04u-%02u-%02u %02u:%02u:%02u", year, month, day, hour, minute, sec);
#else
    snprintf(datebuf, sizeof(datebuf), "%04u-%02u-%02u %02u:%02u:%02u", year, month, day, hour, minute, sec);
#endif
    return datebuf;
    }

std::wstring DataModel::GetItemNameInCurSave(int offset) const
    {
    const MemBufferEX& buf = curSave.fileSections[6];
    int32 itemPID, itemType, msgLine;
    if (!buf.GetInv32(offset + INVEN_ITEM_PID, &itemPID))
        {
        std::wstring error = L"Can't read pid for item at offset " + ntst::to_wstring_hex_pref(offset);
        ntst_log_error_w(error);
        return error;
        }
    if (!itemsPro.Get4b(itemPID, 0x20, &itemType))
        {
        std::wstring error = L"Can't get type for item with pid " + ntst::to_wstring_hex_pref(itemPID);
        ntst_log_error_w(error);
        return error;
        }
    if (!itemsPro.Get4b(itemPID, PRO_DID, &msgLine))
        {
        std::wstring error = L"Can't get did for item with pid " + ntst::to_wstring_hex_pref(itemPID);
        ntst_log_error_w(error);
        return error;
        }
    return proItemMSG.GetText(msgLine) + L'(' + itemsTypes[itemType] + L") x " + ntst::to_wstring(buf.GetInv32(offset));
    }

static void LoadArrayFromMsg(std::wstring array[], size_t arraySize, const MsgFile& msg, int msgStart)
    {
    for (size_t i = 0; i < arraySize; i++)
        array[i] = msg.GetText(msgStart + i);
    }

static void LoadVectorFromMsg(std::vector<std::wstring>& vec, size_t vecSize, const MsgFile& msg, int msgStart)
    {
    vec.clear();
    vec.reserve(vecSize);
    for (size_t i = 0; i < vecSize; i++)
        vec.push_back(msg.GetText(msgStart + i).c_str());
    }

static void LoadVectorFromMsg(std::vector<std::wstring>& vec, size_t vecSize, const std::string& msgPath, int msgStart, StringConvFunc convFunc, FileFinder& ff)
    {
    MsgFile tmpMsg(ff.GetFile(msgPath), convFunc);
    LoadVectorFromMsg(vec, vecSize, tmpMsg, msgStart);
    }

static void LoadIniFileFromMemBuf(IniFile &ini, const MemBuffer &buf)
    {
    ini.Clear(); // При смене пути в настройках, новые файлы могут грузиться поверх старых.
    ini.Load(buf.GetAddress(), buf.GetSize());
    }

bool DataModel::InitAll(const Settings *s)
    {
    try
        {
        return InitAllDo(s);
        }
    catch (std::exception& e)
        {
        ntst_log_error(std::string("DataModel::InitAll failed. Exception: ") + e.what());
        }
    catch (...)
        {
        ntst_log_error("DataModel::InitAll failed. Unknown exception.");
        }
    return false;
    }

bool DataModel::InitAllDo(const Settings *s)
{
    if (s == NULL)
    {
        if (settings.gamePath.empty())
            return false;
    }
    else
        settings = *s;
    const std::string falloutDir = GetFalloutDir();
    const std::string falloutDirSep = falloutDir + DIR_SEPARATOR;
    ntst_log_info("Fallout" + ntst::to_string(GetFalloutNum()) + " directory is set to \"" + falloutDir + "\"");
    GetF2ExeVersionAndPerksDump();
    // Sfall stuff
    bool dataLoadOrderPatch = false;
    bool dataLoadOrderPatchDefault = false;
    unsigned a, b, c, d;
    const std::string sfallPath = falloutDirSep + "ddraw.dll";
    GetFileVersionResult gfvr = GetFileVersion(sfallPath, a, b, c, d);
    if (gfvr == GFVR_NO_FILE)
        ntst_log_info("sfall not found: " + sfallPath);
    else
    {
        if (gfvr == GFVR_ERROR && !settings.isFallout1)
        {
            ntst_log_warning("Can't get sfall version: \"" + sfallPath + "\". Assuming DataLoadOrderPatch is enabled by default.");
            dataLoadOrderPatchDefault = true;
        }
        else
        {
            ntst_log_info("Sfall version: " + ntst::to_string(a) + '.'
                          + ntst::to_string(b) + '.'
                          + ntst::to_string(c) + '.'
                          + ntst::to_string(d));
            // The setting is changed to be enabled by default in the new 4.1.9/3.8.19 (the reason is to prevent some unexpected issues)
            dataLoadOrderPatchDefault = ((a == 1) && (b >= 8)) // Fallout Nevada (Crazy Edition) 1.8.0.0
                    || ((a == 3) && (b > 8))
                    || ((a == 3) && (b == 8) && (c >= 19))
                    || ((a == 4) && (b > 1))
                    || ((a == 4) && (b == 1) && (c >= 9))
                    || (a > 4);
        }
    }
    IniFile ddraw_ini;
    std::vector<std::string> sfallExtraPatches;
    std::string patch000Mask = "patch%03d.dat";
    const bool useSfallSettings = (gfvr != GFVR_NO_FILE)
            && ddraw_ini.Load(falloutDirSep + "ddraw.ini");
    if (useSfallSettings)
    {
        ntst_log_info("ddraw.ini loaded. Maybe it has something interesting.");
        ddraw_ini.TryGetValue("Misc", "PatchFile", patch000Mask);
        dataLoadOrderPatch = ddraw_ini.GetValueBool("Misc", "DataLoadOrderPatch", dataLoadOrderPatchDefault);
    }
    if (dataLoadOrderPatch)
        ntst_log_info("Using new loading order.");
    else
        ntst_log_info("Using old loading order.");

    if (settings.falloutCfgOverride.empty())
    {
        std::string filename;
        if (IsFallout1())
            filename = F1_CFG;
        else
            filename = F2_CFG;
        if (useSfallSettings)
            ddraw_ini.TryGetValue("Misc", "ConfigFile", filename);
        falloutcfgFullPath = falloutDirSep + filename;
    }
    else
        falloutcfgFullPath = falloutDirSep + settings.falloutCfgOverride;
    ntst_log_info("Config file is set to " + falloutcfgFullPath);
    fcfg.Load(falloutcfgFullPath);
    if (fcfg.Empty())
        ntst_log_warning(falloutcfgFullPath + " is empty or does not exist.");

    if (settings.falloutLngDirOverride.empty())
    {
        ntst_log_info("Loading msg dir from: " + falloutcfgFullPath);
        actualLanguageDir = fcfg.GetValue("system", "language", "english");
    }
    else
    {
        actualLanguageDir = settings.falloutLngDirOverride;
        ntst_log_info("Using specified msg dir: " + actualLanguageDir);
    }
    std::string textPath = "Text\\" + actualLanguageDir + "\\game\\";
    ntst_log_info("Path to msg files is set to: " + textPath);

    /*
Searching order:
master_patches (if DataLoadOrderPatch in ddraw.ini)
critter_patches (if DataLoadOrderPatch in ddraw.ini)
mods dir = content of mods_order.txt, or everything sorted if [ExtraPatches] AutoloadingMods=1 and mods_order.txt doesn't exist
ddraw.ini [ExtraPatches] PatchFile99 - PatchFile0
ddraw.ini [ExtraPatches] AutoSearchPath
sfall.dat
patchXXX.dat OR ddraw.ini [Misc] PatchFile
critter_patches (if not DataLoadOrderPatch in ddraw.ini)
critter_dat
f2_res.ini [MAIN] f2_res_patches (if Fallout2)
f2_res.ini [MAIN] f2_res_dat (if Fallout2)
master_patches (if not DataLoadOrderPatch in ddraw.ini)
master_dat
game directory. Although should be current directory. But user can create link to executable and specify any directory as current.
    */
    fileFinder.ClearSources();
    fileFinder.SetDatFileIsF1(settings.isFallout1);
    if (dataLoadOrderPatch)
    {
        AddPathFromCfg(fileFinder, fcfg, "master_patches", "data", falloutDir);
        AddPathFromCfg(fileFinder, fcfg, "critter_patches", "data", falloutDir);
    }
    // I have no idea how to load correctly. My memory may be wrong...
    // But AutoSearchPath was lower priority than ExtraPatches and files were not sorted.(Ordered as Windows return them)
    // Now there is no AutoSearchPath in sfall and mods dir have higher priority with mods_order.txt file inside.
    // Also there was/is mods_order.ini that is different from mods_order.txt
    if (useSfallSettings)
    {
        const IniSection* patchesSec = ddraw_ini.FindSection("ExtraPatches");
        std::string autoSearchPath;
        bool autoLoadMods = false;
        if (patchesSec != NULL)
        {
            autoLoadMods = patchesSec->GetValueBool("AutoloadingMods");
            if (!autoLoadMods)
                autoSearchPath = patchesSec->GetValue("AutoSearchPath");
        }
        const bool newModsDirLogic = autoSearchPath.empty() || autoLoadMods;
        if (newModsDirLogic)
        { // Assume new mods dir with mods_order.txt
            const std::string modsFullPathStart = falloutDirSep + "mods";
            const std::string modsFullPathStartSep = modsFullPathStart + DIR_SEPARATOR;
            const std::string modsOrderFullPath = modsFullPathStartSep + "mods_order.txt";
            std::vector<std::string> modsFullPath;
            std::string patch;
            std::ifstream loadOrderFile(modsOrderFullPath, std::ios::in);
            if (loadOrderFile.is_open())
            {
                while (std::getline(loadOrderFile, patch))
                { // To disable a mod temporarily, you can comment it out by adding a ';' or '#' to the beginning of the line
                    if (patch.empty() || (patch.front() == ';')  || (patch.front() == '#'))
                        continue;
                    std::string fullpath = modsFullPathStartSep + patch;
                    if (FileOrDirectoryExists(fullpath))
                        modsFullPath.push_back(fullpath);
                }
            }
            else if (DirectoryExists(modsFullPathStart))
            {
                modsFullPath = GetSubDirsAndFiles(modsFullPathStart, "*.dat");
                std::sort(modsFullPath.begin(), modsFullPath.end());
                for (std::vector<std::string>::iterator it = modsFullPath.begin(); it != modsFullPath.end(); ++it)
                    *it = modsFullPathStartSep + *it;
            }
            for (std::vector<std::string>::const_reverse_iterator it = modsFullPath.rbegin(); it != modsFullPath.rend(); ++it)
                fileFinder.AddSource(*it);
        }
        if (patchesSec != NULL)
            for (int i = 99; i >= 0; --i)
            {
                std::string path = patchesSec->GetValue("PatchFile" + ntst::to_string(i));
                if (!path.empty())
                {
                    std::string fullpath = MakeFullFalloutPath(path, falloutDir);
                    if (FileOrDirectoryExists(fullpath))
                        fileFinder.AddSource(fullpath);
                }
            }
        if (!newModsDirLogic)
        { // Assume old AutoSearchPath without sorting
            const std::string autoModsFullPath = MakeFullFalloutPath(autoSearchPath, falloutDir);
            if (DirectoryExists(autoModsFullPath))
            {
                std::vector<std::string> mods = GetSubDirsAndFiles(autoModsFullPath, "*.dat");
                for (std::vector<std::string>::const_reverse_iterator it = mods.rbegin(); it != mods.rend(); ++it)
                    AddPathToFF(*it, fileFinder, autoModsFullPath);
            }
        }
    }
    std::vector<std::string> patches;
    for (int i = 0; i < 1000; ++i)
    {
        std::string patch000Path = falloutDirSep + ntst::string_printf(patch000Mask.c_str(), i);
        if (FileOrDirectoryExists(patch000Path))
            patches.push_back(patch000Path);
    }
    const std::string sfalldatPath = falloutDirSep + "sfall.dat";
    if (FileOrDirectoryExists(sfalldatPath))
        patches.push_back(sfalldatPath);
    for (std::vector<std::string>::const_reverse_iterator it = patches.rbegin(); it != patches.rend(); ++it)
    {
        AddPathToFF(*it, fileFinder, falloutDir);
    }
    if (!dataLoadOrderPatch)
        AddPathFromCfg(fileFinder, fcfg, "critter_patches", "data", falloutDir);
    AddPathFromCfg(fileFinder, fcfg, "critter_dat", "critter.dat", falloutDir);
    if (!IsFallout1())
    {
        IniFile f2_res;
        if (f2_res.Load(falloutDirSep + "f2_res.ini"))
        {
            ntst_log_info("f2_res.ini loaded. Maybe it has f2_res_dat and f2_res_patches.");
            std::string tmp;
            if (f2_res.TryGetValue("MAIN", "f2_res_patches", tmp))
                AddPathToFF(tmp, fileFinder, falloutDir);
            if (f2_res.TryGetValue("MAIN", "f2_res_dat", tmp))
                AddPathToFF(tmp, fileFinder, falloutDir);
        }
    }
    saveDirPath = fcfg.GetValue("system", "master_patches", "data") + DIR_SEPARATOR + "SAVEGAME";
    if (IsPathRelative(saveDirPath))
        saveDirPath = falloutDirSep + saveDirPath;
    if (!dataLoadOrderPatch)
        AddPathFromCfg(fileFinder, fcfg, "master_patches", "data", falloutDir);
    AddPathFromCfg(fileFinder, fcfg, "master_dat", "master.dat", falloutDir);
    // At the end should be "Current directory", but I can't get it. Because "current directory" can be not same as game directory.
    // I could add setting for that case, but it was never required.
    // I will add falloutDir just in case.
    fileFinder.AddSource(falloutDir);
    fileFinder.LogLocations();
    ntst_log_info("Loading items pro files.");
    itemsPro.LoadPro(PRO_TYPE_ITEMS, fileFinder);
    ntst_log_info(ntst::to_string(itemsPro.GetProQuantity()) + " items pro files loaded. Loading critters pro files.");
    crittersPro.LoadPro(PRO_TYPE_CRITTERS, fileFinder);
    ntst_log_info(ntst::to_string(crittersPro.GetProQuantity()) + " critters pro files loaded. Loading GVARS from vault13.gam");
    vault13gam.LoadGameVars(fileFinder.GetFile("data\\vault13.gam"));
    ntst_log_info(ntst::to_string(vault13gam.size()) + " GVARS loaded. Loading data\\ai.txt");
    LoadIniFileFromMemBuf(aiTxt, fileFinder.GetFile("data\\ai.txt"));
    if (!IsFallout1())
    {
        ntst_log_info("Loading data\\party.txt");
        LoadIniFileFromMemBuf(partyTxt, fileFinder.GetFile("data\\party.txt"));
        ntst_log_info("Party members count: " + ntst::to_string(partyTxt.GetIniSectionCount()) + " Loading data\\maps.txt");
        LoadIniFileFromMemBuf(mapsTxt, fileFinder.GetFile("data\\maps.txt"));
    }
    else
    {
        partyTxt.Clear();
        mapsTxt.Clear();
    }

    ntst_log_info("frmWorker.InitAll");
    frmWorker.SetGamma(settings.brightness * 0.179993 / 100 + 1.0);
    frmWorker.InitAll(fileFinder);

    if (!IsFallout1())
    {
        ntst_log_info("karmavarTxt.LoadFromMem");
        karmavarTxt.LoadFromMem(fileFinder.GetFile("data\\karmavar.txt"));
        ntst_log_info("holodiskTxt.LoadFromMem");
        holodiskTxt.LoadFromMem(fileFinder.GetFile("data\\holodisk.txt"));
        ntst_log_info("questsTxt.LoadFromMem");
        questsTxt.LoadFromMem(fileFinder.GetFile("data\\quests.txt"));
        ntst_log_info("cityTxt.Load");
        cityTxt.Load(fileFinder.GetFile("data\\city.txt"));
    }
    else
    {
        karmavarTxt.Clear();
        holodiskTxt.Clear();
        questsTxt.Clear();
        cityTxt.Clear();
    }

    ntst_log_info("scriptList.LoadFromMem");
    scriptList.LoadFromMem(fileFinder.GetFile("scripts\\scripts.lst"));

    gameCodePage = GCP_SYSTEM_DEFAULT;
    std::string lngDir = StrToLow(actualLanguageDir);
    if (lngDir == "cht")
    {
        gameCodePage = GCP_CH_TR;
        ntst_log_info("Looks like Chinese traditional version.");
    }
    else if (lngDir == "chs")
    {
        gameCodePage = GCP_CH_S;
        ntst_log_info("Looks like Chinese simplified version.");
    }
    else
    {
        MemBuffer memBuf = fileFinder.GetFile(textPath + "lsgame.msg");
        if (memBuf.GetSize() > MSG_RUS_CHECK_LEN)
        {
            const char* cur = (const char*)memBuf.GetAddress();
            const char* end = cur + memBuf.GetSize() - MSG_RUS_CHECK_LEN + 1;
            bool isRussianVersionNeedsCodePageConversion = false;
            unsigned curMonth = 0;
            while (cur != end)
            {
                if (memcmp(cur, monthsFromRussianVersion[curMonth], MSG_RUS_CHECK_LEN) == 0)
                {
                    ++curMonth;
                    if (curMonth >= 12)
                    {
                        isRussianVersionNeedsCodePageConversion = true;
                        break;
                    }
                }
                ++cur;
            }
            if (isRussianVersionNeedsCodePageConversion)
            {
                bool fargus = true;
                while (cur != end)
                {
                    if (memcmp(cur, levCorpRussianTest, MSG_RUS_CHECK_LEN) == 0)
                    {
                        fargus = false;
                        break;
                    }
                    ++cur;
                }
                if (fargus)
                {
                    gameCodePage = GCP_RUS_866;
                    ntst_log_info("Russian version from Fargus detected.");
                }
                else
                {
                    gameCodePage = GCP_RUS_LEVCORP;
                    ntst_log_info("Russian version from LevCorp detected.");
                }
            }
        }
    }
    StringConvFunc convFunc = GetToWideConvFunc(gameCodePage);

    LoadVectorFromMsg(stats, STATS_CNT, textPath + "stat.msg", 100, convFunc, fileFinder);
    LoadVectorFromMsg(skills, SKILLS_CNT, textPath + "skill.msg", 100, convFunc, fileFinder);
    LoadVectorFromMsg(traits, TRAITS_CNT, textPath + "trait.msg", 100, convFunc, fileFinder);

    editorMSG.Load(fileFinder.GetFile(textPath + "editor.msg"), convFunc);
    LoadArrayFromMsg(months, NTST_ARRAY_LEN(months), editorMSG, 500);

    protoMSG.Load(fileFinder.GetFile(textPath + "proto.msg"), convFunc);
    LoadArrayFromMsg(itemsTypes, NTST_ARRAY_LEN(itemsTypes), protoMSG, 150);
    if (IsFallout1())
    {
        LoadVectorFromMsg(perks, F1_PERKS_CNT, textPath + "perk.msg", 101, convFunc, fileFinder);
        LoadVectorFromMsg(killTypes, F1_KILL_TYPES_CNT, protoMSG, F1_KILL_TYPES_MSG_START);
    }
    else
    {
        LoadVectorFromMsg(perks, F2_PERKS_CNT, textPath + "perk.msg", 101, convFunc, fileFinder);
        LoadVectorFromMsg(killTypes, F2_KILL_TYPES_CNT, protoMSG, F2_KILL_TYPES_MSG_START);
    }

    proItemMSG.Load(fileFinder.GetFile(textPath + "pro_item.msg"), convFunc);
    proCritterMSG.Load(fileFinder.GetFile(textPath + "pro_crit.msg"), convFunc);
    mapMSG.Load(fileFinder.GetFile(textPath + "map.msg"), convFunc);
    worldmapMSG.Load(fileFinder.GetFile(textPath + "worldmap.msg"), convFunc);
    if (!IsFallout1())
    {
        questsMSG.Load(fileFinder.GetFile(textPath + "quests.msg"), convFunc);
    }
    else
    {
        questsMSG.Clear();
    }
    pipboyMSG.Load(fileFinder.GetFile(textPath + "pipboy.msg"), convFunc);

    RefreshSaveSlots();
    if (view != NULL)
        view->UpdateViewAfterInitAll(*this);
#ifdef _WIN32
    char progf[MAX_PATH];
    if (SHGetFolderPathA(NULL, CSIDL_PROGRAM_FILES, NULL, 0, progf) == S_OK)
    {
        std::string progfStr = progf;
        ntst_log_info("Check if game is installed in Program Files to give warning: " + progfStr);
        if (strncmp(progfStr.c_str(), falloutDir.c_str(), progfStr.size()) == 0)
            ntst_log_warning("Seems like game is installed in Program Files. Windows might prevent changes to files there! Don't ask why you don't see changes in game.");
    }
    else
        ntst_log_warning("Can't get Program Files path to check if game installed there.");
#endif
    // Users sometime set path to "gamedir\data\savegame" or "gamedir\data\savegame\slot01" instead of "gamedir"
    if (!IsValid())
    {
        ntst_log_warning("Data model is invalid. Maybe wrong path? Will try to find correct path.");
        std::vector<FalloutDir> falloutDir;
        std::string curdir = GetFalloutDir();
        TrimTrailingSlashes(curdir);
        std::string parentDir, unused;
        while (SplitPath(curdir, parentDir, unused))
        {
            if (AddDirectoryIfFallout(falloutDir, parentDir))
                break;
            curdir = parentDir;
        }
        if (falloutDir.empty())
            ntst_log_warning("Correct path not found.");
        else
        {
            FalloutDir backup(settings.gamePath, settings.isFallout1);
            settings.gamePath = falloutDir.front().path;
            settings.isFallout1 = falloutDir.front().isFallout1;
            InitAllDo(NULL);
            if (IsValid())
                ntst_log_warning("Path was changed to: " + settings.gamePath);
            else
            {
                // Failed. Restoring settings.
                settings.gamePath = backup.path;
                settings.isFallout1 = backup.isFallout1;
            }
        }
    }
    return true;
}

/*
I believe I've figured out where Perks and Traits are stored (though I haven't tested these in-game, so it might all be an elaborate mistake):

Traits: 0x10c184 (Just the location- I haven't looked at them any yet)
Perks: 0x1083cc (15A3A0 in mapper2.exe I believe)

 Perks seem to have the following format (Up to Mutate! at least, after that it changes, and then seems to fall apart).
 Each perk appears to have the following format:

 [00] - ? (Always 0000 0000)
 [04] - ? (Always 0000 0000)
 [08] - ID (Appears to be ID in PERK.MSG -29)
 [0C] - Number of Ranks available
 [10] - Required Level
 [14] - Stat Modified (entry in STAT.msg - 100) (FFFF FFFF = None)
 [18] - Amount Stat Modified
 [1C] - Skill 1 (SKILL.MSG - 100) (FFFF FFFF = None)
 [20] - Amount for (in % points) Skill 1
 [24] - Type*
 [28] - Skill 2 (SKILL.MSG - 100) (FFFF FFFF = None)
 [2C] - Amount (in % points) for Skill 2
 [30] - Req. STR
 [34] - Req. PER
 [38] - Req. END
 [3C] - Req. CHA
 [40] - Req. INT
 [44] - Req. AGI
 [48] - Req. LUC

 *- is 0000 0000 unless there are two skill entries (In these cases, it is a requirement I think) , in which case it looks like the entry is 0200 0000 if they are requirements and 0100 0000 if they are bonus (points the character gains). (For the addiction perks, Ranks looks like it is always FFFF FFFF, and the Stat Req. Fields look like they are rather the change applied FEFF FFFF for -2 to the stat, for example.)

For Fallout record size is 0x3C instead of 0x4C
There aren't [00] [04] [08] [??]
Match [0C] [10] [14] [18] [1C]
*/
void DataModel::GetF2ExeVersionAndPerksDump()
    {
    std::string fexePath = GetFalloutDir();
    if (settings.falloutExeOverride.empty())
        {
        if (IsFallout1())
            fexePath += DIR_SEPARATOR "falloutw.exe";
        else
            fexePath += DIR_SEPARATOR "fallout2.exe";
        }
    else
        fexePath += DIR_SEPARATOR + settings.falloutExeOverride;
    ntst_log_info("Getting executable version: " + fexePath);
    FILE *f = fopen(fexePath.c_str(), "rb");
    if (f != NULL)
        {
        size_t perksOffset;
        int32 fileDate = 0; // if fread fails fileDate will be initialized
        fseek(f, 0x88, SEEK_SET);
        fread(&fileDate, sizeof(int32), 1, f);
        switch (fileDate)
            {
            case 0x3505F1DC: //latest Fallout version
                ntst_log_info("falloutw.exe version is 1.2.");
                perksOffset = 0xE781C;
                perksDumpState = PERKS_DUMP_F1;
                break;
            case 0x362EF769: //1.0 perksOffset = 0x1075FC
                ntst_log_info("fallout2.exe version is 1.00.");
                perksOffset = 0x1075FC;
                perksDumpState = PERKS_DUMP_F2;
                break;
            case 0x365B5B10: //1.02 LevCorp perksOffset = 0x1083BC
                ntst_log_info("fallout2.exe version is 1.02.");
                perksOffset = 0x1083BC;
                perksDumpState = PERKS_DUMP_F2;
                break;
            case 0x3671BF22: //1.02d perksOffset = 0x1083CC
                ntst_log_info("fallout2.exe version is 1.02d.");
                perksOffset = 0x1083CC;
                perksDumpState = PERKS_DUMP_F2;
                break;
            case 0x37562E38:
                ntst_log_info("fallout2.exe version is Chinese.");
                perksOffset = 0x1151BC;
                perksDumpState = PERKS_DUMP_F2;
                break;
            default:
                ntst_log_info("Version is unknown. Automatic correction for perks is disabled.");
                fclose(f);
                return;
            }
        void* buf; size_t bufSize;
        if (perksDumpState == PERKS_DUMP_F1)
            {
            buf = perksFromExeDump.f1;
            bufSize = sizeof(perksFromExeDump.f1);
            }
        else
            {
            buf = perksFromExeDump.f2;
            bufSize = sizeof(perksFromExeDump.f2);
            }
        if (fseek(f, perksOffset, SEEK_SET) == 0 && fread(buf, bufSize, 1, f) == 1)
            {
            ntst_log_info("Perks from executable are successfully dumped.");
            }
        else
            {
            perksDumpState = PERKS_DUMP_INVALID;
            ntst_log_info("Somehow failed to dump perks. Automatic correction for perks is disabled.");
            }
        fclose(f);
        return;
        }
    perksDumpState = PERKS_DUMP_INVALID;
    ntst_log_info(std::string("Can't open file.(") + strerror(errno) + ") Automatic correction for perks is disabled.");
    }

void DataModel::ModPerkAndApplyCorrectionIfNeed(unsigned perkNum, int32 newval)
    {
    unsigned offset = PERKS_OFFSET + perkNum * sizeof(int32);
    const int oldval = curSave.fileSections[11].GetInv32(offset);
    if (oldval == newval)
        return;
    curSave.fileSections[11].SetInv32(offset, newval);
    int32 statNum, statMod;
    switch (perksDumpState)
        {
        case PERKS_DUMP_F1:
            statNum = perksFromExeDump.f1[perkNum][2];
            statMod = perksFromExeDump.f1[perkNum][3];
            break;
        case PERKS_DUMP_F2:
            statNum = perksFromExeDump.f2[perkNum][5];
            statMod = perksFromExeDump.f2[perkNum][6];
            break;
        default:
            statNum = -1;
            break;
        }
    if (statNum == -1)
        return;
    const unsigned uStatNum = statNum;
    if (uStatNum >= stats.size())
        {
        ntst_log_error("Perks table from executable somehow is invalid. statNum=" + ntst::to_string(uStatNum) + " >= stats.size()=" + ntst::to_string(stats.size()));
        return;
        }
    int statoffset = BONUS_STAT_OFFSET + statNum * sizeof(int32);
    int32 oldStatVal = curSave.fileSections[7].GetInv32(statoffset);
    int32 newStatVal = oldStatVal + statMod * (newval - oldval);
    ntst_log_info_w(L"Perk \"" + perks[perkNum] + L"\" changes \"" + stats[statNum] + L"\" by " + ntst::to_wstring(statMod) +
    L" per perk level. Changing stat from " + ntst::to_wstring(oldStatVal) + L" to " + ntst::to_wstring(newStatVal));
    curSave.fileSections[7].SetInv32(statoffset, newStatVal);
    }
