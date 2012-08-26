/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef CHAR_SELECT_H
#define CHAR_SELECT_H

#include "being.h"
#include "main.h"

#include "gui/widgets/window.h"

#include "net/charhandler.h"

#include <guichan/actionlistener.hpp>
#include <guichan/keylistener.hpp>

class CharacterDisplay;
class LocalPlayer;
class LoginData;
class PlayerBox;
class TextDialog;

namespace gcn
{
    class Button;
    class Label;
}

namespace Net
{
    class CharHandler;
}

/**
 * Character selection dialog.
 *
 * \ingroup Interface
 */
class CharSelectDialog : public Window,
                         public gcn::ActionListener,
                         public gcn::KeyListener
{
    public:
        friend class CharDeleteConfirm;
        friend class Net::CharHandler;

        /**
         * Constructor.
         */
        CharSelectDialog(LoginData *const data);

        ~CharSelectDialog();

        void action(const gcn::ActionEvent &event);

        void keyPressed(gcn::KeyEvent &keyEvent);

        enum SelectAction
        {
            Focus = 0,
            Choose
        };

        /**
         * Attempt to select the character with the given name. Returns whether
         * a character with the given name was found.
         *
         * \param action determines what to do when a character with the given
         *               name was found (just focus or also try to choose this
         *               character).
         */
        bool selectByName(const std::string &name,
                          const SelectAction action = Focus);

        void askPasswordForDeletion(const int index);

    private:
        void attemptCharacterDelete(const int index);

        void attemptCharacterSelect(const int index);

        void setCharacters(const Net::Characters &characters);

        void lock();
        void unlock();
        void setLocked(const bool locked);

        bool getFocusedContainer(int &container, int &idx);

        void setFocusedContainer(const int i, const int button);

        bool mLocked;

        gcn::Label *mAccountNameLabel;
        gcn::Label *mLastLoginLabel;

        gcn::Button *mSwitchLoginButton;
        gcn::Button *mChangePasswordButton;
        gcn::Button *mUnregisterButton;
        gcn::Button *mChangeEmailButton;

        /** The player boxes */
        std::vector<CharacterDisplay*> mCharacterEntries;

        LoginData *mLoginData;

        Net::CharHandler *mCharHandler;
        TextDialog *mDeleteDialog;
        int mDeleteIndex;
};

#endif
