/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "IniFile.h"
#include "fallout.h"
#include "misc.h"

template <>
bool DataTxtFile<3>::LoadRow(const MemBuffer& buf, const std::string& line, Row& row)
    {
    int ret = sscanf(line.c_str(), "%d,%d,%d", &row.cols[0], &row.cols[1], &row.cols[2]);
    if (ret != 3)
        {
        if (ret > 0)
            ntst_log_info("Invalid string: " + line + " in file: " + buf.GetName());
        return false;
        }
    return true;
    }

template <>
bool DataTxtFile<4>::LoadRow(const MemBuffer& buf, const std::string& line, Row& row)
    {
    int ret = sscanf(line.c_str(), "%d,%d,%d,%d", &row.cols[0], &row.cols[1], &row.cols[2], &row.cols[3]);
    if (ret != 4)
        {
        if (ret > 0)
            ntst_log_info("Invalid string: " + line + " in file: " + buf.GetName());
        return false;
        }
    return true;
    }

template <>
bool DataTxtFile<5>::LoadRow(const MemBuffer& buf, const std::string& line, Row& row)
    {
    int ret = sscanf(line.c_str(), "%d ,%d ,%d ,%d ,%d", &row.cols[0], &row.cols[1], &row.cols[2], &row.cols[3], &row.cols[4]);
    if (ret != 5)
        {
        if (ret > 0)
            ntst_log_info("Invalid string: " + line + " in file: " + buf.GetName());
        return false;
        }
    return true;
    }

void QuestsTxt::LoadFromMem(const MemBuffer& buf)
   {
   QuestRec tmpQRec;
   DataTxtFile::LoadFromMem(buf);
   Sort(0);//отсортируем по локациям чтобы они были по порядку
   if (Rows() == 0)
      return;
   records.clear();
   size_t curLoc = Get(0, 0);
   tmpQRec.start = 0;
   int length = 1;
   for (size_t i = 1; i < Rows(); i++)
      {
      if (Get(i, 0) == curLoc)
         {
         length++;
         continue;
         }
      curLoc = Get(i, 0);
      tmpQRec.length = length;
      records.push_back(tmpQRec);
      tmpQRec.start = i;
      length = 1;
      }
   tmpQRec.length = length;
   records.push_back(tmpQRec);
   }

bool GetScriptDescSize(unsigned scriptType, int *size)
    {
/*
Тип скрипта(ScriptType)
Тип(по информации от SeaWolf)
Тип(по информации из Mapper2.exe)
0x00 - s_system
0x01 spatial s_spatial
0x02 items s_time
0x03 scenery s_item
0x04 critters s_critter

"Также следует отметить, что автору не встречались карты (MAP-файлы) в которых использовались скрипты типов 0x00 и 0x02."
*/
    switch (scriptType)
        {
        case 1:
            *size = 72;
            return true;
        case 2:
            *size = 68;
            return true;
        case 3:
        case 4:
            *size = 64;
            return true;
        default:
            ntst_log_warning("Requested the size of the invalid script descriptor type: " + ntst::to_string(scriptType));
            return false;
        }
    }

unsigned GetScriptDescSizeForJunk(unsigned scriptType)
    {
    switch (scriptType)
        {
        case 1:
            return 72;
        case 2:
            return 68;
        default:
            return 64;
        }
    }

bool GetItemSizeFromType(unsigned type, unsigned *size)
    {
    static unsigned typesize[] = {0x5C, 0x5C, 0x5C, 0x64, 0x60, 0x60, 0x60};
    if (type < sizeof(typesize))
        {
        *size = typesize[type];
        return true;
        }
    ntst_log_warning("Requested the size of the invalid item type: " + ntst::to_string(type));
    return false;
    }

void ScriptList::LoadFromMem(const MemBuffer& buf)
   {
   VectorOfStrings scrlst;
   ScriptRec tmpRec;
   std::string curline;
   scrlst.LoadFromMemory(buf);
   clear();
   reserve(scrlst.size());
   for (size_t i = 0; i < scrlst.size(); i++)
      {
      curline = scrlst[i];
      std::string::size_type pos1 = curline.find(';');
      std::string::size_type pos2 = curline.find_last_of('#');
      std::string::size_type pos3 = curline.find_last_of('=');
      //если какие нибудь корявые строки то заполним пустотой
      if (pos1 == std::string::npos || pos2 == std::string::npos || pos3 == std::string::npos)
         {
         tmpRec.filename = "!ERROR!";
         tmpRec.comment = "!ERROR!";
         tmpRec.varCount = 0;
         push_back(tmpRec);
         continue;
         }
      tmpRec.filename = curline.substr(0, pos1);
      tmpRec.comment = curline.substr(pos1 + 1, pos2 - pos1 - 1);
      tmpRec.varCount = atoi(curline.c_str() + pos3 + 1);
      tmpRec.filename = Trim(tmpRec.filename);
      tmpRec.comment = Trim(tmpRec.comment);
      push_back(tmpRec);
      }
   }
