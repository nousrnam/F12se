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

// Only for russian versions(fargus and levcorp)
static unsigned char CP866AndLevCorpToCP1251[] = {
0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF};

static unsigned char LevCorpToCP866[] = {
0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF};

static std::wstring StringToWide(UINT codePage, const char* str, size_t str_len)
    {
    std::wstring ret;
    if (str_len != 0)
        {
        int size_need = MultiByteToWideChar(codePage, 0, str, str_len, NULL, 0);
        if (size_need <= 0)
            {
            ntst_log_warning("failed to convert to wide string: \"" + std::string(str, str_len) + "\n error code:" + ntst::to_string(GetLastError()));
            return ret;
            }
        ret.resize(size_need);
        MultiByteToWideChar(codePage, 0, str, str_len, &ret[0], size_need);
        }
    return ret;
    }

static std::string WideToLocalString(UINT codePage, const wchar_t* str, size_t str_len)
    {
    std::string ret;
    if (str_len != 0)
        {
        int size_need = WideCharToMultiByte(codePage, 0, str, str_len, NULL, 0, NULL, NULL);
        if (size_need <= 0)
            {
            ntst_log_warning_w(L"failed to convert to multi byte string: \"" + std::wstring(str, str_len) + L"\n error code:" + ntst::to_wstring(GetLastError()));
            return ret;
            }
        ret.resize(size_need);
        WideCharToMultiByte(codePage, 0, str, str_len, &ret[0], size_need, NULL, NULL);
        }
    return ret;
    }

std::string WideStringToLocal(const wchar_t* str, size_t str_len)
    {
    return WideToLocalString(CP_ACP, str, str_len);
    }

std::string WideStringToRussianDos(const wchar_t* str, size_t str_len)
    {
    return WideToLocalString(866, str, str_len);
    }

#define LEV_OFFET 48
std::string WideStringToRussianLevCorp(const wchar_t* str, size_t str_len)
    {
    std::string ret = WideStringToRussianDos(str, str_len);
    for (std::string::size_type i = 0; i < ret.size(); ++i)
        {
        /*const unsigned char c = ret[i];
        if ((c > 175 + LEV_OFFET) && (c < 192 + LEV_OFFET))
            {
            ret[i] = c - LEV_OFFET;
            }*/
        ret[i] = LevCorpToCP866[(unsigned char)ret[i]];
        }
    return ret;
    }

std::string WideStringToChineseTraditional(const wchar_t* str, size_t str_len)
    {
    return WideToLocalString(950, str, str_len);
    }

std::string WideStringToChineseSimplified(const wchar_t* str, size_t str_len)
    {
    return WideToLocalString(936, str, str_len);
    }

std::wstring LocalStringToWide(const char* str, size_t str_len)
    {
    return StringToWide(CP_ACP, str, str_len);
    }

std::wstring LocalStringToWide(const std::string& str)
    {
    return LocalStringToWide(str.c_str(), str.size());
    }

std::wstring RussianDosStringToWide(const char* str, size_t str_len)
    {
    if (str_len == 0)
        return std::wstring();
    char* tmp = new char[str_len];
    for (size_t i = 0; i < str_len; ++i)
        {
        tmp[i] = CP866AndLevCorpToCP1251[(unsigned char)str[i]];
        }
    std::wstring ret = StringToWide(1251, tmp, str_len);
    delete[] tmp;
    return ret;
    }

std::wstring ChineseTraditionalStringToWide(const char* str, size_t str_len)
    {
    return StringToWide(950, str, str_len);
    }

std::wstring ChineseSimplifiedStringToWide(const char* str, size_t str_len)
    {
    return StringToWide(936, str, str_len);
    }

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
