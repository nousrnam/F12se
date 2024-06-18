/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#if defined(_MSC_VER)
#   define NOMINMAX // someone defines max
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "misc.h"
#include <assert.h>
#include <stdarg.h>
#include <algorithm>
#include "Defines.h"
#include "ntst_stringpp.hpp"
#include "ntst_loggingpp.hpp"

void ConvertStr(char *str, unsigned char *table)
    {
    size_t i = 0;
    while (str[i] != 0)
        {
        str[i] = table[(unsigned char)str[i]];
        i++;
        }
    }

void ConvertStr(std::string &str, unsigned char *table)
    {
    //TODO too dirty, need replace
    ConvertStr((char*)str.c_str(), table);
    }

int ClearMask(int val, int mask)
    {
    //return ~(~val | mask);
    return (val & ~mask);
    }

char NonPortableToLower(char in)
    {
    if (in >= 'A' && in <= 'Z')
        return in - ('A'-'a');
    return in;
    }

void StrToLowInplace(std::string& str)
    {
    for (std::string::iterator it = str.begin(); it != str.end(); ++it)
        *it = NonPortableToLower(*it);
    }

std::string StrToLow(const std::string& str)
    {
    std::string newString = str;
    StrToLowInplace(newString);
    return newString;
    }

std::string Trim(const std::string &S)
    {
    size_t b = 0;
    size_t e = S.size();
    while ((b < e) && (S[b] <= ' ')) b++;
    while ((b < e) && (S[e - 1] <= ' ')) e--;
    size_t substrSize = e - b;
    /*if (b == e)
        return std::string();*/
    const std::string tmpS = S.substr(b, substrSize);
    return tmpS;
    }

template<class ValType>
std::string IntToStr_(ValType val, const char* format)
    {
    char buf[21];
#if _MSC_VER
    _snprintf_s(buf, sizeof(buf), sizeof(buf), format, val);
#else
    snprintf(buf, sizeof(buf), format, val);
#endif
    return buf;
    }

std::string IntToHex(int val, unsigned len)
    {
    std::string format = "0x%0" + ntst::to_string(len) + "X";
    return IntToStr_(val, format.c_str());
    }

void savemem(void* addr,int size)
{
FILE* f;

f=fopen("savemem.txt","wb");
if (f==NULL) return;
fwrite(addr, size, 1, f);
fclose(f);
}

int AlignValUp(int valuealign, int alignmentfactor)
    {
    if (valuealign % alignmentfactor > 0)
        return valuealign - (valuealign % alignmentfactor) + alignmentfactor;
    else
        return valuealign;
    }

void LoadStrFromMem(std::string &str, void *addr, int size)
   {
   str.assign((char*)addr, size);
   }

void TrimTrailingSlashes(std::string &str)
	{
	size_t found = str.find_last_not_of("/\\");
	if (found != std::string::npos)
		str.resize(found+1);
	}

bool BlocksOverlap(unsigned offset1, unsigned size1, unsigned offset2, unsigned size2)
    {
    if (offset1 >= offset2)
        {
        if ((offset2 + size2 > offset1) && (size1 > 0))
            return true;
        }
    else
        {
        if ((offset1 + size1 > offset2) && (size2 > 0))
            return true;
        }
    return false;
    }

bool BlocksOverlap(unsigned offset1, unsigned size1, unsigned offset2, unsigned size2, unsigned *resultOffset, unsigned *resultSize)
    {
    *resultOffset = std::max(offset1, offset2);
    int size = std::min(offset1 + size1, offset2 + size2) - *resultOffset;
    if (size <= 0)
        return false;
    *resultSize = size;
    return true;
    }

bool BlocksOverlap2(unsigned offset1, unsigned size1, unsigned offset2, unsigned size2)
    {
    if (std::max(offset1, offset2) >=
        std::min(offset1 + size1,
            offset2 + size2))
        return false;
    return true;
    }

long int GetFileSize(const char* filename)
    {
    FILE* f = fopen(filename, "rb");
    if (f == NULL)
        return 0;
    fseek(f,0,SEEK_END);
    long int result = ftell(f);
    fclose(f);
    return result;
    }

long int GetFileSize(FILE *f)
    {
    long int curPos = ftell(f);
    fseek(f, 0, SEEK_END);
	long int fileSize = ftell(f);
	fseek(f, curPos, SEEK_SET);
    return fileSize;
    }

bool SplitPath(const std::string &path, std::string &dir, std::string &file)
    {
    size_t found = path.find_last_of("/\\");
    if (found == std::string::npos)
        return false;
    dir = path.substr(0, found);
    file = path.substr(found+1);
    return true;
    }

std::string ChangeFileExt(const std::string& filename, const std::string& newExt)
    {
    const size_t pos = filename.find_last_of('.');
    if (pos == std::string::npos)
        return filename + newExt;
    return filename.substr(0, pos) + newExt;
    }

void AddResultToResults(std::vector<FalloutDir>& results, std::string path, bool isF1)
    {
    TrimTrailingSlashes(path);
    results.push_back(FalloutDir(path, isF1));
    }

bool AddDirectoryIfFallout2(std::vector<FalloutDir>& results, const std::string& path)
    {
    if (DirLooksLikeFallout2(path))
        {
        AddResultToResults(results, path, false);
        ntst_log_info("Fallout2 was found in: " + path);
        return true;
        }
    return false;
    }

bool AddDirectoryIfFallout1(std::vector<FalloutDir>& results, const std::string& path)
    {
    if (DirLooksLikeFallout1(path))
        {
        AddResultToResults(results, path, true);
        ntst_log_info("Fallout1 was found in: " + path);
        return true;
        }
    return false;
    }

bool AddDirectoryIfFallout(std::vector<FalloutDir>& results, const std::string& path)
    {
    bool result = AddDirectoryIfFallout1(results, path) | AddDirectoryIfFallout2(results, path);
    if (!result)
        ntst_log_info("Fallout wasn't found in: " + path);
    return result;
    }

#ifdef _WIN32

#include <windows.h>

std::string GetApplicationDir()
    {
    std::string dir, file;
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, sizeof(path));
    if (SplitPath(path, dir, file))
        return dir;
    return std::string();
    }

bool FileExists(const char* path)
    {
    DWORD dwAttrib = GetFileAttributesA(path);
    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
           !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    }

bool DirectoryExists(const char* path)
    {
    DWORD dwAttrib = GetFileAttributesA(path);
    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
           (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    }

bool FileOrDirectoryExists(const char* path)
    {
    DWORD dwAttrib = GetFileAttributesA(path);
    return dwAttrib != INVALID_FILE_ATTRIBUTES;
    }

static void LogWinError(std::string msgStart, DWORD err, ntst_log_modes logMode)
    {
    char sysMsg[1024];
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL, err,
                    MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), sysMsg, sizeof(sysMsg), NULL);
    msgStart += sysMsg;
    size_t found = msgStart.find_last_not_of("\r\n");
    if (found != std::string::npos)
        msgStart.resize(found+1);
    switch (logMode)
        {
        case NTST_LOGM_WARNING: ntst_log_warning(msgStart);break;
        case NTST_LOGM_ERROR: ntst_log_error(msgStart);break;
        default: ntst_log_info(msgStart);break;
        }
    }

std::string ReadRegKey(const char* path, const char* key)
    {
    char buf[MAX_PATH];
    std::string result;
    HKEY hKey;
    DWORD dwSize = sizeof(buf);
    // tdmgcc does not know this constants.
    const unsigned KEY_WOW64_64KEY_ = 0x0100;
    const unsigned KEY_WOW64_32KEY_ = 0x0200;
    DWORD er = RegOpenKeyExA(HKEY_LOCAL_MACHINE, path, 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY_, &hKey);
    if (er == ERROR_FILE_NOT_FOUND)
        er = RegOpenKeyExA(HKEY_LOCAL_MACHINE, path, 0, KEY_QUERY_VALUE | KEY_WOW64_32KEY_, &hKey);
    if (er == ERROR_SUCCESS)
        {
        er = RegQueryValueExA(hKey, key, NULL, NULL, (LPBYTE)buf, &dwSize);
        if (er == ERROR_SUCCESS)
            result = buf;
        else
            LogWinError(std::string("Can't query reg value:") + key + ' ', er, NTST_LOGM_WARNING);
        RegCloseKey(hKey);
        }
    else
        LogWinError(std::string("Can't open reg key:") + path + ' ', er, NTST_LOGM_INFO);
    return result;
    }

std::vector<FalloutDir> GetFalloutDirs(const std::string &appDir, const std::string &ini)
    {
    char dir[MAX_PATH];
    std::vector<FalloutDir> results;
    GetPrivateProfileStringA(INI_SECTION, INI_FALLOUT_DIR, "", dir, MAX_PATH, ini.c_str());
    if (dir[0] != 0)
        {
        bool isF1 = (GetPrivateProfileIntA(INI_SECTION, INI_FALLOUT_NUM, 0, ini.c_str()) != 0);
        AddResultToResults(results, dir, isF1);
        }
    std::vector<FalloutDir> found = FindFalloutDirs(appDir);
    results.insert(results.end(), found.begin(), found.end());
    return results;
    }

std::vector<FalloutDir> FindFalloutDirs(const std::string &appDir)
    {
    std::vector<FalloutDir> results;
    AddDirectoryIfFallout(results, appDir);
    std::string curdir = appDir;
    std::string parentDir, unused;
    while (SplitPath(curdir, parentDir, unused))
        {
        AddDirectoryIfFallout(results, parentDir);
        curdir = parentDir;
        }
    AddDirectoryIfFallout2(results, ReadRegKey(F2_REG_KEY, "Path"));
    AddDirectoryIfFallout2(results, ReadRegKey("SOFTWARE\\GOG.com\\GOGFALLOUT2", "Path"));
    AddDirectoryIfFallout2(results, ReadRegKey("SOFTWARE\\GOG.com\\GAMES\\2", "Path"));
    AddDirectoryIfFallout2(results, ReadRegKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Steam App 38410", "InstallLocation"));
    AddDirectoryIfFallout1(results, ReadRegKey(F1_REG_KEY, "Path"));
    AddDirectoryIfFallout1(results, ReadRegKey("SOFTWARE\\GOG.com\\GOGFALLOUT", "Path"));
    AddDirectoryIfFallout1(results, ReadRegKey("SOFTWARE\\GOG.com\\GAMES\\1", "Path"));
    AddDirectoryIfFallout1(results, ReadRegKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Steam App 38400", "InstallLocation"));
    return results;
    }

std::vector<std::string> GetSubDirsVec(const std::string& dir)
    {
    WIN32_FIND_DATAA fd;
    std::vector<std::string> subDirs;

    HANDLE hFind = FindFirstFileA((dir + "\\*.*").c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE)
        {
        BOOL success;
        do
            {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                std::string tmpStr = fd.cFileName;
                if (tmpStr != "." && tmpStr != "..")
                    subDirs.push_back(tmpStr);
                }
            success = FindNextFileA(hFind, &fd);
            }
        while (success);
        FindClose(hFind);
        }
    else
        {
        DWORD er = GetLastError();
        ntst_log_modes logMode;
        if ((er == ERROR_PATH_NOT_FOUND) || (er == ERROR_FILE_NOT_FOUND))
            logMode = NTST_LOGM_INFO;
        else
            logMode = NTST_LOGM_WARNING;
        LogWinError(std::string("Can't open directory: ") + dir + " error: ", er, logMode);
        }
    return subDirs;
    }

std::vector<std::string> GetFiles(const std::string& dir, const std::string& mask)
    {
    WIN32_FIND_DATAA fd;
    std::vector<std::string> files;

    const std::string& fullpath = dir + '\\' + mask;
    HANDLE hFind = FindFirstFileA(fullpath.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE)
        {
        BOOL success;
        do
            {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                files.push_back(fd.cFileName);
            success = FindNextFileA(hFind, &fd);
            }
        while (success);
        FindClose(hFind);
        }
    else
        {
        DWORD er = GetLastError();
        ntst_log_modes logMode;
        if ((er == ERROR_PATH_NOT_FOUND) || (er == ERROR_FILE_NOT_FOUND))
            logMode = NTST_LOGM_INFO;
        else
            logMode = NTST_LOGM_WARNING;
        LogWinError(std::string("Can't get files by mask: ") + fullpath + " error: ", er, logMode);
        }
    return files;
    }

std::vector<std::string> GetSubDirsAndFiles(const std::string& dir, const std::string& mask)
    {
    WIN32_FIND_DATAA fd;
    std::vector<std::string> ret;
    const std::string& fullpath = dir + '\\' + mask;
    HANDLE hFind = FindFirstFileA(fullpath.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE)
        {
        BOOL success;
        do
            {
            ret.push_back(fd.cFileName);
            success = FindNextFileA(hFind, &fd);
            }
        while (success);
        FindClose(hFind);
        }
    else
        {
        DWORD er = GetLastError();
        LogWinError(std::string("Can't get dirs and files by mask: ") + fullpath + " error: ", er, er == ERROR_FILE_NOT_FOUND ? NTST_LOGM_INFO : NTST_LOGM_WARNING);
        }
    return ret;
    }

#else

#include <sys/stat.h>

std::string GetApplicationDir()
    {
    return ".";
    }

bool FileExists(const char* path)
    {
    struct stat sb;
    if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
        return true;
    return false;
    }

bool DirectoryExists(const char* path)
    {
    struct stat sb;
    if (stat(path, &sb) == 0 && !S_ISDIR(sb.st_mode))
        return true;
    return false;
    }

#endif

bool FileExists(const std::string &path)
    {
    return FileExists(path.c_str());
    }

bool DirectoryExists(const std::string &path)
    {
    return DirectoryExists(path.c_str());
    }

bool FileOrDirectoryExists(const std::string &path)
    {
    return FileOrDirectoryExists(path.c_str());
    }

bool DirLooksLikeFallout1(const std::string &path)
    {
    return FileExists(path + DIR_SEPARATOR F1_CFG) || FileExists(path + DIR_SEPARATOR F1_EXE);
    }

bool DirLooksLikeFallout2(const std::string &path)
    {
    return FileExists(path + DIR_SEPARATOR F2_CFG) || FileExists(path + DIR_SEPARATOR F2_EXE);
    }

#include "U8String.h"

bool StringIsValidUtf8(const std::string& str)
    {
    size_t strsize;
    size_t strlength;
    return u8sa_get_str_size_len(str.c_str(), &strsize, &strlength) == U8S_ERROR_SUCCESS;
    }

GetFileVersionResult GetFileVersion(const std::string& filename, unsigned& a, unsigned& b, unsigned& c, unsigned& d)
    {
    DWORD dwHandle, size = GetFileVersionInfoSizeA(filename.c_str(), &dwHandle);
    if (size == 0)
        {
        DWORD er = GetLastError();
        if (er == ERROR_FILE_NOT_FOUND)
            return GFVR_NO_FILE;
        LogWinError("GetFileVersionInfoSize failed(" + filename + ") reason:", er, NTST_LOGM_WARNING);
        return GFVR_ERROR;
        }
    std::vector<char> buf;
    buf.resize(size);
    if (!GetFileVersionInfoA(filename.c_str(), dwHandle, size, buf.data()))
        {
        DWORD er = GetLastError();
        LogWinError("GetFileVersionInfo failed(" + filename + ") reason:", er, NTST_LOGM_WARNING);
        return GFVR_ERROR;
        }
    VS_FIXEDFILEINFO* pvi;
    UINT len = sizeof(VS_FIXEDFILEINFO);
    if (!VerQueryValueA(buf.data(), "\\", (LPVOID*)&pvi, &len))
        {
        ntst_log_warning("VerQueryValue failed(" + filename + ")");
        return GFVR_ERROR;
        }
    a = pvi->dwFileVersionMS >> 16;
    b = pvi->dwFileVersionMS & 0xFFFF;
    c = pvi->dwFileVersionLS >> 16;
    d = pvi->dwFileVersionLS & 0xFFFF;
    return GFVR_OK;
    }

#ifndef NDEBUG

#include <assert.h>

class MiscTester
    {
public:
    MiscTester();
    };

MiscTester::MiscTester()
    {
    assert(Trim("a") == "a");
    assert(Trim(" a") == "a");
    assert(Trim("a ") == "a");
    assert(Trim(" a ") == "a");
    assert(Trim("") == "");

    assert(BlocksOverlap(1, 1, 2, 1) == false);
    assert(BlocksOverlap(2, 1, 1, 1) == false);
    assert(BlocksOverlap(1, 3, 2, 0) == false);
    assert(BlocksOverlap(2, 0, 1, 3) == false);

    assert(BlocksOverlap(1, 2, 2, 1) == true);
    assert(BlocksOverlap(2, 1, 1, 2) == true);
    assert(BlocksOverlap(1, 1, 1, 2) == true);
    assert(BlocksOverlap(1, 2, 1, 1) == true);
    assert(BlocksOverlap(1, 3, 2, 1) == true);
    assert(BlocksOverlap(2, 1, 1, 3) == true);
    assert(BlocksOverlap(1, 4, 2, 1) == true);
    assert(BlocksOverlap(2, 1, 1, 4) == true);

    assert(BlocksOverlap(1, 2, 1, 2) == true);

    unsigned offset;
    unsigned size;
    assert(BlocksOverlap(1, 1, 2, 1, &offset, &size) == false);
    assert(BlocksOverlap(2, 1, 1, 1, &offset, &size) == false);
    assert(BlocksOverlap(1, 3, 2, 0, &offset, &size) == false);
    assert(BlocksOverlap(2, 0, 1, 3, &offset, &size) == false);

    assert(BlocksOverlap(1, 2, 2, 1, &offset, &size) == true);
    assert(offset == 2);
    assert(size == 1);
    assert(BlocksOverlap(2, 1, 1, 2, &offset, &size) == true);
    assert(offset == 2);
    assert(size == 1);
    assert(BlocksOverlap(1, 1, 1, 2, &offset, &size) == true);
    assert(offset == 1);
    assert(size == 1);
    assert(BlocksOverlap(1, 2, 1, 1, &offset, &size) == true);
    assert(offset == 1);
    assert(size == 1);
    assert(BlocksOverlap(1, 3, 2, 1, &offset, &size) == true);
    assert(offset == 2);
    assert(size == 1);
    assert(BlocksOverlap(2, 1, 1, 3, &offset, &size) == true);
    assert(offset == 2);
    assert(size == 1);
    assert(BlocksOverlap(1, 4, 2, 1, &offset, &size) == true);
    assert(offset == 2);
    assert(size == 1);
    assert(BlocksOverlap(2, 1, 1, 4, &offset, &size) == true);
    assert(offset == 2);
    assert(size == 1);

    assert(BlocksOverlap(1, 2, 1, 2, &offset, &size) == true);
    assert(offset == 1);
    assert(size == 2);
    }

MiscTester miscTester;

#endif
