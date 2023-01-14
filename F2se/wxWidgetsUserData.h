#ifndef WXWIDGETSUSERDATA_H
#define WXWIDGETSUSERDATA_H

#include <wx/textctrl.h>
#include <wx/listbox.h>

struct PairTxtLb
    {
    void Set(wxListBox *l, wxTextCtrl *t, unsigned f, size_t o) {lb = l; txt = t; function = f; offset = o;}
    wxTextCtrl* txt;
    wxListBox* lb;
    unsigned function;
    size_t offset;
    };

void SetWidgetLbTxtPairData(wxListBox *lb, wxTextCtrl *txt, PairTxtLb *p);

void SetWidgetData(wxWindow *widget, size_t offset);
size_t GetWidgetData(wxWindow *widget);
void SetWidgetData(wxWindow *widget, unsigned function, size_t offset);
void GetWidgetData(wxWindow *widget, unsigned *function, size_t *offset);

#endif // WXWIDGETSUSERDATA_H
