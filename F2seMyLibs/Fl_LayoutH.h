#ifndef FL_LAYOUTH_H
#define FL_LAYOUTH_H

#include <FL/Fl_Widget.h>
#include "Fl_Layout.h"

class Fl_LayoutH : public Fl_Layout<Fl_LayoutH>
{
friend class Fl_Layout;

    public:
        Fl_LayoutH(int x, int y, int w, int h, const char *l=0) : Fl_Layout(x, y, w, h, l) {}
    protected:
    static int GetWidgetSize(const Fl_Widget *widget)
        {
        return widget->w();
        }
    int GetWidgetSize2(const Fl_Widget *widget) const
        {
        return h();
        }
    static bool NeedAddExtraToCoordinate(const Fl_Widget *w)
        {
        return AlignLeft(w);
        }
    void SetWidgetNewSizePos(Fl_Widget *widget, int pos, int size, int pos2, int size2)
        {
        widget->resize(x() + pos, y() + pos2, size, size2);
        }
    static int GetLabelEtraSize(const Fl_Widget *w)
        {
        return GetLabelEtraSizeH(w);
        }
    static int GetLabelEtraSize2(const Fl_Widget *w)
        {
        return GetLabelEtraSizeV(w);
        }
    private:
};

#endif // FL_LAYOUTH_H
