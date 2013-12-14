/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#ifndef BEING_ACTOR_H
#define BEING_ACTOR_H

#include "vector.h"

#include <list>

#include "localconsts.h"

class Actor;
class Graphics;
class Map;

typedef std::list<Actor*> Actors;
typedef Actors::const_iterator ActorsCIter;

class Actor
{
public:
    A_DELETE_COPY(Actor)

    virtual ~Actor();

    /**
     * Draws the Actor to the given graphics context.
     *
     * Note: this function could be simplified if the graphics context
     * would support setting a translation offset. It already does this
     * partly with the clipping rectangle support.
     */
    virtual void draw(Graphics *const graphics,
                      const int offsetX, const int offsetY) const = 0;

    /**
     * Returns the horizontal size of the actors graphical representation
     * in pixels or 0 when it is undefined.
     */
    virtual int getWidth() const A_WARN_UNUSED
    { return 0; }

    /**
     * Returns the vertical size of the actors graphical representation
     * in pixels or 0 when it is undefined.
     */
    virtual int getHeight() const A_WARN_UNUSED
    { return 0; }

    /**
     * Returns the pixel position of this actor.
     */
    const Vector &getPosition() const A_WARN_UNUSED
    { return mPos; }

    /**
     * Sets the pixel position of this actor.
     */
    virtual void setPosition(const Vector &pos)
    { mPos = pos; }

    /**
     * Returns the pixels X coordinate of the actor.
     */
    int getPixelX() const A_WARN_UNUSED
    { return static_cast<int>(mPos.x); }

    /**
     * Returns the pixel Y coordinate of the actor.
     */
    virtual int getPixelY() const A_WARN_UNUSED
    { return static_cast<int>(mPos.y); }

    /**
     * Returns the pixel Y coordinate of the actor for sorting only.
     */
    virtual int getSortPixelY() const A_WARN_UNUSED
    { return static_cast<int>(mPos.y) - mYDiff; }

    /**
     * Returns the x coordinate in tiles of the actor.
     */
    virtual int getTileX() const A_WARN_UNUSED;

    /**
     * Returns the y coordinate in tiles of the actor.
     */
    virtual int getTileY() const A_WARN_UNUSED;

    /**
     * Returns the number of Image layers used to draw the actor.
     */
    virtual int getNumberOfLayers() const A_WARN_UNUSED
    { return 0; }

    /**
     * Returns the current alpha value used to draw the actor.
     */
    virtual float getAlpha() const A_WARN_UNUSED = 0;

    /**
     * Sets the alpha value used to draw the actor.
     */
    virtual void setAlpha(float alpha) = 0;

    virtual void setMap(Map *const map);

    const Map* getMap() const A_WARN_UNUSED
    { return mMap; }

protected:
    Actor();

    Map *mMap;
    Vector mPos;                /**< Position in pixels relative to map. */
    int mYDiff;

private:
    Actors::iterator mMapActor;
};

#endif  // BEING_ACTOR_H
