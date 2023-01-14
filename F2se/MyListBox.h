#ifndef MYLISTBOX_H
#define MYLISTBOX_H

#include <wx/listbox.h>
#include "wxWidgetsUserData.h"
#include "ntst_loggingpp.hpp"

class MyListBox : public wxListBox
{
public:
    MyListBox() : wxListBox() {MyInit();}
    MyListBox(wxWindow* parent, wxWindowID id,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        int n = 0, const wxString choices[] = NULL,
        long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxListBoxNameStr)
    : wxListBox(parent, id, pos, size, n, choices, style, validator, name) {MyInit();}
private:
    void MyInit()
        {
        Bind(wxEVT_CHAR_HOOK, &MyListBox::OnChar, this);
        }
    void OnChar(wxKeyEvent& event)
        {
        //ntst_log_info("UnicodeKey=" + ntst::to_string(event.GetUnicodeKey()) + " RawKeyCode=" + ntst::to_string(event.GetRawKeyCode()) + " KeyCode=" + ntst::to_string(event.GetKeyCode()));
        PairTxtLb* p = (PairTxtLb*)GetClientData();
        if (p == NULL)
            {
            ntst_log_warning("OnChar PairTxtLb == NULL.");
            }
        else
            {
            wxTextCtrl* txt = p->txt;
            if (txt == NULL)
                {
                ntst_log_warning("OnChar txt == NULL.");
                }
            else
                {
                int kc = event.GetKeyCode();
                if (kc == WXK_BACK || kc == WXK_DELETE)
                    {
                    txt->SetValue("0");
                    return;
                    }
                wxString val = txt->GetValue();
                if (kc >= 324 && kc <= 333)
                    {
                    kc = kc - 324 + '0'; // numpad numbers
                    }
                if ((kc >= '0' && kc <= '9') || kc == '-')
                    {
                    if (val == "0")
                        txt->SetValue((char)kc);
                    else
                        txt->SetValue(val << (char)kc);
                    return;
                    }
                if (val == "0")
                    {
                    if (kc == '-')
                        {
                        txt->SetValue('-');
                        return;
                        }
                    if (kc == 'X')
                        {
                        txt->SetValue(val << 'x');
                        return;
                        }
                    }
                else if (val.StartsWith("0x") && (kc >= 'A' && kc <= 'F'))
                    {
                    txt->SetValue(val << (char)kc);
                    }
                }
            }
        event.Skip();
        }
};

#endif
