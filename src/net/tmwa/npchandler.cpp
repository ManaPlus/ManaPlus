/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "gui/viewport.h"

#include "gui/windows/npcdialog.h"

#include "net/serverfeatures.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/protocol.h"

#include "net/ea/eaprotocol.h"

#include "debug.h"

extern Net::NpcHandler *npcHandler;

namespace TmwAthena
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
        SMSG_NPC_COMMAND,
        SMSG_NPC_CHANGETITLE,
        0
    };
    handledMessages = _messages;
    npcHandler = this;
}

void NpcHandler::handleMessage(Net::MessageIn &msg)
{
    BLOCK_START("NpcHandler::handleMessage")

    switch (msg.getId())
    {
        case SMSG_NPC_CHOICE:
            processNpcChoice(msg);
            break;

        case SMSG_NPC_MESSAGE:
            processNpcMessage(msg);
            break;

        case SMSG_NPC_CLOSE:
            processNpcClose(msg);
            break;

        case SMSG_NPC_NEXT:
            processNpcNext(msg);
            break;

        case SMSG_NPC_INT_INPUT:
            processNpcIntInput(msg);
            break;

        case SMSG_NPC_STR_INPUT:
            processNpcStrInput(msg);
            break;

        case SMSG_NPC_COMMAND:
            processNpcCommand(msg);
            break;

        case SMSG_NPC_CHANGETITLE:
            processChangeTitle(msg);
            break;

        default:
            break;
    }

    mDialog = nullptr;
    BLOCK_END("NpcHandler::handleMessage")
}

void NpcHandler::talk(const int npcId) const
{
    createOutPacket(CMSG_NPC_TALK);
    outMsg.writeInt32(npcId, "npc id");
    outMsg.writeInt8(0, "unused");
}

void NpcHandler::nextDialog(const int npcId) const
{
    createOutPacket(CMSG_NPC_NEXT_REQUEST);
    outMsg.writeInt32(npcId, "npc id");
}

void NpcHandler::closeDialog(const int npcId)
{
    createOutPacket(CMSG_NPC_CLOSE);
    outMsg.writeInt32(npcId, "npc id");

    const NpcDialogs::iterator it = NpcDialog::mNpcDialogs.find(npcId);
    if (it != NpcDialog::mNpcDialogs.end())
    {
        NpcDialog *const dialog = (*it).second;
        if (dialog)
            dialog->close();
        if (dialog == mDialog)
            mDialog = nullptr;
        NpcDialog::mNpcDialogs.erase(it);
    }
}

void NpcHandler::listInput(const int npcId, const unsigned char value) const
{
    createOutPacket(CMSG_NPC_LIST_CHOICE);
    outMsg.writeInt32(npcId, "npc id");
    outMsg.writeInt8(value, "value");
}

void NpcHandler::integerInput(const int npcId, const int value) const
{
    createOutPacket(CMSG_NPC_INT_RESPONSE);
    outMsg.writeInt32(npcId, "npc id");
    outMsg.writeInt32(value, "value");
}

void NpcHandler::stringInput(const int npcId, const std::string &value) const
{
    createOutPacket(CMSG_NPC_STR_RESPONSE);
    outMsg.writeInt16(static_cast<int16_t>(value.length() + 9), "len");
    outMsg.writeInt32(npcId, "npc id");
    outMsg.writeString(value, static_cast<int>(value.length()), "value");
    outMsg.writeInt8(0, "null byte");
}

void NpcHandler::buy(const int beingId) const
{
    createOutPacket(CMSG_NPC_BUY_SELL_REQUEST);
    outMsg.writeInt32(beingId, "npc id");
    outMsg.writeInt8(0, "action");
}

void NpcHandler::sell(const int beingId) const
{
    createOutPacket(CMSG_NPC_BUY_SELL_REQUEST);
    outMsg.writeInt32(beingId, "npc id");
    outMsg.writeInt8(1, "action");
}

void NpcHandler::buyItem(const int beingId A_UNUSED, const int itemId,
                         const unsigned char color, const int amount) const
{
    createOutPacket(CMSG_NPC_BUY_REQUEST);
    if (serverFeatures->haveItemColors())
    {
        outMsg.writeInt16(10, "len");
        outMsg.writeInt16(static_cast<int16_t>(amount), "amount");
        outMsg.writeInt16(static_cast<int16_t>(itemId), "item id");
        outMsg.writeInt8(color, "color");
        outMsg.writeInt8(0, "unused");
    }
    else
    {
        outMsg.writeInt16(8, "len");
        outMsg.writeInt16(static_cast<int16_t>(amount), "amount");
        outMsg.writeInt16(static_cast<int16_t>(itemId), "item id");
    }
}

void NpcHandler::sellItem(const int beingId A_UNUSED,
                          const int itemId,
                          const int amount) const
{
    createOutPacket(CMSG_NPC_SELL_REQUEST);
    outMsg.writeInt16(8, "len");
    outMsg.writeInt16(static_cast<int16_t>(
        itemId + INVENTORY_OFFSET), "index");
    outMsg.writeInt16(static_cast<int16_t>(amount), "amount");
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

void NpcHandler::cooking(const CookingType::Type type A_UNUSED,
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

int NpcHandler::getNpc(Net::MessageIn &msg)
{
    if (msg.getId() == SMSG_NPC_CHOICE
        || msg.getId() == SMSG_NPC_MESSAGE
        || msg.getId() == SMSG_NPC_CHANGETITLE)
    {
        msg.readInt16("len");
    }

    const int npcId = msg.readInt32("npc id");

    const NpcDialogs::const_iterator diag = NpcDialog::mNpcDialogs.find(npcId);
    mDialog = nullptr;

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
            mDialog = new NpcDialog(npcId);
            mDialog->postInit();
            mDialog->saveCamera();
            if (localPlayer)
                localPlayer->stopWalking(false);
            NpcDialog::mNpcDialogs[npcId] = mDialog;
        }
    }
    else
    {
        NpcDialog *const dialog = diag->second;
        if (mDialog && mDialog != dialog)
            mDialog->restoreCamera();
        mDialog = dialog;
        if (mDialog)
            mDialog->saveCamera();
    }
    return npcId;
}

void NpcHandler::processNpcCommand(Net::MessageIn &msg)
{
    const int npcId = npcHandler->getNpc(msg);
    mRequestLang = false;

    const int cmd = msg.readInt16("cmd");
    switch (cmd)
    {
        case 0:
            mRequestLang = true;
            break;

        case 1:
            if (viewport)
                viewport->moveCameraToActor(npcId);
            break;

        case 2:
            if (viewport)
            {
                const int id = msg.readInt32("id");
                const int x = msg.readInt16("x");
                const int y = msg.readInt16("y");
                if (!id)
                    viewport->moveCameraToPosition(x, y);
                else
                    viewport->moveCameraToActor(id, x, y);
            }
            break;

        case 3:
            if (viewport)
                viewport->returnCamera();
            break;

        case 4:
            if (viewport)
            {
                msg.readInt32("id");
                const int x = msg.readInt16("x");
                const int y = msg.readInt16("y");
                viewport->moveCameraRelative(x, y);
            }
            break;
        case 5:  // close dialog
            npcHandler->closeDialog(npcId);
            break;
        case 6:  // show avatar
            if (mDialog)
            {
                mDialog->showAvatar(static_cast<uint16_t>(
                    msg.readInt32("avatar id")));
            }
            break;
        case 7:  // set avatar direction
            if (mDialog)
            {
                mDialog->setAvatarDirection(
                    Net::MessageIn::fromServerDirection(
                    static_cast<uint8_t>(msg.readInt32("avatar direction"))));
            }
            break;
        case 8:  // set avatar action
            if (mDialog)
                mDialog->setAvatarAction(msg.readInt32("avatar action"));
            break;
        case 9:  // clear npc dialog
            if (mDialog)
                mDialog->clearRows();
            break;
        case 10:  // send selected item id
        {
            int invSize = msg.readInt32("npc inventory size");
            if (!invSize)
                invSize = 1;
            if (mDialog)
                mDialog->itemRequest(invSize);
            break;
        }
        default:
            logger->log("unknown npc command: %d", cmd);
            break;
    }
}

void NpcHandler::processChangeTitle(Net::MessageIn &msg)
{
    npcHandler->getNpc(msg);
    mRequestLang = false;
    const std::string str = msg.readString(-1, "title");
    if (mDialog)
        mDialog->setCaption(str);
}

}  // namespace TmwAthena
