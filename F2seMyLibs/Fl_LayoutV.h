#ifndef FL_LAYOUTV_H
#define FL_LAYOUTV_H

#include <FL/Fl_Widget.h>
#include "Fl_Layout.h"

class Fl_LayoutV : public Fl_Layout<Fl_LayoutV>
{
friend class Fl_Layout;

    public:
        Fl_LayoutV(int x, int y, int w, int h, const char *l=0) : Fl_Layout(x, y, w, h, l) {}
    protected:
    static int GetWidgetSize(const Fl_Widget *widget)
        {
        return widget->h();
        }
    int GetWidgetSize2(const Fl_Widget *widget) const
        {
        return w();
        }
    static bool NeedAddExtraToCoordinate(const Fl_Widget *w)
        {
        return AlignTop(w);
        }
    static int GetLabelEtraSize(const Fl_Widget *w)
        {
        return GetLabelEtraSizeV(w);
        }
    static int GetLabelEtraSize2(const Fl_Widget *w)
        {
        return GetLabelEtraSizeH(w);
        }
    void SetWidgetNewSizePos(Fl_Widget *widget, int pos, int size, int pos2, int size2)
        {
        widget->resize(x() + pos2, y() + pos, size2, size);
        }
    private:
};

#endif // FL_LAYOUTV_H
