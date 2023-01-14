/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#if defined(_MSC_VER)
#define NOMINMAX // disable min max macro
#endif
#include "Common.h"
#include <stdint.h>
#include <limits>

std::string WxStringToStdLocal(const wxString& str)
    {
    return str.ToStdString();
    }

std::string WxStringToStdUTF8(const wxString& str)
    {
    const wxCharBuffer buffer = str.ToUTF8();
    return std::string(buffer.data(), buffer.length());
    }

bool wxTextToInt(int32_t &result, wxTextCtrl *widget)
    {
    long val; unsigned long uval;
    wxString str = widget->GetValue();
    if (str.ToULong(&uval) || str.ToULong(&uval, 16))
        {
        if (uval > std::numeric_limits<uint32_t>::max())
            return false;
        result = uval;
        return true;
        }
    if (str.ToLong(&val) || str.ToLong(&val, 16))
        {
        if ((val > std::numeric_limits<int32_t>::max()) || (val < std::numeric_limits<int32_t>::min()))
            return false;
        result = val;
        return true;
        }
    return false;
    }

wxString GetItemName(const std::vector<std::wstring>& itemsNames, size_t itemIndex)
    {
    wxString itemText;
    if (itemIndex < itemsNames.size())
        itemText << itemsNames[itemIndex];
    else
        itemText << L"Error! Invalid index #" << itemIndex;
    return itemText;
    }

wxString GetItemName(const GamFile& gamfile, size_t itemIndex)
    {
    wxString itemText;
    itemText.reserve(50);
    itemText << itemIndex << ':';
    if (itemIndex < gamfile.size())
        itemText << gamfile[itemIndex].name;
    else
        itemText << "Invalid GVAR";
    return itemText;
    }
