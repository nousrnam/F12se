/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef WINFUNC_H
#define WINFUNC_H

#ifdef _WIN32

#include <string>
#include <vector>
#include <windows.h>

bool BrowseDialog(const std::string &Title, HWND owner, std::string &path);
BOOL OpenFileDialog(HWND owner, const std::string &initialDir, const char* ibitialFilename, std::string &path);
BOOL SaveFileDialog(HWND owner, const std::string &initialDir, const char* ibitialFilename, std::string &path);

#endif // _WIN32

#endif // WINFUNC_H
