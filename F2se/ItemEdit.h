/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef ITEMEDIT_H
#define ITEMEDIT_H

//(*Headers(ItemEdit)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include <vector>
#include "MemBuffer.h"
#include "PRO.h"
#include "Localization.h"
#include "MyListBox.h"

struct itemNamePid
    {
    itemNamePid(const wxString& n, uint32 p) : name(n), pid(p) {}
    wxString name;
    uint32 pid;
    };

class ItemEdit: public wxDialog
{
	public:

		ItemEdit(wxWindow* parent, MemBuffer *b, const std::vector<itemNamePid>& v, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ItemEdit();
        void SelectPidIfPossible(uint32 pid);
		void Localize(const Localization& l, const char* sectionName);
		//(*Declarations(ItemEdit)
		MyListBox* lbOffsets;
		wxButton* btnCancel;
		wxButton* btnOk;
		wxListBox* lbItems;
		wxStaticText* StaticText1;
		wxTextCtrl* txtDescr;
		wxTextCtrl* txtValue;
		//*)
        void SelectFirstIfPossible()
            {
            if (lbItems->GetCount() > 0)
                {
                lbItems->SetSelection(0);
                lbItemsSelect();
                }
            }
	protected:

		//(*Identifiers(ItemEdit)
		static const long ID_LISTBOX1;
		static const long ID_TEXTCTRL2;
		static const long ID_LISTBOX2;
		static const long lblValue;
		static const long ID_TEXTCTRL1;
		static const long bCancel;
		static const long bOk;
		//*)

	private:
	    wxString OffsetToString(unsigned offset) const
	        {
	        wxString valStr;
	        const int val = buf->GetInv32(offset);
            valStr << val;
            return IntToHex(offset, 2) + '=' + valStr + " [" + IntToHex(val, 8) + ']';
	        }
        MemBuffer *buf;
        std::vector<itemNamePid> vec;
        void RefreshOffsetLine(unsigned num);
        wxString description[25];
        PairTxtLb pairTxtLb;
		//(*Handlers(ItemEdit)
		void OnbtnCancelClick(wxCommandEvent& event);
		void OnbtnOkClick(wxCommandEvent& event);
		void OnlbOffsetsSelect(wxCommandEvent& event);
		void OntxtValueText(wxCommandEvent& event);
		void OnlbItemsSelect(wxCommandEvent& event);
		void OnChar(wxKeyEvent& event);
		//*)
        void lbItemsSelect();
		DECLARE_EVENT_TABLE()
};

#endif
