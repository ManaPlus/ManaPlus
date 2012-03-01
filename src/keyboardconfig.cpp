/*
 *  Custom keyboard shortcuts configuration
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
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

#include "configuration.h"
#include "keyboardconfig.h"
#include "logger.h"

#include "gui/sdlinput.h"
#include "gui/setup_keyboard.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

struct KeyData
{
    const char *configField;
    int defaultValue;
    std::string caption;
    int grp;
};

// keyData must be in same order as enum keyAction.
static KeyData const keyData[KeyboardConfig::KEY_TOTAL] = {
    {"", 0, N_("Basic Keys"), 0},
    {"keyMoveUp", SDLK_UP, N_("Move Up"), KeyboardConfig::GRP_DEFAULT},
    {"keyMoveDown", SDLK_DOWN, N_("Move Down"), KeyboardConfig::GRP_DEFAULT},
    {"keyMoveLeft", SDLK_LEFT, N_("Move Left"), KeyboardConfig::GRP_DEFAULT},
    {"keyMoveRight", SDLK_RIGHT, N_("Move Right"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyAttack", SDLK_LCTRL, N_("Attack"), KeyboardConfig::GRP_DEFAULT},
    {"keyTargetAttack", SDLK_x, N_("Target & Attack"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyMoveToTarget", SDLK_v, N_("Move to Target"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyChangeMoveToTarget", SDLK_PERIOD, N_("Change Move to Target type"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyMoveToHome", SDLK_d, N_("Move to Home location"),
        KeyboardConfig::GRP_DEFAULT},
    {"keySetHome", SDLK_KP5, N_("Set home location"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyMoveToPoint", SDLK_RSHIFT, N_("Move to navigation point"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyTalk", SDLK_t, N_("Talk"), KeyboardConfig::GRP_DEFAULT},
    {"keyTarget", SDLK_LSHIFT, N_("Stop Attack"), KeyboardConfig::GRP_DEFAULT},
    {"keyUnTarget", KeyboardConfig::KEY_NO_VALUE,
        N_("Untarget"), KeyboardConfig::GRP_DEFAULT},
    {"keyTargetClosest", SDLK_a, N_("Target Closest"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyTargetNPC", SDLK_n, N_("Target NPC"), KeyboardConfig::GRP_DEFAULT},
    {"keyTargetPlayer", SDLK_q, N_("Target Player"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyPickup", SDLK_z, N_("Pickup"), KeyboardConfig::GRP_DEFAULT},
    {"keyChangePickupType", SDLK_o, N_("Change Pickup Type"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyHideWindows", SDLK_h, N_("Hide Windows"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyBeingSit", SDLK_s, N_("Sit"), KeyboardConfig::GRP_DEFAULT},
    {"keyScreenshot", SDLK_p, N_("Screenshot"), KeyboardConfig::GRP_DEFAULT},
    {"keyTrade", SDLK_r, N_("Enable/Disable Trading"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyPathfind", SDLK_f, N_("Change Map View Mode"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyOK", SDLK_SPACE, N_("Select OK"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyQuit", SDLK_ESCAPE, N_("Quit"), KeyboardConfig::GRP_DEFAULT},
    {"", 0, N_("Shortcuts Keys"), 0},
    {"keyShortcutsKey", SDLK_MENU, N_("Item Shortcuts Key"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut1", SDLK_1, strprintf(N_("Item Shortcut %d"), 1),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut2", SDLK_2, strprintf(N_("Item Shortcut %d"), 2),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut3", SDLK_3, strprintf(N_("Item Shortcut %d"), 3),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut4", SDLK_4, strprintf(N_("Item Shortcut %d"), 4),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut5", SDLK_5, strprintf(N_("Item Shortcut %d"), 5),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut6", SDLK_6, strprintf(N_("Item Shortcut %d"), 6),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut7", SDLK_7, strprintf(N_("Item Shortcut %d"), 7),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut8", SDLK_8, strprintf(N_("Item Shortcut %d"), 8),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut9", SDLK_9, strprintf(N_("Item Shortcut %d"), 9),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut10", SDLK_0, strprintf(N_("Item Shortcut %d"), 10),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut11", SDLK_MINUS, strprintf(N_("Item Shortcut %d"), 11),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut12", SDLK_EQUALS, strprintf(N_("Item Shortcut %d"), 12),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut13", SDLK_BACKSPACE, strprintf(N_("Item Shortcut %d"), 13),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut14", SDLK_INSERT, strprintf(N_("Item Shortcut %d"), 14),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut15", SDLK_HOME, strprintf(N_("Item Shortcut %d"), 15),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut16", KeyboardConfig::KEY_NO_VALUE,
        strprintf(N_("Item Shortcut %d"), 16), KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut17", KeyboardConfig::KEY_NO_VALUE,
        strprintf(N_("Item Shortcut %d"), 17), KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut18", KeyboardConfig::KEY_NO_VALUE,
        strprintf(N_("Item Shortcut %d"), 18), KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut19", KeyboardConfig::KEY_NO_VALUE,
        strprintf(N_("Item Shortcut %d"), 19), KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut20", KeyboardConfig::KEY_NO_VALUE,
        strprintf(N_("Item Shortcut %d"), 20), KeyboardConfig::GRP_DEFAULT},
    {"", 0, N_("Windows Keys"), 0},
    {"keyWindowHelp", SDLK_F1, N_("Help Window"), KeyboardConfig::GRP_DEFAULT
        | KeyboardConfig::GRP_GUI},
    {"keyWindowStatus", SDLK_F2, N_("Status Window"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyWindowInventory", SDLK_F3, N_("Inventory Window"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyWindowEquipment", SDLK_F4, N_("Equipment Window"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyWindowSkill", SDLK_F5, N_("Skill Window"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyWindowMinimap", SDLK_F6, N_("Minimap Window"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyWindowChat", SDLK_F7, N_("Chat Window"), KeyboardConfig::GRP_DEFAULT
        | KeyboardConfig::GRP_GUI},
    {"keyWindowShortcut", SDLK_F8, N_("Item Shortcut Window"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyWindowSetup", SDLK_F9, N_("Setup Window"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyWindowDebug", SDLK_F10, N_("Debug Window"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyWindowSocial", SDLK_F11, N_("Social Window"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyWindowEmoteBar", SDLK_F12, N_("Emote Shortcut Window"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyWindowOutfit", SDLK_BACKQUOTE, N_("Outfits Window"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyWindowShop", -1, N_("Shop Window"), KeyboardConfig::GRP_DEFAULT
        | KeyboardConfig::GRP_GUI},
    {"keyWindowDrop", SDLK_w, N_("Quick drop Window"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyWindowKills", SDLK_e, N_("Kill Stats Window"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyWindowSpells", SDLK_j, N_("Commands Window"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyWindowBotChecker", SDLK_LEFTBRACKET, N_("Bot Checker Window"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyWindowOnline", KeyboardConfig::KEY_NO_VALUE,
        _("Who Is Online Window"), KeyboardConfig::GRP_DEFAULT
        | KeyboardConfig::GRP_GUI},
    {"keyWindowDidYouKnow", -1, N_("Did you know Window"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keySocialPrevTab", KeyboardConfig::KEY_NO_VALUE,
        _("Previous Social Tab"), KeyboardConfig::GRP_DEFAULT
        | KeyboardConfig::GRP_GUI},
    {"keySocialNextTab", KeyboardConfig::KEY_NO_VALUE, N_("Next Social Tab"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"", 0, N_("Emotes Keys"), 0},
    {"keySmilie", SDLK_LALT, N_("Smilie"), KeyboardConfig::GRP_DEFAULT},
    {"keyEmoteShortcut1", SDLK_1, strprintf(N_("Emote Shortcut %d"), 1),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut2", SDLK_2, strprintf(N_("Emote Shortcut %d"), 2),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut3", SDLK_3, strprintf(N_("Emote Shortcut %d"), 3),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut4", SDLK_4, strprintf(N_("Emote Shortcut %d"), 4),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut5", SDLK_5, strprintf(N_("Emote Shortcut %d"), 5),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut6", SDLK_6, strprintf(N_("Emote Shortcut %d"), 6),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut7", SDLK_7, strprintf(N_("Emote Shortcut %d"), 7),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut8", SDLK_8, strprintf(N_("Emote Shortcut %d"), 8),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut9", SDLK_9, strprintf(N_("Emote Shortcut %d"), 9),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut10", SDLK_0, strprintf(N_("Emote Shortcut %d"), 10),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut11", SDLK_MINUS, strprintf(N_("Emote Shortcut %d"), 11),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut12", SDLK_EQUALS, strprintf(N_("Emote Shortcut %d"), 12),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut13", SDLK_BACKSPACE,
        strprintf(N_("Emote Shortcut %d"), 13),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut14", SDLK_INSERT, strprintf(N_("Emote Shortcut %d"), 14),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut15", SDLK_HOME, strprintf(N_("Emote Shortcut %d"), 15),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut16", SDLK_q, strprintf(N_("Emote Shortcut %d"), 16),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut17", SDLK_w, strprintf(N_("Emote Shortcut %d"), 17),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut18", SDLK_e, strprintf(N_("Emote Shortcut %d"), 18),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut19", SDLK_r, strprintf(N_("Emote Shortcut %d"), 19),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut20", SDLK_t, strprintf(N_("Emote Shortcut %d"), 20),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut21", SDLK_y, strprintf(N_("Emote Shortcut %d"), 21),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut22", SDLK_u, strprintf(N_("Emote Shortcut %d"), 22),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut23", SDLK_i, strprintf(N_("Emote Shortcut %d"), 23),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut24", SDLK_o, strprintf(N_("Emote Shortcut %d"), 24),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut25", SDLK_p, strprintf(N_("Emote Shortcut %d"), 25),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut26", SDLK_LEFTBRACKET,
        strprintf(N_("Emote Shortcut %d"), 26),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut27", SDLK_RIGHTBRACKET,
        strprintf(N_("Emote Shortcut %d"), 27),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut28", SDLK_BACKSLASH,
        strprintf(N_("Emote Shortcut %d"), 28),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut29", SDLK_a, strprintf(N_("Emote Shortcut %d"), 29),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut30", SDLK_s, strprintf(N_("Emote Shortcut %d"), 30),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut31", SDLK_d, strprintf(N_("Emote Shortcut %d"), 31),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut32", SDLK_f, strprintf(N_("Emote Shortcut %d"), 32),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut33", SDLK_g, strprintf(N_("Emote Shortcut %d"), 33),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut34", SDLK_h, strprintf(N_("Emote Shortcut %d"), 34),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut35", SDLK_j, strprintf(N_("Emote Shortcut %d"), 35),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut36", SDLK_k, strprintf(N_("Emote Shortcut %d"), 36),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut37", SDLK_l, strprintf(N_("Emote Shortcut %d"), 37),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut38", SDLK_SEMICOLON,
        strprintf(N_("Emote Shortcut %d"), 38),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut39", SDLK_QUOTE, strprintf(N_("Emote Shortcut %d"), 39),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut40", SDLK_z, strprintf(N_("Emote Shortcut %d"), 40),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut41", SDLK_x, strprintf(N_("Emote Shortcut %d"), 41),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut42", SDLK_c, strprintf(N_("Emote Shortcut %d"), 42),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut43", SDLK_v, strprintf(N_("Emote Shortcut %d"), 43),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut44", SDLK_b, strprintf(N_("Emote Shortcut %d"), 44),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut45", SDLK_n, strprintf(N_("Emote Shortcut %d"), 45),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut46", SDLK_m, strprintf(N_("Emote Shortcut %d"), 46),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut47", SDLK_COMMA, strprintf(N_("Emote Shortcut %d"), 47),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut48", SDLK_PERIOD, strprintf(N_("Emote Shortcut %d"), 48),
        KeyboardConfig::GRP_EMOTION},
    {"", 0, N_("Outfits Keys"), 0},
    {"keyWearOutfit", SDLK_RCTRL, N_("Wear Outfit"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyCopyOutfit", SDLK_RALT, N_("Copy Outfit"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyCopyEquipedOutfit", SDLK_RIGHTBRACKET, N_("Copy equipped to Outfit"),
        KeyboardConfig::GRP_DEFAULT},
    {"", 0, N_("Chat Keys"), 0},
    {"keyChat", SDLK_RETURN, N_("Toggle Chat"), KeyboardConfig::GRP_DEFAULT
        | KeyboardConfig::GRP_CHAT},
    {"keyChatScrollUp", SDLK_PAGEUP, N_("Scroll Chat Up"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyChatScrollDown", SDLK_PAGEDOWN, N_("Scroll Chat Down"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyChatPrevTab", SDLK_KP7, N_("Previous Chat Tab"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyChatNextTab", SDLK_KP9, N_("Next Chat Tab"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_GUI},
    {"keyChatCloseTab", KeyboardConfig::KEY_NO_VALUE,
        N_("Close current Chat Tab"), KeyboardConfig::GRP_DEFAULT
        | KeyboardConfig::GRP_GUI},
    {"keyChatPrevHistory", SDLK_KP7, N_("Previous chat line"),
        KeyboardConfig::GRP_CHAT},
    {"keyChatNextHistory", SDLK_KP9, N_("Next chat line"),
        KeyboardConfig::GRP_CHAT},
    {"keyAutoCompleteChat", SDLK_TAB, N_("Chat Auto Complete"),
        KeyboardConfig::GRP_CHAT},
    {"keyDeActivateChat", SDLK_ESCAPE, N_("Deactivate Chat Input"),
        KeyboardConfig::GRP_CHAT},
    {"", 0, N_("Other Keys"), 0},
    {"keyIgnoreInput1", SDLK_LSUPER, N_("Ignore input 1"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyIgnoreInput2", SDLK_RSUPER, N_("Ignore input 2"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyDirectUp", SDLK_l, N_("Direct Up"), KeyboardConfig::GRP_DEFAULT},
    {"keyDirectDown", SDLK_SEMICOLON, N_("Direct Down"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyDirectLeft", SDLK_k, N_("Direct Left"), KeyboardConfig::GRP_DEFAULT},
    {"keyDirectRight", SDLK_QUOTE, N_("Direct Right"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyCrazyMoves", SDLK_SLASH, N_("Crazy moves"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyChangeCrazyMoveType", SDLK_BACKSLASH, N_("Change Crazy Move mode"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyQuickDrop", SDLK_y, N_("Quick Drop N Items from 0 slot"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyQuickDropN", SDLK_u, N_("Quick Drop N Items"),
        KeyboardConfig::GRP_DEFAULT},
    {"keySwitchQuickDrop", SDLK_i, N_("Switch Quick Drop Counter"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyMagicInma1", SDLK_c, N_("Quick heal target or self"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyMagicItenplz", SDLK_m, N_("Use #itenplz spell"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyMagicAttack", SDLK_b, N_("Use magic attack"),
        KeyboardConfig::GRP_DEFAULT},
    {"keySwitchMagicAttack", SDLK_COMMA, N_("Switch magic attack"),
        KeyboardConfig::GRP_DEFAULT},
    {"keySwitchPvpAttack", KeyboardConfig::KEY_NO_VALUE,
        N_("Switch pvp attack"), KeyboardConfig::GRP_DEFAULT},
    {"keyInvertDirection", SDLK_KP0, N_("Change move type"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyChangeAttackWeaponType", SDLK_g, N_("Change Attack Weapon Type"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyChangeAttackType", SDLK_END, N_("Change Attack Type"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyChangeFollowMode", SDLK_KP1, N_("Change Follow mode"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyChangeImitationMode", SDLK_KP4, N_("Change Imitation mode"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyDisableGameModifiers", SDLK_KP8,
        _("Disable / Enable Game modifier keys"), KeyboardConfig::GRP_DEFAULT},
    {"keyChangeAudio", SDLK_KP3, N_("On / Off audio"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyAway", SDLK_KP2, N_("Enable / Disable away mode"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyRightClick", SDLK_TAB, N_("Emulate right click from keyboard"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyCameraMode", SDLK_KP_PLUS, N_("Toggle camera mode"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyMod", SDLK_LSHIFT, N_("Modifier key"), KeyboardConfig::GRP_GUI}
};

void KeyboardConfig::init()
{
    for (int i = 0; i < KEY_TOTAL; i++)
    {
        mKey[i].configField = keyData[i].configField;
        mKey[i].defaultValue = keyData[i].defaultValue;
        mKey[i].caption = gettext(keyData[i].caption.c_str());
        mKey[i].value = KEY_NO_VALUE;
        mKey[i].grp = keyData[i].grp;
    }
    for (int i = KEY_EMOTE_1; i <= KEY_EMOTE_48; i ++)
    {
        mKey[i].caption = strprintf(
            _("Emote Shortcut %d"), i - KEY_EMOTE_1 + 1);
    }
    for (int i = KEY_SHORTCUT_1; i <= KEY_SHORTCUT_20; i ++)
    {
        mKey[i].caption = strprintf(
            _("Item Shortcut %d"), i - KEY_SHORTCUT_1 + 1);
    }

    mNewKeyIndex = KEY_NO_VALUE;
    mEnabled = true;

    retrieve();
}

void KeyboardConfig::retrieve()
{
    for (int i = 0; i < KEY_TOTAL; i++)
    {
        if (*mKey[i].configField)
        {
            mKey[i].value = static_cast<int>(config.getValue(
                mKey[i].configField, mKey[i].defaultValue));
        }
    }
}

void KeyboardConfig::store()
{
    for (int i = 0; i < KEY_TOTAL; i++)
    {
        if (*mKey[i].configField)
            config.setValue(mKey[i].configField, mKey[i].value);
    }
}

void KeyboardConfig::makeDefault()
{
    for (int i = 0; i < KEY_TOTAL; i++)
        mKey[i].value = mKey[i].defaultValue;
}

bool KeyboardConfig::hasConflicts()
{
    int i, j;
    /**
     * No need to parse the square matrix: only check one triangle
     * that's enough to detect conflicts
     */
    for (i = 0; i < KEY_TOTAL; i++)
    {
        if (mKey[i].value == KEY_NO_VALUE || !*mKey[i].configField)
            continue;

        for (j = i, j++; j < KEY_TOTAL; j++)
        {
            // Allow for item shortcut and emote keys to overlap
            // as well as emote and ignore keys, but no other keys
            if (mKey[j].value != KEY_NO_VALUE &&
                mKey[i].value == mKey[j].value &&
                ((mKey[i].grp & mKey[j].grp) != 0 &&
                *mKey[i].configField)
               )
            {
                mBindError = strprintf(_("Conflict \"%s\" and \"%s\" keys. "
                                       "Resolve them, or gameplay may result"
                                       " in strange behaviour."),
                                       mKey[i].caption.c_str(),
                                       mKey[j].caption.c_str());
                return true;
            }
        }
    }
    mBindError = "";
    return false;
}

void KeyboardConfig::callbackNewKey()
{
    mSetupKey->newKeyCallback(mNewKeyIndex);
}

int KeyboardConfig::getKeyIndex(int keyValue, int grp) const
{
    for (int i = 0; i < KEY_TOTAL; i++)
    {
        if (keyValue == mKey[i].value &&
            (grp & mKey[i].grp) != 0)
        {
            return i;
        }
    }
    return KEY_NO_VALUE;
}


int KeyboardConfig::getKeyEmoteOffset(int keyValue) const
{
    for (int i = KEY_EMOTE_1; i <= KEY_EMOTE_48; i++)
    {
        if (keyValue == mKey[i].value)
            return 1 + i - KEY_EMOTE_1;
    }
    return 0;
}

bool KeyboardConfig::isActionActive(int index) const
{
    if (!mActiveKeys)
        return false;
    return mActiveKeys[mKey[index].value];
}

void KeyboardConfig::refreshActiveKeys()
{
    mActiveKeys = SDL_GetKeyState(nullptr);
}

std::string KeyboardConfig::getKeyValueString(int index) const
{
    std::string key = SDL_GetKeyName(
        static_cast<SDLKey>(getKeyValue(index)));

    return getKeyShortString(key);
}

std::string KeyboardConfig::getKeyShortString(const std::string &key) const
{
    if (key == "backspace")
        return "bksp";
    else if (key == "unknown key")
        return "u key";
    return key;
}
