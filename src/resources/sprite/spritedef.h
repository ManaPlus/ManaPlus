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

#ifndef RESOURCES_SPRITE_SPRITEDEF_H
#define RESOURCES_SPRITE_SPRITEDEF_H

#include "enums/resources/spritedirection.h"

#include "utils/xml.h"

#include <map>
#include <set>

class Action;
class Animation;
class ImageSet;

/**
 * Defines a class to load an animation.
 */
class SpriteDef final : public Resource
{
    public:
        A_DELETE_COPY(SpriteDef)

        /**
         * Loads a sprite definition file.
         */
        static SpriteDef *load(const std::string &file,
                               const int variant,
                               const bool prot) A_WARN_UNUSED;

        /**
         * Returns the specified action.
         */
        const Action *getAction(const std::string &action,
                                const unsigned num) const A_WARN_UNUSED;

        unsigned findNumber(const unsigned num) const A_WARN_UNUSED;

        /**
         * Converts a string into a SpriteDirection enum.
         */
        static SpriteDirection::Type
            makeSpriteDirection(const std::string &direction) A_WARN_UNUSED;

        void addAction(const unsigned hp, const std::string &name,
                       Action *const action);

        int calcMemoryLocal() const override final;

        int calcMemoryChilds(const int level) const override final;

        bool addSequence(const int start,
                         const int end,
                         const int delay,
                         const int offsetX,
                         const int offsetY,
                         const int variant_offset,
                         int repeat,
                         const int rand,
                         const ImageSet *const imageSet,
                         Animation *const animation) const;

    private:
        /**
         * Constructor.
         */
        SpriteDef() :
            Resource(),
            mImageSets(),
            mActions(),
            mProcessedFiles()
        { }

        /**
         * Destructor.
         */
        ~SpriteDef() override final;

        /**
         * Loads a sprite element.
         */
        void loadSprite(XmlNodeConstPtr spriteNode,
                        const int variant,
                        const std::string &palettes);

        /**
         * Loads an imageset element.
         */
        void loadImageSet(XmlNodeConstPtr node,
                          const std::string &palettes);

        /**
         * Loads an action element.
         */
        void loadAction(XmlNodeConstPtr node,
                        const int variant_offset);

        /**
         * Loads an animation element.
         */
        void loadAnimation(XmlNodeConstPtr animationNode,
                           Action *const action,
                           const ImageSet *const imageSet,
                           const int variant_offset) const;

        /**
         * Include another sprite into this one.
         */
        void includeSprite(XmlNodeConstPtr includeNode,
                           const int variant);

        const ImageSet *getImageSet(const std::string &imageSetName) const;

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
        void substituteAction(const std::string &restrict complete,
                              const std::string &restrict with);

        typedef std::map<std::string, ImageSet*> ImageSets;
        typedef ImageSets::iterator ImageSetIterator;
        typedef ImageSets::const_iterator ImageSetCIterator;
        typedef std::map<std::string, Action*> ActionMap;
        typedef std::map<unsigned, ActionMap*> Actions;
        typedef Actions::const_iterator ActionsConstIter;
        typedef Actions::iterator ActionsIter;
        typedef Actions::const_iterator ActionsCIter;

        ImageSets mImageSets;
        Actions mActions;
        std::set<std::string> mProcessedFiles;
};

#endif  // RESOURCES_SPRITE_SPRITEDEF_H
