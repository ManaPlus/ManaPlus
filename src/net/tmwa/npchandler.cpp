/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "net/tmwa/npchandler.h"

#include "being/localplayer.h"

#include "const/net/inventory.h"

#include "gui/windows/npcdialog.h"

#include "gui/widgets/createwidget.h"

#include "net/messagein.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/protocolout.h"

#include "net/ea/npcrecv.h"

#include "debug.h"

extern Net::NpcHandler *npcHandler;

namespace TmwAthena
{

NpcHandler::NpcHandler() :
    Ea::NpcHandler()
{
    npcHandler = this;
}

void NpcHandler::talk(const Being *const being) const
{
    if (!being)
        return;
    createOutPacket(CMSG_NPC_TALK);
    outMsg.writeBeingId(being->getId(), "npc id");
    outMsg.writeInt8(0, "unused");
}

void NpcHandler::nextDialog(const BeingId npcId) const
{
    createOutPacket(CMSG_NPC_NEXT_REQUEST);
    outMsg.writeBeingId(npcId, "npc id");
}

void NpcHandler::closeDialog(const BeingId npcId)
{
    createOutPacket(CMSG_NPC_CLOSE);
    outMsg.writeBeingId(npcId, "npc id");

    const NpcDialogs::iterator it = NpcDialog::mNpcDialogs.find(npcId);
    if (it != NpcDialog::mNpcDialogs.end())
    {
        NpcDialog *const dialog = (*it).second;
        if (dialog)
            dialog->close();
        if (dialog == Ea::NpcRecv::mDialog)
            Ea::NpcRecv::mDialog = nullptr;
        NpcDialog::mNpcDialogs.erase(it);
    }
}

void NpcHandler::listInput(const BeingId npcId,
                           const unsigned char value) const
{
    createOutPacket(CMSG_NPC_LIST_CHOICE);
    outMsg.writeBeingId(npcId, "npc id");
    outMsg.writeInt8(value, "value");
}

void NpcHandler::integerInput(const BeingId npcId,
                              const int value) const
{
    createOutPacket(CMSG_NPC_INT_RESPONSE);
    outMsg.writeBeingId(npcId, "npc id");
    outMsg.writeInt32(value, "value");
}

void NpcHandler::stringInput(const BeingId npcId,
                             const std::string &value) const
{
    createOutPacket(CMSG_NPC_STR_RESPONSE);
    outMsg.writeInt16(CAST_S16(value.length() + 9), "len");
    outMsg.writeBeingId(npcId, "npc id");
    outMsg.writeString(value, CAST_S32(value.length()), "value");
    outMsg.writeInt8(0, "null byte");
}

void NpcHandler::buy(const Being *const being) const
{
    if (!being)
        return;
    createOutPacket(CMSG_NPC_BUY_SELL_REQUEST);
    outMsg.writeBeingId(being->getId(), "npc id");
    outMsg.writeInt8(0, "action");
}

void NpcHandler::buy(const BeingId beingId) const
{
    createOutPacket(CMSG_NPC_BUY_SELL_REQUEST);
    outMsg.writeBeingId(beingId, "npc id");
    outMsg.writeInt8(0, "action");
}

void NpcHandler::sell(const BeingId beingId) const
{
    createOutPacket(CMSG_NPC_BUY_SELL_REQUEST);
    outMsg.writeBeingId(beingId, "npc id");
    outMsg.writeInt8(1, "action");
}

void NpcHandler::buyItem(const BeingId beingId A_UNUSED,
                         const int itemId,
                         const ItemColor color A_UNUSED,
                         const int amount) const
{
    createOutPacket(CMSG_NPC_BUY_REQUEST);
    outMsg.writeInt16(8, "len");
    outMsg.writeInt16(CAST_S16(amount), "amount");
    outMsg.writeInt16(CAST_S16(itemId), "item id");
}

void NpcHandler::buyItems(std::vector<ShopItem*> &items A_UNUSED) const
{
}

void NpcHandler::sellItem(const BeingId beingId A_UNUSED,
                          const int itemId,
                          const int amount) const
{
    createOutPacket(CMSG_NPC_SELL_REQUEST);
    outMsg.writeInt16(8, "len");
    outMsg.writeInt16(CAST_S16(
        itemId + INVENTORY_OFFSET), "index");
    outMsg.writeInt16(CAST_S16(amount), "amount");
}

void NpcHandler::sellItems(std::vector<ShopItem*> &items A_UNUSED) const
{
}

void NpcHandler::completeProgressBar() const
{
}

void NpcHandler::produceMix(const int nameId A_UNUSED,
                            const int materialId1 A_UNUSED,
                            const int materialId2 A_UNUSED,
                            const int materialId3 A_UNUSED) const
{
}

void NpcHandler::cooking(const CookingTypeT type A_UNUSED,
                         const int nameId A_UNUSED) const
{
}

void NpcHandler::repair(const int index A_UNUSED) const
{
}

void NpcHandler::refine(const int index A_UNUSED) const
{
}

void NpcHandler::identify(const int index A_UNUSED) const
{
}

void NpcHandler::selectArrow(const int nameId A_UNUSED) const
{
}

void NpcHandler::selectAutoSpell(const int skillId A_UNUSED) const
{
}

BeingId NpcHandler::getNpc(Net::MessageIn &msg,
                           const NpcActionT action)
{
    const BeingId npcId = msg.readBeingId("npc id");

    const NpcDialogs::const_iterator diag = NpcDialog::mNpcDialogs.find(npcId);
    Ea::NpcRecv::mDialog = nullptr;

    if (diag == NpcDialog::mNpcDialogs.end())
    {
        // +++ must be removed packet id checks from here
        // Empty dialogs don't help
        if (action == NpcAction::Close)
        {
            closeDialog(npcId);
            return npcId;
        }
        else if (action == NpcAction::Next)
        {
            nextDialog(npcId);
            return npcId;
        }
        else
        {
            CREATEWIDGETV(Ea::NpcRecv::mDialog, NpcDialog, npcId);
            Ea::NpcRecv::mDialog->saveCamera();
            if (localPlayer)
                localPlayer->stopWalking(false);
            NpcDialog::mNpcDialogs[npcId] = Ea::NpcRecv::mDialog;
        }
    }
    else
    {
        NpcDialog *const dialog = diag->second;
        if (Ea::NpcRecv::mDialog && Ea::NpcRecv::mDialog != dialog)
            Ea::NpcRecv::mDialog->restoreCamera();
        Ea::NpcRecv::mDialog = dialog;
        if (Ea::NpcRecv::mDialog)
            Ea::NpcRecv::mDialog->saveCamera();
    }
    return npcId;
}

}  // namespace TmwAthena
