/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "actorspritemanager.h"
#include "configlistener.h"
#include "position.h"

#include "gui/widgets/windowcontainer.h"

#include <guichan/mouselistener.hpp>

class ActorSprite;
class Button;
class Being;
class BeingPopup;
class ChatTab;
class FloorItem;
class Graphics;
class ImageSet;
class Item;
class ItemShortcut;
class Map;
class PopupMenu;
class ProgressBar;
class StatusPopup;
class TextCommand;
class TextPopup;
class Window;

/** Delay between two mouse calls when dragging mouse and move the player */
const int walkingMouseDelay = 500;

/**
 * The viewport on the map. Displays the current map and handles mouse input
 * and the popup menu.
 *
 * TODO: This class is planned to be extended to allow floating widgets on top
 * of it such as NPC messages, which are positioned using map pixel
 * coordinates.
 */
class Viewport : public WindowContainer, public gcn::MouseListener,
    public ConfigListener
{
    public:
        /**
         * Constructor.
         */
        Viewport();

        /**
         * Destructor.
         */
        ~Viewport();

        /**
         * Sets the map displayed by the viewport.
         */
        void setMap(Map *map);

        /**
         * Draws the viewport.
         */
        void draw(gcn::Graphics *graphics);

        /**
         * Implements player to keep following mouse.
         */
        void logic();

        /**
         * Toggles whether the path debug graphics are shown. normal,
         * debug with all images and grid, debug with out big images
         * and with out grid.
         */
        void toggleDebugPath();

        void toggleCameraMode();

        /**
         * Handles mouse press on map.
         */
        void mousePressed(gcn::MouseEvent &event);

        /**
         * Handles mouse move on map
         */
        void mouseDragged(gcn::MouseEvent &event);

        /**
         * Handles mouse button release on map.
         */
        void mouseReleased(gcn::MouseEvent &event);

        /**
         * Handles mouse move on map.
         */
        void mouseMoved(gcn::MouseEvent &event);

        /**
         * Shows a popup for an item.
         * TODO Find some way to get rid of Item here
         */
        void showPopup(Window *parent, int x, int y, Item *item,
                       bool isInventory = true);

        /**
         * Shows a popup for an item.
         * TODO Find some way to get rid of Item here
         */
        void showPopup(Window *parent, Item *item, bool isInventory = true);

        void showPopup(int x, int y, Button *button);

        void showPopup(int x, int y, ProgressBar *bar);

        void showPopup(MapItem *item);

        void showItemPopup(Item *item);

        void showItemPopup(int itemId, unsigned char color = 1);

        void showDropPopup(Item *item);

        /**
         * Shows a popup for being.
         */
        void showPopup(int x, int y, Being *being);

        void showPopup(Being *being);

        void showPlayerPopup(std::string nick);

        void showOutfitsPopup(int x, int y);

        void showOutfitsPopup();

        void showSpellPopup(TextCommand *cmd);

        void showAttackMonsterPopup(std::string name, int type);

        /**
         * Shows the related popup menu when right click on the chat
         * at the specified mouse coordinates.
         */
        void showChatPopup(int x, int y, ChatTab *tab);

        /**
         * Shows the related popup menu when right click on the chat
         */
        void showChatPopup(ChatTab *tab);

        void showUndressPopup(int x, int y, Being *being, Item *item);

        void showMapPopup(int x, int y);

        /**
         * Closes the popup menu. Needed for when the player dies or switching
         * maps.
         */
        void closePopupMenu();

        /**
         * A relevant config option changed.
         */
        void optionChanged(const std::string &name);

        /**
         * Returns camera x offset in pixels.
         */
        int getCameraX() const
        { return static_cast<int>(mPixelViewX); }

        /**
         * Returns camera y offset in pixels.
         */
        int getCameraY() const
        { return static_cast<int>(mPixelViewY); }

        /**
         * Returns mouse x in pixels.
         */
        int getMouseX() const
        { return mMouseX; }

        /**
         * Returns mouse y in pixels.
         */
        int getMouseY() const
        { return mMouseY; }

        /**
         * Changes viewpoint by relative pixel coordinates.
         */
        void scrollBy(float x, float y)
        { mPixelViewX += x; mPixelViewY += y; }

        /**
         * Returns the current map object.
         */
        Map *getCurrentMap() const
        { return mMap; }

        int getDebugPath() const
        { return mShowDebugPath; }

        void setDebugPath(int n)
        { mShowDebugPath = n; }

        int getCameraMode() const
        { return mCameraMode; }

        /**
         * Hides the BeingPopup.
         */
        void hideBeingPopup();

        /**
         * Clear all hover item\being etc
         */
        void cleanHoverItems();

        Map *getMap() const
        { return mMap; }

        void moveCamera(int dx, int dy);

        int getCameraRelativeX() const
        { return mCameraRelativeX; }

        int getCameraRelativeY() const
        { return mCameraRelativeY; }

        void setCameraRelativeX(int n)
        { mCameraRelativeX = n; }

        void setCameraRelativeY(int n)
        { mCameraRelativeY = n; }

        bool isPopupMenuVisible();

        void moveCameraToActor(int actorId, int x = 0, int y = 0);

        void moveCameraToPosition(int x, int y);

        void moveCameraRelative(int x, int y);

        void returnCamera();

    protected:
        friend class ActorSpriteManager;

        /// Clears any matching hovers
        void clearHover(ActorSprite *actor);

        void validateSpeed();

    private:
        /**
         * Finds a path from the player to the mouse, and draws it. This is for
         * debug purposes.
         */
        void _drawDebugPath(Graphics *graphics);

        /**
         * Draws the given path.
         */
        void _drawPath(Graphics *graphics, const Path &path,
                       gcn::Color color = gcn::Color(255, 0, 0));

        /**
         * Make the player go to the mouse position.
         */
        void _followMouse();

        Map *mMap;                   /**< The current map. */

        int mScrollRadius;
        int mScrollLaziness;
        bool mShowBeingPopup;
        bool mSelfMouseHeal;
        bool mEnableLazyScrolling;
        int mScrollCenterOffsetX;
        int mScrollCenterOffsetY;
        int mMouseX;                /**< Current mouse position in pixels. */
        int mMouseY;                /**< Current mouse position in pixels. */
        float mPixelViewX;          /**< Current viewpoint in pixels. */
        float mPixelViewY;          /**< Current viewpoint in pixels. */
        int mShowDebugPath;         /**< Show a path from player to pointer. */
        int mCameraMode;            /**< Camera mode. */

        bool mPlayerFollowMouse;

        int mLocalWalkTime; /**< Timestamp before the next walk can be sent. */

        PopupMenu *mPopupMenu;       /**< Popup menu. */
        Being *mHoverBeing;          /**< Being mouse is currently over. */
        FloorItem *mHoverItem;       /**< FloorItem mouse is currently over. */
        MapItem *mHoverSign;         /**< Map sign mouse is currently over. */
        BeingPopup *mBeingPopup;     /**< Being information popup. */
        TextPopup *mTextPopup;       /**< Map Item information popup. */

        int mCameraRelativeX;
        int mCameraRelativeY;
};

extern Viewport *viewport;           /**< The viewport. */

#endif
