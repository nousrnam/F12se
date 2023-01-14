/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef F2SEMAIN_H
#define F2SEMAIN_H

//(*Headers(F12seFrame)
#include "shecWX.h"
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/frame.h>
#include <wx/listbox.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/statbmp.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/treectrl.h>
//*)

#include <string>
#include <stdio.h>
#include "DataModel.h"
#include "wxStaticBitmapEx.h"
#include "Settings.h"
#include "SFallGV.h"

#include "Localization.h"
#include "wxWidgetsUserData.h"
#include "MyListBox.h"

class F12seFrame: public wxFrame, public DataModelViewer
{
    public:

        F12seFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~F12seFrame();
        wxString GetF12seTitleStart() const;
        void AddLogStr(const std::string& str);
        void GetSettingsAndApply(const Settings *s);
        template<typename SaveFile>
        void LoadSaveDat(SaveFile file)
            {
            datamodel.LoadSaveDat(file);
            AfterLoadSaveDat();
            }
        void SaveSaveDat(const std::string& path);
        void AfterLoadSaveDat();
        void RefreshHexView();
        void RefreshSelectedHex();
        DataModel datamodel;

        void RefreshHeader(MemBufferEX& head, size_t offset, size_t count);
        void ClearHeader();
        void RefreshF2(MemBufferEX& buf, size_t offset, size_t count);
        void ClearF2();
        void RefreshF3(MemBufferEX& buf, size_t offset, size_t count);
        void RefreshF5(MemBufferEX& buf, size_t offset, size_t count);
        void RefreshF6(MemBufferEX& buf, size_t offset, size_t count);
        void ClearF6();
        void RefreshF7(MemBufferEX& buf, size_t offset, size_t count);
        void ClearF7();
        void RefreshF8(MemBufferEX& buf, size_t offset, size_t count);
        void RefreshF10(MemBufferEX& buf, size_t offset, size_t count);
        void RefreshF13(MemBufferEX& buf, size_t offset, size_t count);
        void RefreshF15(MemBufferEX& buf, size_t offset, size_t count);
        void RefreshF18(MemBufferEX& buf, size_t offset, size_t count);
        void RefreshF19(MemBufferEX& buf, size_t offset, size_t count);
        void RefreshSelectedGvar();
    private:
        FILE* flog;
        Localization localization;
        wxString cityX;
        wxString cityY;
        wxString cityState;
        wxString cityButton;
        wxString cityEntrance;
        wxString fileCrcChanged;
        wxString SFallNewGvarMessage;
        void RefreshSelectedCity();
        int GetCityOffset(int cityNum) const {return (int)lbCities->GetClientData(cityNum);}
        SFallGV sfallgv;
        size_t numberOfTilesOffset;

        PairTxtLb pairCity, pairStatBase, pairStatBonus, pairSkill, pairKill, pairPerk, pairSfallGlobals;
        bool wxChoiceToInt(int32 &varName, wxChoice *widget);
        virtual void RefreshSaveSlots(const std::vector<DataModel::Slot>& slots);
        virtual void UpdateViewAfterInitAll(DataModel& model);
        void WriteString(size_t funct, size_t offset, size_t length, const char* str) {datamodel.curSave.fileSections[funct].WriteZString(offset, length, str);}
        std::wstring ReadString(size_t funct, size_t offset, size_t length)
            {
            return datamodel.GameStringToWide(datamodel.curSave.fileSections[funct].ReadString(offset, length));
            }
        int8 ReadInt8(size_t funct, size_t offset) {return datamodel.curSave.fileSections[funct].Get8(offset);}
        void WriteInt8(size_t funct, size_t offset, int8 val) {datamodel.curSave.fileSections[funct].Set8(offset, val);}
        void WriteInt8Ninja(size_t funct, size_t offset, int8 val)
            {
            datamodel.curSave.fileSections[funct].BeginNinjaMode();
            datamodel.curSave.fileSections[funct].Set8(offset, val);
            datamodel.curSave.fileSections[funct].EndNinjaMode();
            }
        int32 ReadInt32(size_t funct, size_t offset) {return datamodel.curSave.fileSections[funct].GetInv32(offset);}
        void WriteInt32(size_t funct, size_t offset, int32 val) {datamodel.curSave.fileSections[funct].SetInv32(offset, val);}
        void WriteInt32Ninja(size_t funct, size_t offset, int32 val)
            {
            datamodel.curSave.fileSections[funct].BeginNinjaMode();
            datamodel.curSave.fileSections[funct].SetInv32(offset, val);
            datamodel.curSave.fileSections[funct].EndNinjaMode();
            }
        void HeaderUpdateDateTime(int32 ticks);
        template <class SomeWidget>
        void SetWidgetFromBufIfReq(SomeWidget *widget, const MemBufferEX& buf, size_t offset, size_t changeOffset, size_t changeCount)
            {
            if (BlocksOverlap(offset, sizeof(int32), changeOffset, changeCount) || buf.GetSize() < offset + sizeof(int32))
                SetWidgetFromBuf(widget, buf, offset);
            }
        void SetWidgetFromBuf(wxTextCtrl *txt, const MemBufferEX& buf, size_t offset);
        void SetWidgetFromBuf(wxChoice *ch, const MemBufferEX& buf, size_t offset);
        bool GetOffsetForSelectedCity(int *offset);
        void EditSelectedSav();
        void LoadItemImage(uint32 fid);
        int GetSelectedItemOffset();
        void FixInventoryItem(MemBuffer& buf);
        void Localize(const Localization& l);
        void LoadWindowRect();
        void EditSelectedInventoryItem();
        //(*Handlers(F12seFrame)
        void OnQuit(wxCommandEvent& event);
        void OnlbSavesSelect(wxCommandEvent& event);
        void OnbtnRefreshClick(wxCommandEvent& event);
        void OntxtSaveNameText(wxCommandEvent& event);
        void OntxtPlayerNameText(wxCommandEvent& event);
        void OnDataChanged(wxCommandEvent& event);
        void OnlbGvarSelect(wxCommandEvent& event);
        void OnbtnFindGvarClick(wxCommandEvent& event);
        void OntxtGvarValText(wxCommandEvent& event);
        void OnlbHolodisksSelect(wxCommandEvent& event);
        void OnlbKarmasSelect(wxCommandEvent& event);
        void OnchQLocSelect(wxCommandEvent& event);
        void OnchQDescSelect(wxCommandEvent& event);
        void OnbtnEditSAVClick(wxCommandEvent& event);
        void OnbtnDelSavClick(wxCommandEvent& event);
        void OnlbFunctionsSelect(wxCommandEvent& event);
        void OnTxtText(wxCommandEvent& event);
        void OnTxtLbText(wxCommandEvent& event);
        void OnLbTxtSelect(wxCommandEvent& event);
        void OnbtnSaveClick(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OntxtPerkValText(wxCommandEvent& event);
        void OnchSelect(wxCommandEvent& event);
        void OncbPickPerkClick(wxCommandEvent& event);
        void OnlbCitiesSelect(wxCommandEvent& event);
        void OnlbCitySelect(wxCommandEvent& event);
        void OntxtCityValText(wxCommandEvent& event);
        void OntItemsSelectionChanged(wxTreeEvent& event);
        void OntxtItemQText(wxCommandEvent& event);
        void OnlbSavFilesDClick(wxCommandEvent& event);
        void OnbtnEditItemClick(wxCommandEvent& event);
        void OnbtnCreateItemClick(wxCommandEvent& event);
        void OnbtnDeleteItemClick(wxCommandEvent& event);
        void OnbtnClearCombatClick(wxCommandEvent& event);
        void OncbGhostModeClick(wxCommandEvent& event);
        void OntxtLocalCoord(wxCommandEvent& event);
        void OntxtLocalMapLevelText(wxCommandEvent& event);
        void OnchMapSelect(wxCommandEvent& event);
        void OncbCrippledEyeClick(wxCommandEvent& event);
        void OncbCrippledRArmClick(wxCommandEvent& event);
        void OncbCrippledLArmClick(wxCommandEvent& event);
        void OncbCrippledRLegClick(wxCommandEvent& event);
        void OncbCrippledLLegClick(wxCommandEvent& event);
        void OnbtnEditCrittersClick(wxCommandEvent& event);
        void OntxtLevelText(wxCommandEvent& event);
        void OnbtnSaveSFallgvClick(wxCommandEvent& event);
        void OnbtnSFallGvarAddClick(wxCommandEvent& event);
        void OnbtnSFallGvarDelClick(wxCommandEvent& event);
        void OntxtSFallGvarValText(wxCommandEvent& event);
        void OnlbSFallGVarSelect(wxCommandEvent& event);
        void OnlbSFallTraitsSelect(wxCommandEvent& event);
        void OnlbSFallPerksSelect(wxCommandEvent& event);
        void OnlbSFallSelPerksSelect(wxCommandEvent& event);
        void OnbtnWorlmapSetClick(wxCommandEvent& event);
        void OnbtnResetGvarsClick(wxCommandEvent& event);
        void OnbtnResetEntrancesClick(wxCommandEvent& event);
        void OntItemsItemActivated(wxTreeEvent& event);
        //*)

        //(*Identifiers(F12seFrame)
        static const long ID_LISTBOX1;
        static const long bRefresh;
        static const long bSave;
        static const long lblSavegame;
        static const long ID_TEXTCTRL2;
        static const long lblPlayer;
        static const long ID_TEXTCTRL3;
        static const long ID_STATICLINE1;
        static const long lblDate;
        static const long ID_TEXTCTRL5;
        static const long ID_CHOICE1;
        static const long ID_CHOICE2;
        static const long ID_TEXTCTRL6;
        static const long lblHM;
        static const long ID_TEXTCTRL7;
        static const long lblMS;
        static const long ID_TEXTCTRL8;
        static const long lblST;
        static const long ID_TEXTCTRL9;
        static const long lblTicks;
        static const long ID_TEXTCTRL4;
        static const long ID_CUSTOM2;
        static const long ID_PANEL3;
        static const long ID_LISTBOX4;
        static const long ID_TEXTCTRL12;
        static const long lblValueGvar;
        static const long ID_TEXTCTRL14;
        static const long ID_STATICLINE2;
        static const long ID_TEXTCTRL13;
        static const long bFindGvar;
        static const long ID_LISTBOX2;
        static const long ID_LISTBOX3;
        static const long lblQLocation;
        static const long ID_CHOICE3;
        static const long lblQDescription;
        static const long ID_CHOICE4;
        static const long lblDisplayThr;
        static const long ID_TEXTCTRL10;
        static const long lblCompletedThr;
        static const long ID_TEXTCTRL11;
        static const long ID_PANEL4;
        static const long lblSavList;
        static const long ID_LISTBOX7;
        static const long bEditSAV;
        static const long bDelSav;
        static const long lblCities;
        static const long ID_LISTBOX8;
        static const long lblSelectedCity;
        static const long ID_LISTBOX9;
        static const long lblValueCity;
        static const long ID_TEXTCTRL17;
        static const long lblWorldX;
        static const long ID_TEXTCTRL18;
        static const long lblWorldY;
        static const long ID_TEXTCTRL19;
        static const long lblCarMap;
        static const long ID_CHOICE5;
        static const long lblCarFuel;
        static const long ID_TEXTCTRL20;
        static const long ID_CHOICE12;
        static const long bWorlmapSet;
        static const long ID_PANEL7;
        static const long ID_TREECTRL1;
        static const long bItemEdit;
        static const long bItemCreate;
        static const long bItemDelete;
        static const long lblQuantity;
        static const long ID_TEXTCTRL21;
        static const long ID_STATICBITMAP2;
        static const long ID_PANEL6;
        static const long lblStatsBase;
        static const long ID_LISTBOX5;
        static const long lblValueStatBase;
        static const long ID_TEXTCTRL15;
        static const long lblStatsBonus;
        static const long ID_LISTBOX6;
        static const long lblValueStatBonus;
        static const long ID_TEXTCTRL16;
        static const long ID_PANEL5;
        static const long ID_CHOICE6;
        static const long ID_CHOICE7;
        static const long ID_CHOICE8;
        static const long ID_CHOICE9;
        static const long ID_LISTBOX12;
        static const long lblValueSkill;
        static const long ID_TEXTCTRL23;
        static const long ID_LISTBOX11;
        static const long lblValuePerk;
        static const long ID_TEXTCTRL22;
        static const long cboxPickPerk;
        static const long ID_PANEL10;
        static const long ID_LISTBOX13;
        static const long lblValueKill;
        static const long ID_TEXTCTRL24;
        static const long lblExperience;
        static const long ID_TEXTCTRL25;
        static const long lblLevel;
        static const long ID_TEXTCTRL26;
        static const long lblSkillPoints;
        static const long ID_TEXTCTRL27;
        static const long lblHitpoints;
        static const long ID_TEXTCTRL28;
        static const long lblRadiation;
        static const long ID_TEXTCTRL29;
        static const long lblPoison;
        static const long ID_TEXTCTRL30;
        static const long ID_CHOICE10;
        static const long ID_CHOICE11;
        static const long lblLocalX;
        static const long ID_TEXTCTRL32;
        static const long lblLocalY;
        static const long ID_TEXTCTRL33;
        static const long lblLocalMapLevel;
        static const long ID_TEXTCTRL31;
        static const long cboxCrippledEye;
        static const long cboxCrippledRArm;
        static const long cboxCrippledLArm;
        static const long cboxCrippledRLeg;
        static const long cboxCrippledLLeg;
        static const long cboxGhostMode;
        static const long bClearCombat;
        static const long bEditCritters;
        static const long bResetGvars;
        static const long bResetEntrances;
        static const long ID_PANEL8;
        static const long bSaveSFallgv;
        static const long ID_LISTBOX14;
        static const long bSFallGvarAdd;
        static const long bSFallGvarDel;
        static const long lblValueSFallGvar;
        static const long ID_TEXTCTRL34;
        static const long ID_LISTBOX15;
        static const long ID_TEXTCTRL35;
        static const long ID_LISTBOX16;
        static const long ID_TEXTCTRL36;
        static const long ID_LISTBOX17;
        static const long ID_TEXTCTRL37;
        static const long ID_PANEL11;
        static const long ID_LISTBOX10;
        static const long ID_CUSTOM1;
        static const long ID_PANEL9;
        static const long nbPages;
        static const long ID_PANEL1;
        static const long ID_TEXTCTRL1;
        static const long ID_PANEL2;
        static const long ID_SPLITTERWINDOW1;
        static const long ID_Open;
        static const long ID_Save;
        static const long ID_SaveAs;
        static const long ID_Settings;
        static const long ID_Quit;
        static const long ID_Armor;
        static const long ID_Container;
        static const long ID_Drug;
        static const long ID_Weapon;
        static const long ID_Ammo;
        static const long ID_Miscellaneous;
        static const long ID_Key;
        //*)

        //(*Declarations(F12seFrame)
        MyListBox* lbBaseStats;
        MyListBox* lbBonusStats;
        MyListBox* lbCity;
        MyListBox* lbKills;
        MyListBox* lbPerks;
        MyListBox* lbSFallGVar;
        MyListBox* lbSkills;
        SimpleHexEditCtrlWX* hexEditor;
        wxButton* btnClearCombat;
        wxButton* btnCreateItem;
        wxButton* btnDelSav;
        wxButton* btnDeleteItem;
        wxButton* btnEditCritters;
        wxButton* btnEditItem;
        wxButton* btnEditSAV;
        wxButton* btnFindGvar;
        wxButton* btnRefresh;
        wxButton* btnResetEntrances;
        wxButton* btnResetGvars;
        wxButton* btnSFallGvarAdd;
        wxButton* btnSFallGvarDel;
        wxButton* btnSave;
        wxButton* btnSaveSFallgv;
        wxButton* btnWorlmapSet;
        wxCheckBox* cbCrippledEye;
        wxCheckBox* cbCrippledLArm;
        wxCheckBox* cbCrippledLLeg;
        wxCheckBox* cbCrippledRArm;
        wxCheckBox* cbCrippledRLeg;
        wxCheckBox* cbGhostMode;
        wxCheckBox* cbPickPerk;
        wxChoice* chCarMap;
        wxChoice* chMonths;
        wxChoice* chQDesc;
        wxChoice* chQLoc;
        wxChoice* chTag1;
        wxChoice* chTag2;
        wxChoice* chTag3;
        wxChoice* chTag4;
        wxChoice* chTrait1;
        wxChoice* chTrait2;
        wxChoice* chWorldmapState;
        wxChoice* chYears;
        wxListBox* lbCities;
        wxListBox* lbFunctions;
        wxListBox* lbGvar;
        wxListBox* lbHolodisks;
        wxListBox* lbKarmas;
        wxListBox* lbSFallPerks;
        wxListBox* lbSFallSelPerks;
        wxListBox* lbSFallTraits;
        wxListBox* lbSavFiles;
        wxListBox* lbSaves;
        wxMenu menuCreateItem;
        wxMenu* mFile;
        wxMenuBar* mbMain;
        wxMenuItem* mAmmo;
        wxMenuItem* mArmor;
        wxMenuItem* mContainer;
        wxMenuItem* mDrug;
        wxMenuItem* mKey;
        wxMenuItem* mMiscellaneous;
        wxMenuItem* mOpen;
        wxMenuItem* mQuit;
        wxMenuItem* mSave;
        wxMenuItem* mSaveAs;
        wxMenuItem* mSettings;
        wxMenuItem* mWeapon;
        wxNotebook* Notebook1;
        wxPanel* pnlGVARs;
        wxPanel* pnlHeader;
        wxPanel* pnlHex;
        wxPanel* pnlInventory;
        wxPanel* pnlLog;
        wxPanel* pnlMain;
        wxPanel* pnlMisc;
        wxPanel* pnlSFall;
        wxPanel* pnlSkillsPerks;
        wxPanel* pnlStats;
        wxPanel* pnlWorldmap;
        wxSplitterWindow* SplitterWindow1;
        wxStaticBitmap* sbItem;
        wxStaticBitmapEx* bmScreen;
        wxStaticLine* StaticLine1;
        wxStaticLine* StaticLine2;
        wxStaticText* StaticText10;
        wxStaticText* StaticText11;
        wxStaticText* StaticText12;
        wxStaticText* StaticText13;
        wxStaticText* StaticText14;
        wxStaticText* StaticText15;
        wxStaticText* StaticText16;
        wxStaticText* StaticText17;
        wxStaticText* StaticText18;
        wxStaticText* StaticText19;
        wxStaticText* StaticText1;
        wxStaticText* StaticText20;
        wxStaticText* StaticText21;
        wxStaticText* StaticText22;
        wxStaticText* StaticText23;
        wxStaticText* StaticText24;
        wxStaticText* StaticText25;
        wxStaticText* StaticText26;
        wxStaticText* StaticText27;
        wxStaticText* StaticText28;
        wxStaticText* StaticText29;
        wxStaticText* StaticText2;
        wxStaticText* StaticText30;
        wxStaticText* StaticText31;
        wxStaticText* StaticText32;
        wxStaticText* StaticText34;
        wxStaticText* StaticText35;
        wxStaticText* StaticText36;
        wxStaticText* StaticText37;
        wxStaticText* StaticText38;
        wxStaticText* StaticText3;
        wxStaticText* StaticText4;
        wxStaticText* StaticText5;
        wxStaticText* StaticText6;
        wxStaticText* StaticText7;
        wxStaticText* StaticText8;
        wxStaticText* StaticText9;
        wxStaticText* lblSFallGvar;
        wxTextCtrl* txtBaseStatVal;
        wxTextCtrl* txtBonusStatVal;
        wxTextCtrl* txtCityVal;
        wxTextCtrl* txtDay;
        wxTextCtrl* txtExperience;
        wxTextCtrl* txtFindGvar;
        wxTextCtrl* txtFuel;
        wxTextCtrl* txtGvarDescription;
        wxTextCtrl* txtGvarVal;
        wxTextCtrl* txtHitpoints;
        wxTextCtrl* txtHour;
        wxTextCtrl* txtItemQ;
        wxTextCtrl* txtKillVal;
        wxTextCtrl* txtLevel;
        wxTextCtrl* txtLocalMapLevel;
        wxTextCtrl* txtLocalX;
        wxTextCtrl* txtLocalY;
        wxTextCtrl* txtLog;
        wxTextCtrl* txtMinute;
        wxTextCtrl* txtPerkVal;
        wxTextCtrl* txtPlayerName;
        wxTextCtrl* txtPoison;
        wxTextCtrl* txtQComplete;
        wxTextCtrl* txtQDisplay;
        wxTextCtrl* txtRadiation;
        wxTextCtrl* txtSFallGvarVal;
        wxTextCtrl* txtSFallPerkDesc;
        wxTextCtrl* txtSFallSelPerkDesc;
        wxTextCtrl* txtSFallTraitDesc;
        wxTextCtrl* txtSaveName;
        wxTextCtrl* txtSecond;
        wxTextCtrl* txtSkillPoints;
        wxTextCtrl* txtSkillVal;
        wxTextCtrl* txtTick;
        wxTextCtrl* txtTicks;
        wxTextCtrl* txtXPos;
        wxTextCtrl* txtYPos;
        wxTreeCtrl* tItems;
        //*)
        wxChoice* chTags[4];
        void OnOpen(wxCommandEvent& event);
        void OnSave(wxCommandEvent& event);
        void OnSaveAs(wxCommandEvent& event);
        void OnSettings(wxCommandEvent& event);
        void OnCreateArmor(wxCommandEvent&) {CreateItem(ITMT_ARMOR);}
        void OnCreateContainer(wxCommandEvent&) {CreateItem(ITMT_CONTAINER);}
        void OnCreateDrug(wxCommandEvent&) {CreateItem(ITMT_DRUG);}
        void OnCreateWeapon(wxCommandEvent&) {CreateItem(ITMT_WEAPON);}
        void OnCreateAmmo(wxCommandEvent&) {CreateItem(ITMT_AMMO);}
        void OnCreateMiscellaneous(wxCommandEvent&) {CreateItem(ITMT_MISCITEM);}
        void OnCreateKey(wxCommandEvent&) {CreateItem(ITMT_KEY);}
        void CreateItem(unsigned itemType);
        void ModBodyFlag(bool needset, unsigned mask);
        void GenerateLocalizationFile();
        DECLARE_EVENT_TABLE()
};

#endif // F2SEMAIN_H
