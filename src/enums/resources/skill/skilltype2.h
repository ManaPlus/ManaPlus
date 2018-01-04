/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2018  The ManaPlus Developers
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

#ifndef ENUMS_RESOURCES_SKILL_SKILLTYPE2_H
#define ENUMS_RESOURCES_SKILL_SKILLTYPE2_H

// aka inf2
namespace SkillType2
{
    enum SkillType2
    {
        Unknown         = 0,
        Quest           = 1,
        Npc             = 2,
        Wedding         = 4,
        Spirit          = 8,
        Guild           = 16,
        SongDance       = 32,
        Ensemble        = 64,
        Trap            = 128,
        TargetSelf      = 256,
        NoTargetSelf    = 512,
        PartyOnly       = 1024,
        GuildOnly       = 2048,
        NoEnemy         = 4096,
        NoLandProtector = 8192,
        Chorus          = 16384,
        FreeCastNormal  = 32768,
        FreeCastReduced = 65536,
        FreeCastAny     = 98304
    };
}  // namespace SkillType2

#endif  // ENUMS_RESOURCES_SKILL_SKILLTYPE2_H
