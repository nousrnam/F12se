/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef VECTOROFSTRINGS_H
#define VECTOROFSTRINGS_H

#include <vector>
#include <string>

class MemBuffer;

class VectorOfStrings : public std::vector<std::string>
    {
public:
    void LoadFromMemory(void* addr, size_t bufSize);
    void LoadFromMemory(const MemBuffer& buf);
    void DeleteEmptyLinesAtTheEnd();
    };

#endif // VECTOROFSTRINGS_H
