/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifdef _WIN32

#include <shlobj.h>
#include <commdlg.h>

#include "Defines.h"
#include "WinFunc.h"
#include "misc.h"
#include "ntst_loggingpp.hpp"

ntst_static_assert(sizeof(LPARAM) >= sizeof(const char*), "!!!");
static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg, LPARAM, LPARAM param)
{
    const char* startPath = (const char*)param;
    if (uMsg == BFFM_INITIALIZED && startPath != NULL)
    {
        if (DirectoryExists(startPath))
            {
            SendMessage(hwnd, BFFM_SETSELECTION, TRUE, param);
            }
    }
    return 0;
}

bool BrowseDialog(const std::string& Title, HWND owner, std::string& path)
    {
	LPITEMIDLIST lpItemID;
	BROWSEINFOA BrowseInfo;
	char DisplayName[MAX_PATH];
	char tempPath[MAX_PATH];

	memset(&BrowseInfo, 0, sizeof(BrowseInfo));
	BrowseInfo.hwndOwner = owner;
	BrowseInfo.pszDisplayName = DisplayName;
	BrowseInfo.lpszTitle = Title.c_str();
    BrowseInfo.ulFlags = 0;
    BrowseInfo.lpfn = BrowseCallbackProc;
    BrowseInfo.lParam = (LPARAM)path.c_str();
	lpItemID = SHBrowseForFolderA(&BrowseInfo);
	if (lpItemID != NULL)
		{
		SHGetPathFromIDListA(lpItemID, tempPath);
		GlobalFree(lpItemID);
		path = tempPath;
		return true;
		}
	return false;
    }

void InitFileDialog(OPENFILENAMEA *of, const std::string &initialDir)
    {
    memset(of, 0, sizeof(*of));
    of->lStructSize = sizeof(*of);
    of->nMaxFile = MAX_PATH;
    of->lpstrFilter = "*.DAT\0*.DAT\0All Files\0*.*\0\0";
    of->Flags = OFN_CREATEPROMPT/* | OFN_DONTADDTORECENT*/;
    if (DirectoryExists(initialDir))
        of->lpstrInitialDir = initialDir.c_str();
    }

BOOL OpenFileDialog(HWND owner, const std::string &initialDir, const char* ibitialFilename, std::string &path)
    {
    OPENFILENAMEA of;
    char tempPath[MAX_PATH];
    strncpy(tempPath, ibitialFilename, sizeof(tempPath));

    InitFileDialog(&of, initialDir);
    of.hwndOwner = owner;
    of.lpstrFile = tempPath;
    const BOOL result = GetOpenFileNameA(&of);
    if (result)
        path = tempPath;
    return result;
    }

BOOL SaveFileDialog(HWND owner, const std::string &initialDir, const char* ibitialFilename, std::string &path)
    {
    OPENFILENAMEA of;
    char tempPath[MAX_PATH];
    strncpy(tempPath, ibitialFilename, sizeof(tempPath));

    InitFileDialog(&of, initialDir);
    of.hwndOwner = owner;
    of.lpstrFile = tempPath;
    const BOOL result = GetSaveFileNameA(&of);
    if (result)
        path = tempPath;
    return result;
    }

#endif // _WIN32
