/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include <stdlib.h>
#include <string.h>

#include "MemBuffer.h"
#include "ntst_loggingpp.hpp"

std::string ErrnoToStringEnd()
    {
    return " errno(" + ntst::to_string(errno) + "):" + strerror(errno);
    }

MemBuffer::MemBuffer(const std::string& bufferName) : name(bufferName)
    {
    Init();
    }

MemBuffer::MemBuffer(const std::string& filename, const std::string& bufferName) : name(bufferName)
    {
    Init();
    LoadFile(filename);
    }

bool MemBuffer::LoadFile(const std::string& filename)
    {
    Clear();
    FILE* f = fopen(filename.c_str(), "rb");
    if (f == NULL)
        {
        ntst_log_error("MemBuffer(" + GetName() + ")::LoadFile() failed, can't open file: " + filename + ErrnoToStringEnd());
        return false;
        }
    size_t filesize = GetFileSize(f);
    if (!Realloc(filesize))
        {
        fclose(f);
        ntst_log_error("MemBuffer(" + GetName() + ")::LoadFile() failed, can't allocate " + ntst::to_string(filesize) + " bytes to load file: " + filename);
        return false;
        }
    size_t readsize = fread(baddr, 1, filesize, f);
    if (readsize != filesize)
        ntst_log_warning("MemBuffer(" + GetName() + ")::LoadFile() somehow readsize(" + ntst::to_string(readsize) + ") != filesize(" + ntst::to_string(filesize) + ')');
    usize = readsize;
    fclose(f);
    return true;
    }

MemBuffer::MemBuffer(const MemBuffer& buf) : name(buf.name)
    {
    const size_t size = buf.usize;
    if (size == 0)
        {
        Init();
        return;
        }
    baddr = malloc(size);
    if (baddr == NULL)
        {
        Init();
        ntst_log_error("MemBuffer(" + GetName() + ")::MemBuffer(const MemBuffer& buf) failed, can't allocate " + ntst::to_string(size) + " bytes.");
        return;
        }
    memcpy(baddr, buf.baddr, size);
    usize = bsize = size;
    }

MemBuffer& MemBuffer::operator=(const MemBuffer& buf)
    {
    if (!Realloc(buf.usize))
        {
        ntst_log_error("MemBuffer(" + GetName() + ")::operator=(const MemBuffer& buf) failed, can't allocate " + ntst::to_string(buf.usize) + " bytes.");
        return *this;
        }
    name = buf.name;
    usize = buf.usize;
    memcpy(baddr, buf.baddr, usize);
    return *this;
    }

MemBuffer::~MemBuffer()
    {
    free(baddr);
    }

void MemBuffer::Clear()
    {
    free(baddr);
    Init();
    }

bool MemBuffer::Realloc(size_t newSize)
    {
    if (newSize == 0)
        Clear();
    else
        {
        void* newAddr = realloc(baddr, newSize);
        if (newAddr == NULL)
            {
            ntst_log_error("Memory reallocation failed in MemBuffer(" + GetName() + ")::Realloc. Requested size is " + ntst::to_string(newSize) + '.');
            return false;
            }
        baddr = newAddr;
        bsize = newSize;
        if (usize > bsize)
            usize = bsize;
        }
    return true;
    }

bool MemBuffer::Resize(size_t newSize)
    {
    if (!Realloc(newSize))
        return false;
    usize = newSize;
    return true;
    }

bool MemBuffer::WriteMem(void* addr, size_t count)
    {
    if (count > bsize)
        {
        if (!Realloc(count))
            {
            ntst_log_error("MemBuffer(" + GetName() + ")::WriteMem failed, because MemBuffer::Realloc failed.");
            return false;
            }
        }
    usize = count;
    memcpy(baddr, addr, count);
    return true;
    }

void MemBuffer::Delete(size_t offset, size_t count)
    {
    if (offset + count >= usize)
        {
        usize = offset;
        }
    else
        {
        uintptr_t dst = (uintptr_t)baddr + offset;
        memmove((void*)dst, (void*)(dst + count), usize - offset - count);
        usize-=count;
        }
    }

void MemBuffer::Insert(size_t offset, size_t count, void* addr)
    {
    if (offset > usize)
        {
        ntst_log_error("MemBuffer(" + GetName() + ")::Insert failed, because offset(" + ntst::to_string(offset) + ") > size(" + ntst::to_string(usize) + ").");
        return;
        }
    const size_t newusize = usize + count;
    const size_t bytesToMove = usize - offset;
    if (newusize > bsize)
        {
        if (!Realloc(newusize))
            {
            ntst_log_error("MemBuffer(" + GetName() + ")::Insert failed, because MemBuffer::Realloc failed.");
            return;
            }
        }
    uintptr_t dst = (uintptr_t)baddr + offset;
    memmove((void*)(dst + count), (void*)dst, bytesToMove);
    memcpy((void*)dst, addr, count);
    usize = newusize;
    }

bool MemBuffer::Replace(size_t offset, size_t count, void* addr)
    {
    if (offset + count > usize)
        {
        ntst_log_error("MemBuffer(" + GetName() + ")::Replace failed, because offset(" + ntst::to_string(offset) + ") + count(" + ntst::to_string(count) + ") > size(" + ntst::to_string(usize) + ").");
        return false;
        }
    memcpy(GetAddress(offset), addr, count);
    return true;
    }

void MemBuffer::Assign(void* addr, size_t size)
    {
    free(baddr);
    baddr = addr;
    bsize = usize = size;
    }

bool MemBuffer::Get16(size_t offset, int16 *val) const
    {
    if (offset + sizeof(*val) > usize)
        {
        ntst_log_error("MemBuffer(" + GetName() + ")::Get16 failed, because offset(" + ntst::to_string(offset) + ") + sizeof(int16) > size(" + ntst::to_string(usize) + ").");
        return false;
        }
    *val = *(int16*)((uintptr_t)baddr + offset);
    return true;
    }

int16 MemBuffer::Get16(size_t offset) const
    {
    int16 val;
    if (Get16(offset, &val))
        return val;
    return 0;
    }

bool MemBuffer::Get32(size_t offset, int32 *val) const
    {
    if (offset + sizeof(*val) > usize)
        {
        ntst_log_error("MemBuffer(" + GetName() + ")::Get32 failed, because offset(" + ntst::to_string(offset) + ") + sizeof(int32) > size(" + ntst::to_string(usize) + ").");
        return false;
        }
    *val = *(int32*)((uintptr_t)baddr + offset);
    return true;
    }

int32 MemBuffer::Get32(size_t offset) const
    {
    int32 val;
    if (Get32(offset, &val))
        return val;
    return 0;
    }

bool MemBuffer::GetInv32(size_t offset, int32 *val) const
    {
    bool result = Get32(offset, val);
    if (result)
        *val = ntst_bytes_swap32(*val);
    return result;
    }

bool MemBuffer::GetSubBuffer(size_t offset, size_t length, MemBuffer &buf) const
    {
    if (offset + length > usize)
        {
        ntst_log_error("MemBuffer(" + GetName() + ")::GetSubBuffer failed, because offset(" + ntst::to_string(offset) + ") + length(" + ntst::to_string(length) + ") > size(" + ntst::to_string(usize) + ").");
        return false;
        }
    return buf.WriteMem(GetAddress(offset), length);
    }

int8 MemBuffer::Get8(size_t offset) const
    {
    if (offset + sizeof(int8) > usize)
        {
        ntst_log_error("MemBuffer(" + GetName() + ")::Get8 failed, because offset(" + ntst::to_string(offset) + ") + sizeof(int8) > size(" + ntst::to_string(usize) + ").");
        return 0;
        }
    return ((int8*)baddr)[offset];
    }

bool MemBuffer::Set32(size_t offset, int32 val)
    {
    if (offset + sizeof(val) > usize)
        {
        ntst_log_error("MemBuffer(" + GetName() + ")::Set32 failed, because offset(" + ntst::to_string(offset) + ") + sizeof(int32) > size(" + ntst::to_string(usize) + ").");
        return false;
        }
    int32 *oldval = (int32*)((uintptr_t)baddr + offset);
    *oldval = val;
    return true;
    }

bool MemBuffer::Set8(size_t offset, int8 val)
    {
    if (offset + sizeof(val) > usize)
        {
        ntst_log_error("MemBuffer(" + GetName() + ")::Set8 failed, because offset(" + ntst::to_string(offset) + ") + sizeof(int8) > size(" + ntst::to_string(usize) + ").");
        return false;
        }
    int8 *oldval = (int8*)((uintptr_t)baddr + offset);
    *oldval = val;
    return true;
    }

bool MemBuffer::SaveInFile(const std::string& filename)
    {
    if (baddr == NULL)
        return false;
    FILE* f = fopen(filename.c_str(), "wb");
    if (f == NULL)
        {
        ntst_log_error("MemBuffer(" + GetName() + ")::SaveInFile(const std::string& filename) failed, can't open file: " + filename + ErrnoToStringEnd());
        return false;
        }
    size_t writtenbytes = fwrite(baddr, 1, GetSize(), f);
    if (writtenbytes != GetSize())
        ntst_log_error("Failed to save memory buffer with name: " + GetName() +
                " in file: " + filename +
                "buffer size(" + ntst::to_string(GetSize()) + ") != the number of bytes written(" + ntst::to_string(writtenbytes) + ')');
    fclose(f);
    return true;
    }
