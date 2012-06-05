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

#ifndef MAPLAYER_H
#define MAPLAYER_H

#include "actor.h"
#include "configlistener.h"
#include "position.h"
#include "properties.h"

#include <string>
#include <vector>

class MapItem;
class MapLayer;
class SpecialLayer;
class ImageVertexes;

typedef std::vector<ImageVertexes*> MepRowImages;

class MapRowVertexes
{
    public:
        MapRowVertexes()
        {
            images.reserve(30);
        }

        ~MapRowVertexes();

        MepRowImages images;
};

class MapObject
{
    public:
        MapObject(int type0, std::string data0) :
            type(type0), data(data0)
        {
        }

        int type;
        std::string data;
};

class MapObjectList
{
    public:
        std::vector<MapObject> objects;
};

/**
 * A map layer. Stores a grid of tiles and their offset, and implements layer
 * rendering.
 */
class MapLayer: public ConfigListener
{
    public:
        friend class Map;

        /**
         * Constructor, taking layer origin, size and whether this layer is the
         * fringe layer. The fringe layer is the layer that draws the actors.
         * There can be only one fringe layer per map.
         */
        MapLayer(int x, int y, int width, int height, bool isFringeLayer);

        /**
         * Destructor.
         */
        ~MapLayer();

        /**
         * Set tile image, with x and y in layer coordinates.
         */
        void setTile(int x, int y, Image *img);

        /**
         * Set tile image with x + y * width already known.
         */
        void setTile(int index, Image *img)
        { mTiles[index] = img; }

        /**
         * Draws this layer to the given graphics context. The coordinates are
         * expected to be in map range and will be translated to local layer
         * coordinates and clipped to the layer's dimensions.
         *
         * The given actors are only drawn when this layer is the fringe
         * layer.
         */
        void draw(Graphics *graphics,
                  int startX, int startY,
                  int endX, int endY,
                  int scrollX, int scrollY,
                  int mDebugFlags) const;

        void drawOGL(Graphics *graphics);

        void drawSDL(Graphics *graphics);

        void updateOGL(Graphics *graphics,
                       int startX, int startY,
                       int endX, int endY,
                       int scrollX, int scrollY,
                       int mDebugFlags);

        void updateSDL(Graphics *graphics,
                       int startX, int startY,
                       int endX, int endY,
                       int scrollX, int scrollY,
                       int mDebugFlags);

        void drawFringe(Graphics *graphics,
                        int startX, int startY,
                        int endX, int endY,
                        int scrollX, int scrollY,
                        const Actors *actors,
                        int mDebugFlags, int yFix) const;

        bool isFringeLayer() const
        { return mIsFringeLayer; }

        void setSpecialLayer(SpecialLayer *val)
        { mSpecialLayer = val; }

        void setTempLayer(SpecialLayer *val)
        { mTempLayer = val; }

        int getWidth() const
        { return mWidth; }

        int getHeight() const
        { return mHeight; }

//        void setTileInfo(int x, int y, int width, int cnt);

//        void getTileInfo(int x, int y, int &width, int &cnt) const;

        void optionChanged(const std::string &value);

        int getTileDrawWidth(Image *img, int endX, int &width) const;

//        void initTileInfo();

    private:
        int mX, mY;
        int mWidth, mHeight;
        bool mIsFringeLayer;    /**< Whether the actors are drawn. */
        bool mHighlightAttackRange;
        Image **mTiles;
//        int *mTilesWidth;
//        int *mTilesCount;
        SpecialLayer *mSpecialLayer;
        SpecialLayer *mTempLayer;
        typedef std::vector<MapRowVertexes*> MapRows;
        MapRows mTempRows;
};

class SpecialLayer
{
    public:
        friend class Map;
        friend class MapLayer;

        SpecialLayer(int width, int height, bool drawSprites = false);

        ~SpecialLayer();

        void draw(Graphics *graphics, int startX, int startY,
                  int endX, int endY, int scrollX, int scrollY);

        MapItem* getTile(int x, int y) const;

        void setTile(int x, int y, MapItem* item);

        void setTile(int x, int y, int type);

        void addRoad(Path road);

        void clean();

        void itemDraw(Graphics *graphics, int x, int y,
                      int scrollX, int scrollY);

    private:
        int mWidth, mHeight;
        bool mDrawSprites;
        MapItem **mTiles;
};

class MapItem
{
    public:
        friend class Map;
        friend class MapLayer;

        enum ItemType
        {
            EMPTY = 0,
            HOME = 1,
            ROAD = 2,
            CROSS = 3,
            ARROW_UP = 4,
            ARROW_DOWN = 5,
            ARROW_LEFT = 6,
            ARROW_RIGHT = 7,
            PORTAL = 8,
            MUSIC = 9,
            ATTACK = 10,
            PRIORITY = 11,
            IGNORE_ = 12,
            PICKUP = 13,
            NOPICKUP = 14,
            SEPARATOR = 15
        };

        MapItem();

        MapItem(int type);

        MapItem(int type, std::string comment);

        MapItem(int type, std::string comment, int x, int y);

        ~MapItem();

        int getType() const
        { return mType; }

        void setType(int type);

        void setPos(int x, int y);

        int getX() const
        { return mX; }

        int getY() const
        { return mY; }

        std::string &getComment()
        { return mComment; }

        void setComment(std::string comment)
        { mComment = comment; }

        std::string &getName()
        { return mName; }

        void setName(std::string name)
        { mName = name; }

        void draw(Graphics *graphics, int x, int y, int dx, int dy);

    private:
        int mType;
        Image *mImage;
        std::string mComment;
        std::string mName;
        int mX;
        int mY;
};

class ObjectsLayer
{
    public:
        ObjectsLayer(unsigned width, unsigned height);
        ~ObjectsLayer();

        void addObject(std::string name, int type, unsigned x, unsigned y,
                       unsigned dx, unsigned dy);

        MapObjectList *getAt(unsigned x, unsigned y);
    private:
        MapObjectList **mTiles;
        unsigned mWidth;
        unsigned mHeight;
};

#endif
