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

#include "gui/viewport.h"

#include "actormanager.h"
#include "configuration.h"
#include "game.h"
#include "settings.h"
#include "sdlshared.h"
#include "textmanager.h"

#include "being/flooritem.h"
#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "enums/resources/map/blockmask.h"
#include "enums/resources/map/mapitemtype.h"

#include "gui/gui.h"
#include "gui/popupmanager.h"
#include "gui/userpalette.h"

#include "gui/fonts/font.h"

#include "gui/popups/beingpopup.h"
#include "gui/popups/popupmenu.h"
#include "gui/popups/textpopup.h"

#include "gui/windows/ministatuswindow.h"

#include "input/inputmanager.h"

#include "utils/checkutils.h"
#include "utils/foreach.h"

#include "resources/map/map.h"
#include "resources/map/mapitem.h"
#include "resources/map/speciallayer.h"

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
    mMidTileX(0),
    mMidTileY(0),
    mViewXmax(0),
    mViewYmax(0),
    mLocalWalkTime(-1),
    mCameraRelativeX(0),
    mCameraRelativeY(0),
    mShowBeingPopup(config.getBoolValue("showBeingPopup")),
    mSelfMouseHeal(config.getBoolValue("selfMouseHeal")),
    mEnableLazyScrolling(config.getBoolValue("enableLazyScrolling")),
    mMouseDirectionMove(config.getBoolValue("mouseDirectionMove")),
    mLongMouseClick(config.getBoolValue("longmouseclick")),
    mAllowMoveByMouse(config.getBoolValue("allowMoveByMouse")),
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
    config.addListener("allowMoveByMouse", this);

    setFocusable(true);
    updateMidVars();
}

Viewport::~Viewport()
{
    config.removeListeners(this);
    CHECKLISTENERS
}

void Viewport::setMap(Map *const map)
{
    if ((mMap != nullptr) && (map != nullptr))
        map->setDrawLayersFlags(mMap->getDrawLayersFlags());
    mMap = map;
    updateMaxVars();
}

void Viewport::draw(Graphics *const graphics)
{
    BLOCK_START("Viewport::draw 1")
    static int lastTick = tick_time;

    if ((mMap == nullptr) || (localPlayer == nullptr))
    {
        graphics->setColor(Color(64, 64, 64));
        graphics->fillRectangle(
                Rect(0, 0, getWidth(), getHeight()));
        BLOCK_END("Viewport::draw 1")
        return;
    }

    // Avoid freaking out when tick_time overflows
    if (tick_time < lastTick)
        lastTick = tick_time;

    // Calculate viewpoint

    const int player_x = localPlayer->mPixelX - mMidTileX;
    const int player_y = localPlayer->mPixelY - mMidTileY;

    if (mScrollLaziness < 1)
        mScrollLaziness = 1;  // Avoids division by zero

    if (mEnableLazyScrolling)
    {
        int cnt = 0;

        // Apply lazy scrolling
        while (lastTick < tick_time && cnt < mapTileSize)
        {
            if (player_x > mPixelViewX + mScrollRadius)
            {
                mPixelViewX += CAST_S32(
                    static_cast<float>(player_x
                    - mPixelViewX - mScrollRadius) /
                    static_cast<float>(mScrollLaziness));
            }
            if (player_x < mPixelViewX - mScrollRadius)
            {
                mPixelViewX += CAST_S32(
                    static_cast<float>(player_x
                    - mPixelViewX + mScrollRadius) /
                    static_cast<float>(mScrollLaziness));
            }
            if (player_y > mPixelViewY + mScrollRadius)
            {
                mPixelViewY += CAST_S32(
                    static_cast<float>(player_y
                    - mPixelViewY - mScrollRadius) /
                    static_cast<float>(mScrollLaziness));
            }
            if (player_y < mPixelViewY - mScrollRadius)
            {
                mPixelViewY += CAST_S32(
                    static_cast<float>(player_y
                    - mPixelViewY + mScrollRadius) /
                    static_cast<float>(mScrollLaziness));
            }
            lastTick ++;
            cnt ++;
        }

        // Auto center when player is off screen
        if (cnt > 30 || player_x - mPixelViewX
            > graphics->mWidth / 2 || mPixelViewX
            - player_x > graphics->mWidth / 2 || mPixelViewY
            - player_y > graphics->getHeight() / 2 ||  player_y
            - mPixelViewY > graphics->getHeight() / 2)
        {
            if (player_x <= 0 || player_y <= 0)
            {
                logger->log("incorrect player position: %d, %d, %d, %d",
                    player_x, player_y, mPixelViewX, mPixelViewY);
                logger->log("tile position: %d, %d",
                    localPlayer->getTileX(), localPlayer->getTileY());
            }
            mPixelViewX = player_x;
            mPixelViewY = player_y;
        }
    }
    else
    {
        mPixelViewX = player_x;
        mPixelViewY = player_y;
    }

    if (mPixelViewX < 0)
        mPixelViewX = 0;
    if (mPixelViewY < 0)
        mPixelViewY = 0;
    if (mPixelViewX > mViewXmax)
        mPixelViewX = mViewXmax;
    if (mPixelViewY > mViewYmax)
        mPixelViewY = mViewYmax;

    // Draw tiles and sprites
    mMap->draw(graphics, mPixelViewX, mPixelViewY);

    const MapTypeT drawType = settings.mapDrawType;
    if (drawType != MapType::NORMAL)
    {
        if (drawType != MapType::SPECIAL4)
        {
            mMap->drawCollision(graphics, mPixelViewX,
                mPixelViewY, drawType);
        }
        if (drawType == MapType::DEBUGTYPE)
            drawDebugPath(graphics);
    }

    if (localPlayer->getCheckNameSetting())
    {
        localPlayer->setCheckNameSetting(false);
        localPlayer->setName(localPlayer->getName());
    }

    // Draw text
    if (textManager != nullptr)
        textManager->draw(graphics, mPixelViewX, mPixelViewY);

    // Draw player names, speech, and emotion sprite as needed
    const ActorSprites &actors = actorManager->getAll();
    FOR_EACH (ActorSpritesIterator, it, actors)
    {
        if ((*it)->getType() == ActorType::FloorItem)
            continue;
        Being *const b = static_cast<Being*>(*it);
        b->drawEmotion(graphics, mPixelViewX, mPixelViewY);
        b->drawSpeech(mPixelViewX, mPixelViewY);
    }

    if (miniStatusWindow != nullptr)
        miniStatusWindow->drawIcons(graphics);

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
    if (gui == nullptr)
        return;
    const MouseStateType button = Gui::getMouseState(mMouseX, mMouseY);
    // If the left button is dragged
    if (mPlayerFollowMouse && ((button & SDL_BUTTON(1)) != 0))
    {
        // We create a mouse event and send it to mouseDragged.
        const MouseEvent event(nullptr,
            MouseEventType::DRAGGED,
            MouseButton::LEFT,
            mMouseX,
            mMouseY,
            0);

        walkByMouse(event);
    }
}

void Viewport::drawDebugPath(Graphics *const graphics)
{
    if (localPlayer == nullptr ||
        userPalette == nullptr ||
        actorManager == nullptr ||
        mMap == nullptr ||
        gui == nullptr)
    {
        return;
    }

    Gui::getMouseState(mMouseX, mMouseY);

    static Path debugPath;
    static Vector lastMouseDestination = Vector(0.0F, 0.0F);
    const int mousePosX = mMouseX + mPixelViewX;
    const int mousePosY = mMouseY + mPixelViewY;
    Vector mouseDestination(mousePosX, mousePosY);

    if (mouseDestination.x != lastMouseDestination.x
        || mouseDestination.y != lastMouseDestination.y)
    {
        debugPath = mMap->findPath(
            CAST_S32(localPlayer->mPixelX - mapTileSize / 2) / mapTileSize,
            CAST_S32(localPlayer->mPixelY - mapTileSize) / mapTileSize,
            mousePosX / mapTileSize,
            mousePosY / mapTileSize,
            localPlayer->getBlockWalkMask(),
            500);
        lastMouseDestination = mouseDestination;
    }
    drawPath(graphics, debugPath, userPalette->getColorWithAlpha(
        UserColorId::ROAD_POINT));

    const ActorSprites &actors = actorManager->getAll();
    FOR_EACH (ActorSpritesConstIterator, it, actors)
    {
        const Being *const being = dynamic_cast<const Being*>(*it);
        if ((being != nullptr) && being != localPlayer)
        {
            const Path &beingPath = being->getPath();
            drawPath(graphics, beingPath, userPalette->getColorWithAlpha(
                UserColorId::ROAD_POINT));
        }
    }
}

void Viewport::drawPath(Graphics *const graphics,
                        const Path &path,
                        const Color &color) const
{
    graphics->setColor(color);
    Font *const font = getFont();

    int cnt = 1;
    FOR_EACH (Path::const_iterator, i, path)
    {
        const int squareX = i->x * mapTileSize - mPixelViewX + 12;
        const int squareY = i->y * mapTileSize - mPixelViewY + 12;

        graphics->fillRectangle(Rect(squareX, squareY, 8, 8));
        if (mMap != nullptr)
        {
            const std::string str = toString(cnt);
            font->drawString(graphics,
                color, color,
                str,
                squareX + 4 - font->getWidth(str) / 2,
                squareY + 12);
        }
        cnt ++;
    }
}

bool Viewport::openContextMenu(const MouseEvent &event)
{
    mPlayerFollowMouse = false;
    const int eventX = event.getX();
    const int eventY = event.getY();
    if (popupMenu == nullptr)
        return false;
    if (mHoverBeing != nullptr)
    {
        validateSpeed();
        if (actorManager != nullptr)
        {
            STD_VECTOR<ActorSprite*> beings;
            const int x = mMouseX + mPixelViewX;
            const int y = mMouseY + mPixelViewY;
            actorManager->findBeingsByPixel(beings, x, y, AllPlayers_true);
            if (beings.size() > 1)
                popupMenu->showPopup(eventX, eventY, beings);
            else
                popupMenu->showPopup(eventX, eventY, mHoverBeing);
            return true;
        }
    }
    else if (mHoverItem != nullptr)
    {
        validateSpeed();
        popupMenu->showPopup(eventX, eventY, mHoverItem);
        return true;
    }
    else if (mHoverSign != nullptr)
    {
        validateSpeed();
        popupMenu->showPopup(eventX, eventY, mHoverSign);
        return true;
    }
    else if (settings.cameraMode != 0u)
    {
        if (mMap == nullptr)
            return false;
        popupMenu->showMapPopup(eventX, eventY,
            (mMouseX + mPixelViewX) / mMap->getTileWidth(),
            (mMouseY + mPixelViewY) / mMap->getTileHeight(),
            false);
        return true;
    }
    return false;
}

bool Viewport::leftMouseAction()
{
    const bool stopAttack = inputManager.isActionActive(
        InputAction::STOP_ATTACK);
    // Interact with some being
    if (mHoverBeing != nullptr)
    {
        if (!mHoverBeing->isAlive())
            return true;

        if (mHoverBeing->canTalk())
        {
            validateSpeed();
            mHoverBeing->talkTo();
            return true;
        }

        const ActorTypeT type = mHoverBeing->getType();
        switch (type)
        {
            case ActorType::Player:
                validateSpeed();
                if (actorManager != nullptr)
                {
#ifdef TMWA_SUPPORT
                    if (localPlayer != mHoverBeing || mSelfMouseHeal)
                        actorManager->heal(mHoverBeing);
#endif  // TMWA_SUPPORT

                    if (localPlayer == mHoverBeing &&
                        mHoverItem != nullptr)
                    {
                        localPlayer->pickUp(mHoverItem);
                    }
                    return true;
                }
                break;
            case ActorType::Monster:
            case ActorType::Npc:
            case ActorType::SkillUnit:
                if (!stopAttack)
                {
                    if ((localPlayer->withinAttackRange(mHoverBeing) ||
                        inputManager.isActionActive(InputAction::ATTACK)))
                    {
                        validateSpeed();
                        if (!mStatsReUpdated && localPlayer != mHoverBeing)
                        {
                            localPlayer->attack(mHoverBeing,
                                !inputManager.isActionActive(
                                InputAction::STOP_ATTACK));
                            return true;
                        }
                    }
                    else if (!inputManager.isActionActive(
                             InputAction::ATTACK))
                    {
                        validateSpeed();
                        if (!mStatsReUpdated && localPlayer != mHoverBeing)
                        {
                            localPlayer->setGotoTarget(mHoverBeing);
                            return true;
                        }
                    }
                }
                break;
            case ActorType::FloorItem:
            case ActorType::Portal:
            case ActorType::Pet:
            case ActorType::Mercenary:
            case ActorType::Homunculus:
            case ActorType::Elemental:
                break;
            case ActorType::Unknown:
            case ActorType::Avatar:
            default:
                reportAlways("Left click on unknown actor type: %d",
                    CAST_S32(type));
                break;
        }
    }
    // Picks up a item if we clicked on one
    if (mHoverItem != nullptr)
    {
        validateSpeed();
        localPlayer->pickUp(mHoverItem);
    }
    else if (stopAttack)
    {
        if (mMap != nullptr)
        {
            const int mouseTileX = (mMouseX + mPixelViewX)
                / mMap->getTileWidth();
            const int mouseTileY = (mMouseY + mPixelViewY)
                / mMap->getTileHeight();
            inputManager.executeChatCommand(InputAction::PET_MOVE,
                strprintf("%d %d", mouseTileX, mouseTileY),
                nullptr);
        }
        return true;
    }
    // Just walk around
    else if (!inputManager.isActionActive(InputAction::ATTACK) &&
             localPlayer->canMove())
    {
        validateSpeed();
        localPlayer->stopAttack();
        localPlayer->cancelFollow();
        mPlayerFollowMouse = mAllowMoveByMouse;
        if (mPlayerFollowMouse)
        {
            // Make the player go to the mouse position
            followMouse();
        }
    }
    return false;
}

void Viewport::mousePressed(MouseEvent &event)
{
    if (event.getSource() != this || event.isConsumed())
        return;

    // Check if we are alive and kickin'
    if ((mMap == nullptr) || (localPlayer == nullptr))
        return;

    // Check if we are busy
    // if commented, allow context menu if npc dialog open
    if (PlayerInfo::isTalking())
    {
        mMouseClicked = false;
        return;
    }

    mMouseClicked = true;

    mMousePressX = event.getX();
    mMousePressY = event.getY();
    const MouseButtonT eventButton = event.getButton();
    const int pixelX = mMousePressX + mPixelViewX;
    const int pixelY = mMousePressY + mPixelViewY;

    // Right click might open a popup
    if (eventButton == MouseButton::RIGHT)
    {
        if (openContextMenu(event))
            return;
    }

    // If a popup is active, just remove it
    if (PopupManager::isPopupMenuVisible())
    {
        mPlayerFollowMouse = false;
        PopupManager::hidePopupMenu();
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
    else if (eventButton == MouseButton::MIDDLE)
    {
        mPlayerFollowMouse = false;
        validateSpeed();
        // Find the being nearest to the clicked position
        if (actorManager != nullptr)
        {
            Being *const target = actorManager->findNearestLivingBeing(
                pixelX, pixelY, 20, ActorType::Monster, nullptr);

            if (target != nullptr)
                localPlayer->setTarget(target);
        }
    }
}

void Viewport::getMouseTile(int &destX, int &destY) const
{
    getMouseTile(mMouseX, mMouseY, destX, destY);
}

void Viewport::getMouseTile(const int x, const int y,
                            int &destX, int &destY) const
{
    if (mMap == nullptr)
        return;
    const int tw = mMap->getTileWidth();
    const int th = mMap->getTileHeight();
    destX = CAST_S32(x + mPixelViewX)
        / static_cast<float>(tw);

    if (mMap->isHeightsPresent())
    {
        const int th2 = th / 2;
        const int clickY = y + mPixelViewY - th2;
        destY = y + mPixelViewY;
        int newDiffY = 1000000;
        const int heightTiles = mainGraphics->mHeight / th;
        const int tileViewY = mPixelViewY / th;
        for (int f = tileViewY; f < tileViewY + heightTiles; f ++)
        {
            if (!mMap->getWalk(destX,
                f,
                BlockMask::WALL |
                BlockMask::AIR |
                BlockMask::WATER |
                BlockMask::PLAYERWALL))
            {
                continue;
            }

            const int offset = mMap->getHeightOffset(
                destX, f) * th2;
            const int pixelF = f * th;
            const int diff = abs(clickY + offset - pixelF);
            if (diff < newDiffY)
            {
                destY = pixelF;
                newDiffY = diff;
            }
        }
        destY /= 32;
    }
    else
    {
        destY = CAST_S32((y + mPixelViewY) / static_cast<float>(th));
    }
}

void Viewport::walkByMouse(const MouseEvent &event)
{
    if ((mMap == nullptr) || (localPlayer == nullptr))
        return;
    if (mPlayerFollowMouse
        && !inputManager.isActionActive(InputAction::STOP_ATTACK)
        && !inputManager.isActionActive(InputAction::UNTARGET))
    {
        if (!mMouseDirectionMove)
            mPlayerFollowMouse = false;
        if (mLocalWalkTime != localPlayer->getActionTime())
        {
            mLocalWalkTime = cur_time;
            localPlayer->unSetPickUpTarget();
            int playerX = localPlayer->getTileX();
            int playerY = localPlayer->getTileY();
            if (mMouseDirectionMove)
            {
                const int width = mainGraphics->mWidth / 2;
                const int height = mainGraphics->mHeight / 2;
                const float wh = static_cast<float>(width)
                    / static_cast<float>(height);
                int x = event.getX() - width;
                int y = event.getY() - height;
                if ((x == 0) && (y == 0))
                    return;
                const int x2 = abs(x);
                const int y2 = abs(y);
                const float diff = 2;
                int dx = 0;
                int dy = 0;
                if (x2 > y2)
                {
                    if (y2 != 0 &&
                        static_cast<float>(x2) / static_cast<float>(y2) /
                        wh > diff)
                    {
                        y = 0;
                    }
                }
                else
                {
                    if ((x2 != 0) && y2 * wh / x2 > diff)
                        x = 0;
                }
                if (x > 0)
                    dx = 1;
                else if (x < 0)
                    dx = -1;
                if (y > 0)
                    dy = 1;
                else if (y < 0)
                    dy = -1;

                if (mMap->getWalk(playerX + dx,
                    playerY + dy,
                    BlockMask::WALL |
                    BlockMask::AIR |
                    BlockMask::WATER |
                    BlockMask::PLAYERWALL))
                {
                    localPlayer->navigateTo(playerX + dx, playerY + dy);
                }
                else
                {
                    if ((dx != 0) && (dy != 0))
                    {
                        // try avoid diagonal collision
                        if (x2 > y2)
                        {
                            if (mMap->getWalk(playerX + dx,
                                playerY,
                                BlockMask::WALL |
                                BlockMask::AIR |
                                BlockMask::WATER |
                                BlockMask::PLAYERWALL))
                            {
                                dy = 0;
                            }
                            else
                            {
                                dx = 0;
                            }
                        }
                        else
                        {
                            if (mMap->getWalk(playerX,
                                playerY + dy,
                                BlockMask::WALL |
                                BlockMask::AIR |
                                BlockMask::WATER |
                                BlockMask::PLAYERWALL))
                            {
                                dx = 0;
                            }
                            else
                            {
                                dy = 0;
                            }
                        }
                    }
                    else
                    {
                        // try avoid vertical or horisontal collision
                        if (dx == 0)
                        {
                            if (mMap->getWalk(playerX + 1,
                                playerY + dy,
                                BlockMask::WALL |
                                BlockMask::AIR |
                                BlockMask::WATER |
                                BlockMask::PLAYERWALL))
                            {
                                dx = 1;
                            }
                            if (mMap->getWalk(playerX - 1,
                                playerY + dy,
                                BlockMask::WALL |
                                BlockMask::AIR |
                                BlockMask::WATER |
                                BlockMask::PLAYERWALL))
                            {
                                dx = -1;
                            }
                        }
                        if (dy == 0)
                        {
                            if (mMap->getWalk(playerX + dx,
                                playerY + 1,
                                BlockMask::WALL |
                                BlockMask::AIR |
                                BlockMask::WATER |
                                BlockMask::PLAYERWALL))
                            {
                                dy = 1;
                            }
                            if (mMap->getWalk(playerX + dx,
                                playerY - 1,
                                BlockMask::WALL |
                                BlockMask::AIR |
                                BlockMask::WATER |
                                BlockMask::PLAYERWALL))
                            {
                                dy = -1;
                            }
                        }
                    }
                    localPlayer->navigateTo(playerX + dx, playerY + dy);
                }
            }
            else
            {
                int destX;
                int destY;
                getMouseTile(event.getX(), event.getY(),
                    destX, destY);
                if (playerX != destX || playerY != destY)
                {
                    if (!localPlayer->navigateTo(destX, destY))
                    {
                        if (playerX > destX)
                            playerX --;
                        else if (playerX < destX)
                            playerX ++;
                        if (playerY > destY)
                            playerY --;
                        else if (playerY < destY)
                            playerY ++;
                        if (mMap->getWalk(playerX, playerY, 0))
                            localPlayer->navigateTo(playerX, playerY);
                    }
                }
            }
        }
    }
}

void Viewport::mouseDragged(MouseEvent &event)
{
    if (event.getSource() != this || event.isConsumed())
    {
        mPlayerFollowMouse = false;
        return;
    }
    if (mAllowMoveByMouse &&
        mMouseClicked &&
        (localPlayer != nullptr) &&
        localPlayer->canMove())
    {
        if (abs(event.getX() - mMousePressX) > 32
            || abs(event.getY() - mMousePressY) > 32)
        {
            mPlayerFollowMouse = true;
        }

        walkByMouse(event);
    }
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
            if ((gui != nullptr) && gui->isLongPress())
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
    else if (name == "allowMoveByMouse")
        mAllowMoveByMouse = config.getBoolValue("allowMoveByMouse");
}

void Viewport::mouseMoved(MouseEvent &event)
{
    // Check if we are on the map
    if (mMap == nullptr ||
        localPlayer == nullptr ||
        actorManager == nullptr)
    {
        return;
    }

    if (mMouseDirectionMove)
        mPlayerFollowMouse = false;

    const int x = mMouseX + mPixelViewX;
    const int y = mMouseY + mPixelViewY;

    ActorTypeT type = ActorType::Unknown;
    mHoverBeing = actorManager->findBeingByPixel(x, y, AllPlayers_true);
    if (mHoverBeing != nullptr)
        type = mHoverBeing->getType();
    if ((mHoverBeing != nullptr)
        && (type == ActorType::Player
        || type == ActorType::Npc
        || type == ActorType::Homunculus
        || type == ActorType::Mercenary
        || type == ActorType::Pet))
    {
        PopupManager::hideTextPopup();
        if (mShowBeingPopup && (beingPopup != nullptr))
            beingPopup->show(mMouseX, mMouseY, mHoverBeing);
    }
    else
    {
        PopupManager::hideBeingPopup();
    }

    mHoverItem = actorManager->findItem(x / mMap->getTileWidth(),
        y / mMap->getTileHeight());

    if ((mHoverBeing == nullptr) && (mHoverItem == nullptr))
    {
        const SpecialLayer *const specialLayer = mMap->getSpecialLayer();
        if (specialLayer != nullptr)
        {
            const int mouseTileX = (mMouseX + mPixelViewX)
                / mMap->getTileWidth();
            const int mouseTileY = (mMouseY + mPixelViewY)
                / mMap->getTileHeight();

            mHoverSign = specialLayer->getTile(mouseTileX, mouseTileY);
            if (mHoverSign != nullptr &&
                mHoverSign->getType() != MapItemType::EMPTY)
            {
                if (!mHoverSign->getComment().empty())
                {
                    PopupManager::hideBeingPopup();
                    if (textPopup != nullptr)
                    {
                        textPopup->show(mMouseX, mMouseY,
                            mHoverSign->getComment());
                    }
                }
                else
                {
                    if (PopupManager::isTextPopupVisible())
                        PopupManager::hideTextPopup();
                }
                gui->setCursorType(Cursor::CURSOR_UP);
                return;
            }
        }
    }
    if (!event.isConsumed() &&
        PopupManager::isTextPopupVisible())
    {
        PopupManager::hideTextPopup();
    }

    if (mHoverBeing != nullptr)
    {
        switch (type)
        {
            case ActorType::Npc:
            case ActorType::Monster:
            case ActorType::Portal:
            case ActorType::Pet:
            case ActorType::Mercenary:
            case ActorType::Homunculus:
            case ActorType::SkillUnit:
            case ActorType::Elemental:
                gui->setCursorType(mHoverBeing->getHoverCursor());
                break;

            case ActorType::Avatar:
            case ActorType::FloorItem:
            case ActorType::Unknown:
            case ActorType::Player:
            default:
                gui->setCursorType(Cursor::CURSOR_POINTER);
                break;
        }
    }
    // Item mouseover
    else if (mHoverItem != nullptr)
    {
        gui->setCursorType(mHoverItem->getHoverCursor());
    }
    else
    {
        gui->setCursorType(Cursor::CURSOR_POINTER);
    }
}

void Viewport::toggleMapDrawType()
{
    settings.mapDrawType = static_cast<MapTypeT>(
        CAST_S32(settings.mapDrawType) + 1);
    if (settings.mapDrawType > MapType::BLACKWHITE)
        settings.mapDrawType = MapType::NORMAL;
    if (mMap != nullptr)
        mMap->setDrawLayersFlags(settings.mapDrawType);
}

void Viewport::toggleCameraMode()
{
    settings.cameraMode ++;
    if (settings.cameraMode > 1)
        settings.cameraMode = 0;
    if (settings.cameraMode == 0u)
    {
        mCameraRelativeX = 0;
        mCameraRelativeY = 0;
        updateMidVars();
    }
    UpdateStatusListener::distributeEvent();
}

void Viewport::clearHover(const ActorSprite *const actor)
{
    if (mHoverBeing == actor)
        mHoverBeing = nullptr;

    if (mHoverItem == actor)
        mHoverItem = nullptr;
}

void Viewport::cleanHoverItems()
{
    mHoverBeing = nullptr;
    mHoverItem = nullptr;
    mHoverSign = nullptr;
}

void Viewport::moveCamera(const int dx, const int dy)
{
    mCameraRelativeX += dx;
    mCameraRelativeY += dy;
    updateMidVars();
}

void Viewport::moveCameraToActor(const BeingId actorId,
                                 const int x, const int y)
{
    if ((localPlayer == nullptr) || (actorManager == nullptr))
        return;

    const Actor *const actor = actorManager->findBeing(actorId);
    if (actor == nullptr)
        return;
    settings.cameraMode = 1;
    mCameraRelativeX = actor->mPixelX - localPlayer->mPixelX + x;
    mCameraRelativeY = actor->mPixelY - localPlayer->mPixelY + y;
    updateMidVars();
}

void Viewport::moveCameraToPosition(const int x, const int y)
{
    if (localPlayer == nullptr)
        return;

    settings.cameraMode = 1;
    mCameraRelativeX = x - localPlayer->mPixelX;
    mCameraRelativeY = y - localPlayer->mPixelY;
    updateMidVars();
}

void Viewport::moveCameraRelative(const int x, const int y)
{
    settings.cameraMode = 1;
    mCameraRelativeX += x;
    mCameraRelativeY += y;
    updateMidVars();
}

void Viewport::returnCamera()
{
    settings.cameraMode = 0;
    mCameraRelativeX = 0;
    mCameraRelativeY = 0;
    updateMidVars();
}

void Viewport::validateSpeed()
{
    if (!inputManager.isActionActive(InputAction::TARGET_ATTACK) &&
        !inputManager.isActionActive(InputAction::ATTACK))
    {
        if (Game::instance() != nullptr)
            Game::instance()->setValidSpeed();
    }
}

void Viewport::updateMidVars()
{
    mMidTileX = (mainGraphics->mWidth + mScrollCenterOffsetX) / 2
        - mCameraRelativeX;
    mMidTileY = (mainGraphics->mHeight + mScrollCenterOffsetY) / 2
        - mCameraRelativeY;
}

void Viewport::updateMaxVars()
{
    if (mMap == nullptr)
        return;
    mViewXmax = mMap->getWidth() * mMap->getTileWidth()
        - mainGraphics->mWidth;
    mViewYmax = mMap->getHeight() * mMap->getTileHeight()
        - mainGraphics->mHeight;
}

void Viewport::videoResized()
{
    updateMidVars();
    updateMaxVars();
}
