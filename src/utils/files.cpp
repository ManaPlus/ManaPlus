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

#include "utils/files.h"

#include "logger.h"

#if defined(ANDROID) || defined(__native_client__)
#include "resources/resourcemanager/resourcemanager.h"

#include "utils/mkdir.h"
#endif  // defined(ANDROID) || defined(__native_client__)

#include "utils/mkdir.h"
#include "utils/paths.h"
#include "utils/virtfs.h"
#include "utils/stringutils.h"

#include <algorithm>
#include <dirent.h>
#include <sstream>

#include "debug.h"

#ifdef ANDROID
void Files::extractLocale()
{
    // in future need also remove all locales in local dir

    const std::string fileName2 = std::string(getenv(
        "APPDIR")).append("/locale.zip");
    VirtFs::addZipToSearchPath(fileName2, Append_false);

    const std::string localDir = std::string(getenv("APPDIR")).append("/");
    char **rootDirs = VirtFs::enumerateFiles("locale");
    for (char **i = rootDirs; *i; i++)
    {
        const std::string dir = std::string("locale/").append(*i);
        if (VirtFs::isDirectory(dir.c_str()))
        {
            const std::string moFile = dir + "/LC_MESSAGES/manaplus.mo";
            if (VirtFs::exists((moFile).c_str()))
            {
                const std::string localFile = localDir + moFile;
                const std::string localDir2 = localDir + dir + "/LC_MESSAGES";
                mkdir_r(localDir2.c_str());
                copyPhysFsFile(moFile, localFile);
            }
        }
    }
    VirtFs::freeList(rootDirs);
    VirtFs::removeZipFromSearchPath(fileName2);
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

void Files::copyPhysFsFile(const std::string &restrict inFile,
                           const std::string &restrict outFile)
{
    int size = 0;
    void *const buf = VirtFs::loadFile(inFile, size);
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
#endif  // ANDROID
}

void Files::copyPhysFsDir(const std::string &restrict inDir,
                          const std::string &restrict outDir)
{
    mkdir_r(outDir.c_str());
    char **files = VirtFs::enumerateFiles(inDir.c_str());
    for (char **i = files; *i; i++)
    {
        const std::string file = std::string(inDir).append("/").append(*i);
        const std::string outDir2 = std::string(outDir).append("/").append(*i);
        if (VirtFs::isDirectory(file.c_str()))
            copyPhysFsDir(file, outDir2);
        else
            copyPhysFsFile(file, outDir2);
    }
    VirtFs::freeList(files);
}

void Files::extractZip(const std::string &restrict zipName,
                       const std::string &restrict inDir,
                       const std::string &restrict outDir)
{
    VirtFs::addZipToSearchPath(zipName, Append_false);
    copyPhysFsDir(inDir, outDir);
    VirtFs::removeZipFromSearchPath(zipName);
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
            return -1;
        }
    }

    delete [] buf;
    fclose(srcFile);
    fclose(dstFile);
    return 0;
}

void Files::getFiles(const std::string &path, StringVect &list)
{
    char **const fonts = VirtFs::enumerateFiles(path.c_str());
    for (char *const *i = fonts; *i; i++)
    {
        if (!VirtFs::isDirectory((path + *i).c_str()))
            list.push_back(*i);
    }
    VirtFs::freeList(fonts);
}

void Files::getDirs(const std::string &path, StringVect &list)
{
    char **const fonts = VirtFs::enumerateFiles(path.c_str());
    for (char *const *i = fonts; *i; i++)
    {
        if (VirtFs::isDirectory((path + *i).c_str()))
            list.push_back(*i);
    }
    VirtFs::freeList(fonts);
}

void Files::getFilesWithDir(const std::string &path, StringVect &list)
{
    char **const fonts = VirtFs::enumerateFiles(path.c_str());
    for (char *const *i = fonts; *i; i++)
    {
        if (!VirtFs::isDirectory((path + *i).c_str()))
            list.push_back(path + *i);
    }
    VirtFs::freeList(fonts);
}

bool Files::existsLocal(const std::string &path)
{
    bool flg(false);
    std::fstream file;
    file.open(path.c_str(), std::ios::in);
    if (file.is_open())
        flg = true;
    file.close();
    return flg;
}

std::string Files::getPath(const std::string &file)
{
    // get the real path to the file
    const char *const tmp = VirtFs::getRealDir(file.c_str());
    std::string path;

    // if the file is not in the search path, then its nullptr
    if (tmp)
    {
        path = std::string(tmp).append(dirSeparator).append(file);
#if defined __native_client__
        std::string dataZip = "/http/data.zip/";
        if (path.substr(0, dataZip.length()) == dataZip)
            path = path.replace(0, dataZip.length(), "/http/data/");
#endif  // defined __native_client__
    }
    else
    {
        // if not found in search path return the default path
        path = getPackageDir().append(dirSeparator).append(file);
    }

    return path;
}

std::string Files::loadTextFileString(const std::string &fileName)
{
    int contentsLength;
    char *fileContents = static_cast<char*>(
        VirtFs::loadFile(fileName, contentsLength));

    if (!fileContents)
    {
        logger->log("Couldn't load text file: %s", fileName.c_str());
        return std::string();
    }
    const std::string str = std::string(fileContents, contentsLength);
    free(fileContents);
    return str;
}

bool Files::loadTextFile(const std::string &fileName,
                         StringVect &lines)
{
    int contentsLength;
    char *fileContents = static_cast<char*>(
        VirtFs::loadFile(fileName, contentsLength));

    if (!fileContents)
    {
        logger->log("Couldn't load text file: %s", fileName.c_str());
        return false;
    }

    std::istringstream iss(std::string(fileContents, contentsLength));
    std::string line;

    while (getline(iss, line))
        lines.push_back(line);

    free(fileContents);
    return true;
}

bool Files::loadTextFileLocal(const std::string &fileName,
                              StringVect &lines)
{
    std::ifstream file;
    char line[501];

    file.open(fileName.c_str(), std::ios::in);

    if (!file.is_open())
    {
        logger->log("Couldn't load text file: %s", fileName.c_str());
        return false;
    }

    while (file.getline(line, 500))
        lines.push_back(line);

    return true;
}

void Files::saveTextFile(std::string path,
                         const std::string &restrict name,
                         const std::string &restrict text)
{
    if (!mkdir_r(path.c_str()))
    {
        std::ofstream file;
        file.open((path.append("/").append(name)).c_str(), std::ios::out);
        if (file.is_open())
            file << text << std::endl;
        file.close();
    }
}

void Files::deleteFilesInDirectory(std::string path)
{
    path += "/";
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

void Files::getFilesInDir(const std::string &dir,
                          StringVect &list,
                          const std::string &ext)
{
    const std::string path = dir + "/";
    StringVect tempList;
    Files::getFilesWithDir(path, tempList);
    FOR_EACH (StringVectCIter, it, tempList)
    {
        const std::string &str = *it;
        if (findLast(str, ext))
            list.push_back(str);
    }
    std::sort(list.begin(), list.end());
}
