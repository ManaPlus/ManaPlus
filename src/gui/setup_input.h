/*
 *  Custom keyboard shortcuts configuration
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef GUI_SETUP_INPUT_H
#define GUI_SETUP_INPUT_H

#include "gui/widgets/setuptab.h"

#include <guichan/actionlistener.hpp>

#include <guichan/widgets/button.hpp>
#include <guichan/widgets/listbox.hpp>

#include <string>

class TabStrip;
struct SetupActionData;

class Setup_Input : public SetupTab
{
    public:
        /**
         * Constructor
         */
        Setup_Input();

        /**
         * Destructor
         */
        ~Setup_Input();

        void apply();

        void cancel();

        void action(const gcn::ActionEvent &event);

        /**
         * Get an update on the assigned key.
         */
        void refreshAssignedKey(int index);

        /**
         * The callback function when a new key has been pressed.
         */
        void newKeyCallback(int index);

        /**
         * Shorthand method to update all the keys.
         */
        void refreshKeys();

        /**
         * If a key function is unresolved, then this reverts it.
         */
        void keyUnresolved();

        int keyToSetupData(int index);

        std::string keyToString(int index);

    private:
        class KeyListModel *mKeyListModel;
        gcn::ListBox *mKeyList;

        gcn::Button *mAssignKeyButton;
        gcn::Button *mUnassignKeyButton;
        gcn::Button *mDefaultButton;
        gcn::Button *mResetKeysButton;
        TabStrip *mTabs;

        bool mKeySetting; /**< flag to check if key being set. */

        int *mActionDataSize;
};

#endif
