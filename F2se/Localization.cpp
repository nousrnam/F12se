/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include "Localization.h"
#include "misc.h"
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/radiobox.h>
#include <wx/notebook.h>
#include "ntst_stringpp.hpp"
#include "ntst_loggingpp.hpp"
#include "Common.h"

template<typename T>
void SetLocalizedText(T *w, const IniSection *s)
    {
    const std::string *str = s->FindValue(WxStringToStdUTF8(w->GetName()));
    if (str != NULL)
        w->SetLabel(wxString::FromUTF8(str->c_str(), str->size()));
    }

void SetLocalizedText(wxStaticBox *w, const IniSection *s)
    {
    const std::string *str = s->FindValue(WxStringToStdUTF8(w->GetName()));
    if (str != NULL)
        w->SetLabel(wxString::FromUTF8(str->c_str(), str->size()));
    }

void Localize(wxWindow *w, const IniSection *s)
    {
    wxWindowList& children = w->GetChildren();
    for (wxWindowList::iterator it = children.begin(); it != children.end(); ++it)
        {
        wxWindow *curW = *it;
        //ntst_log_info(WxStringToStdUTF8(curW->GetName()));
        //ntst_log_info(WxStringToStdUTF8(curW->GetLabel()));
        if (curW->IsKindOf(wxCLASSINFO(wxStaticText)))
            {
            wxStaticText* lbl = (wxStaticText*)curW;
            SetLocalizedText(lbl, s);
            }
        else if (curW->IsKindOf(wxCLASSINFO(wxButton)))
            {
            wxButton* btn = (wxButton*)curW;
            SetLocalizedText(btn, s);
            }
        else if (curW->IsKindOf(wxCLASSINFO(wxCheckBox)))
            {
            wxCheckBox* cb = (wxCheckBox*)curW;
            SetLocalizedText(cb, s);
            }
        else if (curW->IsKindOf(wxCLASSINFO(wxStaticBox)))
            {
            wxStaticBox* stb = (wxStaticBox*)curW;
            SetLocalizedText(stb, s);
            }
        else if (curW->IsKindOf(wxCLASSINFO(wxRadioBox)))
            {
            wxRadioBox* rb = (wxRadioBox*)curW;
            std::string rbName = WxStringToStdUTF8(rb->GetName());
            for (unsigned i = 0; i < rb->GetCount(); ++i)
                {
                const std::string *str = s->FindValue(rbName + ntst::to_string(i));
                if (str != NULL)
                    rb->SetString(i, wxString::FromUTF8(str->c_str(), str->size()));
                }
            SetLocalizedText(rb, s);
            }
        else
            {
            Localize(curW, s);
            if (curW->IsKindOf(wxCLASSINFO(wxNotebook)))
                {
                wxNotebook* nb = (wxNotebook*)curW;
                std::string nbName = WxStringToStdUTF8(nb->GetName());
                for (unsigned i = 0; i < nb->GetPageCount(); ++i)
                    {
                    const std::string *str = s->FindValue(nbName + ntst::to_string(i));
                    if (str != NULL)
                        nb->SetPageText(i, wxString::FromUTF8(str->c_str(), str->size()));
                    }
                }
            }
        }
    }

void Localization::LocalizeWindow(wxWindow *w, const char* sectionName) const
    {
    const IniSection *s = file.FindSection(sectionName);
    if (s != NULL)
        Localize(w, s);
    }

template<typename T>
void SaveLocalizedText(T *w, IniSection *s)
    {
    s->SetValue(WxStringToStdUTF8(w->GetName()), WxStringToStdUTF8(w->GetLabel()));
    }

void SaveLocalizedText(wxStaticBox *w, IniSection *s)
    {
    s->SetValue(WxStringToStdUTF8(w->GetName()), WxStringToStdUTF8(w->GetLabel()));
    }

void Generate(wxWindow *w, IniSection *s)
    {
    wxWindowList& children = w->GetChildren();
    for (wxWindowList::iterator it = children.begin(); it != children.end(); ++it)
        {
        wxWindow *curW = *it;
        if (curW->IsKindOf(wxCLASSINFO(wxStaticText)))
            {
            wxStaticText* lbl = (wxStaticText*)curW;
            SaveLocalizedText(lbl, s);
            }
        else if (curW->IsKindOf(wxCLASSINFO(wxButton)))
            {
            wxButton* btn = (wxButton*)curW;
            SaveLocalizedText(btn, s);
            }
        else if (curW->IsKindOf(wxCLASSINFO(wxCheckBox)))
            {
            wxCheckBox* cb = (wxCheckBox*)curW;
            SaveLocalizedText(cb, s);
            }
        else if (curW->IsKindOf(wxCLASSINFO(wxStaticBox)))
            {
            wxStaticBox* stb = (wxStaticBox*)curW;
            SaveLocalizedText(stb, s);
            }
        else if (curW->IsKindOf(wxCLASSINFO(wxNotebook)))
            {
            wxNotebook* nb = (wxNotebook*)curW;
            std::string nbName = WxStringToStdUTF8(nb->GetName());
            for (unsigned i = 0; i < nb->GetPageCount(); ++i)
                s->SetValue(nbName + ntst::to_string(i), WxStringToStdUTF8(nb->GetPageText(i)));
            }
        else if (curW->IsKindOf(wxCLASSINFO(wxRadioBox)))
            {
            wxRadioBox* rb = (wxRadioBox*)curW;
            SaveLocalizedText(rb, s);
            std::string rbName = WxStringToStdUTF8(rb->GetName());
            for (unsigned i = 0; i < rb->GetCount(); ++i)
                s->SetValue(rbName + ntst::to_string(i), WxStringToStdUTF8(rb->GetString(i)));
            }
        Generate(curW, s);
        }
    }

void Localization::GenerateFileFromWindow(IniFile &f, wxWindow *w, const char* sectionName)
    {
    IniSection *s = f.FindSectionOrCreate(sectionName);
    if (s == NULL)
        return;
    Generate(w, s);
    }

static void CheckIniFileUTF8LogErrors(const IniFile& ini)
    {
    for (IniFile::const_iterator section = ini.begin(); section != ini.end(); ++section)
        {
        const IniSection cursec = section->second;
        for (IniSection::const_iterator keyvalue = cursec.begin(); keyvalue != cursec.end(); ++keyvalue)
            {
            const std::string& value = keyvalue->second;
            if (!StringIsValidUtf8(value))
                {
                ntst_log_warning("Not UTF8 value(" + value + ") for key("
                            + (std::string)keyvalue->first + ") in section("
                            + (std::string)section->first + ")");
                }
            }
        }
    }

bool Localization::LoadFile(const std::string& path)
    {
    bool result = file.Load(path);
    CheckIniFileUTF8LogErrors(file);
    return result;
    }
