/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifndef RESOURCES_MAP_MAP_H
#define RESOURCES_MAP_MAP_H

#include "position.h"

#include "being/actor.h"

#include "enums/render/rendertype.h"

#include "enums/resources/map/blocktype.h"
#include "enums/resources/map/maplayerposition.h"
#include "enums/resources/map/maptype.h"

#include "listeners/configlistener.h"

#include "utils/vector.h"

#include "resources/memorycounter.h"

#include "resources/map/properties.h"

class AmbientLayer;
#ifdef USE_OPENGL
class AtlasResource;
#endif  // USE_OPENGL

class MapHeights;
class MapItem;
class MapLayer;
class ObjectsLayer;
class SpecialLayer;
class Tileset;
class TileAnimation;
class WalkLayer;

struct MetaTile;

typedef STD_VECTOR<Tileset*> Tilesets;
typedef STD_VECTOR<MapLayer*> Layers;
typedef Layers::const_iterator LayersCIter;

typedef STD_VECTOR<AmbientLayer*> AmbientLayerVector;
typedef AmbientLayerVector::const_iterator AmbientLayerVectorCIter;
typedef AmbientLayerVector::iterator AmbientLayerVectorIter;

/**
 * A tile map.
 */
class Map final : public Properties,
                  public ConfigListener,
                  public MemoryCounter
{
    public:
        /**
         * Constructor, taking map and tile size as parameters.
         */
        Map(const std::string &name,
            const int width,
            const int height,
            const int tileWidth,
            const int tileHeight);

        A_DELETE_COPY(Map)

        /**
         * Destructor.
         */
        ~Map() override final;

        /**
         * Initialize ambient layers. Has to be called after all the properties
         * are set.
         */
        void initializeAmbientLayers() restrict2;

        /**
         * Updates animations. Called as needed.
         */
        void update(const int ticks) restrict2;

        /**
         * Draws the map to the given graphics output. This method draws all
         * layers, actors and overlay effects.
         */
        void draw(Graphics *restrict const graphics,
                  int scrollX,
                  int scrollY) restrict2 A_NONNULL(2);

        /**
         * Visualizes collision layer for debugging
         */
        void drawCollision(Graphics *restrict const graphics,
                           const int scrollX,
                           const int scrollY,
                           const MapTypeT drawFlags) const
                           restrict2 A_NONNULL(2);

        /**
         * Adds a layer to this map. The map takes ownership of the layer.
         */
        void addLayer(MapLayer *const layer) restrict2 A_NONNULL(2);

        /**
         * Adds a tileset to this map. The map takes ownership of the tileset.
         */
        void addTileset(Tileset *const tileset) restrict2 A_NONNULL(2);

        /**
         * Finds the tile set that a tile with the given global id is part of.
         */
        const Tileset *getTilesetWithGid(const int gid) const
                                         restrict2 A_WARN_UNUSED;

        /**
         * Get tile reference.
         */
        const MetaTile *getMetaTile(const int x,
                                    const int y) const
                                    restrict2 A_WARN_UNUSED;

        void addBlockMask(const int x, const int y,
                          const BlockTypeT type) restrict2;

        void setBlockMask(const int x, const int y,
                          const BlockTypeT type) restrict2;

        /**
         * Gets walkability for a tile with a blocking bitmask. When called
         * without walkmask, only blocks against colliding tiles.
         */
        bool getWalk(const int x, const int y,
                     const unsigned char blockWalkMask) const
                     restrict2 A_WARN_UNUSED;

        void setWalk(const int x, const int y) restrict2;

        unsigned char getBlockMask(const int x,
                                   const int y) const restrict2;

        /**
         * Returns the width of this map in tiles.
         */
        int getWidth() const restrict2 noexcept2 A_WARN_UNUSED
        { return mWidth; }

        /**
         * Returns the height of this map in tiles.
         */
        int getHeight() const restrict2 noexcept2 A_WARN_UNUSED
        { return mHeight; }

        /**
         * Returns the tile width of this map.
         */
        int getTileWidth() const restrict2 noexcept2 A_WARN_UNUSED
        { return mTileWidth; }

        /**
         * Returns the tile height used by this map.
         */
        int getTileHeight() const restrict2 noexcept2 A_WARN_UNUSED
        { return mTileHeight; }

        const std::string getMusicFile() const restrict2 A_WARN_UNUSED;

        void setMusicFile(const std::string &restrict file) restrict2;

        const std::string getName() const restrict2 A_WARN_UNUSED;

        /**
         * Gives the map id based on filepath (ex: 009-1)
         */
        const std::string getFilename() const restrict2 A_WARN_UNUSED;

        const std::string getGatName() const restrict2 A_WARN_UNUSED;

        /**
         * Find a path from one location to the next.
         */
        Path findPath(const int startX, const int startY,
                      const int destX, const int destY,
                      const unsigned char blockWalkmask,
                      const int maxCost) restrict2 A_WARN_UNUSED;

        /**
         * Adds a particle effect
         */
        void addParticleEffect(const std::string &restrict effectFile,
                               const int x,
                               const int y,
                               const int w,
                               const int h) restrict2;

        /**
         * Initializes all added particle effects
         */
        void initializeParticleEffects() const restrict2;

        /**
         * Adds a tile animation to the map
         */
        void addAnimation(const int gid,
                          TileAnimation *restrict const animation) restrict2
                          A_NONNULL(3);

        void setDrawLayersFlags(const MapTypeT &restrict n) restrict2;

        MapTypeT getDrawLayersFlags() const restrict2 A_WARN_UNUSED
        { return mDrawLayersFlags; }

        void addExtraLayer() restrict2;

        void saveExtraLayer() const restrict2;

        SpecialLayer *getTempLayer() const restrict2 noexcept2 A_WARN_UNUSED
        { return mTempLayer; }

        SpecialLayer *getSpecialLayer() const restrict2 noexcept2 A_WARN_UNUSED
        { return mSpecialLayer; }

        void setHasWarps(const bool n) restrict2 noexcept2
        { mHasWarps = n; }

        bool getHasWarps() const restrict2 noexcept2 A_WARN_UNUSED
        { return mHasWarps; }

        std::string getUserMapDirectory() const restrict2 A_WARN_UNUSED;

        void addPortal(const std::string &restrict name,
                       const int type,
                       const int x, const int y,
                       const int dx, const int dy) restrict2;

        void addRange(const std::string &restrict name,
                      const int type,
                      const int x, const int y,
                      const int dx, const int dy) restrict2;

        void addPortalTile(const std::string &restrict name,
                           const int type,
                           const int x, const int y) restrict2;

        void updatePortalTile(const std::string &restrict name,
                              const int type,
                              const int x, const int y,
                              const bool addNew) restrict2;

        const STD_VECTOR<MapItem*> &getPortals() const restrict2 noexcept2
                                                A_WARN_UNUSED
        { return mMapPortals; }

        /**
         * Gets the tile animation for a specific gid
         */
        const TileAnimation *getAnimationForGid(const int gid)
                                                const restrict2 A_WARN_UNUSED;

        void optionChanged(const std::string &restrict value)
                           restrict2 override final;

        MapItem *findPortalXY(const int x,
                              const int y) const restrict2 A_WARN_UNUSED;

        int getActorsCount() const restrict2 A_WARN_UNUSED
        { return CAST_S32(mActors.size()); }

        void setPvpMode(const int mode) restrict2;

        int getPvpMode() const restrict2 noexcept2 A_WARN_UNUSED
        { return mPvp; }

        const ObjectsLayer* getObjectsLayer() const restrict2 noexcept2
                                            A_WARN_UNUSED
        { return mObjects; }

        std::string getObjectData(const unsigned x,
                                  const unsigned y,
                                  const int type) const
                                  restrict2 A_WARN_UNUSED;

        void indexTilesets() restrict2;

        void clearIndexedTilesets() restrict2;

        void setActorsFix(const int x, const int y) restrict2;

        int getVersion() const restrict2 noexcept2 A_WARN_UNUSED
        { return mVersion; }

        void setVersion(const int n) restrict2 noexcept2
        { mVersion = n; }

        void reduce() restrict2;

        void redrawMap() restrict2 noexcept2
        { mRedrawMap = true; }

        bool empty() const restrict2 A_WARN_UNUSED
        { return mLayers.empty(); }

        void setCustom(const bool b) restrict2 noexcept2
        { mCustom = b; }

        bool isCustom() const restrict2 noexcept2 A_WARN_UNUSED
        { return mCustom; }

        const std::map<int, TileAnimation*> &getTileAnimations() const
                                            restrict2 noexcept2 A_WARN_UNUSED
        { return mTileAnimations; }

#ifdef USE_OPENGL
        int getAtlasCount() const restrict2 A_WARN_UNUSED;

        void setAtlas(AtlasResource *restrict const atlas) restrict2 noexcept2
        { mAtlas = atlas; }

        bool haveAtlas() const
        { return mAtlas != nullptr; }
#endif  // USE_OPENGL

        const MetaTile *getMetaTiles() const restrict2 noexcept2
        { return mMetaTiles; }

        const WalkLayer *getWalkLayer() const restrict2 noexcept2
        { return mWalkLayer; }

        void setWalkLayer(WalkLayer *restrict const layer) restrict2 noexcept2
        { mWalkLayer = layer; }

        void addHeights(const MapHeights *restrict const heights) restrict2
                        A_NONNULL(2);

        uint8_t getHeightOffset(const int x, const int y) const restrict2;

        void setMask(const int mask) restrict2;

        void updateDrawLayersList() restrict2;

        bool isHeightsPresent() const restrict2 noexcept2
        { return mHeights != nullptr; }

        void updateConditionLayers() restrict2;

        void preCacheLayers() restrict2;

        int calcMemoryLocal() const override final;

        int calcMemoryChilds(const int level) const override final;

        std::string getCounterName() const override final
        { return mName; }

    protected:
        friend class Actor;
        friend class Minimap;

        /**
         * Adds an actor to the map.
         */
        Actors::iterator addActor(Actor *const actor) restrict2 A_NONNULL(2);

        /**
         * Removes an actor from the map.
         */
        void removeActor(const Actors::iterator &restrict iterator) restrict2;

    private:
        /**
         * Updates scrolling of ambient layers. Has to be called each game tick.
         */
        void updateAmbientLayers(const float scrollX,
                                 const float scrollY) restrict2;

        /**
         * Draws the foreground or background layers to the given graphics output.
         */
        void drawAmbientLayers(Graphics *restrict const graphics,
                               const MapLayerPositionT type,
                               const int detail) const restrict2 A_NONNULL(2);

        /**
         * Tells whether the given coordinates fall within the map boundaries.
         */
        bool contains(const int x,
                      const int y) const restrict2 A_WARN_UNUSED;

        const int mWidth;
        const int mHeight;
        const int mTileWidth;
        const int mTileHeight;
        int mMaxTileHeight;
        MetaTile *const mMetaTiles;
        WalkLayer *mWalkLayer;
        Layers mLayers;
        Layers mDrawUnderLayers;
        Layers mDrawOverLayers;
        Tilesets mTilesets;
        Actors mActors;
        bool mHasWarps;

        // draw flags
        MapTypeT mDrawLayersFlags;

        // Pathfinding members
        unsigned int mOnClosedList;
        unsigned int mOnOpenList;

        // Overlay data
        AmbientLayerVector mBackgrounds;
        AmbientLayerVector mForegrounds;
        float mLastAScrollX;
        float mLastAScrollY;

        // Particle effect data
        struct ParticleEffectData final
        {
            ParticleEffectData(const std::string &restrict file0,
                               const int x0,
                               const int y0,
                               const int w0,
                               const int h0) noexcept2 :
                file(file0),
                x(x0),
                y(y0),
                w(w0),
                h(h0)
            {
            }

            A_DEFAULT_COPY(ParticleEffectData)

            const2 std::string file;
            const2 int x;
            const2 int y;
            const2 int w;
            const2 int h;
        };
        STD_VECTOR<ParticleEffectData> mParticleEffects;

        STD_VECTOR<MapItem*> mMapPortals;

        std::map<int, TileAnimation*> mTileAnimations;

        std::string mName;
        int mOverlayDetail;
        float mOpacity;
        const RenderType mOpenGL;
        int mPvp;
        bool mTilesetsIndexed;
        Tileset** mIndexedTilesets;
        int mIndexedTilesetsSize;
        int mActorFixX;
        int mActorFixY;
        int mVersion;

        SpecialLayer *mSpecialLayer;
        SpecialLayer *mTempLayer;
        ObjectsLayer *mObjects;
        MapLayer *mFringeLayer;

        int mLastX;
        int mLastY;
        int mLastScrollX;
        int mLastScrollY;

        int mDrawX;
        int mDrawY;
        int mDrawScrollX;
        int mDrawScrollY;
        int mMask;
#ifdef USE_OPENGL
        AtlasResource *mAtlas;
#endif  // USE_OPENGL

        const MapHeights *mHeights;
        bool mRedrawMap;
        bool mBeingOpacity;
        bool mCachedDraw;
        bool mCustom;
        bool mDrawOnlyFringe;
};

#endif  // RESOURCES_MAP_MAP_H
