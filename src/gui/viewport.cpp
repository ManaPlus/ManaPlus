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

#include "gui/viewport.h"

#include "actorsprite.h"
#include "actorspritemanager.h"
#include "client.h"
#include "configuration.h"
#include "graphics.h"
#include "itemshortcut.h"
#include "keyboardconfig.h"
#include "localplayer.h"
#include "map.h"
#include "textmanager.h"

#include "gui/beingpopup.h"
#include "gui/chatwindow.h"
#include "gui/gui.h"
#include "gui/ministatus.h"
#include "gui/popupmenu.h"
#include "gui/statuspopup.h"
#include "gui/textpopup.h"

#include "gui/widgets/button.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/progressbar.h"

#include "net/net.h"

#include "resources/resourcemanager.h"

#include "utils/stringutils.h"

#include "debug.h"

extern volatile int tick_time;

Viewport::Viewport():
    mMap(0),
    mMouseX(0),
    mMouseY(0),
    mPixelViewX(0.0f),
    mPixelViewY(0.0f),
    mShowDebugPath(false),
    mCameraMode(0),
    mPlayerFollowMouse(false),
    mLocalWalkTime(-1),
    mHoverBeing(0),
    mHoverItem(0),
    mHoverSign(0),
    mCameraRelativeX(0),
    mCameraRelativeY(0)
{
    setOpaque(false);
    addMouseListener(this);

    mScrollLaziness = config.getIntValue("ScrollLaziness");
    mScrollRadius = config.getIntValue("ScrollRadius");
    mScrollCenterOffsetX = config.getIntValue("ScrollCenterOffsetX");
    mScrollCenterOffsetY = config.getIntValue("ScrollCenterOffsetY");
    mShowBeingPopup = config.getBoolValue("showBeingPopup");

    config.addListener("ScrollLaziness", this);
    config.addListener("ScrollRadius", this);
    config.addListener("showBeingPopup", this);

    mPopupMenu = new PopupMenu;
    mBeingPopup = new BeingPopup;
    mTextPopup = new TextPopup;

    setFocusable(true);
}

Viewport::~Viewport()
{
    config.removeListener("ScrollLaziness", this);
    config.removeListener("ScrollRadius", this);
    config.removeListener("showBeingPopup", this);

    delete mPopupMenu;
    mPopupMenu = 0;
    delete mBeingPopup;
    mBeingPopup = 0;
    delete mTextPopup;
    mTextPopup = 0;
}

void Viewport::setMap(Map *map)
{
    if (mMap && map)
        map->setDebugFlags(mMap->getDebugFlags());
    mMap = map;
}

extern MiniStatusWindow *miniStatusWindow;

void Viewport::draw(gcn::Graphics *gcnGraphics)
{
    static int lastTick = tick_time;

    if (!mMap || !player_node)
    {
        gcnGraphics->setColor(gcn::Color(64, 64, 64));
        gcnGraphics->fillRectangle(
                gcn::Rectangle(0, 0, getWidth(), getHeight()));
        return;
    }

    Graphics *graphics = static_cast<Graphics*>(gcnGraphics);

    // Avoid freaking out when tick_time overflows
    if (tick_time < lastTick)
        lastTick = tick_time;

    // Calculate viewpoint
    int midTileX = (graphics->mWidth + mScrollCenterOffsetX) / 2;
    int midTileY = (graphics->mHeight + mScrollCenterOffsetX) / 2;

    const Vector &playerPos = player_node->getPosition();
    const int player_x = static_cast<int>(playerPos.x)
                         - midTileX + mCameraRelativeX;
    const int player_y = static_cast<int>(playerPos.y)
                         - midTileY + mCameraRelativeY;

    if (mScrollLaziness < 1)
        mScrollLaziness = 1; // Avoids division by zero

    // Apply lazy scrolling
    while (lastTick < tick_time)
    {
        if (player_x > static_cast<int>(mPixelViewX) + mScrollRadius)
        {
            mPixelViewX += static_cast<float>(player_x
                - static_cast<int>(mPixelViewX) - mScrollRadius) /
                static_cast<float>(mScrollLaziness);
        }
        if (player_x < static_cast<int>(mPixelViewX) - mScrollRadius)
        {
            mPixelViewX += static_cast<float>(player_x
                - static_cast<int>(mPixelViewX) + mScrollRadius) /
                static_cast<float>(mScrollLaziness);
        }
        if (player_y > static_cast<int>(mPixelViewY) + mScrollRadius)
        {
            mPixelViewY += static_cast<float>(player_y
            - static_cast<int>(mPixelViewY) - mScrollRadius) /
            static_cast<float>(mScrollLaziness);
        }
        if (player_y < static_cast<int>(mPixelViewY) - mScrollRadius)
        {
            mPixelViewY += static_cast<float>(player_y
            - static_cast<int>(mPixelViewY) + mScrollRadius) /
            static_cast<float>(mScrollLaziness);
        }
        lastTick++;
    }

    // Auto center when player is off screen
    if (player_x - static_cast<int>(mPixelViewX) > graphics->mWidth / 2
        ||  static_cast<int>(mPixelViewX) - player_x > graphics->mWidth / 2
        ||  static_cast<int>(mPixelViewY) - player_y
        > graphics->getHeight() / 2
        ||  player_y - static_cast<int>(mPixelViewY)
        > graphics->getHeight() / 2)
    {
        mPixelViewX = static_cast<float>(player_x);
        mPixelViewY = static_cast<float>(player_y);
    };

    // Don't move camera so that the end of the map is on screen
    const int viewXmax =
        mMap->getWidth() * mMap->getTileWidth() - graphics->mWidth;
    const int viewYmax =
        mMap->getHeight() * mMap->getTileHeight() - graphics->mHeight;
    if (mMap)
    {
        if (mPixelViewX < 0)
            mPixelViewX = 0;
        if (mPixelViewY < 0)
            mPixelViewY = 0;
        if (mPixelViewX > viewXmax)
            mPixelViewX = static_cast<float>(viewXmax);
        if (mPixelViewY > viewYmax)
            mPixelViewY = static_cast<float>(viewYmax);
    }

    // Draw tiles and sprites
    if (mMap)
    {
        mMap->draw(graphics, static_cast<int>(mPixelViewX),
                   static_cast<int>(mPixelViewY));

        if (mShowDebugPath)
        {
            mMap->drawCollision(graphics,
                                static_cast<int>(mPixelViewX),
                                static_cast<int>(mPixelViewY),
                                mShowDebugPath);
            if (mShowDebugPath == Map::MAP_DEBUG)
                _drawDebugPath(graphics);
        }
    }

    if (player_node->getCheckNameSetting())
    {
        player_node->setCheckNameSetting(false);
        player_node->setName(player_node->getName());
    }

    // Draw text
    if (textManager)
        textManager->draw(graphics, static_cast<int>(mPixelViewX),
                          static_cast<int>(mPixelViewY));

    // Draw player names, speech, and emotion sprite as needed
    const ActorSprites &actors = actorSpriteManager->getAll();
    for (ActorSpritesConstIterator it = actors.begin(), it_end = actors.end();
         it != it_end; ++it)
    {
        if ((*it)->getType() == ActorSprite::FLOOR_ITEM)
            continue;
        Being *b = static_cast<Being*>(*it);

        b->drawSpeech(static_cast<int>(mPixelViewX),
                      static_cast<int>(mPixelViewY));
        b->drawEmotion(graphics, static_cast<int>(mPixelViewX),
                       static_cast<int>(mPixelViewY));
    }

    if (miniStatusWindow)
        miniStatusWindow->drawIcons(graphics);

    // Draw contained widgets
    WindowContainer::draw(gcnGraphics);
}

void Viewport::logic()
{
    WindowContainer::logic();

    // Make the player follow the mouse position
    // if the mouse is dragged elsewhere than in a window.
    _followMouse();
}

void Viewport::_followMouse()
{
    Uint8 button = SDL_GetMouseState(&mMouseX, &mMouseY);
    // If the left button is dragged
    if (mPlayerFollowMouse && button & SDL_BUTTON(1))
    {
        // We create a mouse event and send it to mouseDragged.
        Uint8 *keys = SDL_GetKeyState(NULL);
        gcn::MouseEvent mouseEvent(NULL,
                      (keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT]),
                      false,
                      false,
                      false,
                      gcn::MouseEvent::DRAGGED,
                      gcn::MouseEvent::LEFT,
                      mMouseX,
                      mMouseY,
                      0);

        mouseDragged(mouseEvent);
    }
}

void Viewport::_drawDebugPath(Graphics *graphics)
{
    if (!player_node || !userPalette || !actorSpriteManager)
        return;

    // Get the current mouse position
    SDL_GetMouseState(&mMouseX, &mMouseY);

    static Path debugPath;
    static Vector lastMouseDestination = Vector(0.0f, 0.0f);
    const int mousePosX = mMouseX + static_cast<int>(mPixelViewX);
    const int mousePosY = mMouseY + static_cast<int>(mPixelViewY);
    Vector mouseDestination(mousePosX, mousePosY);

    if (mouseDestination.x != lastMouseDestination.x
        || mouseDestination.y != lastMouseDestination.y)
    {
        const Vector &playerPos = player_node->getPosition();

        debugPath = mMap->findPath(
            static_cast<int>(playerPos.x - 16) / 32,
            static_cast<int>(playerPos.y - 32) / 32,
            mousePosX / 32, mousePosY / 32, player_node->getWalkMask(), 500);

        lastMouseDestination = mouseDestination;
    }
    _drawPath(graphics, debugPath, userPalette->getColorWithAlpha(
        UserPalette::ROAD_POINT));

    // We draw the path proposed by mouse

    // Draw the path debug information for every beings.
    ActorSpritesConstIterator it, it_end;
    const ActorSprites &actors = actorSpriteManager->getAll();
    for (it = actors.begin(), it_end = actors.end() ; it != it_end; ++ it)
    {
        Being *being = dynamic_cast<Being*>(*it);
        if (being && being != player_node)
        {
            Path beingPath = being->getPath();

            _drawPath(graphics, beingPath, userPalette->getColorWithAlpha(
                UserPalette::ROAD_POINT));
        }
    }
}

void Viewport::_drawPath(Graphics *graphics, const Path &path,
                         gcn::Color color)
{
    graphics->setColor(color);

#ifdef MANASERV_SUPPORT
    if (Net::getNetworkType() != ServerInfo::MANASERV)
#endif
    {
        for (Path::const_iterator i = path.begin(); i != path.end(); ++i)
        {
            int squareX = i->x * 32 - static_cast<int>(mPixelViewX) + 12;
            int squareY = i->y * 32 - static_cast<int>(mPixelViewY) + 12;

            graphics->fillRectangle(gcn::Rectangle(squareX, squareY, 8, 8));
            if (mMap)
            {
                graphics->drawText(
                    toString(mMap->getMetaTile(i->x, i->y)->Gcost),
                    squareX + 4, squareY + 12, gcn::Graphics::CENTER);
            }
        }
    }
#ifdef MANASERV_SUPPORT
    else if (Net::getNetworkType() == ServerInfo::MANASERV)
    {
        for (Path::const_iterator i = path.begin(); i != path.end(); ++i)
        {
            int squareX = i->x - static_cast<int>(mPixelViewX);
            int squareY = i->y - static_cast<int>(mPixelViewY);

            graphics->fillRectangle(gcn::Rectangle(squareX - 4, squareY - 4,
                                                   8, 8));
            if (mMap)
            {
                graphics->drawText(
                    toString(mMap->getMetaTile(i->x / 32, i->y / 32)->Gcost),
                    squareX + 4, squareY + 12, gcn::Graphics::CENTER);
            }
        }
    }
#endif
}

void Viewport::mousePressed(gcn::MouseEvent &event)
{
    if (event.getSource() != this)
        return;

    // Check if we are alive and kickin'
//    if (!mMap || !player_node || !player_node->isAlive())
    if (!mMap || !player_node)
        return;

    // Check if we are busy
    // if commented, allow context menu if npc dialog open
    if (Being::isTalking())
        return;


    const int pixelX = event.getX() + static_cast<int>(mPixelViewX);
    const int pixelY = event.getY() + static_cast<int>(mPixelViewY);

    // Right click might open a popup
    if (event.getButton() == gcn::MouseEvent::RIGHT)
    {
        mPlayerFollowMouse = false;
        if (mHoverBeing)
        {
            if (actorSpriteManager)
            {
                std::vector<Being*> beings;
                const int x = getMouseX() + static_cast<int>(mPixelViewX);
                const int y = getMouseY() + static_cast<int>(mPixelViewY);
                actorSpriteManager->findBeingsByPixel(beings, x, y, true);
                if (beings.size() > 1)
                {
                    mPopupMenu->showPopup(event.getX(), event.getY(), beings);
                    return;
                }
                else
                {
                    mPopupMenu->showPopup(event.getX(), event.getY(),
                                          mHoverBeing);
                    return;
                }
            }
        }
        else if (mHoverItem)
        {
            mPopupMenu->showPopup(event.getX(), event.getY(), mHoverItem);
            return;
        }
        else if (mHoverSign)
        {
            mPopupMenu->showPopup(event.getX(), event.getY(), mHoverSign);
            return;
        }
    }

    // If a popup is active, just remove it
    if (mPopupMenu->isVisible())
    {
        mPlayerFollowMouse = false;
        mPopupMenu->setVisible(false);
        return;
    }

    // Left click can cause different actions
    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        // Interact with some being
        if (mHoverBeing)
        {
            if (!mHoverBeing->isAlive())
                return;

            if (mHoverBeing->canTalk())
            {
                mHoverBeing->talkTo();
            }
            else
            {
                if (mHoverBeing->getType() == ActorSprite::PLAYER)
                {
                    if (actorSpriteManager)
                        actorSpriteManager->heal(player_node, mHoverBeing);
                }
                else if (player_node->withinAttackRange(mHoverBeing) ||
                         keyboard.isKeyActive(keyboard.KEY_ATTACK))
                {
                    player_node->attack(mHoverBeing,
                        !keyboard.isKeyActive(keyboard.KEY_TARGET));
                }
                else if (!keyboard.isKeyActive(keyboard.KEY_ATTACK))
                {
                    player_node->setGotoTarget(mHoverBeing);
                }
            }
        // Picks up a item if we clicked on one
        }
        else if (mHoverItem)
        {
            player_node->pickUp(mHoverItem);
        }
        // Just walk around
        else if (!keyboard.isKeyActive(keyboard.KEY_ATTACK))
        {
            player_node->stopAttack();
            player_node->cancelFollow();
            mPlayerFollowMouse = true;

            // Make the player go to the mouse position
            _followMouse();
        }
    }
    else if (event.getButton() == gcn::MouseEvent::MIDDLE)
    {
        mPlayerFollowMouse = false;
        // Find the being nearest to the clicked position
        if (actorSpriteManager)
        {
            Being *target = actorSpriteManager->findNearestLivingBeing(
                pixelX, pixelY, 20, ActorSprite::MONSTER);

            if (target)
                player_node->setTarget(target);
        }
    }
}

void Viewport::mouseDragged(gcn::MouseEvent &event)
{
    if (!mMap || !player_node)
        return;

    if (mPlayerFollowMouse && !event.isShiftPressed())
    {
#ifdef MANASERV_SUPPORT
        if (Net::getNetworkType() == ServerInfo::MANASERV)
        {
            if (get_elapsed_time(mLocalWalkTime) >= walkingMouseDelay)
            {
                mLocalWalkTime = tick_time;
                player_node->unSetPickUpTarget();
                player_node->setDestination(event.getX()
                    + static_cast<int>(mPixelViewX),
                    event.getY()
                    + static_cast<int>(mPixelViewY));
                player_node->pathSetByMouse();
            }
        }
        else
#endif
        {
            if (mLocalWalkTime != player_node->getActionTime())
            {
                mLocalWalkTime = cur_time;
                int destX = static_cast<int>((static_cast<float>(event.getX())
                    + mPixelViewX)
                    / static_cast<float>(mMap->getTileWidth()));
                int destY = static_cast<int>((static_cast<float>(event.getY())
                    + mPixelViewY)
                    / static_cast<float>(mMap->getTileHeight()));
                player_node->unSetPickUpTarget();
                if (!player_node->navigateTo(destX, destY))
                {
                    int playerX = player_node->getTileX();
                    int playerY = player_node->getTileY();
                    if (playerX != destX && playerY != destY)
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
                            player_node->navigateTo(playerX, playerY);
                    }
                }
            }
        }
    }
}

void Viewport::mouseReleased(gcn::MouseEvent &event _UNUSED_)
{
    mPlayerFollowMouse = false;

    // Only useful for eAthena but doesn't hurt under ManaServ
    mLocalWalkTime = -1;
}

void Viewport::showPopup(Window *parent, int x, int y, Item *item,
                         bool isInventory)
{
    mPopupMenu->showPopup(parent, x, y, item, isInventory);
}

void Viewport::showPopup(MapItem *item)
{
    mPopupMenu->showPopup(getMouseX(), getMouseY(), item);
}

void Viewport::showPopup(Window *parent, Item *item, bool isInventory)
{
    mPopupMenu->showPopup(parent, getMouseX(), getMouseY(), item, isInventory);
}

void Viewport::showItemPopup(Item *item)
{
    mPopupMenu->showItemPopup(getMouseX(), getMouseY(), item);
}

void Viewport::showItemPopup(int itemId, unsigned char color)
{
    mPopupMenu->showItemPopup(getMouseX(), getMouseY(), itemId, color);
}

void Viewport::showDropPopup(Item *item)
{
    mPopupMenu->showDropPopup(getMouseX(), getMouseY(), item);
}

void Viewport::showOutfitsPopup(int x, int y)
{
    mPopupMenu->showOutfitsPopup(x, y);
}

void Viewport::showOutfitsPopup()
{
    mPopupMenu->showOutfitsPopup(getMouseX(), getMouseY());
}

void Viewport::showSpellPopup(TextCommand *cmd)
{
    mPopupMenu->showSpellPopup(getMouseX(), getMouseY(), cmd);
}

void Viewport::showChatPopup(int x, int y, ChatTab *tab)
{
    mPopupMenu->showChatPopup(x, y, tab);
}

void Viewport::showChatPopup(ChatTab *tab)
{
    mPopupMenu->showChatPopup(getMouseX(), getMouseY(), tab);
}

void Viewport::showPopup(int x, int y, Being *being)
{
    mPopupMenu->showPopup(x, y, being);
}

void Viewport::showPopup(Being *being)
{
    mPopupMenu->showPopup(getMouseX(), getMouseY(), being);
}

void Viewport::showPlayerPopup(std::string nick)
{
    mPopupMenu->showPlayerPopup(getMouseX(), getMouseY(), nick);
}

void Viewport::showPopup(int x, int y, Button *button)
{
    mPopupMenu->showPopup(x, y, button);
}

void Viewport::showPopup(int x, int y, ProgressBar *bar)
{
    mPopupMenu->showPopup(x, y, bar);
}

void Viewport::showAttackMonsterPopup(std::string name, int type)
{
    mPopupMenu->showAttackMonsterPopup(getMouseX(), getMouseY(),
        name, type);
}

void Viewport::showUndressPopup(int x, int y, Being *being, Item *item)
{
    mPopupMenu->showUndressPopup(x, y, being, item);
}

void Viewport::closePopupMenu()
{
    if (mPopupMenu)
        mPopupMenu->handleLink("cancel", 0);
}

void Viewport::optionChanged(const std::string &name _UNUSED_)
{
    mScrollLaziness = config.getIntValue("ScrollLaziness");
    mScrollRadius = config.getIntValue("ScrollRadius");
    mShowBeingPopup = config.getBoolValue("showBeingPopup");
}

void Viewport::mouseMoved(gcn::MouseEvent &event _UNUSED_)
{
    // Check if we are on the map
    if (!mMap || !player_node || !actorSpriteManager)
        return;

    const int x = getMouseX() + static_cast<int>(mPixelViewX);
    const int y = getMouseY() + static_cast<int>(mPixelViewY);

    mHoverBeing = actorSpriteManager->findBeingByPixel(x, y, true);
    if (mHoverBeing && mHoverBeing->getType() == Being::PLAYER)
    {
        mTextPopup->setVisible(false);
        if (mShowBeingPopup)
            mBeingPopup->show(getMouseX(), getMouseY(), mHoverBeing);
    }
    else
    {
        mBeingPopup->setVisible(false);
    }

    mHoverItem = 0;
    if (!mHoverBeing && actorSpriteManager)
    {
        mHoverItem = actorSpriteManager->findItem(x / mMap->getTileWidth(),
                                                  y / mMap->getTileHeight());
    }
    if (!mHoverBeing && !mHoverItem)
    {
        SpecialLayer *specialLayer = mMap->getSpecialLayer();
        if (specialLayer)
        {
            int mouseTileX = (getMouseX() + getCameraX())
                / mMap->getTileWidth();
            int mouseTileY = (getMouseY() + getCameraY())
                / mMap->getTileHeight();

            mHoverSign = specialLayer->getTile(mouseTileX, mouseTileY);
            if (mHoverSign && mHoverSign->getType() != MapItem::EMPTY)
            {
                if (!mHoverSign->getComment().empty())
                {
                    if (mBeingPopup)
                        mBeingPopup->setVisible(false);
                    mTextPopup->show(getMouseX(), getMouseY(),
                                     mHoverSign->getComment());
                }
                else
                {
                    if (mTextPopup->isVisible())
                        mTextPopup->setVisible(false);
                }
                return;
            }
        }
    }
    if (mTextPopup->isVisible())
        mTextPopup->setVisible(false);

    if (mHoverBeing)
    {
        switch (mHoverBeing->getType())
        {
            // NPCs
            case ActorSprite::NPC:
                gui->setCursorType(Gui::CURSOR_TALK);
                break;

            // Monsters
            case ActorSprite::MONSTER:
                gui->setCursorType(Gui::CURSOR_FIGHT);
                break;

            case ActorSprite::UNKNOWN:
            case ActorSprite::PLAYER:
            case ActorSprite::FLOOR_ITEM:
            case ActorSprite::PORTAL:
            default:
                gui->setCursorType(Gui::CURSOR_POINTER);
                break;
        }
    }
    // Item mouseover
    else if (mHoverItem)
    {
        gui->setCursorType(Gui::CURSOR_PICKUP);
    }
    else
    {
        gui->setCursorType(Gui::CURSOR_POINTER);
    }
}

void Viewport::toggleDebugPath()
{
    mShowDebugPath++;
    if (mShowDebugPath > Map::MAP_BLACKWHITE)
        mShowDebugPath = Map::MAP_NORMAL;
    if (mMap)
        mMap->setDebugFlags(mShowDebugPath);
}

void Viewport::toggleCameraMode()
{
    mCameraMode++;
    if (mCameraMode > 1)
        mCameraMode = 0;
    if (!mCameraMode)
    {
        mCameraRelativeX = 0;
        mCameraRelativeY = 0;
    }
    if (miniStatusWindow)
        miniStatusWindow->updateStatus();
}

void Viewport::hideBeingPopup()
{
    if (mBeingPopup)
        mBeingPopup->setVisible(false);
    if (mTextPopup)
        mTextPopup->setVisible(false);
}

void Viewport::clearHover(ActorSprite *actor)
{
    if (mHoverBeing == actor)
        mHoverBeing = 0;

    if (mHoverItem == actor)
        mHoverItem = 0;
}

void Viewport::cleanHoverItems()
{
    mHoverBeing = 0;
    mHoverItem = 0;
    mHoverSign = 0;
}

void Viewport::moveCamera(int dx, int dy)
{
    mCameraRelativeX += dx;
    mCameraRelativeY += dy;
}
