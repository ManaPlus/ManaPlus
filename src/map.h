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

#ifndef MAP_H
#define MAP_H

#include "actor.h"
#include "configlistener.h"
#include "position.h"
#include "properties.h"

#include <string>
#include <vector>

class Animation;
class AmbientLayer;
class MapLayer;
class Particle;
class SimpleAnimation;
class Tileset;
class SpecialLayer;
class MapItem;
class ObjectsLayer;

typedef std::vector<Tileset*> Tilesets;
typedef std::vector<MapLayer*> Layers;
typedef Layers::const_iterator LayersCIter;

typedef std::vector<std::pair<MapLayer*, int> > TilePairVector;
typedef TilePairVector::const_iterator TilePairVectorCIter;

typedef std::vector<AmbientLayer*> AmbientLayerVector;
typedef AmbientLayerVector::const_iterator AmbientLayerVectorCIter;

/**
 * A meta tile stores additional information about a location on a tile map.
 * This is information that doesn't need to be repeated for each tile in each
 * layer of the map.
 */
struct MetaTile
{
    /**
     * Constructor.
     */
    MetaTile() : Fcost(0), Gcost(0), Hcost(0), whichList(0),
            parentX(0), parentY(0), blockmask(0)
    {}

    // Pathfinding members
    int Fcost;               /**< Estimation of total path cost */
    int Gcost;               /**< Cost from start to this location */
    int Hcost;               /**< Estimated cost to goal */
    unsigned whichList;      /**< No list, open list or closed list */
    int parentX;             /**< X coordinate of parent tile */
    int parentY;             /**< Y coordinate of parent tile */
    unsigned char blockmask; /**< Blocking properties of this tile */
};

/**
 * Animation cycle of a tile image which changes the map accordingly.
 */
class TileAnimation
{
    public:
        TileAnimation(Animation *const ani);

        ~TileAnimation();

        bool update(const int ticks = 1);

        void addAffectedTile(MapLayer *const layer, const int index)
        { mAffected.push_back(std::make_pair(layer, index)); }

    private:
        TilePairVector mAffected;
        SimpleAnimation *mAnimation;
        Image *mLastImage;
};

typedef std::map<int, TileAnimation*> TileAnimationMap;
typedef TileAnimationMap::const_iterator TileAnimationMapCIter;

/**
 * A tile map.
 */
class Map : public Properties, public ConfigListener
{
    public:
        enum BlockType
        {
            BLOCKTYPE_NONE = -1,
            BLOCKTYPE_WALL,
            BLOCKTYPE_CHARACTER,
            BLOCKTYPE_MONSTER,
            BLOCKTYPE_AIR,
            BLOCKTYPE_WATER,
            BLOCKTYPE_GROUND,
            BLOCKTYPE_GROUNDTOP,
            NB_BLOCKTYPES
        };

        enum CollisionTypes
        {
            COLLISION_EMPTY = 0,        // no collision
            COLLISION_WALL = 1,         // full collison
            COLLISION_AIR = 2,          // air units can walk
            COLLISION_WATER = 3,        // water units can walk
            COLLISION_GROUNDTOP = 4,    // no collision (chair, bed, etc)
            COLLISION_MAX = 5           // count index
        };

        enum BlockMask
        {
            BLOCKMASK_WALL      = 0x80, // 1000 0000
            BLOCKMASK_CHARACTER = 0x01, // 0000 0001
            BLOCKMASK_MONSTER   = 0x02, // 0000 0010
            BLOCKMASK_AIR       = 0x04, // 0000 0100
            BLOCKMASK_WATER     = 0x08, // 0000 1000
            BLOCKMASK_GROUND    = 0x16, // 0001 0000
            BLOCKMASK_GROUNDTOP = 0x32  // 0010 0000
        };

        enum DebugType
        {
            MAP_NORMAL  = 0,
            MAP_DEBUG   = 1,
            MAP_SPECIAL = 2,
            MAP_SPECIAL2 = 3,
            MAP_SPECIAL3 = 4,
            MAP_BLACKWHITE = 5
        };

        /**
         * Constructor, taking map and tile size as parameters.
         */
        Map(const int width, const int height,
            const int tileWidth, const int tileHeight);

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
        void draw(Graphics *graphics, int scrollX, int scrollY);

        /**
         * Visualizes collision layer for debugging
         */
        void drawCollision(Graphics *const graphics,
                           const int scrollX, const int scrollY,
                           const int debugFlags) const;

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
        Tileset *getTilesetWithGid(const int gid) const;

        /**
         * Get tile reference.
         */
        MetaTile *getMetaTile(const int x, const int y) const;

        /**
         * Marks a tile as occupied.
         */
        void blockTile(const int x, const int y, const BlockType type);

        /**
         * Gets walkability for a tile with a blocking bitmask. When called
         * without walkmask, only blocks against colliding tiles.
         */
        bool getWalk(const int x, const int y,
                     const unsigned char walkmask = BLOCKMASK_WALL
                     | BLOCKMASK_AIR | BLOCKMASK_WATER) const;

        void setWalk(const int x, const int y, const bool walkable);

        /**
         * Tells whether a tile is occupied by a being.
         */
//        bool occupied(const int x, const int y) const;

        /**
         * Returns the width of this map in tiles.
         */
        int getWidth() const
        { return mWidth; }

        /**
         * Returns the height of this map in tiles.
         */
        int getHeight() const
        { return mHeight; }

        /**
         * Returns the tile width of this map.
         */
        int getTileWidth() const
        { return mTileWidth; }

        /**
         * Returns the tile height used by this map.
         */
        int getTileHeight() const
        { return mTileHeight; }

        const std::string getMusicFile() const;

        const std::string getName() const;

        /**
         * Gives the map id based on filepath (ex: 009-1)
         */
        const std::string getFilename() const;

        /**
         * Check the current position against surrounding blocking tiles, and
         * correct the position offset within tile when needed.
         */
        Position checkNodeOffsets(int radius, const unsigned char walkMask,
                                  const Position &position) const;

        Position checkNodeOffsets(const int radius,
                                  const unsigned char walkMask,
                                  const int x, const int y) const
        { return checkNodeOffsets(radius, walkMask, Position(x, y)); }

        /**
         * Find a pixel path from one location to the next.
         */
        Path findPixelPath(const int startPixelX, const int startPixelY,
                           const int destPixelX, const int destPixelY,
                           const int radius, const unsigned char walkmask,
                           const int maxCost = 20);

        /**
         * Find a path from one location to the next.
         */
        Path findPath(const int startX, const int startY,
                      const int destX, const int destY,
                      const unsigned char walkmask, const int maxCost = 20);

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

        void setDebugFlags(const int n)
        { mDebugFlags = n; }

        int getDebugFlags() const
        { return mDebugFlags; }

        void addExtraLayer();

        void saveExtraLayer() const;

        SpecialLayer *getTempLayer() const
        { return mTempLayer; }

        SpecialLayer *getSpecialLayer() const
        { return mSpecialLayer; }

        void setHasWarps(const bool n)
        { mHasWarps = n; }

        bool getHasWarps() const
        { return mHasWarps; }

        std::string getUserMapDirectory() const;

        void addPortal(const std::string &name, const int type,
                       const int x, const int y, const int dx, const int dy);

        void addRange(const std::string &name, const int type,
                      const int x, const int y, const int dx, const int dy);

        void addPortalTile(const std::string &name, const int type,
                           const int x, const int y);

        void updatePortalTile(const std::string &name, const int type,
                              const int x, const int y,
                              const bool addNew = true);

        std::vector<MapItem*> &getPortals()
        { return mMapPortals; }

        /**
         * Gets the tile animation for a specific gid
         */
        TileAnimation *getAnimationForGid(const int gid) const;

        void optionChanged(const std::string &value);

        MapItem *findPortalXY(const int x, const int y) const;

        int getActorsCount() const
        { return static_cast<int>(mActors.size()); }

        void setPvpMode(const int mode);

        ObjectsLayer* getObjectsLayer() const
        { return mObjects; }

        std::string getObjectData(const unsigned x, const unsigned y,
                                  const int type) const;

        void indexTilesets();

        void clearIndexedTilesets();

        void setActorsFix(const int x, const int y)
        { mActorFixX = x; mActorFixY = y; }

        int getVersion() const
        { return mVersion; }

        void setVersion(const int n)
        { mVersion = n; }

        void reduce();

        void redrawMap();

        bool empty() const
        { return mLayers.empty(); }

        void setCustom(const bool b)
        { mCustom = b; }

        bool isCustom() const
        { return mCustom; }

        std::map<int, TileAnimation*> &getTileAnimations()
        { return mTileAnimations; }

    protected:
        friend class Actor;
        friend class Minimap;

        /**
         * Adds an actor to the map.
         */
        Actors::iterator addActor(Actor *actor);

        /**
         * Removes an actor from the map.
         */
        void removeActor(const Actors::iterator iterator);

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
        bool contains(const int x, const int y) const;

        /**
         * Blockmasks for different entities
         */
        unsigned *mOccupation[NB_BLOCKTYPES];

        int mWidth, mHeight;
        int mTileWidth, mTileHeight;
        int mMaxTileHeight;
        MetaTile *mMetaTiles;
        Layers mLayers;
        Tilesets mTilesets;
        Actors mActors;
        bool mHasWarps;

        // debug flags
        int mDebugFlags;

        // Pathfinding members
        unsigned mOnClosedList, mOnOpenList;

        // Overlay data
        AmbientLayerVector mBackgrounds;
        AmbientLayerVector mForegrounds;
        float mLastAScrollX;
        float mLastAScrollY;
//        bool mSpritesUpdated;

        // Particle effect data
        struct ParticleEffectData
        {
            std::string file;
            int x;
            int y;
            int w;
            int h;
        };
        std::vector<ParticleEffectData> particleEffects;

        std::vector<MapItem*> mMapPortals;

        std::map<int, TileAnimation*> mTileAnimations;

        int mOverlayDetail;
        float mOpacity;
        int mOpenGL;
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
        bool mRedrawMap;
        bool mBeingOpacity;
        bool mCustom;
};

#endif
