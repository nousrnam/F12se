/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include "SavEdit.h"
#include "Common.h"

//(*InternalHeaders(SavEdit)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/statbox.h>

//(*IdInit(SavEdit)
const long SavEdit::ID_LISTBOX1 = wxNewId();
const long SavEdit::ID_TEXTCTRL1 = wxNewId();
const long SavEdit::lblValueMvar = wxNewId();
const long SavEdit::ID_TEXTCTRL2 = wxNewId();
const long SavEdit::lblScripts = wxNewId();
const long SavEdit::ID_LISTBOX2 = wxNewId();
const long SavEdit::ID_TEXTCTRL3 = wxNewId();
const long SavEdit::lblLVars = wxNewId();
const long SavEdit::ID_LISTBOX3 = wxNewId();
const long SavEdit::lblValueLvar = wxNewId();
const long SavEdit::ID_TEXTCTRL4 = wxNewId();
const long SavEdit::bCancel = wxNewId();
const long SavEdit::bSave = wxNewId();
//*)

BEGIN_EVENT_TABLE(SavEdit,wxFrame)
	//(*EventTable(SavEdit)
	//*)
END_EVENT_TABLE()

SavEdit::SavEdit(wxWindow* parent, const SAVfile& savFile, const GamFile& mapgam, const ScriptList& scriptsList, wxWindowID id) : sav(savFile), gam(mapgam), scriptList(scriptsList)
{
    scripts = sav.GetScripts();
	//(*Initialize(SavEdit)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer7;
	wxStaticBoxSizer* sbsLocalVariables;
	wxStaticBoxSizer* sbsMapVariables;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
	SetClientSize(wxSize(640,400));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	sbsMapVariables = new wxStaticBoxSizer(wxVERTICAL, this, _("Map variables"));
	lbMvar = new MyListBox(this, ID_LISTBOX1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
	sbsMapVariables->Add(lbMvar, 1, wxALL|wxEXPAND, 5);
	txtMvarDesc = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	sbsMapVariables->Add(txtMvarDesc, 0, wxALL|wxEXPAND, 5);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(this, lblValueMvar, _("Value"), wxDefaultPosition, wxDefaultSize, 0, _T("lblValueMvar"));
	BoxSizer4->Add(StaticText1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtMVarVal = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	BoxSizer4->Add(txtMVarVal, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	sbsMapVariables->Add(BoxSizer4, 0, wxEXPAND, 5);
	BoxSizer2->Add(sbsMapVariables, 0, wxALL|wxEXPAND, 5);
	sbsLocalVariables = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Local variables"));
	BoxSizer5 = new wxBoxSizer(wxVERTICAL);
	StaticText2 = new wxStaticText(this, lblScripts, _("Scripts"), wxDefaultPosition, wxDefaultSize, 0, _T("lblScripts"));
	BoxSizer5->Add(StaticText2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	lbScripts = new wxListBox(this, ID_LISTBOX2, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX2"));
	BoxSizer5->Add(lbScripts, 1, wxALL|wxEXPAND, 5);
	txtScriptComment = new wxTextCtrl(this, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	BoxSizer5->Add(txtScriptComment, 0, wxALL|wxEXPAND, 5);
	sbsLocalVariables->Add(BoxSizer5, 1, wxLEFT|wxRIGHT|wxEXPAND, 5);
	BoxSizer6 = new wxBoxSizer(wxVERTICAL);
	StaticText3 = new wxStaticText(this, lblLVars, _("Variables"), wxDefaultPosition, wxDefaultSize, 0, _T("lblLVars"));
	BoxSizer6->Add(StaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	lbLvar = new MyListBox(this, ID_LISTBOX3, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX3"));
	BoxSizer6->Add(lbLvar, 1, wxALL|wxEXPAND, 5);
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	StaticText4 = new wxStaticText(this, lblValueLvar, _("Value"), wxDefaultPosition, wxDefaultSize, 0, _T("lblValueLvar"));
	BoxSizer7->Add(StaticText4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtLVarVal = new wxTextCtrl(this, ID_TEXTCTRL4, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	BoxSizer7->Add(txtLVarVal, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer6->Add(BoxSizer7, 0, wxALL|wxEXPAND, 0);
	sbsLocalVariables->Add(BoxSizer6, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
	BoxSizer2->Add(sbsLocalVariables, 1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer2, 1, wxALL|wxEXPAND, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	btnCancel = new wxButton(this, bCancel, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bCancel"));
	BoxSizer3->Add(btnCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	btnSave = new wxButton(this, bSave, _("Save"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bSave"));
	BoxSizer3->Add(btnSave, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_RIGHT, 5);
	SetSizer(BoxSizer1);
	SetSizer(BoxSizer1);
	Layout();

	Connect(ID_LISTBOX1,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&SavEdit::OnlbMvarSelect);
	Connect(ID_TEXTCTRL2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&SavEdit::OntxtMVarValText);
	Connect(ID_LISTBOX2,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&SavEdit::OnlbScriptsSelect);
	Connect(ID_LISTBOX3,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&SavEdit::OnlbLvarSelect);
	Connect(ID_TEXTCTRL4,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&SavEdit::OntxtLVarValText);
	Connect(bCancel,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SavEdit::OnbtnCancelClick);
	Connect(bSave,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SavEdit::OnbtnSaveClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&SavEdit::OnClose);
	//*)
    pairMapVar.Set(lbMvar, txtMVarVal, 0, 0);
    SetWidgetLbTxtPairData(lbMvar, txtMVarVal, &pairMapVar);
    pairScriptVar.Set(lbLvar, txtLVarVal, 0, 0);
    SetWidgetLbTxtPairData(lbLvar, txtLVarVal, &pairScriptVar);

	sbsLocalVariables->GetStaticBox()->SetName("sbLocalVariables");
	sbsMapVariables->GetStaticBox()->SetName("sbMapVariables");
	RefreshMvars();
	RefreshScripts();
	Layout();
}

SavEdit::~SavEdit()
{
	//(*Destroy(SavEdit)
	//*)
}

void SavEdit::RefreshMvars()
    {
    lbMvar->Clear();
    for (unsigned i = 0; i < sav.GetMvarNum(); i++)
        lbMvar->Append(GetMVarText(i));
    }

std::string SavEdit::GetMVarText(unsigned index)
    {
    std::string result = ntst::to_string(index);
    //легкая паранойя еще никогда не вредила
    //else сработало в SFELRONB
    if (index < gam.size())
        result += ":" + gam[index].name + "=";
    else
        result += ": ??? =";
    result += ntst::to_string(sav.GetMvar(index));
    return result;
    }

void SavEdit::RefreshScripts()
    {
    lbScripts->Clear();
    for (unsigned i = 0; i < scripts.size(); i++)
        lbScripts->Append(scriptList.GetScriptFilenameBySid(scripts[i].SID) + '[' + GetScriptType(scripts[i].scrType) + ']');
    }

void SavEdit::OnClose(wxCloseEvent& event)
    {
    event.Skip(true);
    }

void SavEdit::OnbtnCancelClick(wxCommandEvent&)
    {
    Close();
    }

void SavEdit::OnbtnSaveClick(wxCommandEvent&)
    {
    sav.SaveFile();
    Close();
    }

void SavEdit::OnlbMvarSelect(wxCommandEvent&)
    {
    int index = lbMvar->GetSelection();
    if (index == wxNOT_FOUND)
        return;
    txtMvarDesc->ChangeValue(gam.GetVarDescription(index));
    txtMVarVal->ChangeValue(wxString() << sav.GetMvar(index));
    }

static std::wstring GetLVarLine(int num, int val)
    {
    return L"LVAR_" + ntst::to_wstring(num) + L'=' + ntst::to_wstring(val) + L" [" + ntst::to_wstring_hex_pref(val) + L']';
    }

void SavEdit::OnlbScriptsSelect(wxCommandEvent&)
    {
    lbLvar->Clear();
    int index_ = lbScripts->GetSelection();
    if (index_ == wxNOT_FOUND)
        return;
    unsigned index = index_;
    txtScriptComment->ChangeValue(wxString());
    txtLVarVal->ChangeValue(wxString());
    if (index >= scripts.size())
        {
        ntst_log_error("index >= scripts.size()");
        return;
        }
    for (int i = 0; i < scripts[index].lvarCount; i++)
        lbLvar->Append(GetLVarLine(i, sav.GetSvar(scripts[index].startIndex + i)));
    txtScriptComment->ChangeValue(scriptList.GetCommentBySid(scripts[index].SID));
    }

void SavEdit::OnlbLvarSelect(wxCommandEvent&)
    {
    int indexscr = lbScripts->GetSelection();
    int indexlvar = lbLvar->GetSelection();
    if (indexscr == wxNOT_FOUND || indexlvar == wxNOT_FOUND)
        return;
    txtLVarVal->ChangeValue(wxString() << sav.GetSvar(scripts[indexscr].startIndex + indexlvar));
    }

void SavEdit::OntxtMVarValText(wxCommandEvent&)
    {
    int index = lbMvar->GetSelection();
    if (index == wxNOT_FOUND)
        return;
    int32 val;
    if (!wxTextToInt(val, txtMVarVal))
        return;
    //ntst_log_info("Changing MVAR[" + IntToStr(index) + "] from " + IntToStr(sav.GetMvar(index)) + " to " + IntToStr(val));
    sav.SetMvar(index, val);
    lbMvar->SetString(index, GetMVarText(index));
    }

void SavEdit::OntxtLVarValText(wxCommandEvent&)
    {
    int indexscr = lbScripts->GetSelection();
    int indexlvar = lbLvar->GetSelection();
    if (indexscr == wxNOT_FOUND || indexlvar == wxNOT_FOUND)
        return;
    int32 val;
    if (!wxTextToInt(val, txtLVarVal))
        return;
    unsigned svarindex = scripts[indexscr].startIndex + indexlvar;
    //ntst_log_info("Changing LVAR[" + IntToStr(offset / sizeof(int32)) + "] from " + IntToStr(sav.GetInv32(offset)) + " to " + IntToStr(val));
    sav.SetSvar(svarindex, val);
    lbLvar->SetString(indexlvar, GetLVarLine(indexlvar, sav.GetSvar(svarindex)));
    }
