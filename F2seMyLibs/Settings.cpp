/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include "Settings.h"
#include "IniFile.h"
#include "ntst_loggingpp.hpp"

#define GAME_PATH "path"
#define IS_F1 "is_f1"
#define MYINI_FALLOUT_LANGUAGE_DIR "fallout_lng_dir"
#define MYINI_FALLOUT_EXE_FILENAME "fallout_exe"
#define MYINI_FALLOUT_CFG_FILENAME "fallout_cfg"
#define COR_ITEM_FID "c_ifid"
#define COR_AMMO_T "c_ammot"
#define COR_AMMO_Q "c_ammoq"
#define COR_PERKS "c_perks"
#define COR_TRAITS "c_traits"
#define BRIGHTNESS "brightness"
#define LOCALIZE_FILEPATH "lng_file"
#define IMG_RESIZE_QUALITY "image_resize_quality"

#define IMG_RESIZE_QUALITY_DEFAULT_VAL 1

#ifdef _WIN32

#include <vector>
#include <windows.h>
#include <stdlib.h>
#include "Defines.h"
#include "misc.h"
#include "ntst_stringpp.hpp"

std::string GetDefaultLngFilename()
    {
    std::string lngDir = GetApplicationDir() + DIR_SEPARATOR LANGUAGE_DIR;
    std::vector<std::string> lngFiles = GetFiles(lngDir, LANGUAGE_FILE_MASK);
    LANGID defaultLanguage = GetSystemDefaultLangID();
    ntst_log_info("DefaultLangID=" + ntst::to_string_hex_pref(defaultLanguage));
    for (std::vector<std::string>::const_iterator filenameIt = lngFiles.begin(); filenameIt != lngFiles.end(); ++filenameIt)
        {
        IniFile ini;
        ini.Load(lngDir + DIR_SEPARATOR + *filenameIt);
        const IniSection& sec = ini.GetDefaultSectionRef();
        for (IniSection::const_iterator keyvalue = sec.begin(); keyvalue != sec.end(); ++keyvalue)
            {
            long int val = strtol(keyvalue->second.c_str(), NULL, 0);
            if (val == defaultLanguage)
                {
                ntst_log_info(*filenameIt + " matches DefaultLangID.");
                return *filenameIt;
                }
            }
        }
    return std::string();
    }

#else

std::string GetDefaultLngFilename()
    {
    return std::string();
    }

#endif // _WIN32

Settings::Settings() :
        brightness(0),
        logMode(NTST_LOGM_ALL),
        imageResizeQuality(IMG_RESIZE_QUALITY_DEFAULT_VAL),
        isFallout1(false),
        correctItemFID(true),
        correctAmmoType(true),
        correctAmmoQuantity(true),
        correctPerks(true)
        {
        }

bool Settings::Load(const std::string& inifile)
    {
    IniFile iniFile;
    if (!iniFile.Load(inifile))
        return false;
    const IniSection* sec = iniFile.FindSection(SECTION);
    if (sec == NULL)
        return false;
    const std::string* str = sec->FindValue(GAME_PATH);
    if (str == NULL)
        return false;
    gamePath = *str;
    str = sec->FindValue(IS_F1);
    if (str == NULL)
        return false;
    if (*str == "0")
        isFallout1 = false;
    else
        isFallout1 = true;
    falloutExeOverride = sec->GetValue(MYINI_FALLOUT_EXE_FILENAME);
    falloutCfgOverride = sec->GetValue(MYINI_FALLOUT_CFG_FILENAME);
    falloutLngDirOverride = sec->GetValue(MYINI_FALLOUT_LANGUAGE_DIR);
    correctItemFID = sec->GetValueBool(COR_ITEM_FID, true);
    correctAmmoType = sec->GetValueBool(COR_AMMO_T, true);
    correctAmmoQuantity = sec->GetValueBool(COR_AMMO_Q, true);
    correctPerks = sec->GetValueBool(COR_PERKS, true);
    correctTraits = sec->GetValueBool(COR_TRAITS, true);
    std::string br = sec->GetValue(BRIGHTNESS, "0");
    brightness = atoi(br.c_str());
    imageResizeQuality = sec->GetValueInt(IMG_RESIZE_QUALITY, IMG_RESIZE_QUALITY_DEFAULT_VAL);
    lngFile = sec->GetValue(LOCALIZE_FILEPATH);
    const std::string *lm = sec->FindValue(LOG_MODE);
    if (lm != NULL)
        logMode = atoi(lm->c_str());
    return true;
    }

void Settings::Save(const std::string& inifile)
    {
    IniFile iniFile;
    iniFile.Load(inifile);
    IniSection* sec = iniFile.FindSectionOrCreate(SECTION);
    if (sec == NULL)
        return;
    sec->SetValue(GAME_PATH, gamePath);
    sec->SetValue(IS_F1, isFallout1);
    sec->SetValue(MYINI_FALLOUT_EXE_FILENAME, falloutExeOverride);
    sec->SetValue(MYINI_FALLOUT_CFG_FILENAME, falloutCfgOverride);
    sec->SetValue(MYINI_FALLOUT_LANGUAGE_DIR, falloutLngDirOverride);
    sec->SetValue(COR_ITEM_FID, correctItemFID);
    sec->SetValue(COR_AMMO_T, correctAmmoType);
    sec->SetValue(COR_AMMO_Q, correctAmmoQuantity);
    sec->SetValue(COR_PERKS, correctPerks);
    sec->SetValue(COR_TRAITS, correctTraits);
    sec->SetValue(BRIGHTNESS, brightness);
    sec->SetValue(IMG_RESIZE_QUALITY, imageResizeQuality);
    sec->SetValue(LOCALIZE_FILEPATH, lngFile);
    sec->SetValue(LOG_MODE, logMode);
    iniFile.Save(inifile);
    }

void Settings::TryToGuessLng()
    {
    lngFile = GetDefaultLngFilename();
    }
