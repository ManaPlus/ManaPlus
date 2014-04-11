/*
 *  Custom keyboard shortcuts configuration
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
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

#include "gui/widgets/tabs/setup_input.h"

#include "configuration.h"

#include "input/inputmanager.h"
#include "input/keyboardconfig.h"

#include "gui/gui.h"
#include "gui/setupactiondata.h"

#include "gui/windows/okdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/tabstrip.h"

#include "gui/models/listmodel.h"

#include "utils/delete2.h"

#include "debug.h"

static int selectedData = 0;
static const int setupGroups = 9;

/**
 * The list model for key function list.
 *
 * \ingroup Interface
 */
class KeyListModel final : public ListModel
{
    public:
        KeyListModel() :
            mDataNum(0),
            mSize(0)
        {
        }

        A_DELETE_COPY(KeyListModel)

        /**
         * Returns the number of elements in container.
         */
        int getNumberOfElements() override final
        { return mSize; }

        /**
         * Returns element from container.
         */
        std::string getElementAt(int i) override final
        { return setupActionData[selectedData][i].text; }

        /**
         * Sets element from container.
         */
        void setElementAt(const int i, const std::string &caption)
        { setupActionData[selectedData][i].text = caption; }

        void setSize(const int size)
        { mSize = size; }

        void setDataNum(const int num)
        { mDataNum = num; }

    private:
        int mDataNum;
        int mSize;
};

Setup_Input::Setup_Input(const Widget2 *const widget) :
    SetupTab(widget),
    mKeyListModel(new KeyListModel),
    mKeyList(new ListBox(this, mKeyListModel, "")),
    // TRANSLATORS: button in input settings tab
    mAssignKeyButton(new Button(this, _("Assign"), "assign", this)),
    // TRANSLATORS: button in input settings tab
    mUnassignKeyButton(new Button(this, _("Unassign"), "unassign", this)),
    // TRANSLATORS: button in input settings tab
    mDefaultButton(new Button(this, _("Default"), "default", this)),
    // TRANSLATORS: button in input settings tab
    mResetKeysButton(new Button(this, _("Reset all keys"), "resetkeys", this)),
    mTabs(new TabStrip(this, config.getIntValue("fontSize") + 10)),
    mScrollArea(new ScrollArea(this, mKeyList,
        true, "setup_input_background.xml")),
    mKeySetting(false),
    mActionDataSize(new int [9])
{
    mKeyList->postInit();
    inputManager.setSetupInput(this);
    // TRANSLATORS: setting tab name
    setName(_("Input"));

    selectedData = 0;

    for (int f = 0; f < setupGroups; f ++)
    {
        int cnt = 0;
        while (!setupActionData[f][cnt].name.empty())
            cnt ++;
        mActionDataSize[f] = cnt;
    }

    mKeyListModel->setSize(mActionDataSize[0]);
    refreshKeys();
    if (gui)
        mKeyList->setFont(gui->getHelpFont());
    mKeyList->addActionListener(this);

    mScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mAssignKeyButton->addActionListener(this);
    mAssignKeyButton->setEnabled(false);
    mUnassignKeyButton->addActionListener(this);
    mUnassignKeyButton->setEnabled(false);
    mResetKeysButton->addActionListener(this);
    mDefaultButton->addActionListener(this);

    mTabs->addActionListener(this);
    mTabs->setActionEventId("tabs_");
    int k = 0;
    while (pages[k])
    {
        mTabs->addButton(gettext(pages[k]), pages[k]);
        k ++;
    }

    fixTranslations();

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mTabs, 5);
    place(0, 1, mScrollArea, 5, 5).setPadding(2);
    place(0, 6, mResetKeysButton);
    place(2, 6, mAssignKeyButton);
    place(3, 6, mUnassignKeyButton);
    place(4, 6, mDefaultButton);

    int width = 600;
    if (config.getIntValue("screenwidth") >= 730)
        width += 100;

    setDimension(Rect(0, 0, width, 350));
}

Setup_Input::~Setup_Input()
{
    delete2(mKeyList);
    delete2(mKeyListModel);
    delete2(mAssignKeyButton);
    delete2(mUnassignKeyButton);
    delete2(mResetKeysButton);
    delete [] mActionDataSize;
    mActionDataSize = nullptr;
    delete2(mScrollArea);
}

void Setup_Input::apply()
{
    keyUnresolved();
    int key1, key2;

    if (inputManager.hasConflicts(key1, key2))
    {
        const std::string str1 = keyToString(key1);
        const std::string str2 = keyToString(key2);

        // TRANSLATORS: input settings error header
        new OkDialog(_("Key Conflict(s) Detected."),
            // TRANSLATORS: input settings error
            strprintf(_("Conflict \"%s\" and \"%s\" keys. "
            "Resolve them, or gameplay may result in strange behaviour."),
            gettext(str1.c_str()), gettext(str2.c_str())), DIALOG_ERROR);
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

void Setup_Input::action(const ActionEvent &event)
{
    const std::string &id = event.getId();

    if (event.getSource() == mKeyList)
    {
        if (!mKeySetting)
        {
            const int i(mKeyList->getSelected());
            if (i >= 0 && i < mActionDataSize[selectedData])
            {
                if (setupActionData[selectedData][i].actionId
                    == static_cast<int>(Input::KEY_NO_VALUE))
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
        const int i(mKeyList->getSelected());
        if (i >= 0 && i < mActionDataSize[selectedData])
        {
            const SetupActionData &key = setupActionData[selectedData][i];
            const int ik = key.actionId;
            inputManager.setNewKeyIndex(ik);
            mKeyListModel->setElementAt(i, std::string(
                gettext(key.name.c_str())).append(": ?"));
        }
    }
    else if (id == "unassign")
    {
        const int i(mKeyList->getSelected());
        if (i >= 0 && i < mActionDataSize[selectedData])
        {
            const SetupActionData &key = setupActionData[selectedData][i];
            const int ik = key.actionId;
            inputManager.setNewKeyIndex(ik);
            refreshAssignedKey(mKeyList->getSelected());
            inputManager.unassignKey();
            inputManager.setNewKeyIndex(static_cast<int>(Input::KEY_NO_VALUE));
        }
        mAssignKeyButton->setEnabled(true);
    }
    else if (id == "resetkeys")
    {
        inputManager.resetKeys();
        InputManager::update();
        refreshKeys();
    }
    else if (id == "default")
    {
        const int i(mKeyList->getSelected());
        if (i >= 0 && i < mActionDataSize[selectedData])
        {
            const SetupActionData &key = setupActionData[selectedData][i];
            const int ik = key.actionId;
            inputManager.makeDefault(ik);
            refreshKeys();
        }
    }
    else if (strStartWith(id, "tabs_"))
    {
        int k = 0;
        std::string str("tabs_");
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

void Setup_Input::refreshAssignedKey(const int index)
{
    const SetupActionData &key = setupActionData[selectedData][index];
    if (key.actionId == static_cast<int>(Input::KEY_NO_VALUE))
    {
        const std::string str(" \342\200\225\342\200\225\342\200\225"
            "\342\200\225\342\200\225 ");
        mKeyListModel->setElementAt(index,
            str + gettext(key.name.c_str()) + str);
    }
    else
    {
        std::string str = gettext(key.name.c_str());
        unsigned int sz = 20;
        if (mainGraphics->mWidth > 800)
            sz = 30;
        while (str.size() < sz)
            str.append(" ");
        mKeyListModel->setElementAt(index, strprintf("%s: %s", str.c_str(),
            inputManager.getKeyStringLong(key.actionId).c_str()));
    }
}

void Setup_Input::newKeyCallback(const int index)
{
    mKeySetting = false;
    const int i = keyToSetupData(index);
    if (i >= 0)
        refreshAssignedKey(i);
    mAssignKeyButton->setEnabled(true);
}

int Setup_Input::keyToSetupData(const int index) const
{
    for (int i = 0; i < mActionDataSize[selectedData]; i++)
    {
        const SetupActionData &key = setupActionData[selectedData][i];
        if (key.actionId == index)
            return i;
    }
    return -1;
}

std::string Setup_Input::keyToString(const int index) const
{
    for (int f = 0; f < setupGroups; f ++)
    {
        for (int i = 0; i < mActionDataSize[f]; i++)
        {
            const SetupActionData &key = setupActionData[f][i];
            if (key.actionId == index)
                return key.name;
        }
    }
    // TRANSLATORS: unknown key name
    return _("unknown");
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
        inputManager.setNewKeyIndex(static_cast<int>(Input::KEY_NO_VALUE));
    }
}

void Setup_Input::fixTranslation(SetupActionData *const actionDatas,
                                 const int actionStart,
                                 const int actionEnd,
                                 const std::string &text)
{
    int k = 0;

    while (!actionDatas[k].name.empty())
    {
        SetupActionData &data = actionDatas[k];

        const int actionId = data.actionId;
        if (actionId >= actionStart && actionId <= actionEnd)
        {
            data.name = strprintf(gettext(text.c_str()),
                actionId - actionStart + 1);
        }
        k ++;
    }
}

void Setup_Input::fixTranslations()
{
    fixTranslation(setupActionData1, static_cast<int>(Input::KEY_SHORTCUT_1),
        static_cast<int>(Input::KEY_SHORTCUT_20), "Item Shortcut %d");

    fixTranslation(setupActionData3, static_cast<int>(Input::KEY_EMOTE_1),
        static_cast<int>(Input::KEY_EMOTE_48), "Emote Shortcut %d");

    fixTranslation(setupActionData4, static_cast<int>(Input::KEY_OUTFIT_1),
        static_cast<int>(Input::KEY_OUTFIT_48), "Outfit Shortcut %d");

    fixTranslation(setupActionData7, static_cast<int>(
        Input::KEY_MOVE_TO_POINT_1),
        static_cast<int>(Input::KEY_MOVE_TO_POINT_48),
        "Move to point Shortcut %d");
}
