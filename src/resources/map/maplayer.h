/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "resources/memorycounter.h"

#include "being/actor.h"

#include "enums/resources/map/maptype.h"

#include "resources/map/tileinfo.h"

#include <vector>

class Image;
class MapRowVertexes;
class SpecialLayer;

struct MetaTile;

/**
 * A map layer. Stores a grid of tiles and their offset, and implements layer
 * rendering.
 */
class MapLayer final: public MemoryCounter, public ConfigListener
{
    public:
        friend class Map;

        /**
         * Constructor, taking layer origin, size and whether this layer is the
         * fringe layer. The fringe layer is the layer that draws the actors.
         * There can be only one fringe layer per map.
         */
        MapLayer(const std::string &name,
                 const int x,
                 const int y,
                 const int width,
                 const int height,
                 const bool isFringeLayer,
                 const int mask,
                 const int tileCondition);

        A_DELETE_COPY(MapLayer)

        /**
         * Destructor.
         */
        ~MapLayer();

        /**
         * Set tile image, with x and y in layer coordinates.
         */
        void setTile(const int x,
                     const int y,
                     Image *restrict const img) restrict;

        /**
         * Set tile image with x + y * width already known.
         */
        void setTile(const int index,
                     Image *restrict const img) restrict
        { mTiles[index].image = img; }

        /**
         * Draws this layer to the given graphics context. The coordinates are
         * expected to be in map range and will be translated to local layer
         * coordinates and clipped to the layer's dimensions.
         *
         * The given actors are only drawn when this layer is the fringe
         * layer.
         */
        void draw(Graphics *restrict const graphics,
                  int startX,
                  int startY,
                  int endX,
                  int endY,
                  const int scrollX,
                  const int scrollY) const restrict A_NONNULL(2);

        void drawSDL(Graphics *restrict const graphics) const restrict2
                     A_NONNULL(2);

#ifdef USE_OPENGL
        void drawOGL(Graphics *restrict const graphics) const restrict2
                     A_NONNULL(2);

        void updateOGL(Graphics *restrict const graphics,
                       int startX,
                       int startY,
                       int endX,
                       int endY,
                       const int scrollX,
                       const int scrollY) restrict2 A_NONNULL(2);
#endif  // USE_OPENGL

        void updateSDL(const Graphics *restrict const graphics,
                       int startX,
                       int startY,
                       int endX,
                       int endY,
                       const int scrollX,
                       const int scrollY) restrict2 A_NONNULL(2);

        void drawFringe(Graphics *restrict const graphics,
                        int startX,
                        int startY,
                        int endX,
                        int endY,
                        const int scrollX,
                        const int scrollY,
                        const Actors &actors) const restrict A_NONNULL(2);

        bool isFringeLayer() const restrict noexcept2 A_WARN_UNUSED
        { return mIsFringeLayer; }

        void setSpecialLayer(const SpecialLayer *restrict const val) restrict
        { mSpecialLayer = val; }

        void setTempLayer(const SpecialLayer *restrict const val) restrict
        { mTempLayer = val; }

        int getWidth() const restrict noexcept2 A_WARN_UNUSED
        { return mWidth; }

        int getHeight() const restrict noexcept2 A_WARN_UNUSED
        { return mHeight; }

        void optionChanged(const std::string &restrict value)
                           restrict override final;

        void setDrawLayerFlags(const MapTypeT &restrict n) restrict;

        void setActorsFix(const int y) restrict
        { mActorsFix = y; }

        int calcMemoryLocal() const override final;

        int calcMemoryChilds(const int level) const override final;

        std::string getCounterName() const override final
        { return mName; }

    protected:
        static int getTileDrawWidth(const TileInfo *restrict img,
                                    const int endX,
                                    int &width) A_WARN_UNUSED A_NONNULL(1);

        void updateConditionTiles(MetaTile *restrict const metaTiles,
                                  const int width,
                                  const int height) restrict A_NONNULL(2);

    private:
        const int mX;
        const int mY;
        const int mPixelX;
        const int mPixelY;
        const int mWidth;
        const int mHeight;
        TileInfo *restrict const mTiles;
        MapTypeT mDrawLayerFlags;
        const SpecialLayer *restrict mSpecialLayer;
        const SpecialLayer *restrict mTempLayer;
        const std::string mName;
        typedef std::vector<MapRowVertexes*> MapRows;
        MapRows mTempRows;
        int mMask;
        int mTileCondition;
        int mActorsFix;
        const bool mIsFringeLayer;    /**< Whether the actors are drawn. */
        bool mHighlightAttackRange;
        bool mSpecialFlag;
};

#endif  // RESOURCES_MAP_MAPLAYER_H
