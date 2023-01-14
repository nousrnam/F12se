/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef PROH
#define PROH

#include <string>
#include <vector>

#include "VecOfStr.h"
#include "MemBuffer.h"
#include "commontypes.h"

class FileFinder;

enum ProType
    {
    PRO_TYPE_ITEMS = 0,
    PRO_TYPE_CRITTERS = 1,
    PRO_TYPE_SCENERY = 2,
    PRO_TYPE_WALLS = 3,
    PRO_TYPE_TILES = 4,
    PRO_TYPE_MISC = 5
    };

#define PRO_OFFSET_PID 0

#define PRO_FILE_MINIMUM_POSSIBLLE_SIZE 28

class ProWorker
    {
public:
    ProWorker() : ignoreByMinimumSize(true), minimumCorrectSize(PRO_FILE_MINIMUM_POSSIBLLE_SIZE) {;}

    void LoadPro(ProType type, FileFinder& ff);
    void Clear() {proFilesList.clear(); proFiles.clear(); indicesOfSelectedProFiles.clear();}

    bool  Get4b(int32 pid, size_t offset, int32 *val) const;
    int32 Get4b(int32 pid, size_t offset) const;
    int8  Get1b(int32 pid, size_t offset) const;
    bool  Get4bByIndex(size_t ind, size_t offset, int32 *val) const;
    int32 Get4bByIndex(size_t ind, size_t offset) const;
    int8  Get1bByIndex(size_t ind, size_t offset) const;
    void  Set4b(int32 pid, size_t offset, int32 val);
    void  Set1b(int32 pid, size_t offset, int8 val);
    void  Set4bByIndex(size_t ind, size_t offset, int32 val);
    void  Set1bByIndex(size_t ind, size_t offset, int8 val);

    //void SaveInFile(int32 pid, const std::string& filename);

    void SelectAll();
    void SelectEqual(unsigned offset, int32 val);
    /*void Select(size_t offset, int32 val, char logic);
    void selectEqual(size_t offset, int32 val);
    void selectNEqual(size_t offset, int32 val);
    void selectGreater(size_t offset, int32 val);
    void selectNGreater(size_t offset, int32 val);
    void selectLess(size_t offset, int32 val);
    void selectNLess(size_t offset, int32 val);*/

    size_t GetProQuantity() const {return proFiles.size();}
    size_t GetSelectedProQuantity() const {return indicesOfSelectedProFiles.size();}
    unsigned GetSelectedIndex(unsigned num) const {return indicesOfSelectedProFiles[num];} // TODO check num
    bool IsPidValid(uint32 pid);
private:
    VectorOfStrings proFilesList;
    std::vector<MemBuffer> proFiles;
    std::vector<unsigned> indicesOfSelectedProFiles;
    bool ignoreByMinimumSize;
    size_t minimumCorrectSize;//прошки меньше этого размера будут пропускаться
};

//---------------------------------------------------------------------------
#endif
