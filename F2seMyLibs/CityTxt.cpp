#include "CityTxt.h"

#include "IniFile.h"
#include "MemBuffer.h"
#include "ntst_loggingpp.hpp"
#include "ntst_stringpp.hpp"

// Should be <= 9, but just in case
#define MAX_ENTR_NUM 11

static bool StringContainOnBeforeComma(const std::string& str)
    {
    const size_t strSize = str.size();
    for (size_t i = 0; i < strSize; ++i)
        {
        const char c = str[i];
        if (c == 'n')
            return true;
        if (c == ',')
            return false;
        }
    return false;
    }

void CityTxt::Load(IniFile &iniFile)
    {
    CityRec tmpCityRec;

    Clear();
    records.reserve(iniFile.GetIniSectionCount());
    for (IniFile::const_iterator it = iniFile.begin(); it != iniFile.end(); ++it)
        {
        const std::string *area_name = it->second.FindValue("area_name");
        if (area_name == NULL)
            {
            ntst_log_warning("In City.txt " + (std::string)it->first + " doesn't have area_name.");
            tmpCityRec.name = it->first;
            }
        else
            tmpCityRec.name = *area_name;

        tmpCityRec.startStateOn = StringContainOnBeforeComma(it->second.GetValue("start_state"));

        tmpCityRec.entrances.clear();
        unsigned cur_entr_num;
        for (cur_entr_num = 0; cur_entr_num < MAX_ENTR_NUM; ++cur_entr_num)
            {
            const std::string *entr = it->second.FindValue("entrance_" + ntst::to_string(cur_entr_num));
            if (entr == NULL)
                break;
            tmpCityRec.entrances.push_back(Entrance(StringContainOnBeforeComma(*entr)));
            }
        for (; cur_entr_num < MAX_ENTR_NUM; ++cur_entr_num)
            {
            if (it->second.FindValue("entrance_" + ntst::to_string(cur_entr_num)) != NULL)
                {
                ntst_log_warning("Invalid entrances format in area: " + tmpCityRec.name);
                break;
                }
            }
        records.push_back(tmpCityRec);
        }
    }

void CityTxt::Load(const MemBuffer &buf)
    {
    IniFile tmpIni;
    tmpIni.Load(buf.GetAddress(), buf.GetSize());
    Load(tmpIni);
    }
