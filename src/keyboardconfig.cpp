/*
 *  Custom keyboard shortcuts configuration
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
 *
 *  This file is part of The Mana Client.
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
#include "log.h"

#include "gui/sdlinput.h"
#include "gui/setup_keyboard.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

struct KeyData
{
    const char *configField;
    int defaultValue;
    std::string caption;
    int grp;
};

// keyData must be in same order as enum keyAction.
static KeyData const keyData[KeyboardConfig::KEY_TOTAL] = {
    {"keyMoveUp", SDLK_UP, _("Move Up"), KeyboardConfig::GRP_DEFAULT},
    {"keyMoveDown", SDLK_DOWN, _("Move Down"), KeyboardConfig::GRP_DEFAULT},
    {"keyMoveLeft", SDLK_LEFT, _("Move Left"), KeyboardConfig::GRP_DEFAULT},
    {"keyMoveRight", SDLK_RIGHT, _("Move Right"), KeyboardConfig::GRP_DEFAULT},
    {"keyAttack", SDLK_LCTRL, _("Attack"), KeyboardConfig::GRP_DEFAULT},
    {"keyTargetAttack", SDLK_x, _("Target & Attack"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyMoveToTarget", SDLK_v, _("Move to Target"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyChangeMoveToTarget", SDLK_PERIOD, _("Change Move to Target type"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyMoveToHome", SDLK_d, _("Move to Home location"),
        KeyboardConfig::GRP_DEFAULT},
    {"keySetHome", SDLK_KP5, _("Set home location"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyMoveToPoint", SDLK_RSHIFT, _("Move to navigation point"),
        KeyboardConfig::GRP_DEFAULT},
    {"keySmilie", SDLK_LALT, _("Smilie"), KeyboardConfig::GRP_DEFAULT},
    {"keyTalk", SDLK_t, _("Talk"), KeyboardConfig::GRP_DEFAULT},
    {"keyTarget", SDLK_LSHIFT, _("Stop Attack"), KeyboardConfig::GRP_DEFAULT},
    {"keyTargetClosest", SDLK_a, _("Target Closest"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyTargetNPC", SDLK_n, _("Target NPC"), KeyboardConfig::GRP_DEFAULT},
    {"keyTargetPlayer", SDLK_q, _("Target Player"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyPickup", SDLK_z, _("Pickup"), KeyboardConfig::GRP_DEFAULT},
    {"keyChangePickupType", SDLK_o, _("Change Pickup Type"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyHideWindows", SDLK_h, _("Hide Windows"), KeyboardConfig::GRP_DEFAULT},
    {"keyBeingSit", SDLK_s, _("Sit"), KeyboardConfig::GRP_DEFAULT},
    {"keyScreenshot", SDLK_p, _("Screenshot"), KeyboardConfig::GRP_DEFAULT},
    {"keyTrade", SDLK_r, _("Enable/Disable Trading"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyPathfind", SDLK_f, _("Change Map View Mode"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcutsKey", SDLK_MENU, _("Item Shortcuts Key"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut1", SDLK_1, strprintf(_("Item Shortcut %d"), 1),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut2", SDLK_2, strprintf(_("Item Shortcut %d"), 2),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut3", SDLK_3, strprintf(_("Item Shortcut %d"), 3),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut4", SDLK_4, strprintf(_("Item Shortcut %d"), 4),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut5", SDLK_5, strprintf(_("Item Shortcut %d"), 5),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut6", SDLK_6, strprintf(_("Item Shortcut %d"), 6),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut7", SDLK_7, strprintf(_("Item Shortcut %d"), 7),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut8", SDLK_8, strprintf(_("Item Shortcut %d"), 8),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut9", SDLK_9, strprintf(_("Item Shortcut %d"), 9),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut10", SDLK_0, strprintf(_("Item Shortcut %d"), 10),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut11", SDLK_MINUS, strprintf(_("Item Shortcut %d"), 11),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut12", SDLK_EQUALS, strprintf(_("Item Shortcut %d"), 12),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut13", SDLK_BACKSPACE, strprintf(_("Item Shortcut %d"), 13),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut14", SDLK_INSERT, strprintf(_("Item Shortcut %d"), 14),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut15", SDLK_HOME, strprintf(_("Item Shortcut %d"), 15),
        KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut16", KeyboardConfig::KEY_NO_VALUE,
        strprintf(_("Item Shortcut %d"), 16), KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut17", KeyboardConfig::KEY_NO_VALUE,
        strprintf(_("Item Shortcut %d"), 17), KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut18", KeyboardConfig::KEY_NO_VALUE,
        strprintf(_("Item Shortcut %d"), 18), KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut19", KeyboardConfig::KEY_NO_VALUE,
        strprintf(_("Item Shortcut %d"), 19), KeyboardConfig::GRP_DEFAULT},
    {"keyShortcut20", KeyboardConfig::KEY_NO_VALUE,
        strprintf(_("Item Shortcut %d"), 20), KeyboardConfig::GRP_DEFAULT},
    {"keyWindowHelp", SDLK_F1, _("Help Window"), KeyboardConfig::GRP_DEFAULT},
    {"keyWindowStatus", SDLK_F2, _("Status Window"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyWindowInventory", SDLK_F3, _("Inventory Window"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyWindowEquipment", SDLK_F4, _("Equipment Window"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyWindowSkill", SDLK_F5, _("Skill Window"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyWindowMinimap", SDLK_F6, _("Minimap Window"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyWindowChat", SDLK_F7, _("Chat Window"), KeyboardConfig::GRP_DEFAULT},
    {"keyWindowShortcut", SDLK_F8, _("Item Shortcut Window"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyWindowSetup", SDLK_F9, _("Setup Window"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyWindowDebug", SDLK_F10, _("Debug Window"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyWindowSocial", SDLK_F11, _("Social Window"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyWindowEmoteBar", SDLK_F12, _("Emote Shortcut Window"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyWindowOutfit", SDLK_BACKQUOTE, _("Outfits Window"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyWindowShop", -1, _("Shop Window"), KeyboardConfig::GRP_DEFAULT},
    {"keyWindowDrop", SDLK_w, _("Quick drop Window"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyWindowKills", SDLK_e, _("Kills Stats Window"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyWindowSpells", SDLK_j, _("Commands Window"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyWindowBotChecker", SDLK_LEFTBRACKET, _("Bot Checker Window"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyWindowOnline", KeyboardConfig::KEY_NO_VALUE,
        _("Who Is Online Window"), KeyboardConfig::GRP_DEFAULT},
    {"keySocialPrevTab", KeyboardConfig::KEY_NO_VALUE,
        _("Previous Social Tab"), KeyboardConfig::GRP_DEFAULT},
    {"keySocialNextTab", KeyboardConfig::KEY_NO_VALUE, _("Next Social Tab"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyEmoteShortcut1", SDLK_1, strprintf(_("Emote Shortcut %d"), 1),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut2", SDLK_2, strprintf(_("Emote Shortcut %d"), 2),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut3", SDLK_3, strprintf(_("Emote Shortcut %d"), 3),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut4", SDLK_4, strprintf(_("Emote Shortcut %d"), 4),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut5", SDLK_5, strprintf(_("Emote Shortcut %d"), 5),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut6", SDLK_6, strprintf(_("Emote Shortcut %d"), 6),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut7", SDLK_7, strprintf(_("Emote Shortcut %d"), 7),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut8", SDLK_8, strprintf(_("Emote Shortcut %d"), 8),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut9", SDLK_9, strprintf(_("Emote Shortcut %d"), 9),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut10", SDLK_0, strprintf(_("Emote Shortcut %d"), 10),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut11", SDLK_MINUS, strprintf(_("Emote Shortcut %d"), 11),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut12", SDLK_EQUALS, strprintf(_("Emote Shortcut %d"), 12),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut13", SDLK_BACKSPACE,
        strprintf(_("Emote Shortcut %d"), 13),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut14", SDLK_INSERT, strprintf(_("Emote Shortcut %d"), 14),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut15", SDLK_HOME, strprintf(_("Emote Shortcut %d"), 15),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut16", SDLK_q, strprintf(_("Emote Shortcut %d"), 16),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut17", SDLK_w, strprintf(_("Emote Shortcut %d"), 17),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut18", SDLK_e, strprintf(_("Emote Shortcut %d"), 18),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut19", SDLK_r, strprintf(_("Emote Shortcut %d"), 19),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut20", SDLK_t, strprintf(_("Emote Shortcut %d"), 20),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut21", SDLK_y, strprintf(_("Emote Shortcut %d"), 21),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut22", SDLK_u, strprintf(_("Emote Shortcut %d"), 22),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut23", SDLK_i, strprintf(_("Emote Shortcut %d"), 23),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut24", SDLK_o, strprintf(_("Emote Shortcut %d"), 24),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut25", SDLK_p, strprintf(_("Emote Shortcut %d"), 25),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut26", SDLK_LEFTBRACKET,
        strprintf(_("Emote Shortcut %d"), 26),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut27", SDLK_RIGHTBRACKET,
        strprintf(_("Emote Shortcut %d"), 27),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut28", SDLK_BACKSLASH,
        strprintf(_("Emote Shortcut %d"), 28),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut29", SDLK_a, strprintf(_("Emote Shortcut %d"), 29),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut30", SDLK_s, strprintf(_("Emote Shortcut %d"), 30),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut31", SDLK_d, strprintf(_("Emote Shortcut %d"), 31),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut32", SDLK_f, strprintf(_("Emote Shortcut %d"), 32),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut33", SDLK_g, strprintf(_("Emote Shortcut %d"), 33),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut34", SDLK_h, strprintf(_("Emote Shortcut %d"), 34),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut35", SDLK_j, strprintf(_("Emote Shortcut %d"), 35),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut36", SDLK_k, strprintf(_("Emote Shortcut %d"), 36),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut37", SDLK_l, strprintf(_("Emote Shortcut %d"), 37),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut38", SDLK_SEMICOLON,
        strprintf(_("Emote Shortcut %d"), 38),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut39", SDLK_QUOTE, strprintf(_("Emote Shortcut %d"), 39),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut40", SDLK_z, strprintf(_("Emote Shortcut %d"), 40),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut41", SDLK_x, strprintf(_("Emote Shortcut %d"), 41),
        KeyboardConfig::GRP_EMOTION},
    {"keyEmoteShortcut42", SDLK_c, strprintf(_("Emote Shortcut %d"), 42),
        KeyboardConfig::GRP_EMOTION},
    {"keyWearOutfit", SDLK_RCTRL, _("Wear Outfit"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyCopyOutfit", SDLK_RALT, _("Copy Outfit"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyCopyEquipedOutfit", SDLK_RIGHTBRACKET, _("Copy Equiped to Outfit"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyChat", SDLK_RETURN, _("Toggle Chat"),
        KeyboardConfig::GRP_DEFAULT | KeyboardConfig::GRP_CHAT},
    {"keyChatScrollUp", SDLK_PAGEUP, _("Scroll Chat Up"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyChatScrollDown", SDLK_PAGEDOWN, _("Scroll Chat Down"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyChatPrevTab", SDLK_KP7, _("Previous Chat Tab"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyChatNextTab", SDLK_KP9, _("Next Chat Tab"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyChatPrevHistory", SDLK_KP7, _("Previous chat tab line"),
        KeyboardConfig::GRP_CHAT},
    {"keyChatNextHistory", SDLK_KP9, _("Next chat tab line"),
        KeyboardConfig::GRP_CHAT},
    {"keyAutoCompleteChat", SDLK_TAB, _("Chat Auto Complete"),
        KeyboardConfig::GRP_CHAT},
    {"keyDeActivateChat", SDLK_ESCAPE, _("Deactivate Chat Input"),
        KeyboardConfig::GRP_CHAT},
    {"keyOK", SDLK_SPACE, _("Select OK"), KeyboardConfig::GRP_DEFAULT},
    {"keyQuit", SDLK_ESCAPE, _("Quit"), KeyboardConfig::GRP_DEFAULT},
    {"keyIgnoreInput1", SDLK_LSUPER, _("Ignore input 1"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyIgnoreInput2", SDLK_RSUPER, _("Ignore input 2"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyDirectUp", SDLK_l, _("Direct Up"), KeyboardConfig::GRP_DEFAULT},
    {"keyDirectDown", SDLK_SEMICOLON, _("Direct Down"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyDirectLeft", SDLK_k, _("Direct Left"), KeyboardConfig::GRP_DEFAULT},
    {"keyDirectRight", SDLK_QUOTE, _("Direct Right"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyCrazyMoves", SDLK_SLASH, _("Crazy moves"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyChangeCrazyMoveType", SDLK_BACKSLASH, _("Change Crazy Move mode"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyQuickDrop", SDLK_y, _("Quick Drop N Items from 0 slot"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyQuickDropN", SDLK_u, _("Quick Drop N Items"),
        KeyboardConfig::GRP_DEFAULT},
    {"keySwitchQuickDrop", SDLK_i, _("Switch Quick Drop Counter"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyMagicInma1", SDLK_c, _("Quick heal target or self"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyMagicItenplz", SDLK_m, _("Use #itenplz spell"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyMagicAttack", SDLK_b, _("Use magic attack"),
        KeyboardConfig::GRP_DEFAULT},
    {"keySwitchMagicAttack", SDLK_COMMA, _("Switch magic attack"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyInvertDirection", SDLK_KP0, _("Change move type"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyChangeAttackWeaponType", SDLK_g, _("Change Attack Weapon Type"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyChangeAttackType", SDLK_END, _("Change Attack Type"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyChangeFollowMode", SDLK_KP1, _("Change Follow mode"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyChangeImitationMode", SDLK_KP4, _("Change Imitation mode"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyDisableGameModifiers", SDLK_KP8,
        _("Disbale / Enable Game modifier keys"), KeyboardConfig::GRP_DEFAULT},
    {"keyChangeAudio", SDLK_KP3, _("On / Off audio"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyAway", SDLK_KP2, _("Enable / Disable away mode"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyRightClick", SDLK_TAB, _("Emulate right click from keyboard"),
        KeyboardConfig::GRP_DEFAULT},
    {"keyCameraMode", SDLK_KP_PLUS, _("Toggle camera mode"),
        KeyboardConfig::GRP_DEFAULT}
};

void KeyboardConfig::init()
{
    for (int i = 0; i < KEY_TOTAL; i++)
    {
        mKey[i].configField = keyData[i].configField;
        mKey[i].defaultValue = keyData[i].defaultValue;
        mKey[i].caption = keyData[i].caption;
        mKey[i].value = KEY_NO_VALUE;
        mKey[i].grp = keyData[i].grp;
    }
    mNewKeyIndex = KEY_NO_VALUE;
    mEnabled = true;

    retrieve();
}

void KeyboardConfig::retrieve()
{
    for (int i = 0; i < KEY_TOTAL; i++)
    {
        mKey[i].value = (int) config.getValue(
            mKey[i].configField, mKey[i].defaultValue);
    }
}

void KeyboardConfig::store()
{
    for (int i = 0; i < KEY_TOTAL; i++)
        config.setValue(mKey[i].configField, mKey[i].value);
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
        if (mKey[i].value == KEY_NO_VALUE)
            continue;

        for (j = i, j++; j < KEY_TOTAL; j++)
        {
            // Allow for item shortcut and emote keys to overlap
            // as well as emote and ignore keys, but no other keys
            if (mKey[j].value != KEY_NO_VALUE &&
                mKey[i].value == mKey[j].value &&
                ((mKey[i].grp & mKey[j].grp) != 0)
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
    for (int i = KEY_EMOTE_1; i <= KEY_EMOTE_42; i++)
    {
        if (keyValue == mKey[i].value)
            return 1 + i - KEY_EMOTE_1;
    }
    return 0;
}

bool KeyboardConfig::isKeyActive(int index) const
{
    if (!mActiveKeys)
        return false;
    return mActiveKeys[mKey[index].value];
}

void KeyboardConfig::refreshActiveKeys()
{
    mActiveKeys = SDL_GetKeyState(NULL);
}

std::string KeyboardConfig::getKeyValueString(int index) const
{
    std::string key = SDL_GetKeyName(
        (SDLKey) getKeyValue(index));

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