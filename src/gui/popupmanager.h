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

#ifndef GUI_POPUPMANAGER_H
#define GUI_POPUPMANAGER_H

#include "being/actortype.h"

#include <string>
#include <vector>

#include "localconsts.h"

class ActorSprite;
class Button;
class Being;
class BeingPopup;
class ChatTab;
class FloorItem;
class Item;
class MapItem;
class PopupMenu;
class ProgressBar;
class TextCommand;
class TextField;
class TextPopup;
class Window;

class PopupManager final
{
    public:
        /**
         * Constructor.
         */
        PopupManager();

        A_DELETE_COPY(PopupManager)

        /**
         * Destructor.
         */
        ~PopupManager();

        /**
         * Shows a popup for an item.
         * TODO Find some way to get rid of Item here
         */
        void showPopup(Window *const parent,
                       const int x, const int y,
                       Item *const item,
                       const bool isInventory = true);

        /**
         * Shows a popup for an item.
         * TODO Find some way to get rid of Item here
         */
        void showPopup(Window *const parent,
                       Item *const item,
                       const bool isInventory = true);

        void showPopup(const int x, const int y,
                       Button *const button);

        void showPopup(const int x, const int y,
                       const ProgressBar *const bar);

        void showPopup(MapItem *const item);

        void showPopup(const int x, const int y, MapItem *const mapItem);

        void showItemPopup(Item *const item);

        void showItemPopup(const int itemId,
                           const unsigned char color = 1);

        void showDropPopup(Item *const item);

        /**
         * Shows a popup for being.
         */
        void showPopup(const int x, const int y,
                       const Being *const being);

        void showPopup(const Being *const being);

        void showPopup(const int x, const int y,
                       const std::vector<ActorSprite*> &beings);

        void showPopup(const int x, const int y,
                       const FloorItem *const floorItem);

        void showPlayerPopup(const std::string &nick);

        void showOutfitsPopup(const int x, const int y);

        void showOutfitsPopup();

        void showSpellPopup(TextCommand *const cmd);

        void showAttackMonsterPopup(const std::string &name,
                                    const ActorType::Type &type);

        void showPickupItemPopup(const std::string &name);

        /**
         * Shows the related popup menu when right click on the chat
         * at the specified mouse coordinates.
         */
        void showChatPopup(const int x, const int y,
                           ChatTab *const tab);

        /**
         * Shows the related popup menu when right click on the chat
         */
        void showChatPopup(ChatTab *const tab);

        void showUndressPopup(const int x, const int y,
                              const Being *const being,
                              Item *const item);

        void showMapPopup(const int x, const int y);

        void showMapPopup(const int x1, const int y1,
                          const int x2, const int y2);

        void showTextFieldPopup(TextField *const input);

        void showLinkPopup(const std::string &link);

        void showWindowsPopup();

        void showNpcDialogPopup(const int npcId);

        void showWindowPopup(Window *const window);

        void showBeingPopup(const int x, const int y,
                            Being *const b);

        void showTextPopup(const int x, const int y,
                           const std::string &text);

        bool isBeingPopupVisible() const;

        bool isTextPopupVisible() const;

        /**
         * Closes the popup menu. Needed for when the player dies or switching
         * maps.
         */
        void closePopupMenu();

        /**
         * Hides the BeingPopup.
         */
        void hideBeingPopup();

        void hideTextPopup();

        bool isPopupMenuVisible() const A_WARN_UNUSED;

        void clearPopup();

        void hidePopupMenu();

    private:
        BeingPopup *mBeingPopup;     /**< Being information popup. */
        TextPopup *mTextPopup;       /**< Map Item information popup. */
};

extern PopupManager *popupManager;

#endif  // GUI_POPUPMANAGER_H
