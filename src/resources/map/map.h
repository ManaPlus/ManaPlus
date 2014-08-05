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

#ifndef RESOURCES_MAP_MAP_H
#define RESOURCES_MAP_MAP_H

#include "position.h"

#include "being/actor.h"

#include "resources/map/blockmask.h"
#include "resources/map/blocktype.h"
#include "resources/map/maptype.h"
#include "resources/map/properties.h"

#include "listeners/configlistener.h"

#include "render/rendertype.h"

#include <string>
#include <vector>

class AmbientLayer;
class MapHeights;
class MapItem;
class MapLayer;
class ObjectsLayer;
class Particle;
class Resource;
class SpecialLayer;
class Tileset;
class TileAnimation;
class WalkLayer;

struct MetaTile;

typedef std::vector<Tileset*> Tilesets;
typedef std::vector<MapLayer*> Layers;
typedef Layers::const_iterator LayersCIter;

typedef std::vector<AmbientLayer*> AmbientLayerVector;
typedef AmbientLayerVector::const_iterator AmbientLayerVectorCIter;
typedef AmbientLayerVector::iterator AmbientLayerVectorIter;

/**
 * A tile map.
 */
class Map final : public Properties, public ConfigListener
{
    public:
        enum CollisionTypes
        {
            COLLISION_EMPTY = 0,        // no collision
            COLLISION_WALL = 1,         // full collison
            COLLISION_AIR = 2,          // air units can walk
            COLLISION_WATER = 3,        // water units can walk
            COLLISION_GROUNDTOP = 4,    // no collision (chair, bed, etc)
            COLLISION_MAX = 5           // count index
        };

        /**
         * Constructor, taking map and tile size as parameters.
         */
        Map(const int width, const int height,
            const int tileWidth, const int tileHeight);

        A_DELETE_COPY(Map)

        /**
         * Destructor.
         */
        ~Map();

        /**
         * Initialize ambient layers. Has to be called after all the properties
         * are set.
         */
        void initializeAmbientLayers();

        /**
         * Updates animations. Called as needed.
         */
        void update(const int ticks = 1);

        /**
         * Draws the map to the given graphics output. This method draws all
         * layers, actors and overlay effects.
         *
         * TODO: For efficiency reasons, this method could take into account
         * the clipping rectangle set on the Graphics object. However,
         * currently the map is always drawn full-screen.
         */
        void draw(Graphics *const graphics, int scrollX, int scrollY);

        /**
         * Visualizes collision layer for debugging
         */
        void drawCollision(Graphics *const graphics,
                           const int scrollX, const int scrollY,
                           const MapType::MapType drawFlags) const;

        /**
         * Adds a layer to this map. The map takes ownership of the layer.
         */
        void addLayer(MapLayer *const layer);

        /**
         * Adds a tileset to this map. The map takes ownership of the tileset.
         */
        void addTileset(Tileset *const tileset);

        /**
         * Finds the tile set that a tile with the given global id is part of.
         */
        const Tileset *getTilesetWithGid(const int gid) const A_WARN_UNUSED;

        /**
         * Get tile reference.
         */
        const MetaTile *getMetaTile(const int x,
                                    const int y) const A_WARN_UNUSED;

        /**
         * Marks a tile as occupied.
         */
        void blockTile(const int x, const int y,
                       const BlockType::BlockType type);

        /**
         * Gets walkability for a tile with a blocking bitmask. When called
         * without walkmask, only blocks against colliding tiles.
         */
        bool getWalk(const int x, const int y,
                     const unsigned char blockWalkMask = BlockMask::WALL
                     | BlockMask::AIR | BlockMask::WATER) const A_WARN_UNUSED;

        void setWalk(const int x, const int y, const bool walkable);

        unsigned char getBlockMask(const int x, const int y) const;

        /**
         * Returns the width of this map in tiles.
         */
        int getWidth() const A_WARN_UNUSED
        { return mWidth; }

        /**
         * Returns the height of this map in tiles.
         */
        int getHeight() const A_WARN_UNUSED
        { return mHeight; }

        /**
         * Returns the tile width of this map.
         */
        int getTileWidth() const A_WARN_UNUSED
        { return mTileWidth; }

        /**
         * Returns the tile height used by this map.
         */
        int getTileHeight() const A_WARN_UNUSED
        { return mTileHeight; }

        const std::string getMusicFile() const A_WARN_UNUSED;

        void setMusicFile(const std::string &file);

        const std::string getName() const A_WARN_UNUSED;

        /**
         * Gives the map id based on filepath (ex: 009-1)
         */
        const std::string getFilename() const A_WARN_UNUSED;

        /**
         * Find a path from one location to the next.
         */
        Path findPath(const int startX, const int startY,
                      const int destX, const int destY,
                      const unsigned char blockWalkmask,
                      const int maxCost = 20) A_WARN_UNUSED;

        /**
         * Adds a particle effect
         */
        void addParticleEffect(const std::string &effectFile,
                               const int x, const int y,
                               const int w = 0, const int h = 0);

        /**
         * Initializes all added particle effects
         */
        void initializeParticleEffects(Particle *const particleEngine);

        /**
         * Adds a tile animation to the map
         */
        void addAnimation(const int gid, TileAnimation *const animation)
        { mTileAnimations[gid] = animation; }

        void setDrawLayersFlags(const MapType::MapType &n)
        { mDrawLayersFlags = n; }

        MapType::MapType getDrawLayersFlags() const A_WARN_UNUSED
        { return mDrawLayersFlags; }

        void addExtraLayer();

        void saveExtraLayer() const;

        SpecialLayer *getTempLayer() const A_WARN_UNUSED
        { return mTempLayer; }

        SpecialLayer *getSpecialLayer() const A_WARN_UNUSED
        { return mSpecialLayer; }

        void setHasWarps(const bool n)
        { mHasWarps = n; }

        bool getHasWarps() const A_WARN_UNUSED
        { return mHasWarps; }

        std::string getUserMapDirectory() const A_WARN_UNUSED;

        void addPortal(const std::string &name, const int type,
                       const int x, const int y, const int dx, const int dy);

        void addRange(const std::string &name, const int type,
                      const int x, const int y, const int dx, const int dy);

        void addPortalTile(const std::string &name, const int type,
                           const int x, const int y);

        void updatePortalTile(const std::string &name, const int type,
                              const int x, const int y,
                              const bool addNew = true);

        const std::vector<MapItem*> &getPortals() const A_WARN_UNUSED
        { return mMapPortals; }

        /**
         * Gets the tile animation for a specific gid
         */
        const TileAnimation *getAnimationForGid(const int gid)
                                                const A_WARN_UNUSED;

        void optionChanged(const std::string &value) override final;

        MapItem *findPortalXY(const int x, const int y) const A_WARN_UNUSED;

        int getActorsCount() const A_WARN_UNUSED
        { return static_cast<int>(mActors.size()); }

        void setPvpMode(const int mode);

        const ObjectsLayer* getObjectsLayer() const A_WARN_UNUSED
        { return mObjects; }

        std::string getObjectData(const unsigned x, const unsigned y,
                                  const int type) const A_WARN_UNUSED;

        void indexTilesets();

        void clearIndexedTilesets();

        void setActorsFix(const int x, const int y)
        { mActorFixX = x; mActorFixY = y; }

        int getVersion() const A_WARN_UNUSED
        { return mVersion; }

        void setVersion(const int n)
        { mVersion = n; }

        void reduce();

        void redrawMap();

        bool empty() const A_WARN_UNUSED
        { return mLayers.empty(); }

        void setCustom(const bool b)
        { mCustom = b; }

        bool isCustom() const A_WARN_UNUSED
        { return mCustom; }

        const std::map<int, TileAnimation*> &getTileAnimations()
                                            const A_WARN_UNUSED
        { return mTileAnimations; }

        void setAtlas(Resource *const atlas)
        { mAtlas = atlas; }

        const MetaTile *getMetaTiles() const
        { return mMetaTiles; }

        WalkLayer *getWalkLayer()
        { return mWalkLayer; }

        void setWalkLayer(WalkLayer *const layer)
        { mWalkLayer = layer; }

        void addHeights(MapHeights *const heights);

        uint8_t getHeightOffset(const int x, const int y) const;

        void setMask(const int mask);

    protected:
        friend class Actor;
        friend class Minimap;

        /**
         * Adds an actor to the map.
         */
        Actors::iterator addActor(Actor *const actor);

        /**
         * Removes an actor from the map.
         */
        void removeActor(const Actors::iterator &iterator);

    private:
        enum LayerType
        {
            FOREGROUND_LAYERS = 0,
            BACKGROUND_LAYERS
        };

        /**
         * Updates scrolling of ambient layers. Has to be called each game tick.
         */
        void updateAmbientLayers(const float scrollX, const float scrollY);

        /**
         * Draws the foreground or background layers to the given graphics output.
         */
        void drawAmbientLayers(Graphics *const graphics, const LayerType type,
                               const int detail);

        /**
         * Tells whether the given coordinates fall within the map boundaries.
         */
        bool contains(const int x, const int y) const A_WARN_UNUSED;

        /**
         * Blockmasks for different entities
         */
        unsigned *mOccupation[BlockType::NB_BLOCKTYPES];

        int mWidth;
        int mHeight;
        int mTileWidth, mTileHeight;
        int mMaxTileHeight;
        MetaTile *mMetaTiles;
        WalkLayer *mWalkLayer;
        Layers mLayers;
        Tilesets mTilesets;
        Actors mActors;
        bool mHasWarps;

        // draw flags
        MapType::MapType mDrawLayersFlags;

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
            ParticleEffectData() :
                file(),
                x(0),
                y(0),
                w(0),
                h(0)
            {
            }

            std::string file;
            int x;
            int y;
            int w;
            int h;
        };
        std::vector<ParticleEffectData> mParticleEffects;

        std::vector<MapItem*> mMapPortals;

        std::map<int, TileAnimation*> mTileAnimations;

        int mOverlayDetail;
        float mOpacity;
        RenderType mOpenGL;
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
        Resource *mAtlas;
        MapHeights *mHeights;
        bool mRedrawMap;
        bool mBeingOpacity;
        bool mCustom;
};

#endif  // RESOURCES_MAP_MAP_H
