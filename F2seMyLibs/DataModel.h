/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <string>
#include <vector>
#include "commontypes.h"
#include "Defines.h"
#include "fallout.h"
#include "CityTxt.h"
#include "SaveDat.h"
#include "misc.h"
#include "FileFinder.h"
#include "PRO.h"
#include "GAM.h"
#include "FRM.h"
#include "Settings.h"
#include "IniFile.h"

#define SAVE_MAIN_FILE_NAME "SAVE.DAT"

class DataModelViewer;

class DataModel
    {
public:
    DataModel();
    ~DataModel();

    std::string WideToGameString(const std::wstring& wstr) const
        {
        switch(gameCodePage)
            {
            case GCP_RUS_866: return WideStringToRussianDos(wstr.c_str(), wstr.size());
            case GCP_RUS_LEVCORP: return WideStringToRussianLevCorp(wstr.c_str(), wstr.size());
            case GCP_CH_TR: return WideStringToChineseTraditional(wstr.c_str(), wstr.size());
            case GCP_CH_S: return WideStringToChineseSimplified(wstr.c_str(), wstr.size());
            default: return WideStringToLocal(wstr.c_str(), wstr.size());
            }
        }
    std::wstring GameStringToWide(const std::string& str) const
        {
        return GetToWideConvFunc(gameCodePage)(str.c_str(), str.size());
        }
    void SetFalloutDir(const std::string& dir);
    void LoadSaveDat(const std::string& path);
    void LoadSaveDat(size_t index);
    bool InitAll(const Settings *s);
    void SetView(DataModelViewer* modelViewer) {view = modelViewer;}
    void RefreshSaveSlots();
    bool IsFallout1() const {return settings.isFallout1;}
    int GetStartYear() const {return settings.isFallout1 ? F1_START_YEAR : F2_START_YEAR;}
    unsigned GetKillTypesCount() const {return settings.isFallout1 ? F1_KILL_TYPES_CNT : F2_KILL_TYPES_CNT;}
    unsigned GetPerksCount() const {return settings.isFallout1 ? F1_PERKS_CNT : F2_PERKS_CNT;}
    bool GetGamFile(const std::string& path, GamFile& gam)
        {
        if (!fileFinder.FindFile(path))
            return false;
        gam.LoadGameVars(fileFinder.GetFoundFile());
        return true;
        }

    void TicksToDate(uint32 Ticks, uint &year, uint &month, uint &day, uint &hour, uint &minute, uint &sec, uint &tick) const;
    uint32 DateToTicks(uint year, uint month, uint day, uint hour, uint minute, uint sec, uint tick) const;
    std::string TicksToDateStr(uint32 ticks) const;
    std::wstring GetItemNameInCurSave(int offset) const;

    const PaletteRGB& GetMainPalette() const {return frmWorker.palMain;}

    class Slot
        {
    public:
        Slot(const std::string& _dir, const std::string& _name) : dir(_dir), name(_name) {;}
        std::string GetNameToShow() const {return name.empty() ? dir : name + " (" + dir + ')';}
        std::string GetName() const {return name;}
        std::string GetDir() const {return dir;}
    private:
        std::string dir; // Sub dir name (SLOT01, SLOT02, etc)
        std::string name; // Name in header
        };

    SaveDatFile curSave;

    std::vector<std::wstring> stats;
    std::vector<std::wstring> skills;
    std::vector<std::wstring> killTypes;
    std::wstring itemsTypes[ITEMS_TYPES_CNT];
    std::vector<std::wstring> perks;
    std::vector<std::wstring> traits;
    std::wstring months[MONTHS_CNT];

    GamFile vault13gam;
    DataTxtFile<4> karmavarTxt;
    DataTxtFile<3> holodiskTxt;

    MsgFile editorMSG;
    MsgFile protoMSG;
    MsgFile proItemMSG;
    MsgFile proCritterMSG;
    MsgFile mapMSG;
    MsgFile worldmapMSG;
    MsgFile questsMSG;
    MsgFile pipboyMSG;

    CityTxt cityTxt;
    QuestsTxt questsTxt;

    IniFile mapsTxt;

    ProWorker itemsPro;
    ProWorker crittersPro;
    FrmWorker frmWorker;
    FileFinder fileFinder;

    ScriptList scriptList;

    Settings settings;

    std::string GetSaveDir() const {return saveDirPath;}
    void ModPerkAndApplyCorrectionIfNeed(unsigned perkNum, int32 newval);
    std::string GetDirLng() const {return appDir + DIR_SEPARATOR LANGUAGE_DIR;}
    std::string GetFalloutDir() const {return settings.gamePath;}
    const std::string& GetSettingsPath() const {return iniFilePath;}
private:
    bool InitAllDo(const Settings *s);
    std::string actualLanguageDir;
    std::string saveDirPath;
    DataModelViewer* view;
    IniFile fcfg;
    int GetFalloutNum() const {return settings.isFallout1 ? 1 : 2;}
    bool IsValid() const {return vault13gam.size() > 0 && itemsPro.GetProQuantity() > 0;}
    std::string appDir;
    std::string iniFilePath;
    std::string falloutcfgFullPath;

    IniFile aiTxt;
    IniFile partyTxt;

    void GetF2ExeVersionAndPerksDump();
    enum PerksDumpState
        {
        PERKS_DUMP_INVALID,
        PERKS_DUMP_F1,
        PERKS_DUMP_F2
        };
    PerksDumpState perksDumpState;
    union
        {
        int32 f1[F1_PERKS_CNT][15];
        int32 f2[F2_PERKS_CNT][19];
        } perksFromExeDump;

    std::vector<Slot> slots;
    GameCodePage gameCodePage;
    };

class DataModelViewer
    {
public:
    virtual ~DataModelViewer() {}
    virtual void RefreshSaveSlots(const std::vector<DataModel::Slot>& slots) = 0;
    virtual void UpdateViewAfterInitAll(DataModel& model) = 0;
    };

#endif // DATAMODEL_H
