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

#include "gui/setup_input.h"

#include "configuration.h"
#include "inputmanager.h"
#include "keyboardconfig.h"
#include "logger.h"

#include "gui/okdialog.h"
#include "gui/setupactiondata.h"

#include "gui/widgets/button.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/tabstrip.h"

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
        KeyListModel() :
            gcn::ListModel(),
            mDataNum(0),
            mSize(0)
        {
        }

        /**
         * Returns the number of elements in container.
         */
        int getNumberOfElements()
        { return mSize; }

        /**
         * Returns element from container.
         */
        std::string getElementAt(int i)
        { return setupActionData[selectedData][i].text; }

        /**
         * Sets element from container.
         */
        void setElementAt(int i, const std::string &caption)
        { setupActionData[selectedData][i].text = caption; }

        void setSize(int size)
        { mSize = size; }

        void setDataNum(int num)
        { mDataNum = num; }

    private:
        int mDataNum;
        int mSize;
};

Setup_Input::Setup_Input():
    mKeyListModel(new KeyListModel),
    mKeyList(new ListBox(mKeyListModel)),
    mKeySetting(false),
    mActionDataSize(nullptr)
{
    inputManager.setSetupInput(this);
    setName(_("Input"));

    selectedData = 0;
    mActionDataSize = new int [7];

    for (int f = 0; f < 7; f ++)
    {
        int cnt = 0;
        while (!setupActionData[f][cnt].name.empty())
            cnt ++;
        mActionDataSize[f] = cnt;
    }

    mKeyListModel->setSize(mActionDataSize[0]);

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

    mTabs = new TabStrip(config.getIntValue("fontSize") + 10);
    mTabs->addActionListener(this);
    mTabs->setActionEventId("tabs_");
    int k = 0;
    while (pages[k])
    {
        mTabs->addButton(gettext(pages[k]), pages[k]);
        k ++;
    }

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mTabs, 4);
    place(0, 1, scrollArea, 4, 5).setPadding(2);
    place(0, 6, mMakeDefaultButton);
    place(2, 6, mAssignKeyButton);
    place(3, 6, mUnassignKeyButton);

    setDimension(gcn::Rectangle(0, 0, 500, 350));
}

Setup_Input::~Setup_Input()
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

void Setup_Input::apply()
{
    keyUnresolved();
    int key1, key2;

    if (inputManager.hasConflicts(key1, key2))
    {
        int s1 = keyToSetupData(key1);
        int s2 = keyToSetupData(key2);

        new OkDialog(_("Key Conflict(s) Detected."),
            strprintf(_("Conflict \"%s\" and \"%s\" keys. "
            "Resolve them, or gameplay may result in strange behaviour."),
            setupActionData[selectedData][s1].name.c_str(),
            setupActionData[selectedData][s2].name.c_str()), DIALOG_ERROR);
    }
    keyboard.setEnabled(true);
    inputManager.store();
}

void Setup_Input::cancel()
{
    keyUnresolved();

    inputManager.retrieve();
    keyboard.setEnabled(true);

    refreshKeys();
}

void Setup_Input::action(const gcn::ActionEvent &event)
{
    logger->log("event: %s", event.getId().c_str());
    const std::string id = event.getId();

    if (event.getSource() == mKeyList)
    {
        if (!mKeySetting)
        {
            int i(mKeyList->getSelected());
            if (i >= 0 && i < mActionDataSize[selectedData])
            {
                if (setupActionData[selectedData][i].actionId
                    == Input::KEY_NO_VALUE)
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
    }
    else if (id == "assign")
    {
        mKeySetting = true;
        mAssignKeyButton->setEnabled(false);
        keyboard.setEnabled(false);
        int i(mKeyList->getSelected());
        if (i >= 0 && i < mActionDataSize[selectedData])
        {
            const SetupActionData &key = setupActionData[selectedData][i];
            int ik = key.actionId;
            inputManager.setNewKeyIndex(ik);
            mKeyListModel->setElementAt(i, std::string(
                gettext(key.name.c_str())) + ": ?");
        }
    }
    else if (id == "unassign")
    {
        int i(mKeyList->getSelected());
        if (i >= 0 && i < mActionDataSize[selectedData])
        {
            const SetupActionData &key = setupActionData[selectedData][i];
            int ik = key.actionId;
            inputManager.setNewKeyIndex(ik);
            refreshAssignedKey(mKeyList->getSelected());
            inputManager.unassignKey();
        }
        mAssignKeyButton->setEnabled(true);
    }
    else if (id == "makeDefault")
    {
        inputManager.makeDefault();
        refreshKeys();
    }
    else if (!id.find("tabs_"))
    {
        int k = 0;
        std::string str = "tabs_";
        while (pages[k])
        {
            if (str + pages[k] == id)
                break;
            k ++;
        }
        if (pages[k] && str + pages[k] == id)
        {
            selectedData = k;
            mKeyListModel->setSize(mActionDataSize[k]);
            refreshKeys();
            mKeyList->setSelected(0);
        }
    }
}

void Setup_Input::refreshAssignedKey(int index)
{
    const SetupActionData &key = setupActionData[selectedData][index];
    if (key.actionId == Input::KEY_NO_VALUE)
    {
        const std::string str = " \342\200\225\342\200\225\342\200\225"
            "\342\200\225\342\200\225 ";
        mKeyListModel->setElementAt(index,
            str + gettext(key.name.c_str()) + str);
    }
    else
    {
        mKeyListModel->setElementAt(index, strprintf("%s: %s",
            gettext(key.name.c_str()),
            inputManager.getKeyStringLong(key.actionId).c_str()));
    }
}

void Setup_Input::newKeyCallback(int index)
{
    mKeySetting = false;
    int i = keyToSetupData(index);
    if (i >= 0)
        refreshAssignedKey(i);
    mAssignKeyButton->setEnabled(true);
}

int Setup_Input::keyToSetupData(int index)
{
    for (int i = 0; i < mActionDataSize[selectedData]; i++)
    {
        const SetupActionData &key = setupActionData[selectedData][i];
        if (key.actionId == index)
            return i;
    }
    return -1;
}

void Setup_Input::refreshKeys()
{
    for (int i = 0; i < mActionDataSize[selectedData]; i++)
        refreshAssignedKey(i);
}

void Setup_Input::keyUnresolved()
{
    if (mKeySetting)
    {
        newKeyCallback(inputManager.getNewKeyIndex());
        inputManager.setNewKeyIndex(Input::KEY_NO_VALUE);
    }
}
