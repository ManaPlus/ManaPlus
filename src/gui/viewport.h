/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifdef DYECMD
#include "dyetool/gui/viewport.h"

#else

#include "position.h"

#include "enums/simpletypes/beingid.h"

#include "gui/widgets/windowcontainer.h"

#include "listeners/mouselistener.h"

class ActorSprite;
class Being;
class FloorItem;
class Graphics;
class Map;
class MapItem;

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
        void draw(Graphics *const graphics) override final A_NONNULL(2);

        void safeDraw(Graphics *const graphics) override final A_NONNULL(2);

        /**
         * Implements player to keep following mouse.
         */
        void logic() override final;

        /**
         * Toggles whether the path debug graphics are shown. normal,
         * debug with all images and grid, debug with out big images
         * and with out grid.
         */
        void toggleMapDrawType();

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
         * A relevant config option changed.
         */
        void optionChanged(const std::string &name) override final;

        /**
         * Returns camera x offset in pixels.
         */
        int getCameraX() const noexcept2 A_WARN_UNUSED
        { return mPixelViewX; }

        /**
         * Returns camera y offset in pixels.
         */
        int getCameraY() const noexcept2 A_WARN_UNUSED
        { return mPixelViewY; }

        /**
         * Changes viewpoint by relative pixel coordinates.
         */
        void scrollBy(const int x, const int y)
        { mPixelViewX += x; mPixelViewY += y; }

        /**
         * Clear all hover item\being etc
         */
        void cleanHoverItems();

        Map *getMap() const noexcept2 A_WARN_UNUSED
        { return mMap; }

        void moveCamera(const int dx, const int dy);

        int getCameraRelativeX() const noexcept2 A_WARN_UNUSED
        { return mCameraRelativeX; }

        int getCameraRelativeY() const noexcept2 A_WARN_UNUSED
        { return mCameraRelativeY; }

        void setCameraRelativeX(const int n)
        { mCameraRelativeX = n; updateMidVars(); }

        void setCameraRelativeY(const int n)
        { mCameraRelativeY = n; updateMidVars(); }

        void moveCameraToActor(const BeingId actorId,
                               const int x = 0,
                               const int y = 0);

        void moveCameraToPosition(const int x, const int y);

        void moveCameraRelative(const int x, const int y);

        void returnCamera();

        void getMouseTile(int &destX, int &destY) const;

        void videoResized();

        int mMouseX;                /**< Current mouse position in pixels. */
        int mMouseY;                /**< Current mouse position in pixels. */

    protected:
        friend class ActorManager;

        /// Clears any matching hovers
        void clearHover(const ActorSprite *const actor);

        void updateMidVars();

        void updateMaxVars();

        static void validateSpeed();

    private:
        /**
         * Finds a path from the player to the mouse, and draws it. This is for
         * debug purposes.
         */
        void drawDebugPath(Graphics *const graphics) A_NONNULL(2);

        /**
         * Draws the given path.
         */
        void drawPath(Graphics *const graphics,
                      const Path &path,
                      const Color &color = Color(255, 0, 0))
                      const A_NONNULL(2);

        bool leftMouseAction();

        bool openContextMenu(const MouseEvent &event);

        void walkByMouse(const MouseEvent &event);

        void getMouseTile(const int x,
                          const int y,
                          int &destX,
                          int &destY) const;

        /**
         * Make the player go to the mouse position.
         */
        void followMouse();

        Map *mMap;                   /**< The current map. */

        Being *mHoverBeing;          /**< Being mouse is currently over. */
        FloorItem *mHoverItem;       /**< FloorItem mouse is currently over. */
        MapItem *mHoverSign;         /**< Map sign mouse is currently over. */

        int mScrollRadius;
        int mScrollLaziness;
        int mScrollCenterOffsetX;
        int mScrollCenterOffsetY;
        int mMousePressX;
        int mMousePressY;
        int mPixelViewX;            /**< Current viewpoint in pixels. */
        int mPixelViewY;            /**< Current viewpoint in pixels. */
        int mMidTileX;
        int mMidTileY;
        int mViewXmax;
        int mViewYmax;

        time_t mLocalWalkTime;

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

#endif  // DYECMD
#endif  // GUI_VIEWPORT_H
