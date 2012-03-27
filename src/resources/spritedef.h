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

#ifndef SPRITEDEF_H
#define SPRITEDEF_H

#include "resources/resource.h"

#include "utils/xml.h"

#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

class Action;
class Animation;
class ImageSet;

struct SpriteReference
{
    static SpriteReference *Empty;

    SpriteReference():
        sprite(""), variant(0)
    {}

    SpriteReference(std::string sprite0, int variant0)
    {
        sprite = sprite0;
        variant = variant0;
    }

    std::string sprite;
    int variant;
};

struct SpriteDisplay
{
    std::string image;
    std::string floor;
    std::vector<SpriteReference*> sprites;
    std::vector<std::string> particles;
};

typedef std::vector<SpriteReference*>::const_iterator SpriteRefs;

/*
 * Remember those are the main action.
 * Action subtypes, e.g.: "attack_bow" are to be passed by items.xml after
 * an ACTION_ATTACK call.
 * Which special to be use to to be passed with the USE_SPECIAL call.
 * Running, walking, ... is a sub-type of moving.
 * ...
 * Please don't add hard-coded subtypes here!
 */
namespace SpriteAction
{
    static const std::string DEFAULT = "stand";
    static const std::string STAND = "stand";
    static const std::string SIT = "sit";
    static const std::string SLEEP = "sleep";
    static const std::string DEAD = "dead";
    static const std::string MOVE = "walk";
    static const std::string ATTACK = "attack";
    static const std::string HURT = "hurt";
    static const std::string USE_SPECIAL = "special";
    static const std::string CAST_MAGIC = "magic";
    static const std::string USE_ITEM = "item";
    static const std::string SPAWN = "spawn";
    static const std::string INVALID("");
}

enum SpriteDirection
{
    DIRECTION_DEFAULT = 0,
    DIRECTION_UP = 1,
    DIRECTION_DOWN = 2,
    DIRECTION_LEFT = 3,
    DIRECTION_RIGHT = 4,
    DIRECTION_UPLEFT = 5,
    DIRECTION_UPRIGHT = 6,
    DIRECTION_DOWNLEFT = 7,
    DIRECTION_DOWNRIGHT = 8,
    DIRECTION_INVALID
};

/**
 * Defines a class to load an animation.
 */
class SpriteDef : public Resource
{
    public:
        /**
         * Loads a sprite definition file.
         */
        static SpriteDef *load(const std::string &file, int variant);

        /**
         * Returns the specified action.
         */
        Action *getAction(std::string action, unsigned num) const;

        unsigned findNumber(unsigned num) const;

        /**
         * Converts a string into a SpriteDirection enum.
         */
        static SpriteDirection

        makeSpriteDirection(const std::string &direction);

        void addAction(unsigned hp, std::string name, Action *action);

        bool addSequence(int start, int end, int delay,
                         int offsetX, int offsetY, int variant_offset,
                         int repeat, int rand, ImageSet *imageSet,
                         Animation *animation);

    private:
        /**
         * Constructor.
         */
        SpriteDef()
        { }

        /**
         * Destructor.
         */
        ~SpriteDef();

        /**
         * Loads a sprite element.
         */
        void loadSprite(XmlNodePtr spriteNode, int variant,
                        const std::string &palettes = "");

        /**
         * Loads an imageset element.
         */
        void loadImageSet(XmlNodePtr node, const std::string &palettes);

        /**
         * Loads an action element.
         */
        void loadAction(XmlNodePtr node, int variant_offset);

        /**
         * Loads an animation element.
         */
        void loadAnimation(XmlNodePtr animationNode,
                           Action *action, ImageSet *imageSet,
                           int variant_offset);

        /**
         * Include another sprite into this one.
         */
        void includeSprite(XmlNodePtr includeNode);

        /**
         * Complete missing actions by copying existing ones.
         */
        void substituteActions();

        /**
         * Fix bad timeout in last dead action frame
         */
        void fixDeadAction();

        /**
         * When there are no animations defined for the action "complete", its
         * animations become a copy of those of the action "with".
         */
        void substituteAction(std::string complete, std::string with);

        typedef std::map<std::string, ImageSet*> ImageSets;
        typedef ImageSets::iterator ImageSetIterator;
        typedef std::map<std::string, Action*> ActionMap;
        typedef std::map<unsigned, ActionMap*> Actions;
        typedef Actions::const_iterator ActionsConstIter;
        typedef Actions::iterator ActionsIter;

        ImageSets mImageSets;
        Actions mActions;
        std::set<std::string> mProcessedFiles;
};

#endif // SPRITEDEF_H
