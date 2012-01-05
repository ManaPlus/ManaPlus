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

#include "flooritem.h"

#include "client.h"
#include "graphics.h"
#include "item.h"
#include "map.h"
#include "configuration.h"

#include "gui/gui.h"
#include "gui/sdlfont.h"

#include "net/net.h"

#include "resources/itemdb.h"
#include "resources/iteminfo.h"

#include "debug.h"

FloorItem::FloorItem(int id,
                     int itemId,
                     int x,
                     int y,
                     Map *map,
                     int amount,
                     unsigned char color):
    ActorSprite(id),
    mItemId(itemId),
    mX(x),
    mY(y),
    mMap(map),
    mDropTime(cur_time),
    mAmount(amount),
    mPickupCount(0),
    mColor(color),
    mShowMsg(true),
    mHighlight(config.getBoolValue("floorItemsHighlight"))
{
    setMap(map);
    if (map)
    {
        // TODO: Eventually, we probably should fix all sprite offsets so that
        //       these translations aren't necessary anymore. The sprites know
        //       best where their base point should be.
        mPos.x = static_cast<float>(x * map->getTileWidth() + 16);
#ifdef MANASERV_SUPPORT
        mPos.y = static_cast<float>(y * map->getTileHeight() +
                 ((Net::getNetworkType() == ServerInfo::MANASERV) ? 15 : 32));
#else
        mPos.y = static_cast<float>(y * map->getTileHeight() + 32);
#endif
    }
    else
    {
        mPos.x = 0;
        mPos.y = 0;
    }

    const ItemInfo &info = ItemDB::get(itemId);
    setupSpriteDisplay(info.getDisplay(), true, 1,
        info.getDyeColorsString(mColor));
    mYDiff = 31;
}

const ItemInfo &FloorItem::getInfo() const
{
    return ItemDB::get(mItemId);
}

bool FloorItem::draw(Graphics *graphics, int offsetX, int offsetY) const
{
    if (!mMap)
        return false;

    const int dx = 32;
    const int dy = 32;

    const int x = mX * mMap->getTileWidth() + offsetX;
    const int y = mY * mMap->getTileHeight() + offsetY;
    gcn::Font *font = nullptr;

    if (mHighlight)
    {
        int curTime = cur_time;
        font = gui->getFont();
        if (mDropTime < curTime)
        {
            if (curTime > mDropTime + 28 && curTime < mDropTime + 50)
            {
                graphics->setColor(gcn::Color(80, 200, 20, 200));
                graphics->fillRectangle(gcn::Rectangle(
                                        x, y, dx, dy));
            }
            else if (curTime > mDropTime + 19
                     && curTime < mDropTime + 28)
            {
                graphics->setColor(gcn::Color(200, 80, 20,
                    80 + 10 * (curTime - mDropTime - 18)));
                graphics->fillRectangle(gcn::Rectangle(
                    x, y, dx, dy));
            }
            else if (curTime > mDropTime && curTime < mDropTime + 20)
            {
                graphics->setColor(gcn::Color(20, 20, 255,
                    7 * (curTime - mDropTime)));
                graphics->fillRectangle(gcn::Rectangle(
                                        x, y, dx, dy));
            }
        }
    }

    const bool res = ActorSprite::draw(graphics, offsetX, offsetY);

    if (mHighlight)
    {
        if (font && mAmount > 1)
        {
            graphics->setColor(gcn::Color(255, 255, 255, 100));
            font->drawString(graphics, toString(mAmount), x, y);
        }
    }
    return res;
}
