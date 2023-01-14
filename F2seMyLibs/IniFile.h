/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef INIFILE_H
#define INIFILE_H

#include <cstdio>
#include <string>
#include <map>

typedef std::string KeyCaseSensetive;

class KeyCaseInsensetive
    {
public:
    KeyCaseInsensetive() {;}
    KeyCaseInsensetive(const std::string& str);
    const char* c_str() const {return key.c_str();}
    bool operator<(const KeyCaseInsensetive& r) const {return key_low < r.key_low;}
    operator const std::string&() const {return key;}
private:
    std::string key;
    std::string key_low;
    };

typedef KeyCaseInsensetive KeyType;

class IniSection
    {
public:
    IniSection() {;}
    IniSection(const std::string& sectionName) : name(sectionName) {}

    template<class ValType>
    void SetValue(const std::string& key, const ValType& value)
        {
        keyToVal[key] = ValToStr(value);
        }

    bool TryGetValue(const std::string& key, std::string& result) const
        {
        const std::string* str = FindValue(key);
        if (str != NULL)
            {
            result = *str;
            return true;
            }
        return false;
        }

    bool TryGetValue(const std::string& key, int& result) const
        {
        return TryGetValStrTemplate(FindValue(key), "%d", result);
        }

    bool TryGetValue(const std::string& key, double& result) const
        {
        return TryGetValStrTemplate(FindValue(key), "%f", result);
        }

    std::string GetValue(const std::string& key, const std::string& defaultValue = std::string()) const;
    bool GetValueBool(const std::string& key, bool defaultValue = false) const;
    int GetValueInt(const std::string& key, int defaultValue = 0) const;
    double GetValueDouble(const std::string& key, double defaultValue = 0.0) const;
    const std::string* FindValue(const std::string& key) const;
    std::string GetName() const {return name;}
    std::string ToString() const;

    typedef std::map<KeyType, std::string> MapType;
    typedef MapType::iterator iterator;
    typedef MapType::const_iterator const_iterator;

    const_iterator begin() const {return keyToVal.begin();}
    const_iterator end() const {return keyToVal.end();}
private:
    template<class ValType>
    static bool TryGetValStrTemplate(const std::string* str, const char* format, ValType& result)
        {
        if (str == NULL)
            return false;
        return sscanf(str->c_str(), format, &result) == 1;
        }

    template<class ValType>
    static std::string ValToStrTemplate(ValType val, const char* format)
        {
        char buf[21];
#if _MSC_VER
        _snprintf_s(buf, sizeof(buf), sizeof(buf), format, val);
#else
        snprintf(buf, sizeof(buf), format, val);
#endif
        return buf;
        }
    static std::string ValToStr(const std::string& value) {return value;}
    static std::string ValToStr(const int& value) {return ValToStrTemplate(value, "%d");}
    static std::string ValToStr(const double& value) {return ValToStrTemplate(value, "%0.0f");}
    KeyType name;
    std::map<KeyType, std::string> keyToVal;
    };

class IniFile
    {
public:
    bool Load(const char* filename);
    bool Load(const std::string& filename);
    void Load(void* addr, size_t size);
    bool Save(const char* filename) const;
    bool Save(const std::string& filename) const;
    size_t GetIniSectionCount() const {return keyToSection.size();}
    void Clear() {keyToSection.clear();}
    bool Empty() const {return keyToSection.empty();}
    void SetValue(const std::string& section, const std::string& key, const std::string& value);
    void SetValue(const std::string& section, const std::string& key, int value);
    void SetValue(const std::string& section, const std::string& key, double value);
    std::string GetValue(const std::string& section, const std::string& key, const std::string& defaultValue = std::string()) const;
    bool TryGetValue(const std::string& section, const std::string& key, std::string& value) const
        {
        const std::string* val = FindValue(section, key);
        if (val == NULL)
            {
            return false;
            }
        value = *val;
        return true;
        }
    bool GetValueBool(const std::string& section, const std::string& key, bool defaultValue = false) const;
    bool TryGetValue(const std::string& section, const std::string& key, int& value) const;
    int GetValueInt(const std::string& section, const std::string& key, int defaultValue = 0) const;
    double GetValueDouble(const std::string& section, const std::string& key, double defaultValue = 0.0) const;
    const std::string* FindValue(const std::string& section, const std::string& key) const;
    const IniSection* FindSection(const std::string& section) const;
    IniSection GetSection(const std::string& section) const;
    IniSection* FindSection(const std::string& section);
    IniSection* FindSectionOrCreate(const std::string& section);
    IniSection* GetDefaultSection() {return &defaultSection;}
    const IniSection* GetDefaultSection() const {return &defaultSection;}
    IniSection& GetDefaultSectionRef() {return defaultSection;}
    const IniSection& GetDefaultSectionRef() const {return defaultSection;}
    bool DeleteSection(const std::string& section);

    typedef std::map<KeyType, IniSection> MapType;
    typedef MapType::iterator iterator;
    typedef MapType::const_iterator const_iterator;

    const_iterator begin() const {return keyToSection.begin();}
    const_iterator end() const {return keyToSection.end();}

    std::string ToString() const;
private:
    IniSection defaultSection;
    std::map<KeyType, IniSection> keyToSection;
    template <class Source>
    void LoadPrivate(Source src);
    };

#endif // INIFILE_H
