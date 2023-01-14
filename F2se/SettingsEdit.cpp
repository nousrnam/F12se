/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifdef _MSC_VER
// C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable: 4800)
#endif

#include <algorithm>
#include "SettingsEdit.h"
#include "WinFunc.h"
#include "Common.h"

//(*InternalHeaders(SettingsEdit)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(SettingsEdit)
const long SettingsEdit::lblPath = wxNewId();
const long SettingsEdit::ID_COMBOBOX1 = wxNewId();
const long SettingsEdit::bBrowse = wxNewId();
const long SettingsEdit::rboxGame = wxNewId();
const long SettingsEdit::ID_LISTBOX1 = wxNewId();
const long SettingsEdit::cboxLogInfo = wxNewId();
const long SettingsEdit::cboxLogWarning = wxNewId();
const long SettingsEdit::cboxLogError = wxNewId();
const long SettingsEdit::lblOverrideFalloutExe = wxNewId();
const long SettingsEdit::ID_TEXTCTRL3 = wxNewId();
const long SettingsEdit::lblOverrideFalloutCfg = wxNewId();
const long SettingsEdit::ID_TEXTCTRL4 = wxNewId();
const long SettingsEdit::lblOverrideFalloutLanguage = wxNewId();
const long SettingsEdit::ID_TEXTCTRL2 = wxNewId();
const long SettingsEdit::cboxCorItemFid = wxNewId();
const long SettingsEdit::cboxCorAmmoT = wxNewId();
const long SettingsEdit::cboxCorAmmoQ = wxNewId();
const long SettingsEdit::cboxCorPerks = wxNewId();
const long SettingsEdit::cboxCorTraits = wxNewId();
const long SettingsEdit::lblBrightness = wxNewId();
const long SettingsEdit::ID_SCROLLBAR1 = wxNewId();
const long SettingsEdit::rboxImagesResize = wxNewId();
const long SettingsEdit::bCancel = wxNewId();
const long SettingsEdit::bOk = wxNewId();
//*)

BEGIN_EVENT_TABLE(SettingsEdit,wxDialog)
	//(*EventTable(SettingsEdit)
	//*)
END_EVENT_TABLE()

SettingsEdit::SettingsEdit(wxWindow* parent, const DataModel& data, const Settings* s, wxWindowID,const wxPoint&,const wxSize&)
{
	//(*Initialize(SettingsEdit)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer6;
	wxStaticBoxSizer* sbsAutoCorrection;
	wxStaticBoxSizer* sbsInterface;
	wxStaticBoxSizer* sbsLogging;
	wxStaticBoxSizer* sbsOverride;

	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(this, lblPath, _("Path:"), wxDefaultPosition, wxDefaultSize, 0, _T("lblPath"));
	BoxSizer2->Add(StaticText1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	cbPath = new wxComboBox(this, ID_COMBOBOX1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX1"));
	BoxSizer2->Add(cbPath, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	btnBrowse = new wxButton(this, bBrowse, _("Browse..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bBrowse"));
	BoxSizer2->Add(btnBrowse, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	wxString __wxRadioBoxChoices_1[2] =
	{
		_("Fallout"),
		_("Fallout 2")
	};
	rbGame = new wxRadioBox(this, rboxGame, _("Game"), wxDefaultPosition, wxDefaultSize, 2, __wxRadioBoxChoices_1, 2, 0, wxDefaultValidator, _T("rboxGame"));
	rbGame->SetSelection(1);
	BoxSizer2->Add(rbGame, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer4 = new wxBoxSizer(wxVERTICAL);
	sbsInterface = new wxStaticBoxSizer(wxVERTICAL, this, _("Interface language"));
	lbLngFiles = new wxListBox(this, ID_LISTBOX1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
	sbsInterface->Add(lbLngFiles, 0, wxALL|wxEXPAND, 5);
	BoxSizer4->Add(sbsInterface, 1, wxTOP|wxBOTTOM|wxRIGHT|wxEXPAND, 5);
	sbsLogging = new wxStaticBoxSizer(wxVERTICAL, this, _("Logging"));
	cbntst_log_info = new wxCheckBox(this, cboxLogInfo, _("Log info messages"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxLogInfo"));
	cbntst_log_info->SetValue(false);
	sbsLogging->Add(cbntst_log_info, 0, wxALIGN_LEFT, 5);
	cbntst_log_warning = new wxCheckBox(this, cboxLogWarning, _("Log warnings"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxLogWarning"));
	cbntst_log_warning->SetValue(true);
	sbsLogging->Add(cbntst_log_warning, 0, wxALIGN_LEFT, 5);
	cbntst_log_error = new wxCheckBox(this, cboxLogError, _("Log errors"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxLogError"));
	cbntst_log_error->SetValue(true);
	sbsLogging->Add(cbntst_log_error, 0, wxALIGN_LEFT, 5);
	BoxSizer4->Add(sbsLogging, 0, wxBOTTOM|wxRIGHT|wxEXPAND, 5);
	BoxSizer3->Add(BoxSizer4, 1, wxALL|wxEXPAND, 0);
	sbsOverride = new wxStaticBoxSizer(wxVERTICAL, this, _("Override"));
	lblFalloutExe = new wxStaticText(this, lblOverrideFalloutExe, _("fallout\?.exe"), wxDefaultPosition, wxDefaultSize, 0, _T("lblOverrideFalloutExe"));
	sbsOverride->Add(lblFalloutExe, 0, wxALL|wxALIGN_LEFT, 2);
	txtFalloutExe = new wxTextCtrl(this, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	sbsOverride->Add(txtFalloutExe, 0, wxALL|wxEXPAND, 2);
	lblFalloutCfg = new wxStaticText(this, lblOverrideFalloutCfg, _("fallout\?.cfg"), wxDefaultPosition, wxDefaultSize, 0, _T("lblOverrideFalloutCfg"));
	sbsOverride->Add(lblFalloutCfg, 0, wxALL|wxALIGN_LEFT, 2);
	txtFalloutCfg = new wxTextCtrl(this, ID_TEXTCTRL4, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	sbsOverride->Add(txtFalloutCfg, 0, wxALL|wxEXPAND, 2);
	lblFalloutLanguage = new wxStaticText(this, lblOverrideFalloutLanguage, _("fallout\?.cfg [system] language"), wxDefaultPosition, wxDefaultSize, 0, _T("lblOverrideFalloutLanguage"));
	sbsOverride->Add(lblFalloutLanguage, 0, wxALL|wxALIGN_LEFT, 2);
	txtMsgDir = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	sbsOverride->Add(txtMsgDir, 0, wxALL|wxEXPAND, 2);
	BoxSizer3->Add(sbsOverride, 0, wxALL|wxEXPAND, 5);
	sbsAutoCorrection = new wxStaticBoxSizer(wxVERTICAL, this, _("Auto correction"));
	cbCorItemFid = new wxCheckBox(this, cboxCorItemFid, _("Item\'s FID"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxCorItemFid"));
	cbCorItemFid->SetValue(true);
	sbsAutoCorrection->Add(cbCorItemFid, 0, wxALL|wxALIGN_LEFT, 2);
	cbCorAmmoT = new wxCheckBox(this, cboxCorAmmoT, _("Ammo type"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxCorAmmoT"));
	cbCorAmmoT->SetValue(true);
	sbsAutoCorrection->Add(cbCorAmmoT, 0, wxALL|wxALIGN_LEFT, 2);
	cbCorAmmoQ = new wxCheckBox(this, cboxCorAmmoQ, _("Ammo quantity"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxCorAmmoQ"));
	cbCorAmmoQ->SetValue(true);
	sbsAutoCorrection->Add(cbCorAmmoQ, 0, wxALL|wxALIGN_LEFT, 2);
	cbCorPerks = new wxCheckBox(this, cboxCorPerks, _("Perks"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxCorPerks"));
	cbCorPerks->SetValue(true);
	sbsAutoCorrection->Add(cbCorPerks, 0, wxALL|wxALIGN_LEFT, 2);
	cbCorTraits = new wxCheckBox(this, cboxCorTraits, _("Traits"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxCorTraits"));
	cbCorTraits->SetValue(true);
	sbsAutoCorrection->Add(cbCorTraits, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer3->Add(sbsAutoCorrection, 0, wxALL|wxEXPAND, 5);
	BoxSizer5 = new wxBoxSizer(wxVERTICAL);
	StaticText2 = new wxStaticText(this, lblBrightness, _("Images brightness"), wxDefaultPosition, wxDefaultSize, 0, _T("lblBrightness"));
	BoxSizer5->Add(StaticText2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 2);
	sbBrightness = new wxScrollBar(this, ID_SCROLLBAR1, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL, wxDefaultValidator, _T("ID_SCROLLBAR1"));
	sbBrightness->SetScrollbar(0, 1, 101, 1);
	BoxSizer5->Add(sbBrightness, 0, wxALL|wxEXPAND, 2);
	wxString __wxRadioBoxChoices_2[3] =
	{
		_("Nearest"),
		_("Bilinear"),
		_("Bicubic")
	};
	rbImagesResize = new wxRadioBox(this, rboxImagesResize, _("Images resize algorithm"), wxDefaultPosition, wxDefaultSize, 3, __wxRadioBoxChoices_2, 1, 0, wxDefaultValidator, _T("rboxImagesResize"));
	rbImagesResize->SetSelection(1);
	BoxSizer5->Add(rbImagesResize, 0, wxALL|wxEXPAND, 2);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	btnCancel = new wxButton(this, bCancel, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bCancel"));
	BoxSizer6->Add(btnCancel, 1, wxEXPAND, 5);
	btnOk = new wxButton(this, bOk, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bOk"));
	BoxSizer6->Add(btnOk, 1, wxEXPAND, 5);
	BoxSizer5->Add(BoxSizer6, 1, wxALL|wxEXPAND, 5);
	BoxSizer3->Add(BoxSizer5, 0, wxEXPAND, 0);
	BoxSizer1->Add(BoxSizer3, 1, wxBOTTOM|wxLEFT|wxRIGHT, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_COMBOBOX1,wxEVT_COMMAND_COMBOBOX_SELECTED,(wxObjectEventFunction)&SettingsEdit::OncbPathSelected);
	Connect(bBrowse,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SettingsEdit::OnbtnBrowseClick);
	Connect(bCancel,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SettingsEdit::OnbtnCancelClick);
	Connect(bOk,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SettingsEdit::OnbtnOkClick);
	//*)
    Bind(wxEVT_CHAR_HOOK, &SettingsEdit::OnChar, this);
	sbsAutoCorrection->GetStaticBox()->SetName("sbAutoCorrection");
	sbsInterface->GetStaticBox()->SetName("sbInterface");
	sbsLogging->GetStaticBox()->SetName("sbLogging");
	sbsOverride->GetStaticBox()->SetName("sbsOverride");
	lbLngFiles->Append("Default");
	std::vector<std::string> lngFiles = GetFiles(data.GetDirLng(), LANGUAGE_FILE_MASK);
    for (size_t i = 0; i < lngFiles.size(); ++i)
        lbLngFiles->Append(lngFiles[i]);
    std::vector<FalloutDir> found = FindFalloutDirs(GetApplicationDir());
    if (s == NULL)
        {
        ss.TryToGuessLng();
        int lngIdx = lbLngFiles->FindString(ss.lngFile);
        if (lngIdx != wxNOT_FOUND)
            {
            lbLngFiles->SetSelection(lngIdx);
            }
        }
    else
        {
        falloutDirs.push_back(FalloutDir(s->gamePath, s->isFallout1));
        SetSettings(*s);
        }
    for (std::vector<FalloutDir>::const_iterator it = found.begin(); it != found.end(); ++it)
        {
        if (std::find(falloutDirs.begin(), falloutDirs.end(), *it) == falloutDirs.end())
            falloutDirs.push_back(*it);
        }
    for (std::vector<FalloutDir>::const_iterator it = falloutDirs.begin(); it != falloutDirs.end(); ++it)
        {
        cbPath->Append(it->path);
        }
    if (!falloutDirs.empty())
        {
        cbPath->SetSelection(0);
        OncbPathIndexChanged(0);
        }
}

SettingsEdit::~SettingsEdit()
{
	//(*Destroy(SettingsEdit)
	//*)
}


void SettingsEdit::SetSettings(const Settings& s)
    {
    ss = s;
    txtFalloutExe->SetValue(s.falloutExeOverride);
    txtFalloutCfg->SetValue(s.falloutCfgOverride);
    txtMsgDir->SetValue(s.falloutLngDirOverride);
    cbCorItemFid->SetValue(s.correctItemFID);
    cbCorAmmoT->SetValue(s.correctAmmoType);
    cbCorAmmoQ->SetValue(s.correctAmmoQuantity);
    cbCorPerks->SetValue(s.correctPerks);
    cbCorTraits->SetValue(s.correctTraits);
    unsigned val = s.brightness;
    if (val > 100)
        val = 100;
    sbBrightness->SetThumbPosition(val);

    //cbLoadFromFile->SetValue(ss.localize);
    if (ss.NeedLocalize())
        lbLngFiles->SetSelection(lbLngFiles->FindString(ss.lngFile));
    else
        lbLngFiles->SetSelection(0);

    cbntst_log_info->SetValue(s.logMode & NTST_LOGM_INFO);
    cbntst_log_warning->SetValue(s.logMode & NTST_LOGM_WARNING);
    cbntst_log_error->SetValue(s.logMode & NTST_LOGM_ERROR);

    if ((unsigned)ss.imageResizeQuality < rbImagesResize->GetCount())
        rbImagesResize->SetSelection(ss.imageResizeQuality);
    }

Settings SettingsEdit::GetSettings()
    {
    ss.gamePath = cbPath->GetValue();
    ss.isFallout1 = Fallout1Selected();
    ss.falloutExeOverride = txtFalloutExe->GetValue();
    ss.falloutCfgOverride = txtFalloutCfg->GetValue();
    ss.falloutLngDirOverride = txtMsgDir->GetValue();
    ss.correctItemFID = cbCorItemFid->GetValue();
    ss.correctAmmoType = cbCorAmmoT->GetValue();
    ss.correctAmmoQuantity = cbCorAmmoQ->GetValue();
    ss.correctPerks = cbCorPerks->GetValue();
    ss.correctTraits = cbCorTraits->GetValue();
    ss.brightness = sbBrightness->GetThumbPosition();
    const int lngSelection = lbLngFiles->GetSelection();
    const bool lngSelected = (lngSelection != wxNOT_FOUND);
    //ss.localize = lngSelected && (lngSelection > 0);
    if (lngSelected && lngSelection != 0) // 0 is default == no need to translate
        ss.lngFile = WxStringToStdLocal(lbLngFiles->GetString(lngSelection));
    else
        ss.lngFile.clear();
    int logmode = 0;
    if (cbntst_log_info->IsChecked())
        logmode |= NTST_LOGM_INFO;
    if (cbntst_log_warning->IsChecked())
        logmode |= NTST_LOGM_WARNING;
    if (cbntst_log_error->IsChecked())
        logmode |= NTST_LOGM_ERROR;
    ss.logMode = logmode;
    ss.imageResizeQuality = rbImagesResize->GetSelection();
    return ss;
    }

void SettingsEdit::OnbtnOkClick(wxCommandEvent&)
    {
    const std::string path = WxStringToStdLocal(cbPath->GetValue());
    if (Fallout1Selected())
        {
        if (DirLooksLikeFallout2(path))
            ntst_log_warning(path + " looks like Fallout2, but Fallout1 type is selected.");
        }
    else
        {
        if (DirLooksLikeFallout1(path))
            ntst_log_warning(path + " looks like Fallout1, but Fallout2 type is selected.");
        }
    EndModal(wxID_OK);
    }

void SettingsEdit::OnbtnCancelClick(wxCommandEvent&)
    {
    EndModal(wxID_CANCEL);
    }

void SettingsEdit::OnbtnBrowseClick(wxCommandEvent&)
    {
    std::string path = WxStringToStdLocal(cbPath->GetValue());
    if (BrowseDialog("", GetHandle(), path))
        {
        cbPath->SetValue(path);
        if (DirLooksLikeFallout1(path))
            rbGame->SetSelection(0);
        else
            rbGame->SetSelection(1);
        }
    }

void SettingsEdit::OnChar(wxKeyEvent& event)
    {
    if (event.GetKeyCode() == WXK_ESCAPE)
        EndModal(wxID_CANCEL);
    else
        event.Skip();
    }

void SettingsEdit::OncbPathSelected(wxCommandEvent&)
    {
    OncbPathIndexChanged(cbPath->GetSelection());
    }

void SettingsEdit::OncbPathIndexChanged(int idx)
    {
    if (idx >= 0 && idx < (int)falloutDirs.size())
        {
        if (falloutDirs[idx].isFallout1)
            rbGame->SetSelection(0);
        else
            rbGame->SetSelection(1);
        }
    }
