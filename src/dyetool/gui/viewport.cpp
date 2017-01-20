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

#include "dyetool/gui/viewport.h"

#include "configuration.h"
#include "sdlshared.h"

#include "gui/gui.h"
#include "gui/popupmanager.h"

#include "render/graphics.h"

#include "debug.h"

Viewport *viewport = nullptr;

extern volatile int tick_time;

Viewport::Viewport() :
    WindowContainer(nullptr),
    MouseListener(),
    ConfigListener(),
    mMouseX(0),
    mMouseY(0),
    mMap(nullptr),
    mHoverBeing(nullptr),
    mHoverItem(nullptr),
    mHoverSign(nullptr),
    mScrollRadius(config.getIntValue("ScrollRadius")),
    mScrollLaziness(config.getIntValue("ScrollLaziness")),
    mScrollCenterOffsetX(config.getIntValue("ScrollCenterOffsetX")),
    mScrollCenterOffsetY(config.getIntValue("ScrollCenterOffsetY")),
    mMousePressX(0),
    mMousePressY(0),
    mPixelViewX(0),
    mPixelViewY(0),
    mLocalWalkTime(-1),
    mCameraRelativeX(0),
    mCameraRelativeY(0),
    mShowBeingPopup(config.getBoolValue("showBeingPopup")),
    mSelfMouseHeal(config.getBoolValue("selfMouseHeal")),
    mEnableLazyScrolling(config.getBoolValue("enableLazyScrolling")),
    mMouseDirectionMove(config.getBoolValue("mouseDirectionMove")),
    mLongMouseClick(config.getBoolValue("longmouseclick")),
    mMouseClicked(false),
    mPlayerFollowMouse(false)
{
    setOpaque(Opaque_false);
    addMouseListener(this);

    config.addListener("ScrollLaziness", this);
    config.addListener("ScrollRadius", this);
    config.addListener("showBeingPopup", this);
    config.addListener("selfMouseHeal", this);
    config.addListener("enableLazyScrolling", this);
    config.addListener("mouseDirectionMove", this);
    config.addListener("longmouseclick", this);

    setFocusable(true);
}

Viewport::~Viewport()
{
    config.removeListeners(this);
    CHECKLISTENERS
}

void Viewport::setMap(Map *const map A_UNUSED)
{
}

void Viewport::draw(Graphics *const graphics)
{
    BLOCK_START("Viewport::draw 1")
    static int lastTick = tick_time;

    graphics->setColor(Color(64, 64, 64));
    graphics->fillRectangle(
        Rect(0, 0, getWidth(), getHeight()));

    // Avoid freaking out when tick_time overflows
    if (tick_time < lastTick)
        lastTick = tick_time;

    // Draw contained widgets
    WindowContainer::draw(graphics);
    BLOCK_END("Viewport::draw 1")
}

void Viewport::safeDraw(Graphics *const graphics)
{
    Viewport::draw(graphics);
}

void Viewport::logic()
{
    BLOCK_START("Viewport::logic")
    // Make the player follow the mouse position
    // if the mouse is dragged elsewhere than in a window.
    Gui::getMouseState(mMouseX, mMouseY);
    BLOCK_END("Viewport::logic")
}

void Viewport::followMouse()
{
}

void Viewport::drawDebugPath(Graphics *const graphics A_UNUSED)
{
}

bool Viewport::openContextMenu(const MouseEvent &event A_UNUSED)
{
    return false;
}

void Viewport::mousePressed(MouseEvent &event)
{
    if (event.getSource() != this || event.isConsumed())
        return;

    mMouseClicked = true;

    mMousePressX = event.getX();
    mMousePressY = event.getY();
    const MouseButtonT eventButton = event.getButton();

    // Right click might open a popup
    if (eventButton == MouseButton::RIGHT)
    {
        if (openContextMenu(event))
            return;
    }

    // If a popup is active, just remove it
    if (popupManager->isPopupMenuVisible())
    {
        mPlayerFollowMouse = false;
        popupManager->hidePopupMenu();
        return;
    }

    // Left click can cause different actions
    if (!mLongMouseClick && eventButton == MouseButton::LEFT)
    {
        if (leftMouseAction())
        {
            mPlayerFollowMouse = false;
            return;
        }
    }
}

void Viewport::walkByMouse(const MouseEvent &event A_UNUSED)
{
}

void Viewport::mouseDragged(MouseEvent &event)
{
    if (event.getSource() != this || event.isConsumed())
    {
        mPlayerFollowMouse = false;
        return;
    }
    if (mMouseClicked)
    {
        if (abs(event.getX() - mMousePressX) > 32
            || abs(event.getY() - mMousePressY) > 32)
        {
            mPlayerFollowMouse = true;
        }
    }

    walkByMouse(event);
}

void Viewport::mouseReleased(MouseEvent &event)
{
    mPlayerFollowMouse = false;
    mLocalWalkTime = -1;
    if (mLongMouseClick && mMouseClicked)
    {
        mMouseClicked = false;
        if (event.getSource() != this || event.isConsumed())
            return;
        const MouseButtonT eventButton = event.getButton();
        if (eventButton == MouseButton::LEFT)
        {
            // long button press
            if (gui && gui->isLongPress())
            {
                if (openContextMenu(event))
                {
                    gui->resetClickCount();
                    return;
                }
            }
            else
            {
                if (leftMouseAction())
                    return;
            }
            walkByMouse(event);
        }
    }
}

void Viewport::optionChanged(const std::string &name)
{
    if (name == "ScrollLaziness")
        mScrollLaziness = config.getIntValue("ScrollLaziness");
    else if (name == "ScrollRadius")
        mScrollRadius = config.getIntValue("ScrollRadius");
    else if (name == "showBeingPopup")
        mShowBeingPopup = config.getBoolValue("showBeingPopup");
    else if (name == "selfMouseHeal")
        mSelfMouseHeal = config.getBoolValue("selfMouseHeal");
    else if (name == "enableLazyScrolling")
        mEnableLazyScrolling = config.getBoolValue("enableLazyScrolling");
    else if (name == "mouseDirectionMove")
        mMouseDirectionMove = config.getBoolValue("mouseDirectionMove");
    else if (name == "longmouseclick")
        mLongMouseClick = config.getBoolValue("longmouseclick");
}

void Viewport::mouseMoved(MouseEvent &event A_UNUSED)
{
}

void Viewport::toggleMapDrawType()
{
}

void Viewport::toggleCameraMode()
{
}

void Viewport::cleanHoverItems()
{
    mHoverBeing = nullptr;
    mHoverItem = nullptr;
    mHoverSign = nullptr;
}

bool Viewport::leftMouseAction()
{
    return false;
}

void Viewport::videoResized()
{
}
