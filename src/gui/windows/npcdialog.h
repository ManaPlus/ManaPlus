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

#ifndef GUI_WINDOWS_NPCDIALOG_H
#define GUI_WINDOWS_NPCDIALOG_H

#include "configlistener.h"

#include "gui/widgets/extendedlistmodel.h"
#include "gui/widgets/window.h"

#include "utils/stringvector.h"

#include <guichan/actionlistener.hpp>

#include <list>

class Being;
class Button;
class BrowserBox;
class ExtendedListBox;
class ItemLinkHandler;
class Inventory;
class IntTextField;
class ItemContainer;
class NpcDialog;
class PlayerBox;
class ScrollArea;
class TextBox;
class TextField;

typedef std::map<int, NpcDialog*> NpcDialogs;

/**
 * The npc dialog.
 *
 * \ingroup Interface
 */
class NpcDialog final : public Window,
                        public gcn::ActionListener,
                        public ExtendedListModel,
                        public ConfigListener
{
    public:
        /**
         * Constructor.
         *
         * @see Window::Window
         */
        explicit NpcDialog(const int npcId);

        A_DELETE_COPY(NpcDialog)

        ~NpcDialog();

        void postInit() override final;

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const gcn::ActionEvent &event) override final;

        /**
        * Sets the text shows in the dialog.
        *
        * @param string The new text.
        */
//        void setText(const std::string &string);

        /**
         * Adds the text to the text shows in the dialog. Also adds a newline
         * to the end.
         *
         * @param string The text to add.
         */
        void addText(const std::string &string, const bool save = true);

        /**
         * When called, the widget will show a "Next" button.
         */
        void showNextButton();

        /**
         * When called, the widget will show a "Close" button and will close
         * the dialog when clicked.
         */
        void showCloseButton();

        /**
         * Notifies the server that client has performed a next action.
         */
        void nextDialog();

        /**
         * Notifies the server that the client has performed a close action.
         */
        void closeDialog();

        /**
         * Returns the number of items in the choices list.
         */
        int getNumberOfElements() override final A_WARN_UNUSED;

        /**
         * Returns the name of item number i of the choices list.
         */
        std::string getElementAt(int i) override final A_WARN_UNUSED;

        /**
         * Returns the image of item number i of the choices list.
         */
        const Image *getImageAt(int i) override final A_WARN_UNUSED;

        /**
         * Makes this dialog request a choice selection from the user.
         */
        void choiceRequest();

        /**
         * Adds a choice to the list box.
         */
        void addChoice(const std::string &);

        /**
          * Fills the options list for an NPC dialog.
          *
          * @param itemString A string with the options separated with colons.
          */
        void parseListItems(const std::string &itemString);

        /**
         * Requests a text string from the user.
         */
        void textRequest(const std::string &defaultText = "");

        bool isInputFocused() const A_WARN_UNUSED;

        bool isTextInputFocused() const A_WARN_UNUSED;

        static bool isAnyInputFocused() A_WARN_UNUSED;

        /**
         * Requests a interger from the user.
         */
        void integerRequest(const int defaultValue = 0, const int min = 0,
                            const int max = 2147483647);

        void itemRequest(const int size);

        void move(const int amount);

        void setVisible(bool visible) override final;

        void optionChanged(const std::string &name) override final;

        /**
         * Returns true if any instances exist.
         */
        static bool isActive() A_WARN_UNUSED
        { return !instances.empty(); }

        /**
         * Returns the first active instance. Useful for pushing user
         * interaction.
         */
        static NpcDialog *getActive() A_WARN_UNUSED;

        /**
         * Closes all instances.
         */
        static void closeAll();

        /**
         * Closes all instances and destroy also net handler dialogs.
         */
        static void destroyAll();

        void saveCamera();

        void restoreCamera();

        void refocus();

        void showAvatar(const uint16_t avatarId);

        void setAvatarDirection(const uint8_t direction);

        void setAvatarAction(const int actionId);

        void logic() override final;

        void clearRows();

        void mousePressed(gcn::MouseEvent &event);

        static void copyToClipboard(const int npcId, const int x, const int y);

        static NpcDialogs mNpcDialogs;

        static void clearDialogs();

    private:
        typedef std::list<NpcDialog*> DialogList;
        static DialogList instances;

        void buildLayout();

        void placeNormalControls();

        void placeMenuControls();

        void placeTextInputControls();

        void placeIntInputControls();

        void placeItemInputControls();

        int mNpcId;

        int mDefaultInt;
        std::string mDefaultString;

        // Used for the main input area
        BrowserBox *mTextBox;
        ScrollArea *mScrollArea;
        std::string mText;
        std::string mNewText;

        // Used for choice input
        ExtendedListBox *mItemList;
        ScrollArea *mListScrollArea;
        StringVect mItems;
        std::vector<Image *> mImages;
        ItemLinkHandler *mItemLinkHandler;

        // Used for string and integer input
        TextField *mTextField;
        IntTextField *mIntField;
        Button *mPlusButton;
        Button *mMinusButton;
        Button *mClearButton;

        // Used for the button
        Button *mButton;
        Button *mButton2;
        Button *mButton3;

        // Will reset the text and integer input to the provided default
        Button *mResetButton;

        Inventory *mInventory;
        ItemContainer *mItemContainer;
        ScrollArea *mItemScrollArea;

        enum NpcInputState
        {
            NPC_INPUT_NONE = 0,
            NPC_INPUT_LIST,
            NPC_INPUT_STRING,
            NPC_INPUT_INTEGER,
            NPC_INPUT_ITEM
        };

        enum NpcActionState
        {
            NPC_ACTION_WAIT = 0,
            NPC_ACTION_NEXT,
            NPC_ACTION_INPUT,
            NPC_ACTION_CLOSE
        };

        NpcInputState mInputState;
        NpcActionState mActionState;
        int mLastNextTime;
        int mCameraMode;
        int mCameraX;
        int mCameraY;
        PlayerBox *mPlayerBox;
        Being *mAvatarBeing;
        bool mShowAvatar;
        bool mLogInteraction;
};

#endif  // GUI_WINDOWS_NPCDIALOG_H
