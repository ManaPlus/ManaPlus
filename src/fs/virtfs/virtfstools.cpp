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

#include "fs/virtfs/virtfstools.h"

#include "logger.h"

#include "fs/paths.h"

#include "fs/virtfs/virtfs.h"
#include "fs/virtfs/virtlist.h"

#include "utils/stringutils.h"

#include <algorithm>
#include <sstream>

#include "debug.h"

namespace VirtFs
{
    void searchAndAddArchives(const std::string &restrict path,
                              const std::string &restrict ext,
                              const Append append)
    {
        VirtList *const list = VirtFs::enumerateFiles(path);
        FOR_EACH (StringVectCIter, i, list->names)
        {
            const std::string str = *i;
            const size_t len = str.size();

            if (len > ext.length() &&
                !ext.compare(str.substr(len - ext.length())))
            {
                const std::string file = path + str;
                const std::string realPath = VirtFs::getRealDir(file);
                VirtFs::mountZip(pathJoin(realPath, file), append);
            }
        }
        VirtFs::freeList(list);
    }

    void searchAndRemoveArchives(const std::string &restrict path,
                                 const std::string &restrict ext)
    {
        VirtList *const list = VirtFs::enumerateFiles(path);
        FOR_EACH (StringVectCIter, i, list->names)
        {
            const std::string str = *i;
            const size_t len = str.size();
            if (len > ext.length() &&
                !ext.compare(str.substr(len - ext.length())))
            {
                const std::string file = path + str;
                const std::string realPath = VirtFs::getRealDir(file);
                VirtFs::unmountZip(pathJoin(realPath, file));
            }
        }
        VirtFs::freeList(list);
    }

    void getFilesWithDir(const std::string &path,
                         StringVect &list)
    {
        VirtList *const fonts = VirtFs::enumerateFiles(path);
        FOR_EACH (StringVectCIter, i, fonts->names)
        {
            if (!VirtFs::isDirectory(path + *i))
                list.push_back(path + *i);
        }
        VirtFs::freeList(fonts);
    }

    void getFilesInDir(const std::string &dir,
                       StringVect &list,
                       const std::string &ext)
    {
        const std::string path = dir + "/";
        StringVect tempList;
        VirtFs::getFilesWithDir(path, tempList);
        FOR_EACH (StringVectCIter, it, tempList)
        {
            const std::string &str = *it;
            if (findLast(str, ext))
                list.push_back(str);
        }
        std::sort(list.begin(), list.end());
    }

    void getFiles(const std::string &path,
                  StringVect &list)
    {
        VirtList *const fonts = VirtFs::enumerateFiles(path);
        FOR_EACH (StringVectCIter, i, fonts->names)
        {
            if (!VirtFs::isDirectory(pathJoin(path, *i)))
                list.push_back(*i);
        }
        VirtFs::freeList(fonts);
    }

    void getDirs(const std::string &path, StringVect &list)
    {
        VirtList *const fonts = VirtFs::enumerateFiles(path);
        FOR_EACH (StringVectCIter, i, fonts->names)
        {
            if (VirtFs::isDirectory(pathJoin(path, *i)))
                list.push_back(*i);
        }
        VirtFs::freeList(fonts);
    }

    std::string getPath(const std::string &file)
    {
        // get the real path to the file
        const std::string tmp = VirtFs::getRealDir(file);
        std::string path;

        // if the file is not in the search path, then its empty
        if (!tmp.empty())
        {
            path = pathJoin(tmp, file);
#if defined __native_client__
            std::string dataZip = "/http/data.zip/";
            if (path.substr(0, dataZip.length()) == dataZip)
                path = path.replace(0, dataZip.length(), "/http/data/");
#endif  // defined __native_client__
        }
        else
        {
            // if not found in search path return the default path
            path = pathJoin(getPackageDir(), file);
        }

        return path;
    }

    std::string loadTextFileString(const std::string &fileName)
    {
        int contentsLength;
        const char *fileContents = VirtFs::loadFile(fileName, contentsLength);

        if (!fileContents)
        {
            logger->log("Couldn't load text file: %s", fileName.c_str());
            return std::string();
        }
        const std::string str = std::string(fileContents, contentsLength);
        delete [] fileContents;
        return str;
    }

    bool loadTextFile(const std::string &fileName,
                      StringVect &lines)
    {
        int contentsLength;
        const char *fileContents = VirtFs::loadFile(fileName, contentsLength);

        if (!fileContents)
        {
            logger->log("Couldn't load text file: %s", fileName.c_str());
            return false;
        }

        std::istringstream iss(std::string(fileContents, contentsLength));
        std::string line;

        while (getline(iss, line))
            lines.push_back(line);

        delete [] fileContents;
        return true;
    }
}  // namespace VirtFs
