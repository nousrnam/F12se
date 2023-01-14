/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef GAMH
#define GAMH

#include <string>
#include <vector>

#include "commontypes.h"

struct GameVar
    {
    GameVar(const std::string& _name, int32 _val, const std::string& _comment) : name(_name), val(_val), comment(_comment) {;}
    std::string name;
    int32 val;
    std::string comment;
    };

class MemBuffer;

class GamFile : public std::vector<GameVar>
    {
public:
    void LoadGameVars(const MemBuffer& buf);
    std::string GetVarDescription(unsigned num);
    int GetVarInitVal(unsigned num);
    };

//---------------------------------------------------------------------------
#endif
