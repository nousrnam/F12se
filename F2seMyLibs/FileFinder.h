/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef FileFinderH
#define FileFinderH

#include <string>
#include <vector>

#include "commontypes.h"
#include "MemBuffer.h"

//Запись о файле внутри .dat файла
struct FileRec
    {
    std::string name;
    uint32 offset;
    uint32 size;
    };

class FilesSource;

class FileFinder
    {
public:
    FileFinder(bool isF1 = false) : curFileSrc(NULL), isFallout1DatFile(isF1) {}
    ~FileFinder() {ClearSources();}
    void AddSource(const std::string &path);
    void ClearSources();
    bool FindFile(const std::string &filename);
    MemBuffer GetFoundFile();
    MemBuffer GetFile(const std::string &filename);
    size_t GetSourcesQuantity() const {return locList.size();}
    void SetDatFileIsF1(bool isF1) {isFallout1DatFile = isF1;}
    void LogLocations() const;
private:
    FilesSource* curFileSrc;
    std::vector<FilesSource*> locList;//Список всех мест с файлами
    bool isFallout1DatFile;
    };
//---------------------------------------------------------------------------
#endif
