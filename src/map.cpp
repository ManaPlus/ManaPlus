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

#include "map.h"

#include "actorspritemanager.h"
#include "configuration.h"

#include "logger.h"
#include "maplayer.h"
#include "particle.h"
#include "simpleanimation.h"
#include "tileset.h"
#include "localplayer.h"

#include "resources/ambientlayer.h"
#include "resources/image.h"
#include "resources/resourcemanager.h"

#include "gui/widgets/chattab.h"

#include "utils/dtor.h"
#include "utils/mkdir.h"

#include <limits.h>
#include <physfs.h>

#include <sys/stat.h>

#include "debug.h"

/**
 * A location on a tile map. Used for pathfinding, open list.
 */
struct Location
{
    /**
     * Constructor.
     */
    Location(int px, int py, MetaTile *ptile):
        x(px), y(py), tile(ptile)
    {}

    /**
     * Comparison operator.
     */
    bool operator< (const Location &loc) const
    {
        return tile->Fcost > loc.tile->Fcost;
    }

    int x, y;
    MetaTile *tile;
};

class ActorFunctuator
{
    public:
        bool operator()(const Actor *a, const Actor *b) const
        {
            if (!a || !b)
                return false;
            return a->getSortPixelY() < b->getSortPixelY();
        }
} actorCompare;

TileAnimation::TileAnimation(Animation *ani):
    mLastImage(nullptr)
{
    mAnimation = new SimpleAnimation(ani);
}

TileAnimation::~TileAnimation()
{
    delete mAnimation;
    mAnimation = nullptr;
}

bool TileAnimation::update(int ticks)
{
    if (!mAnimation)
        return false;

    // update animation
    if (!mAnimation->update(ticks))
        return false;

    // exchange images
    Image *img = mAnimation->getCurrentImage();
    if (img != mLastImage)
    {
        for (std::vector<std::pair<MapLayer*, int> >::const_iterator i =
             mAffected.begin(); i != mAffected.end(); ++i)
        {
            if (i->first)
                i->first->setTile(i->second, img);
        }
        mLastImage = img;
    }
    return true;
}

Map::Map(int width, int height, int tileWidth, int tileHeight):
    mWidth(width), mHeight(height),
    mTileWidth(tileWidth), mTileHeight(tileHeight),
    mMaxTileHeight(height),
    mHasWarps(false),
    mDebugFlags(MAP_NORMAL),
    mOnClosedList(1), mOnOpenList(2),
    mLastAScrollX(0.0f), mLastAScrollY(0.0f),
    mOverlayDetail(config.getIntValue("OverlayDetail")),
    mOpacity(config.getFloatValue("guialpha")),
    mPvp(0),
    mTilesetsIndexed(false),
    mIndexedTilesets(nullptr),
    mIndexedTilesetsSize(0),
    mActorFixX(0),
    mActorFixY(0),
    mFringeLayer(nullptr),
    mLastX(-1),
    mLastY(-1),
    mLastScrollX(-1),
    mLastScrollY(-1),
    mDrawX(-1),
    mDrawY(-1),
    mDrawScrollX(-1),
    mDrawScrollY(-1),
    mRedrawMap(true),
    mBeingOpacity(false)
{
    const int size = mWidth * mHeight;

    mDebugFlags = 0;
    mMetaTiles = new MetaTile[size];
    for (int i = 0; i < NB_BLOCKTYPES; i++)
    {
        mOccupation[i] = new unsigned[size];
        memset(mOccupation[i], 0, size * sizeof(unsigned));
    }
    mSpecialLayer = new SpecialLayer(width, height);
    mTempLayer = new SpecialLayer(width, height, true);
    mObjects = new ObjectsLayer(width, height);

    config.addListener("OverlayDetail", this);
    config.addListener("guialpha", this);
    config.addListener("beingopacity", this);

    mOpacity = config.getFloatValue("guialpha");
    if (mOpacity != 1.0f)
        mBeingOpacity = config.getBoolValue("beingopacity");
    else
        mBeingOpacity = false;

#ifdef USE_OPENGL
    mOpenGL = config.getIntValue("opengl");
#else
    mOpenGL = 0;
#endif
}

Map::~Map()
{
    config.removeListeners(this);

    // delete metadata, layers, tilesets and overlays
    delete [] mMetaTiles;
    for (int i = 0; i < NB_BLOCKTYPES; i++)
        delete [] mOccupation[i];

    mFringeLayer = nullptr;
    delete_all(mLayers);
    delete_all(mTilesets);
    delete_all(mForegrounds);
    delete_all(mBackgrounds);
    delete_all(mTileAnimations);
    delete mSpecialLayer;
    mSpecialLayer = nullptr;
    delete mTempLayer;
    mTempLayer = nullptr;
    delete mObjects;
    mObjects = nullptr;
    delete_all(mMapPortals);
}

void Map::optionChanged(const std::string &value)
{
    if (value == "OverlayDetail")
    {
        mOverlayDetail = config.getIntValue("OverlayDetail");
    }
    else if (value == "guialpha")
    {
        mOpacity = config.getFloatValue("guialpha");
        if (mOpacity != 1.0f)
            mBeingOpacity = config.getBoolValue("beingopacity");
        else
            mBeingOpacity = false;
    }
    else if (value == "beingopacity")
    {
        if (mOpacity != 1.0f)
            mBeingOpacity = config.getBoolValue("beingopacity");
        else
            mBeingOpacity = false;
    }
}

void Map::initializeAmbientLayers()
{
    ResourceManager *resman = ResourceManager::getInstance();

    // search for "foreground*" or "overlay*" (old term) in map properties
    for (int i = 0; /* terminated by a break */; i++)
    {
        std::string name;
        if (hasProperty("foreground" + toString(i) + "image"))
            name = "foreground" + toString(i);
        else if (hasProperty("overlay" + toString(i) + "image"))
            name = "overlay" + toString(i);
        else
            break; // the FOR loop

        Image *img = resman->getImage(getProperty(name + "image"));
        const float speedX = getFloatProperty(name + "scrollX");
        const float speedY = getFloatProperty(name + "scrollY");
        const float parallax = getFloatProperty(name + "parallax");
        const bool keepRatio = getBoolProperty(name + "keepratio");

        if (img)
        {
            mForegrounds.push_back(
                new AmbientLayer(img, parallax, speedX, speedY, keepRatio));

            // The AmbientLayer takes control over the image.
            img->decRef();
        }
    }


    // search for "background*" in map properties
    for (int i = 0;
         hasProperty("background" + toString(i) + "image");
         i++)
    {
        const std::string name = "background" + toString(i);

        Image *img = resman->getImage(getProperty(name + "image"));
        const float speedX = getFloatProperty(name + "scrollX");
        const float speedY = getFloatProperty(name + "scrollY");
        const float parallax = getFloatProperty(name + "parallax");
        const bool keepRatio = getBoolProperty(name + "keepratio");

        if (img)
        {
            mBackgrounds.push_back(
                new AmbientLayer(img, parallax, speedX, speedY, keepRatio));

            // The AmbientLayer takes control over the image.
            img->decRef();
        }
    }
}

void Map::addLayer(MapLayer *layer)
{
    if (layer)
    {
        mLayers.push_back(layer);
        if (layer->isFringeLayer() && !mFringeLayer)
            mFringeLayer = layer;
    }
}

void Map::addTileset(Tileset *tileset)
{
    if (!tileset)
        return;

    mTilesets.push_back(tileset);

    if (tileset->getHeight() > mMaxTileHeight)
        mMaxTileHeight = tileset->getHeight();
}

void Map::update(int ticks)
{
    // Update animated tiles
    for (std::map<int, TileAnimation*>::const_iterator
         iAni = mTileAnimations.begin();
         iAni != mTileAnimations.end(); ++iAni)
    {
        if (iAni->second && iAni->second->update(ticks))
            mRedrawMap = true;
    }
}

void Map::draw(Graphics *graphics, int scrollX, int scrollY)
{
    if (!player_node)
        return;

    // Calculate range of tiles which are on-screen
    const int endPixelY = graphics->mHeight + scrollY + mTileHeight - 1
        + mMaxTileHeight - mTileHeight;
    const int startX = scrollX / mTileWidth;
    const int startY = scrollY / mTileHeight;
    const int endX = (graphics->mWidth + scrollX + mTileWidth - 1)
        / mTileWidth;
    const int endY = endPixelY / mTileHeight;

    // Make sure actors are sorted ascending by Y-coordinate
    // so that they overlap correctly
//    if (mSpritesUpdated)
//    {
        mActors.sort(actorCompare);
//        mSpritesUpdated = false;
//    }

    // update scrolling of all ambient layers
    updateAmbientLayers(static_cast<float>(scrollX),
                        static_cast<float>(scrollY));

    // Draw backgrounds
    drawAmbientLayers(graphics, BACKGROUND_LAYERS, mOverlayDetail);

    if (mDebugFlags == MAP_BLACKWHITE && userPalette)
    {
        graphics->setColor(userPalette->getColorWithAlpha(
            UserPalette::WALKABLE_HIGHLIGHT));

        graphics->fillRectangle(gcn::Rectangle(0, 0,
            graphics->mWidth, graphics->mHeight));
    }

    // draw the game world
    Layers::const_iterator layeri = mLayers.begin();

    bool overFringe = false;
    int updateFlag = 0;

    if (mOpenGL == 1)
    {
        if (mLastX != startX || mLastY != startY || mLastScrollX != scrollX
            || mLastScrollY != scrollY)
        {   // player moving
            mLastX = startX;
            mLastY = startY;
            mLastScrollX = scrollX;
            mLastScrollY = scrollY;
            updateFlag = 2;
        }
        else if (mRedrawMap || startX != mDrawX || startY != mDrawY ||
                 scrollX != mDrawScrollX || scrollY != mDrawScrollY)
        {   // player mode to new position
            mRedrawMap = false;
            mDrawX = startX;
            mDrawY = startY;
            mDrawScrollX = scrollX;
            mDrawScrollY = scrollY;
            updateFlag = 1;
        }
    }

    if (mDebugFlags == MAP_SPECIAL3 || mDebugFlags == MAP_BLACKWHITE)
    {
        if (mFringeLayer)
        {
            mFringeLayer->setSpecialLayer(mSpecialLayer);
            mFringeLayer->setTempLayer(mTempLayer);
            mFringeLayer->drawFringe(graphics, startX, startY, endX, endY,
                scrollX, scrollY, &mActors, mDebugFlags, mActorFixY);
        }
    }
    else
    {
        for (; layeri != mLayers.end() && !overFringe; ++layeri)
        {
            if ((*layeri)->isFringeLayer())
            {
                (*layeri)->setSpecialLayer(mSpecialLayer);
                (*layeri)->setTempLayer(mTempLayer);
                if (mDebugFlags == MAP_SPECIAL2)
                    overFringe = true;

                (*layeri)->drawFringe(graphics, startX, startY, endX, endY,
                    scrollX, scrollY, &mActors, mDebugFlags, mActorFixY);
            }
            else
            {
                if (mOpenGL == 1 && updateFlag != 2)
                {
                    if (updateFlag)
                    {
                        (*layeri)->updateOGL(graphics, startX, startY,
                            endX, endY, scrollX, scrollY, mDebugFlags);
                    }

                    (*layeri)->drawOGL(graphics);
                }
                else
                {
/*
                    if (updateFlag)
                    {
                        (*layeri)->updateSDL(graphics, startX, startY,
                            endX, endY, scrollX, scrollY, mDebugFlags);
                    }

                    (*layeri)->drawSDL(graphics);
*/
                    (*layeri)->draw(graphics, startX, startY, endX, endY,
                        scrollX, scrollY, mDebugFlags);
                }
            }
        }
    }

    // Don't draw if gui opacity == 1
    if (mBeingOpacity && mOpacity != 1.0f)
    {
        // Draws beings with a lower opacity to make them visible
        // even when covered by a wall or some other elements...
        Actors::const_iterator ai = mActors.begin();
        while (ai != mActors.end())
        {
            if (Actor *actor = *ai)
            {
                if (!mOpenGL && (actor->getTileX() < startX
                    || actor->getTileX() > endX || actor->getTileY() < startY
                    || actor->getTileY() > endY))
                {
                    ++ai;
                    continue;
                }
                // For now, just draw actors with only one layer.
                if (actor->getNumberOfLayers() == 1)
                {
                    actor->setAlpha(0.3f);
                    actor->draw(graphics, -scrollX, -scrollY);
                    actor->setAlpha(1.0f);
                }
            }
            ++ai;
        }
    }

    drawAmbientLayers(graphics, FOREGROUND_LAYERS, mOverlayDetail);
}

void Map::drawCollision(Graphics *graphics, int scrollX, int scrollY,
                        int debugFlags)
{
    int endPixelY = graphics->mHeight + scrollY + mTileHeight - 1;
    int startX = scrollX / mTileWidth;
    int startY = scrollY / mTileHeight;
    int endX = (graphics->mWidth + scrollX + mTileWidth - 1) / mTileWidth;
    int endY = endPixelY / mTileHeight;

    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    if (endX > mWidth) endX = mWidth;
    if (endY > mHeight) endY = mHeight;


    if (debugFlags < MAP_SPECIAL)
    {
        graphics->setColor(gcn::Color(0, 0, 0, 64));
        graphics->drawNet(
            startX * mTileWidth - scrollX,
            startY * mTileHeight - scrollY,
            endX * mTileWidth - scrollX,
            endY * mTileHeight - scrollY,
            32, 32);
    }

    for (int y = startY; y < endY; y++)
    {
        const int yWidth = y * mWidth;
        int tilePtr = startX + yWidth;
        for (int x = startX; x < endX; x++, tilePtr++)
        {
            int width = 0;
            int x0 = x;

            if (mMetaTiles[tilePtr].blockmask & BLOCKMASK_WALL)
            {
                width = 32;
                for (int x2 = tilePtr + 1; x < endX; x2 ++)
                {
                    if (!(mMetaTiles[x2].blockmask
                        & BLOCKMASK_WALL))
                    {
                        break;
                    }
                    width += 32;
                    x ++;
                    tilePtr ++;
                }
                if (width && userPalette)
                {
                    graphics->setColor(userPalette->getColorWithAlpha(
                        UserPalette::COLLISION_HIGHLIGHT));

                    graphics->fillRectangle(gcn::Rectangle(
                        x0 * mTileWidth - scrollX,
                        y * mTileHeight - scrollY,
                        width, 32));
                }
            }

            if (x < endX && mMetaTiles[tilePtr].blockmask
                & BLOCKMASK_AIR)
            {
                width = 32;
                for (int x2 = tilePtr + 1; x < endX; x2 ++)
                {
                    if (!(mMetaTiles[x2].blockmask
                        & BLOCKMASK_AIR))
                    {
                        break;
                    }
                    width += 32;
                    x ++;
                    tilePtr ++;
                }
                if (width && userPalette)
                {
                    graphics->setColor(userPalette->getColorWithAlpha(
                        UserPalette::AIR_COLLISION_HIGHLIGHT));

                    graphics->fillRectangle(gcn::Rectangle(
                        x0 * mTileWidth - scrollX,
                        y * mTileHeight - scrollY,
                        width, 32));
                }
            }

            if (x < endX && mMetaTiles[tilePtr].blockmask
                & BLOCKMASK_WATER)
            {
                width = 32;
                for (int x2 = tilePtr + 1; x < endX; x2 ++)
                {
                    if (!(mMetaTiles[x2].blockmask
                        & BLOCKMASK_WATER))
                    {
                        break;
                    }
                    width += 32;
                    x ++;
                    tilePtr ++;
                }
                if (width && userPalette)
                {
                    graphics->setColor(userPalette->getColorWithAlpha(
                        UserPalette::WATER_COLLISION_HIGHLIGHT));

                    graphics->fillRectangle(gcn::Rectangle(
                        x0 * mTileWidth - scrollX,
                        y * mTileHeight - scrollY,
                        width, 32));
                }
            }
        }
    }
}

void Map::updateAmbientLayers(float scrollX, float scrollY)
{
    static int lastTick = tick_time; // static = only initialized at first call

    if (mLastAScrollX == 0.0f && mLastAScrollY == 0.0f)
    {
        // First call - initialisation
        mLastAScrollX = scrollX;
        mLastAScrollY = scrollY;
    }

    // Update Overlays
    float dx = scrollX - mLastAScrollX;
    float dy = scrollY - mLastAScrollY;
    int timePassed = get_elapsed_time(lastTick);

    std::vector<AmbientLayer*>::const_iterator i;
    for (i = mBackgrounds.begin(); i != mBackgrounds.end(); ++i)
        (*i)->update(timePassed, dx, dy);

    for (i = mForegrounds.begin(); i != mForegrounds.end(); ++i)
        (*i)->update(timePassed, dx, dy);

    mLastAScrollX = scrollX;
    mLastAScrollY = scrollY;
    lastTick = tick_time;
}

void Map::drawAmbientLayers(Graphics *graphics, LayerType type,
                            int detail)
{
    // Detail 0 = no ambient effects except background image
    if (detail <= 0 && type != BACKGROUND_LAYERS)
        return;

    // find out which layer list to draw
    std::vector<AmbientLayer*> *layers;
    switch (type)
    {
        case FOREGROUND_LAYERS:
            layers = &mForegrounds;
            break;
        case BACKGROUND_LAYERS:
            layers = &mBackgrounds;
            break;
        default:
            // New type of ambient layers added here without adding it
            // to Map::drawAmbientLayers.
            assert(false);
            break;
    }

    // Draw overlays
    for (std::vector<AmbientLayer*>::const_iterator i = layers->begin();
         i != layers->end(); ++i)
    {
        if (*i)
            (*i)->draw(graphics, graphics->mWidth, graphics->mHeight);

        // Detail 1: only one overlay, higher: all overlays
        if (detail == 1)
            break;
    }
}

Tileset *Map::getTilesetWithGid(int gid) const
{
    if (gid >= 0 && gid < mIndexedTilesetsSize)
        return mIndexedTilesets[gid];
    else
        return nullptr;
}

void Map::blockTile(int x, int y, BlockType type)
{
    if (type == BLOCKTYPE_NONE || !contains(x, y))
        return;

    const int tileNum = x + y * mWidth;

    if (mOccupation[type][tileNum] < UINT_MAX &&
        (++mOccupation[type][tileNum]) > 0)
    {
        switch (type)
        {
            case BLOCKTYPE_WALL:
                mMetaTiles[tileNum].blockmask |= BLOCKMASK_WALL;
                break;
            case BLOCKTYPE_CHARACTER:
                mMetaTiles[tileNum].blockmask |= BLOCKMASK_CHARACTER;
                break;
            case BLOCKTYPE_MONSTER:
                mMetaTiles[tileNum].blockmask |= BLOCKMASK_MONSTER;
                break;
            case BLOCKTYPE_AIR:
                mMetaTiles[tileNum].blockmask |= BLOCKMASK_AIR;
                break;
            case BLOCKTYPE_WATER:
                mMetaTiles[tileNum].blockmask |= BLOCKMASK_WATER;
                break;
            case BLOCKTYPE_GROUND:
                mMetaTiles[tileNum].blockmask |= BLOCKMASK_GROUND;
                break;
            default:
            case BLOCKTYPE_NONE:
            case NB_BLOCKTYPES:
                // Do nothing.
                break;
        }
    }
}

bool Map::getWalk(int x, int y, unsigned char walkmask) const
{
    // You can't walk outside of the map
    if (x < 0 || y < 0 || x >= mWidth || y >= mHeight)
        return false;

    // Check if the tile is walkable
    return !(mMetaTiles[x + y * mWidth].blockmask & walkmask);
}

bool Map::occupied(int x, int y) const
{
    const ActorSprites &actors = actorSpriteManager->getAll();
    ActorSpritesConstIterator it, it_end;
    for (it = actors.begin(), it_end = actors.end(); it != it_end; ++it)
    {
        const ActorSprite *actor = *it;

//+++        if (actor->getTileX() == x && actor->getTileY() == y
//            && being->getSubType() != 45)
        if (actor->getTileX() == x && actor->getTileY() == y &&
            actor->getType() != ActorSprite::FLOOR_ITEM)
        {
            return true;
        }
    }

    return false;
}

bool Map::contains(int x, int y) const
{
    return x >= 0 && y >= 0 && x < mWidth && y < mHeight;
}

MetaTile *Map::getMetaTile(int x, int y) const
{
    return &mMetaTiles[x + y * mWidth];
}

Actors::iterator Map::addActor(Actor *actor)
{
    mActors.push_front(actor);
//    mSpritesUpdated = true;
    return mActors.begin();
}

void Map::removeActor(Actors::iterator iterator)
{
    mActors.erase(iterator);
//    mSpritesUpdated = true;
}

const std::string Map::getMusicFile() const
{
    return getProperty("music");
}

const std::string Map::getName() const
{
    if (hasProperty("name"))
        return getProperty("name");

    return getProperty("mapname");
}

const std::string Map::getFilename() const
{
    std::string fileName = getProperty("_filename");
    int lastSlash = static_cast<int>(fileName.rfind("/")) + 1;
    int lastDot = static_cast<int>(fileName.rfind("."));

    return fileName.substr(lastSlash, lastDot - lastSlash);
}

Position Map::checkNodeOffsets(int radius, unsigned char walkMask,
                               const Position &position) const
{
    // Pre-computing character's position in tiles
    const int tx = position.x / 32;
    const int ty = position.y / 32;

    // Pre-computing character's position offsets.
    int fx = position.x % 32;
    int fy = position.y % 32;

    // Compute the being radius:
    // FIXME: Hande beings with more than 1/2 tile radius by not letting them
    // go or spawn in too narrow places. The server will have to be aware
    // of being's radius value (in tiles) to handle this gracefully.
    if (radius > 32 / 2)
        radius = 32 / 2;
    // set a default value if no value returned.
    if (radius < 1)
        radius = 32 / 3;

    // We check diagonal first as they are more restrictive.
    // Top-left border check
    if (!getWalk(tx - 1, ty - 1, walkMask)
        && fy < radius && fx < radius)
    {
        fx = radius;
        fy = radius;
    }
    // Top-right border check
    if (!getWalk(tx + 1, ty - 1, walkMask)
        && (fy < radius) && fx > (32 - radius))
    {
        fx = 32 - radius;
        fy = radius;
    }
    // Bottom-left border check
    if (!getWalk(tx - 1, ty + 1, walkMask)
        && fy > (32 - radius) && fx < radius)
    {
        fx = radius;
        fy = 32 - radius;
    }
    // Bottom-right border check
    if (!getWalk(tx + 1, ty + 1, walkMask)
        && fy > (32 - radius) && fx > (32 - radius))
    {
        fx = 32 - radius;
        fy = fx;
    }

    // Fix coordinates so that the player does not seem to dig into walls.
    if (fx > (32 - radius) && !getWalk(tx + 1, ty, walkMask))
        fx = 32 - radius;
    else if (fx < radius && !getWalk(tx - 1, ty, walkMask))
        fx = radius;
    else if (fy > (32 - radius) && !getWalk(tx, ty + 1, walkMask))
        fy = 32 - radius;
    else if (fy < radius && !getWalk(tx, ty - 1, walkMask))
        fy = radius;

    return Position(tx * 32 + fx, ty * 32 + fy);
}

Path Map::findPixelPath(int startPixelX, int startPixelY, int endPixelX,
                        int endPixelY,
                        int radius, unsigned char walkMask, int maxCost)
{
    Path myPath = findPath(startPixelX / 32, startPixelY / 32,
                           endPixelX / 32, endPixelY / 32, walkMask, maxCost);

    // Don't compute empty coordinates.
    if (myPath.empty())
        return myPath;

    // Find the starting offset
    float startOffsetX = static_cast<float>(startPixelX % 32);
    float startOffsetY = static_cast<float>(startPixelY % 32);

    // Find the ending offset
    float endOffsetX = static_cast<float>(endPixelX % 32);
    float endOffsetY = static_cast<float>(endPixelY % 32);

    // Find the distance, and divide it by the number of steps
    int changeX = static_cast<int>((endOffsetX - startOffsetX)
                  / static_cast<float>(myPath.size()));
    int changeY = static_cast<int>((endOffsetY - startOffsetY)
                  / static_cast<float>(myPath.size()));

    // Convert the map path to pixels over tiles
    // And add interpolation between the starting and ending offsets
    Path::iterator it = myPath.begin();
    int i = 0;
    while (it != myPath.end())
    {
        // A position that is valid on the start and end tile is not
        // necessarily valid on all the tiles in between, so check the offsets.
        *it = checkNodeOffsets(radius, walkMask,
            it->x * 32 + startOffsetX + static_cast<float>(changeX * i),
            it->y * 32 + startOffsetY + static_cast<float>(changeY * i));
        i++;
        ++it;
    }

    // Remove the last path node, as it's more clever to go to the destination.
    // It also permit to avoid zigzag at the end of the path,
    // especially with mouse.
    Position destination = checkNodeOffsets(radius, walkMask,
                                            endPixelX, endPixelY);
    myPath.pop_back();
    myPath.push_back(destination);

    return myPath;
}

Path Map::findPath(int startX, int startY, int destX, int destY,
                   unsigned char walkmask, int maxCost)
{
    // The basic walking cost of a tile.
    static int const basicCost = 100;

    // Path to be built up (empty by default)
    Path path;

    if (startX >= mWidth || startY >= mHeight)
        return path;

    // Return when destination not walkable
    if (!getWalk(destX, destY, walkmask))
        return path;

    // Reset starting tile's G cost to 0
    MetaTile *startTile = &mMetaTiles[startX + startY * mWidth];
    if (!startTile)
        return path;

    startTile->Gcost = 0;

    // Declare open list, a list with open tiles sorted on F cost
    std::priority_queue<Location> openList;

    // Add the start point to the open list
    openList.push(Location(startX, startY, startTile));

    bool foundPath = false;

    // Keep trying new open tiles until no more tiles to try or target found
    while (!openList.empty() && !foundPath)
    {
        // Take the location with the lowest F cost from the open list.
        Location curr = openList.top();
        openList.pop();

        // If the tile is already on the closed list, this means it has already
        // been processed with a shorter path to the start point (lower G cost)
        if (curr.tile->whichList == mOnClosedList)
            continue;

        // Put the current tile on the closed list
        curr.tile->whichList = mOnClosedList;

        const int curWidth = curr.y * mWidth;

        // Check the adjacent tiles
        for (int dy = -1; dy <= 1; dy++)
        {
            const int y = curr.y + dy;
            const int yWidth = y * mWidth;
            const int dy1 = std::abs(y - destY);

            for (int dx = -1; dx <= 1; dx++)
            {
                // Calculate location of tile to check
                const int x = curr.x + dx;

                // Skip if if we're checking the same tile we're leaving from,
                // or if the new location falls outside of the map boundaries
                if ((dx == 0 && dy == 0) || !contains(x, y))
                    continue;

                MetaTile *newTile = &mMetaTiles[x + yWidth];

                // Skip if the tile is on the closed list or is not walkable
                // unless its the destination tile
                //+++ here need check block must depend on player abilities.
                if (!newTile || newTile->whichList == mOnClosedList ||
                    ((newTile->blockmask & walkmask)
                    && !(x == destX && y == destY))
                    || (newTile->blockmask & BLOCKMASK_WALL))
                {
                    continue;
                }

                // When taking a diagonal step, verify that we can skip the
                // corner.
                if (dx != 0 && dy != 0)
                {
                    MetaTile *t1 = &mMetaTiles[curr.x +
                        (curr.y + dy) * mWidth];
                    MetaTile *t2 = &mMetaTiles[curr.x + dx + curWidth];

                    //+++ here need check block must depend on player abilities.
                    if (!t1 || !t2 || ((t1->blockmask | t2->blockmask)
                        & BLOCKMASK_WALL))
                    {
                        continue;
                    }
                }

                // Calculate G cost for this route, ~sqrt(2) for moving diagonal
                int Gcost = curr.tile->Gcost +
                    (dx == 0 || dy == 0 ? basicCost : basicCost * 362 / 256);

                /* Demote an arbitrary direction to speed pathfinding by
                   adding a defect (TODO: change depending on the desired
                   visual effect, e.g. a cross-product defect toward
                   destination).
                   Important: as long as the total defect along any path is
                   less than the basicCost, the pathfinder will still find one
                   of the shortest paths! */
                if (dx == 0 || dy == 0)
                {
                    // Demote horizontal and vertical directions, so that two
                    // consecutive directions cannot have the same Fcost.
                    ++Gcost;
                }

                // It costs extra to walk through a being (needs to be enough
                // to make it more attractive to walk around).
//                if (occupied(x, y))
//                {
//                    Gcost += 3 * basicCost;
//                }

                // Skip if Gcost becomes too much
                // Warning: probably not entirely accurate
                if (maxCost > 0 && Gcost > maxCost * basicCost)
                    continue;

                if (newTile->whichList != mOnOpenList)
                {
                    // Found a new tile (not on open nor on closed list)

                    /* Update Hcost of the new tile. The pathfinder does not
                       work reliably if the heuristic cost is higher than the
                       real cost. In particular, using Manhattan distance is
                       forbidden here. */
                    int dx1 = std::abs(x - destX);
                    newTile->Hcost = std::abs(dx1 - dy1) * basicCost +
                        std::min(dx1, dy1) * (basicCost * 362 / 256);

                    // Set the current tile as the parent of the new tile
                    newTile->parentX = curr.x;
                    newTile->parentY = curr.y;

                    // Update Gcost and Fcost of new tile
                    newTile->Gcost = Gcost;
                    newTile->Fcost = Gcost + newTile->Hcost;

                    if (x != destX || y != destY)
                    {
                        // Add this tile to the open list
                        newTile->whichList = mOnOpenList;
                        openList.push(Location(x, y, newTile));
                    }
                    else
                    {
                        // Target location was found
                        foundPath = true;
                    }
                }
                else if (Gcost < newTile->Gcost)
                {
                    // Found a shorter route.
                    // Update Gcost and Fcost of the new tile
                    newTile->Gcost = Gcost;
                    newTile->Fcost = Gcost + newTile->Hcost;

                    // Set the current tile as the parent of the new tile
                    newTile->parentX = curr.x;
                    newTile->parentY = curr.y;

                    // Add this tile to the open list (it's already
                    // there, but this instance has a lower F score)
                    openList.push(Location(x, y, newTile));
                }
            }
        }
    }

    // Two new values to indicate whether a tile is on the open or closed list,
    // this way we don't have to clear all the values between each pathfinding.
    if (mOnOpenList > UINT_MAX - 2)
    {
        // We reset the list memebers value.
        mOnClosedList = 1;
        mOnOpenList = 2;

        // Clean up the metaTiles
        const int size = mWidth * mHeight;
        for (int i = 0; i < size; ++i)
            mMetaTiles[i].whichList = 0;
    }
    else
    {
        mOnClosedList += 2;
        mOnOpenList += 2;
    }

    // If a path has been found, iterate backwards using the parent locations
    // to extract it.
    if (foundPath)
    {
        int pathX = destX;
        int pathY = destY;

        while (pathX != startX || pathY != startY)
        {
            // Add the new path node to the start of the path list
            path.push_front(Position(pathX, pathY));

            // Find out the next parent
            MetaTile *tile = &mMetaTiles[pathX + pathY * mWidth];
            pathX = tile->parentX;
            pathY = tile->parentY;
        }
    }

    return path;
}

void Map::addParticleEffect(const std::string &effectFile,
                            int x, int y, int w, int h)
{
    ParticleEffectData newEffect;
    newEffect.file = effectFile;
    newEffect.x = x;
    newEffect.y = y;
    newEffect.w = w;
    newEffect.h = h;
    particleEffects.push_back(newEffect);
}

void Map::initializeParticleEffects(Particle *engine)
{
    if (!engine)
        return;

    Particle *p;

    if (config.getBoolValue("particleeffects"))
    {
        for (std::vector<ParticleEffectData>::const_iterator
             i = particleEffects.begin();
             i != particleEffects.end(); ++i)
        {
            p = engine->addEffect(i->file, i->x, i->y);
            if (p && i->w > 0 && i->h > 0)
                p->adjustEmitterSize(i->w, i->h);
        }
    }
}

void Map::addExtraLayer()
{
    if (!mSpecialLayer)
    {
        logger->log1("No special layer");
        return;
    }
    std::string mapFileName = getUserMapDirectory() + "/extralayer.txt";
    logger->log("loading extra layer: " + mapFileName);
    struct stat statbuf;
    if (!stat(mapFileName.c_str(), &statbuf) && S_ISREG(statbuf.st_mode))
    {
        std::ifstream mapFile;
        mapFile.open(mapFileName.c_str(), std::ios::in);
        if (!mapFile.is_open())
        {
            mapFile.close();
            return;
        }
        char line[201];

        while (mapFile.getline(line, 200))
        {
            std::string buf;
            std::string str = line;
            if (!str.empty())
            {
                std::string x;
                std::string y;
                std::string type1;
                std::string comment;
                std::stringstream ss(str);
                ss >> x;
                ss >> y;
                ss >> type1;
                std::vector<int> tokens;
                ss >> comment;
                while (ss >> buf)
                    comment += " " + buf;

                int type = atoi(type1.c_str());

                if (comment.empty())
                {
                    if (type < MapItem::ARROW_UP
                        || type > MapItem::ARROW_RIGHT)
                    {
                        comment = "unknown";
                    }
                }
                if (type == MapItem::PORTAL)
                {
                    updatePortalTile(comment, type, atoi(x.c_str()),
                                     atoi(y.c_str()), false);
                }
                else if (type == MapItem::HOME)
                {
                    updatePortalTile(comment, type, atoi(x.c_str()),
                                     atoi(y.c_str()));
                }
                else
                {
                    addPortalTile(comment, type, atoi(x.c_str()),
                                  atoi(y.c_str()));
                }
            }
        }
        mapFile.close();
    }
}

void Map::saveExtraLayer()
{
    if (!mSpecialLayer)
    {
        logger->log1("No special layer");
        return;
    }
    std::string mapFileName = getUserMapDirectory() + "/extralayer.txt";
    logger->log("saving extra layer: " + mapFileName);

    if (mkdir_r(getUserMapDirectory().c_str()))
    {
        logger->log(strprintf("%s doesn't exist and can't be created! "
                              "Exiting.", getUserMapDirectory().c_str()));
        return;
    }

    std::ofstream mapFile;
    mapFile.open(mapFileName.c_str(), std::ios::binary);
    if (!mapFile.is_open())
    {
        logger->log1("Unable to open extralayer.txt for writing");
        return;
    }

    int width = mSpecialLayer->mWidth;
    int height = mSpecialLayer->mHeight;

    for (int x = 0; x < width; x ++)
    {
        for (int y = 0; y < height; y ++)
        {
            MapItem *item = mSpecialLayer->getTile(x, y);
            if (item && item->mType != MapItem::EMPTY
                && item->mType != MapItem::HOME)
            {
                mapFile << x << " " << y << " "
                    << static_cast<int>(item->mType) << " "
                    << item->mComment << std::endl;
            }
        }
    }
    mapFile.close();
}

std::string Map::getUserMapDirectory() const
{
    return Client::getServerConfigDirectory() + PHYSFS_getDirSeparator()
        + getProperty("_realfilename");
}

void Map::addRange(const std::string &name, int type,
                   int x, int y, int dx, int dy)
{
    if (!mObjects)
        return;

    mObjects->addObject(name, type, x / 32, y / 32, dx / 32, dy / 32);
}

void Map::addPortal(const std::string &name, int type,
                    int x, int y, int dx, int dy)
{
    addPortalTile(name, type, (x / 32) + (dx / 64), (y / 32) + (dy / 64));
}

void Map::addPortalTile(const std::string &name, int type, int x, int y)
{
    MapItem *item = new MapItem(type, name, x, y);
    if (mSpecialLayer)
        mSpecialLayer->setTile(x, y, item);

    item = new MapItem(type, name, x, y);
    mMapPortals.push_back(item);
}

void Map::updatePortalTile(const std::string &name, int type,
                           int x, int y, bool addNew)
{
    MapItem *item = findPortalXY(x, y);
    if (item)
    {
        item->mComment = name;
        item->setType(type);
        item->mX = x;
        item->mY = y;
        if (mSpecialLayer)
        {
            item = new MapItem(type, name, x, y);
            mSpecialLayer->setTile(x, y, item);
        }
    }
    else if (addNew)
    {
        addPortalTile(name, type, x, y);
    }
}

MapItem *Map::findPortalXY(int x, int y)
{
    std::vector<MapItem*>::const_iterator it;
    std::vector<MapItem*>::const_iterator it_end;

    for (it = mMapPortals.begin(), it_end = mMapPortals.end();
         it != it_end; ++it)
    {
        if (!*it)
            continue;

        MapItem *item = *it;
        if (item->mX == x && item->mY == y)
            return item;
    }
    return nullptr;
}

TileAnimation *Map::getAnimationForGid(int gid) const
{
    if (mTileAnimations.empty())
        return nullptr;

    std::map<int, TileAnimation*>::const_iterator
        i = mTileAnimations.find(gid);
    return (i == mTileAnimations.end()) ? nullptr : i->second;
}

void Map::setPvpMode(int mode)
{
    int oldMode = mPvp;

    if (!mode)
        mPvp = 0;
    else
        mPvp |= mode;

    if (mPvp != oldMode && player_node)
    {
        switch (mPvp)
        {
            case 0:
                player_node->setSpeech("pvp off, gvg off");
                break;
            case 1:
                player_node->setSpeech("pvp on");
                break;
            case 2:
                player_node->setSpeech("gvg on");
                break;
            case 3:
                player_node->setSpeech("pvp on, gvg on");
                break;
            default:
                player_node->setSpeech("unknown pvp");
                break;
        }
    }
}

std::string Map::getObjectData(unsigned x, unsigned y, int type)
{
    if (!mObjects)
        return "";

    MapObjectList *list = mObjects->getAt(x, y);
    if (!list)
        return "";

    std::vector<MapObject>::const_iterator it = list->objects.begin();
    while (it != list->objects.end())
    {
        if ((*it).type == type)
            return (*it).data;
    }

    return "";
}

void Map::indexTilesets()
{
    if (mTilesetsIndexed)
        return;

    mTilesetsIndexed = true;

    Tileset *s = nullptr;
    for (Tilesets::const_iterator it = mTilesets.begin(),
         it_end = mTilesets.end(); it < it_end;
         ++it)
    {
        if (!s || s->getFirstGid() + s->size()
            < (*it)->getFirstGid() + (*it)->size())
        {
            s = *it;
        }
    }
    if (!s)
    {
        mIndexedTilesetsSize = 0;
        mIndexedTilesets = nullptr;
        return;
    }

    const int size = s->getFirstGid() + s->size();
    mIndexedTilesetsSize = size;
    mIndexedTilesets = new Tileset*[size];
    std::fill_n(mIndexedTilesets, size, static_cast<Tileset*>(nullptr));

    for (Tilesets::const_iterator it = mTilesets.begin(),
         it_end = mTilesets.end(); it < it_end;
         ++it)
    {
        s = *it;
        if (s)
        {
            const int start = s->getFirstGid();
            const int end = start + s->size();
            for (int f = start; f < end; f ++)
            {
                if (f < size)
                    mIndexedTilesets[f] = s;
            }
        }
    }
}

void Map::clearIndexedTilesets()
{
    if (!mTilesetsIndexed)
        return;

    mTilesetsIndexed = false;
    delete [] mIndexedTilesets;
    mIndexedTilesetsSize = 0;
}

void Map::reduce()
{
    if (!mFringeLayer || mOpenGL > 0 ||
        !config.getBoolValue("enableMapReduce"))
    {
        return;
    }

    int cnt = 0;
    for (int x = 0; x < mWidth; x ++)
    {
        for (int y = 0; y < mHeight; y ++)
        {
            Layers::const_iterator layeri = mLayers.begin();
            bool correct(true);
            bool dontHaveAlpha(false);

            for (; layeri != mLayers.end(); ++ layeri)
            {
                MapLayer *layer = *layeri;
                if (x >= layer->mWidth || y >= layer->mHeight)
                    continue;

                Image *img = layer->mTiles[x + y * layer->mWidth];
                if (img)
                {
                    if (img->hasAlphaChannel() && img->isAlphaCalculated())
                    {
                        if (!img->isAlphaVisible())
                        {
                            dontHaveAlpha = true;
                            img->setAlphaVisible(false);
                        }
                    }
                    else if (img->mBounds.w > 32 || img->mBounds.h > 32)
                    {
                        correct = false;
                        img->setAlphaVisible(true);
                        break;
                    }
                    else if (!img->isHasAlphaChannel())
                    {
                        dontHaveAlpha = true;
                        img->setAlphaVisible(false);
                    }
                    else if (img->hasAlphaChannel())
                    {
                        Uint8 *arr = img->SDLgetAlphaChannel();
                        if (!arr)
                            continue;

                        bool bad(false);
                        bool stop(false);
                        int width;
                        SubImage *subImg = dynamic_cast<SubImage*>(img);
                        if (subImg)
                            width = subImg->mInternalBounds.w;
                        else
                            width = img->mBounds.w;

                        for (int f = img->mBounds.x;
                             f < img->mBounds.x + img->mBounds.w; f ++)
                        {
                            for (int d = img->mBounds.y;
                                 d < img->mBounds.y + img->mBounds.h; d ++)
                            {
                                Uint8 chan = arr[f + d * width];
                                if (chan != 255)
                                {
                                    bad = true;
                                    stop = true;
                                    break;
                                }
                            }
                            if (stop)
                                break;
                        }
                        if (!bad)
                        {
                            dontHaveAlpha = true;
                            img->setAlphaVisible(false);
                        }
                        else
                        {
                            img->setAlphaVisible(true);
                        }
                    }
                    img->setAlphaCalculated(true);
                }
            }
            if (!correct || !dontHaveAlpha)
                continue;

            Layers::reverse_iterator ri = mLayers.rbegin();
            while (ri != mLayers.rend())
            {
                MapLayer *layer = *ri;
                if (x >= layer->mWidth || y >= layer->mHeight)
                {
                    ++ ri;
                    continue;
                }

                Image *img = layer->mTiles[x + y * layer->mWidth];
                if (img && !img->isAlphaVisible())
                {   // removing all down tiles
                    ++ ri;
                    while (ri != mLayers.rend())
                    {
                        img = (*ri)->mTiles[x + y * (*ri)->mWidth];
                        if (img)
                        {
                            (*ri)->mTiles[x + y * (*ri)->mWidth] = nullptr;
                            cnt ++;
                        }
                        ++ ri;
                    }
                    break;
                }
                ++ ri;
            }
        }
    }
    logger->log("tiles reduced: %d", cnt);
}

void Map::redrawMap()
{
    mRedrawMap = true;
}
