/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef GUI_WIDGETS_SKILLINFO_H
#define GUI_WIDGETS_SKILLINFO_H

#include "gui/color.h"

#include <vector>
#include <map>

#include "localconsts.h"

struct SkillData;

class SkillModel;

typedef std::map<int, SkillData*> SkillDataMap;
typedef SkillDataMap::iterator SkillDataMapIter;
typedef SkillDataMap::const_iterator SkillDataMapCIter;

struct SkillInfo final
{
    std::string skillLevel;
    std::string skillExp;
    float progress;
    Color color;
    SkillDataMap dataMap;
    SkillModel *model;
    SkillData *data;
    int level;
    int skillLevelWidth;
    unsigned int id;
    int range;
    bool modifiable;
    bool visible;

    SkillInfo();
    A_DELETE_COPY(SkillInfo)
    ~SkillInfo();

    void update();

    SkillData *getData(const int level) const;
    SkillData *getData1(const int level) const;

    void addData(const int level, SkillData *const data);
};

typedef std::vector<SkillInfo*> SkillList;
typedef SkillList::iterator SkillListIter;

#endif  // GUI_WIDGETS_SKILLINFO_H
