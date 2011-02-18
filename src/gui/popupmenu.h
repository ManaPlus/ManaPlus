/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#ifndef POPUP_MENU_H
#define POPUP_MENU_H

#include "gui/widgets/linkhandler.h"
#include "gui/widgets/popup.h"

#include <guichan/actionlistener.hpp>

#ifdef __GNUC__
#define _UNUSED_  __attribute__ ((unused))
#else
#define _UNUSED_
#endif

class Being;
class BrowserBox;
class Button;
class ChatTab;
class FloorItem;
class Item;
class ItemShortcut;
class MapItem;
class TextCommand;
class TextDialog;
class ProgressBar;
class Window;

class RenameListener : public gcn::ActionListener
{
    public:
        void action(const gcn::ActionEvent &event);

        void setMapItem(MapItem* mapItem)
        { mMapItem = mapItem; }

        void setDialog(TextDialog *dialog)
        { mDialog = dialog; }

    private:
        MapItem *mMapItem;
        TextDialog *mDialog;
};

/**
 * Window showing popup menu.
 */
class PopupMenu : public Popup, public LinkHandler
{
    public:
        /**
         * Constructor.
         */
        PopupMenu();

        /**
         * Shows the being related popup menu at the specified mouse coords.
         */
        void showPopup(int x, int y, Being *being);

        /**
         * Shows the beings related popup menu at the specified mouse coords.
         */
        void showPopup(int x, int y, std::list<Being*> &beings);

        void showPlayerPopup(int x, int y, std::string nick);

        /**
         * Shows the floor item related popup menu at the specified
         * mouse coords.
         */
        void showPopup(int x, int y, FloorItem *floorItem);

        /**
         * Shows the related popup menu when right click on the inventory
         * at the specified mouse coordinates.
         */
        void showPopup(Window *parent, int x, int y, Item *item,
                       bool isInventory);

        void showPopup(int x, int y, Button *button);

        void showPopup(int x, int y, ProgressBar *bar);

        void showPopup(int x, int y, MapItem *mapItem);

        void showItemPopup(int x, int y, Item *item);

        void showItemPopup(int x, int y, int itemId);

        void showDropPopup(int x, int y, Item *item);

        void showOutfitsPopup(int x, int y);

        void showSpellPopup(int x, int y, TextCommand *cmd);

        /**
         * Shows the related popup menu when right click on the chat
         * at the specified mouse coordinates.
         */
        void showChatPopup(int x, int y, ChatTab *tab);

        void showChangePos(int x, int y);

        /**
         * Handles link action.
         */
        void handleLink(const std::string &link,
                        gcn::MouseEvent *event _UNUSED_);

    private:
        BrowserBox* mBrowserBox;

        int mBeingId;
        FloorItem* mFloorItem;
        Item *mItem;
        int mItemId;
        MapItem *mMapItem;
        ChatTab *mTab;
        TextCommand *mSpell;
        Window *mWindow;
        RenameListener mRenameListener;
        TextDialog *mDialog;
        Button *mButton;
        std::string mNick;

        /**
         * Shared code for the various showPopup functions.
         */
        void showPopup(int x, int y);
};

#endif
