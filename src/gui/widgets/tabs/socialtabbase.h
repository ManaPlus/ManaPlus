/*
 *  The ManaPlus Client
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

#ifndef GUI_WIDGETS_TABS_SOCIALTABBASE_H
#define GUI_WIDGETS_TABS_SOCIALTABBASE_H

#include "actormanager.h"

ACTORMANAGER_H

#include "enums/resources/map/mapitemtype.h"

ENUMS_RESOURCES_MAP_MAPITEMTYPE_H

#define addAvatars(mob, str, type) \
{\
    ava = new Avatar(str);\
    ava->setOnline(false);\
    ava->setLevel(-1);\
    ava->setType(MapItemType::SEPARATOR);\
    ava->setX(0);\
    ava->setY(0);\
    avatars->push_back(ava);\
    mobs = actorManager->get##mob##s();\
    i = mobs.begin();\
    i_end = mobs.end();\
    while (i != i_end)\
    {\
        std::string name;\
        int level = -1;\
        if ((*i).empty())\
        {\
            name = _("(default)");\
            level = 0;\
        }\
        else\
        {\
            name = *i;\
        }\
        ava = new Avatar(name);\
        ava->setOnline(true);\
        ava->setLevel(level);\
        ava->setType(MapItemType::type);\
        ava->setX(0);\
        ava->setY(0);\
        avatars->push_back(ava);\
        ++ i;\
    }\
}

#define updateAtkListStart() \
    if (!socialWindow || !localPlayer || !actorManager)\
        return;\
    STD_VECTOR<Avatar*> *const avatars = mBeings->getMembers();\
    STD_VECTOR<Avatar*>::iterator ia = avatars->begin();\
    while (ia != avatars->end())\
    {\
        delete *ia;\
        ++ ia;\
    }\
    avatars->clear();\
    Avatar *ava = nullptr;\
    std::list<std::string> mobs;\
    std::list<std::string>::const_iterator i;\
    std::list<std::string>::const_iterator i_end;

#endif  // GUI_WIDGETS_TABS_SOCIALTABBASE_H
