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

#include "gui/minimap.h"

#include "actorspritemanager.h"
#include "being.h"
#include "configuration.h"
#include "graphics.h"
#include "localplayer.h"
#include "logger.h"
#include "map.h"
#include "party.h"

#include "gui/userpalette.h"
#include "gui/setup.h"
#include "gui/viewport.h"

#include "resources/image.h"
#include "resources/resourcemanager.h"

#include "utils/gettext.h"

#include <guichan/font.hpp>

#include "debug.h"

bool Minimap::mShow = true;

Minimap::Minimap():
    Window(_("Map"), false, nullptr, "map.xml"),
    mMapImage(nullptr),
    mWidthProportion(0.5),
    mHeightProportion(0.5),
    mCustomMapImage(false),
    mMapOriginX(0),
    mMapOriginY(0)
{
    setWindowName("Minimap");
    mShow = config.getValueBool(getWindowName() + "Show", true);
    setDefaultSize(5, 25, 100, 100);
    // set this to false as the minimap window size is changed
    //depending on the map size
    setResizable(true);
    setupWindow->registerWindowForReset(this);

    setDefaultVisible(true);
    setSaveVisible(true);

    setStickyButton(true);
    setSticky(false);

    loadWindowState();
    setVisible(mShow, isSticky());
}

Minimap::~Minimap()
{
    config.setValue(getWindowName() + "Show", mShow);

    if (mMapImage)
    {
        if (mCustomMapImage)
            delete mMapImage;
        else
            mMapImage->decRef();
        mMapImage = nullptr;
    }
}

void Minimap::setMap(Map *map)
{
    std::string caption("");
    std::string minimapName;

    if (map)
        caption = map->getName();

    if (caption.empty())
        caption = _("Map");

    setCaption(caption);

    // Adapt the image
    if (mMapImage)
    {
        if (mCustomMapImage)
            delete mMapImage;
        else
            mMapImage->decRef();
        mMapImage = nullptr;
    }

    if (map)
    {
        if (config.getBoolValue("showExtMinimaps"))
        {
            SDL_Surface* surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                map->getWidth(), map->getHeight(), 32,
                0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000);
            if (!surface)
            {
                if (!isSticky())
                    setVisible(false);
                return;
            }

            // I'm not sure if the locks are necessary since it's a SWSURFACE
            SDL_LockSurface(surface);
            int* data = static_cast<int*>(surface->pixels);
            if (!data)
            {
                if (!isSticky())
                    setVisible(false);
                return;
            }
            const int size = surface->h * surface->w;
            const int mask = (Map::BLOCKMASK_WALL | Map::BLOCKMASK_AIR
                | Map::BLOCKMASK_WATER);

            for (int ptr = 0; ptr < size; ptr ++)
                *(data ++) = -!(map->mMetaTiles[ptr].blockmask & mask);

            SDL_UnlockSurface(surface);

            mMapImage = Image::load(surface);
            mMapImage->setAlpha(Client::getGuiAlpha());
            mCustomMapImage = true;
            SDL_FreeSurface(surface);
        }
        else
        {
            std::string tempname =
                "graphics/minimaps/" + map->getFilename() + ".png";
            ResourceManager *resman = ResourceManager::getInstance();

            minimapName = map->getProperty("minimap");

            if (minimapName.empty() && resman->exists(tempname))
                minimapName = tempname;

            mMapImage = resman->getImage(minimapName);
            mCustomMapImage = false;
        }
    }

    if (mMapImage && map)
    {
        const int offsetX = 2 * getPadding();
        const int offsetY = getTitleBarHeight() + getPadding();
        const int titleWidth = getFont()->getWidth(getCaption()) + 15;
        const int mapWidth = mMapImage->mBounds.w < 100 ?
                             mMapImage->mBounds.w + offsetX : 100;
        const int mapHeight = mMapImage->mBounds.h < 100 ?
                              mMapImage->mBounds.h + offsetY : 100;

        setMinWidth(mapWidth > titleWidth ? mapWidth : titleWidth);
        setMinHeight(mapHeight);

        mWidthProportion = static_cast<float>(
                mMapImage->mBounds.w) / static_cast<float>(map->getWidth());
        mHeightProportion = static_cast<float>(
                mMapImage->mBounds.h) / static_cast<float>(map->getHeight());

        setMaxWidth(mMapImage->mBounds.w > titleWidth ?
                    mMapImage->mBounds.w + offsetX : titleWidth);
        setMaxHeight(mMapImage->mBounds.h + offsetY);

        setDefaultSize(getX(), getY(), getWidth(), getHeight());
        resetToDefaultSize();

        if (mShow)
            setVisible(true);
    }
    else
    {
        if (!isSticky())
            setVisible(false);
    }
}

void Minimap::toggle()
{
    setVisible(!isVisible(), isSticky());
    mShow = isVisible();
}

void Minimap::draw(gcn::Graphics *graphics)
{
    Window::draw(graphics);

    if (!userPalette || !player_node || !viewport)
        return;

    Graphics *graph = static_cast<Graphics*>(graphics);

    const gcn::Rectangle a = getChildrenArea();

    graphics->pushClipArea(a);

    if (!actorSpriteManager)
        return;

    mMapOriginX = 0;
    mMapOriginY = 0;

    if (mMapImage)
    {
        if (mMapImage->mBounds.w > a.width ||
            mMapImage->mBounds.h > a.height)
        {
            const Vector &p = player_node->getPosition();
            mMapOriginX = ((a.width) / 2) - (static_cast<float>(p.x
                + viewport->getCameraRelativeX()) * mWidthProportion) / 32;

            mMapOriginY = ((a.height) / 2) - (static_cast<float>(p.y
                + viewport->getCameraRelativeY()) * mHeightProportion) / 32;

            const int minOriginX = a.width - mMapImage->mBounds.w;
            const int minOriginY = a.height - mMapImage->mBounds.h;

            if (mMapOriginX < minOriginX)
                mMapOriginX = minOriginX;
            if (mMapOriginY < minOriginY)
                mMapOriginY = minOriginY;
            if (mMapOriginX > 0)
                mMapOriginX = 0;
            if (mMapOriginY > 0)
                mMapOriginY = 0;
        }

        graph->drawImage(mMapImage, mMapOriginX, mMapOriginY);
    }

    const ActorSprites &actors = actorSpriteManager->getAll();

    for (ActorSpritesConstIterator it = actors.begin(), it_end = actors.end();
         it != it_end; ++it)
    {
        if (!(*it) || (*it)->getType() == ActorSprite::FLOOR_ITEM)
            continue;

        const Being *being = static_cast<Being*>(*it);
        if (!being)
            continue;

        int dotSize = 2;

        int type = UserPalette::PC;

        if (being == player_node)
        {
            type = UserPalette::SELF;
            dotSize = 3;
        }
        else if (being->isGM())
        {
            type = UserPalette::GM;
        }
        else if (being->getGuild() == player_node->getGuild()
                 || being->getGuildName() == player_node->getGuildName())
        {
            type = UserPalette::GUILD;
        }
        else if (being)
        {
            switch (being->getType())
            {
                case ActorSprite::MONSTER:
                    type = UserPalette::MONSTER;
                    break;

                case ActorSprite::NPC:
                    type = UserPalette::NPC;
                    break;

                case ActorSprite::UNKNOWN:
                case ActorSprite::PLAYER:
                case ActorSprite::FLOOR_ITEM:
                case ActorSprite::PORTAL:
                default:
                    continue;
            }
        }

        if (userPalette)
            graphics->setColor(userPalette->getColor(type));

        const int offsetHeight = static_cast<int>(static_cast<float>(
                dotSize - 1) * mHeightProportion);
        const int offsetWidth = static_cast<int>(static_cast<float>(
                dotSize - 1) * mWidthProportion);
        const Vector &pos = being->getPosition();

        graphics->fillRectangle(gcn::Rectangle(
                static_cast<float>(pos.x * mWidthProportion) / 32
                + mMapOriginX - offsetWidth,
                static_cast<float>(pos.y * mHeightProportion) / 32
                + mMapOriginY - offsetHeight,
                dotSize, dotSize));
    }

    if (player_node->isInParty())
    {
        Party *party = player_node->getParty();
        if (party)
        {
            PartyMember *m = party->getMember(player_node->getName());
            Party::MemberList *members = party->getMembers();
            if (m && members)
            {
                const std::string curMap = m->getMap();
                Party::MemberList::const_iterator it = members->begin();
                const Party::MemberList::const_iterator
                    it_end = members->end();
                while (it != it_end)
                {
                    PartyMember *member = *it;
                    if (member && member->getMap() == curMap
                        && member->getOnline() && member != m)
                    {
                        if (userPalette)
                        {
                            graphics->setColor(userPalette->getColor(
                                UserPalette::PARTY));
                        }

                        const int offsetHeight = static_cast<int>(
                            mHeightProportion);
                        const int offsetWidth = static_cast<int>(
                            mWidthProportion);

                        graphics->fillRectangle(gcn::Rectangle(
                            static_cast<int>(member->getX()
                            * mWidthProportion) + mMapOriginX - offsetWidth,
                            static_cast<int>(member->getY()
                            * mHeightProportion) + mMapOriginY - offsetHeight,
                            2, 2));
                    }
                    ++ it;
                }
            }
        }
    }

    const Vector &pos = player_node->getPosition();
//    logger->log("width:" + toString(graph->getWidth()));

    int x = static_cast<float>((pos.x - (graph->getWidth() / 2)
        + viewport->getCameraRelativeX())
        * mWidthProportion) / 32 + mMapOriginX;
    int y = static_cast<float>((pos.y - (graph->getHeight() / 2)
        + viewport->getCameraRelativeY())
        * mHeightProportion) / 32 + mMapOriginY;

    const int w = static_cast<int>(static_cast<float>(
        graph->getWidth()) * mWidthProportion / 32);
    const int h = static_cast<int>(static_cast<float>(
        graph->getHeight()) * mHeightProportion / 32);

    if (w <= a.width)
    {
        if (x < 0 && w)
            x = 0;
        if (x + w > a.width)
            x = a.width - w;
    }
    if (h <= a.height)
    {
        if (y < 0 && h)
            y = 0;
        if (y + h > a.height)
            y = a.height - h;
    }

    graphics->setColor(userPalette->getColor(UserPalette::PC));
    graphics->drawRectangle(gcn::Rectangle(x, y, w, h));
    graphics->popClipArea();
}

void Minimap::mouseReleased(gcn::MouseEvent &event)
{
    gcn::Window::mouseReleased(event);

    if (!player_node || !viewport)
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        int x = event.getX();
        int y = event.getY();
        screenToMap(x, y);

        player_node->navigateTo(x, y);
    }
    else if (event.getButton() == gcn::MouseEvent::RIGHT)
    {
        int x = event.getX();
        int y = event.getY();
        screenToMap(x, y);
        viewport->showMapPopup(x, y);
    }
}

void Minimap::screenToMap(int &x, int &y)
{
    const gcn::Rectangle a = getChildrenArea();
    x = (x - a.x - mMapOriginX + mWidthProportion) / mWidthProportion;
    y = (y - a.y - mMapOriginY + mHeightProportion) / mHeightProportion;
}
