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

struct SetupActionData
{
    std::string name;
    int actionId;
};

static SetupActionData const setupActionData[] =
{
    {
        N_("Basic Keys"),
        Input::KEY_NO_VALUE
    },
    {
        N_("Move Up"),
        Input::KEY_MOVE_UP
    },
    {
        N_("Move Down"),
        Input::KEY_MOVE_DOWN
    },
    {
        N_("Move Left"),
        Input::KEY_MOVE_LEFT
    },
    {
        N_("Move Right"),
        Input::KEY_MOVE_RIGHT
    },
    {
        N_("Attack"),
        Input::KEY_ATTACK
    },
    {
        N_("Target & Attack"),
        Input::KEY_TARGET_ATTACK
    },
    {
        N_("Move to Target"),
        Input::KEY_MOVE_TO_TARGET
    },
    {
        N_("Change Move to Target type"),
        Input::KEY_CHANGE_MOVE_TO_TARGET
    },
    {
        N_("Move to Home location"),
        Input::KEY_MOVE_TO_HOME
    },
    {
        N_("Set home location"),
        Input::KEY_SET_HOME
    },
    {
        N_("Move to navigation point"),
        Input::KEY_MOVE_TO_POINT
    },
    {
        N_("Talk"),
        Input::KEY_TALK
    },
    {
        N_("Stop Attack"),
        Input::KEY_TARGET
    },
    {
        N_("Untarget"),
        Input::KEY_UNTARGET
    },
    {
        N_("Target Closest"),
        Input::KEY_TARGET_CLOSEST
    },
    {
        N_("Target NPC"),
        Input::KEY_TARGET_NPC
    },
    {
        N_("Target Player"),
        Input::KEY_TARGET_PLAYER
    },
    {
        N_("Pickup"),
        Input::KEY_PICKUP
    },
    {
        N_("Change Pickup Type"),
        Input::KEY_CHANGE_PICKUP_TYPE
    },
    {
        N_("Hide Windows"),
        Input::KEY_HIDE_WINDOWS
    },
    {
        N_("Sit"),
        Input::KEY_SIT
    },
    {
        N_("Screenshot"),
        Input::KEY_SCREENSHOT
    },
    {
        N_("Enable/Disable Trading"),
        Input::KEY_TRADE
    },
    {
        N_("Change Map View Mode"),
        Input::KEY_PATHFIND
    },
    {
        N_("Select OK"),
        Input::KEY_OK
    },
    {
        N_("Quit"),
        Input::KEY_QUIT
    },
    {
        N_("Shortcuts Keys"),
        Input::KEY_NO_VALUE
    },
    {
        N_("Item Shortcuts Key"),
        Input::KEY_SHORTCUTS_KEY
    },
    {
        strprintf(N_("Item Shortcut %d"), 1),
        Input::KEY_SHORTCUT_1
    },
    {
        strprintf(N_("Item Shortcut %d"), 2),
        Input::KEY_SHORTCUT_2
    },
    {
        strprintf(N_("Item Shortcut %d"), 3),
        Input::KEY_SHORTCUT_3
    },
    {
        strprintf(N_("Item Shortcut %d"), 4),
        Input::KEY_SHORTCUT_4
    },
    {
        strprintf(N_("Item Shortcut %d"), 5),
        Input::KEY_SHORTCUT_5
    },
    {
        strprintf(N_("Item Shortcut %d"), 6),
        Input::KEY_SHORTCUT_6
    },
    {
        strprintf(N_("Item Shortcut %d"), 7),
        Input::KEY_SHORTCUT_7
    },
    {
        strprintf(N_("Item Shortcut %d"), 8),
        Input::KEY_SHORTCUT_8
    },
    {
        strprintf(N_("Item Shortcut %d"), 9),
        Input::KEY_SHORTCUT_9
    },
    {
        strprintf(N_("Item Shortcut %d"), 10),
        Input::KEY_SHORTCUT_10
    },
    {
        strprintf(N_("Item Shortcut %d"), 11),
        Input::KEY_SHORTCUT_11
    },
    {
        strprintf(N_("Item Shortcut %d"), 12),
        Input::KEY_SHORTCUT_12
    },
    {
        strprintf(N_("Item Shortcut %d"), 13),
        Input::KEY_SHORTCUT_13
    },
    {
        strprintf(N_("Item Shortcut %d"), 14),
        Input::KEY_SHORTCUT_14
    },
    {
        strprintf(N_("Item Shortcut %d"), 15),
        Input::KEY_SHORTCUT_15
    },
    {
        strprintf(N_("Item Shortcut %d"), 16),
        Input::KEY_SHORTCUT_16
    },
    {
        strprintf(N_("Item Shortcut %d"), 17),
        Input::KEY_SHORTCUT_17
    },
    {
        strprintf(N_("Item Shortcut %d"), 18),
        Input::KEY_SHORTCUT_18
    },
    {
        strprintf(N_("Item Shortcut %d"), 19),
        Input::KEY_SHORTCUT_19
    },
    {
        strprintf(N_("Item Shortcut %d"), 20),
        Input::KEY_SHORTCUT_20
    },
    {
        N_("Windows Keys"),
        Input::KEY_NO_VALUE
    },
    {
        N_("Help Window"),
        Input::KEY_WINDOW_HELP
    },
    {
        N_("Status Window"),
        Input::KEY_WINDOW_STATUS
    },
    {
        N_("Inventory Window"),
        Input::KEY_WINDOW_INVENTORY
    },
    {
        N_("Equipment Window"),
        Input::KEY_WINDOW_EQUIPMENT
    },
    {
        N_("Skill Window"),
        Input::KEY_WINDOW_SKILL
    },
    {
        N_("Minimap Window"),
        Input::KEY_WINDOW_MINIMAP
    },
    {
        N_("Chat Window"),
        Input::KEY_WINDOW_CHAT
    },
    {
        N_("Item Shortcut Window"),
        Input::KEY_WINDOW_SHORTCUT
    },
    {
        N_("Setup Window"),
        Input::KEY_WINDOW_SETUP
    },
    {
        N_("Debug Window"),
        Input::KEY_WINDOW_DEBUG
    },
    {
        N_("Social Window"),
        Input::KEY_WINDOW_SOCIAL
    },
    {
        N_("Emote Shortcut Window"),
        Input::KEY_WINDOW_EMOTE_SHORTCUT
    },
    {
        N_("Outfits Window"),
        Input::KEY_WINDOW_OUTFIT
    },
    {
        N_("Shop Window"),
        Input::KEY_WINDOW_SHOP
    },
    {
        N_("Quick drop Window"),
        Input::KEY_WINDOW_DROP
    },
    {
        N_("Kill Stats Window"),
        Input::KEY_WINDOW_KILLS
    },
    {
        N_("Commands Window"),
        Input::KEY_WINDOW_SPELLS
    },
    {
        N_("Bot Checker Window"),
        Input::KEY_WINDOW_BOT_CHECKER
    },
    {
        N_("Who Is Online Window"),
        Input::KEY_WINDOW_ONLINE
    },
    {
        N_("Did you know Window"),
        Input::KEY_WINDOW_DIDYOUKNOW
    },
    {
        N_("Previous Social Tab"),
        Input::KEY_PREV_SOCIAL_TAB
    },
    {
        N_("Next Social Tab"),
        Input::KEY_NEXT_SOCIAL_TAB
    },
    {
        N_("Emotes Keys"),
        Input::KEY_NO_VALUE
    },
    {
        N_("Smilie"),
        Input::KEY_EMOTE
    },
    {
        strprintf(N_("Emote Shortcut %d"), 1),
        Input::KEY_EMOTE_1
    },
    {
        strprintf(N_("Emote Shortcut %d"), 2),
        Input::KEY_EMOTE_2
    },
    {
        strprintf(N_("Emote Shortcut %d"), 3),
        Input::KEY_EMOTE_3
    },
    {
        strprintf(N_("Emote Shortcut %d"), 4),
        Input::KEY_EMOTE_4
    },
    {
        strprintf(N_("Emote Shortcut %d"), 5),
        Input::KEY_EMOTE_5
    },
    {
        strprintf(N_("Emote Shortcut %d"), 6),
        Input::KEY_EMOTE_6
    },
    {
        strprintf(N_("Emote Shortcut %d"), 7),
        Input::KEY_EMOTE_7
    },
    {
        strprintf(N_("Emote Shortcut %d"), 8),
        Input::KEY_EMOTE_8
    },
    {
        strprintf(N_("Emote Shortcut %d"), 9),
        Input::KEY_EMOTE_9
    },
    {
        strprintf(N_("Emote Shortcut %d"), 10),
        Input::KEY_EMOTE_10
    },
    {
        strprintf(N_("Emote Shortcut %d"), 11),
        Input::KEY_EMOTE_11
    },
    {
        strprintf(N_("Emote Shortcut %d"), 12),
        Input::KEY_EMOTE_12
    },
    {
        strprintf(N_("Emote Shortcut %d"), 13),
        Input::KEY_EMOTE_13
    },
    {
        strprintf(N_("Emote Shortcut %d"), 14),
        Input::KEY_EMOTE_14
    },
    {
        strprintf(N_("Emote Shortcut %d"), 15),
        Input::KEY_EMOTE_15
    },
    {
        strprintf(N_("Emote Shortcut %d"), 16),
        Input::KEY_EMOTE_16
    },
    {
        strprintf(N_("Emote Shortcut %d"), 17),
        Input::KEY_EMOTE_17
    },
    {
        strprintf(N_("Emote Shortcut %d"), 18),
        Input::KEY_EMOTE_18
    },
    {
        strprintf(N_("Emote Shortcut %d"), 19),
        Input::KEY_EMOTE_19
    },
    {
        strprintf(N_("Emote Shortcut %d"), 20),
        Input::KEY_EMOTE_20
    },
    {
        strprintf(N_("Emote Shortcut %d"), 21),
        Input::KEY_EMOTE_21
    },
    {
        strprintf(N_("Emote Shortcut %d"), 22),
        Input::KEY_EMOTE_22
    },
    {
        strprintf(N_("Emote Shortcut %d"), 23),
        Input::KEY_EMOTE_23
    },
    {
        strprintf(N_("Emote Shortcut %d"), 24),
        Input::KEY_EMOTE_24
    },
    {
        strprintf(N_("Emote Shortcut %d"), 25),
        Input::KEY_EMOTE_25
    },
    {
        strprintf(N_("Emote Shortcut %d"), 26),
        Input::KEY_EMOTE_26
    },
    {
        strprintf(N_("Emote Shortcut %d"), 27),
        Input::KEY_EMOTE_27
    },
    {
        strprintf(N_("Emote Shortcut %d"), 28),
        Input::KEY_EMOTE_28
    },
    {
        strprintf(N_("Emote Shortcut %d"), 29),
        Input::KEY_EMOTE_29
    },
    {
        strprintf(N_("Emote Shortcut %d"), 30),
        Input::KEY_EMOTE_30
    },
    {
        strprintf(N_("Emote Shortcut %d"), 31),
        Input::KEY_EMOTE_31
    },
    {
        strprintf(N_("Emote Shortcut %d"), 32),
        Input::KEY_EMOTE_32
    },
    {
        strprintf(N_("Emote Shortcut %d"), 33),
        Input::KEY_EMOTE_33
    },
    {
        strprintf(N_("Emote Shortcut %d"), 34),
        Input::KEY_EMOTE_34
    },
    {
        strprintf(N_("Emote Shortcut %d"), 35),
        Input::KEY_EMOTE_35
    },
    {
        strprintf(N_("Emote Shortcut %d"), 36),
        Input::KEY_EMOTE_36
    },
    {
        strprintf(N_("Emote Shortcut %d"), 37),
        Input::KEY_EMOTE_37
    },
    {
        strprintf(N_("Emote Shortcut %d"), 38),
        Input::KEY_EMOTE_38
    },
    {
        strprintf(N_("Emote Shortcut %d"), 39),
        Input::KEY_EMOTE_39
    },
    {
        strprintf(N_("Emote Shortcut %d"), 40),
        Input::KEY_EMOTE_40
    },
    {
        strprintf(N_("Emote Shortcut %d"), 41),
        Input::KEY_EMOTE_41
    },
    {
        strprintf(N_("Emote Shortcut %d"), 42),
        Input::KEY_EMOTE_42
    },
    {
        strprintf(N_("Emote Shortcut %d"), 43),
        Input::KEY_EMOTE_43
    },
    {
        strprintf(N_("Emote Shortcut %d"), 44),
        Input::KEY_EMOTE_44
    },
    {
        strprintf(N_("Emote Shortcut %d"), 45),
        Input::KEY_EMOTE_45
    },
    {
        strprintf(N_("Emote Shortcut %d"), 46),
        Input::KEY_EMOTE_46
    },
    {
        strprintf(N_("Emote Shortcut %d"), 47),
        Input::KEY_EMOTE_47
    },
    {
        strprintf(N_("Emote Shortcut %d"), 48),
        Input::KEY_EMOTE_48
    },
    {
        N_("Outfits Keys"),
        Input::KEY_NO_VALUE
    },
    {
        N_("Wear Outfit"),
        Input::KEY_WEAR_OUTFIT
    },
    {
        N_("Copy Outfit"),
        Input::KEY_COPY_OUTFIT
    },
    {
        N_("Copy equipped to Outfit"),
        Input::KEY_COPY_EQUIPED_OUTFIT
    },
    {
        N_("Chat Keys"),
        Input::KEY_NO_VALUE
    },
    {
        N_("Toggle Chat"),
        Input::KEY_TOGGLE_CHAT
    },
    {
        N_("Scroll Chat Up"),
        Input::KEY_SCROLL_CHAT_UP
    },
    {
        N_("Scroll Chat Down"),
        Input::KEY_SCROLL_CHAT_DOWN
    },
    {
        N_("Previous Chat Tab"),
        Input::KEY_PREV_CHAT_TAB
    },
    {
        N_("Next Chat Tab"),
        Input::KEY_NEXT_CHAT_TAB
    },
    {
        N_("Close current Chat Tab"),
        Input::KEY_CLOSE_CHAT_TAB
    },
    {
        N_("Previous chat line"),
        Input::KEY_CHAT_PREV_HISTORY
    },
    {
        N_("Next chat line"),
        Input::KEY_CHAT_NEXT_HISTORY
    },
    {
        N_("Chat Auto Complete"),
        Input::KEY_AUTOCOMPLETE_CHAT
    },
    {
        N_("Deactivate Chat Input"),
        Input::KEY_DEACTIVATE_CHAT
    },
    {
        N_("Other Keys"),
        Input::KEY_NO_VALUE
    },
    {
        N_("Ignore input 1"),
        Input::KEY_IGNORE_INPUT_1
    },
    {
        N_("Ignore input 2"),
        Input::KEY_IGNORE_INPUT_2
    },
    {
        N_("Direct Up"),
        Input::KEY_DIRECT_UP
    },
    {
        N_("Direct Down"),
        Input::KEY_DIRECT_DOWN
    },
    {
        N_("Direct Left"),
        Input::KEY_DIRECT_LEFT
    },
    {
        N_("Direct Right"),
        Input::KEY_DIRECT_RIGHT
    },
    {
        N_("Crazy moves"),
        Input::KEY_CRAZY_MOVES
    },
    {
        N_("Change Crazy Move mode"),
        Input::KEY_CHANGE_CRAZY_MOVES_TYPE
    },
    {
        N_("Quick Drop N Items from 0 slot"),
        Input::KEY_QUICK_DROP
    },
    {
        N_("Quick Drop N Items"),
        Input::KEY_QUICK_DROPN
    },
    {
        N_("Switch Quick Drop Counter"),
        Input::KEY_SWITCH_QUICK_DROP
    },
    {
        N_("Quick heal target or self"),
        Input::KEY_MAGIC_INMA1
    },
    {
        N_("Use #itenplz spell"),
        Input::KEY_MAGIC_ITENPLZ
    },
    {
        N_("Use magic attack"),
        Input::KEY_MAGIC_ATTACK
    },
    {
        N_("Switch magic attack"),
        Input::KEY_SWITCH_MAGIC_ATTACK
    },
    {
        N_("Switch pvp attack"),
        Input::KEY_SWITCH_PVP_ATTACK
    },
    {
        N_("Change move type"),
        Input::KEY_INVERT_DIRECTION
    },
    {
        N_("Change Attack Weapon Type"),
        Input::KEY_CHANGE_ATTACK_WEAPON_TYPE
    },
    {
        N_("Change Attack Type"),
        Input::KEY_CHANGE_ATTACK_TYPE
    },
    {
        N_("Change Follow mode"),
        Input::KEY_CHANGE_FOLLOW_MODE
    },
    {
        N_("Change Imitation mode"),
        Input::KEY_CHANGE_IMITATION_MODE
    },
    {
        N_("Disable / Enable Game modifier keys"),
        Input::KEY_DISABLE_GAME_MODIFIERS
    },
    {
        N_("On / Off audio"),
        Input::KEY_CHANGE_AUDIO
    },
    {
        N_("Enable / Disable away mode"),
        Input::KEY_AWAY
    },
    {
        N_("Emulate right click from keyboard"),
        Input::KEY_RIGHT_CLICK
    },
    {
        N_("Toggle camera mode"),
        Input::KEY_CAMERA
    },
    {
        N_("Modifier key"),
        Input::KEY_MOD
    }
};

const int keysSize = sizeof(setupActionData) / sizeof(SetupActionData);
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
        { return keysSize; }

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
        std::string mKeyFunctions[keysSize];
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
    int key1, key2;

    if (keyboard.hasConflicts(key1, key2))
    {
        int s1 = keyToSetupData(key1);
        int s2 = keyToSetupData(key2);

        new OkDialog(_("Key Conflict(s) Detected."),
            strprintf(_("Conflict \"%s\" and \"%s\" keys. "
            "Resolve them, or gameplay may result in strange behaviour."),
            setupActionData[s1].name.c_str(),
            setupActionData[s2].name.c_str()), DIALOG_ERROR);
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
            if (i >= 0 && i < keysSize)
            {
                if (setupActionData[i].actionId
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
    else if (event.getId() == "assign")
    {
        mKeySetting = true;
        mAssignKeyButton->setEnabled(false);
        keyboard.setEnabled(false);
        int i(mKeyList->getSelected());
        if (i >= 0 && i < keysSize)
        {
            const SetupActionData &key = setupActionData[i];
            int ik = key.actionId;
            keyboard.setNewKeyIndex(ik);
            mKeyListModel->setElementAt(i, std::string(
                gettext(key.name.c_str())) + ": ?");
        }
    }
    else if (event.getId() == "unassign")
    {
        int i(mKeyList->getSelected());
        if (i >= 0 && i < keysSize)
        {
            const SetupActionData &key = setupActionData[i];
            int ik = key.actionId;
            keyboard.setNewKeyIndex(ik);
            refreshAssignedKey(mKeyList->getSelected());
            keyboard.unassignKey();
        }
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
    const SetupActionData &key = setupActionData[index];
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
            keyboard.getKeyStringLong(key.actionId).c_str()));
    }
}

void Setup_Keyboard::newKeyCallback(int index)
{
    mKeySetting = false;
    int i = keyToSetupData(index);
    if (i >= 0)
        refreshAssignedKey(i);
    mAssignKeyButton->setEnabled(true);
}

int Setup_Keyboard::keyToSetupData(int index)
{
    for (int i = 0; i < keysSize; i++)
    {
        const SetupActionData &key = setupActionData[i];
        if (key.actionId == index)
            return i;
    }
    return -1;
}

void Setup_Keyboard::refreshKeys()
{
    for (int i = 0; i < keysSize; i++)
        refreshAssignedKey(i);
}

void Setup_Keyboard::keyUnresolved()
{
    if (mKeySetting)
    {
        newKeyCallback(keyboard.getNewKeyIndex());
        keyboard.setNewKeyIndex(Input::KEY_NO_VALUE);
    }
}
