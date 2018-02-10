/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2018  The ManaPlus Developers
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

#include "utils/itemxmlutils.h"

#include "utils/foreach.h"
#include "utils/stringutils.h"
#include "utils/translation/podict.h"

#include "resources/item/itemfieldtype.h"

#include "debug.h"

void readItemStatsString(std::string &effect,
                         XmlNodeConstPtr node,
                         const ItemFieldInfos &fields)
{
    if (translator == nullptr)
        return;

    FOR_EACH (ItemFieldInfos::const_iterator, it, fields)
    {
        const std::string fieldName = (*it).first;
        const ItemFieldType *const field = (*it).second;

        std::string value = XML::getProperty(node,
            fieldName.c_str(),
            "");
        if (value.empty())
            continue;
        if (!effect.empty())
            effect.append(" / ");
        if (field->sign && isDigit(value))
            value = std::string("+").append(value);
        const std::string format = translator->getStr(field->description);
        effect.append(strprintf(format.c_str(),
            value.c_str()));
    }
}
