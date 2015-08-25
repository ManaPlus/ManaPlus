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

#include "net/eathena/charserverhandler.h"

#include "client.h"
#include "configuration.h"
#include "settings.h"

#include "gui/windows/charcreatedialog.h"
#include "gui/windows/charselectdialog.h"
#include "gui/windows/okdialog.h"

#include "gui/widgets/createwidget.h"

#include "net/character.h"
#include "net/playerhandler.h"
#include "net/serverfeatures.h"

#include "net/ea/token.h"

#include "net/ea/charserverrecv.h"

#include "net/eathena/charserverrecv.h"
#include "net/eathena/gamehandler.h"
#include "net/eathena/loginhandler.h"
#include "net/eathena/messageout.h"
#include "net/eathena/network.h"
#include "net/eathena/protocol.h"
#include "net/eathena/sprite.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include "debug.h"

extern Net::CharServerHandler *charServerHandler;

namespace EAthena
{

extern ServerInfo charServer;
extern ServerInfo mapServer;

CharServerHandler::CharServerHandler() :
    MessageHandler(),
    Ea::CharServerHandler()
{
    CharServerRecv::mNewName.clear();
    CharServerRecv::mPinSeed = 0;
    CharServerRecv::mPinAccountId = BeingId_zero;
    CharServerRecv::mRenameId = BeingId_zero;
    CharServerRecv::mNeedCreatePin = false;

    static const uint16_t _messages[] =
    {
        SMSG_CHAR_LOGIN,
        SMSG_CHAR_LOGIN2,
        SMSG_CHAR_LOGIN_ERROR,
        SMSG_CHAR_CREATE_SUCCEEDED,
        SMSG_CHAR_CREATE_FAILED,
        SMSG_CHAR_DELETE_SUCCEEDED,
        SMSG_CHAR_DELETE_FAILED,
        SMSG_CHAR_MAP_INFO,
        SMSG_CHANGE_MAP_SERVER,
        SMSG_CHAR_PINCODE_STATUS,
        SMSG_CHAR_CHECK_RENAME,
        SMSG_CHAR_RENAME,
        SMSG_CHAR_CHANGE_SLOT,
        SMSG_CHAR_CAPTCHA_NOT_SUPPORTED,
        SMSG_CHAR_DELETE2_ACK,
        SMSG_CHAR_DELETE2_ACCEPT_ACTUAL_ACK,
        SMSG_CHAR_DELETE2_CANCEL_ACK,
        SMSG_CHAR_CHARACTERS,
        SMSG_CHAR_BAN_CHAR_LIST,
        0
    };
    handledMessages = _messages;
    charServerHandler = this;
}

void CharServerHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_CHAR_LOGIN:
            CharServerRecv::processCharLogin(msg);
            break;

        case SMSG_CHAR_LOGIN2:
            CharServerRecv::processCharLogin2(msg);
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

        case SMSG_CHAR_PINCODE_STATUS:
            CharServerRecv::processPincodeStatus(msg);
            break;

        case SMSG_CHAR_CHECK_RENAME:
            CharServerRecv::processCharCheckRename(msg);
            break;

        case SMSG_CHAR_RENAME:
            CharServerRecv::processCharRename(msg);
            break;

        case SMSG_CHAR_CHANGE_SLOT:
            CharServerRecv::processCharChangeSlot(msg);
            break;

        case SMSG_CHAR_CAPTCHA_NOT_SUPPORTED:
            CharServerRecv::processCharCaptchaNotSupported(msg);
            break;

        case SMSG_CHAR_DELETE2_ACK:
            CharServerRecv::processCharDelete2Ack(msg);
            break;

        case SMSG_CHAR_DELETE2_ACCEPT_ACTUAL_ACK:
            CharServerRecv::processCharDelete2AcceptActual(msg);
            break;

        case SMSG_CHAR_DELETE2_CANCEL_ACK:
            CharServerRecv::processCharDelete2CancelAck(msg);
            break;

        case SMSG_CHAR_CHARACTERS:
            CharServerRecv::processCharCharacters(msg);
            break;

        case SMSG_CHAR_BAN_CHAR_LIST:
            CharServerRecv::processCharBanCharList(msg);
            break;

        default:
            break;
    }
}

void CharServerHandler::chooseCharacter(Net::Character *const character)
{
    if (!character)
        return;

    mSelectedCharacter = character;
    mCharSelectDialog = nullptr;

    createOutPacket(CMSG_CHAR_SELECT);
    outMsg.writeInt8(static_cast<unsigned char>(
        mSelectedCharacter->slot), "slot");
}

void CharServerHandler::newCharacter(const std::string &name, const int slot,
                                     const GenderT gender,
                                     const int hairstyle, const int hairColor,
                                     const unsigned char race,
                                     const uint16_t look,
                                     const std::vector<int> &stats A_UNUSED)
                                     const
{
    createOutPacket(CMSG_CHAR_CREATE);
    outMsg.writeString(name, 24, "login");

    outMsg.writeInt8(static_cast<unsigned char>(slot), "slot");
    outMsg.writeInt16(static_cast<int16_t>(hairColor), "hair color");
    outMsg.writeInt16(static_cast<int16_t>(hairstyle), "hair style");
    if (serverFeatures->haveRaceSelection())
        outMsg.writeInt16(static_cast<int16_t>(race), "race");
    if (serverFeatures->haveCreateCharGender())
    {
        uint8_t sex = 0;
        if (gender == Gender::UNSPECIFIED)
            sex = 99;
        else
            sex = Being::genderToInt(gender);
        outMsg.writeInt8(sex, "gender");
    }
    if (serverFeatures->haveLookSelection())
        outMsg.writeInt16(static_cast<int16_t>(look), "look");
}

void CharServerHandler::deleteCharacter(Net::Character *const character,
                                        const std::string &email)
{
    if (!character)
        return;

    mSelectedCharacter = character;

    createOutPacket(CMSG_CHAR_DELETE);
    outMsg.writeBeingId(mSelectedCharacter->dummy->getId(), "id?");
    if (email.empty())
        outMsg.writeString("a@a.com", 40, "email");
    else
        outMsg.writeString(email, 40, "email");
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
    outMsg.writeInt16(CLIENT_PROTOCOL_VERSION, "client protocol version");
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

    const Token &token = static_cast<LoginHandler*>(loginHandler)->getToken();

    mCharCreateDialog->setAttributes(attributes, 0, 0, 0);
    mCharCreateDialog->setDefaultGender(token.sex);
}

void CharServerHandler::setNewPincode(const std::string &pin A_UNUSED)
{
//  here need ecript pin with mPinSeed and pin values.

//    createOutPacket(CMSG_CHAR_CREATE_PIN);
//    outMsg.writeBeingId(mPinAccountId, "account id");
//    outMsg.writeString(pin, 4, "encrypted pin");
}

void CharServerHandler::renameCharacter(const BeingId id,
                                        const std::string &newName)
{
    createOutPacket(CMSG_CHAR_CHECK_RENAME);
    CharServerRecv::mRenameId = id;
    CharServerRecv::mNewName = newName;
    outMsg.writeBeingId(id, "char id");
    outMsg.writeString(newName, 24, "name");
}

void CharServerHandler::changeSlot(const int oldSlot, const int newSlot)
{
    createOutPacket(CMSG_CHAR_CHANGE_SLOT);
    outMsg.writeInt16(static_cast<int16_t>(oldSlot), "old slot");
    outMsg.writeInt16(static_cast<int16_t>(newSlot), "new slot");
    outMsg.writeInt16(0, "unused");
}

void CharServerHandler::ping() const
{
    createOutPacket(CMSG_CHAR_PING);
    outMsg.writeInt32(0, "unused");
}

unsigned int CharServerHandler::hatSprite() const
{
    return 7;
}

bool CharServerHandler::isNeedCreatePin() const
{
    return CharServerRecv::mNeedCreatePin;
}

}  // namespace EAthena
