/*
 *  The ManaPlus Client
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

#include "resources/map/mapitem.h"

#include "enums/resources/map/mapitemtype.h"

#include "gui/gui.h"
#include "gui/userpalette.h"

#include "gui/fonts/font.h"

#include "resources/image/image.h"

#include "resources/loaders/imageloader.h"

#include "render/graphics.h"

#include "debug.h"

MapItem::MapItem() :
    mImage(nullptr),
    mComment(),
    mName(),
    mType(MapItemType::EMPTY),
    mX(-1),
    mY(-1)
{
    setType(MapItemType::EMPTY);
}

MapItem::MapItem(const int type) :
    mImage(nullptr),
    mComment(),
    mName(),
    mType(type),
    mX(-1),
    mY(-1)
{
    setType(type);
}

MapItem::MapItem(const int type,
                 const std::string &comment) :
    mImage(nullptr),
    mComment(comment),
    mName(),
    mType(type),
    mX(-1),
    mY(-1)
{
    setType(type);
}

MapItem::MapItem(const int type,
                 const std::string &comment,
                 const int x,
                 const int y) :
    mImage(nullptr),
    mComment(comment),
    mName(),
    mType(type),
    mX(x),
    mY(y)
{
    setType(type);
}

MapItem::~MapItem()
{
    if (mImage != nullptr)
    {
        mImage->decRef();
        mImage = nullptr;
    }
}

void MapItem::setType(const int type)
{
    std::string name;
    mType = type;
    if (mImage != nullptr)
        mImage->decRef();

    switch (type)
    {
        case MapItemType::ARROW_UP:
            name = "graphics/sprites/arrow_up.png";
            break;
        case MapItemType::ARROW_DOWN:
            name = "graphics/sprites/arrow_down.png";
            break;
        case MapItemType::ARROW_LEFT:
            name = "graphics/sprites/arrow_left.png";
            break;
        case MapItemType::ARROW_RIGHT:
            name = "graphics/sprites/arrow_right.png";
            break;
        default:
            break;
    }

    if (!name.empty())
        mImage = Loader::getImage(name);
    else
        mImage = nullptr;
}

void MapItem::setPos(const int x, const int y)
{
    mX = x;
    mY = y;
}

void MapItem::draw(Graphics *const graphics, const int x, const int y,
                   const int dx, const int dy) const
{
    BLOCK_START("MapItem::draw")
    if (mImage != nullptr)
        graphics->drawImage(mImage, x, y);

    switch (mType)
    {
        case MapItemType::ROAD:
        case MapItemType::CROSS:
            graphics->setColor(userPalette->getColorWithAlpha(
                               UserColorId::ROAD_POINT));
            graphics->fillRectangle(Rect(x + dx / 3, y + dy / 3,
                                    dx / 3, dy / 3));
            break;
        case MapItemType::HOME:
        {
            graphics->setColor(userPalette->getColorWithAlpha(
                               UserColorId::HOME_PLACE));
            graphics->fillRectangle(Rect(x, y, dx, dy));
            graphics->setColor(userPalette->getColorWithAlpha(
                               UserColorId::HOME_PLACE_BORDER));
            graphics->drawRectangle(Rect(x, y, dx, dy));
            break;
        }
        default:
            break;
    }
    if (!mName.empty()
        && mType != MapItemType::PORTAL
        && mType != MapItemType::EMPTY)
    {
        Font *const font = gui->getFont();
        const Color &color = userPalette->getColor(UserColorId::BEING, 255U);
        font->drawString(graphics,
            color,
            color,
            mName,
            x, y);
    }
    BLOCK_END("MapItem::draw")
}
