/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "net/charserverhandler.h"

#include <guichan/actionlistener.hpp>
#include <guichan/keylistener.hpp>

class Button;
class CharacterDisplay;
class CharacterViewBase;
class Label;
class LocalPlayer;
class LoginData;
class PlayerBox;
class TextDialog;

namespace Net
{
    class CharServerHandler;
}

/**
 * Character selection dialog.
 *
 * \ingroup Interface
 */
class CharSelectDialog final : public Window,
                               public gcn::ActionListener,
                               public gcn::KeyListener
{
    public:
        friend class CharDeleteConfirm;
        friend class Net::CharServerHandler;

        /**
         * Constructor.
         */
        explicit CharSelectDialog(LoginData *const data);

        A_DELETE_COPY(CharSelectDialog)

        ~CharSelectDialog();

        void action(const gcn::ActionEvent &event) override;

        void keyPressed(gcn::KeyEvent &keyEvent) override;

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

        void close() override;

        void widgetResized(const gcn::Event &event) override;

        void updateState();

    private:
        void attemptCharacterDelete(const int index);

        void attemptCharacterSelect(const int index);

        void setCharacters(const Net::Characters &characters);

        void use(const int selected);

        void lock();
        void unlock();
        void setLocked(const bool locked);

        LoginData *mLoginData;

        Button *mSwitchLoginButton;
        Button *mChangePasswordButton;
        Button *mUnregisterButton;
        Button *mChangeEmailButton;
        Button *mPlayButton;
        Button *mInfoButton;
        Button *mDeleteButton;
        CharacterViewBase *mCharacterView;

        std::vector<CharacterDisplay*> mCharacterEntries;

        Net::CharServerHandler *mCharServerHandler;
        TextDialog *mDeleteDialog;
        int mDeleteIndex;
        bool mLocked;
        bool mSmallScreen;
};

#endif
