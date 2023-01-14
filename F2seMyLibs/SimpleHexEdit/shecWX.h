/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef SIMPLEHEXEDITCTRLWX_H
#define SIMPLEHEXEDITCTRLWX_H

#include <wx/window.h>
#include <wx/dcmemory.h>

#include "shecBase.h"

class SimpleHexEditCtrlWX : public wxWindow, public SimpleHexEditCtrlBase<SimpleHexEditCtrlWX>
{
#if _MSC_VER
template<class Derived>
#endif
friend class SimpleHexEditCtrlBase;
public:
    SimpleHexEditCtrlWX() {}
    SimpleHexEditCtrlWX(wxWindow *parent,
                    wxWindowID winid,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = 0,
                    const wxString& name = "SimpleHexEditCtrl")
        {
        Create(parent, winid, pos, size, style, name);
        }
protected:
    void AdjustScrollbars();
    void OnScroll(wxScrollWinEvent& event);
    bool MSWProcessMessage(WXMSG* pMsg);

    void ClearBackground()
    {
    m_DoubleBufferDC.Clear();
    }
    bool HaveFocus() const {return HasFocus();}
    int GetW() const {return GetClientSize().GetWidth();}
    int GetH() const {return GetClientSize().GetHeight();}
    void NeedRedraw()
        {
        m_DoubleBufferDC.SetFont(GetFont());
        DrawCtrl();
        Refresh();
        }
    void PushClip() {;}// TODO
    void PopClip() {;}// TODO
    int GetFontHeight() const {return wxWindow::GetCharHeight();}
    int GetCharWidth(unsigned char c) const {return wxWindow::GetTextExtent(wxString((char)c)).GetWidth()/*wxWindow::GetCharWidth()*/;}
    void DrawText(const char* text, int x_, int y_) {m_DoubleBufferDC.DrawText(text, x_, y_);}
    void DrawLine(int x1, int y1, int x2, int y2) {m_DoubleBufferDC.DrawLine(x1, y1, x2, y2);}
    void DrawRect(int x_, int y_, int w_, int h_) {m_DoubleBufferDC.DrawRectangle(x_, y_, w_, h_);}
    //void FillRect(int x_, int y_, int w_, int h_) {;}// TODO
    //static int GetKeyUp() {return FL_Up;}
    static const int GetKeyUp;
    static const int GetKeyDown;
    static const int GetKeyLeft;
    static const int GetKeyRight;
    static const int GetKeyBackSpace;
    static const int GetKeyDelete;
    static const int GetKeyEnd;
    static const int GetKeyHome;
    static const int GetKeyInsert;

    void OnSize();
    void OnSize(wxSizeEvent &event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnPaint(wxPaintEvent &event);
    void OnMouseWheel(wxMouseEvent &event);
    void OnMouseLeftDown(wxMouseEvent &event);
    void OnMouseLeftUp(wxMouseEvent &event);
    void OnChar(wxKeyEvent &event);
private:
    wxMemoryDC m_DoubleBufferDC;
    wxBitmap m_DoubleBufferBitmap;
    DECLARE_EVENT_TABLE()
};

#endif // SIMPLEHEXEDITCTRLWX_H
