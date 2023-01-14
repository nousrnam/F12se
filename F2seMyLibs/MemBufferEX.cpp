/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include "MemBufferEX.h"

void MemBufferEX::Subscribe(void (*callback)(MemBufferEX&, size_t, size_t, void*), void* data)
    {
    Callback c;
    c.func = callback;
    c.data = data;
    observers.push_back(c);
    callback(*this, 0, GetSize(), data);
    }

bool MemBufferEX::WriteMem(void* addr, size_t count)
    {
    bool result = MemBuffer::WriteMem(addr, count);
    if (result && !ninjaMode)
        NotifyObservers(0, count);
    return result;
    }

void MemBufferEX::WriteZString(size_t offset, size_t size, const char* str)
    {
    if (size == 0)
        return;
    size_t realSize = strlen(str);
    if (realSize + 1 > size)
        realSize = size - 1;
    if (offset + size > usize)
        {
        ntst_log_error("MemBufferEX(" + GetName() + ")::WriteZString failed, offset + size > buffer size.");
        return;
        }
    char* dst = (char*)baddr;
    dst += offset;
    memcpy(dst, str, realSize);
    memset(dst + realSize, 0, size - realSize);
    if (!ninjaMode)
        NotifyObservers(offset, size);
    }

std::string MemBufferEX::ReadString(size_t offset, size_t maxSize)
    {
    if (offset + maxSize > usize)
        {
        ntst_log_error("MemBufferEX(" + GetName() + ")::ReadString failed, offset + maxSize > buffer size.");
        return std::string();
        }
    char *curChar = (char*)baddr + offset;
    const char *start = curChar;
    char *endChar = curChar + maxSize;
    while (curChar < endChar)
        {
        if (*curChar == 0)
            break;
        ++curChar;
        }
    return std::string(start, size_t(curChar - start));
    }

bool MemBufferEX::Set8(size_t offset, int8 val)
    {
    bool result = MemBuffer::Set8(offset, val);
    if (result && !ninjaMode)
        NotifyObservers(offset, sizeof(val));
    return result;
    }

bool MemBufferEX::Set32(size_t offset, int32 val)
    {
    bool result = MemBuffer::Set32(offset, val);
    if (result && !ninjaMode)
        NotifyObservers(offset, sizeof(val));
    return result;
    }

bool MemBufferEX::SetInv32(size_t offset, int32 val)
    {
    bool result = MemBuffer::SetInv32(offset, val);
    if (result && !ninjaMode)
        NotifyObservers(offset, sizeof(val));
    return result;
    }

void MemBufferEX::Delete(size_t offset, size_t count)
    {
    MemBuffer::Delete(offset, count);
    if (!ninjaMode)
        NotifyObservers(offset, count);
    }

void MemBufferEX::Insert(size_t offset, size_t count, void* addr)
    {
    MemBuffer::Insert(offset, count, addr);
    if (!ninjaMode)
        NotifyObservers(offset, usize - offset);
    }

bool MemBufferEX::Replace(size_t offset, size_t count, void* addr)
    {
    bool result = MemBuffer::Replace(offset, count, addr);
    if (result && !ninjaMode)
        NotifyObservers(offset, count);
    return result;
    }

size_t MemBufferEX::WriteFile(FILE* f, size_t count)
    {
    if (count > bsize)
        {
        if (!Realloc(count))
            {
            ntst_log_error("MemBufferEX(" + GetName() + ")::WriteFile failed, because Realloc failed.");
            return 0;
            }
        }
    usize = fread(baddr, 1, count, f);
    if (usize != count)
        ntst_log_warning("MemBufferEX(" + GetName() + ")::WriteFile fread != count.");
    if (!ninjaMode)
        NotifyObservers(0, usize);
    return usize;
    }

void MemBufferEX::Clear()
    {
    if (empty())
        return;
    MemBuffer::Clear();
    if (!ninjaMode)
        NotifyObservers(0, 0);
    }

void MemBufferEX::NotifyObservers(size_t offset, size_t count)
    {
    if (updateState)
        return;
    for (std::vector<Callback>::iterator it = observers.begin(); it != observers.end(); ++it)
        (*it).func(*this, offset, count, (*it).data);
    }
