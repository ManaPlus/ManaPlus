/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef CONST_RESOURCES_SPRITEACTION_H
#define CONST_RESOURCES_SPRITEACTION_H

#include <string>

/*
 * Remember those are the main action.
 * Action subtypes, e.g.: "attack_bow" are to be passed by items.xml after
 * an ACTION_ATTACK call.
 * Which special to be use to to be passed with the USE_SPECIAL call.
 * Running, walking, ... is a sub-type of moving.
 * ...
 * Please don't add hard-coded subtypes here!
 */
namespace SpriteAction
{
    static const std::string DEFAULT("stand");
    static const std::string STAND("stand");
    static const std::string SIT("sit");
    static const std::string SITTOP("sittop");
    static const std::string DEAD("dead");
    static const std::string MOVE("walk");
    static const std::string ATTACK("attack");
    static const std::string SPAWN("spawn");
    static const std::string CAST("cast");

    static const std::string FLY("fly");
    static const std::string SWIM("swim");
    static const std::string RIDE("ride");
    static const std::string STANDSKY("standsky");
    static const std::string STANDWATER("standwater");
    static const std::string STANDRIDE("standride");
    static const std::string SITSKY("sitsky");
    static const std::string SITWATER("sitwater");
    static const std::string SITRIDE("sitride");
    static const std::string ATTACKSKY("attacksky");
    static const std::string ATTACKWATER("attackwater");
    static const std::string ATTACKRIDE("attackride");
    static const std::string CASTSKY("castsky");
    static const std::string CASTWATER("castwater");
    static const std::string CASTRIDE("castride");
    static const std::string SPAWNSKY("spawnsky");
    static const std::string SPAWNWATER("spawnwater");
    static const std::string SPAWNRIDE("spawnride");
    static const std::string DEADSKY("deadsky");
    static const std::string DEADWATER("deadwater");
    static const std::string DEADRIDE("deadride");

    static const std::string INVALID;
}  // namespace SpriteAction

#endif  // CONST_RESOURCES_SPRITEACTION_H
