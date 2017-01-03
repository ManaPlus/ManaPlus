/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef NET_CHARSERVERHANDLER_H
#define NET_CHARSERVERHANDLER_H

#include "enums/being/gender.h"

#include "enums/simpletypes/beingid.h"

#include "net/characters.h"

#include <string>
#include <vector>

class CharCreateDialog;
class CharSelectDialog;

namespace Net
{

struct Character;

class CharServerHandler notfinal
{
    public:
        A_DELETE_COPY(CharServerHandler)

        virtual ~CharServerHandler()
        { }

        virtual void setCharSelectDialog(CharSelectDialog *const window)
                                         const = 0;

        virtual void setCharCreateDialog(CharCreateDialog *const window)
                                         const = 0;

        virtual void requestCharacters() const = 0;

        virtual void chooseCharacter(Net::Character *const character)
                                     const = 0;

        virtual void newCharacter(const std::string &name,
                                  const int slot,
                                  const GenderT gender,
                                  const int hairstyle,
                                  const int hairColor,
                                  const unsigned char race,
                                  const uint16_t look,
                                  const std::vector<int> &stats) const = 0;

        virtual void deleteCharacter(Net::Character *const character,
                                     const std::string &email) const = 0;

        virtual void renameCharacter(const BeingId id,
                                     const std::string &newName) const = 0;

        virtual void switchCharacter() const = 0;

        virtual unsigned int baseSprite() const A_WARN_UNUSED = 0;

        virtual unsigned int hairSprite() const A_WARN_UNUSED = 0;

        virtual unsigned int hatSprite() const A_WARN_UNUSED = 0;

        virtual unsigned int maxSprite() const A_WARN_UNUSED = 0;

        virtual void clear() const = 0;

        virtual bool isNeedCreatePin() const = 0;

        virtual void setNewPincode(const std::string &pin) const = 0;

        virtual void changeSlot(const int oldSlot,
                                const int newSlot) const = 0;

        virtual void ping() const = 0;

        /** The list of available characters. */
        static Net::Characters mCharacters;

        static CharSelectDialog *mCharSelectDialog;
        static CharCreateDialog *mCharCreateDialog;

        /** The selected character. */
        static Net::Character *mSelectedCharacter;

        static void updateCharSelectDialog();
        static void unlockCharSelectDialog();

    protected:
        CharServerHandler()
        {
            mCharacters.clear();
            mSelectedCharacter = nullptr;
            mCharSelectDialog = nullptr;
            mCharCreateDialog = nullptr;
        }
};

}  // namespace Net

extern Net::CharServerHandler *charServerHandler;

#endif  // NET_CHARSERVERHANDLER_H
