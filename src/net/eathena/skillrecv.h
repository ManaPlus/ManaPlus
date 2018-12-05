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

#ifndef NET_EATHENA_SKILLRECV_H
#define NET_EATHENA_SKILLRECV_H

namespace Net
{
    class MessageIn;
}  // namespace Net

namespace EAthena
{
    namespace SkillRecv
    {
        void processSkillFailed(Net::MessageIn &msg);
        void processPlayerSkills(Net::MessageIn &msg);
        void processSkillCoolDown(Net::MessageIn &msg);
        void processSkillCoolDownList(Net::MessageIn &msg);
        void processSkillAdd(Net::MessageIn &msg);
        void processSkillAdd2(Net::MessageIn &msg);
        void processSkillUpdate(Net::MessageIn &msg);
        void processSkillUpdate2(Net::MessageIn &msg);
        void processSkillDelete(Net::MessageIn &msg);
        void processSkillWarpPoint(Net::MessageIn &msg);
        void processSkillWarpPoint2(Net::MessageIn &msg);
        void processSkillMemoMessage(Net::MessageIn &msg);
        void processSkillProduceMixList(Net::MessageIn &msg);
        void processSkillProduceEffect(Net::MessageIn &msg);
        void processSkillUnitUpdate(Net::MessageIn &msg);
        void processSkillArrowCreateList(Net::MessageIn &msg);
        void processSkillAutoSpells1(Net::MessageIn &msg);
        void processSkillAutoSpells2(Net::MessageIn &msg);
        void processSkillDevotionEffect(Net::MessageIn &msg);
        void processSkillItemListWindow(Net::MessageIn &msg);
        void processSkillScale(Net::MessageIn &msg);
    }  // namespace SkillRecv
}  // namespace EAthena

#endif  // NET_EATHENA_SKILLRECV_H
