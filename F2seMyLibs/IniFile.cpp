/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include <stdlib.h>
#include "IniFile.h"

#define NEW_LINE_STR "\n"

std::string StringToLower(const std::string& str)
    {
    std::string result;
    result.reserve(str.size());
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
        {
        result += tolower(*it);
        }
    return result;
    }

KeyCaseInsensetive::KeyCaseInsensetive(const std::string& str) : key(str), key_low(StringToLower(str))
    {
    ;
    }

static bool IsNewLine(int c)
    {
    return ((c) == '\n' || (c) == '\r');
    }

static bool IsUtf8BomChar(int c)
    {
    return ((c == 0xEF) || (c == 0xBB) || (c == 0xBF));
    }

static void TrimTrailingSpaces(std::string& str)
    {
    str.erase(str.find_last_not_of(" \t")+1);
    }

std::string IniSection::GetValue(const std::string& key, const std::string& defaultValue) const
    {
    const_iterator it = keyToVal.find(key);
    return it == keyToVal.end() ? defaultValue : it->second;
    }

bool IniSection::GetValueBool(const std::string& key, bool defaultValue) const
    {
    const_iterator it = keyToVal.find(key);
    return it == keyToVal.end() ? defaultValue : it->second != "0";
    }

int IniSection::GetValueInt(const std::string& key, int defaultValue) const
    {
    const_iterator it = keyToVal.find(key);
    return it == keyToVal.end() ? defaultValue : atoi(it->second.c_str());
    }

double IniSection::GetValueDouble(const std::string& key, double defaultValue) const
    {
    const_iterator it = keyToVal.find(key);
    return it == keyToVal.end() ? defaultValue : atof(it->second.c_str());
    }

const std::string* IniSection::FindValue(const std::string& key) const
    {
    const_iterator it = keyToVal.find(key);
    return it == keyToVal.end() ? NULL : &it->second;
    }

std::string IniSection::ToString() const
    {
    std::string result;
    for (IniSection::const_iterator it = begin(); it != end(); ++it)
        {
        result += it->first;
        result += '=';
        result += it->second;
        result += NEW_LINE_STR;
        }
    return result;
    }

enum State
    {
    UTF8_BOM_SKEEP,
    NEW_LINE,
    IN_SECTION_NAME,
    IN_KEY_NAME,
    IN_KEY_VALUE,
    IGNORE_UNTIL_NEWLINE
    };

struct MemorySrc
    {
    const unsigned char* cur;
    const unsigned char* end;
    };

static inline int GetC(FILE *f)
    {
    return getc(f);
    }

static inline int GetC(MemorySrc *ms)
    {
    if (ms->cur == ms->end)
        return EOF;
    const int result = *ms->cur;
    ms->cur++;
    return result;
    }

bool IniFile::Load(const char* filename)
    {
    FILE* f = fopen(filename, "rb");
    if (f == NULL)
        {
        return false;
        }
    bool exception = false;
    try
        {
        LoadPrivate(f);
        }
    catch(...)
        {
        // Как минимум может кончиться память
        exception = true;
        }
    return (fclose(f) == 0 && !exception);
    }

bool IniFile::Load(const std::string& filename)
    {
    return Load(filename.c_str());
    }

void IniFile::Load(void* addr, size_t size)
    {
    MemorySrc src;
    src.cur = (const unsigned char*)addr;
    src.end = src.cur + size;
    LoadPrivate(&src);
    }

/*void IniFile::Load(const MemBuffer& buf)
    {
    Load(buf.GetAddress(), buf.GetSize());
    }*/

template <class Source>
void IniFile::LoadPrivate(Source src)
    {
    int c;
    std::string sectionName; sectionName.reserve(16);
    std::string keyName; keyName.reserve(16);
    std::string value; value.reserve(64);
    IniSection *curSection = &defaultSection;//NULL;

    State state = UTF8_BOM_SKEEP;
    while ((c = GetC(src)) != EOF)
        {
        switch (state)
            {
            case UTF8_BOM_SKEEP:
                if (IsUtf8BomChar(c))
                    break;
                state = NEW_LINE;
                // Fall through
            case NEW_LINE:
                if (isspace(c))
                    break;
                if (c == '[')
                    {
                    sectionName.clear();
                    state = IN_SECTION_NAME;
                    }
                else if (c == ';'/* || curSection == NULL*/)
                    state = IGNORE_UNTIL_NEWLINE;
                else
                    {
                    keyName = c;
                    state = IN_KEY_NAME;
                    }
                break;
            case IN_SECTION_NAME:
                if (c == ']')
                    {
                    std::pair<iterator, bool> ret;
                    ret = keyToSection.insert(std::pair<KeyType, IniSection>(sectionName, IniSection(sectionName)));
                    curSection = &ret.first->second;
                    state = IGNORE_UNTIL_NEWLINE;
                    }
                else
                    sectionName += c;
                break;
            case IN_KEY_NAME:
                if (c == '=')
                    {
                    TrimTrailingSpaces(keyName);
                    value.clear();
                    state = IN_KEY_VALUE;
                    }
                else if (IsNewLine(c))
                    state = NEW_LINE;
                else
                    keyName += c;
                break;
            case IN_KEY_VALUE:
                if (IsNewLine(c))
                    {
                    curSection->SetValue(keyName, value);
                    state = NEW_LINE;
                    }
                else
                    value += c;
                break;
            case IGNORE_UNTIL_NEWLINE:
                if (IsNewLine(c))
                    state = NEW_LINE;
                break;
            }
        }

    // Key's name and value are not parsed if curSection == NULL so there is no need to check it.
    if (state == IN_KEY_VALUE)
        curSection->SetValue(keyName, value);
    }

bool IniFile::Save(const char* filename) const
    {
    FILE *f = fopen(filename, "w");
    if (f == NULL)
        {
        return false;
        }
    for (const_iterator itS = begin(); itS != end(); ++itS)
        {
        if (fprintf(f, "[%s]" NEW_LINE_STR, itS->first.c_str()) < 0)
            {
            fclose(f);
            return false;
            }
        for (IniSection::const_iterator itR = itS->second.begin(); itR != itS->second.end(); ++itR)
            {
            if (fprintf(f, "%s=%s" NEW_LINE_STR, itR->first.c_str(), itR->second.c_str()) < 0)
                {
                fclose(f);
                return false;
                }
            }
        }
    return (fclose(f) == 0);
    }

bool IniFile::Save(const std::string& filename) const
    {
    return Save(filename.c_str());
    }

void IniFile::SetValue(const std::string& section, const std::string& key, const std::string& value)
    {
    keyToSection[section].SetValue(key, value);
    }

void IniFile::SetValue(const std::string& section, const std::string& key, int value)
    {
    keyToSection[section].SetValue(key, value);
    }

void IniFile::SetValue(const std::string& section, const std::string& key, double value)
    {
    keyToSection[section].SetValue(key, value);
    }

std::string IniFile::GetValue(const std::string& section, const std::string& key, const std::string& defaultValue) const
    {
    const_iterator it = keyToSection.find(section);
    return (it == keyToSection.end() ? defaultValue : it->second.GetValue(key, defaultValue));
    }

bool IniFile::GetValueBool(const std::string& section, const std::string& key, bool defaultValue) const
    {
    const_iterator it = keyToSection.find(section);
    return (it == keyToSection.end() ? defaultValue : it->second.GetValueBool(key, defaultValue));
    }

bool IniFile::TryGetValue(const std::string& section, const std::string& key, int& value) const
    {
    const_iterator it = keyToSection.find(section);
    if (it == keyToSection.end())
        return false;
    return it->second.TryGetValue(key, value);
    }

int IniFile::GetValueInt(const std::string& section, const std::string& key, int defaultValue) const
    {
    const_iterator it = keyToSection.find(section);
    return (it == keyToSection.end() ? defaultValue : it->second.GetValueInt(key, defaultValue));
    }

double IniFile::GetValueDouble(const std::string& section, const std::string& key, double defaultValue) const
    {
    const_iterator it = keyToSection.find(section);
    return (it == keyToSection.end() ? defaultValue : it->second.GetValueDouble(key, defaultValue));
    }

IniSection IniFile::GetSection(const std::string& section) const
    {
    const_iterator it = keyToSection.find(section);
    return (it == keyToSection.end() ? IniSection(section) : it->second);
    }

const IniSection* IniFile::FindSection(const std::string& section) const
    {
    const_iterator it = keyToSection.find(section);
    return (it == keyToSection.end() ? NULL : &it->second);
    }

IniSection* IniFile::FindSection(const std::string& section)
    {
    iterator it = keyToSection.find(section);
    return (it == keyToSection.end() ? NULL : &it->second);
    }

IniSection* IniFile::FindSectionOrCreate(const std::string& section)
    {
    return &keyToSection[section];
    }

bool IniFile::DeleteSection(const std::string& section)
    {
    iterator it = keyToSection.find(section);
    if (it == keyToSection.end())
        return false;
    keyToSection.erase(it);
    return true;
    }

std::string IniFile::ToString() const
    {
    std::string result;
    for (const_iterator it = begin(); it != end(); ++it)
        {
        result += '[';
        result += it->first;
        result += "]" NEW_LINE_STR;
        result += it->second.ToString();
        result += NEW_LINE_STR;
        }
    return result;
    }

const std::string* IniFile::FindValue(const std::string& section, const std::string& key) const
    {
    const IniSection *sec = FindSection(section);
    return sec == NULL ? NULL : sec->FindValue(key);
    }
