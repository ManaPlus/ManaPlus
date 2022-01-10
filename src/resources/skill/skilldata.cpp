/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "resources/skill/skilldata.h"

#include "configuration.h"

#include "gui/theme.h"

#include "resources/image/image.h"

#include "resources/loaders/imageloader.h"

#include "debug.h"

SkillData::SkillData() :
    name(),
    shortName(),
    dispName(),
    description(),
    missile(),
    castingMissile(),
    invokeCmd(),
    castingAnimation(),
    soundHit(std::string(), 0),
    soundMiss(std::string(), 0),
    icon(nullptr),
    updateEffectId(-1),
    removeEffectId(-1),
    hitEffectId(-1),
    missEffectId(-1),
    castingSrcEffectId(-1),
    castingDstEffectId(-1),
    castingGroundEffectId(-1),
    srcEffectId(-1),
    dstEffectId(-1),
    haveIcon(false),
    autoTab(true)
{
}

SkillData::~SkillData()
{
    if (icon != nullptr)
    {
        icon->decRef();
        icon = nullptr;
    }
}

void SkillData::setIcon(const std::string &iconPath)
{
    if (!iconPath.empty())
        icon = Loader::getImage(iconPath);

    if (icon == nullptr)
    {
        icon = Theme::getImageFromTheme(
            paths.getStringValue("unknownItemFile"));
    }
}
