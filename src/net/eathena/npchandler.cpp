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

#include "net/eathena/npchandler.h"

#include "being/localplayer.h"

#include "gui/windows/npcdialog.h"

#include "net/eathena/messageout.h"
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
        0
    };
    handledMessages = _messages;
    npcHandler = this;
}

void NpcHandler::handleMessage(Net::MessageIn &msg)
{
    int npcId = 0;

    switch (msg.getId())
    {
        case SMSG_NPC_CHOICE:
            getNpc(msg, true);
            processNpcChoice(msg);
            break;

        case SMSG_NPC_MESSAGE:
            getNpc(msg, true);
            processNpcMessage(msg);
            break;

        case SMSG_NPC_CLOSE:
            getNpc(msg, false);
            processNpcClose(msg);
            break;

        case SMSG_NPC_NEXT:
            getNpc(msg, false);
            processNpcNext(msg);
            break;

        case SMSG_NPC_INT_INPUT:
            getNpc(msg, false);
            processNpcIntInput(msg);
            break;

        case SMSG_NPC_STR_INPUT:
            getNpc(msg, false);
            processNpcStrInput(msg);
            break;

        case SMSG_NPC_CUTIN:
            processNpcCutin(msg, npcId);
            break;

        case SMSG_NPC_VIEWPOINT:
            getNpc(msg, true);
            processNpcViewPoint(msg, npcId);
            break;

        default:
            break;
    }

    mDialog = nullptr;
}

void NpcHandler::talk(const int npcId) const
{
    MessageOut outMsg(CMSG_NPC_TALK);
    outMsg.writeInt32(npcId);
    outMsg.writeInt8(0);  // unused
}

void NpcHandler::nextDialog(const int npcId) const
{
    MessageOut outMsg(CMSG_NPC_NEXT_REQUEST);
    outMsg.writeInt32(npcId);
}

void NpcHandler::closeDialog(const int npcId)
{
    MessageOut outMsg(CMSG_NPC_CLOSE);
    outMsg.writeInt32(npcId);

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
    MessageOut outMsg(CMSG_NPC_LIST_CHOICE);
    outMsg.writeInt32(npcId);
    outMsg.writeInt8(value);
}

void NpcHandler::integerInput(const int npcId, const int value) const
{
    MessageOut outMsg(CMSG_NPC_INT_RESPONSE);
    outMsg.writeInt32(npcId);
    outMsg.writeInt32(value);
}

void NpcHandler::stringInput(const int npcId, const std::string &value) const
{
    MessageOut outMsg(CMSG_NPC_STR_RESPONSE);
    outMsg.writeInt16(static_cast<int16_t>(value.length() + 9));
    outMsg.writeInt32(npcId);
    outMsg.writeString(value, static_cast<int>(value.length()));
    outMsg.writeInt8(0);  // Prevent problems with string reading
}

void NpcHandler::buy(const int beingId) const
{
    MessageOut outMsg(CMSG_NPC_BUY_SELL_REQUEST);
    outMsg.writeInt32(beingId);
    outMsg.writeInt8(0);  // Buy
}

void NpcHandler::sell(const int beingId) const
{
    MessageOut outMsg(CMSG_NPC_BUY_SELL_REQUEST);
    outMsg.writeInt32(beingId);
    outMsg.writeInt8(1);  // Sell
}

void NpcHandler::buyItem(const int beingId A_UNUSED, const int itemId,
                         const unsigned char color A_UNUSED,
                         const int amount) const
{
    MessageOut outMsg(CMSG_NPC_BUY_REQUEST);
    outMsg.writeInt16(8);  // One item (length of packet)
    outMsg.writeInt16(static_cast<int16_t>(amount));
    outMsg.writeInt16(static_cast<int16_t>(itemId));
}

void NpcHandler::sellItem(const int beingId A_UNUSED,
                          const int itemId, const int amount) const
{
    MessageOut outMsg(CMSG_NPC_SELL_REQUEST);
    outMsg.writeInt16(8);  // One item (length of packet)
    outMsg.writeInt16(static_cast<int16_t>(itemId + INVENTORY_OFFSET));
    outMsg.writeInt16(static_cast<int16_t>(amount));
}

int NpcHandler::getNpc(Net::MessageIn &msg, const bool haveLength)
{
    if (haveLength)
        msg.readInt16();  // length

    const int npcId = msg.readInt32();

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
            if (player_node)
                player_node->stopWalking(false);
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

void NpcHandler::processNpcCutin(Net::MessageIn &msg A_UNUSED,
                                 const int npcId A_UNUSED)
{
    msg.readString(64);  // image name
    msg.readInt8();      // type
}

void NpcHandler::processNpcViewPoint(Net::MessageIn &msg A_UNUSED,
                                     const int npcId A_UNUSED)
{
    msg.readInt32();  // type
    msg.readInt32();  // x
    msg.readInt32();  // y
    msg.readInt8();   // byte
    msg.readInt32();  // color
}

}  // namespace EAthena
