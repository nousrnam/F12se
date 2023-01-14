/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include <stdlib.h>
#include <string.h>

#include "GAM.h"
#include "MemBuffer.h"
#include "VecOfStr.h"
#include "misc.h"
#include "ntst_stringpp.hpp"
#include "ntst_loggingpp.hpp"

void GamFile::LoadGameVars(const MemBuffer& buf)
    {
    VectorOfStrings tmplist;

    clear();
    tmplist.LoadFromMemory(buf);
    const size_t listSize = tmplist.size();
    reserve(listSize); //зарезервируем место под столько GVAR сколько строк в файле. Излишне, но сойдет.
    for (size_t i = 0; i < listSize; i++)
        {
        const std::string& curstr = tmplist[i];
        size_t posAssign = curstr.find(":=");
        size_t posComment = curstr.find("//");
        //как выяснилось из первого фаллаута не всегда есть точка с запятой
        //BAD_MONSTER               :=0        //   (160)
        if (posAssign != std::string::npos && (posAssign < posComment || posComment == std::string::npos))
            {
            std::string name = Trim(curstr.substr(0, posAssign));
            int32 val = atoi(&curstr[posAssign] + 2);
            std::string comment;
            if (posComment != std::string::npos)
                {
                comment = Trim(curstr.substr(posComment + 2));
                comment = Trim(comment);
                }
            push_back(GameVar(name, val, comment));
            }
        }
    }

std::string GamFile::GetVarDescription(unsigned num)
    {
    std::string result;
    if (num < size())
        {
        const GameVar& var = operator[](num);
        if (!var.comment.empty())
            result = "Comment: " + var.comment + '\n';
        result += "Initial value: " + ntst::to_string(var.val);
        }
    else
        {
        result = "Requested variable(" + ntst::to_string(num) + ") is outside of the variables list(size=" + ntst::to_string(size()) + ')';
        ntst_log_warning(result);
        }
    return result;
    }

int GamFile::GetVarInitVal(unsigned num)
    {
    if (num < size())
        {
        return operator[](num).val;
        }
    ntst_log_warning("GetVarInitVal variable(" + ntst::to_string(num) + ") is outside of the variables list(size=" + ntst::to_string(size()) + ')');
    return 0;
    }
