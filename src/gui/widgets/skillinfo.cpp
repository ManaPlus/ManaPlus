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

#include "gui/widgets/skillinfo.h"

#include "being/playerinfo.h"

#include "gui/theme.h"

#include "gui/widgets/skilldata.h"

#include "gui/models/skillmodel.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

SkillInfo::SkillInfo() :
    skillLevel(),
    skillExp(),
    progress(0.0F),
    color(),
    dataMap(),
    model(nullptr),
    data(nullptr),
    level(0),
    skillLevelWidth(0),
    id(0),
    range(0),
    modifiable(false),
    visible(false)
{
    dataMap[0] = new SkillData();
    data = dataMap[0];
}

SkillInfo::~SkillInfo()
{
    FOR_EACH (SkillDataMapIter, it, dataMap)
        delete (*it).second;
    dataMap.clear();
}

void SkillInfo::update()
{
    const int baseLevel = PlayerInfo::getSkillLevel(id);
    const std::pair<int, int> exp = PlayerInfo::getStatExperience(id);

    if (!modifiable && baseLevel == 0 && exp.second == 0)
    {
        if (visible)
        {
            visible = false;
            if (model)
                model->updateVisibilities();
        }
        return;
    }

    const bool updateVisibility = !visible;
    visible = true;

    if (baseLevel == 0)
    {
        skillLevel.clear();
    }
    else
    {
        // TRANSLATORS: skills dialog. skill level
        skillLevel = strprintf(_("Lvl: %d"), baseLevel);
    }

    level = baseLevel;
    skillLevelWidth = -1;

    if (exp.second)
    {
        skillExp = strprintf("%d / %d", exp.first, exp.second);
        progress = static_cast<float>(exp.first)
                   / static_cast<float>(exp.second);
    }
    else
    {
        skillExp.clear();
        progress = 0.0F;
    }

    color = Theme::getProgressColor(Theme::PROG_EXP, progress);

    if (updateVisibility && model)
        model->updateVisibilities();

    data = getData(level);
    if (!data)
        data = dataMap[0];
}


void SkillInfo::addData(const int level1, SkillData *const data1)
{
    dataMap[level1] = data1;
}

SkillData *SkillInfo::getData(const int level1) const
{
    const SkillDataMapCIter it = dataMap.find(level1);
    if (it == dataMap.end())
        return nullptr;
    return (*it).second;
}

SkillData *SkillInfo::getData1(const int lev) const
{
    const SkillDataMapCIter it = dataMap.find(lev);
    if (it == dataMap.end())
        return (*dataMap.begin()).second;
    return (*it).second;
}
