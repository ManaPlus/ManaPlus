/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifndef RESOURCES_DB_STATUSEFFECTDB_H
#define RESOURCES_DB_STATUSEFFECTDB_H

#include "enums/simpletypes/enable.h"
#include "enums/simpletypes/skiperror.h"

#include <map>
#include <string>

#include "localconsts.h"

class StatusEffect;

typedef std::map<uint32_t, uint32_t> OptionsMap;
typedef OptionsMap::const_iterator OptionsMapCIter;

namespace StatusEffectDB
{
    /**
     * Retrieves a status effect.
     *
     * \param index Index of the status effect.
     * \param enabling Whether to retrieve the activating effect (true) or
     *    the deactivating effect (false).
     */
    StatusEffect *getStatusEffect(const int index,
                                  const Enable enabling) A_WARN_UNUSED;

    void load();

    void loadXmlFile(const std::string &fileName,
                     const SkipError skipError);

    void unload();

    const OptionsMap& getOptionMap();

    const OptionsMap& getOpt1Map();

    const OptionsMap& getOpt2Map();

    const OptionsMap& getOpt3Map();
}  // namespace StatusEffectDB

#endif  // RESOURCES_DB_STATUSEFFECTDB_H
