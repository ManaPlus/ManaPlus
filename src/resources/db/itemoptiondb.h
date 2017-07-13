/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2017  The ManaPlus Developers
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

#ifndef RESOURCES_DB_ITEMOPTIONDB_H
#define RESOURCES_DB_ITEMOPTIONDB_H

#include "enums/simpletypes/skiperror.h"

#include "utils/vector.h"

#include <map>
#include <string>

#include "localconsts.h"

struct ItemFieldType;

namespace ItemOptionDb
{
    void load();

    void unload();

    void loadXmlFile(const std::string &fileName,
                     const SkipError skipError);

    const STD_VECTOR<ItemFieldType*> &getFields(const int id);

    typedef std::map<int, STD_VECTOR<ItemFieldType*> > OptionInfos;
}  // namespace ItemOptionDb

#endif  // RESOURCES_DB_ITEMOPTIONDB_H
