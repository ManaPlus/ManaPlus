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

#ifndef MAPLAYER_H
#define MAPLAYER_H

#include "position.h"
#include "main.h"

#include "listeners/configlistener.h"

#include "being/actor.h"

#include <string>
#include <vector>

class Image;
class MapItem;
class SpecialLayer;
class ImageVertexes;

typedef std::vector<ImageVertexes*> MepRowImages;

class MapRowVertexes final
{
    public:
        MapRowVertexes() :
            images()
        {
            images.reserve(30);
        }

        A_DELETE_COPY(MapRowVertexes)

        ~MapRowVertexes();

        MepRowImages images;
};

class MapObject final
{
    public:
        MapObject(const int type0, const std::string &data0) :
            type(type0), data(data0)
        {
        }

        int type;
        std::string data;
};

class MapObjectList final
{
    public:
        MapObjectList() :
            objects()
        {
        }

        A_DELETE_COPY(MapObjectList)

        std::vector<MapObject> objects;
};

/**
 * A map layer. Stores a grid of tiles and their offset, and implements layer
 * rendering.
 */
class MapLayer final: public ConfigListener
{
    public:
        enum Type
        {
            TILES = 0,
            COLLISION,
            HEIGHTS
        };

        friend class Map;

        /**
         * Constructor, taking layer origin, size and whether this layer is the
         * fringe layer. The fringe layer is the layer that draws the actors.
         * There can be only one fringe layer per map.
         */
        MapLayer(const int x, const int y, const int width, const int height,
                 const bool isFringeLayer, const int mask);

        A_DELETE_COPY(MapLayer)

        /**
         * Destructor.
         */
        ~MapLayer();

        /**
         * Set tile image, with x and y in layer coordinates.
         */
        void setTile(const int x, const int y, Image *const img);

        /**
         * Set tile image with x + y * width already known.
         */
        void setTile(const int index, Image *const img)
        { mTiles[index] = img; }

        /**
         * Draws this layer to the given graphics context. The coordinates are
         * expected to be in map range and will be translated to local layer
         * coordinates and clipped to the layer's dimensions.
         *
         * The given actors are only drawn when this layer is the fringe
         * layer.
         */
        void draw(Graphics *const graphics,
                  int startX, int startY, int endX, int endY,
                  const int scrollX, const int scrollY,
                  const int mDebugFlags) const;

        void drawSDL(Graphics *const graphics);

#ifdef USE_OPENGL
        void drawOGL(Graphics *const graphics);

        void updateOGL(Graphics *const graphics,
                       int startX, int startY,
                       int endX, int endY,
                       const int scrollX, const int scrollY,
                       const int mDebugFlags);
#endif

        void updateSDL(Graphics *const graphics,
                       int startX, int startY,
                       int endX, int endY,
                       const int scrollX, const int scrollY,
                       const int mDebugFlags);

        void drawFringe(Graphics *const graphics,
                        int startX, int startY,
                        int endX, int endY,
                        const int scrollX, const int scrollY,
                        const Actors *const actors,
                        const int mDebugFlags, const int yFix) const;

        bool isFringeLayer() const A_WARN_UNUSED
        { return mIsFringeLayer; }

        void setSpecialLayer(SpecialLayer *const val)
        { mSpecialLayer = val; }

        void setTempLayer(SpecialLayer *const val)
        { mTempLayer = val; }

        int getWidth() const A_WARN_UNUSED
        { return mWidth; }

        int getHeight() const A_WARN_UNUSED
        { return mHeight; }

        void optionChanged(const std::string &value) override final;

        static int getTileDrawWidth(const Image *img,
                                    const int endX,
                                    int &width) A_WARN_UNUSED;

    private:
        int mX;
        int mY;
        int mWidth;
        int mHeight;
        Image **mTiles;
        SpecialLayer *mSpecialLayer;
        SpecialLayer *mTempLayer;
        typedef std::vector<MapRowVertexes*> MapRows;
        MapRows mTempRows;
        int mMask;
        bool mIsFringeLayer;    /**< Whether the actors are drawn. */
        bool mHighlightAttackRange;
};

class SpecialLayer final
{
    public:
        friend class Map;
        friend class MapLayer;

        SpecialLayer(const int width, const int height);

        A_DELETE_COPY(SpecialLayer)

        ~SpecialLayer();

        void draw(Graphics *const graphics, int startX, int startY,
                  int endX, int endY,
                  const int scrollX, const int scrollY) const;

        MapItem* getTile(const int x, const int y) const A_WARN_UNUSED;

        void setTile(const int x, const int y, MapItem *const item);

        void setTile(const int x, const int y, const int type);

        void addRoad(const Path &road);

        void clean() const;

    private:
        int mWidth;
        int mHeight;
        MapItem **mTiles;
};

class MapItem final
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

        explicit MapItem(const int type);

        MapItem(const int type, std::string comment);

        MapItem(const int type, std::string comment, const int x, const int y);

        A_DELETE_COPY(MapItem)

        ~MapItem();

        int getType() const A_WARN_UNUSED
        { return mType; }

        void setType(const int type);

        void setPos(const int x, const int y);

        int getX() const A_WARN_UNUSED
        { return mX; }

        int getY() const A_WARN_UNUSED
        { return mY; }

        const std::string &getComment() const A_WARN_UNUSED
        { return mComment; }

        void setComment(std::string comment)
        { mComment = comment; }

        const std::string &getName() const A_WARN_UNUSED
        { return mName; }

        void setName(std::string name)
        { mName = name; }

        void draw(Graphics *const graphics, const int x, const int y,
                  const int dx, const int dy) const;

    private:
        Image *mImage;
        std::string mComment;
        std::string mName;
        int mType;
        int mX;
        int mY;
};

class ObjectsLayer final
{
    public:
        ObjectsLayer(const unsigned width, const unsigned height);

        A_DELETE_COPY(ObjectsLayer)

        ~ObjectsLayer();

        void addObject(std::string name, const int type,
                       const unsigned x, const unsigned y,
                       unsigned dx, unsigned dy);

        MapObjectList *getAt(const unsigned x,
                             const unsigned y) const A_WARN_UNUSED;
    private:
        MapObjectList **mTiles;
        unsigned mWidth;
        unsigned mHeight;
};

#endif  // MAPLAYER_H
