/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifdef _MSC_VER
// C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable: 4800)
#endif

#define TITLE_BEGIN "F12se - "

#define SECTION_MAINWINDOW "Main"
#define SECTION_SETTINGSEDIT "SettingsEdit"
#define SECTION_SAVEDIT "SavEdit"
#define SECTION_ITEMEDIT "ItemEdit"
#define SECTION_CRITTERSEDIT "CrittersEdit"

#include "F2seMain.h"
#include "SAV.h"
#include "SavEdit.h"
#include "SettingsEdit.h"
#include "ItemEdit.h"
#include "CrittersEdit.h"
#include "gam.h"
#include "WinFunc.h"
#include "Common.h"

#include <wx/msgdlg.h>
#include <wx/mstream.h>
#include <wx/treebase.h>
#include <wx/version.h>

#if (wxMAJOR_VERSION == 3) && (wxMINOR_VERSION == 1) && !wxUSE_RICHEDIT
wxMenu *wxTextCtrl::MSWCreateContextMenu()
{
    wxMenu *m = new wxMenu;
    m->Append(wxID_UNDO, _("&Undo"));
    m->Append(wxID_REDO, _("&Redo"));
    m->AppendSeparator();
    m->Append(wxID_CUT, _("Cu&t"));
    m->Append(wxID_COPY, _("&Copy"));
    m->Append(wxID_PASTE, _("&Paste"));
    m->Append(wxID_CLEAR, _("&Delete"));
    m->AppendSeparator();
    m->Append(wxID_SELECTALL, _("Select &All"));
    return m;
}
#endif

class ItemTreeData : public wxTreeItemData
{
public:
    ItemTreeData(unsigned offset) : o(offset) {}
    unsigned GetOffset() const {return o;}
private:
    unsigned o;
};

class ArrayTreeData : public wxTreeItemData
{
public:
    ArrayTreeData(unsigned arrayIdx, int elemIdx = -1) : a(arrayIdx), e(elemIdx) {}
    unsigned GetArrayIdx() const {return a;}
    int GetElemIdx() const {return e;}
private:
    unsigned a; /**< Index for SFallGV::arraysNew */
    int e; /**< Index for SFallGV::arraysNew::data -1 if data for array and not an element */
};

//(*InternalHeaders(F12seFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(F12seFrame)
const long F12seFrame::ID_LISTBOX1 = wxNewId();
const long F12seFrame::bRefresh = wxNewId();
const long F12seFrame::bSave = wxNewId();
const long F12seFrame::lblSavegame = wxNewId();
const long F12seFrame::ID_TEXTCTRL2 = wxNewId();
const long F12seFrame::lblPlayer = wxNewId();
const long F12seFrame::ID_TEXTCTRL3 = wxNewId();
const long F12seFrame::ID_STATICLINE1 = wxNewId();
const long F12seFrame::lblDate = wxNewId();
const long F12seFrame::ID_TEXTCTRL5 = wxNewId();
const long F12seFrame::ID_CHOICE1 = wxNewId();
const long F12seFrame::ID_CHOICE2 = wxNewId();
const long F12seFrame::ID_TEXTCTRL6 = wxNewId();
const long F12seFrame::lblHM = wxNewId();
const long F12seFrame::ID_TEXTCTRL7 = wxNewId();
const long F12seFrame::lblMS = wxNewId();
const long F12seFrame::ID_TEXTCTRL8 = wxNewId();
const long F12seFrame::lblST = wxNewId();
const long F12seFrame::ID_TEXTCTRL9 = wxNewId();
const long F12seFrame::lblTicks = wxNewId();
const long F12seFrame::ID_TEXTCTRL4 = wxNewId();
const long F12seFrame::ID_CUSTOM2 = wxNewId();
const long F12seFrame::ID_PANEL3 = wxNewId();
const long F12seFrame::ID_LISTBOX4 = wxNewId();
const long F12seFrame::ID_TEXTCTRL12 = wxNewId();
const long F12seFrame::lblValueGvar = wxNewId();
const long F12seFrame::ID_TEXTCTRL14 = wxNewId();
const long F12seFrame::ID_STATICLINE2 = wxNewId();
const long F12seFrame::ID_TEXTCTRL13 = wxNewId();
const long F12seFrame::bFindGvar = wxNewId();
const long F12seFrame::ID_LISTBOX2 = wxNewId();
const long F12seFrame::ID_LISTBOX3 = wxNewId();
const long F12seFrame::lblQLocation = wxNewId();
const long F12seFrame::ID_CHOICE3 = wxNewId();
const long F12seFrame::lblQDescription = wxNewId();
const long F12seFrame::ID_CHOICE4 = wxNewId();
const long F12seFrame::lblDisplayThr = wxNewId();
const long F12seFrame::ID_TEXTCTRL10 = wxNewId();
const long F12seFrame::lblCompletedThr = wxNewId();
const long F12seFrame::ID_TEXTCTRL11 = wxNewId();
const long F12seFrame::ID_PANEL4 = wxNewId();
const long F12seFrame::lblSavList = wxNewId();
const long F12seFrame::ID_LISTBOX7 = wxNewId();
const long F12seFrame::bEditSAV = wxNewId();
const long F12seFrame::bDelSav = wxNewId();
const long F12seFrame::lblCities = wxNewId();
const long F12seFrame::ID_LISTBOX8 = wxNewId();
const long F12seFrame::lblSelectedCity = wxNewId();
const long F12seFrame::ID_LISTBOX9 = wxNewId();
const long F12seFrame::lblValueCity = wxNewId();
const long F12seFrame::ID_TEXTCTRL17 = wxNewId();
const long F12seFrame::lblWorldX = wxNewId();
const long F12seFrame::ID_TEXTCTRL18 = wxNewId();
const long F12seFrame::lblWorldY = wxNewId();
const long F12seFrame::ID_TEXTCTRL19 = wxNewId();
const long F12seFrame::lblCarMap = wxNewId();
const long F12seFrame::ID_CHOICE5 = wxNewId();
const long F12seFrame::lblCarFuel = wxNewId();
const long F12seFrame::ID_TEXTCTRL20 = wxNewId();
const long F12seFrame::ID_CHOICE12 = wxNewId();
const long F12seFrame::bWorlmapSet = wxNewId();
const long F12seFrame::ID_PANEL7 = wxNewId();
const long F12seFrame::ID_TREECTRL1 = wxNewId();
const long F12seFrame::bItemEdit = wxNewId();
const long F12seFrame::bItemCreate = wxNewId();
const long F12seFrame::bItemDelete = wxNewId();
const long F12seFrame::lblQuantity = wxNewId();
const long F12seFrame::ID_TEXTCTRL21 = wxNewId();
const long F12seFrame::ID_STATICBITMAP2 = wxNewId();
const long F12seFrame::ID_PANEL6 = wxNewId();
const long F12seFrame::lblStatsBase = wxNewId();
const long F12seFrame::ID_LISTBOX5 = wxNewId();
const long F12seFrame::lblValueStatBase = wxNewId();
const long F12seFrame::ID_TEXTCTRL15 = wxNewId();
const long F12seFrame::lblStatsBonus = wxNewId();
const long F12seFrame::ID_LISTBOX6 = wxNewId();
const long F12seFrame::lblValueStatBonus = wxNewId();
const long F12seFrame::ID_TEXTCTRL16 = wxNewId();
const long F12seFrame::ID_PANEL5 = wxNewId();
const long F12seFrame::ID_CHOICE6 = wxNewId();
const long F12seFrame::ID_CHOICE7 = wxNewId();
const long F12seFrame::ID_CHOICE8 = wxNewId();
const long F12seFrame::ID_CHOICE9 = wxNewId();
const long F12seFrame::ID_LISTBOX12 = wxNewId();
const long F12seFrame::lblValueSkill = wxNewId();
const long F12seFrame::ID_TEXTCTRL23 = wxNewId();
const long F12seFrame::ID_LISTBOX11 = wxNewId();
const long F12seFrame::lblValuePerk = wxNewId();
const long F12seFrame::ID_TEXTCTRL22 = wxNewId();
const long F12seFrame::cboxPickPerk = wxNewId();
const long F12seFrame::ID_PANEL10 = wxNewId();
const long F12seFrame::ID_LISTBOX13 = wxNewId();
const long F12seFrame::lblValueKill = wxNewId();
const long F12seFrame::ID_TEXTCTRL24 = wxNewId();
const long F12seFrame::lblExperience = wxNewId();
const long F12seFrame::ID_TEXTCTRL25 = wxNewId();
const long F12seFrame::lblLevel = wxNewId();
const long F12seFrame::ID_TEXTCTRL26 = wxNewId();
const long F12seFrame::lblSkillPoints = wxNewId();
const long F12seFrame::ID_TEXTCTRL27 = wxNewId();
const long F12seFrame::lblHitpoints = wxNewId();
const long F12seFrame::ID_TEXTCTRL28 = wxNewId();
const long F12seFrame::lblRadiation = wxNewId();
const long F12seFrame::ID_TEXTCTRL29 = wxNewId();
const long F12seFrame::lblPoison = wxNewId();
const long F12seFrame::ID_TEXTCTRL30 = wxNewId();
const long F12seFrame::ID_CHOICE10 = wxNewId();
const long F12seFrame::ID_CHOICE11 = wxNewId();
const long F12seFrame::lblLocalX = wxNewId();
const long F12seFrame::ID_TEXTCTRL32 = wxNewId();
const long F12seFrame::lblLocalY = wxNewId();
const long F12seFrame::ID_TEXTCTRL33 = wxNewId();
const long F12seFrame::lblLocalMapLevel = wxNewId();
const long F12seFrame::ID_TEXTCTRL31 = wxNewId();
const long F12seFrame::cboxCrippledEye = wxNewId();
const long F12seFrame::cboxCrippledRArm = wxNewId();
const long F12seFrame::cboxCrippledLArm = wxNewId();
const long F12seFrame::cboxCrippledRLeg = wxNewId();
const long F12seFrame::cboxCrippledLLeg = wxNewId();
const long F12seFrame::cboxGhostMode = wxNewId();
const long F12seFrame::bClearCombat = wxNewId();
const long F12seFrame::bEditCritters = wxNewId();
const long F12seFrame::bResetGvars = wxNewId();
const long F12seFrame::bResetEntrances = wxNewId();
const long F12seFrame::ID_PANEL8 = wxNewId();
const long F12seFrame::bSaveSFallgv = wxNewId();
const long F12seFrame::ID_LISTBOX14 = wxNewId();
const long F12seFrame::bSFallGvarAdd = wxNewId();
const long F12seFrame::bSFallGvarDel = wxNewId();
const long F12seFrame::lblValueSFallGvar = wxNewId();
const long F12seFrame::ID_TEXTCTRL34 = wxNewId();
const long F12seFrame::ID_TREECTRL2 = wxNewId();
const long F12seFrame::ID_STATICTEXT1 = wxNewId();
const long F12seFrame::ID_TEXTCTRL39 = wxNewId();
const long F12seFrame::ID_RADIOBOX1 = wxNewId();
const long F12seFrame::ID_LISTBOX15 = wxNewId();
const long F12seFrame::ID_TEXTCTRL35 = wxNewId();
const long F12seFrame::ID_LISTBOX16 = wxNewId();
const long F12seFrame::ID_TEXTCTRL36 = wxNewId();
const long F12seFrame::ID_LISTBOX17 = wxNewId();
const long F12seFrame::ID_TEXTCTRL37 = wxNewId();
const long F12seFrame::ID_PANEL11 = wxNewId();
const long F12seFrame::ID_LISTBOX10 = wxNewId();
const long F12seFrame::ID_CUSTOM1 = wxNewId();
const long F12seFrame::ID_PANEL9 = wxNewId();
const long F12seFrame::nbPages = wxNewId();
const long F12seFrame::ID_PANEL1 = wxNewId();
const long F12seFrame::ID_TEXTCTRL1 = wxNewId();
const long F12seFrame::ID_PANEL2 = wxNewId();
const long F12seFrame::ID_SPLITTERWINDOW1 = wxNewId();
const long F12seFrame::ID_Open = wxNewId();
const long F12seFrame::ID_Save = wxNewId();
const long F12seFrame::ID_SaveAs = wxNewId();
const long F12seFrame::ID_Settings = wxNewId();
const long F12seFrame::ID_Quit = wxNewId();
const long F12seFrame::ID_Armor = wxNewId();
const long F12seFrame::ID_Container = wxNewId();
const long F12seFrame::ID_Drug = wxNewId();
const long F12seFrame::ID_Weapon = wxNewId();
const long F12seFrame::ID_Ammo = wxNewId();
const long F12seFrame::ID_Miscellaneous = wxNewId();
const long F12seFrame::ID_Key = wxNewId();
//*)

BEGIN_EVENT_TABLE(F12seFrame,wxFrame)
    //(*EventTable(F12seFrame)
    //*)
    EVT_MENU(ID_Open, F12seFrame::OnOpen)
    EVT_MENU(ID_Save, F12seFrame::OnSave)
    EVT_MENU(ID_SaveAs, F12seFrame::OnSaveAs)
    EVT_MENU(ID_Settings, F12seFrame::OnSettings)
    EVT_MENU(ID_Quit, F12seFrame::OnQuit)

    EVT_MENU(ID_Armor, F12seFrame::OnCreateArmor)
    EVT_MENU(ID_Container, F12seFrame::OnCreateContainer)
    EVT_MENU(ID_Drug, F12seFrame::OnCreateDrug)
    EVT_MENU(ID_Weapon, F12seFrame::OnCreateWeapon)
    EVT_MENU(ID_Ammo, F12seFrame::OnCreateAmmo)
    EVT_MENU(ID_Miscellaneous, F12seFrame::OnCreateMiscellaneous)
    EVT_MENU(ID_Key, F12seFrame::OnCreateKey)
END_EVENT_TABLE()

void F12seFrame::GetSettingsAndApply(const Settings *s)
    {
    SettingsEdit dlg(this, datamodel, s);
    if (datamodel.settings.NeedLocalize())
        localization.LocalizeWindow(&dlg, SECTION_SETTINGSEDIT);
    dlg.Layout();
    dlg.Fit(); // without it translated rbImagesResize will look bad.
    if (dlg.ShowModal() == wxID_OK)
        {
        const Settings& dlgs = dlg.GetSettings();
        ntst_log_set_mode(dlgs.logMode);
        datamodel.InitAll(&dlgs);
        }
    }

void F12seFrame::OnOpen(wxCommandEvent&)
    {
    std::string path;
    if (OpenFileDialog(GetHandle(), datamodel.GetSaveDir(), SAVE_MAIN_FILE_NAME, path))
        LoadSaveDat(path);
    }

void F12seFrame::OnSave(wxCommandEvent&)
    {
    SaveSaveDat(datamodel.curSave.GetPath());
    }

void F12seFrame::OnSaveAs(wxCommandEvent&)
    {
    std::string path;
    if (SaveFileDialog(GetHandle(), datamodel.curSave.GetDir(), SAVE_MAIN_FILE_NAME, path))
        SaveSaveDat(path);
    }

void F12seFrame::OnSettings(wxCommandEvent&)
    {
    GetSettingsAndApply(&datamodel.settings);
    }

#ifndef _MSC_VER
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif // _MSC_VER

//F12seFrame* Frame;
void LogCallback(const char* str, size_t str_len, ntst_log_modes mode, const ntst_time_t* nt, void* userData)
    {
    F12seFrame* frame = (F12seFrame*)userData;
    std::string s = ntst_log_param_to_str(str, str_len, mode, nt, true);
    // string may be in local encoding.
    if (!StringIsValidUtf8(s))
        s = WxStringToStdUTF8(wxString(s));
    s += "\r\n";
    frame->AddLogStr(s);
    }

#define REFRESH_FUNCTION(FUNCTION_NAME) \
void FUNCTION_NAME(MemBufferEX& buf, size_t offset, size_t count, void* userData) \
    { \
    F12seFrame* frame = (F12seFrame*)userData; \
    frame->FUNCTION_NAME(buf, offset, count); \
    }

REFRESH_FUNCTION(RefreshHeader)
REFRESH_FUNCTION(RefreshF2)
REFRESH_FUNCTION(RefreshF3)
REFRESH_FUNCTION(RefreshF5)
REFRESH_FUNCTION(RefreshF6)
REFRESH_FUNCTION(RefreshF7)
REFRESH_FUNCTION(RefreshF8)
REFRESH_FUNCTION(RefreshF10)
REFRESH_FUNCTION(RefreshF13)
REFRESH_FUNCTION(RefreshF15)
REFRESH_FUNCTION(RefreshF18)
REFRESH_FUNCTION(RefreshF19)

const char* functions[] =
    {
    "Header", "0-Unused", "1-Unknown", "2-GVARS", "3-Maps", "4-GVARS",
    "5-Player and inventory", "6-Player stats", "7-Kill counts", "8-Tag skills", "9-Unused", "10-Perks",
    "11-Combat", "12-Party AI", "13-Experience and level", "14-Unused", "15-Traits", "16-Unknown",
    "17-Preferences", "18-Character window", "19-World map", "20-Unused", "21-Video Archives", "22-Skill use",
    "23-Party", "24-Event queue", "25-Interface", "26-Unused", "Not loaded data"
    };

wxString worldMapStates[] = {
"Visible",
"Partly visible",
"Hidden"
};

#define FILECRC_CHANGED "Crc32 of file was changed after it was loaded. Save anyway?"
#define SFALL_NEW_GVAR_MESSAGE "Enter new id as a hexadecimal number(0x...) or a string."

F12seFrame::F12seFrame(wxWindow* parent,wxWindowID id) : flog(NULL), numberOfTilesOffset(0)
{
    //(*Initialize(F12seFrame)
    wxBoxSizer* BoxSizer10;
    wxBoxSizer* BoxSizer11;
    wxBoxSizer* BoxSizer12;
    wxBoxSizer* BoxSizer13;
    wxBoxSizer* BoxSizer14;
    wxBoxSizer* BoxSizer15;
    wxBoxSizer* BoxSizer16;
    wxBoxSizer* BoxSizer17;
    wxBoxSizer* BoxSizer18;
    wxBoxSizer* BoxSizer19;
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer20;
    wxBoxSizer* BoxSizer21;
    wxBoxSizer* BoxSizer22;
    wxBoxSizer* BoxSizer23;
    wxBoxSizer* BoxSizer24;
    wxBoxSizer* BoxSizer25;
    wxBoxSizer* BoxSizer26;
    wxBoxSizer* BoxSizer27;
    wxBoxSizer* BoxSizer28;
    wxBoxSizer* BoxSizer29;
    wxBoxSizer* BoxSizer2;
    wxBoxSizer* BoxSizer30;
    wxBoxSizer* BoxSizer31;
    wxBoxSizer* BoxSizer32;
    wxBoxSizer* BoxSizer33;
    wxBoxSizer* BoxSizer34;
    wxBoxSizer* BoxSizer35;
    wxBoxSizer* BoxSizer36;
    wxBoxSizer* BoxSizer37;
    wxBoxSizer* BoxSizer38;
    wxBoxSizer* BoxSizer39;
    wxBoxSizer* BoxSizer3;
    wxBoxSizer* BoxSizer40;
    wxBoxSizer* BoxSizer41;
    wxBoxSizer* BoxSizer42;
    wxBoxSizer* BoxSizer4;
    wxBoxSizer* BoxSizer5;
    wxBoxSizer* BoxSizer6;
    wxBoxSizer* BoxSizer7;
    wxBoxSizer* BoxSizer8;
    wxBoxSizer* BoxSizer9;
    wxFlexGridSizer* FlexGridSizer1;
    wxFlexGridSizer* FlexGridSizer2;
    wxStaticBoxSizer* sbsBodyFlags;
    wxStaticBoxSizer* sbsBugFixes;
    wxStaticBoxSizer* sbsCar;
    wxStaticBoxSizer* sbsCurPos;
    wxStaticBoxSizer* sbsGameReset;
    wxStaticBoxSizer* sbsHolodisks;
    wxStaticBoxSizer* sbsKarmas;
    wxStaticBoxSizer* sbsKills;
    wxStaticBoxSizer* sbsLocalMap;
    wxStaticBoxSizer* sbsPerks;
    wxStaticBoxSizer* sbsPlayer;
    wxStaticBoxSizer* sbsQuests;
    wxStaticBoxSizer* sbsSFallArrays;
    wxStaticBoxSizer* sbsSFallGvars;
    wxStaticBoxSizer* sbsSFallPerks;
    wxStaticBoxSizer* sbsSFallSelectablePerks;
    wxStaticBoxSizer* sbsSFallTraits;
    wxStaticBoxSizer* sbsSkills;
    wxStaticBoxSizer* sbsTag;
    wxStaticBoxSizer* sbsTraits;
    wxStaticBoxSizer* sbsWorldmapState;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(640,480));
    SplitterWindow1 = new wxSplitterWindow(this, ID_SPLITTERWINDOW1, wxDefaultPosition, wxDefaultSize, wxSP_3D, _T("ID_SPLITTERWINDOW1"));
    SplitterWindow1->SetMinimumPaneSize(20);
    SplitterWindow1->SetSashGravity(0.8);
    pnlMain = new wxPanel(SplitterWindow1, ID_PANEL1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_PANEL1"));
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer3 = new wxBoxSizer(wxVERTICAL);
    lbSaves = new wxListBox(pnlMain, ID_LISTBOX1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
    BoxSizer3->Add(lbSaves, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    btnRefresh = new wxButton(pnlMain, bRefresh, _("Refresh"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bRefresh"));
    BoxSizer3->Add(btnRefresh, 0, wxALL|wxEXPAND, 5);
    btnSave = new wxButton(pnlMain, bSave, _("Save"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bSave"));
    btnSave->Disable();
    BoxSizer3->Add(btnSave, 0, wxALL|wxEXPAND, 5);
    BoxSizer2->Add(BoxSizer3, 0, wxALL|wxEXPAND, 5);
    Notebook1 = new wxNotebook(pnlMain, nbPages, wxDefaultPosition, wxDefaultSize, 0, _T("nbPages"));
    pnlHeader = new wxPanel(Notebook1, ID_PANEL3, wxDefaultPosition, wxDefaultSize, 0, _T("ID_PANEL3"));
    BoxSizer4 = new wxBoxSizer(wxVERTICAL);
    BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer6 = new wxBoxSizer(wxVERTICAL);
    StaticText1 = new wxStaticText(pnlHeader, lblSavegame, _("Savegame name"), wxDefaultPosition, wxDefaultSize, 0, _T("lblSavegame"));
    BoxSizer6->Add(StaticText1, 0, wxALL|wxALIGN_LEFT, 2);
    txtSaveName = new wxTextCtrl(pnlHeader, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    txtSaveName->SetMaxLength(29);
    BoxSizer6->Add(txtSaveName, 0, wxALL|wxALIGN_LEFT, 2);
    StaticText2 = new wxStaticText(pnlHeader, lblPlayer, _("Player name"), wxDefaultPosition, wxDefaultSize, 0, _T("lblPlayer"));
    BoxSizer6->Add(StaticText2, 0, wxALL|wxALIGN_LEFT, 2);
    txtPlayerName = new wxTextCtrl(pnlHeader, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
    txtPlayerName->SetMaxLength(31);
    BoxSizer6->Add(txtPlayerName, 0, wxALL|wxALIGN_LEFT, 2);
    BoxSizer5->Add(BoxSizer6, 0, wxALIGN_TOP, 5);
    StaticLine1 = new wxStaticLine(pnlHeader, ID_STATICLINE1, wxDefaultPosition, wxSize(1,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
    BoxSizer5->Add(StaticLine1, 0, wxALL|wxEXPAND, 5);
    BoxSizer7 = new wxBoxSizer(wxVERTICAL);
    StaticText4 = new wxStaticText(pnlHeader, lblDate, _("The date in the game"), wxDefaultPosition, wxDefaultSize, 0, _T("lblDate"));
    BoxSizer7->Add(StaticText4, 0, wxALIGN_LEFT, 5);
    BoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    txtDay = new wxTextCtrl(pnlHeader, ID_TEXTCTRL5, wxEmptyString, wxDefaultPosition, wxSize(21,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
    txtDay->SetMaxLength(2);
    BoxSizer8->Add(txtDay, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    chMonths = new wxChoice(pnlHeader, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
    BoxSizer8->Add(chMonths, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    chYears = new wxChoice(pnlHeader, ID_CHOICE2, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE2"));
    BoxSizer8->Add(chYears, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer7->Add(BoxSizer8, 0, wxALIGN_LEFT, 0);
    BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    txtHour = new wxTextCtrl(pnlHeader, ID_TEXTCTRL6, wxEmptyString, wxDefaultPosition, wxSize(21,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
    txtHour->SetMaxLength(2);
    BoxSizer9->Add(txtHour, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText5 = new wxStaticText(pnlHeader, lblHM, _T(":"), wxDefaultPosition, wxDefaultSize, 0, _T("lblHM"));
    BoxSizer9->Add(StaticText5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
    txtMinute = new wxTextCtrl(pnlHeader, ID_TEXTCTRL7, wxEmptyString, wxDefaultPosition, wxSize(21,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL7"));
    txtMinute->SetMaxLength(2);
    BoxSizer9->Add(txtMinute, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText6 = new wxStaticText(pnlHeader, lblMS, _T(":"), wxDefaultPosition, wxDefaultSize, 0, _T("lblMS"));
    BoxSizer9->Add(StaticText6, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
    txtSecond = new wxTextCtrl(pnlHeader, ID_TEXTCTRL8, wxEmptyString, wxDefaultPosition, wxSize(21,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL8"));
    txtSecond->SetMaxLength(2);
    BoxSizer9->Add(txtSecond, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText7 = new wxStaticText(pnlHeader, lblST, _T("."), wxDefaultPosition, wxDefaultSize, 0, _T("lblST"));
    BoxSizer9->Add(StaticText7, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
    txtTick = new wxTextCtrl(pnlHeader, ID_TEXTCTRL9, wxEmptyString, wxDefaultPosition, wxSize(21,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL9"));
    txtTick->SetMaxLength(1);
    BoxSizer9->Add(txtTick, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer7->Add(BoxSizer9, 0, wxALIGN_LEFT, 0);
    StaticText3 = new wxStaticText(pnlHeader, lblTicks, _("Total number of \'ticks\'"), wxDefaultPosition, wxDefaultSize, 0, _T("lblTicks"));
    BoxSizer7->Add(StaticText3, 0, wxALL|wxALIGN_LEFT, 2);
    txtTicks = new wxTextCtrl(pnlHeader, ID_TEXTCTRL4, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
    BoxSizer7->Add(txtTicks, 0, wxALL|wxALIGN_LEFT, 2);
    BoxSizer5->Add(BoxSizer7, 0, wxALIGN_TOP, 5);
    BoxSizer4->Add(BoxSizer5, 0, wxALL|wxALIGN_LEFT, 5);
    bmScreen = new wxStaticBitmapEx(pnlHeader,ID_CUSTOM2,wxDefaultPosition,wxDefaultSize,00,_T("ID_CUSTOM2"));
    BoxSizer4->Add(bmScreen, 1, wxALL|wxEXPAND, 5);
    pnlHeader->SetSizer(BoxSizer4);
    pnlGVARs = new wxPanel(Notebook1, ID_PANEL4, wxDefaultPosition, wxDefaultSize, 0, _T("ID_PANEL4"));
    BoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer11 = new wxBoxSizer(wxVERTICAL);
    lbGvar = new wxListBox(pnlGVARs, ID_LISTBOX4, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX4"));
    BoxSizer11->Add(lbGvar, 1, wxEXPAND, 5);
    txtGvarDescription = new wxTextCtrl(pnlGVARs, ID_TEXTCTRL12, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL12"));
    BoxSizer11->Add(txtGvarDescription, 0, wxEXPAND, 5);
    BoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
    StaticText12 = new wxStaticText(pnlGVARs, lblValueGvar, _("Value"), wxDefaultPosition, wxDefaultSize, 0, _T("lblValueGvar"));
    BoxSizer13->Add(StaticText12, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    txtGvarVal = new wxTextCtrl(pnlGVARs, ID_TEXTCTRL14, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL14"));
    BoxSizer13->Add(txtGvarVal, 0, wxALIGN_CENTER_VERTICAL, 5);
    StaticLine2 = new wxStaticLine(pnlGVARs, ID_STATICLINE2, wxDefaultPosition, wxSize(1,-1), wxLI_VERTICAL, _T("ID_STATICLINE2"));
    BoxSizer13->Add(StaticLine2, 0, wxALL|wxEXPAND, 5);
    txtFindGvar = new wxTextCtrl(pnlGVARs, ID_TEXTCTRL13, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_NO_VSCROLL, wxDefaultValidator, _T("ID_TEXTCTRL13"));
    BoxSizer13->Add(txtFindGvar, 1, wxALL|wxALIGN_BOTTOM, 5);
    btnFindGvar = new wxButton(pnlGVARs, bFindGvar, _("Find"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bFindGvar"));
    BoxSizer13->Add(btnFindGvar, 0, wxALL|wxALIGN_BOTTOM, 5);
    BoxSizer11->Add(BoxSizer13, 0, wxEXPAND, 5);
    BoxSizer10->Add(BoxSizer11, 1, wxALL|wxEXPAND, 5);
    BoxSizer12 = new wxBoxSizer(wxVERTICAL);
    sbsHolodisks = new wxStaticBoxSizer(wxHORIZONTAL, pnlGVARs, _("Holodisks"));
    lbHolodisks = new wxListBox(pnlGVARs, ID_LISTBOX2, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX2"));
    lbHolodisks->SetMinSize(wxSize(150,0));
    lbHolodisks->SetMaxSize(wxSize(150,-1));
    sbsHolodisks->Add(lbHolodisks, 0, wxEXPAND, 2);
    BoxSizer12->Add(sbsHolodisks, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 1);
    sbsKarmas = new wxStaticBoxSizer(wxHORIZONTAL, pnlGVARs, _("Karmas"));
    lbKarmas = new wxListBox(pnlGVARs, ID_LISTBOX3, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX3"));
    lbKarmas->SetMinSize(wxSize(150,0));
    lbKarmas->SetMaxSize(wxSize(150,-1));
    sbsKarmas->Add(lbKarmas, 0, wxEXPAND, 2);
    BoxSizer12->Add(sbsKarmas, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 1);
    sbsQuests = new wxStaticBoxSizer(wxVERTICAL, pnlGVARs, _("Quests"));
    StaticText8 = new wxStaticText(pnlGVARs, lblQLocation, _("Location"), wxDefaultPosition, wxDefaultSize, 0, _T("lblQLocation"));
    sbsQuests->Add(StaticText8, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    chQLoc = new wxChoice(pnlGVARs, ID_CHOICE3, wxDefaultPosition, wxSize(150,-1), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE3"));
    chQLoc->SetMaxSize(wxSize(150,-1));
    sbsQuests->Add(chQLoc, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    StaticText9 = new wxStaticText(pnlGVARs, lblQDescription, _("Description"), wxDefaultPosition, wxDefaultSize, 0, _T("lblQDescription"));
    sbsQuests->Add(StaticText9, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    chQDesc = new wxChoice(pnlGVARs, ID_CHOICE4, wxDefaultPosition, wxSize(150,-1), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE4"));
    chQDesc->SetMaxSize(wxSize(150,-1));
    sbsQuests->Add(chQDesc, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    StaticText10 = new wxStaticText(pnlGVARs, lblDisplayThr, _("Display threshold"), wxDefaultPosition, wxDefaultSize, 0, _T("lblDisplayThr"));
    sbsQuests->Add(StaticText10, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    txtQDisplay = new wxTextCtrl(pnlGVARs, ID_TEXTCTRL10, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL10"));
    sbsQuests->Add(txtQDisplay, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    StaticText11 = new wxStaticText(pnlGVARs, lblCompletedThr, _("Completed threshold"), wxDefaultPosition, wxDefaultSize, 0, _T("lblCompletedThr"));
    sbsQuests->Add(StaticText11, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    txtQComplete = new wxTextCtrl(pnlGVARs, ID_TEXTCTRL11, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL11"));
    sbsQuests->Add(txtQComplete, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    BoxSizer12->Add(sbsQuests, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1);
    BoxSizer10->Add(BoxSizer12, 0, wxEXPAND, 5);
    pnlGVARs->SetSizer(BoxSizer10);
    pnlWorldmap = new wxPanel(Notebook1, ID_PANEL7, wxDefaultPosition, wxDefaultSize, 0, _T("ID_PANEL7"));
    BoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer19 = new wxBoxSizer(wxVERTICAL);
    StaticText17 = new wxStaticText(pnlWorldmap, lblSavList, _("List of *.sav files"), wxDefaultPosition, wxDefaultSize, 0, _T("lblSavList"));
    BoxSizer19->Add(StaticText17, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    lbSavFiles = new wxListBox(pnlWorldmap, ID_LISTBOX7, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX7"));
    BoxSizer19->Add(lbSavFiles, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    btnEditSAV = new wxButton(pnlWorldmap, bEditSAV, _("Edit"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bEditSAV"));
    BoxSizer19->Add(btnEditSAV, 0, wxALL|wxEXPAND, 5);
    btnDelSav = new wxButton(pnlWorldmap, bDelSav, _("Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bDelSav"));
    BoxSizer19->Add(btnDelSav, 0, wxALL|wxEXPAND, 5);
    BoxSizer18->Add(BoxSizer19, 0, wxEXPAND, 5);
    BoxSizer20 = new wxBoxSizer(wxVERTICAL);
    StaticText18 = new wxStaticText(pnlWorldmap, lblCities, _("Cities"), wxDefaultPosition, wxDefaultSize, 0, _T("lblCities"));
    BoxSizer20->Add(StaticText18, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    lbCities = new wxListBox(pnlWorldmap, ID_LISTBOX8, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX8"));
    BoxSizer20->Add(lbCities, 1, wxALL|wxEXPAND, 5);
    BoxSizer18->Add(BoxSizer20, 1, wxEXPAND, 5);
    BoxSizer21 = new wxBoxSizer(wxVERTICAL);
    StaticText19 = new wxStaticText(pnlWorldmap, lblSelectedCity, _("Selected city"), wxDefaultPosition, wxDefaultSize, 0, _T("lblSelectedCity"));
    BoxSizer21->Add(StaticText19, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    lbCity = new MyListBox(pnlWorldmap, ID_LISTBOX9, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX9"));
    BoxSizer21->Add(lbCity, 1, wxALL|wxEXPAND, 5);
    BoxSizer31 = new wxBoxSizer(wxHORIZONTAL);
    StaticText20 = new wxStaticText(pnlWorldmap, lblValueCity, _("Value"), wxDefaultPosition, wxDefaultSize, 0, _T("lblValueCity"));
    BoxSizer31->Add(StaticText20, 0, wxALIGN_CENTER_VERTICAL, 5);
    txtCityVal = new wxTextCtrl(pnlWorldmap, ID_TEXTCTRL17, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL17"));
    BoxSizer31->Add(txtCityVal, 0, wxALL|wxEXPAND, 5);
    BoxSizer21->Add(BoxSizer31, 0, wxEXPAND, 5);
    sbsCurPos = new wxStaticBoxSizer(wxHORIZONTAL, pnlWorldmap, _("Current position"));
    FlexGridSizer1 = new wxFlexGridSizer(2, 2, 0, 0);
    StaticText21 = new wxStaticText(pnlWorldmap, lblWorldX, _("X"), wxDefaultPosition, wxDefaultSize, 0, _T("lblWorldX"));
    FlexGridSizer1->Add(StaticText21, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    txtXPos = new wxTextCtrl(pnlWorldmap, ID_TEXTCTRL18, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL18"));
    FlexGridSizer1->Add(txtXPos, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    StaticText22 = new wxStaticText(pnlWorldmap, lblWorldY, _("Y"), wxDefaultPosition, wxDefaultSize, 0, _T("lblWorldY"));
    FlexGridSizer1->Add(StaticText22, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    txtYPos = new wxTextCtrl(pnlWorldmap, ID_TEXTCTRL19, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL19"));
    FlexGridSizer1->Add(txtYPos, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    sbsCurPos->Add(FlexGridSizer1, 0, wxEXPAND, 5);
    BoxSizer21->Add(sbsCurPos, 0, wxEXPAND, 5);
    sbsCar = new wxStaticBoxSizer(wxVERTICAL, pnlWorldmap, _("Car"));
    StaticText23 = new wxStaticText(pnlWorldmap, lblCarMap, _("Map"), wxDefaultPosition, wxDefaultSize, 0, _T("lblCarMap"));
    sbsCar->Add(StaticText23, 0, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    chCarMap = new wxChoice(pnlWorldmap, ID_CHOICE5, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE5"));
    sbsCar->Add(chCarMap, 0, wxEXPAND, 5);
    BoxSizer38 = new wxBoxSizer(wxHORIZONTAL);
    StaticText24 = new wxStaticText(pnlWorldmap, lblCarFuel, _("Fuel"), wxDefaultPosition, wxDefaultSize, 0, _T("lblCarFuel"));
    BoxSizer38->Add(StaticText24, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    txtFuel = new wxTextCtrl(pnlWorldmap, ID_TEXTCTRL20, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL20"));
    BoxSizer38->Add(txtFuel, 0, wxEXPAND, 5);
    sbsCar->Add(BoxSizer38, 1, wxALL|wxEXPAND, 5);
    BoxSizer21->Add(sbsCar, 0, wxEXPAND, 5);
    sbsWorldmapState = new wxStaticBoxSizer(wxVERTICAL, pnlWorldmap, _("Worldmap state"));
    chWorldmapState = new wxChoice(pnlWorldmap, ID_CHOICE12, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE12"));
    sbsWorldmapState->Add(chWorldmapState, 0, wxALL|wxEXPAND, 5);
    btnWorlmapSet = new wxButton(pnlWorldmap, bWorlmapSet, _("Set"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bWorlmapSet"));
    sbsWorldmapState->Add(btnWorlmapSet, 0, wxALL|wxEXPAND, 5);
    BoxSizer21->Add(sbsWorldmapState, 0, wxEXPAND, 5);
    BoxSizer18->Add(BoxSizer21, 0, wxEXPAND, 5);
    pnlWorldmap->SetSizer(BoxSizer18);
    pnlInventory = new wxPanel(Notebook1, ID_PANEL6, wxDefaultPosition, wxDefaultSize, 0, _T("ID_PANEL6"));
    BoxSizer22 = new wxBoxSizer(wxHORIZONTAL);
    tItems = new wxTreeCtrl(pnlInventory, ID_TREECTRL1, wxDefaultPosition, wxDefaultSize, wxTR_LINES_AT_ROOT|wxTR_HIDE_ROOT|wxTR_DEFAULT_STYLE, wxDefaultValidator, _T("ID_TREECTRL1"));
    BoxSizer22->Add(tItems, 1, wxALL|wxEXPAND, 5);
    BoxSizer23 = new wxBoxSizer(wxVERTICAL);
    btnEditItem = new wxButton(pnlInventory, bItemEdit, _("Edit"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bItemEdit"));
    BoxSizer23->Add(btnEditItem, 0, wxALL|wxALIGN_LEFT, 5);
    btnCreateItem = new wxButton(pnlInventory, bItemCreate, _("Create"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bItemCreate"));
    BoxSizer23->Add(btnCreateItem, 0, wxALL|wxALIGN_LEFT, 5);
    btnDeleteItem = new wxButton(pnlInventory, bItemDelete, _("Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bItemDelete"));
    BoxSizer23->Add(btnDeleteItem, 0, wxALL|wxALIGN_LEFT, 5);
    StaticText25 = new wxStaticText(pnlInventory, lblQuantity, _("Quantity"), wxDefaultPosition, wxDefaultSize, 0, _T("lblQuantity"));
    BoxSizer23->Add(StaticText25, 0, wxALL|wxALIGN_LEFT, 5);
    txtItemQ = new wxTextCtrl(pnlInventory, ID_TEXTCTRL21, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL21"));
    BoxSizer23->Add(txtItemQ, 0, wxALL|wxALIGN_LEFT, 5);
    sbItem = new wxStaticBitmap(pnlInventory, ID_STATICBITMAP2, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICBITMAP2"));
    BoxSizer23->Add(sbItem, 0, wxALL|wxALIGN_LEFT, 5);
    BoxSizer22->Add(BoxSizer23, 0, wxALL|wxEXPAND, 5);
    pnlInventory->SetSizer(BoxSizer22);
    pnlStats = new wxPanel(Notebook1, ID_PANEL5, wxDefaultPosition, wxDefaultSize, 0, _T("ID_PANEL5"));
    BoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer16 = new wxBoxSizer(wxVERTICAL);
    StaticText13 = new wxStaticText(pnlStats, lblStatsBase, _("Base Stats"), wxDefaultPosition, wxDefaultSize, 0, _T("lblStatsBase"));
    BoxSizer16->Add(StaticText13, 0, wxALL|wxALIGN_LEFT, 5);
    lbBaseStats = new MyListBox(pnlStats, ID_LISTBOX5, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX5"));
    BoxSizer16->Add(lbBaseStats, 1, wxALL|wxEXPAND, 5);
    BoxSizer32 = new wxBoxSizer(wxHORIZONTAL);
    StaticText15 = new wxStaticText(pnlStats, lblValueStatBase, _("Value"), wxDefaultPosition, wxDefaultSize, 0, _T("lblValueStatBase"));
    BoxSizer32->Add(StaticText15, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    txtBaseStatVal = new wxTextCtrl(pnlStats, ID_TEXTCTRL15, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL15"));
    BoxSizer32->Add(txtBaseStatVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer16->Add(BoxSizer32, 0, wxALIGN_LEFT, 5);
    BoxSizer15->Add(BoxSizer16, 1, wxEXPAND, 5);
    BoxSizer17 = new wxBoxSizer(wxVERTICAL);
    StaticText14 = new wxStaticText(pnlStats, lblStatsBonus, _("Bonus Stats"), wxDefaultPosition, wxDefaultSize, 0, _T("lblStatsBonus"));
    BoxSizer17->Add(StaticText14, 0, wxALL|wxALIGN_LEFT, 5);
    lbBonusStats = new MyListBox(pnlStats, ID_LISTBOX6, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX6"));
    BoxSizer17->Add(lbBonusStats, 1, wxALL|wxEXPAND, 5);
    BoxSizer33 = new wxBoxSizer(wxHORIZONTAL);
    StaticText16 = new wxStaticText(pnlStats, lblValueStatBonus, _("Value"), wxDefaultPosition, wxDefaultSize, 0, _T("lblValueStatBonus"));
    BoxSizer33->Add(StaticText16, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    txtBonusStatVal = new wxTextCtrl(pnlStats, ID_TEXTCTRL16, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL16"));
    BoxSizer33->Add(txtBonusStatVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer17->Add(BoxSizer33, 0, wxALIGN_LEFT, 5);
    BoxSizer15->Add(BoxSizer17, 1, wxEXPAND, 5);
    pnlStats->SetSizer(BoxSizer15);
    pnlSkillsPerks = new wxPanel(Notebook1, ID_PANEL10, wxDefaultPosition, wxDefaultSize, 0, _T("ID_PANEL10"));
    BoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer25 = new wxBoxSizer(wxVERTICAL);
    sbsTag = new wxStaticBoxSizer(wxVERTICAL, pnlSkillsPerks, _("Tag skills"));
    chTag1 = new wxChoice(pnlSkillsPerks, ID_CHOICE6, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE6"));
    sbsTag->Add(chTag1, 0, wxALL|wxEXPAND, 2);
    chTag2 = new wxChoice(pnlSkillsPerks, ID_CHOICE7, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE7"));
    sbsTag->Add(chTag2, 0, wxALL|wxEXPAND, 2);
    chTag3 = new wxChoice(pnlSkillsPerks, ID_CHOICE8, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE8"));
    sbsTag->Add(chTag3, 0, wxALL|wxEXPAND, 2);
    chTag4 = new wxChoice(pnlSkillsPerks, ID_CHOICE9, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE9"));
    sbsTag->Add(chTag4, 0, wxALL|wxEXPAND, 2);
    BoxSizer25->Add(sbsTag, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    sbsSkills = new wxStaticBoxSizer(wxVERTICAL, pnlSkillsPerks, _("Skills (raw values)"));
    lbSkills = new MyListBox(pnlSkillsPerks, ID_LISTBOX12, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX12"));
    sbsSkills->Add(lbSkills, 1, wxALL|wxEXPAND, 2);
    BoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
    StaticText27 = new wxStaticText(pnlSkillsPerks, lblValueSkill, _("Value"), wxDefaultPosition, wxDefaultSize, 0, _T("lblValueSkill"));
    BoxSizer27->Add(StaticText27, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    txtSkillVal = new wxTextCtrl(pnlSkillsPerks, ID_TEXTCTRL23, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL23"));
    BoxSizer27->Add(txtSkillVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    sbsSkills->Add(BoxSizer27, 0, wxALIGN_LEFT, 5);
    BoxSizer25->Add(sbsSkills, 1, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 5);
    BoxSizer24->Add(BoxSizer25, 0, wxEXPAND, 5);
    sbsPerks = new wxStaticBoxSizer(wxVERTICAL, pnlSkillsPerks, _("Perks"));
    lbPerks = new MyListBox(pnlSkillsPerks, ID_LISTBOX11, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX11"));
    sbsPerks->Add(lbPerks, 1, wxALL|wxEXPAND, 2);
    BoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
    StaticText26 = new wxStaticText(pnlSkillsPerks, lblValuePerk, _("Value"), wxDefaultPosition, wxDefaultSize, 0, _T("lblValuePerk"));
    BoxSizer26->Add(StaticText26, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    txtPerkVal = new wxTextCtrl(pnlSkillsPerks, ID_TEXTCTRL22, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL22"));
    BoxSizer26->Add(txtPerkVal, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    cbPickPerk = new wxCheckBox(pnlSkillsPerks, cboxPickPerk, _("Pick a perk window"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxPickPerk"));
    cbPickPerk->SetValue(false);
    BoxSizer26->Add(cbPickPerk, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    sbsPerks->Add(BoxSizer26, 0, wxALIGN_LEFT, 5);
    BoxSizer24->Add(sbsPerks, 1, wxEXPAND, 5);
    pnlSkillsPerks->SetSizer(BoxSizer24);
    pnlMisc = new wxPanel(Notebook1, ID_PANEL8, wxDefaultPosition, wxDefaultSize, 0, _T("ID_PANEL8"));
    BoxSizer28 = new wxBoxSizer(wxHORIZONTAL);
    sbsKills = new wxStaticBoxSizer(wxVERTICAL, pnlMisc, _("Kills"));
    lbKills = new MyListBox(pnlMisc, ID_LISTBOX13, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX13"));
    sbsKills->Add(lbKills, 1, wxALL|wxEXPAND, 5);
    BoxSizer30 = new wxBoxSizer(wxHORIZONTAL);
    StaticText28 = new wxStaticText(pnlMisc, lblValueKill, _("Value"), wxDefaultPosition, wxDefaultSize, 0, _T("lblValueKill"));
    BoxSizer30->Add(StaticText28, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    txtKillVal = new wxTextCtrl(pnlMisc, ID_TEXTCTRL24, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL24"));
    BoxSizer30->Add(txtKillVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    sbsKills->Add(BoxSizer30, 0, wxALIGN_LEFT, 5);
    BoxSizer28->Add(sbsKills, 1, wxEXPAND, 5);
    BoxSizer29 = new wxBoxSizer(wxVERTICAL);
    sbsPlayer = new wxStaticBoxSizer(wxHORIZONTAL, pnlMisc, _("Player"));
    FlexGridSizer2 = new wxFlexGridSizer(0, 4, 0, 0);
    StaticText29 = new wxStaticText(pnlMisc, lblExperience, _("Experience"), wxDefaultPosition, wxDefaultSize, 0, _T("lblExperience"));
    FlexGridSizer2->Add(StaticText29, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    txtExperience = new wxTextCtrl(pnlMisc, ID_TEXTCTRL25, wxEmptyString, wxDefaultPosition, wxSize(80,-1), 0, wxDefaultValidator, _T("ID_TEXTCTRL25"));
    txtExperience->SetMaxSize(wxSize(80,-1));
    FlexGridSizer2->Add(txtExperience, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText30 = new wxStaticText(pnlMisc, lblLevel, _("Level"), wxDefaultPosition, wxDefaultSize, 0, _T("lblLevel"));
    FlexGridSizer2->Add(StaticText30, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    txtLevel = new wxTextCtrl(pnlMisc, ID_TEXTCTRL26, wxEmptyString, wxDefaultPosition, wxSize(80,-1), 0, wxDefaultValidator, _T("ID_TEXTCTRL26"));
    txtLevel->SetMaxSize(wxSize(80,-1));
    FlexGridSizer2->Add(txtLevel, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText31 = new wxStaticText(pnlMisc, lblSkillPoints, _("Skill points"), wxDefaultPosition, wxDefaultSize, 0, _T("lblSkillPoints"));
    FlexGridSizer2->Add(StaticText31, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    txtSkillPoints = new wxTextCtrl(pnlMisc, ID_TEXTCTRL27, wxEmptyString, wxDefaultPosition, wxSize(80,-1), 0, wxDefaultValidator, _T("ID_TEXTCTRL27"));
    txtSkillPoints->SetMaxSize(wxSize(80,-1));
    FlexGridSizer2->Add(txtSkillPoints, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText32 = new wxStaticText(pnlMisc, lblHitpoints, _("Hitpoints"), wxDefaultPosition, wxDefaultSize, 0, _T("lblHitpoints"));
    FlexGridSizer2->Add(StaticText32, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    txtHitpoints = new wxTextCtrl(pnlMisc, ID_TEXTCTRL28, wxEmptyString, wxDefaultPosition, wxSize(80,-1), 0, wxDefaultValidator, _T("ID_TEXTCTRL28"));
    txtHitpoints->SetMaxSize(wxSize(80,-1));
    FlexGridSizer2->Add(txtHitpoints, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText34 = new wxStaticText(pnlMisc, lblRadiation, _("Radiation"), wxDefaultPosition, wxDefaultSize, 0, _T("lblRadiation"));
    FlexGridSizer2->Add(StaticText34, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    txtRadiation = new wxTextCtrl(pnlMisc, ID_TEXTCTRL29, wxEmptyString, wxDefaultPosition, wxSize(80,-1), 0, wxDefaultValidator, _T("ID_TEXTCTRL29"));
    txtRadiation->SetMaxSize(wxSize(80,-1));
    FlexGridSizer2->Add(txtRadiation, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText35 = new wxStaticText(pnlMisc, lblPoison, _("Poison"), wxDefaultPosition, wxDefaultSize, 0, _T("lblPoison"));
    FlexGridSizer2->Add(StaticText35, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    txtPoison = new wxTextCtrl(pnlMisc, ID_TEXTCTRL30, wxEmptyString, wxDefaultPosition, wxSize(80,-1), 0, wxDefaultValidator, _T("ID_TEXTCTRL30"));
    txtPoison->SetMaxSize(wxSize(80,-1));
    FlexGridSizer2->Add(txtPoison, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    sbsPlayer->Add(FlexGridSizer2, 1, wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer29->Add(sbsPlayer, 0, wxEXPAND, 5);
    sbsTraits = new wxStaticBoxSizer(wxHORIZONTAL, pnlMisc, _("Traits"));
    chTrait1 = new wxChoice(pnlMisc, ID_CHOICE10, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE10"));
    sbsTraits->Add(chTrait1, 1, wxEXPAND, 5);
    chTrait2 = new wxChoice(pnlMisc, ID_CHOICE11, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE11"));
    sbsTraits->Add(chTrait2, 1, wxEXPAND, 5);
    BoxSizer29->Add(sbsTraits, 0, wxEXPAND, 5);
    sbsLocalMap = new wxStaticBoxSizer(wxVERTICAL, pnlMisc, _("Local map"));
    BoxSizer37 = new wxBoxSizer(wxHORIZONTAL);
    StaticText37 = new wxStaticText(pnlMisc, lblLocalX, _("X[0..199]"), wxDefaultPosition, wxDefaultSize, 0, _T("lblLocalX"));
    BoxSizer37->Add(StaticText37, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
    txtLocalX = new wxTextCtrl(pnlMisc, ID_TEXTCTRL32, wxEmptyString, wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_TEXTCTRL32"));
    txtLocalX->SetMaxSize(wxSize(40,-1));
    BoxSizer37->Add(txtLocalX, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
    StaticText38 = new wxStaticText(pnlMisc, lblLocalY, _("Y[0..199]"), wxDefaultPosition, wxDefaultSize, 0, _T("lblLocalY"));
    BoxSizer37->Add(StaticText38, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
    txtLocalY = new wxTextCtrl(pnlMisc, ID_TEXTCTRL33, wxEmptyString, wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_TEXTCTRL33"));
    txtLocalY->SetMaxSize(wxSize(40,-1));
    BoxSizer37->Add(txtLocalY, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
    StaticText36 = new wxStaticText(pnlMisc, lblLocalMapLevel, _("Level[0..2]"), wxDefaultPosition, wxDefaultSize, 0, _T("lblLocalMapLevel"));
    BoxSizer37->Add(StaticText36, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
    txtLocalMapLevel = new wxTextCtrl(pnlMisc, ID_TEXTCTRL31, wxEmptyString, wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_TEXTCTRL31"));
    txtLocalMapLevel->SetMaxSize(wxSize(40,-1));
    BoxSizer37->Add(txtLocalMapLevel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
    sbsLocalMap->Add(BoxSizer37, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer29->Add(sbsLocalMap, 0, wxEXPAND, 5);
    BoxSizer35 = new wxBoxSizer(wxHORIZONTAL);
    sbsBodyFlags = new wxStaticBoxSizer(wxVERTICAL, pnlMisc, _("Body flags"));
    cbCrippledEye = new wxCheckBox(pnlMisc, cboxCrippledEye, _("Eye Damage"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxCrippledEye"));
    cbCrippledEye->SetValue(false);
    sbsBodyFlags->Add(cbCrippledEye, 1, wxALL|wxALIGN_LEFT, 1);
    cbCrippledRArm = new wxCheckBox(pnlMisc, cboxCrippledRArm, _("Crippled Right Arm"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxCrippledRArm"));
    cbCrippledRArm->SetValue(false);
    sbsBodyFlags->Add(cbCrippledRArm, 1, wxALL|wxALIGN_LEFT, 1);
    cbCrippledLArm = new wxCheckBox(pnlMisc, cboxCrippledLArm, _("Crippled Left Arm"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxCrippledLArm"));
    cbCrippledLArm->SetValue(false);
    sbsBodyFlags->Add(cbCrippledLArm, 1, wxALL|wxALIGN_LEFT, 1);
    cbCrippledRLeg = new wxCheckBox(pnlMisc, cboxCrippledRLeg, _("Crippled Right Leg"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxCrippledRLeg"));
    cbCrippledRLeg->SetValue(false);
    sbsBodyFlags->Add(cbCrippledRLeg, 1, wxALL|wxALIGN_LEFT, 1);
    cbCrippledLLeg = new wxCheckBox(pnlMisc, cboxCrippledLLeg, _("Crippled Left Leg"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxCrippledLLeg"));
    cbCrippledLLeg->SetValue(false);
    sbsBodyFlags->Add(cbCrippledLLeg, 1, wxALL|wxALIGN_LEFT, 1);
    BoxSizer35->Add(sbsBodyFlags, 0, wxALIGN_TOP, 5);
    BoxSizer36 = new wxBoxSizer(wxVERTICAL);
    sbsBugFixes = new wxStaticBoxSizer(wxVERTICAL, pnlMisc, _("Bug fixes (see readme)"));
    cbGhostMode = new wxCheckBox(pnlMisc, cboxGhostMode, _("\"Ghost mode\""), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("cboxGhostMode"));
    cbGhostMode->SetValue(false);
    sbsBugFixes->Add(cbGhostMode, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    btnClearCombat = new wxButton(pnlMisc, bClearCombat, _("Clear combat info"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bClearCombat"));
    sbsBugFixes->Add(btnClearCombat, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    BoxSizer36->Add(sbsBugFixes, 0, wxALIGN_LEFT, 5);
    btnEditCritters = new wxButton(pnlMisc, bEditCritters, _("Edit critters"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bEditCritters"));
    btnEditCritters->Disable();
    BoxSizer36->Add(btnEditCritters, 0, wxALL|wxALIGN_LEFT, 2);
    BoxSizer35->Add(BoxSizer36, 1, wxEXPAND, 5);
    BoxSizer29->Add(BoxSizer35, 0, wxEXPAND, 5);
    sbsGameReset = new wxStaticBoxSizer(wxHORIZONTAL, pnlMisc, _("Game reset"));
    btnResetGvars = new wxButton(pnlMisc, bResetGvars, _("Global variables"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bResetGvars"));
    sbsGameReset->Add(btnResetGvars, 1, wxALIGN_CENTER_VERTICAL, 5);
    btnResetEntrances = new wxButton(pnlMisc, bResetEntrances, _("Worldmap entrances"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bResetEntrances"));
    sbsGameReset->Add(btnResetEntrances, 1, wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer29->Add(sbsGameReset, 0, wxEXPAND, 5);
    BoxSizer28->Add(BoxSizer29, 0, wxLEFT|wxEXPAND, 5);
    pnlMisc->SetSizer(BoxSizer28);
    pnlSFall = new wxPanel(Notebook1, ID_PANEL11, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL11"));
    BoxSizer39 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer40 = new wxBoxSizer(wxVERTICAL);
    btnSaveSFallgv = new wxButton(pnlSFall, bSaveSFallgv, _("Save sfallgv.sav"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bSaveSFallgv"));
    BoxSizer40->Add(btnSaveSFallgv, 0, wxALL|wxEXPAND, 5);
    sbsSFallGvars = new wxStaticBoxSizer(wxVERTICAL, pnlSFall, _("Global variables"));
    lbSFallGVar = new MyListBox(pnlSFall, ID_LISTBOX14, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX14"));
    sbsSFallGvars->Add(lbSFallGVar, 1, wxALL|wxEXPAND, 5);
    btnSFallGvarAdd = new wxButton(pnlSFall, bSFallGvarAdd, _("Add"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bSFallGvarAdd"));
    sbsSFallGvars->Add(btnSFallGvarAdd, 0, wxALL|wxEXPAND, 5);
    btnSFallGvarDel = new wxButton(pnlSFall, bSFallGvarDel, _("Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("bSFallGvarDel"));
    sbsSFallGvars->Add(btnSFallGvarDel, 0, wxALL|wxEXPAND, 5);
    BoxSizer41 = new wxBoxSizer(wxHORIZONTAL);
    lblSFallGvar = new wxStaticText(pnlSFall, lblValueSFallGvar, _("Value"), wxDefaultPosition, wxDefaultSize, 0, _T("lblValueSFallGvar"));
    BoxSizer41->Add(lblSFallGvar, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    txtSFallGvarVal = new wxTextCtrl(pnlSFall, ID_TEXTCTRL34, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL34"));
    BoxSizer41->Add(txtSFallGvarVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    sbsSFallGvars->Add(BoxSizer41, 0, wxALL|wxEXPAND, 5);
    BoxSizer40->Add(sbsSFallGvars, 1, wxALL|wxEXPAND, 5);
    BoxSizer39->Add(BoxSizer40, 1, wxALL|wxEXPAND, 0);
    sbsSFallArrays = new wxStaticBoxSizer(wxVERTICAL, pnlSFall, _("Arrays"));
    tSfallArrays = new wxTreeCtrl(pnlSFall, ID_TREECTRL2, wxDefaultPosition, wxDefaultSize, wxTR_LINES_AT_ROOT|wxTR_HIDE_ROOT|wxTR_DEFAULT_STYLE, wxDefaultValidator, _T("ID_TREECTRL2"));
    sbsSFallArrays->Add(tSfallArrays, 5, wxALL|wxEXPAND, 5);
    BoxSizer42 = new wxBoxSizer(wxHORIZONTAL);
    lblSFallArrayVal = new wxStaticText(pnlSFall, ID_STATICTEXT1, _("Value"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    BoxSizer42->Add(lblSFallArrayVal, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtSFallArrayNewVal = new wxTextCtrl(pnlSFall, ID_TEXTCTRL39, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL39"));
    BoxSizer42->Add(txtSFallArrayNewVal, 1, wxALL, 5);
    sbsSFallArrays->Add(BoxSizer42, 0, wxALL|wxEXPAND, 5);
    wxString __wxRadioBoxChoices_1[4] =
    {
        _("NONE"),
        _("INT"),
        _("FLOAT"),
        _("STR")
    };
    rbSfallArrType = new wxRadioBox(pnlSFall, ID_RADIOBOX1, _("Type"), wxDefaultPosition, wxDefaultSize, 4, __wxRadioBoxChoices_1, 1, wxRA_SPECIFY_ROWS, wxDefaultValidator, _T("ID_RADIOBOX1"));
    sbsSFallArrays->Add(rbSfallArrType, 0, wxALL|wxEXPAND, 5);
    BoxSizer39->Add(sbsSFallArrays, 1, wxALL|wxEXPAND, 5);
    sbsSFallTraits = new wxStaticBoxSizer(wxVERTICAL, pnlSFall, _("Traits"));
    lbSFallTraits = new wxListBox(pnlSFall, ID_LISTBOX15, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX15"));
    sbsSFallTraits->Add(lbSFallTraits, 5, wxALL|wxEXPAND, 5);
    txtSFallTraitDesc = new wxTextCtrl(pnlSFall, ID_TEXTCTRL35, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL35"));
    sbsSFallTraits->Add(txtSFallTraitDesc, 1, wxALL|wxEXPAND, 5);
    BoxSizer39->Add(sbsSFallTraits, 1, wxALL|wxEXPAND, 5);
    sbsSFallPerks = new wxStaticBoxSizer(wxVERTICAL, pnlSFall, _("Perks"));
    lbSFallPerks = new wxListBox(pnlSFall, ID_LISTBOX16, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX16"));
    sbsSFallPerks->Add(lbSFallPerks, 5, wxALL|wxEXPAND, 5);
    txtSFallPerkDesc = new wxTextCtrl(pnlSFall, ID_TEXTCTRL36, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL36"));
    sbsSFallPerks->Add(txtSFallPerkDesc, 1, wxALL|wxEXPAND, 5);
    BoxSizer39->Add(sbsSFallPerks, 1, wxALL|wxEXPAND, 5);
    sbsSFallSelectablePerks = new wxStaticBoxSizer(wxVERTICAL, pnlSFall, _("Selectable perks"));
    lbSFallSelPerks = new wxListBox(pnlSFall, ID_LISTBOX17, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX17"));
    sbsSFallSelectablePerks->Add(lbSFallSelPerks, 5, wxALL|wxEXPAND, 5);
    txtSFallSelPerkDesc = new wxTextCtrl(pnlSFall, ID_TEXTCTRL37, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL37"));
    sbsSFallSelectablePerks->Add(txtSFallSelPerkDesc, 1, wxALL|wxEXPAND, 5);
    BoxSizer39->Add(sbsSFallSelectablePerks, 1, wxALL|wxEXPAND, 5);
    pnlSFall->SetSizer(BoxSizer39);
    pnlHex = new wxPanel(Notebook1, ID_PANEL9, wxDefaultPosition, wxDefaultSize, 0, _T("ID_PANEL9"));
    BoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer34 = new wxBoxSizer(wxVERTICAL);
    lbFunctions = new wxListBox(pnlHex, ID_LISTBOX10, wxDefaultPosition, wxSize(132,174), 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX10"));
    lbFunctions->SetMinSize(wxSize(260,-1));
    BoxSizer34->Add(lbFunctions, 1, wxALL|wxEXPAND, 5);
    BoxSizer14->Add(BoxSizer34, 0, wxEXPAND, 5);
    hexEditor = new SimpleHexEditCtrlWX(pnlHex,ID_CUSTOM1,wxDefaultPosition,wxSize(220,389),00,_T("ID_CUSTOM1"));
    BoxSizer14->Add(hexEditor, 1, wxEXPAND, 0);
    pnlHex->SetSizer(BoxSizer14);
    Notebook1->AddPage(pnlHeader, _("Header"), false);
    Notebook1->AddPage(pnlGVARs, _("GVARs"), false);
    Notebook1->AddPage(pnlWorldmap, _("World map"), false);
    Notebook1->AddPage(pnlInventory, _("Inventory"), false);
    Notebook1->AddPage(pnlStats, _("Stats"), false);
    Notebook1->AddPage(pnlSkillsPerks, _("Skills/Perks"), false);
    Notebook1->AddPage(pnlMisc, _("Misc"), false);
    Notebook1->AddPage(pnlSFall, _("SFall"), false);
    Notebook1->AddPage(pnlHex, _("HEXview"), false);
    BoxSizer2->Add(Notebook1, 1, wxALL|wxEXPAND, 5);
    pnlMain->SetSizer(BoxSizer2);
    pnlLog = new wxPanel(SplitterWindow1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, 0, _T("ID_PANEL2"));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    txtLog = new wxTextCtrl(pnlLog, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    BoxSizer1->Add(txtLog, 1, wxALL|wxEXPAND, 5);
    pnlLog->SetSizer(BoxSizer1);
    SplitterWindow1->SplitHorizontally(pnlMain, pnlLog);
    SplitterWindow1->SetSashPosition(600);
    mbMain = new wxMenuBar();
    mFile = new wxMenu();
    mOpen = new wxMenuItem(mFile, ID_Open, _("&Open\tCTRL+O"), wxEmptyString, wxITEM_NORMAL);
    mFile->Append(mOpen);
    mSave = new wxMenuItem(mFile, ID_Save, _("&Save\tCTRL+S"), wxEmptyString, wxITEM_NORMAL);
    mFile->Append(mSave);
    mSave->Enable(false);
    mSaveAs = new wxMenuItem(mFile, ID_SaveAs, _("Save as...\tCTRL+SHIFT+S"), wxEmptyString, wxITEM_NORMAL);
    mFile->Append(mSaveAs);
    mSaveAs->Enable(false);
    mFile->AppendSeparator();
    mSettings = new wxMenuItem(mFile, ID_Settings, _("Settings\tF12"), wxEmptyString, wxITEM_NORMAL);
    mFile->Append(mSettings);
    mFile->AppendSeparator();
    mQuit = new wxMenuItem(mFile, ID_Quit, _("&Quit\tCTRL+Q"), wxEmptyString, wxITEM_NORMAL);
    mFile->Append(mQuit);
    mbMain->Append(mFile, _("File"));
    SetMenuBar(mbMain);
    mArmor = new wxMenuItem((&menuCreateItem), ID_Armor, _("Armor"), wxEmptyString, wxITEM_NORMAL);
    menuCreateItem.Append(mArmor);
    mContainer = new wxMenuItem((&menuCreateItem), ID_Container, _("Container"), wxEmptyString, wxITEM_NORMAL);
    menuCreateItem.Append(mContainer);
    mDrug = new wxMenuItem((&menuCreateItem), ID_Drug, _("Drug"), wxEmptyString, wxITEM_NORMAL);
    menuCreateItem.Append(mDrug);
    mWeapon = new wxMenuItem((&menuCreateItem), ID_Weapon, _("Weapon"), wxEmptyString, wxITEM_NORMAL);
    menuCreateItem.Append(mWeapon);
    mAmmo = new wxMenuItem((&menuCreateItem), ID_Ammo, _("Ammo"), wxEmptyString, wxITEM_NORMAL);
    menuCreateItem.Append(mAmmo);
    mMiscellaneous = new wxMenuItem((&menuCreateItem), ID_Miscellaneous, _("Miscellaneous"), wxEmptyString, wxITEM_NORMAL);
    menuCreateItem.Append(mMiscellaneous);
    mKey = new wxMenuItem((&menuCreateItem), ID_Key, _("Key"), wxEmptyString, wxITEM_NORMAL);
    menuCreateItem.Append(mKey);

    Connect(ID_LISTBOX1,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&F12seFrame::OnlbSavesSelect);
    Connect(bRefresh,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&F12seFrame::OnbtnRefreshClick);
    Connect(bSave,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&F12seFrame::OnbtnSaveClick);
    Connect(ID_TEXTCTRL2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OntxtSaveNameText);
    Connect(ID_TEXTCTRL3,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OntxtPlayerNameText);
    Connect(ID_TEXTCTRL5,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OnDataChanged);
    Connect(ID_CHOICE1,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&F12seFrame::OnDataChanged);
    Connect(ID_CHOICE2,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&F12seFrame::OnDataChanged);
    Connect(ID_TEXTCTRL6,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OnDataChanged);
    Connect(ID_TEXTCTRL7,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OnDataChanged);
    Connect(ID_TEXTCTRL8,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OnDataChanged);
    Connect(ID_TEXTCTRL9,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OnDataChanged);
    Connect(ID_TEXTCTRL4,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OnTxtText);
    Connect(ID_LISTBOX4,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&F12seFrame::OnlbGvarSelect);
    Connect(ID_TEXTCTRL14,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OntxtGvarValText);
    Connect(bFindGvar,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&F12seFrame::OnbtnFindGvarClick);
    Connect(ID_LISTBOX2,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&F12seFrame::OnlbHolodisksSelect);
    Connect(ID_LISTBOX3,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&F12seFrame::OnlbKarmasSelect);
    Connect(ID_CHOICE3,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&F12seFrame::OnchQLocSelect);
    Connect(ID_CHOICE4,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&F12seFrame::OnchQDescSelect);
    Connect(ID_LISTBOX7,wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,(wxObjectEventFunction)&F12seFrame::OnlbSavFilesDClick);
    Connect(bEditSAV,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&F12seFrame::OnbtnEditSAVClick);
    Connect(bDelSav,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&F12seFrame::OnbtnDelSavClick);
    Connect(ID_LISTBOX8,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&F12seFrame::OnlbCitiesSelect);
    Connect(ID_LISTBOX9,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&F12seFrame::OnlbCitySelect);
    Connect(ID_TEXTCTRL17,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OntxtCityValText);
    Connect(ID_TEXTCTRL18,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OnTxtText);
    Connect(ID_TEXTCTRL19,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OnTxtText);
    Connect(ID_CHOICE5,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&F12seFrame::OnchSelect);
    Connect(ID_TEXTCTRL20,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OnTxtText);
    Connect(bWorlmapSet,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&F12seFrame::OnbtnWorlmapSetClick);
    Connect(ID_TREECTRL1,wxEVT_COMMAND_TREE_ITEM_ACTIVATED,(wxObjectEventFunction)&F12seFrame::OntItemsItemActivated);
    Connect(ID_TREECTRL1,wxEVT_COMMAND_TREE_SEL_CHANGED,(wxObjectEventFunction)&F12seFrame::OntItemsSelectionChanged);
    Connect(bItemEdit,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&F12seFrame::OnbtnEditItemClick);
    Connect(bItemCreate,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&F12seFrame::OnbtnCreateItemClick);
    Connect(bItemDelete,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&F12seFrame::OnbtnDeleteItemClick);
    Connect(ID_TEXTCTRL21,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OntxtItemQText);
    Connect(ID_LISTBOX5,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&F12seFrame::OnLbTxtSelect);
    Connect(ID_TEXTCTRL15,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OnTxtLbText);
    Connect(ID_LISTBOX6,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&F12seFrame::OnLbTxtSelect);
    Connect(ID_TEXTCTRL16,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OnTxtLbText);
    Connect(ID_CHOICE6,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&F12seFrame::OnchSelect);
    Connect(ID_CHOICE7,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&F12seFrame::OnchSelect);
    Connect(ID_CHOICE8,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&F12seFrame::OnchSelect);
    Connect(ID_CHOICE9,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&F12seFrame::OnchSelect);
    Connect(ID_LISTBOX12,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&F12seFrame::OnLbTxtSelect);
    Connect(ID_TEXTCTRL23,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OnTxtLbText);
    Connect(ID_LISTBOX11,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&F12seFrame::OnLbTxtSelect);
    Connect(ID_TEXTCTRL22,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OntxtPerkValText);
    Connect(cboxPickPerk,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&F12seFrame::OncbPickPerkClick);
    Connect(ID_LISTBOX13,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&F12seFrame::OnLbTxtSelect);
    Connect(ID_TEXTCTRL24,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OnTxtLbText);
    Connect(ID_TEXTCTRL25,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OnTxtText);
    Connect(ID_TEXTCTRL26,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OntxtLevelText);
    Connect(ID_TEXTCTRL27,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OnTxtText);
    Connect(ID_TEXTCTRL28,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OnTxtText);
    Connect(ID_TEXTCTRL29,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OnTxtText);
    Connect(ID_TEXTCTRL30,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OnTxtText);
    Connect(ID_CHOICE10,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&F12seFrame::OnchSelect);
    Connect(ID_CHOICE11,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&F12seFrame::OnchSelect);
    Connect(ID_TEXTCTRL32,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OntxtLocalCoord);
    Connect(ID_TEXTCTRL33,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OntxtLocalCoord);
    Connect(ID_TEXTCTRL31,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OntxtLocalMapLevelText);
    Connect(cboxCrippledEye,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&F12seFrame::OncbCrippledEyeClick);
    Connect(cboxCrippledRArm,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&F12seFrame::OncbCrippledRArmClick);
    Connect(cboxCrippledLArm,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&F12seFrame::OncbCrippledLArmClick);
    Connect(cboxCrippledRLeg,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&F12seFrame::OncbCrippledRLegClick);
    Connect(cboxCrippledLLeg,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&F12seFrame::OncbCrippledLLegClick);
    Connect(cboxGhostMode,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&F12seFrame::OncbGhostModeClick);
    Connect(bClearCombat,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&F12seFrame::OnbtnClearCombatClick);
    Connect(bEditCritters,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&F12seFrame::OnbtnEditCrittersClick);
    Connect(bResetGvars,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&F12seFrame::OnbtnResetGvarsClick);
    Connect(bResetEntrances,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&F12seFrame::OnbtnResetEntrancesClick);
    Connect(bSaveSFallgv,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&F12seFrame::OnbtnSaveSFallgvClick);
    Connect(ID_LISTBOX14,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&F12seFrame::OnlbSFallGVarSelect);
    Connect(bSFallGvarAdd,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&F12seFrame::OnbtnSFallGvarAddClick);
    Connect(bSFallGvarDel,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&F12seFrame::OnbtnSFallGvarDelClick);
    Connect(ID_TEXTCTRL34,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OntxtSFallGvarValText);
    Connect(ID_TREECTRL2,wxEVT_COMMAND_TREE_SEL_CHANGED,(wxObjectEventFunction)&F12seFrame::OntSfallArraysSelectionChanged);
    Connect(ID_TEXTCTRL39,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&F12seFrame::OntxtSFallArrayNewValText);
    Connect(ID_RADIOBOX1,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&F12seFrame::OnrbSfallArrTypeSelect);
    Connect(ID_LISTBOX15,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&F12seFrame::OnlbSFallTraitsSelect);
    Connect(ID_LISTBOX16,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&F12seFrame::OnlbSFallPerksSelect);
    Connect(ID_LISTBOX17,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&F12seFrame::OnlbSFallSelPerksSelect);
    Connect(ID_LISTBOX10,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&F12seFrame::OnlbFunctionsSelect);
    //*)
    ntst_log_set_callback(LogCallback, this);
    sbItem->SetBitmap(wxBitmap(1, 1)); // default wxNullBitmap triggers assert "bmp.IsOk()" failed in DoDrawBitmap(): invalid bitmap in wxMSWDCImpl::DrawBitmap
    SetTitle(GetF12seTitleStart());
    cityX = "x";
    cityY = "y";
    cityState = "state";
    cityButton = "button";
    cityEntrance = "entrance";
    fileCrcChanged = FILECRC_CHANGED;
    SFallNewGvarMessage = SFALL_NEW_GVAR_MESSAGE;
    for (unsigned i = 0; i < NTST_ARRAY_LEN(worldMapStates); ++i)
    {
        chWorldmapState->Append(worldMapStates[i]);
    }
    chWorldmapState->SetSelection(0);

    sbsHolodisks->GetStaticBox()->SetName("sbHolodisks");
    sbsKarmas->GetStaticBox()->SetName("sbKarmas");
    sbsQuests->GetStaticBox()->SetName("sbQuests");
    sbsCurPos->GetStaticBox()->SetName("sbCurPos");
    sbsCar->GetStaticBox()->SetName("sbCar");
    sbsTag->GetStaticBox()->SetName("sbTag");
    sbsSkills->GetStaticBox()->SetName("sbSkills");
    sbsPerks->GetStaticBox()->SetName("sbPerks");
    sbsKills->GetStaticBox()->SetName("sbKills");
    sbsPlayer->GetStaticBox()->SetName("sbPlayer");
    sbsTraits->GetStaticBox()->SetName("sbTraits");
    sbsLocalMap->GetStaticBox()->SetName("sbLocalMap");
    sbsBodyFlags->GetStaticBox()->SetName("sbBodyFlags");
    sbsBugFixes->GetStaticBox()->SetName("sbBugFixes");
    sbsSFallGvars->GetStaticBox()->SetName("sbSFallGvars");
    sbsSFallTraits->GetStaticBox()->SetName("sbSFallTraits");
    sbsSFallPerks->GetStaticBox()->SetName("sbSFallPerks");
    sbsSFallSelectablePerks->GetStaticBox()->SetName("sbSFallSelectablePerks");
    sbsGameReset->GetStaticBox()->SetName("sbGameReset");
    sbsWorldmapState->GetStaticBox()->SetName("sbWorldmapState");

    chTags[0] = chTag1;
    chTags[1] = chTag2;
    chTags[2] = chTag3;
    chTags[3] = chTag4;

    txtLog->SetMaxLength(65536);
    for (size_t i = 0; i < NTST_ARRAY_LEN(functions); ++i)
        lbFunctions->Append(functions[i]);
    datamodel.SetView(this);
    if (!datamodel.InitAll(NULL))
        {
        GetSettingsAndApply(NULL);
        }
    datamodel.curSave.fileSections[0].Subscribe(::RefreshHeader, this);
    datamodel.curSave.fileSections[3].Subscribe(::RefreshF2, this);
    datamodel.curSave.fileSections[4].Subscribe(::RefreshF3, this);
    datamodel.curSave.fileSections[6].Subscribe(::RefreshF5, this);
    datamodel.curSave.fileSections[7].Subscribe(::RefreshF6, this);
    datamodel.curSave.fileSections[8].Subscribe(::RefreshF7, this);
    datamodel.curSave.fileSections[9].Subscribe(::RefreshF8, this);
    datamodel.curSave.fileSections[11].Subscribe(::RefreshF10, this);
    datamodel.curSave.fileSections[14].Subscribe(::RefreshF13, this);
    datamodel.curSave.fileSections[16].Subscribe(::RefreshF15, this);
    datamodel.curSave.fileSections[19].Subscribe(::RefreshF18, this);
    datamodel.curSave.fileSections[20].Subscribe(::RefreshF19, this);

    SetWidgetData(txtTicks, 0, TICKS_OFFSET);

    SetWidgetData(txtXPos, 20, WORLD_MAP_X_OFFSET);
    SetWidgetData(txtYPos, 20, WORLD_MAP_Y_OFFSET);
    SetWidgetData(chCarMap, 20, WORLD_MAP_CAR_MAP_OFFSET);
    SetWidgetData(txtFuel, 20, WORLD_MAP_FUEL_OFFSET);

    for (size_t i = 0; i < NTST_ARRAY_LEN(chTags); ++i)
        SetWidgetData(chTags[i], 9, i * sizeof(int32));

    SetWidgetData(txtExperience, 14, EXPERIENCE_OFFSET);
    SetWidgetData(txtSkillPoints, 14, SKILL_POINTS_OFFSET);

    SetWidgetData(txtHitpoints, 6, HITPOINTS_OFFSET);
    SetWidgetData(txtRadiation, 6, RADIATION_LEVEL_OFFSET);
    SetWidgetData(txtPoison, 6, POISON_LEVEL_OFFSET);

    SetWidgetData(chTrait1, 16, 0);
    SetWidgetData(chTrait2, 16, sizeof(int32));

    pairCity.Set(lbCity, txtCityVal, 0, 0);
    SetWidgetLbTxtPairData(lbCity, txtCityVal, &pairCity);

    pairStatBase.Set(lbBaseStats, txtBaseStatVal, 7, BASE_STAT_OFFSET);
    SetWidgetLbTxtPairData(lbBaseStats, txtBaseStatVal, &pairStatBase);
    pairStatBonus.Set(lbBonusStats, txtBonusStatVal, 7, BONUS_STAT_OFFSET);
    SetWidgetLbTxtPairData(lbBonusStats, txtBonusStatVal, &pairStatBonus);
    pairSkill.Set(lbSkills, txtSkillVal, 7, SKILLS_OFFSET);
    SetWidgetLbTxtPairData(lbSkills, txtSkillVal, &pairSkill);

    pairKill.Set(lbKills, txtKillVal, 8, 0);
    SetWidgetLbTxtPairData(lbKills, txtKillVal, &pairKill);

    pairPerk.Set(lbPerks, txtPerkVal, 11, PERKS_OFFSET);
    SetWidgetLbTxtPairData(lbPerks, txtPerkVal, &pairPerk);

    pairSfallGlobals.Set(lbSFallGVar, txtSFallGvarVal, 0, 0);
    SetWidgetLbTxtPairData(lbSFallGVar, txtSFallGvarVal, &pairSfallGlobals);

    LoadWindowRect();
}

F12seFrame::~F12seFrame()
{
    //(*Destroy(F12seFrame)
    //*)
    IniFile iniFile;
    iniFile.Load(datamodel.GetSettingsPath());
    IniSection* sec = iniFile.FindSectionOrCreate("window");
    if (sec == NULL)
        return;
    const bool isMaximized = IsMaximized();
    if (!isMaximized)
        {
        wxRect rect = GetScreenRect();
        sec->SetValue("x", rect.GetX());
        sec->SetValue("y", rect.GetY());
        sec->SetValue("w", rect.GetWidth());
        sec->SetValue("h", rect.GetHeight());
        }
    sec->SetValue("maximized", isMaximized);
    iniFile.Save(datamodel.GetSettingsPath());
    ntst_log_set_callback(NULL, NULL);
    if (flog != NULL)
        fclose(flog);
}

wxString F12seFrame::GetF12seTitleStart() const
    {
    return wxString(TITLE_BEGIN"Build: ") + __DATE__ " " + wxVERSION_STRING + ", zlib " ZLIB_VERSION;
    }

void F12seFrame::AddLogStr(const std::string& str)
    {
    if (flog == NULL)
        flog = fopen("F12se.log", "wb");
    if (flog != NULL)
        fwrite(str.c_str(), str.size(), 1, flog);
    if (txtLog->GetLastPosition() > 0)
        txtLog->ShowPosition(txtLog->GetLastPosition() - 1);
    txtLog->AppendText(wxString::FromUTF8(str.c_str(), str.size()));
    }

#include <wx/display.h>

void F12seFrame::LoadWindowRect()
    {
    IniFile iniFile;
    iniFile.Load(datamodel.GetSettingsPath());
    IniSection* sec = iniFile.FindSection("window");
    if (sec == NULL)
        return;
    wxRect rect = GetScreenRect();
    rect.SetX(sec->GetValueInt("x", rect.GetX()));
    rect.SetY(sec->GetValueInt("y", rect.GetY()));
    int width = sec->GetValueInt("w", rect.GetWidth());
    if (width < 300)
        width = 640;
    rect.SetWidth(width);
    int height = sec->GetValueInt("h", rect.GetHeight());
    if (height < 300)
        height = 480;
    rect.SetHeight(height);
    wxDisplay display(wxDisplay::GetFromWindow(this));
    wxRect displayrect = display.GetGeometry();
    if (rect.GetX() + rect.GetWidth() > displayrect.GetWidth())
        rect.SetX(displayrect.GetWidth() - rect.GetWidth());
    if (rect.GetY() + rect.GetHeight() > displayrect.GetHeight())
        rect.SetY(displayrect.GetHeight() - rect.GetHeight());
    if (rect.GetX() < 0)
        rect.SetX(0);
    if (rect.GetY() < 0)
        rect.SetY(0);
    SetSize(rect);
    Maximize(sec->GetValueBool("maximized", false));
    }

#ifndef _MSC_VER
#pragma GCC diagnostic pop
#endif // _MSC_VER

void F12seFrame::OnQuit(wxCommandEvent&)
{
    Close();
}

unsigned MapNumToIndex(int32 mapNum)
    {
    return mapNum & 0xFF;
    }

unsigned MapNumToLevel(int32 mapNum)
    {
    return (mapNum >> 8) & 0xFF;
    }

void MapNumToIndexAndLevel(int32 mapNum, unsigned &mapIndex, unsigned &mapLevel)
    {
    mapIndex = MapNumToIndex(mapNum);
    mapLevel = MapNumToLevel(mapNum);
    }

int32 MapIndexAndLevelToNum(unsigned mapIndex, unsigned mapLevel)
    {
    return (mapIndex & 0xFF) | ((mapLevel & 0xFF) << 8);
    }

void F12seFrame::RefreshHeader(MemBufferEX& head, size_t offset, size_t count)
    {
    if (head.empty())
        {
        ClearHeader();
        return;
        }
    if (BlocksOverlap(PLAYER_NAME_OFFSET, PLAYER_NAME_SIZE, offset, count))
        txtPlayerName->ChangeValue(ReadString(0, PLAYER_NAME_OFFSET, PLAYER_NAME_SIZE));
    if (BlocksOverlap(SAVEGAME_NAME_OFFSET, SAVEGAME_NAME_SIZE, offset, count))
        txtSaveName->ChangeValue(ReadString(0, SAVEGAME_NAME_OFFSET, SAVEGAME_NAME_SIZE));
    if (BlocksOverlap(TICKS_OFFSET, sizeof(int32), offset, count))
        HeaderUpdateDateTime(ReadInt32(0, TICKS_OFFSET));

    if (BlocksOverlap(CURRENT_MAP_OFFSET, sizeof(int32), offset, count))
        {
        int32 mapNum = head.GetInv32(CURRENT_MAP_OFFSET);
        unsigned mapIndex, mapLevel;
        MapNumToIndexAndLevel(mapNum, mapIndex, mapLevel);
        //chMap->Select(mapIndex);
        txtLocalMapLevel->ChangeValue(wxString() << mapLevel);
        }

    if (BlocksOverlap(BITMAP_OFFSET, BITMAP_SIZE, offset, count))
        {
        if (head.GetSize() >= BITMAP_OFFSET + BITMAP_SIZE)
            {
            RGBImage image(head, BITMAP_OFFSET, datamodel.GetMainPalette(), BITMAP_WIDTH, BITMAP_HEIGHT);
            bmScreen->SetImage(wxImage(BITMAP_WIDTH, BITMAP_HEIGHT, (unsigned char*)image.rgbArray.GetAddress(), true));
            }
        }
    }

void F12seFrame::ClearHeader()
    {
    txtPlayerName->ChangeValue(wxString());
    txtSaveName->ChangeValue(wxString());
    txtTicks->ChangeValue(wxString());

    chYears->Select(wxNOT_FOUND);
    chMonths->Select(wxNOT_FOUND);
    txtDay->ChangeValue(wxString());
    txtHour->ChangeValue(wxString());
    txtMinute->ChangeValue(wxString());
    txtSecond->ChangeValue(wxString());
    txtTick->ChangeValue(wxString());

    bmScreen->Clear();
    }

void F12seFrame::RefreshF2(MemBufferEX& buf, size_t offset, size_t count)
    {
    if (buf.empty())
        {
        ClearF2();
        return;
        }
    RefreshListBox(lbGvar, buf, 0, datamodel.vault13gam.size() * sizeof(int32), offset, count, datamodel.vault13gam);
    }

void F12seFrame::ClearF2()
    {
    lbGvar->Clear();
    txtGvarVal->ChangeValue(wxString());
    txtGvarDescription->Clear();
    }

void F12seFrame::RefreshF3(MemBufferEX& buf, size_t /*offset*/, size_t)
    {
    lbSavFiles->Clear();
    if (buf.GetSize() <= 2*sizeof(int32))
        {
        if (buf.GetSize() > 0)
            ntst_log_warning("Invalid data in function 3. There should be at least one map.");
        return;
        }
    const size_t endOffset = buf.GetSize() - sizeof(int32);
    size_t curOffset = 4;
    uint32 mapCount = buf.GetInv32(0);
    std::string mapName;
    mapName.reserve(MAP_FILENAME_MAX_LENGTH);
    for (unsigned map = 0; map < mapCount; ++map)
        {
        size_t mapNameStartOffset = curOffset;
        char c;
        while ((c = buf.Get8(curOffset)) != 0)
            {
            mapName += c;
            ++curOffset;
            if (curOffset >= endOffset)
                {
                ntst_log_warning("Invalid data in function 3.");
                return;
                }
            }
        ++curOffset;
        lbSavFiles->Append(mapName, (void*)mapNameStartOffset);
        mapName.erase();
        }
    }

bool GetItemPidTypeSyze(const MemBufferEX& buf, unsigned offset, const ProWorker& pw, int32 *itemPID, int32 *itemType, unsigned *itemSize)
    {
    if (!buf.GetInv32(offset + INVEN_ITEM_PID, itemPID))
        return false;
    if (!pw.Get4b(*itemPID, 0x20, itemType))
        return false;
    return GetItemSizeFromType(*itemType, itemSize);
    }

void LoadContainer(size_t &curOffset, int32 itemsCount, wxTreeCtrl *tree, wxTreeItemId& parent, const MemBufferEX& buf, const DataModel& datamodel)
    {
    for (int32 i = 0; i < itemsCount; i++)
        {
        int32 itemPID, itemType;
        unsigned itemSize;
        if (!GetItemPidTypeSyze(buf, curOffset, datamodel.itemsPro, &itemPID, &itemType, &itemSize))
            return;
        wxString itemName = datamodel.GetItemNameInCurSave(curOffset);
        wxTreeItemId parentItemId = tree->AppendItem(parent, itemName, curOffset);
        tree->SetItemData(parentItemId, new ItemTreeData(curOffset)); // TODO check if leak
        //проверка предмета на контейнерность
        if (itemType == ITMT_CONTAINER)
            {
            int32 subitemsCount = buf.GetInv32(curOffset + 0x4C);
            curOffset += itemSize;
            if (subitemsCount > 0)
                LoadContainer(curOffset, subitemsCount, tree, parentItemId, buf, datamodel);
            }
        else
            curOffset += itemSize;
        }
    }

void F12seFrame::RefreshF5(MemBufferEX& buf, size_t offset, size_t count)
    {
    SetWidgetFromBufIfReq(txtHitpoints, buf, HITPOINTS_OFFSET, offset, count);
    SetWidgetFromBufIfReq(txtRadiation, buf, RADIATION_LEVEL_OFFSET, offset, count);
    SetWidgetFromBufIfReq(txtPoison, buf, POISON_LEVEL_OFFSET, offset, count);
    if (BlocksOverlap(LOCAL_COORD_OFFSET, sizeof(int32), offset, count))
        {
        uint32 coord = buf.GetInv32(LOCAL_COORD_OFFSET);
        unsigned x = coord % 200;
        unsigned y = coord / 200;
        txtLocalX->ChangeValue(wxString() << x);
        txtLocalY->ChangeValue(wxString() << y);
        }
    if (BlocksOverlap(0x64, sizeof(int32), offset, count))
        {
        uint32 flags = buf.GetInv32(0x64);
        cbCrippledEye->SetValue(flags == (flags | 0x40));
        cbCrippledRArm->SetValue(flags == (flags | 0x20));
        cbCrippledLArm->SetValue(flags == (flags | 0x10));
        cbCrippledRLeg->SetValue(flags == (flags | 0x08));
        cbCrippledLLeg->SetValue(flags == (flags | 0x04));
        }

    if (BlocksOverlap(0x24, sizeof(int32), offset, count))
        {
        if (buf.GetInv32(0x24) & 0x18)
            cbGhostMode->SetValue(true);
        else
            cbGhostMode->SetValue(false);
        }
    //Items
    tItems->UnselectAll(); // otherwise DeleteAllItems selects items
    tItems->DeleteAllItems();
    txtItemQ->ChangeValue(wxString());
    if (buf.GetSize() < ITEMS_COUNT_OFFSET + sizeof(int32))
        return;
    int32 itemsCount = buf.GetInv32(ITEMS_COUNT_OFFSET);
    size_t curOffset = FIRST_ITEM_OFFSET;
    wxTreeItemId parentItemId = tItems->AddRoot("dummy");
    //wxTreeItemId parentItemId = tItems->GetRootItem();
    LoadContainer(curOffset, itemsCount, tItems, parentItemId, buf, datamodel);
    }

void F12seFrame::RefreshF6(MemBufferEX& buf, size_t offset, size_t count)
    {
    if (buf.empty())
        {
        ClearF6();
        return;
        }
    RefreshListBox(lbBaseStats, buf, BASE_STAT_OFFSET, STAT_SIZE, offset, count, datamodel.stats);
    RefreshListBox(lbBonusStats, buf, BONUS_STAT_OFFSET, STAT_SIZE, offset, count, datamodel.stats);
    RefreshListBox(lbSkills, buf, SKILLS_OFFSET, SKILLS_SIZE, offset, count, datamodel.skills);
    }

void F12seFrame::ClearF6()
    {
    lbBaseStats->Clear();
    lbBonusStats->Clear();
    lbSkills->Clear();
    txtBaseStatVal->ChangeValue(wxString());
    txtBonusStatVal->ChangeValue(wxString());
    txtSkillVal->ChangeValue(wxString());
    }

void F12seFrame::RefreshF7(MemBufferEX& buf, size_t offset, size_t count)
    {
    if (buf.empty())
        {
        ClearF7();
        return;
        }
    RefreshListBox(lbKills, buf, KILLS_COUNT_OFFSET, datamodel.GetKillTypesCount() * sizeof(int32), offset, count, datamodel.killTypes);
    }

void F12seFrame::ClearF7()
    {
    lbKills->Clear();
    txtKillVal->ChangeValue(wxString());
    }

void F12seFrame::RefreshF8(MemBufferEX& buf, size_t offset, size_t count)
    {
    for (unsigned i = 0; i < NTST_ARRAY_LEN(chTags); ++i)
        SetWidgetFromBufIfReq(chTags[i], buf, i * sizeof(int32), offset, count);
    }

void F12seFrame::RefreshF10(MemBufferEX& buf, size_t offset, size_t count)
    {
    if (buf.empty())
        {
        lbPerks->Clear();
        txtPerkVal->ChangeValue(wxString());
        return;
        }
    RefreshListBox(lbPerks, buf, PERKS_OFFSET, datamodel.GetPerksCount() * sizeof(int32), offset, count, datamodel.perks);
    }

void F12seFrame::RefreshF13(MemBufferEX& buf, size_t offset, size_t count)
    {
    SetWidgetFromBufIfReq(txtSkillPoints, buf, SKILL_POINTS_OFFSET, offset, count);
    SetWidgetFromBufIfReq(txtLevel, buf, PLAYER_LEVEL_F13_OFFSET, offset, count);
    SetWidgetFromBufIfReq(txtExperience, buf, EXPERIENCE_OFFSET, offset, count);
    }

void F12seFrame::RefreshF15(MemBufferEX& buf, size_t offset, size_t count)
    {
    SetWidgetFromBufIfReq(chTrait1, buf, 0x00, offset, count);
    SetWidgetFromBufIfReq(chTrait2, buf, 0x04, offset, count);
    }

void F12seFrame::RefreshF18(MemBufferEX& buf, size_t offset, size_t count)
    {
    if (buf.GetSize() < 0x05)
        {
        cbPickPerk->SetValue(false);
        return;
        }
    if (BlocksOverlap(0x04, sizeof(int8), offset, count))
        cbPickPerk->SetValue(buf.Get8(0x04));
    }

void F12seFrame::RefreshF19(MemBufferEX& buf, size_t offset, size_t count)
    {
    if (buf.empty())
        {
        lbCity->Clear();
        txtCityVal->ChangeValue(wxString());
        return;
        }
    SetWidgetFromBufIfReq(txtXPos, buf, WORLD_MAP_X_OFFSET, offset, count);
    SetWidgetFromBufIfReq(txtYPos, buf, WORLD_MAP_Y_OFFSET, offset, count);
    SetWidgetFromBufIfReq(chCarMap, buf, WORLD_MAP_CAR_MAP_OFFSET, offset, count);
    SetWidgetFromBufIfReq(txtFuel, buf, WORLD_MAP_FUEL_OFFSET, offset, count);

    if (offset + count < WORLD_MAP_CITIES_NUM_OFFSET + sizeof(int32))
        return;
    unsigned citiesCount = buf.GetInv32(WORLD_MAP_CITIES_NUM_OFFSET);
    if (citiesCount != datamodel.cityTxt.Count())
        ntst_log_error("Number of cities in savegame is not equal to the number in the Data\\city.txt. Something is very wrong.");
    const int minimalSizeOfCityRecord = 0x14;
    if (citiesCount * minimalSizeOfCityRecord + WORLD_MAP_CITIES_NUM_OFFSET + sizeof(int32) >= buf.GetSize())
        {
        ntst_log_error("Number of cities in savegame is wrong(" + ntst::to_string(citiesCount) + "). Loading aborted.");
        numberOfTilesOffset = 0;
        return;
        }
    unsigned curOffset = 0x30;
    for (unsigned i = 0; i < citiesCount; ++i)
        {
        if (i < lbCities->GetCount())
            lbCities->SetClientData(i, (void*)curOffset);
        int32 entrNum = buf.GetInv32(curOffset + 0x10);
        curOffset += 0x14 + (entrNum * 4);
        }
    numberOfTilesOffset = curOffset;
    }

void F12seFrame::RefreshSaveSlots(const std::vector<DataModel::Slot>& slots)
    {
    lbSaves->Clear();
    for (std::vector<DataModel::Slot>::const_iterator it = slots.begin(); it != slots.end(); ++it)
        lbSaves->Append(it->GetNameToShow());
    }

void F12seFrame::UpdateViewAfterInitAll(DataModel& model)
    {
    switch (model.settings.imageResizeQuality)
        {
        case 0:
            bmScreen->SetImageResizeQuality(wxIMAGE_QUALITY_NEAREST);
            break;
        case 2:
            bmScreen->SetImageResizeQuality(wxIMAGE_QUALITY_BICUBIC);
            break;
        default:
            bmScreen->SetImageResizeQuality(wxIMAGE_QUALITY_BILINEAR);
            break;
        }
    model.curSave.Close();
    btnSave->Enable(false);
    mSave->Enable(false);
    mSaveAs->Enable(false);
    btnEditCritters->Enable(!model.IsFallout1());
    // Header
    chYears->Clear();
    const int startYear = model.GetStartYear();
    for (int i = 0; i <= 14; i++)
        chYears->Append(ntst::to_string(startYear + i));
    chMonths->Clear();
    for (int i = 0; i < MONTHS_CNT; i++)
        chMonths->Append(model.months[i]);
    // Gvars
    lbHolodisks->Clear();
    lbKarmas->Clear();
    chQLoc->Clear();
    chQDesc->Clear();
    txtQDisplay->Clear();
    txtQComplete->Clear();
    lbHolodisks->Enable(!model.IsFallout1());
    lbKarmas->Enable(!model.IsFallout1());
    chQLoc->Enable(!model.IsFallout1());
    chQDesc->Enable(!model.IsFallout1());
    if (!model.IsFallout1())
        {
        for (size_t i = 0; i < model.holodiskTxt.Rows(); i++)
            lbHolodisks->Append(model.pipboyMSG.GetText(model.holodiskTxt.Get(i, 1)));
        for (size_t i = 0; i < model.karmavarTxt.Rows(); i++)
            lbKarmas->Append(model.editorMSG.GetText(model.karmavarTxt.Get(i, 2)));
        for (size_t i = 0; i < model.questsTxt.records.size(); i++)
            chQLoc->Append(model.mapMSG.GetText(model.questsTxt.Get(model.questsTxt.records[i].start, 0)));
        }
    // Tags
    for (unsigned tag = 0; tag < NTST_ARRAY_LEN(chTags); ++tag)
        {
        chTags[tag]->Clear();
        chTags[tag]->Append(wxString());
        for (unsigned skill = 0; skill < model.skills.size(); ++skill)
            chTags[tag]->Append(model.skills[skill]);
        }
    // Traits
    chTrait1->Clear();
    chTrait1->Append(wxString());
    for (unsigned trait = 0; trait < model.traits.size(); ++trait)
        chTrait1->Append(model.traits[trait]);
    chTrait2->Clear();
    chTrait2->Append(chTrait1->GetStrings());
    // Cities
    lbCities->Clear();
    chCarMap->Clear();
    txtCityVal->Enable(!model.IsFallout1());
    txtXPos->Enable(!model.IsFallout1());
    txtYPos->Enable(!model.IsFallout1());
    txtFuel->Enable(!model.IsFallout1());
    chCarMap->Enable(!model.IsFallout1());
    chCarMap->Append(wxString());
    for (size_t i = 0; i < model.cityTxt.Count(); ++i)
        {
        wxString name = model.mapMSG.GetText(1500 + i) + '(' + model.cityTxt.Name(i) + ')';
        lbCities->Append(name);
        chCarMap->Append(name);
        }
    // Maps
    /*chMap->Clear();
    unsigned mapnum = 0;
    for (IniFile::const_iterator it = model.mapsTxt.begin(); it != model.mapsTxt.end(); ++it, ++mapnum)
        {
        unsigned mapnumInFile;
        int result = sscanf(it->second.GetName().c_str(), "Map %u", &mapnumInFile);
        if (result == 1)
            {
            if (mapnum != mapnumInFile)
                ntst_log_warning("Seems like section name(" + it->second.GetName() + ") in maps.txt is invalid.");
            }
        else
            ntst_log_warning("Strange section name in maps.txt: " + it->second.GetName());

        std::string name = it->second.GetValue("lookup_name", it->second.GetName()) + '(' + it->second.GetValue("map_name", "ERROR! map_name not found!") + ')';
        chMap->Append(name);
        }*/
    if (model.settings.NeedLocalize())
        {
        localization.LoadFile(model.GetDirLng() + '\\' + model.settings.lngFile);
        Localize(localization);
        }
    //pnlWorldmap->Enable(!model.IsFallout1());
    //cbPickPerk->Show(!model.IsFallout1());
    SetTitle(GetF12seTitleStart() + " - " + datamodel.GetFalloutDir());
    pnlMain->Layout();
    //GenerateLocalizationFile();
    }

void F12seFrame::HeaderUpdateDateTime(int32 ticks)
    {
    uint year, month, day, hour, minute, sec, tick;
    datamodel.TicksToDate(ticks, year, month, day, hour, minute, sec, tick);
    chYears->Select(year);
    chMonths->Select(month);
    txtDay->ChangeValue(wxString() << day);
    txtHour->ChangeValue(wxString() << hour);
    txtMinute->ChangeValue(wxString() << minute);
    txtSecond->ChangeValue(wxString() << sec);
    txtTick->ChangeValue(wxString() << tick);
    txtTicks->ChangeValue(wxString() << ticks);
    }

void F12seFrame::SetWidgetFromBuf(wxTextCtrl *txt, const MemBufferEX& buf, size_t offset)
    {
    if (buf.GetSize() >= offset + sizeof(int32))
        {
        if (!txt->HasFocus())
            txt->ChangeValue(wxString() << buf.GetInv32(offset));
        txt->Enable(true);
        }
    else
        {
        txt->ChangeValue(wxString());
        txt->Enable(false);
        }
    txt->Refresh();
    }

void F12seFrame::SetWidgetFromBuf(wxChoice *ch, const MemBufferEX& buf, size_t offset)
    {
    uint32 index;
    if (buf.GetSize() >= offset + sizeof(int32)
        && (index = buf.GetInv32(offset) + 1) < ch->GetCount())
        {
        ch->Select(index);
        ch->Enable(true);
        }
    else
        {
        ch->Select(wxNOT_FOUND);
        ch->Enable(false);
        }
    ch->Refresh();
    }

void F12seFrame::RefreshHexView()
    {
    size_t offset = 0;
    for (size_t i = 0; i < NTST_ARRAY_LEN(functions); ++i)
        {
        size_t size = datamodel.curSave.fileSections[i].GetSize();
        std::string str = functions[i];
        if (size > 0)
            str = str + " o:" + ntst::to_string_hex_pref(offset) + " s:" + ntst::to_string_hex_pref(size);
        lbFunctions->SetString(i, str.c_str());
        offset += size;
        }
    RefreshSelectedHex();
    pnlHex->Layout();
    }

void FillListBoxWithFakePerks(wxListBox* lb, const SFallGV::FakePerks& fakes)
    {
    lb->Clear();
    for (SFallGV::FakePerks::const_iterator it = fakes.begin(); it != fakes.end(); ++it)
        {
        lb->Append(it->GetString());
        }
    }

static wxString GetLabelForSfallArrayElem(const ArrayNew* ar, const ArrayNew::KeyValPair* kv = NULL)
{
    wxString ret;
    if (kv)
    {
        std::string str = kv->first.GetString() + '=' + kv->second.GetString();
        ret = wxString::From8BitData(str.data(), str.size());
    }
    else if (ar)
    {
        std::string key = ar->key.GetString();
        ret = wxString::From8BitData(key.data(), key.size());
    }
    else
        ret = "NULL PTR BUG";
    return ret;
}

void F12seFrame::AfterLoadSaveDat()
{
    SetTitle(TITLE_BEGIN + datamodel.curSave.GetPath());
    btnSave->Enable();
    mSave->Enable();
    mSaveAs->Enable();
    btnEditCritters->Enable(!datamodel.IsFallout1());
    RefreshHexView();
    // SFALL
    std::string sfallgvpath = datamodel.curSave.GetDir() + DIR_SEPARATOR SFALLGVSAV;
    const bool sfallgvExist = FileExists(sfallgvpath);
    if (sfallgvExist)
    {
        try
        {
            sfallgv.Load(sfallgvpath);
        }
        catch (std::exception& e)
        {
            ntst_log_error(std::string("sfallgv.sav loading failed. Exception: ") + e.what());
            sfallgv.Clear();
        }
    }
    else
        sfallgv.Clear();
    btnSaveSFallgv->Enable(sfallgv.mIsValid);
    lbSFallGVar->Clear();
    for (SFallGV::GlobalVars::const_iterator it = sfallgv.globalVars.begin(); it != sfallgv.globalVars.end(); ++it)
    {
        lbSFallGVar->Append(it->GetString());
    }
    FillListBoxWithFakePerks(lbSFallTraits, sfallgv.fakeTraits);
    txtSFallTraitDesc->Clear();
    FillListBoxWithFakePerks(lbSFallPerks, sfallgv.fakePerks);
    txtSFallPerkDesc->Clear();
    FillListBoxWithFakePerks(lbSFallSelPerks, sfallgv.fakeSelectablePerks);
    txtSFallSelPerkDesc->Clear();
    tSfallArrays->UnselectAll(); // otherwise DeleteAllItems selects items
    tSfallArrays->DeleteAllItems();
    wxTreeItemId rootId = tSfallArrays->AddRoot("dummy");
    for (size_t i = 0; i < sfallgv.arraysNew.size(); ++i)
    {
        ArrayNew& ar = sfallgv.arraysNew[i];
        wxString label = GetLabelForSfallArrayElem(&ar);
        wxTreeItemId arrayId = tSfallArrays->AppendItem(rootId, label);
        tSfallArrays->SetItemData(arrayId, new ArrayTreeData(i)); // TODO check if leak
        for (size_t j = 0; j < ar.data.size(); ++j)
        {
            const ArrayNew::KeyValPair& kv = ar.data[j];
            std::string str = kv.first.GetString() + '=' + kv.second.GetString();
            label = GetLabelForSfallArrayElem(&ar, &kv);
            wxTreeItemId elemId = tSfallArrays->AppendItem(arrayId, label);
            tSfallArrays->SetItemData(elemId, new ArrayTreeData(i, j)); // TODO check if leak
        }
    }
}

void F12seFrame::OnlbSavesSelect(wxCommandEvent&)
    {
    int selected = lbSaves->GetSelection();
    if (selected == wxNOT_FOUND)
        return;
    LoadSaveDat(selected);
    }

void F12seFrame::OnbtnRefreshClick(wxCommandEvent&)
    {
    datamodel.RefreshSaveSlots();
    }

void F12seFrame::OntxtSaveNameText(wxCommandEvent&)
    {
    std::wstring wstr = txtSaveName->GetValue().ToStdWstring().c_str();
    WriteString(0, SAVEGAME_NAME_OFFSET, SAVEGAME_NAME_SIZE, datamodel.WideToGameString(wstr).c_str());
    }

void F12seFrame::OntxtPlayerNameText(wxCommandEvent&)
    {
    std::wstring wstr = txtPlayerName->GetValue().ToStdWstring();
    WriteString(0, PLAYER_NAME_OFFSET, PLAYER_NAME_SIZE, datamodel.WideToGameString(wstr).c_str());
    }

bool F12seFrame::wxChoiceToInt(int32 &varName, wxChoice *widget)
    {
    if (widget->GetSelection() != wxNOT_FOUND)
        {
        varName = widget->GetSelection();
        return true;
        }
    return false;
    }

void F12seFrame::OnDataChanged(wxCommandEvent&)
    {
    bool correct = true;

    int year, month;
    correct &= wxChoiceToInt(year, chYears);
    correct &= wxChoiceToInt(month, chMonths);

    int day, hour, minute, seconds, tick;

    correct &= wxTextToInt(day, txtDay);
    correct &= wxTextToInt(hour, txtHour);
    correct &= wxTextToInt(minute, txtMinute);
    correct &= wxTextToInt(seconds, txtSecond);
    correct &= wxTextToInt(tick, txtTick);

    if (!correct)
        return;

    uint ticks = datamodel.DateToTicks(
        year,
        month,
        day,
        hour,
        minute,
        seconds,
        tick);
    WriteInt32(0, TICKS_OFFSET, ticks);
    }

void F12seFrame::RefreshSelectedGvar()
    {
    txtGvarDescription->ChangeValue(wxString());
    txtGvarVal->ChangeValue(wxString());
    int selectedIndex = lbGvar->GetSelection();
    if (selectedIndex == wxNOT_FOUND)
        return;
    txtGvarDescription->ChangeValue(datamodel.vault13gam.GetVarDescription(selectedIndex));
    txtGvarVal->ChangeValue(wxString() << datamodel.curSave.GetGvarVal(selectedIndex));
    }

void F12seFrame::OnlbGvarSelect(wxCommandEvent&)
    {
    RefreshSelectedGvar();
    }

void F12seFrame::OnbtnFindGvarClick(wxCommandEvent&)
    {
    wxString strToFind = txtFindGvar->GetValue().Lower();
    int start;
    if (lbGvar->GetSelection() == wxNOT_FOUND)
        start = -1;
    else
        start = lbGvar->GetSelection();

    for (unsigned i = start + 1; i < lbGvar->GetCount(); ++i)
        {
        if (lbGvar->GetString(i).Lower().Find(strToFind) != wxNOT_FOUND)
            {
            lbGvar->Select(i);
            RefreshSelectedGvar();
            return;
            }
        }
    for (int i = 0; i <= start; ++i)
        {
        if (lbGvar->GetString(i).Lower().Find(strToFind) != wxNOT_FOUND)
            {
            lbGvar->Select(i);
            RefreshSelectedGvar();
            return;
            }
        }
    }

void F12seFrame::OntxtGvarValText(wxCommandEvent&)
    {
    const int selected = lbGvar->GetSelection();
    if (selected == wxNOT_FOUND)
        return;
    int val;
    bool correct = wxTextToInt(val, txtGvarVal);
    if (!correct)
        return;
    datamodel.curSave.SetGvarVal(selected, val);
    }

void F12seFrame::OnlbHolodisksSelect(wxCommandEvent&)
    {
    const int selected = lbHolodisks->GetSelection();
    if (selected == wxNOT_FOUND)
        return;
    lbGvar->Select(datamodel.holodiskTxt.Get(selected, 0));
    RefreshSelectedGvar();
    }

void F12seFrame::OnlbKarmasSelect(wxCommandEvent&)
    {
    const int selected = lbKarmas->GetSelection();
    if (selected == wxNOT_FOUND)
        return;
    lbGvar->Select(datamodel.karmavarTxt.Get(selected, 0));
    RefreshSelectedGvar();
    }

void F12seFrame::OnchQLocSelect(wxCommandEvent&)
    {
    const int selected = chQLoc->GetSelection();
    if (selected == wxNOT_FOUND)
        return;
    chQDesc->Clear();
    txtQDisplay->Clear();
    txtQComplete->Clear();
    for (int i = 0; i < datamodel.questsTxt.records[selected].length; i++)
        chQDesc->Append(datamodel.questsMSG.GetText(datamodel.questsTxt.Get(datamodel.questsTxt.records[selected].start + i, 1)));
    }

void F12seFrame::OnchQDescSelect(wxCommandEvent&)
    {
    const int selectedL = chQLoc->GetSelection();
    const int selectedD = chQDesc->GetSelection();
    if (selectedL == wxNOT_FOUND || selectedD == wxNOT_FOUND)
        return;
    int rowIndex = datamodel.questsTxt.records[selectedL].start + selectedD;
    txtQDisplay->ChangeValue(wxString() << datamodel.questsTxt.Get(rowIndex, 3));
    txtQComplete->ChangeValue(wxString() << datamodel.questsTxt.Get(rowIndex, 4));
    lbGvar->Select(datamodel.questsTxt.Get(rowIndex, 2));
    RefreshSelectedGvar();
    }

void F12seFrame::EditSelectedSav()
    {
    const int selected = lbSavFiles->GetSelection();
    if (selected == wxNOT_FOUND)
        return;
    std::string fullpath, filedir, filename;
    fullpath = datamodel.curSave.GetPath();
    if (!SplitPath(fullpath, filedir, filename))
        {
        ntst_log_warning("Can't get directory name from: " + fullpath);
        return;
        }
    std::string savfilename = WxStringToStdLocal(lbSavFiles->GetString(selected));
    fullpath = filedir + '\\' + savfilename;
    SAVfile savFile;
    if (!savFile.LoadFile(fullpath))
        return;

    std::string gampath = "maps\\" + ChangeFileExt(savfilename, ".gam");
    GamFile mapgam;
    if (datamodel.GetGamFile(gampath, mapgam))
        ntst_log_info(gampath + " file was found and has " + ntst::to_string(mapgam.size()) + " variables.");
    else
        ntst_log_info(gampath + " file was not found.");
    if (savFile.GetMvarNum() != mapgam.size())
        {
        ntst_log_warning("Files have different number of variables, " +
        fullpath + " has " + ntst::to_string(savFile.GetMvarNum()) +
        " but " + gampath + " has " + ntst::to_string(mapgam.size()));
        }

    SavEdit *savEditForm;
    try
        { savEditForm = new SavEdit(this, savFile, mapgam, datamodel.scriptList); }
    catch (...)
        { return; }
    if (datamodel.settings.NeedLocalize())
        localization.LocalizeWindow(savEditForm, SECTION_SAVEDIT);
    savEditForm->SetTitle(fullpath);
    savEditForm->Layout();
    savEditForm->Show();
    }

void F12seFrame::OnbtnEditSAVClick(wxCommandEvent&)
    {
    EditSelectedSav();
    }

void F12seFrame::OnbtnDelSavClick(wxCommandEvent&)
    {
    const int selected = lbSavFiles->GetSelection();
    if (selected == wxNOT_FOUND)
        return;
    size_t offset = (size_t)lbSavFiles->GetClientData(selected);
    if (offset == 0)
        {
        ntst_log_error("Strange logical error while map deletion(selected map offset equal 0).");
        return;
        }
    size_t bytesToDelete = lbSavFiles->GetString(selected).Length() + NULL_CHARACTER_SIZE;
    MemBufferEX &buf = datamodel.curSave.fileSections[4];
    buf.BeginUpdate();
    // Delete filename
    buf.Delete(offset, bytesToDelete);
    // Decrease the number of open maps
    const int32 size = buf.GetInv32(NUMBER_OF_OPEN_MAPS_OFFSET);
    if (size == 0)
        {
        ntst_log_error("Strange logical error while map deletion(number of maps equal 0).");
        return;
        }
    buf.SetInv32(NUMBER_OF_OPEN_MAPS_OFFSET, size - 1);
    buf.EndUpdate();
    }

void F12seFrame::RefreshSelectedHex()
    {
    int index = lbFunctions->GetSelection();
    if (index == wxNOT_FOUND || index >= (int)NTST_ARRAY_LEN(datamodel.curSave.fileSections))
        hexEditor->Clear();
    else
        hexEditor->SetData(datamodel.curSave.fileSections[index].GetAddress(), datamodel.curSave.fileSections[index].GetSize());
    }

void F12seFrame::OnlbFunctionsSelect(wxCommandEvent&)
    {
    RefreshSelectedHex();
    }

void F12seFrame::OnTxtText(wxCommandEvent& event)
    {
    wxTextCtrl *txt = (wxTextCtrl*)event.GetEventObject();
    int32 val;
    if (!wxTextToInt(val, txt))
        return;
    unsigned function;
    size_t offset;
    GetWidgetData(txt, &function, &offset);
    WriteInt32(function, offset, val);
    }

void F12seFrame::OnTxtLbText(wxCommandEvent& event)
    {
    wxTextCtrl *txt = (wxTextCtrl*)event.GetEventObject();
    PairTxtLb *p = (PairTxtLb*)txt->GetClientData();
    if (p->lb == NULL)
        {
        ntst_log_warning("Listbox is not set for " + WxStringToStdUTF8(txt->GetName()));
        return;
        }
    if (p->lb->GetSelection() == wxNOT_FOUND)
        return;
    int32 val;
    if (!wxTextToInt(val, txt))
        return;
    WriteInt32(p->function, p->offset + p->lb->GetSelection() * sizeof(int32), val);
    }

void F12seFrame::OnLbTxtSelect(wxCommandEvent& event)
    {
    wxListBox *lb = (wxListBox*)event.GetEventObject();
    PairTxtLb *p = (PairTxtLb*)lb->GetClientData();
    if (p->txt == NULL)
        {
        ntst_log_warning("TextCtrl is not set for " + WxStringToStdUTF8(lb->GetName()));
        return;
        }
    p->txt->ChangeValue(wxString() << ReadInt32(p->function, p->offset + lb->GetSelection() * sizeof(int32)));
    }

void F12seFrame::OnchSelect(wxCommandEvent& event)
    {
    wxChoice *ch = (wxChoice*)event.GetEventObject();
    int index = ch->GetSelection();
    if (index == wxNOT_FOUND)
        return;
    unsigned function;
    size_t offset;
    GetWidgetData(ch, &function, &offset);
    if (function == 0 && offset == 0)
        return;
    WriteInt32(function, offset, index - 1);
    }

void F12seFrame::SaveSaveDat(const std::string& path)
    {
    if (path == datamodel.curSave.GetPath() && datamodel.curSave.FileCrcChanged())
        {
        int answer = wxMessageBox(fileCrcChanged, wxString(), wxYES_NO | wxNO_DEFAULT, this);
        if (answer == wxNO)
            return;
        }
    int32* buf = (int32*)datamodel.curSave.fileSections[16].GetAddress();
    bool swapTraits = datamodel.settings.correctTraits
            && (datamodel.curSave.fileSections[16].GetSize() == 2*sizeof(int32))
            && (buf[0] == -1) && (buf[1] != -1);
    if (swapTraits)
        std::swap(buf[0], buf[1]);
    std::string errMsg;
    if (!datamodel.curSave.Save(path, errMsg))
        wxMessageBox(errMsg, "Can't save file!", wxOK|wxCENTRE|wxICON_ERROR);
    if (swapTraits)
        std::swap(buf[0], buf[1]);
    }

void F12seFrame::OnbtnSaveClick(wxCommandEvent&)
    {
    SaveSaveDat(datamodel.curSave.GetPath());
    }

void F12seFrame::OntxtPerkValText(wxCommandEvent&)
    {
    int perkNum = lbPerks->GetSelection();
    if (perkNum == wxNOT_FOUND)
        return;
    int32 val;
    if (!wxTextToInt(val, txtPerkVal))
        return;
    datamodel.ModPerkAndApplyCorrectionIfNeed(perkNum, val);
    }

void F12seFrame::OncbPickPerkClick(wxCommandEvent&)
    {
    if (cbPickPerk->IsChecked())
        WriteInt8(19, 4, 1);
    else
        WriteInt8(19, 4, 0);
    }

void F12seFrame::RefreshSelectedCity()
    {
    const int cityindex = lbCity->GetSelection();
    lbCity->Clear();
    if (datamodel.curSave.fileSections[20].empty())
        return; // to prevent error messages about reading empty buffer.
    int index = lbCities->GetSelection();
    if (index == wxNOT_FOUND)
        return;
    int offset = GetCityOffset(index);
    lbCity->Append(cityX + '=' + ntst::to_string(ReadInt32(20, offset)));
    lbCity->Append(cityY + '=' + ntst::to_string(ReadInt32(20, offset + 0x4)));
    lbCity->Append(cityState + '=' + ntst::to_string(ReadInt32(20, offset + 0x8)));
    lbCity->Append(cityButton + '=' + ntst::to_string(ReadInt32(20, offset + 0xC)));
    int entrNum = ReadInt32(20, offset + 0x10);
    offset += 0x14;
    for (int i = 0; i < entrNum; ++i)
        lbCity->Append(cityEntrance + ntst::to_string(i) + '=' + ntst::to_string(ReadInt32(20, offset + i * sizeof(int32))));
    // restore selection
    if (cityindex != wxNOT_FOUND && (unsigned)cityindex < lbCity->GetCount())
        lbCity->SetSelection(cityindex);
    }

void F12seFrame::OnlbCitiesSelect(wxCommandEvent&)
    {
    RefreshSelectedCity();
    }

bool F12seFrame::GetOffsetForSelectedCity(int *offset)
    {
    int indexy = lbCity->GetSelection();
    int indexs = lbCities->GetSelection();
    if (indexy == wxNOT_FOUND || indexs == wxNOT_FOUND)
        return false;
    *offset = indexy * 4;
    if (*offset >= 0x10)
        *offset += 4;
    *offset += GetCityOffset(indexs);
    return true;
    }

void F12seFrame::OnlbCitySelect(wxCommandEvent&)
    {
    int offset;
    if (!GetOffsetForSelectedCity(&offset))
        return;
    txtCityVal->SetValue(wxString() << ReadInt32(20, offset));
    }

void F12seFrame::OntxtCityValText(wxCommandEvent&)
    {
    int offset;
    if (!GetOffsetForSelectedCity(&offset))
        return;
    int32 val;
    if (!wxTextToInt(val, txtCityVal))
        return;
    WriteInt32Ninja(20, offset, val);
    RefreshSelectedCity();
    }

int F12seFrame::GetSelectedItemOffset()
    {
    wxTreeItemId itemId = tItems->GetSelection();
    if (!itemId.IsOk())
        return -1;
    if (tItems->GetItemData(itemId) == NULL)
        return -1;
    ItemTreeData *data = (ItemTreeData*)tItems->GetItemData(itemId);
    return data->GetOffset();
    }

void F12seFrame::OntItemsSelectionChanged(wxTreeEvent&)
    {
    txtItemQ->ChangeValue(wxString());
    int offset = GetSelectedItemOffset();
    if (offset == -1)
        return;
    txtItemQ->ChangeValue(wxString() << ReadInt32(6, offset));
    uint32 pid = ReadInt32(6, offset + INVEN_ITEM_PID);
    uint32 fid = datamodel.itemsPro.Get4b(pid, 0x34);
    LoadItemImage(fid);
    }

void F12seFrame::OntxtItemQText(wxCommandEvent&)
    {
    int offset = GetSelectedItemOffset();
    if (offset == -1)
        return;
    wxTreeItemId itemId = tItems->GetSelection();
    int32 val;
    if (!wxTextToInt(val, txtItemQ))
        return;
    WriteInt32Ninja(6, offset, val);
    tItems->SetItemText(itemId, datamodel.GetItemNameInCurSave(offset));
    }

void F12seFrame::OnlbSavFilesDClick(wxCommandEvent&)
    {
    EditSelectedSav();
    }

void F12seFrame::LoadItemImage(uint32 fid)
    {
    RGBAImage img;
    if (datamodel.frmWorker.LoadFrmByIdInRGBA(fid, datamodel.fileFinder, img))
        {
        wxImage wximg(img.GetWidth(), img.GetHeight(), (unsigned char*)img.rgbArray.GetAddress(), (unsigned char*)img.alphaArray.GetAddress(), true);
        sbItem->SetSize(wximg.GetSize());
        sbItem->SetBitmap(wxBitmap(wximg));
        pnlInventory->Layout();
        }
    }

std::vector<itemNamePid> GetSelectedItems(ProWorker& p, const MsgFile& msg, int32 itemType)
    {
    std::vector<itemNamePid> result;
    p.SelectAll();
    p.SelectEqual(0x20, itemType);
    result.reserve(p.GetSelectedProQuantity());
    for (unsigned i = 0; i < p.GetSelectedProQuantity(); ++i)
        {
        // TODO checks
        unsigned index = p.GetSelectedIndex(i);
        int32 pid = p.Get4bByIndex(index, PRO_PID);
        int32 msgLine = p.Get4bByIndex(index, PRO_DID);
        result.push_back(itemNamePid(msg.GetText(msgLine), pid));
        }
    return result;
    }

void F12seFrame::EditSelectedInventoryItem()
    {
    int offset = GetSelectedItemOffset();
    if (offset == -1)
        return;
    int32 itemPID, itemType;
    unsigned itemSize;
    if (!GetItemPidTypeSyze(datamodel.curSave.fileSections[6], offset, datamodel.itemsPro, &itemPID, &itemType, &itemSize))
        return;
    MemBuffer b;
    datamodel.curSave.fileSections[6].GetSubBuffer(offset, itemSize, b);
    ItemEdit dlg(this, &b, GetSelectedItems(datamodel.itemsPro, datamodel.proItemMSG, itemType));
    dlg.SelectPidIfPossible(itemPID);
    if (datamodel.settings.NeedLocalize())
        dlg.Localize(localization, SECTION_ITEMEDIT);
    dlg.Layout();
    dlg.Fit();
    if (dlg.ShowModal() == wxID_OK)
        {
        FixInventoryItem(b);
        datamodel.curSave.fileSections[6].Replace((size_t)offset, b.GetSize(), b.GetAddress());
        }
    }

void F12seFrame::OnbtnEditItemClick(wxCommandEvent&)
    {
    EditSelectedInventoryItem();
    }

void F12seFrame::OntItemsItemActivated(wxTreeEvent&)
    {
    EditSelectedInventoryItem();
    }

void F12seFrame::OnbtnCreateItemClick(wxCommandEvent&)
    {
    // Check savegame is loaded. minimal valid size is FIRST_ITEM_OFFSET if inventory is empty
    if (datamodel.curSave.fileSections[6].GetSize() >= FIRST_ITEM_OFFSET)
        PopupMenu(&menuCreateItem);
    }

void F12seFrame::OnbtnDeleteItemClick(wxCommandEvent&)
    {
    int offset = GetSelectedItemOffset();
    if (offset == -1)
        return;
    int32 itemPID, itemType;
    unsigned itemSize;
    if (!GetItemPidTypeSyze(datamodel.curSave.fileSections[6], offset, datamodel.itemsPro, &itemPID, &itemType, &itemSize))
        return;
    int32 itemsCount = ReadInt32(6, ITEMS_COUNT_OFFSET) - 1;
    if (itemType == ITMT_CONTAINER)
        {
        int32 subitemsCount = ReadInt32(6, offset + 0x4C);
        itemsCount += subitemsCount;
        }
    if (itemsCount < 0)
        {
        ntst_log_error("There is error somethere. itemsCount(" + ntst::to_string(itemsCount) + ") < 0.");
        return;
        }
    WriteInt32Ninja(6, ITEMS_COUNT_OFFSET, itemsCount);
    datamodel.curSave.fileSections[6].Delete(offset, itemSize);
    }

void F12seFrame::FixInventoryItem(MemBuffer& buf)
    {
    int32 pid;
    if (!buf.GetInv32(INVEN_ITEM_PID, &pid))
        return;
    int32 fid;
    if (datamodel.settings.correctItemFID && datamodel.itemsPro.Get4b(pid, PRO_FID, &fid))
        {
        int32 oldfid;
        if (buf.GetInv32(INVEN_ITEM_FID, &oldfid) && fid != oldfid && buf.SetInv32(INVEN_ITEM_FID, fid))
            ntst_log_info("Item's on-ground image was changed from " + ntst::to_string_hex_pref(oldfid) + " to " + ntst::to_string_hex_pref(fid));
        }
    if (datamodel.itemsPro.Get4b(pid, 0x20) == ITMT_WEAPON)
        {
        int32 ammoT, ammoQ;
        if (datamodel.settings.correctAmmoType && datamodel.itemsPro.Get4b(pid, PRO_AMMO_T, &ammoT))
            {
            int32 oldammoT;
            if (buf.GetInv32(INVEN_ITEM_AMMO_T, &oldammoT) && ammoT != oldammoT && buf.SetInv32(INVEN_ITEM_AMMO_T, ammoT))
                ntst_log_info("Weapon's type of ammunition was changed from " + ntst::to_string_hex_pref(oldammoT) + " to " + ntst::to_string_hex_pref(ammoT));
            }
        if (datamodel.settings.correctAmmoQuantity && datamodel.itemsPro.Get4b(pid, PRO_AMMO_Q, &ammoQ))
            {
            int32 oldammoQ;
            if (buf.GetInv32(INVEN_ITEM_AMMO_Q, &oldammoQ) && ammoQ > oldammoQ && buf.SetInv32(INVEN_ITEM_AMMO_Q, ammoQ))
                ntst_log_info("Weapon's number of loaded rounds was changed from " + ntst::to_string_hex_pref(oldammoQ) + " to " + ntst::to_string_hex_pref(ammoQ));
            }
        }
    }

static uint32 armor[0x17] =   {0x01000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static uint32 container[0x17]={0x01000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static uint32 drug[0x17] =    {0x01000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static uint32 weapon[0x19] =  {0x01000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF};
static uint32 ammo[0x18] =    {0x01000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x01000000};
static uint32 misc[0x18] =    {0x01000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static uint32 key[0x18] =     {0x01000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF};

void F12seFrame::CreateItem(unsigned itemType)
    {
    MemBuffer b;
    switch(itemType)
        {
        case ITMT_ARMOR:
            b.WriteMem(armor, sizeof(armor));
            break;
        case ITMT_CONTAINER:
            b.WriteMem(container, sizeof(container));
            break;
        case ITMT_DRUG:
            b.WriteMem(drug, sizeof(drug));
            break;
        case ITMT_WEAPON:
            b.WriteMem(weapon, sizeof(weapon));
            break;
        case ITMT_AMMO:
            b.WriteMem(ammo, sizeof(ammo));
            break;
        case ITMT_MISCITEM:
            b.WriteMem(misc, sizeof(misc));
            break;
        case ITMT_KEY:
            b.WriteMem(key, sizeof(key));
            break;
        default:
            return;
        }
    ItemEdit dlg(this, &b, GetSelectedItems(datamodel.itemsPro, datamodel.proItemMSG, itemType));
    dlg.SelectFirstIfPossible();
    if (datamodel.settings.NeedLocalize())
        dlg.Localize(localization, SECTION_ITEMEDIT);
    dlg.Layout();
    dlg.Fit();
    if (dlg.ShowModal() == wxID_OK)
        {
        int32 pid;
        if (!b.GetInv32(INVEN_ITEM_PID, &pid) || !datamodel.itemsPro.IsPidValid(pid))
            {
            ntst_log_warning("Can't create item with invalid proto ID.");
            return;
            }
        FixInventoryItem(b);
        int32 itemsCount = ReadInt32(6, ITEMS_COUNT_OFFSET) + 1;
        WriteInt32Ninja(6, ITEMS_COUNT_OFFSET, itemsCount);
        datamodel.curSave.fileSections[6].Insert(FIRST_ITEM_OFFSET, b.GetSize(), b.GetAddress());
        }
    }

void F12seFrame::OnbtnClearCombatClick(wxCommandEvent&)
    {
    if (datamodel.curSave.fileSections[12].GetSize() != 0)
        {
        uint32 val = 0x02000000;
        datamodel.curSave.fileSections[12].WriteMem(&val, sizeof(val));
        }
    }

void F12seFrame::OncbGhostModeClick(wxCommandEvent&)
    {
    int32 val = ReadInt32(6, 0x24);
    if (cbGhostMode->IsChecked())
        val = val | 0x10;//set ghost mode
    else
        val = val & 0xFFFFFFE7;//clear ghost mode
    WriteInt32(6, 0x24, val);
    }

void F12seFrame::OntxtLocalCoord(wxCommandEvent&)
    {
    int32 x;
    if (!wxTextToInt(x, txtLocalX))
        return;
    int32 y;
    if (!wxTextToInt(y, txtLocalY))
        return;
    if ((unsigned)x >= 200 || (unsigned)y >= 200)
        return;
    uint32 coord = y * 200 + x;
    datamodel.curSave.fileSections[6].SetInv32(LOCAL_COORD_OFFSET, coord);
    datamodel.curSave.fileSections[6].SetInv32(datamodel.curSave.fileSections[6].GetSize() - 4, coord); // camera position
    }

void F12seFrame::OntxtLocalMapLevelText(wxCommandEvent&)
    {
    int32 mapLevel;
    if (!wxTextToInt(mapLevel, txtLocalMapLevel))
        return;
    int32 mapNum = datamodel.curSave.fileSections[0].GetInv32(CURRENT_MAP_OFFSET);
    unsigned mapIndex = MapNumToIndex(mapNum);
    mapNum = MapIndexAndLevelToNum(mapIndex, mapLevel);
    datamodel.curSave.fileSections[0].SetInv32(CURRENT_MAP_OFFSET, mapNum);
    datamodel.curSave.fileSections[6].SetInv32(LOCAL_MAP_LEVEL_OFFSET, mapLevel);
    }

void F12seFrame::OnchMapSelect(wxCommandEvent&)
    {
    /*int index = chMap->GetSelection();
    if (index == wxNOT_FOUND)
        return;
    int32 mapNum = datamodel.curSave.fileSections[0].GetInv32(CURRENT_MAP_OFFSET);
    unsigned mapLevel = MapNumToLevel(mapNum);
    mapNum = MapIndexAndLevelToNum(index, mapLevel);

    IniFile::const_iterator it = datamodel.mapsTxt.begin();
    for (int i = 0; i < index; ++i)
        ++it;
    const std::string *mapName = it->second.FindValue("map_name");
    if (mapName != NULL)
        {
        std::string mapNameSav = *mapName + ".sav";
        WriteString(0, CURRENT_MAPNAME_OFFSET, CURRENT_MAPNAME_SIZE, mapNameSav.c_str());
        datamodel.curSave.fileSections[0].SetInv32(CURRENT_MAP_OFFSET, mapNum);
        }*/
    }

void F12seFrame::ModBodyFlag(bool needset, unsigned mask)
    {
    uint32 flags = datamodel.curSave.fileSections[6].GetInv32(0x64);
    if (needset)
        flags |= mask;
    else
        flags &= ~mask;
    datamodel.curSave.fileSections[6].BeginNinjaMode();
    datamodel.curSave.fileSections[6].SetInv32(0x64, flags);
    datamodel.curSave.fileSections[6].EndNinjaMode();
    }

void F12seFrame::OncbCrippledEyeClick(wxCommandEvent&)
    {
    ModBodyFlag(cbCrippledEye->IsChecked(), 0x40);
    }

void F12seFrame::OncbCrippledRArmClick(wxCommandEvent&)
    {
    ModBodyFlag(cbCrippledRArm->IsChecked(), 0x20);
    }

void F12seFrame::OncbCrippledLArmClick(wxCommandEvent&)
    {
    ModBodyFlag(cbCrippledLArm->IsChecked(), 0x10);
    }

void F12seFrame::OncbCrippledRLegClick(wxCommandEvent&)
    {
    ModBodyFlag(cbCrippledRLeg->IsChecked(), 0x08);
    }

void F12seFrame::OncbCrippledLLegClick(wxCommandEvent&)
    {
    ModBodyFlag(cbCrippledLLeg->IsChecked(), 0x04);
    }

extern wxString descriptionDefault[25];

void F12seFrame::GenerateLocalizationFile()
    {
    /*IniFile f;
    Localization::GenerateFileFromWindow(f, this, SECTION_MAINWINDOW);
    f.SetValue(SECTION_MAINWINDOW, "msgFileChanged", FILECRC_CHANGED);
    f.SetValue(SECTION_MAINWINDOW, "msgSFallNewGvar", SFALL_NEW_GVAR_MESSAGE);

    f.SetValue(SECTION_MAINWINDOW, "cityX", "x");
    f.SetValue(SECTION_MAINWINDOW, "cityY", "y");
    f.SetValue(SECTION_MAINWINDOW, "cityState", "state");
    f.SetValue(SECTION_MAINWINDOW, "cityButton", "button");
    f.SetValue(SECTION_MAINWINDOW, "cityEntrance", "entrance");

    for (unsigned i = 0; i < chWorldmapState->GetCount(); ++i)
        {
        f.SetValue(SECTION_MAINWINDOW, wxString() << "worldmapState" << i, chWorldmapState->GetString(i));
        }

    SettingsEdit se(this, datamodel, NULL);
    Localization::GenerateFileFromWindow(f, &se, SECTION_SETTINGSEDIT);
    MemBuffer b;
    ItemEdit ie(this, &b, GetSelectedItems(datamodel.itemsPro, datamodel.proItemMSG, 0));
    Localization::GenerateFileFromWindow(f, &ie, SECTION_ITEMEDIT);

    SavEdit *savEditForm;
    SAVfile savFile;
    GamFile mapgam;
    ScriptList scriptsList;
    savEditForm = new SavEdit(this, savFile, mapgam, scriptsList);
    Localization::GenerateFileFromWindow(f, savEditForm, SECTION_SAVEDIT);
    delete savEditForm;

    CrittersEdit *editCritters = new CrittersEdit(this, datamodel);
    Localization::GenerateFileFromWindow(f, editCritters, SECTION_CRITTERSEDIT);
    delete editCritters;

    for (unsigned i = 0; i < NTST_ARRAY_LEN(descriptionDefault); ++i)
        f.SetValue("ItemsOffsets", IntToHex(i * 4, 2), WxStringToStdUTF8(descriptionDefault[i]));
    IniSection *s = f.FindSectionOrCreate("Menu");
    s->SetValue("File", WxStringToStdUTF8(mFile->GetTitle()));
    s->SetValue("Open", WxStringToStdUTF8(mOpen->GetItemLabel()));
    s->SetValue("Save", WxStringToStdUTF8(mSave->GetItemLabel()));
    s->SetValue("SaveAs", WxStringToStdUTF8(mSaveAs->GetItemLabel()));
    s->SetValue("Settings", WxStringToStdUTF8(mSettings->GetItemLabel()));
    s->SetValue("Quit", WxStringToStdUTF8(mQuit->GetItemLabel()));

    s->SetValue("itmArmor", WxStringToStdUTF8(mArmor->GetItemLabel()));
    s->SetValue("itmContainer", WxStringToStdUTF8(mContainer->GetItemLabel()));
    s->SetValue("itmDrug", WxStringToStdUTF8(mDrug->GetItemLabel()));
    s->SetValue("itmWeapon", WxStringToStdUTF8(mWeapon->GetItemLabel()));
    s->SetValue("itmAmmo", WxStringToStdUTF8(mAmmo->GetItemLabel()));
    s->SetValue("itmMiscellaneous", WxStringToStdUTF8(mMiscellaneous->GetItemLabel()));
    s->SetValue("itmKey", WxStringToStdUTF8(mKey->GetItemLabel()));
    f.Save("default.lng");*/
    }

void LocalizeMenu(wxMenuItem *mi, const std::string& name, const std::string& defval, const IniSection *s)
    {
    const std::string str = s->GetValue(name, defval);
    mi->SetItemLabel(wxString::FromUTF8(str.c_str(), str.size()));
    }

wxString GetLocalizedString(const std::string& name, const std::string& defval, const IniSection& s)
    {
    const std::string str = s.GetValue(name, defval);
    return wxString::FromUTF8(str.c_str(), str.size());
    }

void F12seFrame::Localize(const Localization& l)
    {
    l.LocalizeWindow(this, SECTION_MAINWINDOW);
    IniSection mainsec = l.file.GetSection(SECTION_MAINWINDOW);
    fileCrcChanged = GetLocalizedString("msgFileChanged", FILECRC_CHANGED, mainsec);
    cityX = GetLocalizedString("cityX", "x", mainsec);
    cityY = GetLocalizedString("cityY", "y", mainsec);
    cityState = GetLocalizedString("cityState", "state", mainsec);
    cityButton = GetLocalizedString("cityButton", "button", mainsec);
    cityEntrance = GetLocalizedString("cityEntrance", "entrance", mainsec);
    SFallNewGvarMessage = GetLocalizedString("msgSFallNewGvar", SFALL_NEW_GVAR_MESSAGE, mainsec);

    for (unsigned i = 0; i < NTST_ARRAY_LEN(worldMapStates); ++i)
        {
        chWorldmapState->SetString(i, GetLocalizedString("worldmapState" + ntst::to_string(i), WxStringToStdUTF8(worldMapStates[i]), mainsec));
        }

    const IniSection *s = l.file.FindSection("Menu");
    if (s == NULL)
        return;
    mbMain->SetMenuLabel(0, GetLocalizedString("File", "File", *s));
    LocalizeMenu(mOpen, "Open", "&Open	CTRL+O", s);
    LocalizeMenu(mSave, "Save", "&Save	CTRL+S", s);
    LocalizeMenu(mSaveAs, "SaveAs", "Save as...	CTRL+SHIFT+S", s);
    LocalizeMenu(mSettings, "Settings", "Settings	F12", s);
    LocalizeMenu(mQuit, "Quit", "&Quit	CTRL+Q", s);

    LocalizeMenu(mArmor, "itmArmor", "Armor", s);
    LocalizeMenu(mContainer, "itmContainer", "Container", s);
    LocalizeMenu(mDrug, "itmDrug", "Drug", s);
    LocalizeMenu(mWeapon, "itmWeapon", "Weapon", s);
    LocalizeMenu(mAmmo, "itmAmmo", "Ammo", s);
    LocalizeMenu(mMiscellaneous, "itmMiscellaneous", "Miscellaneous", s);
    LocalizeMenu(mKey, "itmKey", "Key", s);
    }

void F12seFrame::OnbtnEditCrittersClick(wxCommandEvent&)
    {
    CrittersEdit *editWindow;
    try
        { editWindow = new CrittersEdit(this, datamodel); }
    catch (...)
        { return; }
    if (datamodel.settings.NeedLocalize())
        localization.LocalizeWindow(editWindow, SECTION_CRITTERSEDIT);
    editWindow->Layout();
    editWindow->Fit();
    editWindow->SetTitle(datamodel.curSave.GetCrittersDir());
    editWindow->Show();
    }

void F12seFrame::OntxtLevelText(wxCommandEvent&)
    {
    int32 val;
    if (!wxTextToInt(val, txtLevel))
        return;
    int32 levelCur = ReadInt32(14, 4);
    int leveldif = val - levelCur;
    WriteInt32(14, 4, val);
    int32 levelCur2 = ReadInt32(19, 0) + leveldif;
    WriteInt32(19, 0, levelCur2);
    }

void F12seFrame::OnbtnSaveSFallgvClick(wxCommandEvent&)
    {
    if (datamodel.curSave.GetDir().empty())
        return;
    std::string sfallgvpath = datamodel.curSave.GetDir() + DIR_SEPARATOR SFALLGVSAV;
    sfallgv.Save(sfallgvpath);
    }

#include <wx/textdlg.h>

void F12seFrame::OnbtnSFallGvarAddClick(wxCommandEvent&)
    {
    wxString idWxStr = wxGetTextFromUser(SFallNewGvarMessage, wxEmptyString);
    if (idWxStr.empty())
        return;
    unsigned long long id;
    if (!idWxStr.StartsWith("0x") || !idWxStr.ToULongLong(&id, 16))
        {
        std::string idstr = WxStringToStdLocal(idWxStr);
        id = 0;
        unsigned char* idptr = (unsigned char*)&id;
        unsigned len;
        if (idstr.size() >= 8)
            len = 8;
        else
            len = idstr.size();
        for (unsigned i = 0; i < len; ++i, ++idptr)
            *idptr = idstr[i];
        }
    sfallgv.globalVars.push_back(GlobalVar(id, 0));
    lbSFallGVar->Append(sfallgv.globalVars[sfallgv.globalVars.size() - 1].GetString());
    }

void F12seFrame::OnbtnSFallGvarDelClick(wxCommandEvent&)
    {
    const int index = lbSFallGVar->GetSelection();
    if (index == wxNOT_FOUND)
        return;
    const unsigned uindex = index;
    if (uindex < sfallgv.globalVars.size())
        {
        sfallgv.globalVars.erase(sfallgv.globalVars.begin() + uindex);
        lbSFallGVar->Delete(uindex);
        }
    else
        ntst_log_error("Logical error in SFallGvarDelClick.");
    }

void F12seFrame::OntxtSFallGvarValText(wxCommandEvent&)
    {
    int32 val;
    if (!wxTextToInt(val, txtSFallGvarVal))
        return;
    const int index = lbSFallGVar->GetSelection();
    if (index == wxNOT_FOUND)
        return;
    const unsigned uindex = index;
    if (uindex < sfallgv.globalVars.size())
        {
        sfallgv.globalVars[uindex].paddedVal.val = val;
        lbSFallGVar->SetString(uindex, sfallgv.globalVars[uindex].GetString());
        }
    else
        ntst_log_error("Logical error in SFallGvarValText.");
    }

void F12seFrame::OnlbSFallGVarSelect(wxCommandEvent&)
    {
    const int index = lbSFallGVar->GetSelection();
    if (index == wxNOT_FOUND)
        return;
    const unsigned uindex = index;
    if (uindex < sfallgv.globalVars.size())
        {
        txtSFallGvarVal->SetValue(ntst::to_string(sfallgv.globalVars[uindex].paddedVal.val));
        }
    else
        ntst_log_error("Logical error in SFallGVarSelect.");
    }

void SetFakeDescription(const wxListBox* lb, wxTextCtrl* txt, const SFallGV::FakePerks& fakes)
    {
    int index = lb->GetSelection();
    if (index == wxNOT_FOUND)
        {
        txt->Clear();
        return;
        }
    const unsigned uindex = index;
    if (uindex < fakes.size())
        {
        txt->SetValue(fakes[uindex].GetDesc());
        }
    else
        ntst_log_error("Logical error in SetFakeDescription.");
    }

void F12seFrame::OnlbSFallTraitsSelect(wxCommandEvent&)
    {
    SetFakeDescription(lbSFallTraits, txtSFallTraitDesc, sfallgv.fakeTraits);
    }

void F12seFrame::OnlbSFallPerksSelect(wxCommandEvent&)
    {
    SetFakeDescription(lbSFallPerks, txtSFallPerkDesc, sfallgv.fakePerks);
    }

void F12seFrame::OnlbSFallSelPerksSelect(wxCommandEvent&)
    {
    SetFakeDescription(lbSFallSelPerks, txtSFallSelPerkDesc, sfallgv.fakeSelectablePerks);
    }

void F12seFrame::OnbtnWorlmapSetClick(wxCommandEvent& /*event*/)
    {
    unsigned fillVal;
    switch (chWorldmapState->GetSelection())
        {
        case 0:
            fillVal = 2; // Visible
            break;
        case 1:
            fillVal = 1; // Partly
            break;
        default:
            fillVal = 0; // Hidden
            break;
        }
    if (datamodel.IsFallout1())
        {
        for (int i = 0; i < 869; i++) // 869 - bruteforced magic number
            WriteInt8Ninja(21, i, fillVal);
        }
    else
        {
        int numberOfTiles = ReadInt32(20, numberOfTilesOffset);
        for (int i = 0; i < numberOfTiles * 7 * 6; i++)
            WriteInt32Ninja(20, numberOfTilesOffset + (i + 2) * 4, fillVal);
        }
    }

void F12seFrame::OnbtnResetGvarsClick(wxCommandEvent& /*event*/)
    {
    unsigned gvarNum = datamodel.curSave.GetGvarNum();
    for (unsigned i = 0; i < gvarNum; ++i)
        {
        datamodel.curSave.SetGvarVal(i, datamodel.vault13gam.GetVarInitVal(i));
        }
    }

void F12seFrame::OnbtnResetEntrancesClick(wxCommandEvent& /*event*/)
    {
    const unsigned citiesCount = ReadInt32(20, WORLD_MAP_CITIES_NUM_OFFSET);
    if (citiesCount != datamodel.cityTxt.Count())
        {
        ntst_log_error("Number of cities in savegame is not equal to the number in the Data\\city.txt. Something is very wrong.");
        return;
        }
    if (citiesCount != lbCities->GetCount())
        {
        ntst_log_error("Number of cities in savegame is not equal to the number on main form.");
        return;
        }
    for (unsigned curCity = 0; curCity < citiesCount; ++curCity)
        {
        int offset = GetCityOffset(curCity);
        // Reset state
        if (datamodel.cityTxt.CityStartOn(curCity))
            WriteInt32Ninja(20, offset + 0x08, 1);
        else
            WriteInt32Ninja(20, offset + 0x08, 0);
        // Disable button
        WriteInt32Ninja(20, offset + 0x0C, 0);
        const unsigned entrancesCount = datamodel.cityTxt.EntrNum(curCity);
        const unsigned entrancesCountInSave = ReadInt32(20, offset + 0x10);
        if (entrancesCount != entrancesCountInSave)
            {
            ntst_log_error("Number of entrances in savegame(" + ntst::to_string(entrancesCountInSave)
                + ") is not equal to the number in the Data\\city.txt(" + ntst::to_string(entrancesCount)
                + ") city:" + WxStringToStdUTF8(lbCities->GetString(curCity)));
            return;
            }
        offset += 0x14;
        for (unsigned curEntr = 0; curEntr < entrancesCount; ++curEntr)
            {
            int valEntr;
            if (datamodel.cityTxt.EntrStartOn(curCity, curEntr))
                valEntr = 1;
            else
                valEntr = 0;
            WriteInt32Ninja(20, offset + sizeof(int32_t) * curEntr, valEntr);
            }
        }
    }

/**
 * @brief Returns selected array and element.
 * If only array selected, element == NULL
 * If nothing selected, array == NULL and element == NULL
 * @param outArray
 * @param outKeyVal
 */
wxTreeItemId F12seFrame::TryGetSelectedSfallArray(ArrayNew*& outArray, ArrayNew::KeyValPair*& outKeyVal)
{
    outArray = NULL;
    outKeyVal = NULL;
    wxTreeItemId itemId = tSfallArrays->GetSelection();
    if (!itemId.IsOk())
        return itemId;
    ArrayTreeData* dataPtr = (ArrayTreeData*)tSfallArrays->GetItemData(itemId);
    if (dataPtr)
    {
        const unsigned ai = dataPtr->GetArrayIdx();
        if (ai < sfallgv.arraysNew.size())
        {
            outArray = &sfallgv.arraysNew[ai];
            const unsigned ei = dataPtr->GetElemIdx();
            if (ei < outArray->data.size())
                outKeyVal = &outArray->data[ei];
        }
    }
    return itemId;
}

void F12seFrame::RefreshSelectedSfallArrayTreeLabel(wxTreeItemId itemId, ArrayNew* arrayPtr, ArrayNew::KeyValPair* keyValPtr)
{
    wxString label = GetLabelForSfallArrayElem(arrayPtr, keyValPtr);
    tSfallArrays->SetItemText(itemId, label);
}

void F12seFrame::OntSfallArraysSelectionChanged(wxTreeEvent&)
{
    ArrayNew* arrayPtr;
    ArrayNew::KeyValPair* keyValPtr;
    TryGetSelectedSfallArray(arrayPtr, keyValPtr);
    if (!arrayPtr)
        return;
    ArrayVarNew* val;
    if (keyValPtr)
        val = &keyValPtr->second; // element selected
    else
        val = &arrayPtr->key; // array selected
    const int sel = (val->type < rbSfallArrType->GetCount()) ? val->type : -1;
    rbSfallArrType->SetSelection(sel);
    txtSFallArrayNewVal->SetLabel(val->GetString());
}

void F12seFrame::OnrbSfallArrTypeSelect(wxCommandEvent&)
{
    ArrayNew* arrayPtr;
    ArrayNew::KeyValPair* keyValPtr;
    const wxTreeItemId itemId = TryGetSelectedSfallArray(arrayPtr, keyValPtr);
    if (!arrayPtr)
        return;
    ArrayVarNew* val;
    if (keyValPtr)
        val = &keyValPtr->second; // element selected
    else
        val = &arrayPtr->key; // array selected
    const unsigned selIdx = rbSfallArrType->GetSelection();
    if (selIdx < SFDT_END)
    {
        val->SetType((SFallDataType)selIdx);
        RefreshSelectedSfallArrayTreeLabel(itemId, arrayPtr, keyValPtr);
    }
}

void F12seFrame::OntxtSFallArrayNewValText(wxCommandEvent& event)
{
    ArrayNew* arrayPtr;
    ArrayNew::KeyValPair* keyValPtr;
    const wxTreeItemId itemId = TryGetSelectedSfallArray(arrayPtr, keyValPtr);
    if (!arrayPtr)
        return;
    ArrayVarNew* val;
    if (keyValPtr)
        val = &keyValPtr->second; // element selected
    else
        val = &arrayPtr->key; // array selected
    switch (val->type)
    {
    case SFDT_INT:
    {
        int32 valI;
        if (wxTextToInt(valI, txtSFallArrayNewVal))
            val->SetInt(valI);
        break;
    }
    case SFDT_FLOAT:
    {
        double valD;
        if (txtSFallArrayNewVal->GetValue().ToDouble(&valD))
            val->SetFloat(valD);
        break;
    }
    case SFDT_STR:
        val->SetString(WxStringToStdLocal(txtSFallArrayNewVal->GetValue()));
        break;
    default:
        return;
    }
    RefreshSelectedSfallArrayTreeLabel(itemId, arrayPtr, keyValPtr);
}
