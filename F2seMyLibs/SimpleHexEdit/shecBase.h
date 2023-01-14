/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef SIMPLEHEXEDITCTRLBASE_H
#define SIMPLEHEXEDITCTRLBASE_H

#include <vector>
#include <climits>

#if _MSC_VER
//TODO: _snprintf! U NO NULL TERMINATOR!!! "If len > count, then count characters are stored in buffer, no null-terminator is appended"
#define snprintf _snprintf
#endif

#define ROW_SPACE 2
#define ROW_SPACE_HALF (ROW_SPACE/2)

template<class Derived>
class SimpleHexEditCtrlBase
    {
public:
    SimpleHexEditCtrlBase() {Init();}
    void Clear();
    void Insert(unsigned char data);
    void Replace(unsigned char data);
    void SetData(char* data, size_t dataSize);
    void SetData(void* data, size_t dataSize) {SetData((char*)data, dataSize);}
    void SetSelectionStartPos(int pos) {SetSomePos(pos, selectionStartPos);}
    int GetSelectionStartPos() const {return selectionStartPos;}
    void SetSelectionEndPos(int pos) {SetSomePos(pos, selectionEndPos);}
    int GetSelectionEndPos() const {return selectionEndPos;}
    int GetCaretPos() const {return selectionEndPos;}
    void SetCaretPos(int pos)
        {
        SetSelectionStartPos(pos);
        SetSelectionEndPos(pos);
        }
    void SetInsertMode(bool mode)
        {
        if (insertMode != mode)
            {
            insertMode = mode;
            static_cast<Derived*>(this)->NeedRedraw();
            }
        }
    bool GetInsertMode() const {return insertMode;}
    void DrawByteAsHex(int x, int y, unsigned char b, int charWidth);
    void DrawCtrl()
    {
    static_cast<Derived*>(this)->ClearBackground();
    const int clientWidth = static_cast<Derived*>(this)->GetW();
    const int clientHeight = static_cast<Derived*>(this)->GetH();
    static_cast<Derived*>(this)->PushClip();
    int xCoordOffset = -colOffset * colWidth;
    //Header Row
    /*char offsetFormat[6];
    char textBuf[10];
    snprintf(offsetFormat, sizeof(offsetFormat), "%c0%uX", '%' , offsetDigits);
    snprintf(textBuf, sizeof(textBuf), offsetFormat, selectionEndPos / 2);
    static_cast<Derived*>(this)->DrawText(textBuf, 0, 0);*/
    //caret
    if (static_cast<Derived*>(this)->HaveFocus())
        {
        const int caretLine = selectionEndPos / 2 / bytesPerRow;
        int caretX;
        if (hexColSelected)
            {
            caretX = xCoordOffset + GetColHexStartPixelOffset() + (selectionEndPos % (2 * bytesPerRow)) / 2 * 3 * colWidth;
            if (selectionEndPos & 1)
                caretX += colWidth;
            }
        else
            caretX = xCoordOffset + GetColTextStartPixelOffset() + ((selectionEndPos / 2) % bytesPerRow) * colWidth;
        int caretY = (caretLine - lineOffset + 1) * rowHeight;
        if (caretY > 0)
            {
            if (GetInsertMode())
                static_cast<Derived*>(this)->DrawLine(caretX, caretY, caretX, caretY + rowHeight);
            else
                static_cast<Derived*>(this)->DrawRect(caretX, caretY, colWidth, rowHeight);
            }
        }
    // offset
    for (unsigned i = 0; i < offsetBytes; ++i)
        DrawByteAsHex(xCoordOffset + 2 * colWidth * (offsetBytes - i - 1), 0, (selectionEndPos / 2 >> (i * 8)) & 0xff, colWidth);
    // Hex offsets
    for (unsigned i = 0; i < bytesPerRow; ++i)
        DrawByteAsHex(xCoordOffset + GetColHexStartPixelOffset() + i * colWidth * 3, 0, i, colWidth);
    //Data rows
    int lastLineToDraw = lineOffset + GetRowWithDataToDrawCount();
    const int fulllinesCount = GetFullDataRowCount();
    if (lastLineToDraw > fulllinesCount)
        lastLineToDraw = fulllinesCount;
    int yOffset = rowHeight;
    unsigned char charAsText[2];
    charAsText[1] = 0;
    for (int line = lineOffset; line <= lastLineToDraw; ++line)
        {
        //offsets
        /*snprintf(textBuf, sizeof(textBuf), offsetFormat, line * bytesPerRow);
        static_cast<Derived*>(this)->DrawText(textBuf, 0, yOffset);*/
        for (unsigned i = 0; i < offsetBytes; ++i)
            DrawByteAsHex(xCoordOffset + 2 * colWidth * (offsetBytes - i - 1), yOffset, (line * bytesPerRow >> (i * 8)) & 0xff, colWidth);

        const unsigned bytesInCurRow = GetBytesCountInRow(line);
        for (unsigned i = 0; i < bytesInCurRow; ++i)
            {
            //hex
            charAsText[0] = buffer[line * bytesPerRow + i];
            DrawByteAsHex(xCoordOffset + GetColHexStartPixelOffset() + i * colWidth * 3, yOffset, charAsText[0], colWidth);
            //text
            const int charWidth = static_cast<Derived*>(this)->GetCharWidth(charAsText[0]);
            int charOffsetX = (colWidth - charWidth) / 2;
            static_cast<Derived*>(this)->DrawText((char*)charAsText, xCoordOffset +  GetColTextStartPixelOffset() + i * colWidth + charOffsetX, yOffset);
            }
        yOffset += rowHeight;
        }
    const int colWidthD2 = colWidth / 2;
    //horizontal line
    static_cast<Derived*>(this)->DrawLine(0, rowHeight - ROW_SPACE_HALF, clientWidth, rowHeight - ROW_SPACE_HALF);
    //vertical lines
    const int vline1x = xCoordOffset + colOffsetWidth + colWidthD2;
    static_cast<Derived*>(this)->DrawLine(vline1x, 0, vline1x, clientHeight);
    const int vline2x = xCoordOffset + colOffsetWidth + GetColHexTotalWidth();
    static_cast<Derived*>(this)->DrawLine(vline2x, 0, vline2x, clientHeight);

    static_cast<Derived*>(this)->PopClip();
    }
protected:
    void Init()
        {
        bytesPerRow = 16;
        offsetBytes = 2;

        for (int i = 0; i < 168; ++i)
            buffer.push_back(100-i);
        insertMode = false;
        hexColSelected = true;
        lineOffset = 0;
        colOffset = 0;
        selectionStartPos = 0;
        selectionEndPos = 0;
        colOffsetWidth = 1;
        Adjust();
        }
    std::vector<unsigned char> buffer;
    bool insertMode;
    bool hexColSelected;
    int lineOffset;
    int colOffset;
    int selectionStartPos;
    int selectionEndPos;
    unsigned bytesPerRow;
    unsigned offsetBytes;
    int colOffsetWidth;
    int colWidth;
    int rowHeight;
    int GetColHexTotalWidth() const {return colWidth * 3 * bytesPerRow;}
    int GetColHexStartPixelOffset() const {return colOffsetWidth + colWidth;}
    int GetColTextTotalWidth() const {return colWidth * bytesPerRow;}
    int GetColTextStartPixelOffset() const {return GetColHexStartPixelOffset() + GetColHexTotalWidth() + colWidth;}
    int GetRowWithDataToDrawCount() const {return ((static_cast<const Derived*>(this)->GetH() + rowHeight / 2) / rowHeight) - 1/* Header line*/;}
    int GetFullDataRowCount() const {return buffer.size() / bytesPerRow;}
    int GetBytesCountInIncompleteRow() const {return buffer.size() % bytesPerRow;}
    unsigned GetTotalColNum() const {return (offsetBytes * 2 + 1) + (bytesPerRow * 3 + 1) + bytesPerRow;}
    unsigned GetVisibleColNum() const {return static_cast<const Derived*>(this)->GetW() / colWidth;}
    unsigned GetTotalRowNum() const {return GetFullDataRowCount() + (GetBytesCountInIncompleteRow() > 0 ? 1 : 0);}
    unsigned GetVisibleRowNum() const {return static_cast<const Derived*>(this)->GetH() / rowHeight;}
    int GetLineOffset() const {return lineOffset;}
    void SetLineOffset(int offset) {if (lineOffset == offset) return; lineOffset = offset; static_cast<Derived*>(this)->NeedRedraw();}
    int GetColOffset() const {return colOffset;}
    void SetColOffset(int offset) {if (colOffset == offset) return; colOffset = offset; static_cast<Derived*>(this)->NeedRedraw();}
    int GetBytesCountInRow(unsigned row)
        {
        if (row < (unsigned)GetFullDataRowCount())
            return bytesPerRow;
        else if (row > (unsigned)GetFullDataRowCount())
            return 0;
        else
            return GetBytesCountInIncompleteRow();
        }
    void OnMouseLeftDown(int x, int y);
    void OnMouseWheel(int /*dx*/, int dy);
    void OnKeyDown(int key, unsigned char c, bool shiftMod, bool ctrlMod);
    void Adjust() //Resize or font chage
        {
        colWidth = static_cast<Derived*>(this)->GetCharWidth('W');
        rowHeight = static_cast<Derived*>(this)->GetFontHeight() + ROW_SPACE;
        colOffsetWidth = colWidth * offsetBytes * 2;
        static_cast<Derived*>(this)->AdjustScrollbars();
        }
private:
    void SetSomePos(int pos, int& some)
        {
        if (pos < 0)
            pos = 0;
        else if ((unsigned)pos > buffer.size() * 2)
            pos = buffer.size() * 2;
        if (some != pos)
            {
            some = pos;
            static_cast<Derived*>(this)->NeedRedraw();
            }
        }
    };

template<class Derived>
void SimpleHexEditCtrlBase<Derived>::Clear()
    {
    buffer.clear();
    selectionStartPos = 0;
    selectionEndPos = 0;
    lineOffset = 0;
    colOffset = 0;
    static_cast<Derived*>(this)->NeedRedraw();
    }

template<class Derived>
void SimpleHexEditCtrlBase<Derived>::Insert(unsigned char data)
    {
    std::vector<unsigned char>::iterator it = buffer.begin() + (unsigned)selectionEndPos / 2;
    if (it >= buffer.end())
        buffer.insert(buffer.end(), data);
    else
        buffer.insert(it, data);
    static_cast<Derived*>(this)->NeedRedraw();
    }

template<class Derived>
void SimpleHexEditCtrlBase<Derived>::Replace(unsigned char data)
    {
    std::size_t pos = (unsigned)selectionEndPos / 2;
    if (pos >= buffer.size())
        buffer.insert(buffer.end(), data);
    else
        buffer[pos] = data;
    SetCaretPos(GetCaretPos() + 2);
    static_cast<Derived*>(this)->NeedRedraw();
    }

template<class Derived>
void SimpleHexEditCtrlBase<Derived>::SetData(char* data, size_t dataSize)
    {
    buffer.assign(data, data + dataSize);
    selectionStartPos = 0;
    selectionEndPos = 0;
    lineOffset = 0;
    colOffset = 0;
    Adjust();
    static_cast<Derived*>(this)->NeedRedraw();
    }

template<class Derived>
void SimpleHexEditCtrlBase<Derived>::DrawByteAsHex(int x, int y, unsigned char b, int charWidth)
    {
    char textBuf[2];
    snprintf(textBuf, sizeof(textBuf), "%X", (int)(b / 16));
    static_cast<Derived*>(this)->DrawText(textBuf, x, y);
    snprintf(textBuf, sizeof(textBuf), "%X", (int)(b % 16));
    static_cast<Derived*>(this)->DrawText(textBuf, x + charWidth, y);
    }

template<class Derived>
void SimpleHexEditCtrlBase<Derived>::OnMouseLeftDown(int x, int y)
    {
    x += colOffset * colWidth;
    if (y <= rowHeight)
        return;
    int line = lineOffset + y / rowHeight - 1;
    int newCaretPos = line * bytesPerRow * 2;
    int col;
    int colWidthD2 = colWidth / 2;
    if (x >= GetColTextStartPixelOffset() - colWidthD2)
        {
        col = 2 * (x - GetColTextStartPixelOffset()) / colWidth;
        hexColSelected = false;
        }
    else
        {
        col = (x - GetColHexStartPixelOffset() + colWidthD2) * 2 / (3 * colWidth);
        hexColSelected = true;
        }
    if (col < 0)
        col = 0;
    if ((unsigned)col >= bytesPerRow*2)
        col = bytesPerRow*2 - 1;
    newCaretPos += col;
    SetCaretPos(newCaretPos);
    }

template<class Derived>
void SimpleHexEditCtrlBase<Derived>::OnMouseWheel(int /*dx*/, int dy)
    {
    int newCurLine = lineOffset + dy;
    if (newCurLine >= GetFullDataRowCount())
        {
        newCurLine = GetFullDataRowCount();
        if (GetBytesCountInIncompleteRow() == 0)
            newCurLine--;
        }
    if (newCurLine < 0)
        newCurLine = 0;
    if (newCurLine != lineOffset)
        {
        SetLineOffset(newCurLine);
        static_cast<Derived*>(this)->NeedRedraw();
        }
    }

template<class Derived>
void SimpleHexEditCtrlBase<Derived>::OnKeyDown(int key, unsigned char c, bool /*shiftMod*/, bool /*ctrlMod*/)
    {
    switch (key)
        {
        case Derived::GetKeyUp:
            SetCaretPos(GetCaretPos() - 2 * bytesPerRow);
            break;
        case Derived::GetKeyDown:
            SetCaretPos(GetCaretPos() + 2 * bytesPerRow);
            break;
        case Derived::GetKeyLeft:
            if (hexColSelected)
                SetCaretPos(GetCaretPos() - 1);
            else
                SetCaretPos(GetCaretPos() - 2);
            break;
        case Derived::GetKeyRight:
            if (hexColSelected)
                SetCaretPos(GetCaretPos() + 1);
            else
                SetCaretPos(GetCaretPos() + 2);
            break;
        case Derived::GetKeyBackSpace:
            break;
        case Derived::GetKeyDelete:
            break;
        case Derived::GetKeyEnd:
            SetCaretPos(INT_MAX);
            break;
        case Derived::GetKeyHome:
            SetCaretPos(0);
            break;
        case Derived::GetKeyInsert:
            SetInsertMode(!GetInsertMode());
            return;
        default:
            if (GetInsertMode())
                Insert(c);
            else
                Replace(c);
            break;
        }
    static_cast<Derived*>(this)->NeedRedraw();
    }

#endif // SIMPLEHEXEDITCTRLBASE_H
