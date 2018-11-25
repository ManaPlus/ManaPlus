/*
 *  The ManaPlus Client
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
 *  Copyright (C) 2009  The Mana World Development Team
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

#include "gui/widgets/tabs/setup_input.h"

#include "configuration.h"

#include "const/gui/pages.h"

#include "input/inputactionoperators.h"
#include "input/inputmanager.h"
#include "input/keyboardconfig.h"

#include "input/pages/craft.h"
#include "input/pages/emotes.h"
#include "input/pages/move.h"
#include "input/pages/outfits.h"
#include "input/pages/shortcuts.h"

#include "gui/gui.h"
#include "gui/setupinputpages.h"

#include "gui/windows/okdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/tabstrip.h"

#include "gui/models/keylistmodel.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "debug.h"

Setup_Input::Setup_Input(const Widget2 *const widget) :
    SetupTab(widget),
    mKeyListModel(new KeyListModel),
    mKeyList(CREATEWIDGETR(ListBox, this, mKeyListModel, "")),
    // TRANSLATORS: button in input settings tab
    mAssignKeyButton(new Button(this, _("Assign"), "assign",
        BUTTON_SKIN, this)),
    // TRANSLATORS: button in input settings tab
    mUnassignKeyButton(new Button(this, _("Unassign"), "unassign",
        BUTTON_SKIN, this)),
    // TRANSLATORS: button in input settings tab
    mDefaultButton(new Button(this, _("Default"), "default",
        BUTTON_SKIN, this)),
    // TRANSLATORS: button in input settings tab
    mResetKeysButton(new Button(this, _("Reset all keys"), "resetkeys",
        BUTTON_SKIN, this)),
    mTabs(new TabStrip(this, config.getIntValue("fontSize") + 10, 0)),
    mScrollArea(new ScrollArea(this, mKeyList,
        Opaque_true, "setup_input_background.xml")),
    mKeySetting(false),
    mActionDataSize(new int [SETUP_PAGES])
{
    inputManager.setSetupInput(this);
    // TRANSLATORS: setting tab name
    setName(_("Input"));

    mKeyListModel->setSelectedData(0);

    for (int f = 0; f < SETUP_PAGES; f ++)
    {
        int cnt = 0;
        while (!setupActionData[f][cnt].name.empty())
            cnt ++;
        mActionDataSize[f] = cnt;
    }

    mKeyListModel->setSize(mActionDataSize[0]);
    refreshKeys();
    if (gui != nullptr)
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
    while (pages[k] != nullptr)
    {
        mTabs->addButton(gettext(pages[k]), pages[k], false);
        k ++;
    }

    fixTranslations();

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mTabs, 5, 1);
    place(0, 1, mScrollArea, 5, 5).setPadding(2);
    place(0, 6, mResetKeysButton, 1, 1);
    place(2, 6, mAssignKeyButton, 1, 1);
    place(3, 6, mUnassignKeyButton, 1, 1);
    place(4, 6, mDefaultButton, 1, 1);

    int width = 600;
    if (config.getIntValue("screenwidth") >= 730)
        width += 100;

    setDimension(Rect(0, 0, width, 350));
}

Setup_Input::~Setup_Input()
{
    delete2(mKeyList)
    delete2(mKeyListModel)
    delete2(mAssignKeyButton)
    delete2(mUnassignKeyButton)
    delete2(mResetKeysButton)
    delete [] mActionDataSize;
    mActionDataSize = nullptr;
    delete2(mScrollArea)
}

void Setup_Input::apply()
{
    keyUnresolved();
    InputActionT key1;
    InputActionT key2;

    if (inputManager.hasConflicts(key1, key2))
    {
        const std::string str1 = keyToString(key1);
        const std::string str2 = keyToString(key2);

        CREATEWIDGET(OkDialog,
            // TRANSLATORS: input settings error header
            _("Key Conflict(s) Detected."),
            // TRANSLATORS: input settings error
            strprintf(_("Conflict \"%s\" and \"%s\" keys. "
            "Resolve them, or gameplay may result in strange behaviour."),
            gettext(str1.c_str()), gettext(str2.c_str())),
            // TRANSLATORS: ok dialog button
            _("OK"),
            DialogType::ERROR,
            Modal_true,
            ShowCenter_true,
            nullptr,
            260);
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
    const int selectedData = mKeyListModel->getSelectedData();

    if (event.getSource() == mKeyList)
    {
        if (!mKeySetting)
        {
            const int i(mKeyList->getSelected());
            if (i >= 0 && i < mActionDataSize[selectedData])
            {
                if (setupActionData[selectedData][i].actionId
                    == InputAction::NO_VALUE)
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
            const InputActionT ik = key.actionId;
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
            const InputActionT ik = key.actionId;
            inputManager.setNewKeyIndex(ik);
            refreshAssignedKey(mKeyList->getSelected());
            inputManager.unassignKey();
            inputManager.setNewKeyIndex(InputAction::NO_VALUE);
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
            const InputActionT ik = key.actionId;
            inputManager.makeDefault(ik);
            refreshKeys();
        }
    }
    else if (strStartWith(id, "tabs_"))
    {
        int k = 0;
        std::string str("tabs_");
        while (pages[k] != nullptr)
        {
            if (str + pages[k] == id)
                break;
            k ++;
        }
        if ((pages[k] != nullptr) && str + pages[k] == id)
        {
            mKeyListModel->setSelectedData(k);
            mKeyListModel->setSize(mActionDataSize[k]);
            refreshKeys();
            mKeyList->setSelected(0);
        }
    }
}

void Setup_Input::refreshAssignedKey(const int index)
{
    const int selectedData = mKeyListModel->getSelectedData();
    const SetupActionData &key = setupActionData[selectedData][index];
    if (key.actionId == InputAction::NO_VALUE)
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

void Setup_Input::newKeyCallback(const InputActionT index)
{
    mKeySetting = false;
    const int i = keyToSetupData(index);
    if (i >= 0)
        refreshAssignedKey(i);
    mAssignKeyButton->setEnabled(true);
}

int Setup_Input::keyToSetupData(const InputActionT index) const
{
    const int selectedData = mKeyListModel->getSelectedData();
    for (int i = 0; i < mActionDataSize[selectedData]; i++)
    {
        const SetupActionData &key = setupActionData[selectedData][i];
        if (key.actionId == index)
            return i;
    }
    return -1;
}

std::string Setup_Input::keyToString(const InputActionT index) const
{
    for (int f = 0; f < SETUP_PAGES; f ++)
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
    const int selectedData = mKeyListModel->getSelectedData();
    for (int i = 0; i < mActionDataSize[selectedData]; i++)
        refreshAssignedKey(i);
}

void Setup_Input::keyUnresolved()
{
    if (mKeySetting)
    {
        newKeyCallback(inputManager.getNewKeyIndex());
        inputManager.setNewKeyIndex(InputAction::NO_VALUE);
    }
}

void Setup_Input::fixTranslation(SetupActionData *const actionDatas,
                                 const InputActionT actionStart,
                                 const InputActionT actionEnd,
                                 const std::string &text)
{
    int k = 0;

    while (!actionDatas[k].name.empty())
    {
        SetupActionData &data = actionDatas[k];

        const InputActionT actionId = data.actionId;
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
    fixTranslation(setupActionDataShortcuts,
        InputAction::SHORTCUT_1,
        InputAction::SHORTCUT_20,
        "Item Shortcut %d");

    fixTranslation(setupActionDataEmotes,
        InputAction::EMOTE_1,
        InputAction::EMOTE_48,
        "Emote Shortcut %d");

    fixTranslation(setupActionDataCraft,
        InputAction::CRAFT_1,
        InputAction::CRAFT_9,
        "Craft shortcut %d");

    fixTranslation(setupActionDataOutfits,
        InputAction::OUTFIT_1,
        InputAction::OUTFIT_48,
        "Outfit Shortcut %d");

    fixTranslation(setupActionDataMove,
        InputAction::MOVE_TO_POINT_1,
        InputAction::MOVE_TO_POINT_48,
        "Move to point Shortcut %d");
}
