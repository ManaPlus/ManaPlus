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

#include "maplayer.h"

#include "configuration.h"
#include "graphicsvertexes.h"

#ifndef USE_OPENGL
#include "render/graphics.h"
#endif

#include "being/localplayer.h"

#include "render/graphics.h"

#include "resources/resourcemanager.h"

#include "gui/gui.h"
#include "gui/sdlfont.h"

#include "utils/dtor.h"

#include "debug.h"

MapLayer::MapLayer(const int x, const int y, const int width, const int height,
                   const bool fringeLayer, const int mask):
    mX(x),
    mY(y),
    mWidth(width),
    mHeight(height),
    mTiles(new Image*[mWidth * mHeight]),
    mSpecialLayer(nullptr),
    mTempLayer(nullptr),
    mTempRows(),
    mMask(mask),
    mIsFringeLayer(fringeLayer),
    mHighlightAttackRange(config.getBoolValue("highlightAttackRange"))
{
    std::fill_n(mTiles, mWidth * mHeight, static_cast<Image*>(nullptr));

    config.addListener("highlightAttackRange", this);
}

MapLayer::~MapLayer()
{
    config.removeListener("highlightAttackRange", this);
    CHECKLISTENERS
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

void MapLayer::setTile(const int x, const int y, Image *const img)
{
    mTiles[x + y * mWidth] = img;
}

void MapLayer::draw(Graphics *const graphics,
                    int startX, int startY, int endX, int endY,
                    const int scrollX, const int scrollY,
                    const int debugFlags) const
{
    if (!player_node)
        return;

    BLOCK_START("MapLayer::draw")
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

    const int dx = (mX * mapTileSize) - scrollX;
    const int dy = (mY * mapTileSize) - scrollY + mapTileSize;
    const bool flag = (debugFlags != Map::MAP_SPECIAL
        && debugFlags != Map::MAP_SPECIAL2);

    for (int y = startY; y < endY; y++)
    {
        const int y32 = y * mapTileSize;
        const int yWidth = y * mWidth;

        const int py0 = y32 + dy;

        Image **tilePtr = mTiles + startX + yWidth;

        for (int x = startX; x < endX; x++, tilePtr++)
        {
            const int x32 = x * mapTileSize;

            int c = 0;
            const Image *const img = *tilePtr;
            if (img)
            {
                const int px = x32 + dx;
                const int py = py0 - img->mBounds.h;
                if (flag || img->mBounds.h <= mapTileSize)
                {
                    int width = 0;
                    // here need not draw over player position
                    c = getTileDrawWidth(img, endX - x, width);

                    if (!c)
                    {
                        graphics->drawImage2(img, px, py);
                    }
                    else
                    {
                        graphics->drawPattern(img, px, py,
                            width, img->mBounds.h);
                    }
                }
            }

            x += c;
        }
    }
    BLOCK_END("MapLayer::draw")
}

void MapLayer::drawSDL(Graphics *const graphics)
{
    BLOCK_START("MapLayer::drawSDL")
    MapRows::const_iterator rit = mTempRows.begin();
    const MapRows::const_iterator rit_end = mTempRows.end();
    while (rit != rit_end)
    {
        MepRowImages *const images = &(*rit)->images;
        MepRowImages::const_iterator iit = images->begin();
        const MepRowImages::const_iterator iit_end = images->end();
        while (iit != iit_end)
        {
            graphics->drawTileVertexes(*iit);
            ++ iit;
        }
        ++ rit;
    }
    BLOCK_END("MapLayer::drawSDL")
}

#ifdef USE_OPENGL
void MapLayer::updateSDL(Graphics *const graphics, int startX, int startY,
                         int endX, int endY,
                         const int scrollX, const int scrollY,
                         const int debugFlags)
{
    BLOCK_START("MapLayer::updateSDL")
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

    const int dx = (mX * mapTileSize) - scrollX;
    const int dy = (mY * mapTileSize) - scrollY + mapTileSize;
    const bool flag = (debugFlags != Map::MAP_SPECIAL
        && debugFlags != Map::MAP_SPECIAL2);

    for (int y = startY; y < endY; y++)
    {
        MapRowVertexes *const row = new MapRowVertexes();
        mTempRows.push_back(row);

        const Image *lastImage = nullptr;
        ImageVertexes *imgVert = nullptr;

        const int yWidth = y * mWidth;
        const int py0 = y * mapTileSize + dy;
        Image **tilePtr = mTiles + startX + yWidth;

        for (int x = startX; x < endX; x++, tilePtr++)
        {
            Image *const img = *tilePtr;
            if (img)
            {
                const int px = x * mapTileSize + dx;
                const int py = py0 - img->mBounds.h;
                if (flag || img->mBounds.h <= mapTileSize)
                {
                    if (lastImage != img)
                    {
                        imgVert = new ImageVertexes();
                        imgVert->image = img;
                        row->images.push_back(imgVert);
                        lastImage = img;
                    }
                    graphics->calcTileSDL(imgVert, px, py);
                }
            }
        }
    }
    BLOCK_END("MapLayer::updateSDL")
}

void MapLayer::updateOGL(Graphics *const graphics, int startX, int startY,
                         int endX, int endY,
                         const int scrollX, const int scrollY,
                         const int debugFlags)
{
    BLOCK_START("MapLayer::updateOGL")
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

    const int dx = (mX * mapTileSize) - scrollX;
    const int dy = (mY * mapTileSize) - scrollY + mapTileSize;
    const bool flag = (debugFlags != Map::MAP_SPECIAL
        && debugFlags != Map::MAP_SPECIAL2);

    MapRowVertexes *const row = new MapRowVertexes();
    mTempRows.push_back(row);
    Image *lastImage = nullptr;
    ImageVertexes *imgVert = nullptr;
    std::map<int, ImageVertexes*> imgSet;

    for (int y = startY; y < endY; y++)
    {
        const int yWidth = y * mWidth;
        const int py0 = y * mapTileSize + dy;
        Image **tilePtr = mTiles + startX + yWidth;
        for (int x = startX; x < endX; x++, tilePtr++)
        {
            Image *const img = *tilePtr;
            if (img)
            {
                const int px = x * mapTileSize + dx;
                const int py = py0 - img->mBounds.h;
                const GLuint imgGlImage = img->mGLImage;
                if (flag || img->mBounds.h <= mapTileSize)
                {
                    if (!lastImage || lastImage->mGLImage != imgGlImage)
                    {
                        if (img->mBounds.w > mapTileSize)
                            imgSet.clear();

                        if (imgSet.find(imgGlImage) != imgSet.end())
                        {
                            imgVert = imgSet[imgGlImage];
                        }
                        else
                        {
                            if (lastImage)
                                imgSet[lastImage->mGLImage] = imgVert;
                            imgVert = new ImageVertexes();
                            imgVert->ogl.init();
                            imgVert->image = img;
                            row->images.push_back(imgVert);
                        }
                    }
                    lastImage = img;
//                    if (imgVert->image->mGLImage != lastImage->mGLImage)
//                        logger->log("wrong image draw");
                    graphics->calcTileVertexes(imgVert, lastImage, px, py);
                }
            }
        }
    }
    BLOCK_END("MapLayer::updateOGL")
}

void MapLayer::drawOGL(Graphics *const graphics)
{
    BLOCK_START("MapLayer::drawOGL")
    MapRows::const_iterator rit = mTempRows.begin();
    const MapRows::const_iterator rit_end = mTempRows.end();
//    int k = 0;
    while (rit != rit_end)
    {
        const MepRowImages *const images = &(*rit)->images;
        MepRowImages::const_iterator iit = images->begin();
        const MepRowImages::const_iterator iit_end = images->end();
        while (iit != iit_end)
        {
            graphics->drawTileVertexes(*iit);
            ++ iit;
//            k ++;
        }
        ++ rit;
    }
    BLOCK_END("MapLayer::drawOGL")
//    logger->log("draws: %d", k);
}
#endif

void MapLayer::drawFringe(Graphics *const graphics, int startX, int startY,
                          int endX, int endY,
                          const int scrollX, const int scrollY,
                          const Actors *const actors,
                          const int debugFlags, const int yFix) const
{
    BLOCK_START("MapLayer::drawFringe")
    if (!player_node || !mSpecialLayer || !mTempLayer)
    {
        BLOCK_END("MapLayer::drawFringe")
        return;
    }

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
    const ActorsCIter ai_end = actors->end();

    const int dx = (mX * mapTileSize) - scrollX;
    const int dy = (mY * mapTileSize) - scrollY + mapTileSize;

    const int specialWidth = mSpecialLayer->mWidth;
    const int specialHeight = mSpecialLayer->mHeight;

    for (int y = startY; y < endY; y++)
    {
        const int y32 = y * mapTileSize;
        const int y32s = (y + yFix) * mapTileSize;
        const int yWidth = y * mWidth;

        BLOCK_START("MapLayer::drawFringe drawmobs")
        // If drawing the fringe layer, make sure all actors above this row of
        // tiles have been drawn
        while (ai != ai_end && (*ai)->getSortPixelY() <= y32s)
        {
            (*ai)->draw(graphics, -scrollX, -scrollY);
            ++ ai;
        }
        BLOCK_END("MapLayer::drawFringe drawmobs")

        if (debugFlags == Map::MAP_SPECIAL3
            || debugFlags == Map::MAP_BLACKWHITE)
        {
            if (y < specialHeight)
            {
                const int ptr = y * specialWidth;
                const int py1 = y32 - scrollY;
                int endX1 = endX;
                if (endX1 > specialWidth)
                    endX1 = specialWidth;
                if (endX1 < 0)
                    endX1 = 0;

                for (int x = startX; x < endX1; x++)
                {
                    const int px1 = x * mapTileSize - scrollX;

                    const MapItem *item = mSpecialLayer->mTiles[ptr + x];
                    if (item)
                    {
                        item->draw(graphics, px1, py1,
                            mapTileSize, mapTileSize);
                    }

                    item = mTempLayer->mTiles[ptr + x];
                    if (item)
                    {
                        item->draw(graphics, px1, py1,
                            mapTileSize, mapTileSize);
                    }
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
                const int x32 = x * mapTileSize;

                const int px1 = x32 - scrollX;
                int c = 0;
                const Image *const img = *tilePtr;
                if (img)
                {
                    const int px = x32 + dx;
                    const int py = py0 - img->mBounds.h;
                    if ((debugFlags != Map::MAP_SPECIAL
                        && debugFlags != Map::MAP_SPECIAL2)
                        || img->mBounds.h <= mapTileSize)
                    {
                        int width = 0;
                        // here need not draw over player position
                        c = getTileDrawWidth(img, endX - x, width);

                        if (!c)
                        {
                            graphics->drawImage2(img, px, py);
                        }
                        else
                        {
                            graphics->drawPattern(img, px, py,
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

                    const int ptr = y * specialWidth + x;

                    for (int x1 = 0; x1 < c1 + 1; x1 ++)
                    {
                        const MapItem *const item1
                            = mSpecialLayer->mTiles[ptr + x1];
                        const MapItem *const item2
                            = mTempLayer->mTiles[ptr + x1];
                        if (item1 || item2)
                        {
                            const int px2 = px1 + (x1 * mapTileSize);
                            if (item1 && item1->mType != MapItem::EMPTY)
                            {
                                item1->draw(graphics, px2, py1,
                                    mapTileSize, mapTileSize);
                            }

                            if (item2 && item2->mType != MapItem::EMPTY)
                            {
                                item2->draw(graphics, px2, py1,
                                    mapTileSize, mapTileSize);
                            }
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
        BLOCK_START("MapLayer::drawFringe drawmobs")
        while (ai != ai_end)
        {
            (*ai)->draw(graphics, -scrollX, -scrollY);
            ++ai;
        }
        BLOCK_END("MapLayer::drawFringe drawmobs")
        if (mHighlightAttackRange && player_node)
        {
            const int px = player_node->getPixelX()
                - scrollX - mapTileSize / 2;
            const int py = player_node->getPixelY() - scrollY - mapTileSize;
            const int attackRange = player_node->getAttackRange()
                * mapTileSize;

            int x = px - attackRange;
            int y = py - attackRange;
            int w = 2 * attackRange + mapTileSize;
            int h = w;
            if (attackRange <= mapTileSize)
            {
                x -= mapTileSize / 2;
                y -= mapTileSize / 2;
                w += mapTileSize;
                h += mapTileSize;
            }

            if (userPalette)
            {
                graphics->setColor(userPalette->getColorWithAlpha(
                    UserPalette::ATTACK_RANGE));
                graphics->fillRectangle(gcn::Rectangle(x, y, w, h));
                graphics->setColor(userPalette->getColorWithAlpha(
                    UserPalette::ATTACK_RANGE_BORDER));
                graphics->drawRectangle(gcn::Rectangle(x, y, w, h));
            }
        }
    }
    BLOCK_END("MapLayer::drawFringe")
}

int MapLayer::getTileDrawWidth(const Image *img,
                               const int endX, int &width) const
{
    BLOCK_START("MapLayer::getTileDrawWidth")
    const Image *const img1 = img;
    int c = 0;
    if (!img1)
    {
        width = 0;
        BLOCK_END("MapLayer::getTileDrawWidth")
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
    BLOCK_END("MapLayer::getTileDrawWidth")
    return c;
}

SpecialLayer::SpecialLayer(const int width, const int height) :
    mWidth(width),
    mHeight(height),
    mTiles(new MapItem*[mWidth * mHeight])
{
    std::fill_n(mTiles, mWidth * mHeight, static_cast<MapItem*>(nullptr));
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

MapItem* SpecialLayer::getTile(const int x, const int y) const
{
    if (x < 0 || x >= mWidth ||
        y < 0 || y >= mHeight)
    {
        return nullptr;
    }
    return mTiles[x + y * mWidth];
}

void SpecialLayer::setTile(const int x, const int y, MapItem *const item)
{
    if (x < 0 || x >= mWidth ||
        y < 0 || y >= mHeight)
    {
        return;
    }

    const int idx = x + y * mWidth;
    delete mTiles[idx];
    if (item)
        item->setPos(x, y);
    mTiles[idx] = item;
}

void SpecialLayer::setTile(const int x, const int y, const int type)
{
    if (x < 0 || x >= mWidth ||
        y < 0 || y >= mHeight)
    {
        return;
    }

    const int idx = x + y * mWidth;
    MapItem *const tile = mTiles[idx];
    if (tile)
    {
        tile->setType(type);
        tile->setPos(x, y);
    }
    else
    {
        mTiles[idx] = new MapItem(type);
        mTiles[idx]->setPos(x, y);
    }
}

void SpecialLayer::addRoad(const Path &road)
{
    FOR_EACH (Path::const_iterator, i, road)
    {
        const Position &pos = (*i);
        MapItem *const item = getTile(pos.x, pos.y);
        if (!item)
            setTile(pos.x, pos.y, new MapItem(MapItem::ROAD));
        else
            item->setType(MapItem::ROAD);
    }
}

void SpecialLayer::clean() const
{
    if (!mTiles)
        return;

    for (int f = 0; f < mWidth * mHeight; f ++)
    {
        MapItem *const item = mTiles[f];
        if (item)
            item->setType(MapItem::EMPTY);
    }
}

void SpecialLayer::draw(Graphics *const graphics, int startX, int startY,
                        int endX, int endY,
                        const int scrollX, const int scrollY) const
{
    BLOCK_START("SpecialLayer::draw")
    if (startX < 0)
        startX = 0;
    if (startY < 0)
        startY = 0;
    if (endX > mWidth)
        endX = mWidth;
    if (endY > mHeight)
        endY = mHeight;

    for (int y = startY; y < endY; y ++)
    {
        const int py = y * mapTileSize - scrollY;
        const int y2 = y * mWidth;
        for (int x = startX; x < endX; x ++)
        {
            const MapItem *const item = mTiles[x + y2];
            if (item)
            {
                item->draw(graphics, x * mapTileSize - scrollX, py,
                    mapTileSize, mapTileSize);
            }
        }
    }
    BLOCK_END("SpecialLayer::draw")
}

MapItem::MapItem():
    mImage(nullptr),
    mComment(),
    mName(),
    mType(EMPTY),
    mX(-1),
    mY(-1)
{
    setType(EMPTY);
}

MapItem::MapItem(const int type):
    mImage(nullptr),
    mComment(),
    mName(),
    mType(type),
    mX(-1),
    mY(-1)
{
    setType(type);
}

MapItem::MapItem(const int type, std::string comment):
    mImage(nullptr),
    mComment(comment),
    mName(),
    mType(type),
    mX(-1),
    mY(-1)
{
    setType(type);
}

MapItem::MapItem(const int type, std::string comment,
                 const int x, const int y):
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
    if (mImage)
    {
        mImage->decRef();
        mImage = nullptr;
    }
}

void MapItem::setType(const int type)
{
    std::string name;
    mType = type;
    if (mImage)
        mImage->decRef();

    switch (type)
    {
        case ARROW_UP:
            name = "graphics/sprites/arrow_up.png";
            break;
        case ARROW_DOWN:
            name = "graphics/sprites/arrow_down.png";
            break;
        case ARROW_LEFT:
            name = "graphics/sprites/arrow_left.png";
            break;
        case ARROW_RIGHT:
            name = "graphics/sprites/arrow_right.png";
            break;
        default:
            break;
    }

    if (!name.empty())
    {
        ResourceManager *const resman = ResourceManager::getInstance();
        mImage = resman->getImage(name);
    }
    else
    {
        mImage = nullptr;
    }
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
    if (mImage)
        graphics->drawImage2(mImage, x, y);

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
            graphics->fillRectangle(gcn::Rectangle(x, y, dx, dy));
            graphics->setColor(userPalette->getColorWithAlpha(
                               UserPalette::HOME_PLACE_BORDER));
            graphics->drawRectangle(gcn::Rectangle(x, y, dx, dy));
            break;
        }
        default:
            break;
    }
    if (!mName.empty() && mType != PORTAL && mType != EMPTY)
    {
        gcn::Font *const font = gui->getFont();
        if (font)
        {
            graphics->setColor(userPalette->getColor(UserPalette::BEING));
            font->drawString(graphics, mName, x, y);
        }
    }
    BLOCK_END("MapItem::draw")
}

ObjectsLayer::ObjectsLayer(const unsigned width, const unsigned height) :
    mTiles(new MapObjectList*[width * height]),
    mWidth(width),
    mHeight(height)
{
    std::fill_n(mTiles, width * height, static_cast<MapObjectList*>(nullptr));
}

ObjectsLayer::~ObjectsLayer()
{
    const unsigned size = mWidth * mHeight;
    for (unsigned f = 0; f < size; f ++)
        delete mTiles[f];

    delete [] mTiles;
    mTiles = nullptr;
}

void ObjectsLayer::addObject(std::string name, const int type,
                             const unsigned x, const unsigned y,
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
        const unsigned idx1 = x + y1 * mWidth;
        const unsigned idx2 = idx1 + dx;

        for (unsigned i = idx1; i < idx2; i ++)
        {
            if (!mTiles[i])
                mTiles[i] = new MapObjectList();
            mTiles[i]->objects.push_back(MapObject(type, name));
        }
    }
}

MapObjectList *ObjectsLayer::getAt(const unsigned x, const unsigned y) const
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
