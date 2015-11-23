/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#ifndef NET_EA_SKILLRECV_H
#define NET_EA_SKILLRECV_H

#include "localconsts.h"

namespace Net
{
    class MessageIn;
}

/** job dependend identifiers (?)  */
static const unsigned int SKILL_BASIC =       0x0001;
static const unsigned int SKILL_WARP =        0x001b;
static const unsigned int SKILL_STEAL =       0x0032;
static const unsigned int SKILL_ENVENOM =     0x0034;

/** basic skills identifiers       */
static const unsigned int BSKILL_TRADE =      0x0000;
static const unsigned int BSKILL_EMOTE =      0x0001;
static const unsigned int BSKILL_SIT =        0x0002;
static const unsigned int BSKILL_CREATECHAT = 0x0003;
static const unsigned int BSKILL_JOINPARTY =  0x0004;
static const unsigned int BSKILL_SHOUT =      0x0005;

/** reasons why action failed      */
static const unsigned int RFAIL_SKILLDEP =    0x00;
static const unsigned int RFAIL_INSUFSP =     0x01;
static const unsigned int RFAIL_INSUFHP =     0x02;
static const unsigned int RFAIL_NOMEMO =      0x03;
static const unsigned int RFAIL_SKILLDELAY =  0x04;
static const unsigned int RFAIL_ZENY =        0x05;
static const unsigned int RFAIL_WEAPON =      0x06;
static const unsigned int RFAIL_REDGEM =      0x07;
static const unsigned int RFAIL_BLUEGEM =     0x08;
static const unsigned int RFAIL_OVERWEIGHT =  0x09;

static const unsigned int RFAIL_SUMMON         = 19;
static const unsigned int RFAIL_NEED_ITEM      = 71;
static const unsigned int RFAIL_NEED_EQUIPMENT = 72;
static const unsigned int RFAIL_SPIRITS        = 74;

/** should always be zero if failed */
static const unsigned int SKILL_FAILED =      0x00;

namespace Ea
{
    namespace SkillRecv
    {
        void processPlayerSkillUp(Net::MessageIn &msg);
    }  // namespace SkillRecv
}  // namespace Ea

#endif  // NET_EA_SKILLRECV_H
