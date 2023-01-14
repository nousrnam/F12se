/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef SAVEDIT_H
#define SAVEDIT_H

//(*Headers(SavEdit)
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include <string>
#include "SAV.h"
#include "GAM.h"
#include "fallout.h"
#include "misc.h"
#include "ntst_stringpp.hpp"
#include "MyListBox.h"

template<typename T>
std::string GetScriptType(T stype)
    {
    switch (stype)
        {
        case 0:
            return "0(unknown)";
        case 1:
            return "spatial";
        case 2:
            return "item???";
        case 3:
            return "scenery";
        case 4:
            return "critter";
        default:
            return "invalid(" + ntst::to_string(stype) + ')';
        }
    }

class SavEdit: public wxFrame
{
	public:
        SavEdit(wxWindow* parent, const SAVfile& savFile, const GamFile& mapgam, const ScriptList& scriptsList, wxWindowID id=wxID_ANY);
		virtual ~SavEdit();
		void RefreshMvars();
        std::string GetMVarText(unsigned index);
        void RefreshScripts();
		//(*Declarations(SavEdit)
		MyListBox* lbLvar;
		MyListBox* lbMvar;
		wxButton* btnCancel;
		wxButton* btnSave;
		wxListBox* lbScripts;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxTextCtrl* txtLVarVal;
		wxTextCtrl* txtMVarVal;
		wxTextCtrl* txtMvarDesc;
		wxTextCtrl* txtScriptComment;
		//*)

	protected:
		//(*Identifiers(SavEdit)
		static const long ID_LISTBOX1;
		static const long ID_TEXTCTRL1;
		static const long lblValueMvar;
		static const long ID_TEXTCTRL2;
		static const long lblScripts;
		static const long ID_LISTBOX2;
		static const long ID_TEXTCTRL3;
		static const long lblLVars;
		static const long ID_LISTBOX3;
		static const long lblValueLvar;
		static const long ID_TEXTCTRL4;
		static const long bCancel;
		static const long bSave;
		//*)

	private:
        SAVfile sav;
        GamFile gam;
        ScriptList scriptList;
        std::vector<SavScriptRec> scripts;
        PairTxtLb pairMapVar, pairScriptVar;
		//(*Handlers(SavEdit)
		void OnClose(wxCloseEvent& event);
		void OnbtnCancelClick(wxCommandEvent& event);
		void OnbtnSaveClick(wxCommandEvent& event);
		void OnlbMvarSelect(wxCommandEvent& event);
		void OnlbScriptsSelect(wxCommandEvent& event);
		void OnlbLvarSelect(wxCommandEvent& event);
		void OntxtMVarValText(wxCommandEvent& event);
		void OntxtLVarValText(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
