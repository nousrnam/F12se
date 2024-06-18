/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include <stdlib.h>
#include <stdio.h>

#include "MSG.h"
#include "MemBuffer.h"
#include "ntst_loggingpp.hpp"
#include "misc.h"

#include <windows.h>

StringConvFunc GetToWideConvFunc(GameCodePage gcp)
    {
    switch (gcp)
        {
        case GCP_RUS_866: return RussianDosStringToWide;
        case GCP_RUS_LEVCORP: return RussianDosStringToWide;
        case GCP_CH_TR: return ChineseTraditionalStringToWide;
        case GCP_CH_S: return ChineseSimplifiedStringToWide;
        default: return LocalStringToWide;
        }
    }

MsgFile::MsgFile(const MemBuffer& buf, StringConvFunc convFunc)
   {
   Load(buf, convFunc);
   }

static void SkeepUntilNewline(const wchar_t* *startChar, const wchar_t* endChar)
    {
    while (*startChar < endChar)
        {
        (*startChar)++;
        if (**startChar == L'\r' || **startChar == L'\n')
            return;
        }
    }

static bool FindTextInBrackets(const wchar_t* *startChar, const wchar_t* endChar, const wchar_t* *startText, const wchar_t* *endText)
    {
    while (*startChar < endChar)
        {
        while (**startChar != L'{')
            {
            if (**startChar == L'#')
                {
                SkeepUntilNewline(startChar, endChar);
                return false;
                }
            (*startChar)++;
            if (*startChar >= endChar)
                return false;
            }
        *startText = *startChar + 1;
        while (**startChar != L'}')
            {
            (*startChar)++;
            if (*startChar >= endChar)
                return false;
            }
        *endText = *startChar;
        return true;
        }
    return false;
    }

void MsgFile::Load(const MemBuffer& buf, StringConvFunc convFunc)
    {
    name = buf.GetName();
    if (convFunc == NULL)
        {
        ntst_log_error("convFunc == NULL for file: " + name);
        return;
        }
    std::wstring tmp = convFunc((const char*)buf.GetAddress(), buf.GetSize());
    const wchar_t *curChar = tmp.c_str();
    const wchar_t *endChar = curChar + tmp.size();
    Clear();
    while (curChar < endChar)
        {
        const wchar_t* startText;
        const wchar_t* endText;
        if (!FindTextInBrackets(&curChar, endChar, &startText, &endText))
            continue;
        //int key = atoi(startText);
        int key;
        if(swscanf(startText, L"%d", &key) != 1)
            {
            ntst_log_error_w(L"Invalid integer key: " + std::wstring(startText, endText) + L" in file: " + LocalStringToWide(name));
            continue;
            }
        if (!FindTextInBrackets(&curChar, endChar, &startText, &endText))
            {
            ntst_log_error("There is no ACM sound file name after key: " + ntst::to_string(key) + " in file: " + name);
            continue;
            }
        std::wstring acmName = std::wstring(startText, endText);
        if (!FindTextInBrackets(&curChar, endChar, &startText, &endText))
            {
            ntst_log_error("There is no text after key: " + ntst::to_string(key) + " in file: " + name);
            continue;
            }
        std::wstring text = std::wstring(startText, endText);
        std::pair<std::map<int, MsgRec>::iterator, bool> ret = keyToRec.insert(std::pair<int, MsgRec>(key, MsgRec(acmName, text)));
        if (!ret.second)
            {
            const std::wstring& oldtext = ret.first->second.text;
            ntst_log_warning_w(L"The key(" + ntst::to_wstring(key) + L") with the text(" + text + L") already exists(" + oldtext + L") in file: " + LocalStringToWide(name) + L" new value is ignored.");
            }
        }
    }

std::wstring MsgFile::GetText(int key) const
    {
    std::map<int, MsgRec>::const_iterator it = keyToRec.find(key);
    if (it != keyToRec.end())
        return it->second.text;
    const std::wstring msg = L"Can't find " + ntst::to_wstring(key) + L" in " + LocalStringToWide(name);
    ntst_log_warning_w(msg);
    return L"*ERROR* " + msg;
    }
