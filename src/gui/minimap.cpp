/*
 *  The Mana Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/minimap.h"

#include "actorspritemanager.h"
#include "being.h"
#include "configuration.h"
#include "graphics.h"
#include "localplayer.h"
#include "log.h"
#include "map.h"

#include "gui/userpalette.h"
#include "gui/setup.h"
#include "gui/viewport.h"

#include "resources/image.h"
#include "resources/resourcemanager.h"

#include "utils/gettext.h"

#include <guichan/font.hpp>

bool Minimap::mShow = true;

Minimap::Minimap():
    Window(_("Map")),
    mMapImage(0),
    mWidthProportion(0.5),
    mHeightProportion(0.5)
{
    setWindowName("Minimap");
    mShow = config.getValueBool(getWindowName() + "Show", true);
    setDefaultSize(5, 25, 100, 100);
    // set this to false as the minimap window size is changed
    //depending on the map size
    setResizable(false);
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
        mMapImage->decRef();
}

void Minimap::setMap(Map *map)
{
    std::string caption = "";
    std::string minimapName;

    if (map)
        caption = map->getName();

    if (caption.empty())
        caption = _("Map");

    setCaption(caption);

    // Adapt the image
    if (mMapImage)
    {
        mMapImage->decRef();
        mMapImage = 0;
    }

    if (true/* TODO replace this with an option*/) {
       // should this optionally happen only if there is no resourcemanager option? i.e. a tristate always, fallback, never?
       SDL_Surface* surface = SDL_CreateRGBSurface(SDL_SWSURFACE, map->getWidth(), map->getHeight(), 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000);
       // I'm not sure if the locks are necessary since it's a SWSURFACE
       SDL_LockSurface(surface);
       int* data = (int*)surface->pixels;
       for (int y = 0; y < surface->h; y++)
           for (int x = 0; x < surface->w; x++)
               *(data++) = -map->getWalk(x,y);
        SDL_UnlockSurface(surface);
        mMapImage = Image::load(surface);
        SDL_FreeSurface(surface);
    }
    else if (map)
    {
        std::string tempname =
            "graphics/minimaps/" + map->getFilename() + ".png";
        ResourceManager *resman = ResourceManager::getInstance();

        minimapName = map->getProperty("minimap");

        if (minimapName.empty() && resman->exists(tempname))
            minimapName = tempname;

        mMapImage = resman->getImage(minimapName);
    }

    if (mMapImage && map)
    {
        const int offsetX = 2 * getPadding();
        const int offsetY = getTitleBarHeight() + getPadding();
        const int titleWidth = getFont()->getWidth(getCaption()) + 15;
        const int mapWidth = mMapImage->getWidth() < 100 ?
                             mMapImage->getWidth() + offsetX : 100;
        const int mapHeight = mMapImage->getHeight() < 100 ?
                              mMapImage->getHeight() + offsetY : 100;

        setMinWidth(mapWidth > titleWidth ? mapWidth : titleWidth);
        setMinHeight(mapHeight);

        mWidthProportion = static_cast<float>(
                mMapImage->getWidth()) / static_cast<float>(map->getWidth());
        mHeightProportion = static_cast<float>(
                mMapImage->getHeight()) / static_cast<float>(map->getHeight());

        setMaxWidth(mMapImage->getWidth() > titleWidth ?
                    mMapImage->getWidth() + offsetX : titleWidth);
        setMaxHeight(mMapImage->getHeight() + offsetY);

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

    int mapOriginX = 0;
    int mapOriginY = 0;

    if (mMapImage)
    {
        if (mMapImage->getWidth() > a.width ||
            mMapImage->getHeight() > a.height)
        {
            const Vector &p = player_node->getPosition();
            mapOriginX = ((a.width) / 2) - static_cast<int>((p.x
            + viewport->getCameraRelativeX()) * mWidthProportion) / 32;
            mapOriginY = ((a.height) / 2) - static_cast<int>((p.y
            + viewport->getCameraRelativeX()) * mHeightProportion) / 32;

            const int minOriginX = a.width - mMapImage->getWidth();
            const int minOriginY = a.height - mMapImage->getHeight();

            if (mapOriginX < minOriginX)
                mapOriginX = minOriginX;
            if (mapOriginY < minOriginY)
                mapOriginY = minOriginY;
            if (mapOriginX > 0)
                mapOriginX = 0;
            if (mapOriginY > 0)
                mapOriginY = 0;
        }

        graph->drawImage(mMapImage, mapOriginX, mapOriginY);
    }

    if (!actorSpriteManager)
        return;

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
        else if (being->isInParty())
        {
            type = UserPalette::PARTY;
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
                static_cast<int>(pos.x * mWidthProportion) / 32
                + mapOriginX - offsetWidth,
                static_cast<int>(pos.y * mHeightProportion) / 32
                + mapOriginY - offsetHeight,
                dotSize, dotSize));
    }

    const Vector &pos = player_node->getPosition();
//    logger->log("width:" + toString(graph->getWidth()));

    int x = static_cast<int>((pos.x - (graph->getWidth() / 2)
        + viewport->getCameraRelativeX())
        * mWidthProportion) / 32 + mapOriginX;
    int y = static_cast<int>((pos.y - (graph->getHeight() / 2)
        + viewport->getCameraRelativeY())
        * mHeightProportion) / 32 + mapOriginY;

    const int w = graph->getWidth() * mWidthProportion / 32;
    const int h = graph->getHeight() * mHeightProportion / 32;

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
