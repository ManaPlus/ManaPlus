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

#ifndef GUI_VIEWPORT_H
#define GUI_VIEWPORT_H

#include "position.h"

#include "listeners/configlistener.h"

#include "gui/widgets/windowcontainer.h"

#include "listeners/mouselistener.h"

class ActorSprite;
class Button;
class Being;
class BeingPopup;
class ChatTab;
class FloorItem;
class Graphics;
class Item;
class Map;
class MapItem;
class PopupMenu;
class ProgressBar;
class TextCommand;
class TextField;
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
class Viewport final : public WindowContainer,
                       public MouseListener,
                       public ConfigListener
{
    public:
        /**
         * Constructor.
         */
        Viewport();

        A_DELETE_COPY(Viewport)

        /**
         * Destructor.
         */
        ~Viewport();

        /**
         * Sets the map displayed by the viewport.
         */
        void setMap(Map *const map);

        /**
         * Draws the viewport.
         */
        void draw(Graphics *graphics) override final;

        /**
         * Implements player to keep following mouse.
         */
        void logic() override final;

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
        void mousePressed(MouseEvent &event) override final;

        /**
         * Handles mouse move on map
         */
        void mouseDragged(MouseEvent &event) override final;

        /**
         * Handles mouse button release on map.
         */
        void mouseReleased(MouseEvent &event) override final;

        /**
         * Handles mouse move on map.
         */
        void mouseMoved(MouseEvent &event) override final;

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

        void showPlayerPopup(const std::string &nick);

        void showOutfitsPopup(const int x, const int y);

        void showOutfitsPopup();

        void showSpellPopup(TextCommand *const cmd);

        void showAttackMonsterPopup(const std::string &name,
                                    const int type);

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

        void showTextFieldPopup(TextField *const input);

        void showLinkPopup(const std::string &link);

        void showWindowsPopup();

        void showNpcDialogPopup(const int npcId);

        void showWindowPopup(Window *const window);

        /**
         * Closes the popup menu. Needed for when the player dies or switching
         * maps.
         */
        void closePopupMenu();

        /**
         * A relevant config option changed.
         */
        void optionChanged(const std::string &name) override final;

        /**
         * Returns camera x offset in pixels.
         */
        int getCameraX() const A_WARN_UNUSED
        { return mPixelViewX; }

        /**
         * Returns camera y offset in pixels.
         */
        int getCameraY() const A_WARN_UNUSED
        { return mPixelViewY; }

        /**
         * Returns mouse x in pixels.
         */
        int getMouseX() const A_WARN_UNUSED
        { return mMouseX; }

        /**
         * Returns mouse y in pixels.
         */
        int getMouseY() const A_WARN_UNUSED
        { return mMouseY; }

        /**
         * Changes viewpoint by relative pixel coordinates.
         */
        void scrollBy(const int x, const int y)
        { mPixelViewX += x; mPixelViewY += y; }

        int getDebugPath() const A_WARN_UNUSED
        { return mShowDebugPath; }

        void setDebugPath(const int n)
        { mShowDebugPath = n; }

        int getCameraMode() const A_WARN_UNUSED
        { return mCameraMode; }

        /**
         * Hides the BeingPopup.
         */
        void hideBeingPopup();

        /**
         * Clear all hover item\being etc
         */
        void cleanHoverItems();

        Map *getMap() const A_WARN_UNUSED
        { return mMap; }

        void moveCamera(const int dx, const int dy);

        int getCameraRelativeX() const A_WARN_UNUSED
        { return mCameraRelativeX; }

        int getCameraRelativeY() const A_WARN_UNUSED
        { return mCameraRelativeY; }

        void setCameraRelativeX(const int n)
        { mCameraRelativeX = n; }

        void setCameraRelativeY(const int n)
        { mCameraRelativeY = n; }

        bool isPopupMenuVisible() const A_WARN_UNUSED;

        void moveCameraToActor(const int actorId,
                               const int x = 0,
                               const int y = 0);

        void moveCameraToPosition(const int x, const int y);

        void moveCameraRelative(const int x, const int y);

        void returnCamera();

        void clearPopup();

    protected:
        friend class ActorManager;

        /// Clears any matching hovers
        void clearHover(const ActorSprite *const actor);

        static void validateSpeed();

    private:
        /**
         * Finds a path from the player to the mouse, and draws it. This is for
         * debug purposes.
         */
        void drawDebugPath(Graphics *const graphics);

        /**
         * Draws the given path.
         */
        void drawPath(Graphics *const graphics,
                      const Path &path,
                      const Color &color = Color(255, 0, 0)) const;

        bool leftMouseAction();

        bool openContextMenu(const MouseEvent &event);

        void walkByMouse(const MouseEvent &event);

        /**
         * Make the player go to the mouse position.
         */
        void followMouse();

        Map *mMap;                   /**< The current map. */

        PopupMenu *mPopupMenu;       /**< Popup menu. */
        Being *mHoverBeing;          /**< Being mouse is currently over. */
        FloorItem *mHoverItem;       /**< FloorItem mouse is currently over. */
        MapItem *mHoverSign;         /**< Map sign mouse is currently over. */
        BeingPopup *mBeingPopup;     /**< Being information popup. */
        TextPopup *mTextPopup;       /**< Map Item information popup. */

        int mScrollRadius;
        int mScrollLaziness;
        int mScrollCenterOffsetX;
        int mScrollCenterOffsetY;
        int mMouseX;                /**< Current mouse position in pixels. */
        int mMouseY;                /**< Current mouse position in pixels. */
        int mMousePressX;
        int mMousePressY;
        int mPixelViewX;            /**< Current viewpoint in pixels. */
        int mPixelViewY;            /**< Current viewpoint in pixels. */
        int mShowDebugPath;         /**< Show a path from player to pointer. */
        int mCameraMode;            /**< Camera mode. */

        int mLocalWalkTime; /**< Timestamp before the next walk can be sent. */

        int mCameraRelativeX;
        int mCameraRelativeY;

        bool mShowBeingPopup;
        bool mSelfMouseHeal;
        bool mEnableLazyScrolling;
        bool mMouseDirectionMove;
        bool mLongMouseClick;
        bool mMouseClicked;
        bool mPlayerFollowMouse;
};

extern Viewport *viewport;           /**< The viewport. */

#endif  // GUI_VIEWPORT_H
