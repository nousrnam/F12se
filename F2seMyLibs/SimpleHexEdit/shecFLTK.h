#ifndef SIMPLEHEXEDITCTRLFLTK_H
#define SIMPLEHEXEDITCTRLFLTK_H

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <vector>
#include "shecBase.h"

class SimpleHexEditCtrlFltk : public Fl_Widget, public SimpleHexEditCtrlBase<SimpleHexEditCtrlFltk>
    {
friend class SimpleHexEditCtrlBase;
public:
    SimpleHexEditCtrlFltk(int x, int y, int w, int h, const char* l = 0) : Fl_Widget(x, y, w, h, l) {}
    virtual int handle(int event);
    virtual void draw() {DrawCtrl();}
protected:
    void ClearBackground()
    {
    fl_color(FL_BACKGROUND_COLOR);
    fl_rectf(x(), y(), w(), h());
    fl_color(FL_FOREGROUND_COLOR);
    }
    bool HaveFocus() const {return this == Fl::focus();}
    int GetW() const {return w();}
    int GetH() const {return h();}
    void NeedRedraw() {redraw();}
    void PushClip() {fl_push_clip(x(), y(), w(), h());}
    void PopClip() {fl_pop_clip();}
    int GetFontHeight() const {return fl_height();}
    int GetCharWidth(unsigned char c) const {return fl_width(c);}
    void DrawText(const char* text, int x_, int y_) {fl_draw(text, x_ + x(), y_ + y() - fl_descent() + fl_height());}
    void DrawLine(int x1, int y1, int x2, int y2) {fl_line(x1 + x(), y1 + y(), x2 + x(), y2 + y());}
    void DrawRect(int x_, int y_, int w_, int h_) {fl_rect(x_ + x(), y_ + y(), w_, h_);}
    void FillRect(int x_, int y_, int w_, int h_) {fl_rectf(x_ + x(), y_ + y(), w_, h_);}
    //static int GetKeyUp() {return FL_Up;}
    static const int GetKeyUp = FL_Up;
    static const int GetKeyDown = FL_Down;
    static const int GetKeyLeft = FL_Left;
    static const int GetKeyRight = FL_Right;
    static const int GetKeyBackSpace = FL_BackSpace;
    static const int GetKeyDelete = FL_Delete;
    static const int GetKeyEnd = FL_End;
    static const int GetKeyHome = FL_Home;
    static const int GetKeyInsert = FL_Insert;
    static const int GetKeyShift = FL_Shift_L;
    };

#endif // SIMPLEHEXEDITCTRLFLTK_H
