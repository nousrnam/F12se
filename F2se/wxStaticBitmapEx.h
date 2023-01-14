/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef WXSTATICBITMAPEX_H
#define WXSTATICBITMAPEX_H

#include <wx/statbmp.h>
#include <wx/dcmemory.h>

class wxStaticBitmapEx : public wxWindow
{
    public:
        /*wxStaticBitmapEx(wxWindow *parent,
                   wxWindowID id,
                   const wxGDIImage& label,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = 0,
                   const wxString& name = wxStaticBitmapNameStr) : wxStaticBitmap(parent,
                   id,
                   label,
                   pos,
                   size,
                   style,
                   name)*/
        wxStaticBitmapEx(wxWindow *parent,
                    wxWindowID winid,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = 0,
                    const wxString& name = "StaticBitmapEx") : keepAspect(true), resizeQuality(wxIMAGE_QUALITY_BILINEAR)
        {
        Create(parent, winid, pos, size, style, name);
        }
        void SetImageResizeQuality(const wxImageResizeQuality& irq) {resizeQuality = irq;}
        void SetImage(const wxImage &image) {img = image.Copy(); ScaleImage();}
        void Clear() {bitmap = wxNullBitmap; Refresh();}
    protected:
        wxImage img;
        wxBitmap bitmap;
        bool keepAspect;
        wxImageResizeQuality resizeQuality;
    private:
    void ScaleImage();
    void OnSize(wxSizeEvent &event);
    void OnPaint(wxPaintEvent &event);
    DECLARE_EVENT_TABLE()
};

#endif // WXSTATICBITMAPEX_H
