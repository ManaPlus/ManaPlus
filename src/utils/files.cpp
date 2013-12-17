/*
 *  The ManaPlus Client
 *  Copyright (C) 2013  The ManaPlus Developers
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

#include "utils/files.h"

#if defined(ANDROID) || defined(__native_client__)
#include "resources/resourcemanager.h"
#include "utils/physfstools.h"
#endif

#include "utils/mkdir.h"

#include "localconsts.h"

#ifdef ANDROID
void Files::extractLocale()
{
    // in future need also remove all locales in local dir

    const std::string fileName2 = std::string(getenv(
        "APPDIR")).append("/locale.zip");
    const ResourceManager *const resman = ResourceManager::getInstance();
    resman->addToSearchPath(fileName2, false);

    const std::string localDir = std::string(getenv("APPDIR")).append("/");
    char **rootDirs = PhysFs::enumerateFiles("locale");
    for (char **i = rootDirs; *i; i++)
    {
        const std::string dir = std::string("locale/").append(*i);
        if (PhysFs::isDirectory(dir.c_str()))
        {
            const std::string moFile = dir + "/LC_MESSAGES/manaplus.mo";
            if (PhysFs::exists((moFile).c_str()))
            {
                const std::string localFile = localDir + moFile;
                const std::string localDir2 = localDir + dir + "/LC_MESSAGES";
                mkdir_r(localDir2.c_str());
                copyPhysFsFile(moFile, localFile);
            }
        }
    }
    PhysFs::freeList(rootDirs);
    resman->removeFromSearchPath(fileName2);
    remove(fileName2.c_str());
}
#endif // ANDROID

#if defined(ANDROID) || defined(__native_client__)

namespace
{
    int mFilesCount = 0;
    Files::CopyFileCallbackPtr mCallbackPtr = nullptr;
}  // namespace

void Files::setCopyCallBack(Files::CopyFileCallbackPtr callback)
{
    mCallbackPtr = callback;
}

void Files::copyPhysFsFile(const std::string &restrict inFile,
                           const std::string &restrict outFile)
{
    int size = 0;
    void *const buf = ResourceManager::loadFile(inFile, size);
    FILE *const file = fopen(outFile.c_str(), "w");
    fwrite(buf, 1, size, file);
    fclose(file);
    free(buf);
#ifdef ANDROID
    if (mCallbackPtr)
    {
        mCallbackPtr(mFilesCount);
        mFilesCount ++;
    }
#endif
}

void Files::copyPhysFsDir(const std::string &restrict inDir,
                          const std::string &restrict outDir)
{
    mkdir_r(outDir.c_str());
    char **files = PhysFs::enumerateFiles(inDir.c_str());
    for (char **i = files; *i; i++)
    {
        const std::string file = std::string(inDir).append("/").append(*i);
        const std::string outDir2 = std::string(outDir).append("/").append(*i);
        if (PhysFs::isDirectory(file.c_str()))
            copyPhysFsDir(file, outDir2);
        else
            copyPhysFsFile(file, outDir2);
    }
    PhysFs::freeList(files);
}

void Files::extractZip(const std::string &restrict zipName,
                       const std::string &restrict inDir,
                       const std::string &restrict outDir)
{
    const ResourceManager *const resman = ResourceManager::getInstance();
    resman->addToSearchPath(zipName, false);
    copyPhysFsDir(inDir, outDir);
    resman->removeFromSearchPath(zipName);
    remove(zipName.c_str());
}

#endif  // ANDROID __native_client__

int Files::renameFile(const std::string &restrict srcName,
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
}
