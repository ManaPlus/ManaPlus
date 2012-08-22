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

#include "maplayer.h"

#include "configuration.h"

#include "graphicsvertexes.h"

#include "localplayer.h"

#include "resources/image.h"
#include "resources/resourcemanager.h"

#include "gui/gui.h"
#include "gui/sdlfont.h"

#include "utils/dtor.h"

#include "debug.h"

MapLayer::MapLayer(int x, int y, int width, int height, bool fringeLayer):
    mX(x), mY(y),
    mWidth(width), mHeight(height),
    mIsFringeLayer(fringeLayer),
    mHighlightAttackRange(config.getBoolValue("highlightAttackRange")),
    mTiles(new Image*[mWidth * mHeight])
{
    std::fill_n(mTiles, mWidth * mHeight, static_cast<Image*>(nullptr));

    config.addListener("highlightAttackRange", this);
}

MapLayer::~MapLayer()
{
    config.removeListener("highlightAttackRange", this);
    delete [] mTiles;
    delete_all(mTempRows);
    mTempRows.clear();
}

void MapLayer::optionChanged(const std::string &value)
{
    if (value == "highlightAttackRange")
    {
        mHighlightAttackRange =
            config.getBoolValue("highlightAttackRange");
    }
}

void MapLayer::setTile(int x, int y, Image *img)
{
    mTiles[x + y * mWidth] = img;
}

void MapLayer::draw(Graphics *graphics, int startX, int startY,
                    int endX, int endY, int scrollX, int scrollY,
                    int debugFlags) const
{
    if (!player_node)
        return;

    startX -= mX;
    startY -= mY;
    endX -= mX;
    endY -= mY;

    if (startX < 0)
        startX = 0;
    if (startY < 0)
        startY = 0;
    if (endX > mWidth)
        endX = mWidth;
    if (endY > mHeight)
        endY = mHeight;

    const int dx = (mX * 32) - scrollX;
    const int dy = (mY * 32) - scrollY + 32;
    const bool flag = (debugFlags != Map::MAP_SPECIAL
        && debugFlags != Map::MAP_SPECIAL2);

    for (int y = startY; y < endY; y++)
    {
        const int y32 = y * 32;
        const int yWidth = y * mWidth;

        const int py0 = y32 + dy;

        Image **tilePtr = mTiles + startX + yWidth;

        for (int x = startX; x < endX; x++, tilePtr++)
        {
            const int x32 = x * 32;

            int c = 0;
            Image *img = *tilePtr;
            if (img)
            {
                const int px = x32 + dx;
                const int py = py0 - img->mBounds.h;
                if (flag || img->mBounds.h <= 32)
                {
                    int width = 0;
                    // here need not draw over player position
                    c = getTileDrawWidth(img, endX - x, width);

                    if (!c)
                    {
                        graphics->drawImage(img, px, py);
                    }
                    else
                    {
                        graphics->drawImagePattern(img, px, py,
                            width, img->mBounds.h);
                    }
                }
            }

            x += c;
        }
    }
}

void MapLayer::updateSDL(Graphics *graphics, int startX, int startY,
                         int endX, int endY, int scrollX, int scrollY,
                         int debugFlags)
{
    delete_all(mTempRows);
    mTempRows.clear();

    startX -= mX;
    startY -= mY;
    endX -= mX;
    endY -= mY;

    if (startX < 0)
        startX = 0;
    if (startY < 0)
        startY = 0;
    if (endX > mWidth)
        endX = mWidth;
    if (endY > mHeight)
        endY = mHeight;

    const int dx = (mX * 32) - scrollX;
    const int dy = (mY * 32) - scrollY + 32;
    const bool flag = (debugFlags != Map::MAP_SPECIAL
        && debugFlags != Map::MAP_SPECIAL2);

    for (int y = startY; y < endY; y++)
    {
        MapRowVertexes *row = new MapRowVertexes();
        mTempRows.push_back(row);

        Image *lastImage = nullptr;
        ImageVertexes *imgVert = nullptr;

        const int yWidth = y * mWidth;
        const int py0 = y * 32 + dy;
        Image **tilePtr = mTiles + startX + yWidth;

        for (int x = startX; x < endX; x++, tilePtr++)
        {
            Image *img = *tilePtr;
            if (img)
            {
                const int px = x * 32 + dx;
                const int py = py0 - img->mBounds.h;
                if (flag || img->mBounds.h <= 32)
                {
                    if (lastImage != img)
                    {
                        imgVert = new ImageVertexes();
                        imgVert->image = img;
                        row->images.push_back(imgVert);
                        lastImage = img;
                    }
                    graphics->calcTile(imgVert, px, py);
                }
            }
        }
    }
}

void MapLayer::drawSDL(Graphics *graphics)
{
    MapRows::const_iterator rit = mTempRows.begin();
    MapRows::const_iterator rit_end = mTempRows.end();
    while (rit != rit_end)
    {
        MepRowImages *images = &(*rit)->images;
        MepRowImages::const_iterator iit = images->begin();
        MepRowImages::const_iterator iit_end = images->end();
        while (iit != iit_end)
        {
            graphics->drawTile(*iit);
            ++ iit;
        }
        ++ rit;
    }
}

void MapLayer::updateOGL(Graphics *graphics, int startX, int startY,
                         int endX, int endY, int scrollX, int scrollY,
                         int debugFlags)
{
    delete_all(mTempRows);
    mTempRows.clear();

    startX -= mX;
    startY -= mY;
    endX -= mX;
    endY -= mY;

    if (startX < 0)
        startX = 0;
    if (startY < 0)
        startY = 0;
    if (endX > mWidth)
        endX = mWidth;
    if (endY > mHeight)
        endY = mHeight;

    const int dx = (mX * 32) - scrollX;
    const int dy = (mY * 32) - scrollY + 32;
    const bool flag = (debugFlags != Map::MAP_SPECIAL
        && debugFlags != Map::MAP_SPECIAL2);

    for (int y = startY; y < endY; y++)
    {
        MapRowVertexes *row = new MapRowVertexes();
        mTempRows.push_back(row);

        Image *lastImage = nullptr;
        ImageVertexes *imgVert = nullptr;

        const int yWidth = y * mWidth;
        const int py0 = y * 32 + dy;
        std::map<Image*, ImageVertexes*> imgSet;

        Image **tilePtr = mTiles + startX + yWidth;
        for (int x = startX; x < endX; x++, tilePtr++)
        {
            Image *img = *tilePtr;
            if (img)
            {
                const int px = x * 32 + dx;
                const int py = py0 - img->mBounds.h;
                if (flag || img->mBounds.h <= 32)
                {
                    if (lastImage != img)
                    {
                        if (img->mBounds.w > 32)
                            imgSet.clear();

                        imgSet[lastImage] = imgVert;
                        if (imgSet.find(img) != imgSet.end())
                        {
                            imgVert = imgSet[img];
                        }
                        else
                        {
                            imgVert = new ImageVertexes();
                            imgVert->image = img;
                            row->images.push_back(imgVert);
                        }
                        lastImage = img;
                    }
                    graphics->calcTile(imgVert, px, py);
                }
            }
        }
    }
}

void MapLayer::drawOGL(Graphics *graphics)
{
    MapRows::const_iterator rit = mTempRows.begin();
    MapRows::const_iterator rit_end = mTempRows.end();
    while (rit != rit_end)
    {
        MepRowImages *images = &(*rit)->images;
        MepRowImages::const_iterator iit = images->begin();
        MepRowImages::const_iterator iit_end = images->end();
        while (iit != iit_end)
        {
            graphics->drawTile(*iit);
            ++ iit;
        }
        ++ rit;
    }
}

void MapLayer::drawFringe(Graphics *graphics, int startX, int startY,
                          int endX, int endY, int scrollX, int scrollY,
                          const Actors *actors, int debugFlags, int yFix) const
{
    if (!player_node || !mSpecialLayer || !mTempLayer)
        return;

    startX -= mX;
    startY -= mY;
    endX -= mX;
    endY -= mY;

    if (startX < 0)
        startX = 0;
    if (startY < 0)
        startY = 0;
    if (endX > mWidth)
        endX = mWidth;
    if (endY > mHeight)
        endY = mHeight;

    ActorsCIter ai = actors->begin();
    ActorsCIter ai_end = actors->end();

    const int dx = (mX * 32) - scrollX;
    const int dy = (mY * 32) - scrollY + 32;

    int specialWidth = mSpecialLayer->mWidth;
    int specialHeight = mSpecialLayer->mHeight;

    for (int y = startY; y < endY; y++)
    {
        const int y32 = y * 32;
        const int y32s = (y + yFix) * 32;
        const int yWidth = y * mWidth;

        // If drawing the fringe layer, make sure all actors above this row of
        // tiles have been drawn
        while (ai != ai_end && (*ai)->getPixelY() <= y32s)
        {
            (*ai)->draw(graphics, -scrollX, -scrollY);
            ++ ai;
        }

        if (debugFlags == Map::MAP_SPECIAL3
            || debugFlags == Map::MAP_BLACKWHITE)
        {
            if (y < specialHeight)
            {
                int ptr = y * specialWidth;
                const int py1 = y32 - scrollY;
                int endX1 = endX;
                if (endX1 > specialWidth)
                    endX1 = specialWidth;
                if (endX1 < 0)
                    endX1 = 0;

                for (int x = startX; x < endX1; x++)
                {
                    const int px1 = x * 32 - scrollX;

                    MapItem *item = mSpecialLayer->mTiles[ptr + x];
                    if (item)
                        item->draw(graphics, px1, py1, 32, 32);

                    item = mTempLayer->mTiles[ptr + x];
                    if (item)
                        item->draw(graphics, px1, py1, 32, 32);
                }
            }
        }
        else
        {
            const int py0 = y32 + dy;
            const int py1 = y32 - scrollY;

            Image **tilePtr = mTiles + startX + yWidth;
            for (int x = startX; x < endX; x++, tilePtr++)
            {
                const int x32 = x * 32;

                const int px1 = x32 - scrollX;
                int c = 0;
                Image *img = *tilePtr;
                if (img)
                {
                    const int px = x32 + dx;
                    const int py = py0 - img->mBounds.h;
                    if ((debugFlags != Map::MAP_SPECIAL
                        && debugFlags != Map::MAP_SPECIAL2)
                        || img->mBounds.h <= 32)
                    {
                        int width = 0;
                        // here need not draw over player position
                        c = getTileDrawWidth(img, endX - x, width);

                        if (!c)
                        {
                            graphics->drawImage(img, px, py);
                        }
                        else
                        {
                            graphics->drawImagePattern(img, px, py,
                                width, img->mBounds.h);
                        }
                    }
                }

                if (y < specialHeight)
                {
                    int c1 = c;
                    if (c1 + x + 1 > specialWidth)
                        c1 = specialWidth - x - 1;
                    if (c1 < 0)
                        c1 = 0;

                    int ptr = y * specialWidth + x;

                    for (int x1 = 0; x1 < c1 + 1; x1 ++)
                    {
                        MapItem *item1 = mSpecialLayer->mTiles[ptr + x1];
                        MapItem *item2 = mTempLayer->mTiles[ptr + x1];
                        if (item1 || item2)
                        {
                            const int px2 = px1 + (x1 * 32);
                            if (item1 && item1->mType != MapItem::EMPTY)
                                item1->draw(graphics, px2, py1, 32, 32);

                            if (item2 && item2->mType != MapItem::EMPTY)
                                item2->draw(graphics, px2, py1, 32, 32);
                        }
                    }
                }
                x += c;
            }
        }
    }

    // Draw any remaining actors
    if (debugFlags != Map::MAP_SPECIAL3)
    {
        while (ai != ai_end)
        {
            (*ai)->draw(graphics, -scrollX, -scrollY);
            ++ai;
        }
        if (mHighlightAttackRange && player_node)
        {
            const int px = player_node->getPixelX() - scrollX - 16;
            const int py = player_node->getPixelY() - scrollY - 32;
            const int attackRange = player_node->getAttackRange() * 32;

            int x = px - attackRange;
            int y = py - attackRange;
            int w = 2 * attackRange + 32;
            int h = w;
            if (attackRange <= 32)
            {
                x -= 16;
                y -= 16;
                w += 32;
                h += 32;
            }

            if (userPalette)
            {
                graphics->setColor(userPalette->getColorWithAlpha(
                    UserPalette::ATTACK_RANGE));

                graphics->fillRectangle(gcn::Rectangle(
                                                       x, y,
                                                       w, h));

                graphics->setColor(userPalette->getColorWithAlpha(
                    UserPalette::ATTACK_RANGE_BORDER));

                graphics->drawRectangle(gcn::Rectangle(
                                                       x, y,
                                                       w, h));
            }
        }
    }
}

int MapLayer::getTileDrawWidth(Image *img, int endX, int &width) const
{
    Image *img1 = img;
    int c = 0;
    if (!img1)
    {
        width = 0;
        return c;
    }
    width = img1->mBounds.w;
    for (int x = 1; x < endX; x++)
    {
        img ++;
        if (img != img1)
            break;
        c ++;
        if (img)
            width += img->mBounds.w;
    }
    return c;
}

SpecialLayer::SpecialLayer(int width, int height, bool drawSprites):
    mWidth(width), mHeight(height),
    mDrawSprites(drawSprites)
{
    const int size = mWidth * mHeight;
    mTiles = new MapItem*[size];
    std::fill_n(mTiles, size, static_cast<MapItem*>(nullptr));
}

SpecialLayer::~SpecialLayer()
{
    for (int f = 0; f < mWidth * mHeight; f ++)
    {
        delete mTiles[f];
        mTiles[f] = nullptr;
    }
    delete [] mTiles;
}

MapItem* SpecialLayer::getTile(int x, int y) const
{
    if (x < 0 || x >= mWidth ||
        y < 0 || y >= mHeight)
    {
        return nullptr;
    }
    return mTiles[x + y * mWidth];
}

void SpecialLayer::setTile(int x, int y, MapItem *item)
{
    if (x < 0 || x >= mWidth ||
        y < 0 || y >= mHeight)
    {
        return;
    }

    int idx = x + y * mWidth;
    delete mTiles[idx];
    if (item)
        item->setPos(x, y);
    mTiles[idx] = item;
}

void SpecialLayer::setTile(int x, int y, int type)
{
    if (x < 0 || x >= mWidth ||
        y < 0 || y >= mHeight)
    {
        return;
    }

    int idx = x + y * mWidth;
    if (mTiles[idx])
        mTiles[idx]->setType(type);
    else
        mTiles[idx] = new MapItem(type);
    mTiles[idx]->setPos(x, y);
}

void SpecialLayer::addRoad(Path road)
{
    for (Path::const_iterator i = road.begin(), i_end = road.end();
         i != i_end; ++i)
    {
        Position pos = (*i);
        MapItem *item = getTile(pos.x, pos.y);
        if (!item)
        {
            item = new MapItem(MapItem::ROAD);
            setTile(pos.x, pos.y, item);
        }
        else
        {
            item->setType(MapItem::ROAD);
        }
    }
}

void SpecialLayer::clean()
{
    if (!mTiles)
        return;

    for (int f = 0; f < mWidth * mHeight; f ++)
    {
        MapItem *item = mTiles[f];
        if (item)
            item->setType(MapItem::EMPTY);
    }
}

void SpecialLayer::draw(Graphics *graphics, int startX, int startY,
                        int endX, int endY, int scrollX, int scrollY)
{
    if (startX < 0)
        startX = 0;
    if (startY < 0)
        startY = 0;
    if (endX > mWidth)
        endX = mWidth;
    if (endY > mHeight)
        endY = mHeight;

    for (int y = startY; y < endY; y++)
    {
        for (int x = startX; x < endX; x++)
            itemDraw(graphics, x, y, scrollX, scrollY);
    }
}

void SpecialLayer::itemDraw(Graphics *graphics, int x, int y,
                            int scrollX, int scrollY)
{
    MapItem *item = getTile(x, y);
    if (item)
    {
        const int px = x * 32 - scrollX;
        const int py = y * 32 - scrollY;
        item->draw(graphics, px, py, 32, 32);
    }
}


MapItem::MapItem():
    mImage(nullptr), mComment(""), mName(""), mX(-1), mY(-1)
{
    setType(EMPTY);
}

MapItem::MapItem(int type):
    mImage(nullptr), mComment(""), mName(""), mX(-1), mY(-1)
{
    setType(type);
}

MapItem::MapItem(int type, std::string comment):
    mImage(nullptr), mComment(comment), mName(""), mX(-1), mY(-1)
{
    setType(type);
}

MapItem::MapItem(int type, std::string comment, int x, int y):
    mImage(nullptr), mComment(comment), mName(""), mX(x), mY(y)
{
    setType(type);
}

MapItem::~MapItem()
{
    if (mImage)
    {
        mImage->decRef();
        mImage = nullptr;
    }
}

void MapItem::setType(int type)
{
    std::string name("");
    mType = type;
    if (mImage)
        mImage->decRef();

    switch (type)
    {
        case ARROW_UP:
            name = "graphics/sprites/arrow_up.gif";
            break;
        case ARROW_DOWN:
            name = "graphics/sprites/arrow_down.gif";
            break;
        case ARROW_LEFT:
            name = "graphics/sprites/arrow_left.gif";
            break;
        case ARROW_RIGHT:
            name = "graphics/sprites/arrow_right.gif";
            break;
        default:
            break;
    }

    if (name != "")
    {
        ResourceManager *resman = ResourceManager::getInstance();
        mImage = resman->getImage(name);
    }
    else
    {
        mImage = nullptr;
    }
}

void MapItem::setPos(int x, int y)
{
    mX = x;
    mY = y;
}

void MapItem::draw(Graphics *graphics, int x, int y, int dx, int dy)
{
    if (mImage)
        graphics->drawImage(mImage, x, y);

    switch (mType)
    {
        case ROAD:
        case CROSS:
            graphics->setColor(userPalette->getColorWithAlpha(
                               UserPalette::ROAD_POINT));
            graphics->fillRectangle(gcn::Rectangle(x + dx / 3, y + dy / 3,
                                    dx / 3, dy / 3));
            break;
        case HOME:
        {
            graphics->setColor(userPalette->getColorWithAlpha(
                               UserPalette::HOME_PLACE));
            graphics->fillRectangle(gcn::Rectangle(
                                                   x, y,
                                                   dx, dy));
            graphics->setColor(userPalette->getColorWithAlpha(
                               UserPalette::HOME_PLACE_BORDER));
            graphics->drawRectangle(gcn::Rectangle(
                                                   x, y,
                                                   dx, dy));
            break;
        }
        default:
            break;
    }
    if (!mName.empty() && mType != PORTAL && mType != EMPTY)
    {
        gcn::Font *font = gui->getFont();
        if (font)
        {
            graphics->setColor(userPalette->getColor(UserPalette::BEING));
            font->drawString(graphics, mName, x, y);
        }
    }
}

ObjectsLayer::ObjectsLayer(unsigned width, unsigned height) :
    mWidth(width), mHeight(height)
{
    const unsigned size = width * height;
    mTiles = new MapObjectList*[size];
    std::fill_n(mTiles, size, static_cast<MapObjectList*>(nullptr));
}

ObjectsLayer::~ObjectsLayer()
{
    const unsigned size = mWidth * mHeight;
    for (unsigned f = 0; f < size; f ++)
        delete mTiles[f];

    delete [] mTiles;
    mTiles = nullptr;
}

void ObjectsLayer::addObject(std::string name, int type,
                             unsigned x, unsigned y,
                             unsigned dx, unsigned dy)
{
    if (!mTiles)
        return;

    if (x + dx > mWidth)
        dx = mWidth - x;
    if (y + dy > mHeight)
        dy = mHeight - y;

    for (unsigned y1 = y; y1 < y + dy; y1 ++)
    {
        unsigned idx1 = x + y1 * mWidth;
        unsigned idx2 = idx1 + dx;

        for (unsigned i = idx1; i < idx2; i ++)
        {
            if (!mTiles[i])
                mTiles[i] = new MapObjectList();
            mTiles[i]->objects.push_back(MapObject(type, name));
        }
    }
}

MapObjectList *ObjectsLayer::getAt(unsigned x, unsigned y)
{
    if (x >= mWidth || y >= mHeight)
        return nullptr;
    return mTiles[x + y * mWidth];
}

MapRowVertexes::~MapRowVertexes()
{
    delete_all(images);
    images.clear();
}
