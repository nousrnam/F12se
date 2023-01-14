/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include <stdlib.h>
#include <stdio.h>
#include "Defines.h"
#include "FileFinder.h"
#include "misc.h"
#include "WinFunc.h"
#include "zlib/zlib.h"
#include "lzss.h"
#include "ntst_stringpp.hpp"
#include "ntst_loggingpp.hpp"

void uncompressF1(uint8 *in, uint32 inSize, uint8 *out)
    {
    int16 n;
    uint32 inOffset, outOffset;

    inOffset = outOffset = 0;

    n = *(int16*)in;
    n = ntst_bytes_swap16(n);
    inOffset += 2;
    while (n != 0 && inOffset < inSize)
        {
        if (n > 0)
            {
            outOffset += decompress_lzss(&out[outOffset], &in[inOffset], n);
            }
        else
            {
            n = n * -1;
            memcpy((void*)&out[outOffset], (void*)&in[inOffset], n);
            outOffset += n;
            }
        inOffset += n;
        n = *(int16*)&in[inOffset];
        n = ntst_bytes_swap16(n);
        inOffset += 2;
        }
    }

class FilesSource
    {
public:
    virtual ~FilesSource() {;}
    static FilesSource* CreateFilesSource(const std::string& path, bool isFallout1DatFile);
    std::string GetPath() const {return m_path;}
    virtual bool FindFile(const std::string& file) = 0;
    virtual MemBuffer GetFoundFile() = 0;
    bool operator==(const FilesSource& fs) const {return m_path == fs.m_path;}
protected:
    FilesSource(const std::string& path) : m_path(path) {;}
    std::string m_path; //Путь к месту поиска
    };

class FilesSourceDirectory : public FilesSource
    {
public:
    FilesSourceDirectory(const std::string& path);
protected:
    bool FindFile(const std::string& file);
    MemBuffer GetFoundFile();
    std::string curFilePath;
    };

class FilesSourceDatFile : public FilesSource
    {
public:
    bool FindFile(const std::string& file);
    MemBuffer GetFoundFile();
    bool IsValid(){return !files.empty();}
protected:
    FilesSourceDatFile(const std::string& _path) : FilesSource(_path), curFileRec(NULL)  {;}
    virtual void ScanFile() = 0;
    virtual MemBuffer GetFileFromDat(FileRec *fileRec) = 0;
    std::vector<FileRec> files; //Массив со списком файлов внутри .dat файла
    FileRec *curFileRec;
    };

class FilesSourceF1DatFile : public FilesSourceDatFile
    {
public:
    FilesSourceF1DatFile(const std::string& _path) : FilesSourceDatFile(_path) {ScanFile();}
protected:
    void ScanFile();
    MemBuffer GetFileFromDat(FileRec *fileRec);
    };

class FilesSourceF2DatFile : public FilesSourceDatFile
    {
public:
    FilesSourceF2DatFile(const std::string& _path) : FilesSourceDatFile(_path) {ScanFile();}
protected:
    void ScanFile();
    MemBuffer GetFileFromDat(FileRec *fileRec);
    };

FilesSource* FilesSource::CreateFilesSource(const std::string& path, bool isFallout1DatFile)
    {
    FilesSource *source;
    FilesSourceDatFile *datfile;
    if (FileExists(path))
        {
        if (isFallout1DatFile)
            {
            datfile = new FilesSourceF1DatFile(path);
            if (datfile->IsValid())
                source = datfile;
            else
                {
                delete datfile;
                source = NULL;
                ntst_log_info(path + " is ignored, because it is invalid Fallout1 .dat file.");
                }
            }
        else
            {
            datfile = new FilesSourceF2DatFile(path);
            if (datfile->IsValid())
                source = datfile;
            else
                {
                delete datfile;
                source = NULL;
                ntst_log_info(path + " is ignored, because it is invalid Fallout2 .dat file.");
                }
            }
        }
    else if (DirectoryExists(path))
        source = new FilesSourceDirectory(path);
    else
        {
        ntst_log_info(path + " is not valid source of files.");
        source = NULL;
        }
    return source;
    }

FilesSourceDirectory::FilesSourceDirectory(const std::string& path) : FilesSource(path)
    {
    TrimTrailingSlashes(m_path);
    }

bool FilesSourceDirectory::FindFile(const std::string& file)
    {
    std::string fullpath = m_path + DIR_SEPARATOR + file;
    if (FileExists(fullpath))
        {
        curFilePath = fullpath;
        return true;
        }
    curFilePath.clear();
    return false;
    }

MemBuffer FilesSourceDirectory::GetFoundFile()
    {
    if (curFilePath.empty())
        {
        ntst_log_warning("Logical error in FilesSourceDirectory::GetFoundFile: get not found file.");
        return MemBuffer("not found file in directory " + GetPath());
        }
    return MemBuffer(curFilePath, curFilePath);
    }

bool SearchBinary(const std::vector<FileRec>& files, const std::string& target, size_t *result)
    {
    const size_t size = files.size();
    size_t first = 0;
    size_t last = size;
    while (first < last)
        {
        size_t mid = first + (last - first) / 2;
        if (target <= files[mid].name)
            last = mid;
        else
            first = mid + 1;
        }
    if (last < size && files[last].name == target)
        {
        *result = last;
        return true;
        }
   return false;
   }

bool FilesSourceDatFile::FindFile(const std::string& file)
    {
    size_t pos;
    if (SearchBinary(files, file, &pos))
        {
        curFileRec = &files[pos];
        return true;
        }
    curFileRec = NULL;
    return false;
    }

MemBuffer FilesSourceDatFile::GetFoundFile()
    {
    if (curFileRec == NULL)
        {
        ntst_log_warning("Logical error: get not found file.");
        return MemBuffer("not found file in file " + GetPath());
        }
    return GetFileFromDat(curFileRec);
    }

void FileFinder::AddSource(const std::string &path)
    {
    FilesSource* src = FilesSource::CreateFilesSource(path, isFallout1DatFile);
    if (src != NULL)
        {
        for (std::vector<FilesSource*>::iterator it=locList.begin(); it!=locList.end(); ++it)
            {
            if (*src == **it)
                {
                ntst_log_info("Skip already present source of files: " + path);
                delete src;
                return;
                }
            }
        ntst_log_info("Added new source of files: " + path);
        locList.push_back(src);
        }
    }

void FileFinder::ClearSources()
    {
    if (locList.empty())
        return;
    for (std::vector<FilesSource*>::iterator it=locList.begin(); it!=locList.end(); ++it)
        delete *it;
    locList.clear();
    ntst_log_info("All sources of files cleared.");
    }

MemBuffer FileFinder::GetFile(const std::string &filename)
    {
    if (FindFile(filename))
        return GetFoundFile();
    ntst_log_warning("GetFile function failed to find file: " + filename);
    return MemBuffer("not found file: " + filename);
    }

void FileFinder::LogLocations() const
    {
    std::string buf;
    for (std::vector<FilesSource*>::const_iterator it = locList.begin(); it != locList.end(); ++it)
        {
        if (!buf.empty())
            buf += "\n";
        buf += (*it)->GetPath();
        }
    if (!buf.empty())
        ntst_log_info("Search order: " + buf);
    }

MemBuffer FileFinder::GetFoundFile()
    {
    if (curFileSrc != NULL)
        return curFileSrc->GetFoundFile();
    ntst_log_warning("Logical error in GetFoundFile(): get not found file.");
    return MemBuffer("not found file");
    }

bool FileFinder::FindFile(const std::string &filename)
    {
    std::string file = StrToLow(filename);
    for (std::vector<FilesSource*>::iterator it = locList.begin(); it != locList.end(); ++it)
        {
        if ((*it)->FindFile(file))
            {
            curFileSrc = *it;
            ntst_log_info("Found file " + filename + " in " + (*it)->GetPath());
            return true;
            }
        }
    curFileSrc = NULL;
    //ntst_log_info("Requsted file " + filename + " wasn't found.");
    return false;
    }

bool ReadFileOrDirName(FILE *f, std::string& name, unsigned namesize)
    {
    if (namesize > 1000)
        {
        ntst_log_error("File or directory name length > 1000. Something is very wrong.");
        return false;
        }
#if defined __GNUC__
    char* buf = (char*)__builtin_alloca(namesize);
#elif defined _MSC_VER
    char* buf = (char*)_alloca(namesize);
#else
    char buf[namesize];
#endif
    if (fread(buf, namesize, 1, f) != 1)
        return false;
    name.assign(buf, namesize);
    StrToLowInplace(name);
    return true;
    }

void FilesSourceF1DatFile::ScanFile()
    {
    uint32 i, j, k,
           dirCount,
           fileCount, fileCountTotal,
           fileRealSize,
           filePackedSize,
           fileOffset,
           fileStart,
           dirStart,
           attributes;
    uint8  nameLength;
    std::string filename, fullName, prevFilename;
    std::vector<std::string> dirNames;

    FILE *f = fopen(GetPath().c_str(), "rb");
    if (f == NULL)
        {
        ntst_log_error("F1DatFile::ScanFile() failed to open file: " + GetPath() + ErrnoToStringEnd());
        return;
        }

    long int filesize = GetFileSize(f);
    if (filesize <= 40)
        {
        fclose(f);
        ntst_log_error(GetPath() + " is too small, therefore ignored.");
        return;
        }

    fseek(f, 0, SEEK_SET);
    fread(&dirCount, 4, 1, f);
    int32 mustBeNotZero;
    if (fread(&mustBeNotZero, 4, 1, f) != 1 || mustBeNotZero == 0)
        {
        fclose(f);
        ntst_log_error(GetPath() + " is not valid Fallout 1 .dat file.");
        return;
        }
    int32 mustBeZero;
    if (fread(&mustBeZero, 4, 1, f) != 1 || mustBeZero != 0)
        {
        fclose(f);
        ntst_log_error(GetPath() + " is not valid Fallout 1 .dat file.");
        return;
        }
    dirCount = ntst_bytes_swap32(dirCount);
    dirNames.resize(dirCount);

    fseek(f, 16, SEEK_SET);
    for (i = 0; i < dirCount; i++)
        {
        fread(&nameLength,1,1,f);
        ReadFileOrDirName(f, dirNames[i], nameLength);
        }

    //для того чтобы узнать общее число файлов придется написать длинный медленный код.
    //жаль что я не придумал ничего получше.
    fileCountTotal = 0;
    dirStart = ftell(f);
    for (i = 0; i < dirCount; i++)
        {
        fread(&fileCount, 4, 1, f);
        fileCount = ntst_bytes_swap32(fileCount);
        fileCountTotal += fileCount;
        if (fseek(f, 12, SEEK_CUR) != 0)
            {
            ntst_log_error(GetPath() + " is not valid Fallout 1 .dat file or something strange happened." + ErrnoToStringEnd());
            fclose(f);
            return;
            }
        for (j = 0; j < fileCount; j++)
            {
            fread(&nameLength,1,1,f);
            fseek(f, nameLength + 16, SEEK_CUR);
            }
        }

    files.resize(fileCountTotal);

    k = 0;
    fseek(f, dirStart, SEEK_SET);
    for (i = 0; i < dirCount; i++)
        {
        fread(&fileCount, 4, 1, f);
        fileCount = ntst_bytes_swap32(fileCount);
        fseek(f, 12, SEEK_CUR);
        for (j = 0; j < fileCount; j++)
            {
            fileStart = ftell(f);
            fread(&nameLength,1,1,f);
            ReadFileOrDirName(f, filename, nameLength);
            //для файлов в корне, папка обозначается точкой. Во втором фоле такого нет. Там просто имя файла.
            if (dirNames[i] != ".")
                fullName = dirNames[i] + '\\' + filename;
            else
                fullName = filename;
            fread(&attributes,4,1,f);
            fread(&fileOffset,4,1,f);
            fread(&fileRealSize,4,1,f);
            fread(&filePackedSize,4,1,f);
            attributes = ntst_bytes_swap32(attributes);
            fileOffset = ntst_bytes_swap32(fileOffset);
            fileRealSize = ntst_bytes_swap32(fileRealSize);
            filePackedSize = ntst_bytes_swap32(filePackedSize);
            if (attributes != 0x20 && attributes != 0x40)
                ntst_log_warning(fullName + " have strange attributes(" + ntst::to_string_hex_pref(attributes) + ") normally should be 0x20 or 0x40.");
            // In vanilla version some files aren't sorted.
            /*if (prevFilename > fullName)
                ntst_log_warning("In " + GetPath() + " files aren't sorted alphabetically in a descendent direction. " + fullName + " is after " + prevFilename);*/
            prevFilename = fullName;

            //запомним положение этого файла
            files[k].name = fullName;
            files[k].offset = fileStart;
            files[k].size = fileRealSize;
            k++;
            }
        }
    fclose(f);
    // Almost all files are sorted. So insertion sort is fine.
    FileRec tmpElement;
    for (i = 1; i < fileCountTotal; i++)
        {
        int location = i;
        tmpElement = files[location];
        while (location >= 1 && files[location - 1].name > tmpElement.name)
           {
           files[location] = files[location - 1];
           location--;
           }
        files[location] = tmpElement;
        }
    }

void FilesSourceF2DatFile::ScanFile()
    {
    uint32 treeSize,
        dataSize,
        dirTreeAddr,
        filesQuantity,
        filenameSize,
        fileRealSize,
        filePackedSize,
        fileOffset;
    uint8 isCompressed;
    std::string filename,
        prevFilename;

    FILE *f = fopen(GetPath().c_str(), "rb");
    if (f == NULL)
        {
        ntst_log_error("F2DatFile::ScanFile() failed to open file: " + GetPath() + ErrnoToStringEnd());
        return;
        }

    ntst_log_info("Reading Fallout 2 .dat file: " + GetPath());

    uint32 filesize = GetFileSize(f);
    if (filesize <= 12)
        {
        ntst_log_error(GetPath() + " is too small, therefore ignored.");
        return;
        }

    const char* freadFailed = "fread(...) in F2DatFile::ScanFile() failed.";
    if (fseek(f, -8, SEEK_END) != 0)
        {
        ntst_log_error("fseek(f, -8, SEEK_END) in F2DatFile::ScanFile() failed." + ErrnoToStringEnd());
        goto QuitScanF2;
        }
    if (fread(&treeSize, 4, 1, f) != 1)
        {
        ntst_log_error(freadFailed);
        goto QuitScanF2;
        }
    if (fread(&dataSize, 4, 1, f) != 1)
        {
        ntst_log_error(freadFailed);
        goto QuitScanF2;
        }
    if (filesize != dataSize)
        {
        ntst_log_error(GetPath() + " is not valid Fallout 2 .dat file.");
        goto QuitScanF2;
        }
    dirTreeAddr = dataSize - treeSize - 4;
    if (fseek(f, dirTreeAddr - 4, SEEK_SET) != 0)
        {
        ntst_log_error("fseek(f, dirTreeAddr - 4, SEEK_SET) in F2DatFile::ScanFile() failed." + ErrnoToStringEnd());
        goto QuitScanF2;
        }
    if (fread(&filesQuantity, 4, 1, f) != 1)
        {
        ntst_log_error(freadFailed);
        goto QuitScanF2;
        }

    files.resize(filesQuantity);
    for (uint32 i = 0; i < filesQuantity; i++)
        {
        uint32 fileStart = ftell(f);
        if (fread(&filenameSize, 4, 1, f) != 1)
            {
            ntst_log_error(freadFailed);
            goto QuitScanF2;
            }

        if (!ReadFileOrDirName(f, filename, filenameSize))
            {
            ntst_log_error(freadFailed);
            goto QuitScanF2;
            }

        if (fread(&isCompressed, 1, 1, f) != 1)
            {
            ntst_log_error(freadFailed);
            goto QuitScanF2;
            }
        if (fread(&fileRealSize, 4, 1, f) != 1)
            {
            ntst_log_error(freadFailed);
            goto QuitScanF2;
            }
        if (fread(&filePackedSize, 4, 1, f) != 1)
            {
            ntst_log_error(freadFailed);
            goto QuitScanF2;
            }
        if (fread(&fileOffset, 4, 1, f) != 1)
            {
            ntst_log_error(freadFailed);
            goto QuitScanF2;
            }

        if ((uint)isCompressed > 1)
            ntst_log_info("In " + GetPath() + " compression flag(" + ntst::to_string(isCompressed) + ") != 0 && != 1 for file " + filename);
        if (isCompressed == 0 && fileRealSize != filePackedSize)
            ntst_log_info("In " + GetPath() + " fileRealSize != filePackedSize for not compressed file " + filename);
        if (prevFilename > filename)
            ntst_log_error("In " + GetPath() + " files aren't sorted alphabetically in a descendent direction. " + filename + " is after " + prevFilename);
        prevFilename = filename;

        files[i].name = filename;
        files[i].offset = fileStart;
        files[i].size = fileRealSize;
        }
QuitScanF2:
    fclose(f);
    }

MemBuffer FilesSourceF1DatFile::GetFileFromDat(FileRec *fileRec)
    {
    uint32 attributes,fileOffset,filePackedSize;
    uint8 filenameSize;
    uLongf fileRealSize;
    void* unpackedfile;
    void* packedfile;
    FILE* f;
    std::string filename;
    MemBuffer result(fileRec->name);

    f = fopen(GetPath().c_str(), "rb");
    if (f == NULL)
        {
        ntst_log_error("GetFileFromDat(F1) failed to open file: " + GetPath() + ErrnoToStringEnd());
        goto QuitGetFFDF1;
        }

    fseek(f, fileRec->offset, SEEK_SET);

    fread(&filenameSize, 1, 1, f);

    fseek(f, fileRec->offset + filenameSize + 1, SEEK_SET);
    fread(&attributes, 4, 1, f);
    fread(&fileOffset, 4, 1, f);
    fread(&fileRealSize, 4, 1, f);
    if (fread(&filePackedSize, 4, 1, f) != 1)
        {
        ntst_log_error("GetFileFromDat(F1) failed, can't read file info block for " + fileRec->name);
        goto QuitGetFFDF1;
        }

    attributes = ntst_bytes_swap32(attributes);
    fileOffset = ntst_bytes_swap32(fileOffset);
    fileRealSize = ntst_bytes_swap32(fileRealSize);
    filePackedSize = ntst_bytes_swap32(filePackedSize);

    fseek(f, fileOffset, SEEK_SET);

    unpackedfile = malloc(fileRealSize);
    if (unpackedfile == NULL)
        {
        ntst_log_error("GetFileFromDat(F1) failed, can't allocate " + ntst::to_string(fileRealSize) + " bytes for unpacked file " + fileRec->name);
        goto QuitGetFFDF1;
        }
    if (attributes == 0x40)
        {
        packedfile = malloc(filePackedSize);
        if (packedfile == NULL)
            {
            ntst_log_error("GetFileFromDat(F1) failed, can't allocate " + ntst::to_string(filePackedSize) + " bytes for packed file " + fileRec->name);
            free(unpackedfile);
            goto QuitGetFFDF1;
            }
        if (fread(packedfile, filePackedSize, 1, f) == 1)
            uncompressF1((uint8*)packedfile, filePackedSize, (uint8*)unpackedfile);
        else
            {
            ntst_log_error("GetFileFromDat(F1) failed, can't read file content " + fileRec->name);
            free(unpackedfile);
            free(packedfile);
            goto QuitGetFFDF1;
            }
        free(packedfile);
        }
    else
        {
        if (fread(unpackedfile, fileRealSize, 1, f) != 1)
            {
            ntst_log_error("GetFileFromDat(F1) failed, can't read file content " + fileRec->name);
            free(unpackedfile);
            goto QuitGetFFDF1;
            }
        }
    result.Assign(unpackedfile, fileRealSize);
QuitGetFFDF1:
    fclose(f);
    return result;
    }

MemBuffer FilesSourceF2DatFile::GetFileFromDat(FileRec *fileRec)
    {
    uint32 filenameSize, filePackedSize, fOffset;
    uLongf fileRealSize;
    int8 isCompressed;
    std::string filename;
    MemBuffer result(fileRec->name);
    void* addr;

    FILE *f = fopen(GetPath().c_str(), "rb");
    if (f == NULL)
        {
        ntst_log_error("GetFileFromDat(F2) failed to open file: " + GetPath() + ErrnoToStringEnd());
        return result;
        }
    if (fseek(f, fileRec->offset, SEEK_SET) != 0)
        {
        ntst_log_error("GetFileFromDat(F2) failed to fseek(f, fileRec->offset, SEEK_SET)" + ErrnoToStringEnd());
        goto QuitGetFFDF2;
        }

    if (fread(&filenameSize, 4, 1, f) != 1)
        {
        ntst_log_error("GetFileFromDat(F2) failed to fread(&filenameSize, 4, 1, f)");
        goto QuitGetFFDF2;
        }

    if (fseek(f, fileRec->offset + filenameSize + 4, SEEK_SET) != 0)
        {
        ntst_log_error("GetFileFromDat(F2) failed to fseek(f, fileRec->offset + filenameSize + 4, SEEK_SET)" + ErrnoToStringEnd());
        goto QuitGetFFDF2;
        }
    if (fread(&isCompressed, 1, 1, f) != 1)
        {
        ntst_log_error("GetFileFromDat(F2) failed to fread(&isCompressed, 1, 1, f)");
        goto QuitGetFFDF2;
        }
    if (fread(&fileRealSize, 4, 1, f) != 1)
        {
        ntst_log_error("GetFileFromDat(F2) failed to fread(&fileRealSize, 4, 1, f)");
        goto QuitGetFFDF2;
        }
    if (fread(&filePackedSize, 4, 1, f) != 1)
        {
        ntst_log_error("GetFileFromDat(F2) failed to fread(&filePackedSize, 4, 1, f)");
        goto QuitGetFFDF2;
        }
    if (fread(&fOffset, 4, 1, f) != 1)
        {
        ntst_log_error("GetFileFromDat(F2) failed to fread(&fOffset, 4, 1, f)");
        goto QuitGetFFDF2;
        }

    if (fseek(f, fOffset, SEEK_SET) != 0)
        {
        ntst_log_error("GetFileFromDat(F2) failed to fseek(f, fOffset, SEEK_SET)" + ErrnoToStringEnd());
        goto QuitGetFFDF2;
        }
    addr = malloc(fileRealSize);
    if (addr == NULL)
        {
        ntst_log_error("GetFileFromDat(F2) failed, can't allocate " + ntst::to_string(fileRealSize) + " bytes for unpacked file " + fileRec->name);
        goto QuitGetFFDF2;
        }

    if (isCompressed)
        {
        void* packedfile = malloc(filePackedSize);
        if (packedfile == NULL)
            {
            ntst_log_error("GetFileFromDat(F2) failed, can't allocate " + ntst::to_string(filePackedSize) + " bytes for packed file " + fileRec->name);
            free(addr);
            goto QuitGetFFDF2;
            }
        if (fread(packedfile, filePackedSize, 1, f) != 1)
            {
            ntst_log_error("GetFileFromDat(F2) failed to fread(packedfile, filePackedSize, 1, f)");
            free(packedfile);
            free(addr);
            goto QuitGetFFDF2;
            }
        if (uncompress((Bytef*)addr, &fileRealSize, (Bytef*)packedfile, filePackedSize) != Z_OK)
            ntst_log_error("GetFileFromDat(F2) failed to uncompress data.");;
        free(packedfile);
        }
    else
        {
        if (fread(addr, fileRealSize, 1, f) != 1)
            {
            ntst_log_error("GetFileFromDat(F2) failed to fread(addr, fileRealSize, 1, f)");
            free(addr);
            goto QuitGetFFDF2;
            }
        }
    result.Assign(addr, fileRealSize);
QuitGetFFDF2:
    fclose(f);
    return result;
    }

