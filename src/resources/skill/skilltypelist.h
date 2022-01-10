/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2019  The ManaPlus Developers
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

#ifndef RESOURCES_SKILLTYPELIST_H
#define RESOURCES_SKILLTYPELIST_H

#include "utils/gettext.h"

#include "resources/skill/skilltypeentry.h"

#include "localconsts.h"

const size_t skillTypeListSize = 6;

SkillTypeEntry skillTypeList[skillTypeListSize] =
{
    // TRANSLATORS: Skill type
    { SkillType::Attack,     N_("Attack") },
    // TRANSLATORS: Skill type
    { SkillType::Ground,     N_("Ground") },
    // TRANSLATORS: Skill type
    { SkillType::Self,       N_("Self") },
    // TRANSLATORS: Skill type
    { SkillType::Unused,     N_("Unused") },
    // TRANSLATORS: Skill type
    { SkillType::Support,    N_("Support") },
    // TRANSLATORS: Skill type
    { SkillType::TargetTrap, N_("TargetTrap") }
};

#endif  // RESOURCES_SKILLTYPELIST_H
