/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include <wx/dcclient.h>
#include "wxStaticBitmapEx.h"

BEGIN_EVENT_TABLE(wxStaticBitmapEx, wxWindow)
    EVT_PAINT(wxStaticBitmapEx::OnPaint)
    EVT_SIZE(wxStaticBitmapEx::OnSize)
END_EVENT_TABLE()

void wxStaticBitmapEx::OnPaint(wxPaintEvent&)
{
    wxPaintDC dc(this);
    PrepareDC(dc);
    dc.SetBackground(GetBackgroundColour());
    dc.Clear();
    if (bitmap.IsOk())
        dc.DrawBitmap(bitmap, 0, 0);
}

void wxStaticBitmapEx::ScaleImage()
{
    int width, height;
    if (!img.IsOk())
        return;
    GetSize(&width, &height);
    if (width <= 0 || height <= 0)
        return;
    if (keepAspect)
        {
        if (width == img.GetWidth() || height == img.GetHeight())
            return;
        // TODO maybe check division for 0?
        const double widthCoef = (double)width/img.GetWidth();
        const double heightCoef = (double)height/img.GetHeight();
        if (widthCoef < heightCoef)
            bitmap = wxBitmap(img.Scale(width, img.GetHeight()*widthCoef, resizeQuality));
        else
            bitmap = wxBitmap(img.Scale(img.GetWidth()*heightCoef, height, resizeQuality));
        }
    else
        bitmap = wxBitmap(img.Scale(width, height, resizeQuality));
    Refresh();
}

void wxStaticBitmapEx::OnSize(wxSizeEvent&)
{
    ScaleImage();
}
