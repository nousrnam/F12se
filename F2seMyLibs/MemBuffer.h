/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef MEMBUFFER_H
#define MEMBUFFER_H

#include <stdint.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include "misc.h"
#include "ntst_bytes.h"
#include "ntst_stringpp.hpp"

std::string ErrnoToStringEnd();

class MemBuffer
    {
public:
    MemBuffer(const std::string& bufferName = std::string());
    MemBuffer(const std::string& filename, const std::string& bufferName);
    MemBuffer(const MemBuffer& buf);
    MemBuffer& operator=(const MemBuffer& buf);
    ~MemBuffer();
    bool WriteMem(void* addr, size_t count);
    int8  Get8 (size_t offset) const;
    bool  Set8 (size_t offset, int8 val);
    int16 Get16(size_t offset) const;
    bool  Get16(size_t offset, int16 *val) const;
    int32 Get32(size_t offset) const;
    bool  Get32(size_t offset, int32 *val) const;
    int16 GetInv16(size_t offset) const {return ntst_bytes_swap16(Get16(offset));}
    bool  GetInv16(size_t offset, int16 *val) const;
    int32 GetInv32(size_t offset) const {return ntst_bytes_swap32(Get32(offset));}
    bool  GetInv32(size_t offset, int32 *val) const;
    bool  GetSubBuffer(size_t offset, size_t length, MemBuffer &buf) const;
    bool  Set32(size_t offset, int32 val);
    bool  SetInv32(size_t offset, int32 val){return Set32(offset, ntst_bytes_swap32(val));}
    bool empty() const {return GetSize() == 0;}
    size_t GetSize() const {return usize;}
    size_t GetReserved() const {return bsize;}
    void* GetAddress() const {return baddr;}
    void* GetAddress(size_t offset) const {return (void*)((uintptr_t)baddr + offset);}
    void Delete(size_t offset, size_t count);
    void Insert(size_t offset, size_t count, void* addr);
    bool Replace(size_t offset, size_t count, void* addr);
    void Assign(void* addr, size_t size);
    void SetName(const std::string& n) {name = n;}
    const std::string& GetName() const {return name;}

    bool SaveInFile(const std::string& filename);
    bool LoadFile(const std::string& filename);

    bool operator==(const MemBuffer& buf) {return (usize == buf.usize) && (memcmp(baddr, buf.baddr, usize) == 0);}

    bool operator!=(const MemBuffer& buf) {return !(*this == buf);}

    void Clear();
    bool Resize(size_t newSize);
protected:
    //void LoadFile(FILE *f);
    void Init(){bsize = usize = 0; baddr = NULL;};
    bool Realloc(size_t newSize);
    std::string name;
    void* baddr;//адрес буфера
    size_t bsize;//размер буфера
    size_t usize;//используемая часть
    };

#endif // MEMBUFFER_H
