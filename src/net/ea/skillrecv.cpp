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

#include "net/ea/skillrecv.h"

#include "being/playerinfo.h"

#include "gui/windows/skilldialog.h"

#include "net/messagein.h"

#include "debug.h"

namespace Ea
{

void SkillRecv::processPlayerSkillUp(Net::MessageIn &msg)
{
    const int skillId = msg.readInt16("skill id");
    const int level = msg.readInt16("skill level");
    const int sp = msg.readInt16("sp");
    const int range = msg.readInt16("range");
    const Modifiable up = fromBool(msg.readUInt8("up flag"), Modifiable);

    if (skillDialog != nullptr && PlayerInfo::getSkillLevel(skillId) != level)
        skillDialog->playUpdateEffect(skillId);
    PlayerInfo::setSkillLevel(skillId, level);
    if (skillDialog != nullptr)
    {
        if (!skillDialog->updateSkill(skillId, range,
            up, SkillType::Unknown, sp))
        {
            skillDialog->addSkill(SkillOwner::Player,
                skillId, "", level,
                range, up, SkillType::Unknown, sp);
        }
    }
}

}  // namespace Ea
