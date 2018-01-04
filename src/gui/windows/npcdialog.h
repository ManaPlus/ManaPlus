/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "enums/simpletypes/beingid.h"
#include "enums/simpletypes/beingtypeid.h"

#include "enums/gui/npcactionstate.h"
#include "enums/gui/npcinputstate.h"

#include "gui/models/extendedlistmodel.h"

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"

class Being;
class Button;
class BrowserBox;
class ComplexInventory;
class ComplexItem;
class Container;
class ExtendedListBox;
class Item;
class ItemLinkHandler;
class Inventory;
class IntTextField;
class ItemContainer;
class NpcDialog;
class PlayerBox;
class ScrollArea;
class TextField;

struct NpcDialogInfo;

typedef std::map<BeingId, NpcDialog*> NpcDialogs;

/**
 * The npc dialog.
 *
 * \ingroup Interface
 */
class NpcDialog final : public Window,
                        public ActionListener,
                        public ExtendedListModel,
                        public ConfigListener
{
    public:
        /**
         * Constructor.
         *
         * @see Window::Window
         */
        explicit NpcDialog(const BeingId npcId);

        A_DELETE_COPY(NpcDialog)

        ~NpcDialog() override final;

        void postInit() override final;

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override final;

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
        void addText(const std::string &string,
                     const bool save);

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
        void textRequest(const std::string &defaultText);

        bool isInputFocused() const A_WARN_UNUSED;

        bool isTextInputFocused() const A_WARN_UNUSED;

        static bool isAnyInputFocused() A_WARN_UNUSED;

        /**
         * Requests a interger from the user.
         */
        void integerRequest(const int defaultValue,
                            const int min,
                            const int max);

        void itemRequest(const int size);

        void itemIndexRequest(const int size);

        void itemCraftRequest(const int size);

        void move(const int amount);

        void setVisible(Visible visible) override final;

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

        void showAvatar(const BeingTypeId avatarId);

        void setAvatarDirection(const uint8_t direction);

        void setAvatarAction(const int actionId);

        void logic() override final;

        void clearRows();

        void mousePressed(MouseEvent &event) override final;

        int isCloseState() const
        { return static_cast<int>(mActionState == NpcActionState::CLOSE); }

        void setSkin(const std::string &skin);

        void addCraftItem(Item *const item,
                          const int amount,
                          const int slot);

        NpcInputStateT getInputState()
        { return mInputState; }

        void copyToClipboard(const int x, const int y) const;

        static NpcDialogs mNpcDialogs;

        static void clearDialogs();

    private:
        typedef std::list<NpcDialog*> DialogList;
        static DialogList instances;

        void buildLayout();

        void placeNormalControls();

        void placeMenuControls();

        void placeSkinControls();

        void placeTextInputControls();

        void placeIntInputControls();

        void placeItemInputControls();

        void createSkinControls();

        void deleteSkinControls();

        void restoreVirtuals();

        std::string complexItemToStr(const ComplexItem *const item);

        BeingId mNpcId;

        int mDefaultInt;
        std::string mDefaultString;

        // Used for the main input area
        BrowserBox *mTextBox A_NONNULLPOINTER;
        ScrollArea *mScrollArea A_NONNULLPOINTER;
        std::string mText;
        std::string mNewText;

        // Used for choice input
        StringVect mItems;
        STD_VECTOR<Image *> mImages;
        ExtendedListBox *mItemList A_NONNULLPOINTER;
        ScrollArea *mListScrollArea A_NONNULLPOINTER;
        Container *mSkinContainer A_NONNULLPOINTER;
        ScrollArea *mSkinScrollArea A_NONNULLPOINTER;
        ItemLinkHandler *mItemLinkHandler A_NONNULLPOINTER;

        // Used for string and integer input
        TextField *mTextField A_NONNULLPOINTER;
        IntTextField *mIntField A_NONNULLPOINTER;
        Button *mPlusButton A_NONNULLPOINTER;
        Button *mMinusButton A_NONNULLPOINTER;
        Button *mClearButton A_NONNULLPOINTER;

        // Used for the button
        Button *mButton A_NONNULLPOINTER;
        Button *mButton2 A_NONNULLPOINTER;
        Button *mButton3 A_NONNULLPOINTER;

        // Will reset the text and integer input to the provided default
        Button *mResetButton A_NONNULLPOINTER;

        Inventory *mInventory A_NONNULLPOINTER;
        ComplexInventory *mComplexInventory A_NONNULLPOINTER;
        ItemContainer *mItemContainer A_NONNULLPOINTER;
        ScrollArea *mItemScrollArea A_NONNULLPOINTER;

        NpcInputStateT mInputState;
        NpcActionStateT mActionState;
        STD_VECTOR<Widget*> mSkinControls;
        std::string mSkinName;
        PlayerBox *mPlayerBox A_NONNULLPOINTER;
        Being *mAvatarBeing;
        const NpcDialogInfo *mDialogInfo;
        int mLastNextTime;
        int mCameraMode;
        int mCameraX;
        int mCameraY;
        bool mShowAvatar;
        bool mLogInteraction;
};

#endif  // GUI_WINDOWS_NPCDIALOG_H
