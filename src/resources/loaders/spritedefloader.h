/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef RESOURCES_LOADERS_SPRITEDEFLOADER_H
#define RESOURCES_LOADERS_SPRITEDEFLOADER_H

#include <string>

#include "localconsts.h"

class SpriteDef;

namespace Loader
{
    /**
     * Creates a sprite definition based on a given path and the supplied
     * variant.
     */
    SpriteDef *getSprite(const std::string &path,
                         const int variant) A_WARN_UNUSED;

}  // namespace Loader

#endif  // RESOURCES_LOADERS_SPRITEDEFLOADER_H
