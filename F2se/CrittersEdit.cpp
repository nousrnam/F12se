/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifdef _MSC_VER
// C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable: 4800)
#endif

#include "CrittersEdit.h"
#include "WinFunc.h"
#include "Common.h"
#include "Defines.h"
#include "wxWidgetsUserData.h"

//(*InternalHeaders(CrittersEdit)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(CrittersEdit)
const long CrittersEdit::lblCrittersList = wxNewId();
const long CrittersEdit::ID_LISTBOX1 = wxNewId();
const long CrittersEdit::bSave = wxNewId();
const long CrittersEdit::lblStatsBase = wxNewId();
const long CrittersEdit::ID_LISTBOX2 = wxNewId();
const long CrittersEdit::lblStatsBaseValue = wxNewId();
const long CrittersEdit::ID_TEXTCTRL1 = wxNewId();
const long CrittersEdit::lblStatsBonus = wxNewId();
const long CrittersEdit::ID_LISTBOX3 = wxNewId();
const long CrittersEdit::lblStatsBonusValue = wxNewId();
const long CrittersEdit::ID_TEXTCTRL2 = wxNewId();
const long CrittersEdit::lblSkills = wxNewId();
const long CrittersEdit::ID_LISTBOX4 = wxNewId();
const long CrittersEdit::lblSkillVal = wxNewId();
const long CrittersEdit::ID_TEXTCTRL3 = wxNewId();
const long CrittersEdit::cboxBarter = wxNewId();
const long CrittersEdit::cboxSteal = wxNewId();
const long CrittersEdit::cboxDrop = wxNewId();
const long CrittersEdit::cboxLimbs = wxNewId();
const long CrittersEdit::cboxAges = wxNewId();
const long CrittersEdit::cboxHeal = wxNewId();
const long CrittersEdit::cboxInvulnerable = wxNewId();
const long CrittersEdit::cboxFlatten = wxNewId();
const long CrittersEdit::cboxSpecial = wxNewId();
const long CrittersEdit::cboxRange = wxNewId();
const long CrittersEdit::cboxKnock = wxNewId();
const long CrittersEdit::lblFrmId = wxNewId();
const long CrittersEdit::ID_TEXTCTRL4 = wxNewId();
const long CrittersEdit::bCancel = wxNewId();
//*)

BEGIN_EVENT_TABLE(CrittersEdit,wxFrame)
	//(*EventTable(CrittersEdit)
	//*)
END_EVENT_TABLE()

CrittersEdit::CrittersEdit(wxWindow* parent, const DataModel& datamodel, wxWindowID id,const wxPoint&,const wxSize&) : data(datamodel)
{
	//(*Initialize(CrittersEdit)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer9;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	StaticText1 = new wxStaticText(this, lblCrittersList, _("Critters list"), wxDefaultPosition, wxDefaultSize, 0, _T("lblCrittersList"));
	BoxSizer2->Add(StaticText1, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	lbFiles = new wxListBox(this, ID_LISTBOX1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
	BoxSizer2->Add(lbFiles, 1, wxTOP|wxBOTTOM|wxEXPAND, 5);
	btnSave = new wxButton(this, bSave, _("Save selected file"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bSave"));
	BoxSizer2->Add(btnSave, 0, wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer2, 1, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 5);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	StaticText2 = new wxStaticText(this, lblStatsBase, _("Base Stats"), wxDefaultPosition, wxDefaultSize, 0, _T("lblStatsBase"));
	BoxSizer3->Add(StaticText2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	lbBaseStats = new MyListBox(this, ID_LISTBOX2, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX2"));
	BoxSizer3->Add(lbBaseStats, 1, wxALL|wxEXPAND, 5);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	StaticText4 = new wxStaticText(this, lblStatsBaseValue, _("Value"), wxDefaultPosition, wxDefaultSize, 0, _T("lblStatsBaseValue"));
	BoxSizer5->Add(StaticText4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtBaseStatVal = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	BoxSizer5->Add(txtBaseStatVal, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3->Add(BoxSizer5, 0, wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer3, 1, wxTOP|wxLEFT|wxEXPAND, 5);
	BoxSizer4 = new wxBoxSizer(wxVERTICAL);
	StaticText3 = new wxStaticText(this, lblStatsBonus, _("Bonus Stats"), wxDefaultPosition, wxDefaultSize, 0, _T("lblStatsBonus"));
	BoxSizer4->Add(StaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	lbBonusStats = new MyListBox(this, ID_LISTBOX3, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX3"));
	BoxSizer4->Add(lbBonusStats, 1, wxALL|wxEXPAND, 5);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	StaticText5 = new wxStaticText(this, lblStatsBonusValue, _("Value"), wxDefaultPosition, wxDefaultSize, 0, _T("lblStatsBonusValue"));
	BoxSizer6->Add(StaticText5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtBonusStatVal = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	BoxSizer6->Add(txtBonusStatVal, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer4->Add(BoxSizer6, 0, wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer4, 1, wxTOP|wxLEFT|wxEXPAND, 5);
	BoxSizer7 = new wxBoxSizer(wxVERTICAL);
	StaticText6 = new wxStaticText(this, lblSkills, _("Skills"), wxDefaultPosition, wxDefaultSize, 0, _T("lblSkills"));
	BoxSizer7->Add(StaticText6, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	lbSkills = new MyListBox(this, ID_LISTBOX4, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX4"));
	BoxSizer7->Add(lbSkills, 1, wxALL|wxEXPAND, 5);
	BoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	StaticText7 = new wxStaticText(this, lblSkillVal, _("Value"), wxDefaultPosition, wxDefaultSize, 0, _T("lblSkillVal"));
	BoxSizer8->Add(StaticText7, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtSkillVal = new wxTextCtrl(this, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	BoxSizer8->Add(txtSkillVal, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer7->Add(BoxSizer8, 0, wxEXPAND, 5);
	cbBarter = new wxCheckBox(this, cboxBarter, _("Can trade with"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxBarter"));
	cbBarter->SetValue(false);
	BoxSizer7->Add(cbBarter, 0, wxALL|wxALIGN_LEFT, 5);
	cbSteal = new wxCheckBox(this, cboxSteal, _("Can\'t steal from"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxSteal"));
	cbSteal->SetValue(false);
	BoxSizer7->Add(cbSteal, 0, wxALL|wxALIGN_LEFT, 5);
	cbDrop = new wxCheckBox(this, cboxDrop, _("Doesn\'t drop items"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxDrop"));
	cbDrop->SetValue(false);
	BoxSizer7->Add(cbDrop, 0, wxALL|wxALIGN_LEFT, 5);
	cbLimbs = new wxCheckBox(this, cboxLimbs, _("Can\'t lose limbs"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxLimbs"));
	cbLimbs->SetValue(false);
	BoxSizer7->Add(cbLimbs, 0, wxALL|wxALIGN_LEFT, 5);
	cbAges = new wxCheckBox(this, cboxAges, _("Dead body doesn\'t disappear"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxAges"));
	cbAges->SetValue(false);
	BoxSizer7->Add(cbAges, 0, wxALL|wxALIGN_LEFT, 5);
	cbHeal = new wxCheckBox(this, cboxHeal, _("Damage isn\'t cured with time"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxHeal"));
	cbHeal->SetValue(false);
	BoxSizer7->Add(cbHeal, 0, wxALL|wxALIGN_LEFT, 5);
	cbInvulnerable = new wxCheckBox(this, cboxInvulnerable, _("Can\'t be hurt"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxInvulnerable"));
	cbInvulnerable->SetValue(false);
	BoxSizer7->Add(cbInvulnerable, 0, wxALL|wxALIGN_LEFT, 5);
	cbFlatten = new wxCheckBox(this, cboxFlatten, _("Leaves no dead body"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxFlatten"));
	cbFlatten->SetValue(false);
	BoxSizer7->Add(cbFlatten, 0, wxALL|wxALIGN_LEFT, 5);
	cbSpecial = new wxCheckBox(this, cboxSpecial, _("Special type of death"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxSpecial"));
	cbSpecial->SetValue(false);
	BoxSizer7->Add(cbSpecial, 0, wxALL|wxALIGN_LEFT, 5);
	cbRange = new wxCheckBox(this, cboxRange, _("Melee attack at a distance"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxRange"));
	cbRange->SetValue(false);
	BoxSizer7->Add(cbRange, 0, wxALL|wxALIGN_LEFT, 5);
	cbKnock = new wxCheckBox(this, cboxKnock, _("Can\'t be knocked down"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxKnock"));
	cbKnock->SetValue(false);
	BoxSizer7->Add(cbKnock, 0, wxALL|wxALIGN_LEFT, 5);
	BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
	StaticText8 = new wxStaticText(this, lblFrmId, _("FRM ID(no effect)"), wxDefaultPosition, wxDefaultSize, 0, _T("lblFrmId"));
	BoxSizer9->Add(StaticText8, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtFID = new wxTextCtrl(this, ID_TEXTCTRL4, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	BoxSizer9->Add(txtFID, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer7->Add(BoxSizer9, 0, wxEXPAND, 5);
	btnCancel = new wxButton(this, bCancel, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bCancel"));
	BoxSizer7->Add(btnCancel, 0, wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer7, 1, wxALL|wxEXPAND, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_LISTBOX1,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&CrittersEdit::OnlbFilesSelect);
	Connect(bSave,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CrittersEdit::OnbtnSaveClick);
	Connect(ID_LISTBOX2,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&CrittersEdit::OnlbBaseStatsSelect);
	Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&CrittersEdit::OntxtBaseStatValText);
	Connect(ID_LISTBOX3,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&CrittersEdit::OnlbBonusStatsSelect);
	Connect(ID_TEXTCTRL2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&CrittersEdit::OntxtBonusStatValText);
	Connect(ID_LISTBOX4,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&CrittersEdit::OnlbSkillsSelect);
	Connect(ID_TEXTCTRL3,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&CrittersEdit::OntxtSkillValText);
	Connect(cboxBarter,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&CrittersEdit::OncboxClick);
	Connect(cboxSteal,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&CrittersEdit::OncboxClick);
	Connect(cboxDrop,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&CrittersEdit::OncboxClick);
	Connect(cboxLimbs,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&CrittersEdit::OncboxClick);
	Connect(cboxAges,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&CrittersEdit::OncboxClick);
	Connect(cboxHeal,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&CrittersEdit::OncboxClick);
	Connect(cboxInvulnerable,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&CrittersEdit::OncboxClick);
	Connect(cboxFlatten,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&CrittersEdit::OncboxClick);
	Connect(cboxSpecial,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&CrittersEdit::OncboxClick);
	Connect(cboxRange,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&CrittersEdit::OncboxClick);
	Connect(cboxKnock,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&CrittersEdit::OncboxClick);
	Connect(ID_TEXTCTRL4,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&CrittersEdit::OnTxtText);
	Connect(bCancel,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CrittersEdit::OnbtnCloseClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&CrittersEdit::OnClose);
	//*)
    pairStatBase.Set(lbBaseStats, txtBaseStatVal, 0, 0);
    SetWidgetLbTxtPairData(lbBaseStats, txtBaseStatVal, &pairStatBase);
    pairStatBonus.Set(lbBonusStats, txtBonusStatVal, 0, 0);
    SetWidgetLbTxtPairData(lbBonusStats, txtBonusStatVal, &pairStatBonus);
    pairSkill.Set(lbSkills, txtSkillVal, 0, 0);
    SetWidgetLbTxtPairData(lbSkills, txtSkillVal, &pairSkill);
	dir = data.curSave.GetCrittersDir();
	std::vector<std::string> files = GetFiles(dir, "*.pro");
	std::string fullpath;
	for (size_t i = 0; i < files.size(); ++i)
        {
        fullpath = dir + '\\' + files[i];
        MemBufferEX b;
        LoadGZipFileInMemBuffer(fullpath, b);
        buffers.push_back(b);

        uint32 descriptionID = b.GetInv32(PRO_DID);
        std::wstring critterName = data.proCritterMSG.GetText(descriptionID);

        lbFiles->Append(critterName + L'(' + files[i] + L')');
        }
    SetWidgetData(txtFID, PRO_FID);
    cbBarter->SetClientData((void*)2);
    cbSteal->SetClientData((void*)32);
    cbDrop->SetClientData((void*)64);
    cbLimbs->SetClientData((void*)128);
    cbAges->SetClientData((void*)256);
    cbHeal->SetClientData((void*)512);
    cbInvulnerable->SetClientData((void*)1024);
    cbFlatten->SetClientData((void*)2048);
    cbSpecial->SetClientData((void*)4096);
    cbRange->SetClientData((void*)8192);
    cbKnock->SetClientData((void*)16384);
}

CrittersEdit::~CrittersEdit()
{
	//(*Destroy(CrittersEdit)
	//*)
}


void CrittersEdit::OnClose(wxCloseEvent& event)
    {
    event.Skip(true);
    }

void CrittersEdit::OnbtnCloseClick(wxCommandEvent&)
    {
    Close();
    }

void CrittersEdit::OnbtnSaveClick(wxCommandEvent&)
    {
    int index = lbFiles->GetSelection();
    if (index == wxNOT_FOUND)
        return;
    MemBufferSaveInGZipFile(buffers[index].GetName(), buffers[index]);
    }

void UpdateCheckBoxState(wxCheckBox* cb, const MemBuffer& buf)
    {
    cb->SetValue(buf.GetInv32(PRO_CRITTER_FLAGS_OFFSET) & (uint32)cb->GetClientData());
    }

void CrittersEdit::OnlbFilesSelect(wxCommandEvent&)
    {
    int index = lbFiles->GetSelection();
    if (index == wxNOT_FOUND)
        return;
    const MemBuffer& buf = buffers[index];
    RefreshListBox(lbBaseStats, buf, PRO_CRITTER_BASE_STAT_OFFSET, STAT_SIZE, 0, buf.GetSize(), data.stats);
    RefreshListBox(lbBonusStats, buf, PRO_CRITTER_BONUS_STAT_OFFSET, STAT_SIZE, 0, buf.GetSize(), data.stats);
    RefreshListBox(lbSkills, buf, PRO_CRITTER_SKILL_OFFSET, SKILLS_SIZE, 0, buf.GetSize(), data.skills);
    UpdateCheckBoxState(cbBarter, buf);
    UpdateCheckBoxState(cbSteal, buf);
    UpdateCheckBoxState(cbDrop, buf);
    UpdateCheckBoxState(cbLimbs, buf);
    UpdateCheckBoxState(cbAges, buf);
    UpdateCheckBoxState(cbHeal, buf);
    UpdateCheckBoxState(cbInvulnerable, buf);
    UpdateCheckBoxState(cbFlatten, buf);
    UpdateCheckBoxState(cbSpecial, buf);
    UpdateCheckBoxState(cbRange, buf);
    UpdateCheckBoxState(cbKnock, buf);
    txtFID->ChangeValue(wxString() << buf.GetInv32(GetWidgetData(txtFID)));
    }

void CrittersEdit::OncboxClick(wxCommandEvent& event)
    {
    wxCheckBox *cb = (wxCheckBox*)event.GetEventObject();
    int index = lbFiles->GetSelection();
    if (index == wxNOT_FOUND)
        return;
    MemBuffer& buf = buffers[index];
    uint32 mask = (uint32)cb->GetClientData();
    uint32 val = buf.GetInv32(PRO_CRITTER_FLAGS_OFFSET);
    if (cb->IsChecked())
        val = val | mask;
    else
        val = val & ~mask;
    buf.SetInv32(PRO_CRITTER_FLAGS_OFFSET, val);
    }

void CrittersEdit::OnTxtForListChange(wxListBox *lb, wxTextCtrl* txt, unsigned blockOffset, unsigned blockSize, const std::vector<std::wstring>& names)
    {
    int indexFile = lbFiles->GetSelection();
    if (indexFile == wxNOT_FOUND)
        return;
    int indexVal = lb->GetSelection();
    if (indexVal == wxNOT_FOUND)
        return;
    int32 val;
    if (wxTextToInt(val, txt))
        {
        int offset = blockOffset + indexVal * sizeof(int32);
        MemBuffer& buf = buffers[indexFile];
        buf.SetInv32(offset, val);
        RefreshListBox(lb, buf, blockOffset, blockSize, offset, sizeof(int32), names);
        }
    }

void CrittersEdit::OntxtBaseStatValText(wxCommandEvent&)
    {
    OnTxtForListChange(lbBaseStats, txtBaseStatVal, PRO_CRITTER_BASE_STAT_OFFSET, STAT_SIZE, data.stats);
    }

void CrittersEdit::OntxtBonusStatValText(wxCommandEvent&)
    {
    OnTxtForListChange(lbBonusStats, txtBonusStatVal, PRO_CRITTER_BONUS_STAT_OFFSET, STAT_SIZE, data.stats);
    }

void CrittersEdit::OntxtSkillValText(wxCommandEvent&)
    {
    OnTxtForListChange(lbSkills, txtSkillVal, PRO_CRITTER_SKILL_OFFSET, SKILLS_SIZE, data.skills);
    }

void CrittersEdit::OnLbSelect(wxListBox *lb, wxTextCtrl* txt, unsigned blockOffset)
    {
    const int indexFile = lbFiles->GetSelection();
    if (indexFile == wxNOT_FOUND)
        return;
    const int indexVal = lb->GetSelection();
    if (indexVal == wxNOT_FOUND)
        return;
    int32 val = buffers[indexFile].GetInv32(blockOffset + indexVal * sizeof(int32));
    txt->ChangeValue(wxString() << val);
    }

void CrittersEdit::OnlbBaseStatsSelect(wxCommandEvent&)
    {
    OnLbSelect(lbBaseStats, txtBaseStatVal, PRO_CRITTER_BASE_STAT_OFFSET);
    }

void CrittersEdit::OnlbBonusStatsSelect(wxCommandEvent&)
    {
    OnLbSelect(lbBonusStats, txtBonusStatVal, PRO_CRITTER_BONUS_STAT_OFFSET);
    }

void CrittersEdit::OnlbSkillsSelect(wxCommandEvent&)
    {
    OnLbSelect(lbSkills, txtSkillVal, PRO_CRITTER_SKILL_OFFSET);
    }

void CrittersEdit::OnTxtText(wxCommandEvent& event)
    {
    const int indexFile = lbFiles->GetSelection();
    if (indexFile == wxNOT_FOUND)
        return;

    wxTextCtrl *txt = (wxTextCtrl*)event.GetEventObject();
    int32 val;
    if (wxTextToInt(val, txt))
        {
        unsigned offset = GetWidgetData(txt);
        MemBuffer& buf = buffers[indexFile];
        buf.SetInv32(offset, val);
        }
    }
