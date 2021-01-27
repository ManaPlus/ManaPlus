/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "being/flooritem.h"

#include "configuration.h"

#include "render/graphics.h"

#include "gui/gui.h"
#include "gui/userpalette.h"

#include "gui/fonts/font.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "resources/map/map.h"

#ifdef TMWA_SUPPORT
#include "net/net.h"
#endif  // TMWA_SUPPORT
#include "net/serverfeatures.h"

#include "debug.h"

extern volatile time_t cur_time;

FloorItem::FloorItem(const BeingId id,
                     const int itemId,
                     const int x, const int y,
                     const ItemTypeT itemType,
                     const int amount,
                     const int refine,
                     const ItemColor color,
                     const Identified identified,
                     const Damaged damaged,
                     const int *const cards) :
    ActorSprite(id),
    mCards(),
    mItemId(itemId),
    mX(x),
    mY(y),
    mDropTime(cur_time),
    mAmount(amount),
    mRefine(refine),
    mHeightPosDiff(0),
    mItemType(itemType),
    mPickupCount(0),
    mCursor(Cursor::CURSOR_PICKUP),
    mColor(color),
    mIdentified(identified),
    mDamaged(damaged),
    mShowMsg(true),
    mHighlight(config.getBoolValue("floorItemsHighlight"))
{
    setCards(cards, maxCards);
}

void FloorItem::postInit(Map *const map, int subX, int subY)
{
    setMap(map);
    const ItemInfo &info = ItemDB::get(mItemId);
    if (map != nullptr)
    {
        const int maxX = info.getMaxFloorOffsetX();
        const int maxY = info.getMaxFloorOffsetY();

        if (!serverFeatures->haveExtendedDropsPosition())
        {
            if (subX > maxX)
                subX = maxX;
            else if (subX < -maxX)
                subX = -maxX;
            if (subY > maxY)
                subY = maxY;
            else if (subY < -maxY)
                subY = -maxY;

            subX -= 8;
            subY -= 8;
        }

        mHeightPosDiff = map->getHeightOffset(mX, mY) * 16;
        mPixelX = mX * map->getTileWidth()
            + subX + mapTileSize / 2;
        mPixelY = mY * map->getTileHeight()
            + subY + mapTileSize - mHeightPosDiff;
        mPos.x = static_cast<float>(mPixelX);
        mPos.y = static_cast<float>(mPixelY);
        mYDiff = 31 - mHeightPosDiff;
    }
    else
    {
        mPixelX = 0;
        mPixelY = 0;
        mPos.x = 0;
        mPos.y = 0;
        mYDiff = 31;
    }

    mCursor = info.getPickupCursor();
    setupSpriteDisplay(info.getDisplay(),
        ForceDisplay_true,
        DisplayType::Floor,
        info.getDyeIconColorsString(mColor));
}

void FloorItem::setCards(const int *const cards,
                         int sz)
{
    if (sz < 0 || cards == nullptr)
        return;
    if (sz > maxCards)
        sz = maxCards;
    for (int f = 0; f < sz; f ++)
        mCards[f] = cards[f];
}

const ItemInfo &FloorItem::getInfo() const
{
    return ItemDB::get(mItemId);
}

std::string FloorItem::getName() const
{
    const ItemInfo &info = ItemDB::get(mItemId);
#ifdef TMWA_SUPPORT
    if (Net::getNetworkType() == ServerType::TMWATHENA)
    {
        return info.getName();
    }
#endif  // TMWA_SUPPORT

    return info.getName(mColor);
}

void FloorItem::draw(Graphics *const graphics,
                     const int offsetX, const int offsetY) const
{
    if (mMap == nullptr)
        return;

    BLOCK_START("FloorItem::draw")
    const int x = mX * mMap->getTileWidth() + offsetX;
    const int y = mY * mMap->getTileHeight() + offsetY - mHeightPosDiff;
    Font *font = nullptr;

    if (mHighlight)
    {
        const time_t curTime = cur_time;
        font = gui->getFont();
        if (mDropTime < curTime)
        {
            const int dx = mapTileSize;
            const int dy = mapTileSize;

            if (curTime > mDropTime + 28 && curTime < mDropTime + 50)
            {
                graphics->setColor(Color(80, 200, 20, 200));
                graphics->fillRectangle(Rect(
                                        x, y, dx, dy));
            }
            else if (curTime > mDropTime + 19
                     && curTime < mDropTime + 28)
            {
                graphics->setColor(Color(200, 80, 20,
                    80 + 10 * CAST_S32(curTime - mDropTime - 18)));
                graphics->fillRectangle(Rect(
                    x, y, dx, dy));
            }
            else if (curTime > mDropTime && curTime < mDropTime + 20)
            {
                graphics->setColor(Color(20, 20, 255,
                    7 * CAST_S32(curTime - mDropTime)));
                graphics->fillRectangle(Rect(x, y, dx, dy));
            }
        }
    }

    const int px = getActorX() + offsetX;
    const int py = getActorY() + offsetY;
    CompoundSprite::drawSimple(graphics, px, py);

    if (mHighlight)
    {
        if (font != nullptr && mAmount > 1)
        {
            const Color &color = userPalette->getColor(
                UserColorId::FLOOR_ITEM_TEXT,
                255U);
            font->drawString(graphics,
                color, color,
                toString(mAmount),
                x, y);
        }
    }
    BLOCK_END("FloorItem::draw")
}

int FloorItem::getCard(const int index) const
{
    if (index < 0 || index >= maxCards)
        return 0;
    return mCards[index];
}
