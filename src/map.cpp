/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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
#include "client.h"
#include "configuration.h"
#include "graphics.h"
#include "log.h"
#include "particle.h"
#include "simpleanimation.h"
#include "tileset.h"
#include "localplayer.h"

#include "resources/ambientlayer.h"
#include "resources/image.h"
#include "resources/resourcemanager.h"

#include "gui/gui.h"
#include "gui/palette.h"
#include "gui/sdlfont.h"

#include "gui/widgets/chattab.h"

#include "utils/dtor.h"
#include "utils/mkdir.h"
#include "utils/stringutils.h"

#include <queue>
#include <limits.h>

#include <sys/stat.h>

bool actorCompare(const Actor *a, const Actor *b);

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

TileAnimation::TileAnimation(Animation *ani):
    mLastImage(NULL)
{
    mAnimation = new SimpleAnimation(ani);
}

TileAnimation::~TileAnimation()
{
    delete mAnimation;
    mAnimation = 0;
}

void TileAnimation::update(int ticks)
{
    if (!mAnimation)
        return;

    // update animation
    mAnimation->update(ticks);

    // exchange images
    Image *img = mAnimation->getCurrentImage();
    if (img != mLastImage)
    {
        for (std::list<std::pair<MapLayer*, int> >::iterator i =
             mAffected.begin(); i != mAffected.end(); ++i)
        {
            i->first->setTile(i->second, img);
        }
        mLastImage = img;
    }
}

MapLayer::MapLayer(int x, int y, int width, int height, bool isFringeLayer):
    mX(x), mY(y),
    mWidth(width), mHeight(height),
    mIsFringeLayer(isFringeLayer),
    mHighlightAttackRange(config.getBoolValue("highlightAttackRange"))
{
    const int size = mWidth * mHeight;
    mTiles = new Image*[size];

    std::fill_n(mTiles, size, static_cast<Image*>(0));

    config.addListener("highlightAttackRange", this);
}

MapLayer::~MapLayer()
{
    config.removeListener("highlightAttackRange", this);
    delete[] mTiles;
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
    setTile(x + y * mWidth, img);
}

Image* MapLayer::getTile(int x, int y) const
{
    return mTiles[x + y * mWidth];
}

void MapLayer::draw(Graphics *graphics, int startX, int startY,
                    int endX, int endY, int scrollX, int scrollY,
                    const Actors &actors, int debugFlags, int yFix) const
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

    Actors::const_iterator ai = actors.begin();

    const int dx = (mX * 32) - scrollX;
    const int dy = (mY * 32) - scrollY + 32;

    int specialWidth = 0;
    int specialHeight = 0;

    const bool extraDraw =
        mIsFringeLayer && mSpecialLayer && mTempLayer;

    if (mIsFringeLayer)
    {
        if (mSpecialLayer)
        {
            specialWidth = mSpecialLayer->mWidth;
            specialHeight = mSpecialLayer->mHeight;
        }
    }

    for (int y = startY; y < endY; y++)
    {
        const int y32 = y * 32;
        const int y32s = (y + yFix) * 32;
        const int yWidth = y * mWidth;

        // If drawing the fringe layer, make sure all actors above this row of
        // tiles have been drawn
        if (mIsFringeLayer)
        {
            while (ai != actors.end() && (*ai)->getPixelY() <= y32s)
            {
                (*ai)->draw(graphics, -scrollX, -scrollY);
                ++ ai;
            }
        }

        if (debugFlags == Map::MAP_SPECIAL3
            || debugFlags == Map::MAP_BLACKWHITE)
        {
            if (extraDraw && y < specialHeight)
            {
                //x + y * mWidth
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

            for (int x = startX; x < endX; x++)
            {
                const int x32 = x * 32;

                const int px1 = x32 - scrollX;
                const int tilePtr = x + yWidth;
                int c = 0;
                Image *img = mTiles[tilePtr];
                if (img)
                {
                    const int px = x32 + dx;
                    const int py = py0 - img->getHeight();
                    if ((debugFlags != Map::MAP_SPECIAL
                        && debugFlags != Map::MAP_SPECIAL2)
                        || img->getHeight() <= 32)
                    {
                        int width = 0;
                        // here need not draw over player position
                        c = getTileDrawWidth(tilePtr, endX - x, width);

                        if (!c)
                        {
                            graphics->drawImage(img, px, py);
                        }
                        else
                        {
                            graphics->drawImagePattern(img, px, py,
                                width, img->getHeight());
                        }
                    }
                }

                if (extraDraw && y < specialHeight)
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
    if (mIsFringeLayer && debugFlags != Map::MAP_SPECIAL3)
    {
        while (ai != actors.end())
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

int MapLayer::getTileDrawWidth(int tilePtr, int endX, int &width) const
{
    Image *img1 = mTiles[tilePtr];
    int c = 0;
    if (!img1)
    {
        width = 0;
        return c;
    }
    width = img1->getWidth();
    for (int x = 1; x < endX; x++)
    {
        tilePtr ++;
        Image *img = mTiles[tilePtr];
        if (img != img1)
            break;
        c ++;
        if (img)
            width += img->getWidth();
    }
    return c;
}

Map::Map(int width, int height, int tileWidth, int tileHeight):
    mWidth(width), mHeight(height),
    mTileWidth(tileWidth), mTileHeight(tileHeight),
    mMaxTileHeight(height),
    mHasWarps(false),
    mDebugFlags(MAP_NORMAL),
    mOnClosedList(1), mOnOpenList(2),
    mLastScrollX(0.0f), mLastScrollY(0.0f),
    mOverlayDetail(config.getIntValue("OverlayDetail")),
    mOpacity(config.getFloatValue("guialpha")),
    mPvp(0),
    mTilesetsIndexed(false),
    mIndexedTilesets(0),
    mIndexedTilesetsSize(0),
    mActorFixX(0),
    mActorFixY(0)
//    mSpritesUpdated(true)
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

#ifdef USE_OPENGL
    mOpenGL = config.getIntValue("opengl");
#else
    mOpenGL = 0;
#endif
}

Map::~Map()
{
    config.removeListener("OverlayDetail", this);
    config.removeListener("guialpha", this);

    // delete metadata, layers, tilesets and overlays
    delete[] mMetaTiles;
    for (int i = 0; i < NB_BLOCKTYPES; i++)
        delete[] mOccupation[i];

    delete_all(mLayers);
    delete_all(mTilesets);
    delete_all(mForegrounds);
    delete_all(mBackgrounds);
    delete_all(mTileAnimations);
    delete mSpecialLayer;
    mSpecialLayer = 0;
    delete mTempLayer;
    mTempLayer = 0;
    delete mObjects;
    mObjects = 0;
    delete_all(mMapPortals);
}

void Map::optionChanged(const std::string &value)
{
    if (value == "OverlayDetail")
        mOverlayDetail = config.getIntValue("OverlayDetail");
    else if (value == "guialpha")
        mOpacity = config.getFloatValue("guialpha");
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
    mLayers.push_back(layer);
}

void Map::addTileset(Tileset *tileset)
{
    if (!tileset)
        return;

    mTilesets.push_back(tileset);

    if (tileset->getHeight() > mMaxTileHeight)
        mMaxTileHeight = tileset->getHeight();
}

bool actorCompare(const Actor *a, const Actor *b)
{
    if (!a || !b)
        return false;

    return a->getPixelY() < b->getPixelY();
}

void Map::update(int ticks)
{
    // Update animated tiles
    for (std::map<int, TileAnimation*>::iterator
         iAni = mTileAnimations.begin();
         iAni != mTileAnimations.end(); ++iAni)
    {
        iAni->second->update(ticks);
    }
}

void Map::draw(Graphics *graphics, int scrollX, int scrollY)
{
    // Calculate range of tiles which are on-screen
    int endPixelY = graphics->getHeight() + scrollY + mTileHeight - 1;
    endPixelY += mMaxTileHeight - mTileHeight;
    int startX = scrollX / mTileWidth;
    int startY = scrollY / mTileHeight;
    int endX = (graphics->getWidth() + scrollX + mTileWidth - 1) / mTileWidth;
    int endY = endPixelY / mTileHeight;

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

    if (mDebugFlags == MAP_BLACKWHITE)
    {
        graphics->setColor(userPalette->getColorWithAlpha(
            UserPalette::WALKABLE_HIGHLIGHT));

        graphics->fillRectangle(gcn::Rectangle(0, 0,
            graphics->getWidth(), graphics->getHeight()));
    }

    // draw the game world
    Layers::const_iterator layeri = mLayers.begin();

    bool overFringe = false;

    if (mDebugFlags == MAP_SPECIAL3 || mDebugFlags == MAP_BLACKWHITE)
    {
        for (; layeri != mLayers.end(); ++layeri)
        {
            if ((*layeri)->isFringeLayer())
            {
                (*layeri)->setSpecialLayer(mSpecialLayer);
                (*layeri)->setTempLayer(mTempLayer);
                (*layeri)->draw(graphics,
                                startX, startY, endX, endY,
                                scrollX, scrollY,
                                mActors, mDebugFlags, mActorFixY);
                break;
            }
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
            }

            (*layeri)->draw(graphics,
                            startX, startY, endX, endY,
                            scrollX, scrollY,
                            mActors, mDebugFlags, mActorFixY);
        }
    }

    // Dont draw if gui opacity == 1
    if (mOpacity != 1.0f)
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
    int endPixelY = graphics->getHeight() + scrollY + mTileHeight - 1;
    int startX = scrollX / mTileWidth;
    int startY = scrollY / mTileHeight;
    int endX = (graphics->getWidth() + scrollX + mTileWidth - 1) / mTileWidth;
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
        for (int x = startX; x < endX; x++)
        {
            int width = 0;
            int x0 = x;

            if (!getWalk(x, y, BLOCKMASK_WALL))
            {
                width = 32;
                for (int x2 = x + 1; x < endX; x2 ++)
                {
                    if (getWalk(x2, y, BLOCKMASK_WALL))
                        break;
                    width += 32;
                    x ++;
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

            if (x < endX && !getWalk(x, y, BLOCKMASK_AIR))
            {
                width = 32;
                for (int x2 = x + 1; x < endX; x2 ++)
                {
                    if (getWalk(x2, y, BLOCKMASK_AIR))
                        break;
                    width += 32;
                    x ++;
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

            if (x < endX && !getWalk(x, y, BLOCKMASK_WATER))
            {
                width = 32;
                for (int x2 = x + 1; x < endX; x2 ++)
                {
                    if (getWalk(x2, y, BLOCKMASK_WATER))
                        break;
                    width += 32;
                    x ++;
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

    if (mLastScrollX == 0.0f && mLastScrollY == 0.0f)
    {
        // First call - initialisation
        mLastScrollX = scrollX;
        mLastScrollY = scrollY;
    }

    // Update Overlays
    float dx = scrollX - mLastScrollX;
    float dy = scrollY - mLastScrollY;
    int timePassed = get_elapsed_time(lastTick);

    std::list<AmbientLayer*>::iterator i;
    for (i = mBackgrounds.begin(); i != mBackgrounds.end(); ++i)
        (*i)->update(timePassed, dx, dy);

    for (i = mForegrounds.begin(); i != mForegrounds.end(); ++i)
        (*i)->update(timePassed, dx, dy);

    mLastScrollX = scrollX;
    mLastScrollY = scrollY;
    lastTick = tick_time;
}

void Map::drawAmbientLayers(Graphics *graphics, LayerType type,
                            int detail)
{
    // Detail 0 = no ambient effects except background image
    if (detail <= 0 && type != BACKGROUND_LAYERS)
        return;

    // find out which layer list to draw
    std::list<AmbientLayer*> *layers;
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
    for (std::list<AmbientLayer*>::iterator i = layers->begin();
         i != layers->end(); ++i)
    {
        (*i)->draw(graphics, graphics->getWidth(), graphics->getHeight());

        // Detail 1: only one overlay, higher: all overlays
        if (detail == 1)
            break;
    }
}

Tileset *Map::getTilesetWithGid(int gid) const
{
    if (gid < mIndexedTilesetsSize)
        return mIndexedTilesets[gid];
    else
        return 0;
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
    if (!contains(x, y))
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

    // Declare open list, a list with open tiles sorted on F cost
    std::priority_queue<Location> openList;

    // Return when destination not walkable
    if (!getWalk(destX, destY, walkmask))
        return path;

    // Reset starting tile's G cost to 0
    MetaTile *startTile = getMetaTile(startX, startY);
    startTile->Gcost = 0;

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

        // Check the adjacent tiles
        for (int dy = -1; dy <= 1; dy++)
        {
            for (int dx = -1; dx <= 1; dx++)
            {
                // Calculate location of tile to check
                const int x = curr.x + dx;
                const int y = curr.y + dy;

                // Skip if if we're checking the same tile we're leaving from,
                // or if the new location falls outside of the map boundaries
                if ((dx == 0 && dy == 0) || !contains(x, y))
                    continue;

                MetaTile *newTile = getMetaTile(x, y);

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
                    MetaTile *t1 = getMetaTile(curr.x, curr.y + dy);
                    MetaTile *t2 = getMetaTile(curr.x + dx, curr.y);

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
                    int dx = std::abs(x - destX), dy = std::abs(y - destY);
                    newTile->Hcost = std::abs(dx - dy) * basicCost +
                        std::min(dx, dy) * (basicCost * 362 / 256);

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
            MetaTile *tile = getMetaTile(pathX, pathY);
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

void Map::initializeParticleEffects(Particle *particleEngine)
{
    if (!particleEngine)
        return;

    Particle *p;

    if (config.getBoolValue("particleeffects"))
    {
        for (std::vector<ParticleEffectData>::iterator
             i = particleEffects.begin();
             i != particleEffects.end(); ++i)
        {
            p = particleEngine->addEffect(i->file, i->x, i->y);
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
        char line[201];
        std::string buf;
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
/*
                MapItem *item = new MapItem(atoi(type.c_str()), comment);
                int x1 = atoi(x.c_str());
                int y1 = atoi(y.c_str());
                mSpecialLayer->setTile(x1, y1, item);
*/
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
    return Client::getServerConfigDirectory() + "/"
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
    std::list<MapItem*>::iterator it;
    std::list<MapItem*>::iterator it_end;

    for (it = mMapPortals.begin(), it_end = mMapPortals.end();
         it != it_end; ++it)
    {
        MapItem *item = *it;
        if (item->mX == x && item->mY == y)
            return item;
    }
    return 0;
}

TileAnimation *Map::getAnimationForGid(int gid) const
{
    if (mTileAnimations.empty())
        return 0;

    std::map<int, TileAnimation*>::const_iterator
        i = mTileAnimations.find(gid);
    return (i == mTileAnimations.end()) ? NULL : i->second;
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

    std::list<MapObject>::iterator it = list->objects.begin();
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

    Tileset *s = 0;
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
        mIndexedTilesets = 0;
        return;
    }

    const int size = s->getFirstGid() + s->size();
    mIndexedTilesetsSize = size;
    mIndexedTilesets = new Tileset*[size];
    std::fill_n(mIndexedTilesets, size, static_cast<Tileset*>(0));

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
    delete[] mIndexedTilesets;
    mIndexedTilesetsSize = 0;
}

SpecialLayer::SpecialLayer(int width, int height, bool drawSprites):
    mWidth(width), mHeight(height)
{
    const int size = mWidth * mHeight;
    mTiles = new MapItem*[size];
    std::fill_n(mTiles, size, static_cast<MapItem*>(0));
    mDrawSprites = drawSprites;
}

SpecialLayer::~SpecialLayer()
{
    for (int f = 0; f < mWidth * mHeight; f ++)
    {
        delete mTiles[f];
        mTiles[f] = 0;
    }
    delete[] mTiles;
}

MapItem* SpecialLayer::getTile(int x, int y) const
{
    if (x < 0 || x >= mWidth ||
        y < 0 || y >= mHeight)
    {
        return 0;
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
    {
        mTiles[idx]->setType(type);
    }
    else
    {
        delete mTiles[idx];
        mTiles[idx] = new MapItem(type);
    }
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

//    MapSprites::const_iterator si = sprites.begin();

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
    mImage(0), mComment(""), mName(""), mX(-1), mY(-1)
{
    setType(EMPTY);
}

MapItem::MapItem(int type):
    mImage(0), mComment(""), mName(""), mX(-1), mY(-1)
{
    setType(type);
}

MapItem::MapItem(int type, std::string comment):
    mImage(0), mComment(comment), mName(""), mX(-1), mY(-1)
{
    setType(type);
}

MapItem::MapItem(int type, std::string comment, int x, int y):
    mImage(0), mComment(comment), mName(""), mX(x), mY(y)
{
    setType(type);
}

MapItem::~MapItem()
{
    if (mImage)
        mImage->decRef();
}

void MapItem::setType(int type)
{
    std::string name = "";
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
        mImage = 0;
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

    switch(mType)
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
    std::fill_n(mTiles, size, static_cast<MapObjectList*>(0));
}

ObjectsLayer::~ObjectsLayer()
{
    delete [] mTiles;
    mTiles = 0;
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
        return 0;
    return mTiles[x + y * mWidth];
}