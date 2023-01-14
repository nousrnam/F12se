#ifndef FL_LAYOUT_H
#define FL_LAYOUT_H

#include <assert.h>
#include <vector>
#include <FL/Fl.h>
#include <Fl/Fl_Group.H>
#include <FL/Fl_draw.H>

typedef enum
    {
    EXTEND,
    ALIGN_CENTER,
    ALIGN_LEFT,
    ALIGN_TOP = ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_BOTTOM = ALIGN_RIGHT
    } Behaviour;

typedef struct
    {
    unsigned proportion;
    int minSize;
    int maxSize;
    Behaviour b;
    } LayoutItemData;

template<class Derived>
class Fl_Layout : public Fl_Group
    {
public:
    Fl_Layout(int x, int y, int w, int h, const char *l=0) : Fl_Group(x, y, w, h, l), proportionsSum(0)
        {
        Fl_Group::end();
#ifndef NDEBUG
        box(FL_BORDER_BOX);
#endif
        }
    virtual ~Fl_Layout() {clear();}
    void add(Fl_Widget *w, unsigned proportion = 1, Behaviour b = EXTEND, int minSize = -1, int maxSize = -1)
        {
        Fl_Group::add(w);
        proportionsSum += proportion;
        LayoutItemData d;
        d.b = b;
        d.minSize = minSize;
        d.maxSize = maxSize;
        d.proportion = proportion;
        children_data.push_back(d);
        }
    void clear()
        {
        Fl_Group::clear();
        children_data.clear();
        }
    int spacing() const {return spacing_;}
    void spacing(int i) {spacing_ = i;}
    void resize(int X, int Y, int W, int H)
        {
        Fl_Widget::resize(X,Y,W,H);
        Layout();
        }
    void Layout()
        {
        assert((size_t)children() == children_data.size());
        int curCoord = 0;
        const int spaceForResize = GetSpaceForResizing();
        for (int i = 0; i < children(); ++i)
            {
            Fl_Widget *w = child(i);
            if (!w->visible())
                continue;
            const int extra = Derived::GetLabelEtraSize(w);
            const int extra2 = Derived::GetLabelEtraSize2(w);
            int newSize, newSize2;
            GetWidgetNewSize(w, spaceForResize, proportionsSum, children_data[i], &newSize, &newSize2);
            int newSizeModified = newSize;
            if (newSizeModified < 0)
                newSizeModified = 0;
            if (Derived::NeedAddExtraToCoordinate(w))
                {
                static_cast<Derived*>(this)->SetWidgetNewSizePos(w, curCoord + extra, newSizeModified, extra2, newSize2 - extra2);
                curCoord += extra;
                }
            else
                {
                static_cast<Derived*>(this)->SetWidgetNewSizePos(w, curCoord, newSizeModified, extra2, newSize2 - extra2);
                }

            curCoord += (spacing() + newSize);
            }
        }
protected:
    std::vector<LayoutItemData> children_data;
    int spacing_;
    int GetSpaceForResizing() const
        {
        int space = Derived::GetWidgetSize(this);
        int visibleCount = 0;
        for (int i = 0; i < children(); ++i)
            {
            if (!child(i)->visible())
                continue;
            visibleCount++;
            const int extra = Derived::GetLabelEtraSize(child(i));
            space -= extra;
            if (children_data[i].proportion == 0)
                {
                const int size = Derived::GetWidgetSize(child(i));
                space -= size;
                }
            }
        if (visibleCount > 1)
            space -= (visibleCount - 1) * spacing();
        return space;
        }
    void GetWidgetNewSize(Fl_Widget *w, int layoutSize, unsigned proportionsSum, const LayoutItemData& data, int *size, int *size2) const
        {
        *size = Derived::GetWidgetSize(w);
        *size2 = static_cast<const Derived*>(this)->GetWidgetSize2(w);
        if (data.proportion == 0)
            return;
        if (layoutSize < 0)
            {
            *size = 0;
            return;
            }
        if (proportionsSum != 0)
            {
            *size = (layoutSize * data.proportion + proportionsSum / 2) / proportionsSum;
            if (data.maxSize >=0 && *size > data.maxSize)
                *size = data.maxSize;
            else if (data.minSize >=0 && *size < data.minSize)
                *size = data.minSize;
            }
        }
    static bool AlignLeft(const Fl_Widget *w)
        {
        return w->align() & (FL_ALIGN_LEFT | FL_ALIGN_LEFT_TOP | FL_ALIGN_BOTTOM_LEFT);
        }
    static bool AlignTop(const Fl_Widget *w)
        {
        return w->align() & (FL_ALIGN_TOP | FL_ALIGN_LEFT_TOP | FL_ALIGN_RIGHT_TOP);
        }
    static int GetLabelEtraSizeV(const Fl_Widget *w)
        {
        if (w->label() == NULL || *w->label() == 0)
            return 0;
        if ((w->align() == FL_ALIGN_CENTER) || (w->align() & FL_ALIGN_INSIDE))
            return 0;
        if (w->align() & (FL_ALIGN_TOP | FL_ALIGN_BOTTOM))
            return w->labelsize();
        return 0;
        }
    static int GetLabelEtraSizeH(const Fl_Widget *w)
        {
        if (w->label() == NULL || *w->label() == 0)
            return 0;
        if ((w->align() == FL_ALIGN_CENTER) || (w->align() & (FL_ALIGN_INSIDE | FL_ALIGN_TOP | FL_ALIGN_BOTTOM)))
            return 0;
        if (w->align() & (FL_ALIGN_LEFT | FL_ALIGN_RIGHT))
            return fl_width(w->label()) + 3 + 0.5;// +3 is result of magic constants in FLTK, +0.5 to round up.
        return 0;
        }
private:
    unsigned proportionsSum;
    };

#endif // FL_LAYOUT_H
