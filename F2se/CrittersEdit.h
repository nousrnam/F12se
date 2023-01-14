/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef CRITTERSEDIT_H
#define CRITTERSEDIT_H

//(*Headers(CrittersEdit)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/frame.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include <string>
#include <vector>
#include "DataModel.h"
#include "MemBufferEX.h"
#include "MyListBox.h"

class CrittersEdit: public wxFrame
{
	public:

		CrittersEdit(wxWindow* parent, const DataModel& datamodel, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~CrittersEdit();

		//(*Declarations(CrittersEdit)
		MyListBox* lbBaseStats;
		MyListBox* lbBonusStats;
		MyListBox* lbSkills;
		wxButton* btnCancel;
		wxButton* btnSave;
		wxCheckBox* cbAges;
		wxCheckBox* cbBarter;
		wxCheckBox* cbDrop;
		wxCheckBox* cbFlatten;
		wxCheckBox* cbHeal;
		wxCheckBox* cbInvulnerable;
		wxCheckBox* cbKnock;
		wxCheckBox* cbLimbs;
		wxCheckBox* cbRange;
		wxCheckBox* cbSpecial;
		wxCheckBox* cbSteal;
		wxListBox* lbFiles;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxTextCtrl* txtBaseStatVal;
		wxTextCtrl* txtBonusStatVal;
		wxTextCtrl* txtFID;
		wxTextCtrl* txtSkillVal;
		//*)

	protected:

		//(*Identifiers(CrittersEdit)
		static const long lblCrittersList;
		static const long ID_LISTBOX1;
		static const long bSave;
		static const long lblStatsBase;
		static const long ID_LISTBOX2;
		static const long lblStatsBaseValue;
		static const long ID_TEXTCTRL1;
		static const long lblStatsBonus;
		static const long ID_LISTBOX3;
		static const long lblStatsBonusValue;
		static const long ID_TEXTCTRL2;
		static const long lblSkills;
		static const long ID_LISTBOX4;
		static const long lblSkillVal;
		static const long ID_TEXTCTRL3;
		static const long cboxBarter;
		static const long cboxSteal;
		static const long cboxDrop;
		static const long cboxLimbs;
		static const long cboxAges;
		static const long cboxHeal;
		static const long cboxInvulnerable;
		static const long cboxFlatten;
		static const long cboxSpecial;
		static const long cboxRange;
		static const long cboxKnock;
		static const long lblFrmId;
		static const long ID_TEXTCTRL4;
		static const long bCancel;
		//*)

	private:
        std::string dir;
        std::vector<MemBufferEX> buffers;

        const DataModel& data;
        PairTxtLb pairStatBase, pairStatBonus, pairSkill;
        void OnTxtForListChange(wxListBox *lb, wxTextCtrl* txt, unsigned blockOffset, unsigned blockSize, const std::vector<std::wstring>& names);
        void OnLbSelect(wxListBox *lb, wxTextCtrl* txt, unsigned blockOffset);
		//(*Handlers(CrittersEdit)
		void OnClose(wxCloseEvent& event);
		void OnbtnCloseClick(wxCommandEvent& event);
		void OnbtnSaveClick(wxCommandEvent& event);
		void OnlbFilesSelect(wxCommandEvent& event);
		void OncboxClick(wxCommandEvent& event);
		void OntxtBaseStatValText(wxCommandEvent& event);
		void OntxtBonusStatValText(wxCommandEvent& event);
		void OntxtSkillValText(wxCommandEvent& event);
		void OnlbBaseStatsSelect(wxCommandEvent& event);
		void OnlbBonusStatsSelect(wxCommandEvent& event);
		void OnlbSkillsSelect(wxCommandEvent& event);
		void OnTxtText(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
