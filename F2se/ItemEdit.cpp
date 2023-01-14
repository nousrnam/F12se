/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include "Defines.h"
#include "ItemEdit.h"
#include "Common.h"

//(*InternalHeaders(ItemEdit)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(ItemEdit)
const long ItemEdit::ID_LISTBOX1 = wxNewId();
const long ItemEdit::ID_TEXTCTRL2 = wxNewId();
const long ItemEdit::ID_LISTBOX2 = wxNewId();
const long ItemEdit::lblValue = wxNewId();
const long ItemEdit::ID_TEXTCTRL1 = wxNewId();
const long ItemEdit::bCancel = wxNewId();
const long ItemEdit::bOk = wxNewId();
//*)

BEGIN_EVENT_TABLE(ItemEdit,wxDialog)
	//(*EventTable(ItemEdit)
	//*)
END_EVENT_TABLE()

wxString descriptionDefault[25] = {
"The quantity of this item.",
"Always 0.",
"Location of item. Necessarily always -1 (in inventory)",
"Always 0.",
"Always 0.",
"Unknown, 0 for most objects. Only non-zero for some items with special/quest uses. (E.g. poison tank, rubber boots, tool).",
"Unknown, like field 0x14, only nonzero for items with special uses.",
"Unknown, always 0.",
"Unknown, usually 0. For plant spikes, this is 1, and for sharpened poles, 2.",
"Item's on-ground image.",
"A bitfield: 0x01000000 indicates the item is held in the first item slot, 0x02000000 - in the second slot, and 0x04000000 - worn (as armor). Use of other bits is unknown.",
"0, 1, or 2. Map level, which is irrelevant since it's in inventory.",
"Object ID.",
"Always -1.",
"For Roentgen Rum and Gamma Gulp Beer, this is 0x00000001, for all others 0.",
"For Roentgen Rum and Gamma Gulp Beer, this is 0x00010000, for all others 0.",
"Always 0.",
"Script ID; -1 for none.",
"Always -1.",
"Usually 0; for containers it is the number of items in the container. The enclosed items are the ones immediately following it.",
"Unknown. Non-zero only for containers.",
"Unknown. Non-zero only for containers.",
"Always 0.",
"Many uses. For weapons, the number of loaded rounds. For ammunition, the number of rounds in the 'top' package (the one that is partly full). Number of charges for geiger counters/motion sensors. May be a script ID in some cases, like 0x44. For everything else, it is usually -1 or 0xCCCCCCCC. Not present for armor, containers, or drugs.",
"For weapons, the type of ammunition it uses; -1 if it needs no ammo. Only present for weapons."
};

ItemEdit::ItemEdit(wxWindow* parent, MemBuffer *b, const std::vector<itemNamePid>& v, wxWindowID id,const wxPoint&,const wxSize&)
{
    for (unsigned i = 0; i < NTST_ARRAY_LEN(description); ++i)
        description[i] = descriptionDefault[i];
	//(*Initialize(ItemEdit)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(640,450));
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer4 = new wxBoxSizer(wxVERTICAL);
	lbOffsets = new MyListBox(this, ID_LISTBOX1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
	BoxSizer4->Add(lbOffsets, 4, wxALL|wxEXPAND, 5);
	txtDescr = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	BoxSizer4->Add(txtDescr, 1, wxALL|wxEXPAND, 5);
	BoxSizer2->Add(BoxSizer4, 1, wxEXPAND, 5);
	lbItems = new wxListBox(this, ID_LISTBOX2, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX2"));
	BoxSizer2->Add(lbItems, 1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer2, 1, wxEXPAND, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(this, lblValue, _("Value"), wxDefaultPosition, wxDefaultSize, 0, _T("lblValue"));
	BoxSizer3->Add(StaticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	txtValue = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	BoxSizer3->Add(txtValue, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	btnCancel = new wxButton(this, bCancel, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bCancel"));
	BoxSizer3->Add(btnCancel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	btnOk = new wxButton(this, bOk, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bOk"));
	BoxSizer3->Add(btnOk, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxEXPAND, 5);
	SetSizer(BoxSizer1);
	SetSizer(BoxSizer1);
	Layout();

	Connect(ID_LISTBOX1,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&ItemEdit::OnlbOffsetsSelect);
	Connect(ID_LISTBOX2,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&ItemEdit::OnlbItemsSelect);
	Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ItemEdit::OntxtValueText);
	Connect(bCancel,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ItemEdit::OnbtnCancelClick);
	Connect(bOk,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ItemEdit::OnbtnOkClick);
	//*)
    pairTxtLb.Set(lbOffsets, txtValue, 0, 0);
    SetWidgetLbTxtPairData(lbOffsets, txtValue, &pairTxtLb);
    Bind(wxEVT_CHAR_HOOK, &ItemEdit::OnChar, this);
	buf = b;
	vec = v;
    for (unsigned i = 0; i < buf->GetSize() / 4; ++i)
        {
        unsigned offset = i * 4;
        lbOffsets->Append(OffsetToString(offset));
        }
    for (unsigned i = 0; i < vec.size(); ++i)
        {
        lbItems->Append(vec[i].name);
        }
    Layout();
}

ItemEdit::~ItemEdit()
{
	//(*Destroy(ItemEdit)
	//*)
}

void ItemEdit::SelectPidIfPossible(uint32 pid)
    {
    for (unsigned i = 0; i < vec.size(); ++i)
        {
        if (vec[i].pid == pid)
            lbItems->SetSelection(i);
        }
    }

void ItemEdit::OnbtnCancelClick(wxCommandEvent&)
    {
    EndModal(wxID_CANCEL);
    }

void ItemEdit::OnbtnOkClick(wxCommandEvent&)
    {
    EndModal(wxID_OK);
    }

void ItemEdit::RefreshOffsetLine(unsigned num)
    {
    if (num > lbOffsets->GetCount())
        return;
    unsigned offset = num * sizeof(int32);
    lbOffsets->SetString(num, OffsetToString(offset));
    }

void ItemEdit::OnlbOffsetsSelect(wxCommandEvent&)
    {
    int index = lbOffsets->GetSelection();
    if (index == wxNOT_FOUND)
        return;
    unsigned offset = index * sizeof(int32);
    txtValue->ChangeValue(wxString() << buf->GetInv32(offset));
    if ((unsigned)index < sizeof(description)/sizeof(*description))
        txtDescr->ChangeValue(description[index]);
    }

void ItemEdit::OntxtValueText(wxCommandEvent&)
    {
    int index = lbOffsets->GetSelection();
    if (index == wxNOT_FOUND)
        return;
    int32 val;
    if (wxTextToInt(val, txtValue))
        {
        buf->SetInv32(index * sizeof(int32), val);
        RefreshOffsetLine(index);
        }
    }

void ItemEdit::OnlbItemsSelect(wxCommandEvent&)
    {
    lbItemsSelect();
    }

void ItemEdit::lbItemsSelect()
    {
    int index = lbItems->GetSelection();
    if (index == wxNOT_FOUND)
        return;
    if ((size_t)index >= vec.size())
        return;
    buf->SetInv32(INVEN_ITEM_PID, vec[index].pid);
    int num = INVEN_ITEM_PID / sizeof(int32);
    RefreshOffsetLine(num);
    if (num == lbOffsets->GetSelection())
        txtValue->ChangeValue(wxString() << buf->GetInv32(INVEN_ITEM_PID));
    }

void ItemEdit::Localize(const Localization& l, const char* sectionName)
    {
    l.LocalizeWindow(this, sectionName);
    const IniSection *s = l.file.FindSection("ItemsOffsets");
    if (s == NULL)
        return;
    for (unsigned i = 0; i < NTST_ARRAY_LEN(description); ++i)
        {
        const std::string *str = s->FindValue(IntToHex(i * 4, 2));
        if (str != NULL)
            description[i] = wxString::FromUTF8(str->c_str(), str->size());
        }
    }

void ItemEdit::OnChar(wxKeyEvent& event)
    {
    if (event.GetKeyCode() == WXK_ESCAPE)
        EndModal(wxID_CANCEL);
    else
        event.Skip();
    }
