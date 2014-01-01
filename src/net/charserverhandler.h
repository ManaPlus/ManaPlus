/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef NET_CHARSERVERHANDLER_H
#define NET_CHARSERVERHANDLER_H

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include <string>
#include <vector>

class CharCreateDialog;
class CharSelectDialog;

namespace Net
{

/**
 * A structure to hold information about a character.
 */
struct Character final
{
    Character() :
        slot(0),
        dummy(nullptr),
        data()
    {
    }

    A_DELETE_COPY(Character)

    ~Character()
    {
        delete dummy;
        dummy = nullptr;
    }

    int slot;            /**< The index in the list of characters */
    LocalPlayer *dummy;  /**< A dummy representing this character */
    PlayerInfoBackend data;
};

typedef std::list<Character*> Characters;

class CharServerHandler
{
    public:
        A_DELETE_COPY(CharServerHandler)

        virtual ~CharServerHandler()
        { }

        virtual void setCharSelectDialog(CharSelectDialog *const window) = 0;

        virtual void setCharCreateDialog(CharCreateDialog *const window) = 0;

        virtual void requestCharacters() = 0;

        virtual void chooseCharacter(Net::Character *const character) = 0;

        virtual void newCharacter(const std::string &name, const int slot,
                                  const bool gender, const int hairstyle,
                                  const int hairColor,
                                  const unsigned char race,
                                  const unsigned char look,
                                  const std::vector<int> &stats) const = 0;

        virtual void deleteCharacter(Net::Character *const character) = 0;

        virtual void switchCharacter() const = 0;

        virtual unsigned int baseSprite() const A_WARN_UNUSED = 0;

        virtual unsigned int hairSprite() const A_WARN_UNUSED = 0;

        virtual unsigned int maxSprite() const A_WARN_UNUSED = 0;

        virtual void clear() = 0;

    protected:
        CharServerHandler() :
            mCharacters(),
            mSelectedCharacter(nullptr),
            mCharSelectDialog(nullptr),
            mCharCreateDialog(nullptr)
        {}

        void updateCharSelectDialog();
        void unlockCharSelectDialog();

        /** The list of available characters. */
        Net::Characters mCharacters;

        /** The selected character. */
        Net::Character *mSelectedCharacter;

        CharSelectDialog *mCharSelectDialog;
        CharCreateDialog *mCharCreateDialog;
};

}  // namespace Net

#endif  // NET_CHARSERVERHANDLER_H
