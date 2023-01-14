/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef miscH
#define miscH

#include <stdio.h>
#include <string>
#include <vector>
#include "commontypes.h"

void ConvertStr(char *str, unsigned char *table);
void ConvertStr(std::string &str, unsigned char *table);
void StrToLowInplace(std::string& str);
std::string StrToLow(const std::string& str);
std::string Trim(const std::string &S);
std::string IntToHex(int val, unsigned len);
void savemem(void* addr,int size);
int AlignValUp(int valuealign, int alignmentfactor);
void LoadStrFromMem(std::string &str, void *addr, int size);

void TrimTrailingSlashes(std::string &str);
bool BlocksOverlap(unsigned offset1, unsigned size1, unsigned offset2, unsigned size2);
bool BlocksOverlap(unsigned offset1, unsigned size1, unsigned offset2, unsigned size2, unsigned *resultOffset, unsigned *resultSize);
long int GetFileSize(const char* filename);
long int GetFileSize(FILE *f);

bool SplitPath(const std::string &path, std::string &dir, std::string &file);
std::string ChangeFileExt(const std::string& filename, const std::string& newExt);
std::string GetApplicationDir();

bool FileExists(const char* path);
bool FileExists(const std::string &path);
bool DirectoryExists(const char* path);
bool DirectoryExists(const std::string &path);
bool FileOrDirectoryExists(const char* path);
bool FileOrDirectoryExists(const std::string &path);
bool DirLooksLikeFallout1(const std::string &path);
bool DirLooksLikeFallout2(const std::string &path);

struct FalloutDir
    {
    FalloutDir(const std::string& _path, bool isF1) : path(_path), isFallout1(isF1) {;}
    std::string path;
    bool isFallout1;
    bool operator==(const FalloutDir& fd) const {return (isFallout1 == fd.isFallout1) && (path == fd.path);}
    };

bool AddDirectoryIfFallout(std::vector<FalloutDir>& results, const std::string& path);

std::vector<FalloutDir> GetFalloutDirs(const std::string& appDir, const std::string& ini);
std::vector<FalloutDir> FindFalloutDirs(const std::string& appDir);

std::vector<std::string> GetSubDirsVec(const std::string& dir);
std::vector<std::string> GetFiles(const std::string& dir, const std::string& mask);
std::vector<std::string> GetSubDirsAndFiles(const std::string& dir, const std::string& mask);

bool StringIsValidUtf8(const std::string& str);

enum GetFileVersionResult
    {
    GFVR_OK,
    GFVR_ERROR,
    GFVR_NO_FILE
    };
GetFileVersionResult GetFileVersion(const std::string& filename, unsigned& a, unsigned& b, unsigned& c, unsigned& d);

#endif
