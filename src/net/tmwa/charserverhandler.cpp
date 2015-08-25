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

#include "net/tmwa/charserverhandler.h"

#include "client.h"
#include "configuration.h"
#include "settings.h"

#include "gui/windows/charcreatedialog.h"
#include "gui/windows/okdialog.h"

#include "gui/widgets/createwidget.h"

#include "net/character.h"

#include "net/ea/token.h"

#include "net/ea/charserverrecv.h"

#include "net/tmwa/charserverrecv.h"
#include "net/tmwa/gamehandler.h"
#include "net/tmwa/loginhandler.h"
#include "net/tmwa/messageout.h"
#include "net/tmwa/network.h"
#include "net/tmwa/protocol.h"
#include "net/tmwa/sprite.h"

#include "resources/iteminfo.h"

#include "resources/db/chardb.h"
#include "resources/db/itemdb.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include "debug.h"

extern Net::CharServerHandler *charServerHandler;

namespace TmwAthena
{

extern ServerInfo mapServer;

extern ServerInfo charServer;

CharServerHandler::CharServerHandler() :
    MessageHandler(),
    Ea::CharServerHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_CHAR_LOGIN,
        SMSG_CHAR_LOGIN_ERROR,
        SMSG_CHAR_CREATE_SUCCEEDED,
        SMSG_CHAR_CREATE_FAILED,
        SMSG_CHAR_DELETE_SUCCEEDED,
        SMSG_CHAR_DELETE_FAILED,
        SMSG_CHAR_MAP_INFO,
        SMSG_CHANGE_MAP_SERVER,
        0
    };
    handledMessages = _messages;
    charServerHandler = this;
}

void CharServerHandler::handleMessage(Net::MessageIn &msg)
{
    BLOCK_START("CharServerHandler::handleMessage")
    switch (msg.getId())
    {
        case SMSG_CHAR_LOGIN:
            CharServerRecv::processCharLogin(msg);
            break;

        case SMSG_CHAR_LOGIN_ERROR:
            Ea::CharServerRecv::processCharLoginError(msg);
            break;

        case SMSG_CHAR_CREATE_SUCCEEDED:
            CharServerRecv::processCharCreate(msg);
            break;

        case SMSG_CHAR_CREATE_FAILED:
            Ea::CharServerRecv::processCharCreateFailed(msg);
            break;

        case SMSG_CHAR_DELETE_SUCCEEDED:
            Ea::CharServerRecv::processCharDelete(msg);
            break;

        case SMSG_CHAR_DELETE_FAILED:
            CharServerRecv::processCharDeleteFailed(msg);
            break;

        case SMSG_CHAR_MAP_INFO:
            CharServerRecv::processCharMapInfo(msg);
            break;

        case SMSG_CHANGE_MAP_SERVER:
            CharServerRecv::processChangeMapServer(msg);
            break;

        default:
            break;
    }
    BLOCK_END("CharServerHandler::handleMessage")
}

void CharServerHandler::chooseCharacter(Net::Character *const character)
{
    if (!character)
        return;

    mSelectedCharacter = character;
    mCharSelectDialog = nullptr;

    createOutPacket(CMSG_CHAR_SELECT);
    outMsg.writeInt8(static_cast<unsigned char>(mSelectedCharacter->slot),
        "slot");
}

void CharServerHandler::newCharacter(const std::string &name, const int slot,
                                     const GenderT gender A_UNUSED,
                                     const int hairstyle,
                                     const int hairColor,
                                     const unsigned char race A_UNUSED,
                                     const uint16_t look A_UNUSED,
                                     const std::vector<int> &stats) const
{
    createOutPacket(CMSG_CHAR_CREATE);
    outMsg.writeString(name, 24, "name");
    for (int i = 0; i < 6; i++)
        outMsg.writeInt8(static_cast<unsigned char>(stats[i]), "stat");

    outMsg.writeInt8(static_cast<unsigned char>(slot), "slot");
    outMsg.writeInt8(static_cast<int8_t>(hairColor), "hair color");
    outMsg.writeInt8(0, "unused");
    outMsg.writeInt8(static_cast<int8_t>(hairstyle), "hair style");
    outMsg.writeInt8(0, "unused");
}

void CharServerHandler::deleteCharacter(Net::Character *const character,
                                        const std::string &email A_UNUSED)
{
    if (!character)
        return;

    mSelectedCharacter = character;

    createOutPacket(CMSG_CHAR_DELETE);
    outMsg.writeBeingId(mSelectedCharacter->dummy->getId(), "id?");
    outMsg.writeString("a@a.com", 40, "email");
}

void CharServerHandler::switchCharacter() const
{
    // This is really a map-server packet
    createOutPacket(CMSG_PLAYER_RESTART);
    outMsg.writeInt8(1, "flag");
}

void CharServerHandler::connect()
{
    const Token &token =
        static_cast<LoginHandler*>(loginHandler)->getToken();

    if (!mNetwork)
        return;

    mNetwork->disconnect();
    mNetwork->connect(charServer);
    createOutPacket(CMSG_CHAR_SERVER_CONNECT);
    outMsg.writeBeingId(token.account_ID, "account id");
    outMsg.writeInt32(token.session_ID1, "session id1");
    outMsg.writeInt32(token.session_ID2, "session id2");
    // [Fate] The next word is unused by the old char server, so we squeeze in
    //        mana client version information
    if (serverVersion > 0)
    {
        outMsg.writeInt16(CLIENT_PROTOCOL_VERSION, "client protocol version");
    }
    else
    {
        outMsg.writeInt16(CLIENT_TMW_PROTOCOL_VERSION,
            "client protocol version");
    }
    outMsg.writeInt8(Being::genderToInt(token.sex), "gender");

    // We get 4 useless bytes before the real answer comes in (what are these?)
    mNetwork->skip(4);
}

void CharServerHandler::setCharCreateDialog(CharCreateDialog *const window)
{
    mCharCreateDialog = window;

    if (!mCharCreateDialog)
        return;

    StringVect attributes;
    // TRANSLATORS: playe stat
    attributes.push_back(_("Strength:"));
    // TRANSLATORS: playe stat
    attributes.push_back(_("Agility:"));
    // TRANSLATORS: playe stat
    attributes.push_back(_("Vitality:"));
    // TRANSLATORS: playe stat
    attributes.push_back(_("Intelligence:"));
    // TRANSLATORS: playe stat
    attributes.push_back(_("Dexterity:"));
    // TRANSLATORS: playe stat
    attributes.push_back(_("Luck:"));

    const Token &token = static_cast<LoginHandler*>(loginHandler)->getToken();

    int minStat = CharDB::getMinStat();
    if (!minStat)
        minStat = 1;
    int maxStat = CharDB::getMaxStat();
    if (!maxStat)
        maxStat = 9;
    int sumStat = CharDB::getSumStat();
    if (!sumStat)
        sumStat = 30;

    mCharCreateDialog->setAttributes(attributes, sumStat, minStat, maxStat);
    mCharCreateDialog->setDefaultGender(token.sex);
}

void CharServerHandler::renameCharacter(const BeingId id A_UNUSED,
                                        const std::string &newName A_UNUSED)
{
}

void CharServerHandler::changeSlot(const int oldSlot A_UNUSED,
                                   const int newSlot A_UNUSED)
{
}

void CharServerHandler::ping() const
{
}

unsigned int CharServerHandler::hatSprite() const
{
    return 7;
}

}  // namespace TmwAthena
