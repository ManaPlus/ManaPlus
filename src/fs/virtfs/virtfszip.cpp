/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2017  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "fs/virtfs/virtfszip.h"

#include "fs/virtfs/virtfile.h"
#include "fs/virtfs/virtfs.h"
#include "fs/virtfs/virtfsfuncs.h"
#include "fs/virtfs/virtfsziprwops.h"
#include "fs/virtfs/virtlist.h"
#include "fs/virtfs/virtzipentry.h"
#include "fs/virtfs/zip.h"
#include "fs/virtfs/ziplocalheader.h"

#include "utils/checkutils.h"
#include "utils/stringutils.h"

#include "debug.h"

extern const char *dirSeparator;

namespace
{
    VirtFsFuncs funcs;
}  // namespace

namespace VirtFsZip
{
    VirtFsFuncs *getFuncs()
    {
        return &funcs;
    }

    void deinit()
    {
    }

    void init()
    {
        initFuncs(&funcs);
    }

    void initFuncs(VirtFsFuncs *restrict const ptr)
    {
        ptr->close = &VirtFsZip::close;
        ptr->read = &VirtFsZip::read;
        ptr->write = &VirtFsZip::write;
        ptr->fileLength = &VirtFsZip::fileLength;
        ptr->tell = &VirtFsZip::tell;
        ptr->seek = &VirtFsZip::seek;
        ptr->eof = &VirtFsZip::eof;
        ptr->exists = &VirtFsZip::exists;
        ptr->getRealDir = &VirtFsZip::getRealDir;
        ptr->enumerate = &VirtFsZip::enumerate;
        ptr->isDirectory = &VirtFsZip::isDirectory;
        ptr->openRead = &VirtFsZip::openRead;
        ptr->openWrite = &VirtFsZip::openWrite;
        ptr->openAppend = &VirtFsZip::openAppend;
        ptr->loadFile = &VirtFsZip::loadFile;
        ptr->getFiles = &VirtFsZip::getFiles;
        ptr->getDirs = &VirtFsZip::getDirs;
        ptr->rwops_seek = &VirtFsZip::rwops_seek;
        ptr->rwops_read = &VirtFsZip::rwops_read;
        ptr->rwops_write = &VirtFsZip::rwops_write;
        ptr->rwops_close = &VirtFsZip::rwops_close;
#ifdef USE_SDL2
        ptr->rwops_size = &VirtFsZip::rwops_size;
#endif  // USE_SDL2
    }

    bool getRealDir(VirtFsEntry *restrict const entry,
                    const std::string &filename,
                    const std::string &dirName,
                    std::string &realDir)
    {
        VirtZipEntry *const zipEntry = static_cast<VirtZipEntry*>(entry);
        FOR_EACH (std::vector<ZipLocalHeader*>::const_iterator,
                  it2,
                  zipEntry->mHeaders)
        {
            if ((*it2)->fileName == filename)
            {
                realDir = entry->root;
                return true;
            }
        }
        FOR_EACH (std::vector<std::string>::const_iterator,
                  it2,
                  zipEntry->mDirs)
        {
            if (*it2 == dirName)
            {
                realDir = entry->root;
                return true;
            }
        }
        return false;
    }

    bool exists(VirtFsEntry *restrict const entry,
                const std::string &filename,
                const std::string &dirName)
    {
        VirtZipEntry *const zipEntry = static_cast<VirtZipEntry*>(entry);
        FOR_EACH (std::vector<ZipLocalHeader*>::const_iterator,
                  it2,
                  zipEntry->mHeaders)
        {
            if ((*it2)->fileName == filename)
                return true;
        }
        FOR_EACH (std::vector<std::string>::const_iterator,
                  it2,
                  zipEntry->mDirs)
        {
            if (*it2 == dirName)
                return true;
        }
        return false;
    }

    void enumerate(VirtFsEntry *restrict const entry,
                   const std::string &dirName,
                   StringVect &names)
    {
        VirtZipEntry *const zipEntry = static_cast<VirtZipEntry*>(entry);
        if (dirName == dirSeparator)
        {
            FOR_EACH (std::vector<ZipLocalHeader*>::const_iterator,
                      it2,
                      zipEntry->mHeaders)
            {
                ZipLocalHeader *const header = *it2;
                std::string fileName = header->fileName;
                // skip subdirs from enumeration
                const size_t idx = fileName.find(dirSeparator);
                if (idx != std::string::npos)
                    fileName.erase(idx);
                bool found(false);
                FOR_EACH (StringVectCIter, itn, names)
                {
                    if (*itn == fileName)
                    {
                        found = true;
                        break;
                    }
                }
                if (found == false)
                    names.push_back(fileName);
            }
        }
        else
        {
            FOR_EACH (std::vector<ZipLocalHeader*>::const_iterator,
                      it2,
                      zipEntry->mHeaders)
            {
                ZipLocalHeader *const header = *it2;
                std::string fileName = header->fileName;
                if (findCutFirst(fileName, dirName) == true)
                {
                    // skip subdirs from enumeration
                    const size_t idx = fileName.find(dirSeparator);
                    if (idx != std::string::npos)
                        fileName.erase(idx);
                    bool found(false);
                    FOR_EACH (StringVectCIter, itn, names)
                    {
                        if (*itn == fileName)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (found == false)
                        names.push_back(fileName);
                }
            }
        }
    }

    void getFiles(VirtFsEntry *restrict const entry,
                  const std::string &dirName,
                  StringVect &names)
    {
        VirtZipEntry *const zipEntry = static_cast<VirtZipEntry*>(entry);
        if (dirName == dirSeparator)
        {
            FOR_EACH (std::vector<ZipLocalHeader*>::const_iterator,
                      it2,
                      zipEntry->mHeaders)
            {
                ZipLocalHeader *const header = *it2;
                std::string fileName = header->fileName;
                // skip subdirs from enumeration
                const size_t idx = fileName.find(dirSeparator);
                if (idx != std::string::npos)
                    fileName.erase(idx);
                bool found(false);
                FOR_EACH (StringVectCIter, itn, names)
                {
                    if (*itn == fileName)
                    {
                        found = true;
                        break;
                    }
                }
                if (found == false)
                {
                    std::string dirName2 = pathJoin(dirName, fileName);
                    if (findLast(dirName2, std::string(dirSeparator)) == false)
                        dirName2 += dirSeparator;
                    FOR_EACH (std::vector<std::string>::const_iterator,
                              it,
                              zipEntry->mDirs)
                    {
                        if (*it == dirName2)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (found == false)
                        names.push_back(fileName);
                }
            }
        }
        else
        {
            FOR_EACH (std::vector<ZipLocalHeader*>::const_iterator,
                      it2,
                      zipEntry->mHeaders)
            {
                ZipLocalHeader *const header = *it2;
                std::string fileName = header->fileName;
                if (findCutFirst(fileName, dirName) == true)
                {
                    // skip subdirs from enumeration
                    const size_t idx = fileName.find(dirSeparator);
                    if (idx != std::string::npos)
                        fileName.erase(idx);
                    bool found(false);
                    FOR_EACH (StringVectCIter, itn, names)
                    {
                        if (*itn == fileName)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (found == false)
                    {
                        std::string dirName2 = pathJoin(dirName, fileName);
                        if (findLast(dirName2, std::string(dirSeparator)) == false)
                            dirName2 += dirSeparator;
                        FOR_EACH (std::vector<std::string>::const_iterator,
                                  it,
                                  zipEntry->mDirs)
                        {
                            if (*it == dirName2)
                            {
                                found = true;
                                break;
                            }
                        }
                        if (found == false)
                            names.push_back(fileName);
                    }
                }
            }
        }
    }

    void getDirs(VirtFsEntry *restrict const entry,
                 const std::string &dirName,
                 StringVect &names)
    {
        VirtZipEntry *const zipEntry = static_cast<VirtZipEntry*>(entry);
        if (dirName == dirSeparator)
        {
            FOR_EACH (std::vector<ZipLocalHeader*>::const_iterator,
                      it2,
                      zipEntry->mHeaders)
            {
                ZipLocalHeader *const header = *it2;
                std::string fileName = header->fileName;
                // skip subdirs from enumeration
                const size_t idx = fileName.find(dirSeparator);
                if (idx != std::string::npos)
                    fileName.erase(idx);
                bool found(false);
                FOR_EACH (StringVectCIter, itn, names)
                {
                    if (*itn == fileName)
                    {
                        found = true;
                        break;
                    }
                }
                if (found == false)
                {
                    std::string dirName2 = pathJoin(dirName, fileName);
                    if (findLast(dirName2, std::string(dirSeparator)) == false)
                        dirName2 += dirSeparator;
                    FOR_EACH (std::vector<std::string>::const_iterator,
                              it,
                              zipEntry->mDirs)
                    {
                        if (*it == dirName2)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (found == true)
                        names.push_back(fileName);
                }
            }
        }
        else
        {
            FOR_EACH (std::vector<ZipLocalHeader*>::const_iterator,
                      it2,
                      zipEntry->mHeaders)
            {
                ZipLocalHeader *const header = *it2;
                std::string fileName = header->fileName;
                if (findCutFirst(fileName, dirName) == true)
                {
                    // skip subdirs from enumeration
                    const size_t idx = fileName.find(dirSeparator);
                    if (idx != std::string::npos)
                        fileName.erase(idx);
                    bool found(false);
                    FOR_EACH (StringVectCIter, itn, names)
                    {
                        if (*itn == fileName)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (found == false)
                    {
                        std::string dirName2 = pathJoin(dirName, fileName);
                        if (findLast(dirName2, std::string(dirSeparator)) == false)
                            dirName2 += dirSeparator;
                        FOR_EACH (std::vector<std::string>::const_iterator,
                                  it,
                                  zipEntry->mDirs)
                        {
                            if (*it == dirName2)
                            {
                                found = true;
                                break;
                            }
                        }
                        if (found == true)
                            names.push_back(fileName);
                    }
                }
            }
        }
    }

    bool isDirectory(VirtFsEntry *restrict const entry,
                     const std::string &dirName,
                     bool &isDirFlag)
    {
        VirtZipEntry *const zipEntry = static_cast<VirtZipEntry*>(entry);
        FOR_EACH (std::vector<std::string>::const_iterator,
                  it2,
                  zipEntry->mDirs)
        {
            if (*it2 == dirName)
            {
                isDirFlag = true;
                return true;
            }
        }
        return false;
    }

    void freeList(VirtList *restrict const handle)
    {
        delete handle;
    }

    VirtFile *openRead(VirtFsEntry *restrict const entry,
                       const std::string &filename)
    {
        VirtZipEntry *const zipEntry = static_cast<VirtZipEntry*>(entry);
        FOR_EACH (std::vector<ZipLocalHeader*>::const_iterator,
                  it2,
                  zipEntry->mHeaders)
        {
            const ZipLocalHeader *restrict const header = *it2;
            if (header->fileName == filename)
            {
                const uint8_t *restrict const buf = Zip::readFile(header);
                if (buf == nullptr)
                    return nullptr;
                VirtFile *restrict const file = new VirtFile(&funcs,
                    buf,
                    header->uncompressSize);
                return file;
            }
        }
        return nullptr;
    }

    VirtFile *openWrite(VirtFsEntry *restrict const entry A_UNUSED,
                        const std::string &filename A_UNUSED)
    {
        reportAlways("VirtFs::openWrite for zip not implemented.");
        return nullptr;
    }

    VirtFile *openAppend(VirtFsEntry *restrict const entry A_UNUSED,
                         const std::string &filename A_UNUSED)
    {
        reportAlways("VirtFs::openAppend for zip not implemented.");
        return nullptr;
    }

    int close(VirtFile *restrict const file)
    {
        if (file == nullptr)
            return 0;
        delete file;
        return 1;
    }

    int64_t read(VirtFile *restrict const file,
                 void *restrict const buffer,
                 const uint32_t objSize,
                 const uint32_t objCount)
    {
        if (file == nullptr ||
            objSize == 0 ||
            objCount == 0)
        {
            return 0;
        }
        if (buffer == nullptr)
        {
            reportAlways("VirtFsZip::read buffer is null");
            return 0;
        }
        const uint32_t pos = CAST_S32(file->mPos);
        const uint32_t sz = CAST_S32(file->mSize);
        // if outside of buffer, return
        if (pos >= sz)
            return 0;
        // pointer to start for buffer ready to read
        const uint8_t *restrict const memPtr = file->mBuf + pos;
        // left buffer size from pos to end
        const uint32_t memSize = sz - pos;
        // number of objects possible to read
        uint32_t memCount = memSize / objSize;
        if (memCount == 0)
            return 0;
        // limit number of possible objects to read to objCount
        if (memCount > objCount)
            memCount = objCount;
        // number of bytes to read from buffer
        const uint32_t memEnd = memCount * objSize;
        memcpy(buffer, memPtr, memEnd);
        file->mPos += memEnd;
        return memCount;
    }

    int64_t write(VirtFile *restrict const file A_UNUSED,
                  const void *restrict const buffer A_UNUSED,
                  const uint32_t objSize A_UNUSED,
                  const uint32_t objCount A_UNUSED)
    {
        return 0;
    }

    int64_t fileLength(VirtFile *restrict const file)
    {
        if (file == nullptr)
            return -1;

        return file->mSize;
    }

    int64_t tell(VirtFile *restrict const file)
    {
        if (file == nullptr)
            return -1;

        return file->mPos;
    }

    int seek(VirtFile *restrict const file,
             const uint64_t pos)
    {
        if (file == nullptr)
            return 0;

        if (pos > file->mSize)
            return 0;
        file->mPos = pos;
        return 1;
    }

    int eof(VirtFile *restrict const file)
    {
        if (file == nullptr)
            return -1;

        return file->mPos >= file->mSize;
    }

    const char *loadFile(VirtFsEntry *restrict const entry,
                         const std::string &restrict filename,
                         int &restrict fileSize)
    {
        VirtZipEntry *const zipEntry = static_cast<VirtZipEntry*>(entry);
        FOR_EACH (std::vector<ZipLocalHeader*>::const_iterator,
                  it2,
                  zipEntry->mHeaders)
        {
            const ZipLocalHeader *restrict const header = *it2;
            if (header->fileName == filename)
            {
                const uint8_t *restrict const buf = Zip::readFile(header);
                if (buf == nullptr)
                    return nullptr;

                logger->log("Loaded %s/%s",
                    entry->root.c_str(),
                    filename.c_str());

                fileSize = header->uncompressSize;
                return reinterpret_cast<const char*>(buf);
            }
        }
        return nullptr;
    }
}  // namespace VirtFsZip
