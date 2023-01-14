/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef SETTINGSEDIT_H
#define SETTINGSEDIT_H

//(*Headers(SettingsEdit)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/radiobox.h>
#include <wx/scrolbar.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include <Settings.h>
#include "DataModel.h"

class SettingsEdit: public wxDialog
{
	public:

		SettingsEdit(wxWindow* parent, const DataModel& data, const Settings* s, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~SettingsEdit();

        Settings GetSettings();
		//(*Declarations(SettingsEdit)
		wxButton* btnBrowse;
		wxButton* btnCancel;
		wxButton* btnOk;
		wxCheckBox* cbCorAmmoQ;
		wxCheckBox* cbCorAmmoT;
		wxCheckBox* cbCorItemFid;
		wxCheckBox* cbCorPerks;
		wxCheckBox* cbCorTraits;
		wxCheckBox* cbntst_log_error;
		wxCheckBox* cbntst_log_info;
		wxCheckBox* cbntst_log_warning;
		wxComboBox* cbPath;
		wxListBox* lbLngFiles;
		wxRadioBox* rbGame;
		wxRadioBox* rbImagesResize;
		wxScrollBar* sbBrightness;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* lblFalloutCfg;
		wxStaticText* lblFalloutExe;
		wxStaticText* lblFalloutLanguage;
		wxTextCtrl* txtFalloutCfg;
		wxTextCtrl* txtFalloutExe;
		wxTextCtrl* txtMsgDir;
		//*)

	protected:
		//(*Identifiers(SettingsEdit)
		static const long lblPath;
		static const long ID_COMBOBOX1;
		static const long bBrowse;
		static const long rboxGame;
		static const long ID_LISTBOX1;
		static const long cboxLogInfo;
		static const long cboxLogWarning;
		static const long cboxLogError;
		static const long lblOverrideFalloutExe;
		static const long ID_TEXTCTRL3;
		static const long lblOverrideFalloutCfg;
		static const long ID_TEXTCTRL4;
		static const long lblOverrideFalloutLanguage;
		static const long ID_TEXTCTRL2;
		static const long cboxCorItemFid;
		static const long cboxCorAmmoT;
		static const long cboxCorAmmoQ;
		static const long cboxCorPerks;
		static const long cboxCorTraits;
		static const long lblBrightness;
		static const long ID_SCROLLBAR1;
		static const long rboxImagesResize;
		static const long bCancel;
		static const long bOk;
		//*)

	private:
        Settings ss;
        std::vector<FalloutDir> falloutDirs;
        bool Fallout1Selected() const {return rbGame->GetSelection() == 0;}
        void SetSettings(const Settings& s);
		//(*Handlers(SettingsEdit)
		void OnbtnOkClick(wxCommandEvent& event);
		void OnbtnCancelClick(wxCommandEvent& event);
		void OnbtnBrowseClick(wxCommandEvent& event);
		void OnrbImageResizeSelect(wxCommandEvent& event);
		void OnInit(wxInitDialogEvent& event);
		void OnChar(wxKeyEvent& event);
		void OncbPathSelected(wxCommandEvent& event);
		//*)
        void OncbPathIndexChanged(int idx);
		DECLARE_EVENT_TABLE()
};

#endif
