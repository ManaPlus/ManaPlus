/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef RESOURCES_DB_NPCDIALOGDB_H
#define RESOURCES_DB_NPCDIALOGDB_H

#include "enums/simpletypes/skiperror.h"

#include <map>
#include <string>

#include "localconsts.h"

struct NpcDialogInfo;

/**
 * Color information database.
 */
namespace NpcDialogDB
{
    /**
     * Loads the map remap data from <code>maps\remap.xml</code>.
     */
    void load();

    void loadXmlFile(const std::string &fileName,
                     const SkipError skipError);

    /**
     * Clear the remap data
     */
    void unload();

    void deleteDialog(const std::string &name);

    NpcDialogInfo *getDialog(const std::string &name);

    typedef std::map<std::string, NpcDialogInfo*> Dialogs;
    typedef Dialogs::iterator DialogsIter;

}  // namespace NpcDialogDB

#endif  // RESOURCES_DB_NPCDIALOGDB_H
