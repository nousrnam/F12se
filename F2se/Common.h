/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>

#include <wx/listbox.h>
#include <wx/string.h>
#include <wx/textctrl.h>

#include "GAM.h"
#include "MemBufferEX.h"

std::string WxStringToStdLocal(const wxString& str);
std::string WxStringToStdUTF8(const wxString& str);

bool wxTextToInt(int32 &result, wxTextCtrl *widget);

wxString GetItemName(const std::vector<std::wstring>& itemsNames, size_t itemIndex);
wxString GetItemName(const GamFile& gamfile, size_t itemIndex);

/** \brief
 *
 * \param lb wxListBox* кого обновляем
 * \param buf const MemBufferEX& откуда берем данные
 * \param blockOffset size_t смещение откуда берутся данные для записи в lb
 * \param blockCount size_t максимальный размер блока откуда заносятся данные в lb. (Например для параметров STATS_CNT(35) * sizeof(int32))
 * \param changeOffset size_t смещение откуда начались изменения
 * \param changeCount size_t число измененных байт
 * \param names const std::vector<std::string>& откуда берутся имена для записей в lb
 * \return void
 *
 */
template <class NamesType>
void RefreshListBox(wxListBox *lb, const MemBufferEX& buf, size_t blockOffset, size_t blockCount, size_t changeOffset, size_t changeCount, const NamesType& names)
    {
    if (blockCount % sizeof(int32) != 0)
        {
        ntst_log_error("blockCount % sizeof(int32) != 0 in RefreshListBox function.");
        return;
        }
    unsigned updateOffset;
    unsigned updateCount;
    if (!BlocksOverlap(blockOffset, blockCount, changeOffset, changeCount, &updateOffset, &updateCount))
        return;

    const unsigned firstItem = (updateOffset - blockOffset) / sizeof(int32);
    const unsigned lastItem = firstItem + (updateCount + sizeof(int32) - 1) / sizeof(int32);
    for (unsigned item = firstItem; item < lastItem; ++item)
        {
        wxString itemText = GetItemName(names, item);
        itemText << " = ";
        const size_t readOffset = blockOffset + item * sizeof(int32);
        if (readOffset + sizeof(int32) <= buf.GetSize())
            itemText << buf.GetInv32(readOffset);
        else
            itemText << "INVALID_VALUE";
        if (item < lb->GetCount())
            lb->SetString(item, itemText);
        else if (item == lb->GetCount())
            lb->Append(itemText);
        else
            ntst_log_error("Error in RefreshListBox().");
        }
    }

#endif // COMMON_H
