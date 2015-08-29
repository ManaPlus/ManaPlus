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

#include "net/eathena/npchandler.h"

#include "being/localplayer.h"

#include "gui/windows/npcdialog.h"

#include "gui/widgets/createwidget.h"

#include "net/ea/npcrecv.h"

#include "net/eathena/messageout.h"
#include "net/eathena/npcrecv.h"
#include "net/eathena/protocol.h"

#include "net/ea/eaprotocol.h"

#include "debug.h"

extern Net::NpcHandler *npcHandler;

namespace EAthena
{

NpcHandler::NpcHandler() :
    MessageHandler(),
    Ea::NpcHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_NPC_CHOICE,
        SMSG_NPC_MESSAGE,
        SMSG_NPC_NEXT,
        SMSG_NPC_CLOSE,
        SMSG_NPC_INT_INPUT,
        SMSG_NPC_STR_INPUT,
        SMSG_NPC_CUTIN,
        SMSG_NPC_VIEWPOINT,
        SMSG_NPC_SHOW_PROGRESS_BAR,
        SMSG_NPC_CLOSE_TIMEOUT,
        SMSG_NPC_COMMAND,
        SMSG_NPC_CHANGETITLE,
        SMSG_NPC_AREA,
        SMSG_NPC_SHOW_DIGIT,
        SMSG_NPC_PROGRESS_BAR_ABORT,
        0
    };
    handledMessages = _messages;
    npcHandler = this;
}

void NpcHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_NPC_CHOICE:
            Ea::NpcRecv::processNpcChoice(msg);
            break;

        case SMSG_NPC_MESSAGE:
            Ea::NpcRecv::processNpcMessage(msg);
            break;

        case SMSG_NPC_CLOSE:
            Ea::NpcRecv::processNpcClose(msg);
            break;

        case SMSG_NPC_NEXT:
            Ea::NpcRecv::processNpcNext(msg);
            break;

        case SMSG_NPC_INT_INPUT:
            Ea::NpcRecv::processNpcIntInput(msg);
            break;

        case SMSG_NPC_STR_INPUT:
            Ea::NpcRecv::processNpcStrInput(msg);
            break;

        case SMSG_NPC_CUTIN:
            NpcRecv::processNpcCutin(msg);
            break;

        case SMSG_NPC_VIEWPOINT:
            NpcRecv::processNpcViewPoint(msg);
            break;

        case SMSG_NPC_SHOW_PROGRESS_BAR:
            NpcRecv::processNpcShowProgressBar(msg);
            break;

        case SMSG_NPC_CLOSE_TIMEOUT:
            NpcRecv::processNpcCloseTimeout(msg);
            break;

        case SMSG_NPC_COMMAND:
            Ea::NpcRecv::processNpcCommand(msg);
            break;

        case SMSG_NPC_CHANGETITLE:
            Ea::NpcRecv::processChangeTitle(msg);
            break;

        case SMSG_NPC_AREA:
            NpcRecv::processArea(msg);
            break;

        case SMSG_NPC_SHOW_DIGIT:
            NpcRecv::processShowDigit(msg);
            break;

        case SMSG_NPC_PROGRESS_BAR_ABORT:
            NpcRecv::processProgressBarAbort(msg);
            break;

        default:
            break;
    }

    Ea::NpcRecv::mDialog = nullptr;
}

void NpcHandler::talk(const BeingId npcId) const
{
    createOutPacket(CMSG_NPC_TALK);
    outMsg.writeBeingId(npcId, "npc id");
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
    outMsg.writeInt16(static_cast<int16_t>(value.length() + 9), "len");
    outMsg.writeBeingId(npcId, "npc id");
    outMsg.writeString(value, static_cast<int>(value.length()), "value");
    outMsg.writeInt8(0, "null byte");
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
    outMsg.writeInt16(static_cast<int16_t>(amount), "amount");
    outMsg.writeInt16(static_cast<int16_t>(itemId), "item id");
}

void NpcHandler::sellItem(const BeingId beingId A_UNUSED,
                          const int itemId, const int amount) const
{
    createOutPacket(CMSG_NPC_SELL_REQUEST);
    outMsg.writeInt16(8, "len");
    outMsg.writeInt16(static_cast<int16_t>(itemId + INVENTORY_OFFSET),
        "item id");
    outMsg.writeInt16(static_cast<int16_t>(amount), "amount");
}

void NpcHandler::completeProgressBar() const
{
    createOutPacket(CMSG_NPC_COMPLETE_PROGRESS_BAR);
}

void NpcHandler::produceMix(const int nameId,
                            const int materialId1,
                            const int materialId2,
                            const int materialId3) const
{
    createOutPacket(CMSG_NPC_PRODUCE_MIX);
    outMsg.writeInt16(static_cast<int16_t>(nameId), "name id");
    outMsg.writeInt16(static_cast<int16_t>(materialId1), "material 1");
    outMsg.writeInt16(static_cast<int16_t>(materialId2), "material 2");
    outMsg.writeInt16(static_cast<int16_t>(materialId3), "material 3");
}

void NpcHandler::cooking(const CookingTypeT type,
                         const int nameId) const
{
    createOutPacket(CMSG_NPC_COOKING);
    outMsg.writeInt16(static_cast<int16_t>(type), "type");
    outMsg.writeInt16(static_cast<int16_t>(nameId), "name id");
}

void NpcHandler::repair(const int index) const
{
    createOutPacket(CMSG_NPC_REPAIR);
    outMsg.writeInt16(static_cast<int16_t>(index), "index");
}

void NpcHandler::refine(const int index) const
{
    createOutPacket(CMSG_NPC_REFINE);
    outMsg.writeInt32(index, "index");
}

void NpcHandler::identify(const int index) const
{
    createOutPacket(CMSG_NPC_IDENTIFY);
    outMsg.writeInt16(static_cast<int16_t>(index), "index");
}

void NpcHandler::selectArrow(const int nameId) const
{
    createOutPacket(CMSG_NPC_SELECT_ARROW);
    outMsg.writeInt16(static_cast<int16_t>(nameId), "name id");
}

void NpcHandler::selectAutoSpell(const int skillId) const
{
    createOutPacket(CMSG_NPC_SELECT_AUTO_SPELL);
    outMsg.writeInt32(static_cast<int16_t>(skillId), "skill id");
}

BeingId NpcHandler::getNpc(Net::MessageIn &msg)
{
    if (msg.getId() == SMSG_NPC_CHOICE
        || msg.getId() == SMSG_NPC_MESSAGE
        || msg.getId() == SMSG_NPC_CHANGETITLE)
    {
        msg.readInt16("len");
    }

    const BeingId npcId = msg.readBeingId("npc id");

    const NpcDialogs::const_iterator diag = NpcDialog::mNpcDialogs.find(npcId);
    Ea::NpcRecv::mDialog = nullptr;

    if (msg.getId() == SMSG_NPC_VIEWPOINT)
        return npcId;

    if (diag == NpcDialog::mNpcDialogs.end())
    {
        // Empty dialogs don't help
        if (msg.getId() == SMSG_NPC_CLOSE)
        {
            closeDialog(npcId);
            return npcId;
        }
        else if (msg.getId() == SMSG_NPC_NEXT)
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

}  // namespace EAthena
