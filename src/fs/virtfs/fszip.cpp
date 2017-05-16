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

#include "fs/virtfs/fszip.h"

#include "fs/virtfs/file.h"
#include "fs/virtfs/fsfuncs.h"
#include "fs/virtfs/fsziprwops.h"
#include "fs/virtfs/list.h"
#include "fs/virtfs/zipentry.h"
#include "fs/virtfs/zipreader.h"
#include "fs/virtfs/ziplocalheader.h"

#include "utils/checkutils.h"
#include "utils/stringutils.h"

#include "debug.h"

extern const char *dirSeparator;

namespace
{
    VirtFs::FsFuncs funcs;
}  // namespace

namespace VirtFs
{

namespace FsZip
{
    FsFuncs *getFuncs()
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

    void initFuncs(FsFuncs *restrict const ptr)
    {
        ptr->close = &FsZip::close;
        ptr->read = &FsZip::read;
        ptr->write = &FsZip::write;
        ptr->fileLength = &FsZip::fileLength;
        ptr->tell = &FsZip::tell;
        ptr->seek = &FsZip::seek;
        ptr->eof = &FsZip::eof;
        ptr->exists = &FsZip::exists;
        ptr->getRealDir = &FsZip::getRealDir;
        ptr->enumerate = &FsZip::enumerate;
        ptr->isDirectory = &FsZip::isDirectory;
        ptr->openRead = &FsZip::openRead;
        ptr->openWrite = &FsZip::openWrite;
        ptr->openAppend = &FsZip::openAppend;
        ptr->loadFile = &FsZip::loadFile;
        ptr->getFiles = &FsZip::getFiles;
        ptr->getFilesWithDir = &FsZip::getFilesWithDir;
        ptr->getDirs = &FsZip::getDirs;
        ptr->rwops_seek = &FsZip::rwops_seek;
        ptr->rwops_read = &FsZip::rwops_read;
        ptr->rwops_write = &FsZip::rwops_write;
        ptr->rwops_close = &FsZip::rwops_close;
#ifdef USE_SDL2
        ptr->rwops_size = &FsZip::rwops_size;
#endif  // USE_SDL2
    }

    bool getRealDir(FsEntry *restrict const entry,
                    std::string filename,
                    std::string dirName,
                    std::string &realDir)
    {
        ZipEntry *const zipEntry = static_cast<ZipEntry*>(entry);
        std::string subDir = zipEntry->subDir;
        if (subDir != dirSeparator)
        {
            filename = pathJoin(subDir, filename);
            dirName = pathJoin(subDir, dirName);
        }
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

    bool exists(FsEntry *restrict const entry,
                std::string filename,
                std::string dirName)
    {
        ZipEntry *const zipEntry = static_cast<ZipEntry*>(entry);
        std::string subDir = zipEntry->subDir;
        if (subDir != dirSeparator)
        {
            filename = pathJoin(subDir, filename);
            dirName = pathJoin(subDir, dirName);
        }
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

    void enumerate(FsEntry *restrict const entry,
                   std::string dirName,
                   StringVect &names)
    {
        ZipEntry *const zipEntry = static_cast<ZipEntry*>(entry);
        std::string subDir = zipEntry->subDir;
        if (subDir != dirSeparator)
            dirName = pathJoin(subDir, dirName);
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

    void getFiles(FsEntry *restrict const entry,
                  const std::string &dirName,
                  StringVect &names)
    {
        ZipEntry *const zipEntry = static_cast<ZipEntry*>(entry);
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
                        if (findLast(dirName2, std::string(dirSeparator)) ==
                            false)
                        {
                            dirName2 += dirSeparator;
                        }
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

    void getFilesWithDir(FsEntry *restrict const entry,
                         const std::string &dirName,
                         StringVect &names)
    {
        ZipEntry *const zipEntry = static_cast<ZipEntry*>(entry);
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
                        names.push_back(pathJoin(dirName, fileName));
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
                        if (findLast(dirName2, std::string(dirSeparator)) ==
                            false)
                        {
                            dirName2 += dirSeparator;
                        }
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
                            names.push_back(pathJoin(dirName, fileName));
                    }
                }
            }
        }
    }

    void getDirs(FsEntry *restrict const entry,
                 const std::string &dirName,
                 StringVect &names)
    {
        ZipEntry *const zipEntry = static_cast<ZipEntry*>(entry);
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
                        if (findLast(dirName2, std::string(dirSeparator)) ==
                            false)
                        {
                            dirName2 += dirSeparator;
                        }
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

    bool isDirectory(FsEntry *restrict const entry,
                     const std::string &dirName,
                     bool &isDirFlag)
    {
        ZipEntry *const zipEntry = static_cast<ZipEntry*>(entry);
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

    void freeList(List *restrict const handle)
    {
        delete handle;
    }

    File *openRead(FsEntry *restrict const entry,
                   const std::string &filename)
    {
        ZipEntry *const zipEntry = static_cast<ZipEntry*>(entry);
        FOR_EACH (std::vector<ZipLocalHeader*>::const_iterator,
                  it2,
                  zipEntry->mHeaders)
        {
            const ZipLocalHeader *restrict const header = *it2;
            if (header->fileName == filename)
            {
                const uint8_t *restrict const buf =
                    ZipReader::readFile(header);
                if (buf == nullptr)
                    return nullptr;
                File *restrict const file = new File(&funcs,
                    buf,
                    header->uncompressSize);
                return file;
            }
        }
        return nullptr;
    }

    File *openWrite(FsEntry *restrict const entry A_UNUSED,
                    const std::string &filename A_UNUSED)
    {
        reportAlways("VirtFs::openWrite for zip not implemented.");
        return nullptr;
    }

    File *openAppend(FsEntry *restrict const entry A_UNUSED,
                     const std::string &filename A_UNUSED)
    {
        reportAlways("VirtFs::openAppend for zip not implemented.");
        return nullptr;
    }

    int close(File *restrict const file)
    {
        if (file == nullptr)
            return 0;
        delete file;
        return 1;
    }

    int64_t read(File *restrict const file,
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
            reportAlways("FsZip::read buffer is null");
            return 0;
        }
        const size_t pos = file->mPos;
        const size_t sz = file->mSize;
        // if outside of buffer, return
        if (pos >= sz)
            return 0;
        // pointer to start for buffer ready to read
        const uint8_t *restrict const memPtr = file->mBuf + pos;
        // left buffer size from pos to end
        const uint32_t memSize = CAST_U32(sz - pos);
        // number of objects possible to read
        uint32_t memCount = memSize / objSize;
        if (memCount == 0)
            return 0;
        // limit number of possible objects to read to objCount
        if (memCount > objCount)
            memCount = objCount;
        // number of bytes to read from buffer
        const size_t memEnd = memCount * objSize;
        memcpy(buffer, memPtr, memEnd);
        file->mPos += memEnd;
        return memCount;
    }

    int64_t write(File *restrict const file A_UNUSED,
                  const void *restrict const buffer A_UNUSED,
                  const uint32_t objSize A_UNUSED,
                  const uint32_t objCount A_UNUSED)
    {
        return 0;
    }

    int64_t fileLength(File *restrict const file)
    {
        if (file == nullptr)
            return -1;

        return file->mSize;
    }

    int64_t tell(File *restrict const file)
    {
        if (file == nullptr)
            return -1;

        return file->mPos;
    }

    int seek(File *restrict const file,
             const uint64_t pos)
    {
        if (file == nullptr)
            return 0;

        if (pos > file->mSize)
            return 0;
        file->mPos = pos;
        return 1;
    }

    int eof(File *restrict const file)
    {
        if (file == nullptr)
            return -1;

        return file->mPos >= file->mSize;
    }

    const char *loadFile(FsEntry *restrict const entry,
                         const std::string &restrict filename,
                         int &restrict fileSize)
    {
        ZipEntry *const zipEntry = static_cast<ZipEntry*>(entry);
        FOR_EACH (std::vector<ZipLocalHeader*>::const_iterator,
                  it2,
                  zipEntry->mHeaders)
        {
            const ZipLocalHeader *restrict const header = *it2;
            if (header->fileName == filename)
            {
                const uint8_t *restrict const buf =
                    ZipReader::readFile(header);
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
}  // namespace FsZip

}  // namespace VirtFs
