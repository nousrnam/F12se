/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include "VecOfStr.h"
#include "MemBuffer.h"

void VectorOfStrings::LoadFromMemory(void* addr, size_t bufSize)
    {
    if (bufSize == 0)
        return;
    //reserve(500); // most files are less than 500 lines.
    char *c = (char*)addr;
    char *lineStart = (char*)addr;
    const char * const endChar = (char*)addr + bufSize;
    char prevC = 0, prevprevC = 0;
    bool lineBreak = false;
    for (; c < endChar; ++c)
        {
        //prevlineBreak = lineBreak;
        if (lineBreak)
            {
            if (*c == '\n')
                {
                if (prevC == '\n' || (prevprevC == '\n' && prevC == '\r'))
                    push_back(std::string());
                }
            else if (*c == '\r')
                {
                if (prevC == '\r' || (prevprevC == '\r' && prevC == '\n'))
                    push_back(std::string());
                }
            else
                {
                lineStart = c;
                lineBreak = false;
                }
            }
        else
            {
            if (*c == '\r' || *c == '\n')
                {
                push_back(std::string(lineStart, (size_t)(c - lineStart)));
                lineBreak = true;
                }
            }
        prevprevC = prevC;
        prevC = *c;
        }
    if (!lineBreak)
        push_back(std::string(lineStart, (size_t)(endChar - lineStart))); // Add last line
    else if (prevprevC != '\r' && prevprevC != '\n')
        push_back(std::string());
    //else if (!prevlineBreak && (prevC == '\r' || prevC == '\n'))
    //    push_back(std::string());
    }

void VectorOfStrings::LoadFromMemory(const MemBuffer& buf)
    {
    LoadFromMemory(buf.GetAddress(), buf.GetSize());
    }

void VectorOfStrings::DeleteEmptyLinesAtTheEnd()
    {
    size_t newSize = size();
    while (newSize > 0)
        {
        if (operator[](newSize-1).empty())
            newSize--;
        else
            break;
        }
    resize(newSize);
    }
/*
void StringList::DeleteComment(const char* str)
{
int i, j;
for (i = 0; i < count; i++)
 {
 j = strvec[i].find(str);
 if (j != -1)
	strvec[i].erase(strvec[i].begin() + j, strvec[i].end());
 }
}*/

#ifndef NDEBUG

#include <assert.h>

class VectorOfStringsTest
    {
public:
    VectorOfStringsTest();
    };

VectorOfStringsTest::VectorOfStringsTest()
    {
    VectorOfStrings testV;
    char test0[] = {'1','\r','\n','2'};
    testV.LoadFromMemory(test0, sizeof(test0));
    assert(testV.size() == 2);
    assert(testV[0] == "1");
    assert(testV[1] == "2");
    testV.clear();
    char test1[] = {'1','\n','\r','2'};
    testV.LoadFromMemory(test1, sizeof(test1));
    assert(testV.size() == 2);
    assert(testV[0] == "1");
    assert(testV[1] == "2");
    testV.clear();
    char test2[] = {'1','\n','\n','2'};
    testV.LoadFromMemory(test2, sizeof(test2));
    assert(testV.size() == 3);
    assert(testV[0] == "1");
    assert(testV[1] == "");
    assert(testV[2] == "2");
    testV.clear();
    char test3[] = {'1','\r','\r','2'};
    testV.LoadFromMemory(test3, sizeof(test3));
    assert(testV.size() == 3);
    assert(testV[0] == "1");
    assert(testV[1] == "");
    assert(testV[2] == "2");
    testV.clear();
    char test4[] = {'\r','\r'};
    testV.LoadFromMemory(test4, sizeof(test4));
    assert(testV.size() == 2);
    assert(testV[0] == "");
    assert(testV[1] == "");
    testV.DeleteEmptyLinesAtTheEnd();
    assert(testV.size() == 0);
    testV.clear();
    char test5[] = {'\n','\n'};
    testV.LoadFromMemory(test5, sizeof(test5));
    assert(testV.size() == 2);
    assert(testV[0] == "");
    assert(testV[1] == "");
    testV.clear();
    char test6[] = {'\r','\n','\r'};
    testV.LoadFromMemory(test6, sizeof(test6));
    assert(testV.size() == 2);
    assert(testV[0] == "");
    assert(testV[1] == "");
    testV.DeleteEmptyLinesAtTheEnd();
    assert(testV.size() == 0);
    testV.clear();
    char test7[] = {'\n','\r','\n'};
    testV.LoadFromMemory(test7, sizeof(test7));
    assert(testV.size() == 2);
    assert(testV[0] == "");
    assert(testV[1] == "");
    testV.DeleteEmptyLinesAtTheEnd();
    assert(testV.size() == 0);
    testV.clear();
    char test8[] = {'1','\r','2','\r'};
    testV.LoadFromMemory(test8, sizeof(test8));
    assert(testV.size() == 3);
    assert(testV[0] == "1");
    assert(testV[1] == "2");
    assert(testV[2] == "");
    testV.DeleteEmptyLinesAtTheEnd();
    assert(testV.size() == 2);
    testV.clear();
    char test9[] = {'1','\n','2','\n'};
    testV.LoadFromMemory(test9, sizeof(test9));
    assert(testV.size() == 3);
    assert(testV[0] == "1");
    assert(testV[1] == "2");
    assert(testV[2] == "");
    testV.DeleteEmptyLinesAtTheEnd();
    assert(testV.size() == 2);
    }

VectorOfStringsTest vectorOfStringsTest;

#endif
