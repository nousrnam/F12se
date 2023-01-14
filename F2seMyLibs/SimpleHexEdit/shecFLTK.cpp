#include "shecFLTK.h"

int SimpleHexEditCtrlFltk::handle(int event)
    {
    switch (event)
        {
        case FL_PUSH:
            take_focus();
            if (Fl::event_button() == FL_LEFT_MOUSE)
                OnMouseLeftDown(Fl::event_x() - x(), Fl::event_y() - y());
            return 1;
        case FL_MOUSEWHEEL:
            OnMouseWheel(Fl::event_dx(), Fl::event_dy());
            return 1;
        case FL_FOCUS:
            return 1;
        case FL_UNFOCUS:
            redraw();
            return 1;
        case FL_PASTE:
            return 1;
        case FL_KEYDOWN:
            if (Fl::event_key() == FL_Shift_R)
                OnKeyDown(FL_Shift_L, *Fl::event_text());
            else
                OnKeyDown(Fl::event_key(), *Fl::event_text());
            return 1;
        default:
            return Fl_Widget::handle(event);
        }
    }
