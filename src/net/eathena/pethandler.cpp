/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#include "net/eathena/pethandler.h"

#include "actormanager.h"
#include "inventory.h"
#include "notifymanager.h"

#include "being/localplayer.h"
#include "being/petinfo.h"
#include "being/playerinfo.h"

#include "gui/chatconsts.h"

#include "gui/windows/eggselectiondialog.h"

#include "gui/widgets/tabs/chat/chattab.h"

#include "net/chathandler.h"
#include "net/inventoryhandler.h"

#include "net/ea/eaprotocol.h"

#include "net/eathena/menu.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "resources/notifytypes.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

extern Net::PetHandler *petHandler;

namespace EAthena
{

PetHandler::PetHandler() :
    MessageHandler(),
    mRandCounter(1000)
{
    static const uint16_t _messages[] =
    {
        SMSG_PET_MESSAGE,
        SMSG_PET_ROULETTE,
        SMSG_PET_EGGS_LIST,
        SMSG_PET_DATA,
        SMSG_PET_STATUS,
        SMSG_PET_FOOD,
        0
    };
    handledMessages = _messages;
    petHandler = this;
}

void PetHandler::handleMessage(Net::MessageIn &msg)
{
    BLOCK_START("PetHandler::handleMessage")
    switch (msg.getId())
    {
        case SMSG_PET_MESSAGE:
            processPetMessage(msg);
            break;

        case SMSG_PET_ROULETTE:
            processPetRoulette(msg);
            break;

        case SMSG_PET_EGGS_LIST:
            processEggsList(msg);
            break;

        case SMSG_PET_DATA:
            processPetData(msg);
            break;

        case SMSG_PET_STATUS:
            processPetStatus(msg);
            break;

        case SMSG_PET_FOOD:
            processPetFood(msg);
            break;

        default:
            break;
    }
    BLOCK_END("PetHandler::handleMessage")
}

void PetHandler::move(const int petId A_UNUSED,
                      const int x A_UNUSED, const int y A_UNUSED) const
{
}

void PetHandler::spawn(const Being *const being A_UNUSED,
                       const int petId A_UNUSED,
                       const int x A_UNUSED, const int y A_UNUSED) const
{
}

void PetHandler::emote(const uint8_t emoteId, const int petId A_UNUSED)
{
    mRandCounter ++;
    if (mRandCounter > 10000)
        mRandCounter = 1000;

    chatHandler->talk(strprintf("\302\202\302e%dz%d",
        static_cast<int>(emoteId), mRandCounter), GENERAL_CHANNEL);
}

void PetHandler::catchPet(const Being *const being) const
{
    if (!being)
        return;

    createOutPacket(CMSG_PET_CATCH);
    outMsg.writeInt32(being->getId(), "monster id");
}

void PetHandler::sendPetMessage(const int data) const
{
    createOutPacket(CMSG_PET_SEND_MESSAGE);
    outMsg.writeInt32(data, "param");
}

void PetHandler::setName(const std::string &name) const
{
    createOutPacket(CMSG_PET_SET_NAME);
    outMsg.writeString(name, 24, "name");
}

void PetHandler::processPetMessage(Net::MessageIn &msg)
{
    const int id = msg.readInt32("pet id");
    const int data = msg.readInt32("param");
    Being *const dstBeing = actorManager->findBeing(id);
    if (!dstBeing)
        return;

    const int hungry = data - (dstBeing->getSubType() - 100) * 100 - 50;
    if (hungry >= 0 && hungry <= 4)
    {
        if (localChatTab && localPlayer)
        {
            std::string nick = strprintf(_("%s's pet"),
                localPlayer->getName().c_str());
            localChatTab->chatLog(nick, strprintf("hungry level %d", hungry));
        }
        PetInfo *const info = PlayerInfo::getPet();
        if (!info || info->id != id)
            return;
        info->hungry = hungry;
    }
}

void PetHandler::processPetRoulette(Net::MessageIn &msg)
{
    const uint8_t data = msg.readUInt8("data");
    switch (data)
    {
        case 0:
            NotifyManager::notify(NotifyTypes::PET_CATCH_FAILED);
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::PET_CATCH_SUCCESS);
            break;
        default:
            NotifyManager::notify(NotifyTypes::PET_CATCH_UNKNOWN, data);
            break;
    }
}

void PetHandler::processEggsList(Net::MessageIn &msg)
{
    const int count = (msg.readInt16("len") - 4) / 2;
    Inventory *const inv = PlayerInfo::getInventory();
    menu = MenuType::Eggs;

    if (count == 1)
    {
        const int index = msg.readInt16("index") - INVENTORY_OFFSET;
        const Item *const item = inv->getItem(index);
        inventoryHandler->selectEgg(item);
        return;
    }
    SellDialog *const dialog = new EggSelectionDialog;
    dialog->postInit();

    for (int f = 0; f < count; f ++)
    {
        const int index = msg.readInt16("index") - INVENTORY_OFFSET;
        const Item *const item = inv->getItem(index);

        if (item)
            dialog->addItem(item, 0);
    }
}

void PetHandler::processPetData(Net::MessageIn &msg)
{
    const int cmd = msg.readUInt8("type");
    const int id = msg.readInt32("pet id");
    Being *const dstBeing = actorManager->findBeing(id);
    const int data = msg.readInt32("data");
    if (!cmd)  // pre init
    {
        PetInfo *const info = new PetInfo;
        info->id = id;
        PlayerInfo::setPet(info);
        PlayerInfo::setPetBeing(dstBeing);
        return;
    }
    PetInfo *const info = PlayerInfo::getPet();
    if (!info)
        return;
    switch (cmd)
    {
        case 1:  // intimacy
            info->intimacy = data;
            break;
        case 2:  // hunger
            info->hungry = data;
            break;
        case 3:  // accesory
            info->equip = data;
            break;
        case 4:  // performance
            info->performance = data;
            break;
        case 5:  // hair style
            info->hairStyle = data;
            break;
        default:
            break;
    }
}

void PetHandler::processPetStatus(Net::MessageIn &msg)
{
    const std::string name = msg.readString(24, "pet name");
    msg.readUInt8("rename flag");
    const int level = msg.readInt16("level");
    const int hungry = msg.readInt16("hungry");
    const int intimacy = msg.readInt16("intimacy");
    const int equip = msg.readInt16("equip");
    const int race = msg.readInt16("class");

//    Being *const being = PlayerInfo::getPetBeing();
//    if (being)
//        being->setLevel(level);

    PetInfo *const info = PlayerInfo::getPet();
    if (!info)
        return;
    info->name = name;
    info->level = level;
    info->hungry = hungry;
    info->intimacy = intimacy;
    info->equip = equip;
    info->race = race;
}

void PetHandler::processPetFood(Net::MessageIn &msg)
{
    // +++ need show notification message about success or fail
    msg.readUInt8("result");
    msg.readInt16("food id");
}

void PetHandler::requestStatus() const
{
    createOutPacket(CMSG_PET_MENU_ACTION);
    outMsg.writeInt8(0, "action");
}

void PetHandler::feed() const
{
    createOutPacket(CMSG_PET_MENU_ACTION);
    outMsg.writeInt8(1, "action");
}

void PetHandler::dropLoot() const
{
    createOutPacket(CMSG_PET_MENU_ACTION);
    outMsg.writeInt8(2, "action");  // performance
}

void PetHandler::returnToEgg() const
{
    createOutPacket(CMSG_PET_MENU_ACTION);
    outMsg.writeInt8(3, "action");
    PlayerInfo::setPet(nullptr);
}

void PetHandler::unequip() const
{
    createOutPacket(CMSG_PET_MENU_ACTION);
    outMsg.writeInt8(4, "action");
}

}  // namespace EAthena
