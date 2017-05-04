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

#include "fs/files.h"

#include "fs/mkdir.h"
#if defined(ANDROID) || defined(__native_client__)
#include "fs/paths.h"

#include "fs/virtfs/virtfs.h"
#include "fs/virtfs/virtfstools.h"
#include "fs/virtfs/virtlist.h"
#endif  // defined(ANDROID) || defined(__native_client__)

#include "utils/checkutils.h"
#include "utils/stringutils.h"

#include <dirent.h>
#include <sys/stat.h>

#include "debug.h"

extern const char *dirSeparator;

#ifdef ANDROID
void Files::extractLocale()
{
    // in future need also remove all locales in local dir

    const std::string fileName2 = pathJoin(getenv("APPDIR"), "locale.zip");
    VirtFs::mountZip(fileName2, Append_false);

    const std::string localDir = std::string(getenv("APPDIR"));
    VirtList *const rootDirs = VirtFs::enumerateFiles("locale");
    FOR_EACH (StringVectCIter, i, rootDirs->names)
    {
        const std::string dir = pathJoin("locale", *i);
        if (VirtFs::isDirectory(dir))
        {
            const std::string moFile = dir + "/LC_MESSAGES/manaplus.mo";
            if (VirtFs::exists((moFile)))
            {
                const std::string localFile = pathJoin(localDir, moFile);
                const std::string localDir2 = pathJoin(localDir,
                    dir,
                    "LC_MESSAGES");
                mkdir_r(localDir2.c_str());
                copyVirtFsFile(moFile, localFile);
            }
        }
    }
    VirtFs::freeList(rootDirs);
    VirtFs::unmountZip(fileName2);
    remove(fileName2.c_str());
}
#endif  // ANDROID

#if defined(ANDROID) || defined(__native_client__)

namespace
{
#ifdef ANDROID
    int mFilesCount = 0;
#endif  // ANDROID

    Files::CopyFileCallbackPtr mCallbackPtr = nullptr;
}  // namespace

void Files::setCopyCallBack(Files::CopyFileCallbackPtr callback)
{
    mCallbackPtr = callback;
}

void Files::copyVirtFsFile(const std::string &restrict inFile,
                           const std::string &restrict outFile)
{
    int size = 0;
    const char *const buf = VirtFs::loadFile(inFile, size);
    FILE *const file = fopen(outFile.c_str(), "w");
    fwrite(buf, 1, size, file);
    fclose(file);
    delete [] buf;
#ifdef ANDROID
    if (mCallbackPtr)
    {
        mCallbackPtr(mFilesCount);
        mFilesCount ++;
    }
#endif  // ANDROID
}

void Files::copyVirtFsDir(const std::string &restrict inDir,
                          const std::string &restrict outDir)
{
    mkdir_r(outDir.c_str());
    VirtList *const files = VirtFs::enumerateFiles(inDir);
    FOR_EACH (StringVectCIter, i, files->names)
    {
        const std::string file = pathJoin(inDir, *i);
        const std::string outDir2 = pathJoin(outDir, *i);
        if (VirtFs::isDirectory(file))
            copyVirtFsDir(file, outDir2);
        else
            copyVirtFsFile(file, outDir2);
    }
    VirtFs::freeList(files);
}

void Files::extractZip(const std::string &restrict zipName,
                       const std::string &restrict inDir,
                       const std::string &restrict outDir)
{
    VirtFs::mountZip(zipName, Append_false);
    copyVirtFsDir(inDir, outDir);
    VirtFs::unmountZip(zipName);
    remove(zipName.c_str());
}

#endif  // ANDROID __native_client__

int Files::renameFile(const std::string &restrict srcName,
                      const std::string &restrict dstName)
{
#if defined __native_client__
    FILE *srcFile = fopen(srcName.c_str(), "rb");
    if (srcFile == nullptr)
        return -1;
    FILE *dstFile = fopen(dstName.c_str(), "w+b");
    if (dstFile == nullptr)
    {
        fclose(srcFile);
        return -1;
    }

    const int chunkSize = 500000;
    char *buf = new char[chunkSize];
    size_t sz = 0;
    while ((sz = fread(buf, 1, chunkSize, srcFile)))
    {
        if (fwrite(buf, 1, sz, dstFile) != sz)
        {
            delete [] buf;
            fclose(srcFile);
            fclose(dstFile);
            ::remove(dstName.c_str());
            return -1;
        }
    }

    delete [] buf;
    fclose(srcFile);
    fclose(dstFile);
    if (!::remove(srcName.c_str()))
        return 0;

    return -1;
#else  // defined __native_client__

    return ::rename(srcName.c_str(), dstName.c_str());
#endif  // defined __native_client__
}

int Files::copyFile(const std::string &restrict srcName,
                    const std::string &restrict dstName)
{
    FILE *srcFile = fopen(srcName.c_str(), "rb");
    if (srcFile == nullptr)
        return -1;
    FILE *dstFile = fopen(dstName.c_str(), "w+b");
    if (dstFile == nullptr)
    {
        fclose(srcFile);
        return -1;
    }

    const int chunkSize = 512000;
    char *buf = new char[chunkSize];
    size_t sz = 0;
    while ((sz = fread(buf, 1, chunkSize, srcFile)))
    {
        if (fwrite(buf, 1, sz, dstFile) != sz)
        {
            delete [] buf;
            fclose(srcFile);
            fclose(dstFile);
            return -1;
        }
    }

    delete [] buf;
    fclose(srcFile);
    fclose(dstFile);
    return 0;
}

bool Files::existsLocal(const std::string &path)
{
    struct stat statbuf;
#ifdef WIN32
    // in windows path\file.ext\ by default detected as exists
    // if file.ext is not directory, need return false
    const bool res = (stat(path.c_str(), &statbuf) == 0);
    if (res == false)
        return false;
    if ((findLast(path, "/") == true || findLast(path, "\\") == true) &&
        S_ISDIR(statbuf.st_mode) == 0)
    {
        return false;
    }
    return true;
#else  // WIN32
    return stat(path.c_str(), &statbuf) == 0;
#endif  // WIN32
}

bool Files::loadTextFileLocal(const std::string &fileName,
                              StringVect &lines)
{
    std::ifstream file;
    char line[501];

    file.open(fileName.c_str(), std::ios::in);

    if (!file.is_open())
    {
        reportAlways("Couldn't load text file: %s",
            fileName.c_str());
        return false;
    }

    while (file.getline(line, 500))
        lines.push_back(line);

    return true;
}

void Files::saveTextFile(const std::string &path,
                         const std::string &restrict name,
                         const std::string &restrict text)
{
    if (!mkdir_r(path.c_str()))
    {
        std::ofstream file;
        std::string fileName = pathJoin(path, name);
        file.open(fileName.c_str(), std::ios::out);
        if (file.is_open())
        {
            file << text << std::endl;
        }
        else
        {
            reportAlways("Error opening file for writing: %s",
                fileName.c_str());
        }
        file.close();
    }
}

void Files::deleteFilesInDirectory(std::string path)
{
    path == pathJoin(path, dirSeparator);
    const struct dirent *next_file = nullptr;
    DIR *const dir = opendir(path.c_str());

    if (dir)
    {
        while ((next_file = readdir(dir)))
        {
            const std::string file = next_file->d_name;
            if (file != "." && file != "..")
                remove((path + file).c_str());
        }
        closedir(dir);
    }
}

void Files::enumFiles(StringVect &files,
                      std::string path,
                      const bool skipSymlinks A_WIN_UNUSED)
{
    if (findLast(path, dirSeparator) == false)
        path += dirSeparator;
    const struct dirent *next_file = nullptr;
    DIR *const dir = opendir(path.c_str());

    if (dir)
    {
        while ((next_file = readdir(dir)))
        {
            const std::string file = next_file->d_name;
            if (file == "." || file == "..")
                continue;
#ifndef WIN32
            if (skipSymlinks == true)
            {
                struct stat statbuf;
                if (lstat(path.c_str(), &statbuf) == 0 &&
                    S_ISLNK(statbuf.st_mode) != 0)
                {
                    continue;
                }
            }
#endif  // WIN32
            files.push_back(file);
        }
        closedir(dir);
    }
}

