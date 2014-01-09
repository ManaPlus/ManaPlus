/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef RESOURCES_MODINFO_H
#define RESOURCES_MODINFO_H

#include <string>
#include <map>

#include "localconsts.h"

class ModInfo final
{
    public:
        ModInfo();

        A_DELETE_COPY(ModInfo)

        void setName(const std::string &name)
        { mName = name; }

        const std::string &getName() const A_WARN_UNUSED
        { return mName; }

        void setDescription(const std::string &text)
        { mDescription = text; }

        const std::string &getDescription() const A_WARN_UNUSED
        { return mDescription; }

        void setHelp(const std::string &text)
        { mHelp = text; }

        const std::string &getHelp() const A_WARN_UNUSED
        { return mHelp; }

        void setLocalDir(const std::string &text)
        { mLocalDir = text; }

        const std::string &getLocalDir() const A_WARN_UNUSED
        { return mLocalDir; }

    private:
        std::string mName;
        std::string mDescription;
        std::string mHelp;
        std::string mLocalDir;
};

typedef std::map<std::string, ModInfo*> ModInfos;
typedef ModInfos::iterator ModInfoIterator;
typedef ModInfos::const_iterator ModInfoCIterator;

#endif  // RESOURCES_MODINFO_H
