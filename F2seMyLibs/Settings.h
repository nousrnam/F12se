/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <string>

#define SECTION "Settings"
#define LOG_MODE "logmode"

struct Settings
    {
    Settings();

    std::string gamePath;
    std::string falloutExeOverride;
    std::string falloutCfgOverride;
    std::string falloutLngDirOverride;
    std::string lngFile;
    int brightness;
    int logMode;
    int imageResizeQuality;
    bool isFallout1;
    bool correctItemFID;
    bool correctAmmoType;
    bool correctAmmoQuantity;
    bool correctPerks;
    /**
     * @brief The game expects the player has the first trait slot set when trying to print the trait info on the character screen.
     * If the first slot is empty (value -1) it skips to perk printing without checking the second slot, so in the character screen the player won't see his only trait.
     * The game does take the trait into account when checking/applying related effects, just not showing up.
     */
    bool correctTraits;

    bool NeedLocalize() const {return !lngFile.empty();}
    bool Load(const std::string& inifile);
    void Save(const std::string& inifile);
    void TryToGuessLng();
    };

#endif // SETTINGS_H_
