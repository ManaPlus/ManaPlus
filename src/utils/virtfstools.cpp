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

#include "utils/virtfstools.h"

#include "logger.h"

#include "utils/virtfile.h"
#include "utils/virtfs.h"

#include <unistd.h>

#include "debug.h"

namespace VirtFs
{
    void *loadFile(const std::string &restrict fileName,
                   int &restrict fileSize)
    {
        // Attempt to open the specified file using PhysicsFS
        VirtFile *restrict const file = VirtFs::openRead(fileName.c_str());

        if (!file)
        {
            logger->log("Warning: Failed to load %s: %s",
                fileName.c_str(),
                VirtFs::getLastError());
            return nullptr;
        }

        logger->log("Loaded %s/%s",
            VirtFs::getRealDir(fileName.c_str()),
            fileName.c_str());

        fileSize = CAST_S32(VirtFs::fileLength(file));
        // Allocate memory and load the file
        void *restrict const buffer = calloc(fileSize, 1);
        VirtFs::read(file, buffer, 1, fileSize);
        VirtFs::close(file);

        return buffer;
    }

    void searchAndAddArchives(const std::string &restrict path,
                              const std::string &restrict ext,
                              const Append append)
    {
        char **list = VirtFs::enumerateFiles(path.c_str());

        for (char **i = list; *i; i++)
        {
            const size_t len = strlen(*i);

            if (len > ext.length() &&
                !ext.compare((*i) + (len - ext.length())))
            {
                const std::string file = path + (*i);
                const std::string realPath = std::string(
                    VirtFs::getRealDir(file.c_str()));
                VirtFs::addZipToSearchPath(std::string(realPath).append(
                    dirSeparator).append(file), append);
            }
        }
        VirtFs::freeList(list);
    }

    void searchAndRemoveArchives(const std::string &restrict path,
                                 const std::string &restrict ext)
    {
        char **list = VirtFs::enumerateFiles(path.c_str());

        for (char **i = list; *i; i++)
        {
            const size_t len = strlen(*i);
            if (len > ext.length() &&
                !ext.compare((*i) + (len - ext.length())))
            {
                const std::string file = path + (*i);
                const std::string realPath = std::string(
                    VirtFs::getRealDir(file.c_str()));
                VirtFs::removeZipFromSearchPath(std::string(
                    realPath).append(
                    dirSeparator).append(
                    file));
            }
        }
        VirtFs::freeList(list);
    }
}  // namespace VirtFs
