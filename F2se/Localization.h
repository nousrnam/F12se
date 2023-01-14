/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <wx/window.h>

#include <string>
#include "IniFile.h"

class Localization
    {
public:
    bool LoadFile(const std::string& path);
    void LocalizeWindow(wxWindow *w, const char* sectionName) const;
    static void GenerateFileFromWindow(IniFile &f, wxWindow *w, const char* sectionName);
    IniFile file;
    };

#endif // LOCALIZATION_H
