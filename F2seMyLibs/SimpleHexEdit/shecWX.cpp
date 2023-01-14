/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifdef _MSC_VER
// C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable: 4800)
#endif

#include <wx/dcclient.h>

#include "shecWX.h"

BEGIN_EVENT_TABLE(SimpleHexEditCtrlWX, wxWindow)
    EVT_ERASE_BACKGROUND(SimpleHexEditCtrlWX::OnEraseBackground)
    EVT_PAINT(SimpleHexEditCtrlWX::OnPaint)
    EVT_SIZE(SimpleHexEditCtrlWX::OnSize)
    EVT_MOUSEWHEEL(SimpleHexEditCtrlWX::OnMouseWheel)
    EVT_LEFT_DOWN(SimpleHexEditCtrlWX::OnMouseLeftDown)
    EVT_LEFT_UP(SimpleHexEditCtrlWX::OnMouseLeftUp)
    EVT_CHAR(SimpleHexEditCtrlWX::OnChar)
    EVT_SCROLLWIN(SimpleHexEditCtrlWX::OnScroll)
END_EVENT_TABLE()

void SimpleHexEditCtrlWX::AdjustScrollbars()
    {
    SetScrollbar(wxVERTICAL, 0, GetVisibleRowNum(), GetTotalRowNum());
    SetScrollbar(wxHORIZONTAL, 0, GetVisibleColNum(), GetTotalColNum());
    }

void SimpleHexEditCtrlWX::OnScroll(wxScrollWinEvent& event)
    {
    int newPos;
    if (event.GetOrientation() == wxVERTICAL)
        {
        if (event.GetEventType() == wxEVT_SCROLLWIN_LINEUP || event.GetEventType() == wxEVT_SCROLLWIN_PAGEUP)
            newPos = GetLineOffset() - 1;
        else if (event.GetEventType() == wxEVT_SCROLLWIN_LINEDOWN || event.GetEventType() == wxEVT_SCROLLWIN_PAGEDOWN)
            newPos = GetLineOffset() + 1;
        else
            newPos = event.GetPosition();
        SetLineOffset(newPos);
        }
    else
        {
        if (event.GetEventType() == wxEVT_SCROLLWIN_LINEUP || event.GetEventType() == wxEVT_SCROLLWIN_PAGEUP)
            newPos = GetColOffset() - 1;
        else if (event.GetEventType() == wxEVT_SCROLLWIN_LINEDOWN || event.GetEventType() == wxEVT_SCROLLWIN_PAGEDOWN)
            newPos = GetColOffset() + 1;
        else
            newPos = event.GetPosition();
        SetColOffset(newPos);
        }
    SetScrollPos(event.GetOrientation(), newPos);
    }

void SimpleHexEditCtrlWX::OnSize()
{
    m_DoubleBufferDC.SelectObject(wxNullBitmap);
    const int w = GetClientSize().GetWidth();
    const int h = GetClientSize().GetHeight();
    if ((w > 0) && (h > 0))
    {
        m_DoubleBufferBitmap = wxBitmap(w, h);
        m_DoubleBufferDC.SelectObject(m_DoubleBufferBitmap);
        AdjustScrollbars();
        NeedRedraw();
    }
}

void SimpleHexEditCtrlWX::OnSize(wxSizeEvent&)
    {
    OnSize();
    }

void SimpleHexEditCtrlWX::OnEraseBackground(wxEraseEvent&)
    {
    }

void SimpleHexEditCtrlWX::OnPaint(wxPaintEvent&)
    {
    if(m_DoubleBufferDC.IsOk())
        {
        wxPaintDC dc(this);
        dc.Blit(0, 0, m_DoubleBufferDC.GetSize().GetWidth(),
        m_DoubleBufferDC.GetSize().GetHeight(), &m_DoubleBufferDC, 0, 0);
        }
    }

void SimpleHexEditCtrlWX::OnMouseWheel(wxMouseEvent &event)
    {
    event.Skip();
    int d = -event.GetWheelRotation() * event.GetLinesPerAction() / 120;
    SimpleHexEditCtrlBase<SimpleHexEditCtrlWX>::OnMouseWheel(0, d);
    }

void SimpleHexEditCtrlWX::OnMouseLeftDown(wxMouseEvent &event)
    {
    event.Skip();
    SetFocus();
    SimpleHexEditCtrlBase<SimpleHexEditCtrlWX>::OnMouseLeftDown(event.GetX(), event.GetY());
    }

void SimpleHexEditCtrlWX::OnMouseLeftUp(wxMouseEvent &event)
    {
    event.Skip();
    }

void SimpleHexEditCtrlWX::OnChar(wxKeyEvent &event)
    {
    SimpleHexEditCtrlBase<SimpleHexEditCtrlWX>::OnKeyDown(event.GetKeyCode(), event.GetUnicodeKey(), event.ShiftDown(), event.ControlDown());
    }

#include <windows.h>

bool SimpleHexEditCtrlWX::MSWProcessMessage(WXMSG* pMsg)
    {
    if (pMsg->message == WM_KEYUP)
        {
        if (pMsg->wParam == VK_UP
                || pMsg->wParam == VK_LEFT
                || pMsg->wParam == VK_DOWN
                || pMsg->wParam == VK_RIGHT)
            return true;
        }
    else if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_CHAR)
        {
            bool shift = GetKeyState(VK_SHIFT);
            bool ctrl = GetKeyState(VK_CONTROL);
            switch (pMsg->wParam)
            {
                case VK_UP:
                    SimpleHexEditCtrlBase<SimpleHexEditCtrlWX>::OnKeyDown(WXK_UP, 0, shift, ctrl);
                    break;
                case VK_LEFT:
                    SimpleHexEditCtrlBase<SimpleHexEditCtrlWX>::OnKeyDown(WXK_LEFT, 0, shift, ctrl);
                    break;
                case VK_DOWN:
                    SimpleHexEditCtrlBase<SimpleHexEditCtrlWX>::OnKeyDown(WXK_DOWN, 0, shift, ctrl);
                    break;
                case VK_RIGHT:
                    SimpleHexEditCtrlBase<SimpleHexEditCtrlWX>::OnKeyDown(WXK_RIGHT, 0, shift, ctrl);
                    break;
            }
        return true;
        }
    return wxWindow::MSWProcessMessage(pMsg);
    }

const int SimpleHexEditCtrlWX::GetKeyUp = WXK_UP;
const int SimpleHexEditCtrlWX::GetKeyDown = WXK_DOWN;
const int SimpleHexEditCtrlWX::GetKeyLeft = WXK_LEFT;
const int SimpleHexEditCtrlWX::GetKeyRight = WXK_RIGHT;
const int SimpleHexEditCtrlWX::GetKeyBackSpace = WXK_BACK;
const int SimpleHexEditCtrlWX::GetKeyDelete = WXK_DELETE;
const int SimpleHexEditCtrlWX::GetKeyEnd = WXK_END;
const int SimpleHexEditCtrlWX::GetKeyHome = WXK_HOME;
const int SimpleHexEditCtrlWX::GetKeyInsert = WXK_INSERT;
