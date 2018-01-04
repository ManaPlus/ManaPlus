/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef GUI_WIDGETS_SKILLINFO_H
#define GUI_WIDGETS_SKILLINFO_H

#include "enums/simpletypes/modifiable.h"
#include "enums/simpletypes/visible.h"

#include "enums/resources/skill/casttype.h"
#include "enums/resources/skill/skillowner.h"
#include "enums/resources/skill/skilltype.h"

#include "utils/vector.h"

#include <string>
#include <map>

#include "localconsts.h"

struct SkillData;

class SkillModel;
class SkillTab;

typedef std::map<int, SkillData*> SkillDataMap;
typedef SkillDataMap::iterator SkillDataMapIter;
typedef SkillDataMap::const_iterator SkillDataMapCIter;

struct SkillInfo final
{
    std::string skillLevel;
    std::string skillEffect;
    std::string useButton;
    std::string errorText;
    std::string castingAction;
    std::string castingRideAction;
    std::string castingSkyAction;
    std::string castingWaterAction;
    SkillDataMap dataMap;
    SkillModel *model;
    SkillTab *tab;
    SkillData *data;
    int level;
    int customSelectedLevel;
    int customOffsetX;
    int customOffsetY;
    int skillLevelWidth;
    unsigned int id;
    int range;
    int sp;
    int duration;
    int durationTime;
    int cooldown;
    int x;
    int y;
    SkillType::SkillType type;
    SkillOwner::Type owner;
    CastTypeT customCastType;
    Modifiable modifiable;
    Visible visible;
    Visible alwaysVisible;
    bool useTextParameter;

    SkillInfo();
    A_DELETE_COPY(SkillInfo)
    ~SkillInfo();

    void update();

    SkillData *getData(const int level) const A_WARN_UNUSED;
    SkillData *getData1(const int level) const A_WARN_UNUSED;

    void addData(const int level, SkillData *const data);

    bool isUsable() const noexcept2 A_WARN_UNUSED
    {
        return type == SkillType::Attack
            || type == SkillType::Self
            || type == SkillType::Support;
    }

    std::string toDataStr() const A_WARN_UNUSED;
};

typedef STD_VECTOR<SkillInfo*> SkillList;
typedef SkillList::iterator SkillListIter;

#endif  // GUI_WIDGETS_SKILLINFO_H
