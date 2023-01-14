/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef MEMBUFFEREX_H
#define MEMBUFFEREX_H

#include <vector>
#include <algorithm>
#include <stdio.h>
#include <string.h>

#include "MemBuffer.h"
#include "ntst_loggingpp.hpp"
#include "zlib/zlib.h"

class MemBufferEX;

class MemBufferObserver
{
public:
    virtual ~MemBufferObserver() {}
    virtual void MemBufferDataChanged(MemBufferEX &buf, size_t offset, size_t count) = 0;
    void (MemBufferObserver::*my_const_memfunc_ptr)(int, char*) const;
};

class MemBufferEX : public MemBuffer
{
public:
    MemBufferEX(const std::string& bufferName = std::string()) : MemBuffer(bufferName), updateState(false), ninjaMode(false) {}
    MemBufferEX(const std::string& filename, const std::string& bufferName) : MemBuffer(filename, bufferName), updateState(false), ninjaMode(false) {}
    MemBufferEX(const MemBuffer& buf) : MemBuffer(buf), updateState(false), ninjaMode(false) {}
    void Subscribe(void (*callback)(MemBufferEX&, size_t, size_t, void*), void* data);
    bool WriteMem(void* addr, size_t count);
    void WriteZString(size_t offset, size_t size, const char* str);
    std::string ReadString(size_t offset, size_t maxSize);
    bool Set8(size_t offset, int8 val);
    bool Set32(size_t offset, int32 val);
    bool SetInv32(size_t offset, int32 val);
    void Delete(size_t offset, size_t count);
    void Insert(size_t offset, size_t count, void* addr);
    bool Replace(size_t offset, size_t count, void* addr);
    size_t WriteFile(FILE* f, size_t count);
    void Clear();

    void BeginUpdate() {updateState = true;}
    void EndUpdate() {updateState = false; NotifyObservers(0, usize);}
    void BeginNinjaMode() {ninjaMode = true;}
    void EndNinjaMode() {ninjaMode = false;}
private:
    bool updateState;
    bool ninjaMode;
    //typedef void (*Callback)(MemBufferEX&, size_t, size_t);
    typedef struct Callback
        {
        void (*func)(MemBufferEX&, size_t, size_t, void*);
        void *data;
        } Callback;
    void NotifyObservers(size_t offset, size_t count);
    std::vector<Callback> observers;
};

template<typename MemBufferT>
bool LoadGZipFileInMemBuffer(const std::string& path, MemBufferT& buf)
    {
    buf.Clear();
    buf.SetName(path);
    ntst_log_info("Loading gzip file: " + path);
    FILE *f = fopen(path.c_str(), "rb");
    if (f == NULL)
        {
        ntst_log_warning("Can't open gzip file." + ErrnoToStringEnd());
        return false;
        }
    //узнаем размер распакованного файла
    fseek(f, -4, SEEK_END);
    int32 uSize;
    if (fread(&uSize, sizeof(uSize), 1, f) != 1)
        {
        ntst_log_warning("Can't read unpacked file size.");
        fclose(f);
        return false;
        }
    ntst_log_info("Unpacked file size = " + ntst::to_string(uSize));
    fclose(f);
    //распакуем его в память
    void *uSavFile = malloc(uSize);
    if (uSavFile == NULL)
        {
        ntst_log_warning("Can't allocate memory.");
        return false;
        }
    gzFile gzf = gzopen(path.c_str(), "rb");
    if (gzf == NULL)
        {
        ntst_log_warning("Can't gzopen file fo read." + ErrnoToStringEnd());
        free(uSavFile);
        return false;
        }
    if (gzread(gzf, uSavFile, uSize) != uSize)
        {
        ntst_log_warning("Can't gzread file." + ErrnoToStringEnd());
        free(uSavFile);
        gzclose(gzf);
        return false;
        }
    gzclose(gzf);
    buf.Assign(uSavFile, uSize);
    return true;
    }

template<typename MemBufferT>
bool MemBufferSaveInGZipFile(const std::string& path, const MemBufferT& buf)
    {
    ntst_log_info("Saving file: " + path);
    //сохраним файл обратно
    gzFile gzf = gzopen(path.c_str(), "wb");
    if (gzf == NULL)
        {
        ntst_log_warning("Can't gzopen file for write." + ErrnoToStringEnd());
        return false;
        }
    if (gzwrite(gzf, buf.GetAddress(), buf.GetSize()) != (int)buf.GetSize())
        {
        ntst_log_warning("Can't gzwrite file." + ErrnoToStringEnd());
        gzclose(gzf);
        return false;
        }
    gzclose(gzf);
    return true;
    }


#endif // MEMBUFFEREX_H
