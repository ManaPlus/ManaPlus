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

#include "fs/virtfszip.h"

#include "fs/files.h"
#include "fs/paths.h"
#include "fs/virtfile.h"
#include "fs/virtfileprivate.h"
#include "fs/virtlist.h"
#include "fs/virtzipentry.h"
#include "fs/zip.h"
#include "fs/ziplocalheader.h"

#include "utils/checkutils.h"
#include "utils/dtor.h"
#include "utils/stringutils.h"

#include "debug.h"

extern const char *dirSeparator;

namespace
{
    std::vector<VirtZipEntry*> mEntries;
}  // namespace

namespace VirtFsZip
{
    VirtZipEntry *searchEntryByArchive(const std::string &restrict archiveName)
    {
        FOR_EACH (std::vector<VirtZipEntry*>::const_iterator, it, mEntries)
        {
            if ((*it)->mArchiveName == archiveName)
                return *it;
        }
        return nullptr;
    }

    ZipLocalHeader *searchHeaderByName(const std::string &restrict filename)
    {
        FOR_EACH (std::vector<VirtZipEntry*>::const_iterator, it, mEntries)
        {
            VirtZipEntry *const entry = *it;
            FOR_EACH (std::vector<ZipLocalHeader*>::const_iterator,
                      it2,
                      entry->mHeaders)
            {
                if ((*it2)->fileName == filename)
                    return *it2;;
            }
        }
        return nullptr;
    }

    bool addToSearchPathSilent(const std::string &newDir,
                               const Append append)
    {
        if (Files::existsLocal(newDir) == false)
        {
            logger->log("VirtFsZip::addToSearchPath file not exists: %s",
                newDir.c_str());
            return false;
        }
        if (findLast(newDir, ".zip") == false)
        {
            reportAlways("Called VirtFsZip::addToSearchPath without zip archive");
            return false;
        }
        VirtZipEntry *entry = VirtFsZip::searchEntryByArchive(newDir);
        if (entry != nullptr)
        {
            reportAlways("VirtFsZip::addToSearchPath already exists: %s",
                newDir.c_str());
            return false;
        }
        entry = new VirtZipEntry(newDir);
        if (Zip::readArchiveInfo(entry) == false)
        {
            delete entry;
            return false;
        }

        logger->log("Add virtual zip: " + newDir);
        if (append == Append_true)
            mEntries.push_back(entry);
        else
        {
            mEntries.insert(mEntries.begin(),
                entry);
        }
        return true;
    }

    bool addToSearchPath(const std::string &newDir,
                         const Append append)
    {
        if (Files::existsLocal(newDir) == false)
        {
            reportAlways("VirtFsZip::addToSearchPath directory not exists: %s",
                newDir.c_str());
            return false;
        }
        if (findLast(newDir, ".zip") == false)
        {
            reportAlways("Called VirtFsZip::addToSearchPath without zip archive");
            return false;
        }
        VirtZipEntry *entry = VirtFsZip::searchEntryByArchive(newDir);
        if (entry != nullptr)
        {
            reportAlways("VirtFsZip::addToSearchPath already exists: %s",
                newDir.c_str());
            return false;
        }
        entry = new VirtZipEntry(newDir);
        if (Zip::readArchiveInfo(entry) == false)
        {
            delete entry;
            return false;
        }

        logger->log("Add virtual zip: " + newDir);
        if (append == Append_true)
            mEntries.push_back(entry);
        else
        {
            mEntries.insert(mEntries.begin(),
                entry);
        }
        return true;
    }

    bool removeFromSearchPathSilent(std::string oldDir)
    {
        if (findLast(oldDir, ".zip") == false)
        {
            reportAlways("Called removeFromSearchPath without zip archive");
            return false;
        }
        FOR_EACH (std::vector<VirtZipEntry*>::iterator, it, mEntries)
        {
            VirtZipEntry *const entry = *it;
            if (entry->mArchiveName == oldDir)
            {
                logger->log("Remove virtual zip: " + oldDir);
                mEntries.erase(it);
                delete entry;
                return true;
            }
        }

        logger->log("VirtFsZip::removeFromSearchPath not exists: %s",
            oldDir.c_str());
        return false;
    }

    bool removeFromSearchPath(std::string oldDir)
    {
        if (findLast(oldDir, ".zip") == false)
        {
            reportAlways("Called removeFromSearchPath without zip archive");
            return false;
        }
        FOR_EACH (std::vector<VirtZipEntry*>::iterator, it, mEntries)
        {
            VirtZipEntry *const entry = *it;
            if (entry->mArchiveName == oldDir)
            {
                logger->log("Remove virtual zip: " + oldDir);
                mEntries.erase(it);
                delete entry;
                return true;
            }
        }

        reportAlways("VirtFsZip::removeFromSearchPath not exists: %s",
            oldDir.c_str());
        return false;
    }

    std::vector<VirtZipEntry*> &getEntries()
    {
        return mEntries;
    }

    void deinit()
    {
        delete_all(mEntries);
        mEntries.clear();
    }

    std::string getRealDir(const std::string &restrict filename)
    {
        if (checkPath(filename) == false)
        {
            reportAlways("VirtFsZip::exists invalid path: %s",
                filename.c_str());
            return std::string();
        }
        ZipLocalHeader *restrict const header = searchHeaderByName(filename);
        if (header != nullptr)
            return header->zipEntry->mArchiveName;
        return std::string();
    }

    bool exists(const std::string &restrict name)
    {
        if (checkPath(name) == false)
        {
            reportAlways("VirtFsZip::exists invalid path: %s",
                name.c_str());
            return false;
        }
        ZipLocalHeader *restrict const header = searchHeaderByName(name);
        if (header != nullptr)
            return true;
        return false;
    }

    VirtList *enumerateFiles(std::string dirName)
    {
        VirtList *const list = new VirtList;
        if (checkPath(dirName) == false)
        {
            reportAlways("VirtFsZip::enumerateFiles invalid path: %s",
                dirName.c_str());
            return list;
        }
        if (findLast(dirName, std::string(dirSeparator)) == false)
            dirName += dirSeparator;
        StringVect &names = list->names;
        FOR_EACH (std::vector<VirtZipEntry*>::const_iterator, it, mEntries)
        {
            VirtZipEntry *const entry = *it;
            FOR_EACH (std::vector<ZipLocalHeader*>::const_iterator,
                      it2,
                      entry->mHeaders)
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

        return list;
    }

    bool isDirectory(std::string dirName)
    {
        if (checkPath(dirName) == false)
        {
            reportAlways("VirtFsZip::isDirectory invalid path: %s",
                dirName.c_str());
            return false;
        }
        if (findLast(dirName, std::string(dirSeparator)) == false)
            dirName += dirSeparator;
        FOR_EACH (std::vector<VirtZipEntry*>::const_iterator, it, mEntries)
        {
            VirtZipEntry *const entry = *it;
            FOR_EACH (std::vector<std::string>::const_iterator,
                      it2,
                      entry->mDirs)
            {
                if (*it2 == dirName)
                    return true;
            }
        }
        return false;
    }

    bool isSymbolicLink(const std::string &restrict name)
    {
        if (checkPath(name) == false)
        {
            reportAlways("VirtFsZip::isSymbolicLink invalid path: %s",
                name.c_str());
            return false;
        }
        // look like in zip files can be symlinks, but here they useless
        return false;
    }

    void freeList(VirtList *restrict const handle)
    {
        delete handle;
    }

    VirtFile *openRead(const std::string &restrict filename)
    {
        if (checkPath(filename) == false)
        {
            reportAlways("VirtFsZip::openRead invalid path: %s",
                filename.c_str());
            return nullptr;
        }
        ZipLocalHeader *restrict const header = searchHeaderByName(filename);
        if (header != nullptr)
        {
            uint8_t *restrict const buf = Zip::readFile(header);
            if (buf == nullptr)
                return nullptr;
            VirtFile *restrict const file = new VirtFile;
            file->mPrivate = new VirtFilePrivate(buf,
                header->uncompressSize);
            return file;
        }
        return nullptr;
    }

    VirtFile *openWrite(const std::string &restrict filename A_UNUSED)
    {
        return nullptr;
    }

    VirtFile *openAppend(const std::string &restrict filename A_UNUSED)
    {
        return nullptr;
    }

    bool setWriteDir(const std::string &restrict newDir A_UNUSED)
    {
        return false;
    }

    bool mkdir(const std::string &restrict dirname A_UNUSED)
    {
        return false;
    }

    bool remove(const std::string &restrict filename A_UNUSED)
    {
        return false;
    }

    void permitLinks(const bool val A_UNUSED)
    {
    }

    const char *getLastError()
    {
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
        VirtFilePrivate *restrict const priv = file->mPrivate;
        const uint32_t pos = priv->mPos;
        const uint32_t sz = priv->mSize;
        // if outside of buffer, return
        if (pos >= sz)
            return 0;
        // pointer to start for buffer ready to read
        const uint8_t *restrict const memPtr = priv->mBuf + pos;
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
        priv->mPos += memEnd;
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

        return file->mPrivate->mSize;
    }

    int64_t tell(VirtFile *restrict const file)
    {
        if (file == nullptr)
            return -1;

        return file->mPrivate->mPos;
    }

    int seek(VirtFile *restrict const file,
             const uint64_t pos)
    {
        if (file == nullptr)
            return 0;

        if (pos > file->mPrivate->mSize)
            return 0;
        file->mPrivate->mPos = pos;
        return 1;
    }

    int eof(VirtFile *restrict const file)
    {
        if (file == nullptr)
            return -1;

        return file->mPrivate->mPos >= file->mPrivate->mSize;
    }
}  // namespace VirtFsZip
