/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "gui/windows/charcreatedialog.h"

#include "net/character.h"
#include "net/serverfeatures.h"

#include "net/ea/token.h"

#include "net/eathena/charserverrecv.h"
#include "net/eathena/loginhandler.h"
#include "net/eathena/messageout.h"
#include "net/eathena/network.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

extern int packetVersion;
extern int serverVersion;

namespace EAthena
{

extern ServerInfo charServer;
extern ServerInfo mapServer;

CharServerHandler::CharServerHandler() :
    Ea::CharServerHandler()
{
    CharServerRecv::mNewName.clear();
    CharServerRecv::mRenameId = BeingId_zero;

    charServerHandler = this;
}

CharServerHandler::~CharServerHandler()
{
    charServerHandler = nullptr;
}

void CharServerHandler::chooseCharacter(Net::Character *const character) const
{
    if (character == nullptr)
        return;

    mSelectedCharacter = character;
    mCharSelectDialog = nullptr;

    createOutPacket(CMSG_CHAR_SELECT);
    outMsg.writeInt8(CAST_U8(
        mSelectedCharacter->slot), "slot");
}

void CharServerHandler::newCharacter(const std::string &name, const int slot,
                                     const GenderT gender,
                                     const int hairstyle, const int hairColor,
                                     const unsigned char race,
                                     const uint16_t look,
                                     const STD_VECTOR<int> &stats A_UNUSED)
                                     const
{
    createOutPacket(CMSG_CHAR_CREATE);
    outMsg.writeString(name, 24, "login");
    if (serverVersion > 0)
    {
        outMsg.writeInt8(CAST_U8(slot), "slot");
        outMsg.writeInt16(CAST_S16(hairColor), "hair color");
        outMsg.writeInt16(CAST_S16(hairstyle), "hair style");
        if (serverFeatures->haveRaceSelection())
            outMsg.writeInt16(CAST_S16(race), "race");
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
            outMsg.writeInt16(CAST_S16(look), "look");
    }
    else
    {
        if (packetVersion >= 20151001)
        {
            outMsg.writeInt8(CAST_U8(slot), "slot");
            outMsg.writeInt16(CAST_S16(hairColor), "hair color");
            outMsg.writeInt16(CAST_S16(hairstyle), "hair style");
            outMsg.writeInt32(CAST_S16(0), "starting job id");
            uint8_t sex = 0;
            if (gender == Gender::UNSPECIFIED)
                sex = 99;
            else
                sex = Being::genderToInt(gender);
            outMsg.writeInt8(sex, "gender");
        }
        else if (packetVersion >= 20120307)
        {
            outMsg.writeInt8(CAST_U8(slot), "slot");
            outMsg.writeInt16(CAST_S16(hairColor), "hair color");
            outMsg.writeInt16(CAST_S16(hairstyle), "hair style");
        }
        else
        {   // < 20120307
            // +++ here need send stat points
            // sending 5 for each stat for now
            for (int i = 0; i < 6; i++)
                outMsg.writeInt8(CAST_U8(5), "stat");

            outMsg.writeInt8(CAST_U8(slot), "slot");
            outMsg.writeInt16(CAST_S16(hairColor), "hair color");
            outMsg.writeInt16(CAST_S16(hairstyle), "hair style");
        }
    }
}

void CharServerHandler::deleteCharacter(Net::Character *const character,
                                        const std::string &email) const
{
    if (character == nullptr)
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

void CharServerHandler::connect() const
{
    const Token &token =
        static_cast<LoginHandler*>(loginHandler)->getToken();

    if (Network::mInstance == nullptr)
        return;

    Network::mInstance->disconnect();
    Network::mInstance->connect(charServer);
    createOutPacket(CMSG_CHAR_SERVER_CONNECT);
    outMsg.writeBeingId(token.account_ID, "account id");
    outMsg.writeInt32(token.session_ID1, "session id1");
    outMsg.writeInt32(token.session_ID2, "session id2");
    outMsg.writeInt16(CLIENT_PROTOCOL_VERSION, "client protocol version");
    outMsg.writeInt8(Being::genderToInt(token.sex), "gender");

    // We get 4 useless bytes before the real answer comes in (what are these?)
    Network::mInstance->skip(4);
}

void CharServerHandler::setCharCreateDialog(CharCreateDialog *const window)
                                            const
{
    mCharCreateDialog = window;

    if (mCharCreateDialog == nullptr)
        return;

    StringVect attributes;

    const Token &token = static_cast<LoginHandler*>(loginHandler)->getToken();

    mCharCreateDialog->setAttributes(attributes, 0, 0, 0);
    mCharCreateDialog->setDefaultGender(token.sex);
}

void CharServerHandler::setNewPincode(const BeingId accountId,
                                      const std::string &pin) const
{
    createOutPacket(CMSG_CHAR_CREATE_PIN);
    outMsg.writeBeingId(accountId, "account id");
    outMsg.writeString(pin, 4, "encrypted pin");
}

void CharServerHandler::sendCheckPincode(const BeingId accountId,
                                         const std::string &pin) const
{
    createOutPacket(CMSG_CHAR_PIN_CHECK);
    outMsg.writeBeingId(accountId, "account id");
    outMsg.writeString(pin, 4, "encrypted pin");
}

void CharServerHandler::changePincode(const BeingId accountId,
                                      const std::string &oldPin,
                                      const std::string &newPin) const
{
    createOutPacket(CMSG_CHAR_PIN_CHANGE);
    outMsg.writeBeingId(accountId, "account id");
    outMsg.writeString(oldPin, 4, "old encrypted pin");
    outMsg.writeString(newPin, 4, "new encrypted pin");
}

void CharServerHandler::renameCharacter(const BeingId id,
                                        const std::string &newName) const
{
    createOutPacket(CMSG_CHAR_CHECK_RENAME);
    CharServerRecv::mRenameId = id;
    CharServerRecv::mNewName = newName;
    outMsg.writeBeingId(id, "char id");
    outMsg.writeString(newName, 24, "name");
}

void CharServerHandler::changeSlot(const int oldSlot,
                                   const int newSlot) const
{
    createOutPacket(CMSG_CHAR_CHANGE_SLOT);
    outMsg.writeInt16(CAST_S16(oldSlot), "old slot");
    outMsg.writeInt16(CAST_S16(newSlot), "new slot");
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

}  // namespace EAthena
