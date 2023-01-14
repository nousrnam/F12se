/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef FRMH
#define FRMH

// TODO full rewrite. should return rgba array or rgb and alpha arrays for wxWidgets...
// Somehow get rid of windows.h(ifdef _WIN32), but provide method for header in save.dat to convert indicies to rgb array.

#include <string>
#include "commontypes.h"
#include "FileFinder.h"
#include "VecOfStr.h"
#include "MemBuffer.h"

class FileFinder;

#define IMAGES_TYPES_NUM 11
#define GAMMA_ARRAY_SIZE 64

typedef unsigned char PaletteRGB[256*3];

//#define FRM_OLD_CODE 1
#ifdef FRM_OLD_CODE
#include <windows.h>
#endif

class RGBImage
    {
public:
    RGBImage() : width(0), height(0) {}
    RGBImage(const MemBuffer& colorIndicies, size_t startOffset, const PaletteRGB& palette, unsigned w, unsigned h);
    MemBuffer rgbArray;
    unsigned GetWidth() const {return width;}
    unsigned GetHeight() const {return height;}
protected:
    unsigned width;
    unsigned height;
    };

class RGBAImage : public RGBImage
    {
public:
    RGBAImage() : RGBImage() {}
    RGBAImage(const MemBuffer& colorIndicies, size_t startOffset, const PaletteRGB& palette, unsigned w, unsigned h);
    MemBuffer alphaArray;
    };

class FrmWorker
    {
public:
    FrmWorker();
    void InitAll(FileFinder& ff);
#ifdef FRM_OLD_CODE
    bool LoadFrmByNameInBmp(const std::string& name, FileFinder& ff, MemBuffer &out);
    bool LoadFrmByIdInBmp(uint32 id, FileFinder& ff, MemBuffer &out);
    int32 palmain[256];
    int32 palfrm[256];
#endif
    bool LoadFrmByNameInRGBA(const std::string& name, FileFinder& ff, RGBAImage &out);
    bool LoadFrmByIdInRGBA(uint32 id, FileFinder& ff, RGBAImage &out);
    PaletteRGB palMain; // TODO init in constructor
    PaletteRGB palFrm; // TODO init in constructor
    std::string GetNameFromId(int32 id);
    void SetGamma(double gamma);
    double GetGamma() const {return currentgamma;}
    //static void ColorArrayToRgbArray(unsigned char *in, size_t inSize, int32 palette[256], unsigned char *out);
private:
    VectorOfStrings frmlist[IMAGES_TYPES_NUM];
    uint8 gammatable[GAMMA_ARRAY_SIZE];
    double currentgamma;
    bool LoadPal(const std::string& filename, FileFinder& ff, PaletteRGB& pal);
#ifdef FRM_OLD_CODE
    bool LoadPal(const std::string& filename, FileFinder& ff, int32 pal[256]);
    BITMAPFILEHEADER fheader;
    BITMAPINFOHEADER header;
#endif
    };

#endif
