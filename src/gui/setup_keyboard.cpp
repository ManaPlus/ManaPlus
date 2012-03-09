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

#include "gui/setup_keyboard.h"

#include "keyboardconfig.h"

#include "gui/okdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <guichan/listmodel.hpp>

#include <SDL_keyboard.h>

#include "debug.h"

/**
 * The list model for key function list.
 *
 * \ingroup Interface
 */
class KeyListModel : public gcn::ListModel
{
    public:
        /**
         * Returns the number of elements in container.
         */
        int getNumberOfElements()
        { return keyboard.KEY_TOTAL; }

        /**
         * Returns element from container.
         */
        std::string getElementAt(int i)
        { return mKeyFunctions[i]; }

        /**
         * Sets element from container.
         */
        void setElementAt(int i, const std::string &caption)
        { mKeyFunctions[i] = caption; }

    private:
        std::string mKeyFunctions[KeyboardConfig::KEY_TOTAL];
};

Setup_Keyboard::Setup_Keyboard():
    mKeyListModel(new KeyListModel),
    mKeyList(new ListBox(mKeyListModel)),
    mKeySetting(false)
{
    keyboard.setSetupKeyboard(this);
    setName(_("Keyboard"));

    refreshKeys();

    mKeyList->addActionListener(this);

    ScrollArea *scrollArea = new ScrollArea(mKeyList);
    scrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

    mAssignKeyButton = new Button(_("Assign"), "assign", this);
    mAssignKeyButton->addActionListener(this);
    mAssignKeyButton->setEnabled(false);

    mUnassignKeyButton = new Button(_("Unassign"), "unassign", this);
    mUnassignKeyButton->addActionListener(this);
    mUnassignKeyButton->setEnabled(false);

    mMakeDefaultButton = new Button(_("Default"), "makeDefault", this);
    mMakeDefaultButton->addActionListener(this);

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, scrollArea, 4, 6).setPadding(2);
    place(0, 6, mMakeDefaultButton);
    place(2, 6, mAssignKeyButton);
    place(3, 6, mUnassignKeyButton);

    setDimension(gcn::Rectangle(0, 0, 500, 350));
}

Setup_Keyboard::~Setup_Keyboard()
{
    delete mKeyList;
    mKeyList = nullptr;
    delete mKeyListModel;
    mKeyListModel = nullptr;

    delete mAssignKeyButton;
    mAssignKeyButton = nullptr;
    delete mUnassignKeyButton;
    mUnassignKeyButton = nullptr;
    delete mMakeDefaultButton;
    mMakeDefaultButton = nullptr;
}

void Setup_Keyboard::apply()
{
    keyUnresolved();

    if (keyboard.hasConflicts())
    {
        new OkDialog(_("Key Conflict(s) Detected."),
            keyboard.getBindError(), DIALOG_ERROR);
    }
    keyboard.setEnabled(true);
    keyboard.store();
}

void Setup_Keyboard::cancel()
{
    keyUnresolved();

    keyboard.retrieve();
    keyboard.setEnabled(true);

    refreshKeys();
}

void Setup_Keyboard::action(const gcn::ActionEvent &event)
{
    if (event.getSource() == mKeyList)
    {
        if (!mKeySetting)
        {
            int i(mKeyList->getSelected());
            if (keyboard.isSeparator(i))
            {
                mAssignKeyButton->setEnabled(false);
                mUnassignKeyButton->setEnabled(false);
            }
            else
            {
                mAssignKeyButton->setEnabled(true);
                mUnassignKeyButton->setEnabled(true);
            }
        }
    }
    else if (event.getId() == "assign")
    {
        mKeySetting = true;
        mAssignKeyButton->setEnabled(false);
        keyboard.setEnabled(false);
        int i(mKeyList->getSelected());
        keyboard.setNewKeyIndex(i);
        mKeyListModel->setElementAt(i, keyboard.getKeyCaption(i) + ": ?");
    }
    else if (event.getId() == "unassign")
    {
        int i(mKeyList->getSelected());
        keyboard.setNewKeyIndex(i);
        refreshAssignedKey(mKeyList->getSelected());
        keyboard.unassignKey();
        mAssignKeyButton->setEnabled(true);
    }
    else if (event.getId() == "makeDefault")
    {
        keyboard.makeDefault();
        refreshKeys();
    }
}

void Setup_Keyboard::refreshAssignedKey(int index)
{
    if (keyboard.isSeparator(index))
    {
        const std::string str = " \342\200\225\342\200\225\342\200\225"
            "\342\200\225\342\200\225 ";
        mKeyListModel->setElementAt(index, str
            + keyboard.getKeyCaption(index) + str);
    }
    else
    {
        std::string caption;
        char *temp = SDL_GetKeyName(
            static_cast<SDLKey>(keyboard.getKeyValue(index)));
        caption = keyboard.getKeyCaption(index) + ": " + toString(temp);
        mKeyListModel->setElementAt(index, caption);
    }
}

void Setup_Keyboard::newKeyCallback(int index)
{
    mKeySetting = false;
    refreshAssignedKey(index);
    mAssignKeyButton->setEnabled(true);
}

void Setup_Keyboard::refreshKeys()
{
    for (int i = 0; i < keyboard.KEY_TOTAL; i++)
        refreshAssignedKey(i);
}

void Setup_Keyboard::keyUnresolved()
{
    if (mKeySetting)
    {
        newKeyCallback(keyboard.getNewKeyIndex());
        keyboard.setNewKeyIndex(keyboard.KEY_NO_VALUE);
    }
}
