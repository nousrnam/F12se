/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "PRO.h"
#include "FileFinder.h"
#include "misc.h"
#include "ntst_loggingpp.hpp"

uint32 PidToIndex(uint32 pid)
    {
    return (pid - 1) & 0xFFFFFF;
    }

void ProWorker::LoadPro(ProType type, FileFinder& ff)
    {
    std::string pathbegin, lstname;

    Clear();
    switch (type)
        {
        case PRO_TYPE_ITEMS:
            pathbegin = "proto\\items\\";
            lstname = "items.lst";
            break;
        case PRO_TYPE_CRITTERS:
            pathbegin = "proto\\critters\\";
            lstname = "critters.lst";
            break;
        case PRO_TYPE_SCENERY:
            pathbegin = "proto\\scenery\\";
            lstname = "scenery.lst";
            break;
        case PRO_TYPE_WALLS:
            pathbegin = "proto\\walls\\";
            lstname = "walls.lst";
            break;
        case PRO_TYPE_TILES:
            pathbegin = "proto\\tiles\\";
            lstname = "tiles.lst";
            break;
        case PRO_TYPE_MISC:
            pathbegin = "proto\\misc\\";
            lstname = "misc.lst";
            break;
        default:
            ntst_log_error("Invalid PRO type : " + ntst::to_string(type));
            return;
        }
    proFilesList.LoadFromMemory(ff.GetFile(pathbegin + lstname));
    proFilesList.DeleteEmptyLinesAtTheEnd();
    size_t proQuantity = proFilesList.size();
    proFiles.reserve(proQuantity);
    for (size_t i = 0; i < proQuantity; i++)
        {
        std::string filename = pathbegin + proFilesList[i];
        if (proFilesList[i].size() != 12)
            ntst_log_info("The unusual length of the file name: " + filename);
        if (ff.FindFile(filename))
            {
            const MemBuffer& file = ff.GetFoundFile();
            size_t index = PidToIndex(ntst_bytes_swap32(file.Get32(PRO_OFFSET_PID)));
            if (index != i)
                ntst_log_info("ObjectID for file " + filename + " expected to be " + ntst::to_string(i) + " but it is " + ntst::to_string(index));
            proFiles.push_back(file);
            }
        else
            {
            ntst_log_warning("Can't find file: " + filename + " Creating dummy file.");
            proFiles.push_back(MemBuffer("dummy for pro file: " + filename));
            }
        }
    //SelectAll();
    }

bool ProWorker::Get4b(int32 pid, size_t offset, int32 *val) const
    {
    return Get4bByIndex(PidToIndex(pid), offset, val);
    }

int32 ProWorker::Get4b(int32 pid, size_t offset) const
    {
    return Get4bByIndex(PidToIndex(pid), offset);
    }

int8 ProWorker::Get1b(int32 pid, size_t offset) const
    {
    return Get1bByIndex(PidToIndex(pid), offset);
    }

void ProWorker::Set4b(int32 pid, size_t offset, int32 val)
    {
    Set4bByIndex(PidToIndex(pid), offset, val);
    }

void ProWorker::Set1b(int32 pid, size_t offset, int8 val)
    {
    Set1bByIndex(PidToIndex(pid), offset, val);
    }

bool ProWorker::Get4bByIndex(size_t ind, size_t offset, int32 *val) const
    {
    if (ind >= proFiles.size())
        return false;
    if (offset + sizeof(int32) > proFiles[ind].GetSize())
        return false;
    *val = proFiles[ind].GetInv32(offset);
    return true;
    }

int32 ProWorker::Get4bByIndex(size_t ind, size_t offset) const
    {
    if (ind >= proFiles.size())
        {
        ntst_log_error("Reading 4 bytes from pro file with invalid index: " + ntst::to_string(ind) + ", number of files is: " + ntst::to_string(proFiles.size()));
        return 0;
        }
    return proFiles[ind].GetInv32(offset);
    }

int8 ProWorker::Get1bByIndex(size_t ind, size_t offset) const
    {
    if (ind >= proFiles.size())
        {
        ntst_log_error("Reading 1 byte from pro file with invalid index: " + ntst::to_string(ind) + ", number of files is: " + ntst::to_string(proFiles.size()));
        return 0;
        }
    return proFiles[ind].Get8(offset);
    }

void ProWorker::Set4bByIndex(size_t ind, size_t offset, int32 val)
    {
    if (ind >= proFiles.size())
        {
        ntst_log_error("Writing 4 bytes to pro file with invalid index: " + ntst::to_string(ind) + ", number of files is: " + ntst::to_string(proFiles.size()));
        return;
        }
    proFiles[ind].SetInv32(offset, val);
    }

void ProWorker::Set1bByIndex(size_t ind, size_t offset, int8 val)
    {
    if (ind >= proFiles.size())
        {
        ntst_log_error("Writing 1 byte to pro file with invalid index: " + ntst::to_string(ind) + ", number of files is: " + ntst::to_string(proFiles.size()));
        return;
        }
    proFiles[ind].Set8(offset, val);
    }

void ProWorker::SelectAll()
    {
    indicesOfSelectedProFiles.clear();
    indicesOfSelectedProFiles.reserve(proFiles.size());
    size_t proQuantity = GetProQuantity();
    if (ignoreByMinimumSize)
        {
        for (size_t i = 0; i < proQuantity; i++)
            {
            if (proFiles[i].GetSize() >= minimumCorrectSize)
                indicesOfSelectedProFiles.push_back(i);
            else
                ntst_log_info("File " + proFiles[i].GetName() +
                        " was ignored during selection of all files because has size " + ntst::to_string(proFiles[i].GetSize()) +
                        ", minimum size is " + ntst::to_string(minimumCorrectSize));
            }
        }
    else
        {
        for (size_t i = 0; i < proQuantity; i++)
            indicesOfSelectedProFiles.push_back(i);
        }
    }

void ProWorker::SelectEqual(unsigned offset, int32 val)
    {
    std::vector<unsigned> newIndicesOfSelectedProFiles;
    newIndicesOfSelectedProFiles.reserve(newIndicesOfSelectedProFiles.size());
    for (std::vector<unsigned>::const_iterator it = indicesOfSelectedProFiles.begin(); it != indicesOfSelectedProFiles.end(); ++it)
        {
        int32 proval;
        if (proFiles[*it].GetInv32(offset, &proval) && proval == val)
            newIndicesOfSelectedProFiles.push_back(*it);
        }
    indicesOfSelectedProFiles = newIndicesOfSelectedProFiles;
    }

bool ProWorker::IsPidValid(uint32 pid)
    {
    unsigned index = PidToIndex(pid);
    if (index < proFiles.size())
        return true;
    return false;
    }
