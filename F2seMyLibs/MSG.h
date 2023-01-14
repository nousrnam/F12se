/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef MSGH
#define MSGH

#include <string>
#include <map>
#include "commontypes.h"

enum GameCodePage
    {
    GCP_SYSTEM_DEFAULT = 0,
    GCP_RUS_866,
    GCP_RUS_LEVCORP,
    GCP_CH_TR,
    GCP_CH_S
    };

std::string WideStringToLocal(const wchar_t* str, size_t str_len);
std::string WideStringToRussianDos(const wchar_t* str, size_t str_len);
std::string WideStringToRussianLevCorp(const wchar_t* str, size_t str_len);
std::string WideStringToChineseTraditional(const wchar_t* str, size_t str_len);
std::string WideStringToChineseSimplified(const wchar_t* str, size_t str_len);

std::wstring LocalStringToWide(const char* str, size_t str_len);
std::wstring LocalStringToWide(const std::string& str);
std::wstring RussianDosStringToWide(const char* str, size_t str_len);
std::wstring ChineseTraditionalStringToWide(const char* str, size_t str_len);
std::wstring ChineseSimplifiedStringToWide(const char* str, size_t str_len);

typedef std::wstring (*StringConvFunc)(const char*, size_t);

StringConvFunc GetToWideConvFunc(GameCodePage gcp);

struct MsgRec
   {
   MsgRec(const std::wstring& acm, const std::wstring& txt) : acmName(acm), text(txt) {;}
   std::wstring acmName;
   std::wstring text;
   };

class MemBuffer;

class MsgFile
    {
public:
    MsgFile() : name("empty default msg file") {;}
    MsgFile(const MemBuffer& buf, StringConvFunc convFunc = LocalStringToWide);
    void Load(const MemBuffer& buf, StringConvFunc convFunc = LocalStringToWide);
    std::wstring GetText(int key) const;
    size_t Size() const {return keyToRec.size();}\
    void Clear() {keyToRec.clear();}
private:
    std::string name;
    std::map<int, MsgRec> keyToRec;
};
//---------------------------------------------------------------------------
#endif
