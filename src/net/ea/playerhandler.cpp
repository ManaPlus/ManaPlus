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

#include "net/ea/playerhandler.h"

#include "party.h"
#include "notifymanager.h"
#include "units.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/skilldialog.h"
#include "gui/windows/statuswindow.h"

#include "net/ea/eaprotocol.h"
#include "net/ea/playerrecv.h"

#include "debug.h"

namespace Ea
{

PlayerHandler::PlayerHandler()
{
}

void PlayerHandler::decreaseAttribute(const AttributesT attr A_UNUSED) const
{
}

void PlayerHandler::ignorePlayer(const std::string &player A_UNUSED,
                                 const bool ignore A_UNUSED) const
{
}

void PlayerHandler::ignoreAll(const bool ignore A_UNUSED) const
{
}

Vector PlayerHandler::getDefaultWalkSpeed() const
{
    // Return an normalized speed for any side
    // as the offset is calculated elsewhere.
    return Vector(150, 150, 0);
}

bool PlayerHandler::canUseMagic() const
{
    return PlayerInfo::getStatEffective(Attributes::MATK) > 0;
}

#define setStatComplex(stat) \
    PlayerInfo::setStatBase(stat, base, notify); \
    if (mod != NoStat) \
        PlayerInfo::setStatMod(stat, mod)

void PlayerHandler::setStat(Net::MessageIn &msg,
                            const int type,
                            const int base,
                            const int mod,
                            const Notify notify) const
{
    switch (type)
    {
        case Ea::WALK_SPEED:
            localPlayer->setWalkSpeed(Vector(static_cast<float>(
                base), static_cast<float>(base), 0));
            PlayerInfo::setStatBase(Attributes::WALK_SPEED, base);
            PlayerInfo::setStatMod(Attributes::WALK_SPEED, 0);
            break;
        case Ea::EXP:
            PlayerInfo::setAttribute(Attributes::EXP, base);
            break;
        case Ea::JOB_EXP:
            PlayerInfo::setStatExperience(Attributes::JOB, base,
                PlayerInfo::getStatExperience(Attributes::JOB).second);
            break;
        case Ea::KARMA:
            PlayerInfo::setStatBase(Attributes::KARMA, base);
            PlayerInfo::setStatMod(Attributes::KARMA, 0);
            break;
        case Ea::MANNER:
            PlayerInfo::setStatBase(Attributes::MANNER, base);
            PlayerInfo::setStatMod(Attributes::MANNER, 0);
            break;
        case Ea::HP:
            PlayerInfo::setAttribute(Attributes::HP, base);
            if (localPlayer->isInParty() && Party::getParty(1))
            {
                PartyMember *const m = Party::getParty(1)
                    ->getMember(localPlayer->getId());
                if (m)
                {
                    m->setHp(base);
                    m->setMaxHp(PlayerInfo::getAttribute(Attributes::MAX_HP));
                }
            }
            break;
        case Ea::MAX_HP:
            PlayerInfo::setAttribute(Attributes::MAX_HP, base);

            if (localPlayer->isInParty() && Party::getParty(1))
            {
                PartyMember *const m = Party::getParty(1)->getMember(
                    localPlayer->getId());
                if (m)
                {
                    m->setHp(PlayerInfo::getAttribute(Attributes::HP));
                    m->setMaxHp(base);
                }
            }
            break;
        case Ea::MP:
            PlayerInfo::setAttribute(Attributes::MP, base);
            break;
        case Ea::MAX_MP:
            PlayerInfo::setAttribute(Attributes::MAX_MP, base);
            break;
        case Ea::CHAR_POINTS:
            PlayerInfo::setAttribute(Attributes::CHAR_POINTS, base);
            break;
        case Ea::LEVEL:
            PlayerInfo::setAttribute(Attributes::LEVEL, base);
            if (localPlayer)
            {
                localPlayer->setLevel(base);
                localPlayer->updateName();
            }
            break;
        case Ea::SKILL_POINTS:
            PlayerInfo::setAttribute(Attributes::SKILL_POINTS, base);
            if (skillDialog)
                skillDialog->update();
            break;
        case Ea::STR:
            setStatComplex(Attributes::STR);
            break;
        case Ea::AGI:
            setStatComplex(Attributes::AGI);
            break;
        case Ea::VIT:
            setStatComplex(Attributes::VIT);
            break;
        case Ea::INT:
            setStatComplex(Attributes::INT);
            break;
        case Ea::DEX:
            setStatComplex(Attributes::DEX);
            break;
        case Ea::LUK:
            setStatComplex(Attributes::LUK);
            break;
        case Ea::MONEY:
        {
            const int oldMoney = PlayerInfo::getAttribute(Attributes::MONEY);
            const int newMoney = base;
            if (newMoney > oldMoney)
            {
                NotifyManager::notify(NotifyTypes::MONEY_GET,
                    Units::formatCurrency(newMoney - oldMoney));
            }
            else if (newMoney < oldMoney)
            {
                NotifyManager::notify(NotifyTypes::MONEY_SPENT,
                    Units::formatCurrency(oldMoney - newMoney).c_str());
            }

            PlayerInfo::setAttribute(Attributes::MONEY, newMoney);
            break;
        }
        case Ea::EXP_NEEDED:
            PlayerInfo::setAttribute(Attributes::EXP_NEEDED, base);
            break;
        case Ea::JOB_MOD:
            PlayerInfo::setStatExperience(Attributes::JOB,
                PlayerInfo::getStatExperience(Attributes::JOB).first, base);
            break;
        case Ea::TOTAL_WEIGHT:
            PlayerInfo::setAttribute(Attributes::TOTAL_WEIGHT, base);
            break;
        case Ea::MAX_WEIGHT:
            PlayerInfo::setAttribute(Attributes::MAX_WEIGHT, base);
            break;
        case Ea::STR_NEEDED:
            statusWindow->setPointsNeeded(Attributes::STR, base);
            break;
        case Ea::AGI_NEEDED:
            statusWindow->setPointsNeeded(Attributes::AGI, base);
            break;
        case Ea::VIT_NEEDED:
            statusWindow->setPointsNeeded(Attributes::VIT, base);
            break;
        case Ea::INT_NEEDED:
            statusWindow->setPointsNeeded(Attributes::INT, base);
            break;
        case Ea::DEX_NEEDED:
            statusWindow->setPointsNeeded(Attributes::DEX, base);
            break;
        case Ea::LUK_NEEDED:
            statusWindow->setPointsNeeded(Attributes::LUK, base);
            break;

        case Ea::ATK:
            PlayerInfo::setStatBase(Attributes::ATK, base);
            PlayerInfo::updateAttrs();
            break;
        case Ea::ATK_MOD:
            PlayerInfo::setStatMod(Attributes::ATK, base);
            PlayerInfo::updateAttrs();
            break;
        case Ea::MATK:
            PlayerInfo::setStatBase(Attributes::MATK, base);
            break;
        case Ea::MATK_MOD:
            PlayerInfo::setStatMod(Attributes::MATK, base);
            break;
        case Ea::DEF:
            PlayerInfo::setStatBase(Attributes::DEF, base);
            break;
        case Ea::DEF_MOD:
            PlayerInfo::setStatMod(Attributes::DEF, base);
            break;
        case Ea::MDEF:
            PlayerInfo::setStatBase(Attributes::MDEF, base);
            break;
        case Ea::MDEF_MOD:
            PlayerInfo::setStatMod(Attributes::MDEF, base);
            break;
        case Ea::HIT:
            PlayerInfo::setStatBase(Attributes::HIT, base);
            break;
        case Ea::FLEE:
            PlayerInfo::setStatBase(Attributes::FLEE, base);
            break;
        case Ea::FLEE_MOD:
            PlayerInfo::setStatMod(Attributes::FLEE, base);
            break;
        case Ea::CRIT:
            PlayerInfo::setStatBase(Attributes::CRIT, base);
            break;
        case Ea::ATTACK_DELAY:
            localPlayer->setAttackSpeed(base);
            PlayerInfo::setStatBase(Attributes::ATTACK_DELAY, base);
            PlayerInfo::setStatMod(Attributes::ATTACK_DELAY, 0);
            PlayerInfo::updateAttrs();
            break;
        case Ea::JOB:
            PlayerInfo::setStatBase(Attributes::JOB, base);
            break;

        default:
            UNIMPLIMENTEDPACKET;
            break;
    }
}

}  // namespace Ea
