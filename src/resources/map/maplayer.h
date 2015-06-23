/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#ifndef RESOURCES_MAP_MAPLAYER_H
#define RESOURCES_MAP_MAPLAYER_H

#include "listeners/configlistener.h"

#include "being/actor.h"

#include "enums/resources/map/maptype.h"

#include <vector>

class Image;
class MapRowVertexes;
class SpecialLayer;

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
        MapLayer(const int x, const int y,
                 const int width, const int height,
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
                  int startX,
                  int startY,
                  int endX,
                  int endY,
                  const int scrollX,
                  const int scrollY) const A_NONNULL(2);

        void drawSDL(Graphics *const graphics) const A_NONNULL(2);

#ifdef USE_OPENGL
        void drawOGL(Graphics *const graphics) const A_NONNULL(2);

        void updateOGL(Graphics *const graphics,
                       int startX,
                       int startY,
                       int endX,
                       int endY,
                       const int scrollX,
                       const int scrollY) A_NONNULL(2);
#endif

        void updateSDL(const Graphics *const graphics,
                       int startX,
                       int startY,
                       int endX,
                       int endY,
                       const int scrollX,
                       const int scrollY) A_NONNULL(2);

        void drawFringe(Graphics *const graphics,
                        int startX,
                        int startY,
                        int endX,
                        int endY,
                        const int scrollX,
                        const int scrollY,
                        const Actors *const actors) const A_NONNULL(2);

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

        void setDrawLayerFlags(const MapTypeT &n);

        void setActorsFix(const int y)
        { mActorsFix = y; }

    protected:
        static int getTileDrawWidth(const Image *img,
                                    const int endX,
                                    int &width) A_WARN_UNUSED;

    private:
        const int mX;
        const int mY;
        const int mWidth;
        const int mHeight;
        Image **const mTiles;
        MapTypeT mDrawLayerFlags;
        const SpecialLayer *mSpecialLayer;
        const SpecialLayer *mTempLayer;
        typedef std::vector<MapRowVertexes*> MapRows;
        MapRows mTempRows;
        int mMask;
        int mActorsFix;
        const bool mIsFringeLayer;    /**< Whether the actors are drawn. */
        bool mHighlightAttackRange;
        bool mSpecialFlag;
};

#endif  // RESOURCES_MAP_MAPLAYER_H
