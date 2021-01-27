/*
 *  The ManaPlus Client
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef GUI_WIDGETS_TABS_SETUP_INPUT_H
#define GUI_WIDGETS_TABS_SETUP_INPUT_H

#include "enums/input/inputaction.h"

#include "gui/widgets/tabs/setuptab.h"

class Button;
class ListBox;
class KeyListModel;
class ScrollArea;
class TabStrip;

struct SetupActionData;

class Setup_Input final : public SetupTab
{
    public:
        /**
         * Constructor
         */
        explicit Setup_Input(const Widget2 *const widget);

        A_DELETE_COPY(Setup_Input)

        /**
         * Destructor
         */
        ~Setup_Input() override final;

        void apply() override final;

        void cancel() override final;

        void action(const ActionEvent &event) override final;

        /**
         * Get an update on the assigned key.
         */
        void refreshAssignedKey(const int index);

        /**
         * The callback function when a new key has been pressed.
         */
        void newKeyCallback(const InputActionT index);

        /**
         * Shorthand method to update all the keys.
         */
        void refreshKeys();

        /**
         * If a key function is unresolved, then this reverts it.
         */
        void keyUnresolved();

        int keyToSetupData(const InputActionT index) const A_WARN_UNUSED;

        std::string keyToString(const InputActionT index) const A_WARN_UNUSED;

    private:
        static void fixTranslations();

        static void fixTranslation(SetupActionData *const actionDatas,
                                   const InputActionT actionStart,
                                   const InputActionT actionEnd,
                                   const std::string &text) A_NONNULL(1);

        KeyListModel *mKeyListModel;
        ListBox *mKeyList;

        Button *mAssignKeyButton;
        Button *mUnassignKeyButton;
        Button *mDefaultButton;
        Button *mResetKeysButton;
        TabStrip *mTabs;
        ScrollArea *mScrollArea;
        bool mKeySetting; /**< flag to check if key being set. */
        int *mActionDataSize;
};

#endif  // GUI_WIDGETS_TABS_SETUP_INPUT_H
