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

#include "flooritem.h"

#include "configuration.h"

#include "render/graphics.h"

#include "gui/gui.h"
#include "gui/sdlfont.h"
#include "gui/userpalette.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "debug.h"

extern int serverVersion;

FloorItem::FloorItem(const int id, const int itemId, const int x, const int y,
                     const int amount, const unsigned char color) :
    ActorSprite(id),
    mItemId(itemId),
    mX(x),
    mY(y),
    mDropTime(cur_time),
    mAmount(amount),
    mPickupCount(0),
    mCursor(Cursor::CURSOR_PICKUP),
    mColor(color),
    mShowMsg(true),
    mHighlight(config.getBoolValue("floorItemsHighlight"))
{
}

void FloorItem::postInit(Map *const map, int subX, int subY)
{
    setMap(map);
    const ItemInfo &info = ItemDB::get(mItemId);
    if (map)
    {
        const int max = info.getMaxFloorOffset();
        if (subX > max)
            subX = max;
        else if (subX < -max)
            subX = -max;
        if (subY > max)
            subY = max;
        else if (subY < -max)
            subY = -max;
        mPos.x = static_cast<float>(mX * map->getTileWidth()
            + subX + mapTileSize / 2 - 8);
        mPos.y = static_cast<float>(mY * map->getTileHeight()
            + subY + mapTileSize - 8);
    }
    else
    {
        mPos.x = 0;
        mPos.y = 0;
    }

    mCursor = info.getPickupCursor();
    setupSpriteDisplay(info.getDisplay(), true, 1,
        info.getDyeColorsString(mColor));
    mYDiff = 31;
}

const ItemInfo &FloorItem::getInfo() const
{
    return ItemDB::get(mItemId);
}

std::string FloorItem::getName() const
{
    const ItemInfo &info = ItemDB::get(mItemId);
    if (serverVersion > 0)
        return info.getName(mColor);
    else
        return info.getName();
}

void FloorItem::draw(Graphics *const graphics,
                     const int offsetX, const int offsetY) const
{
    if (!mMap)
        return;

    BLOCK_START("FloorItem::draw")
    const int x = mX * mMap->getTileWidth() + offsetX;
    const int y = mY * mMap->getTileHeight() + offsetY;
    gcn::Font *font = nullptr;

    if (mHighlight)
    {
        const int curTime = cur_time;
        font = gui->getFont();
        if (mDropTime < curTime)
        {
            const int dx = mapTileSize;
            const int dy = mapTileSize;

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

    const int px = getActorX() + offsetX;
    const int py = getActorY() + offsetY;
    ActorSprite::draw1(graphics, px, py);
    CompoundSprite::draw(graphics, px, py);

    if (mHighlight)
    {
        if (font && mAmount > 1)
        {
//            graphics->setColor(gcn::Color(255, 255, 255, 100));
            graphics->setColor(userPalette->getColor(
                UserPalette::FLOOR_ITEM_TEXT));
            font->drawString(graphics, toString(mAmount), x, y);
        }
    }
    BLOCK_END("FloorItem::draw")
}
