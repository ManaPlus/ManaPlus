/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Douglas Boffey <dougaboffey@netscape.net>
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef GUI_USERPALETTE_H
#define GUI_USERPALETTE_H

#include "gui/palette.h"

#include "gui/models/listmodel.h"

/**
 * Class controlling the game's color palette.
 */
class UserPalette final : public Palette, public ListModel
{
    public:
        /**
         * Constructor
         */
        UserPalette();

        A_DELETE_COPY(UserPalette)

        /**
         * Destructor
         */
        ~UserPalette();

        /**
         * Gets the committed color associated with the specified type.
         *
         * @param type the color type requested
         *
         * @return the requested committed color
         */
        inline const Color &getCommittedColor(const int type)
                                              const A_WARN_UNUSED
        {
            return mColors[type].committedColor;
        }

        /**
         * Gets the test color associated with the specified type.
         *
         * @param type the color type requested
         *
         * @return the requested test color
         */
        inline const Color &getTestColor(const int type) const A_WARN_UNUSED
        { return mColors[type].testColor; }

        /**
         * Sets the test color associated with the specified type.
         *
         * @param type the color type requested
         * @param color the color that should be tested
         */
        inline void setTestColor(const int type, const Color &color)
        { mColors[type].testColor = color; }

        /**
         * Sets the color for the specified type.
         *
         * @param type color to be set
         * @param r red component
         * @param g green component
         * @param b blue component
         */
        void setColor(const int type, const int r, const int g, const int b);

        /**
         * Sets the gradient type for the specified color.
         *
         * @param grad gradient type to set
         */
        void setGradient(const int type, const GradientTypeT grad);

        /**
         * Sets the gradient delay for the specified color.
         *
         * @param grad gradient type to set
         */
        void setGradientDelay(const int type, const int delay)
        { mColors[type].delay = delay; }

        /**
         * Returns the number of colors known.
         *
         * @return the number of colors known
         */
        inline int getNumberOfElements() override final A_WARN_UNUSED
        { return static_cast<int>(mColors.size()); }

        /**
         * Returns the name of the ith color.
         *
         * @param i index of color interested in
         *
         * @return the name of the color
         */
        std::string getElementAt(int i) override final A_WARN_UNUSED;

        /**
         * Commit the colors
         */
        inline void commit()
        { commit(false); }

        /**
         * Rollback the colors
         */
        void rollback();

        /**
         * Gets the ColorType used by the color for the element at index i in
         * the current color model.
         *
         * @param i the index of the color
         *
         * @return the color type of the color with the given index
         */
        int getColorTypeAt(const int i) A_WARN_UNUSED;

        /**
         * Gets the color associated with the type. Sets the alpha channel
         * before returning.
         *
         * @param type the color type requested
         * @param alpha alpha channel to use
         *
         * @return the requested color
         */
        inline const Color &getColor(int type,
                                     const int alpha = 255) A_WARN_UNUSED
        {
            if (type >= static_cast<signed>(mColors.size()) || type < 0)
            {
                logger->log("incorrect color request type: %d from %u",
                    type, static_cast<unsigned int>(mColors.size()));
                type = 0;
            }
            Color* col = &mColors[type].color;
            col->a = alpha;
            return *col;
        }

        int getIdByChar(const signed char c, bool &valid) const A_WARN_UNUSED;

        /**
         * Gets the GradientType associated with the specified type.
         *
         * @param type the color type of the color
         *
         * @return the gradient type of the color with the given index
         */
        inline GradientTypeT getGradientType(const int type) const A_WARN_UNUSED
        { return mColors[type].grad; }

        /**
         * Gets the gradient delay for the specified type.
         *
         * @param type the color type of the color
         *
         * @return the gradient delay of the color with the given index
         */
        inline int getGradientDelay(const int type) const A_WARN_UNUSED
        { return mColors[type].delay; }

        inline const Color &getColorWithAlpha(const int type) A_WARN_UNUSED
        {
            Color *const col = &mColors[type].color;
            col->a = mColors[type].delay;
            return *col;
        }

    private:
        /**
         * Define a color replacement.
         *
         * @param i the index of the color to replace
         * @param r red component
         * @param g green component
         * @param b blue component
         */
        void setColorAt(int i, int r, int g, int b);

        /**
         * Commit the colors. Commit the non-static color values, if
         * commitNonStatic is true. Only needed in the constructor.
         */
        void commit(const bool commitNonStatic);

        /**
         * Prefixes the given string with "Color", lowercases all letters but
         * the first and all following a '_'. All '_'s will be removed.
         *
         * E.g.: HIT_PLAYER_MONSTER -> HitPlayerMonster
         *
         * @param typeName string to transform
         *
         * @return the transformed string
         */
        static std::string getConfigName(const std::string &typeName)
                                         A_WARN_UNUSED;

        /**
         * Initialise color
         *
         * @param c character that needs initialising
         * @param rgb default color if not found in config
         * @param text identifier of color
         */
        void addColor(const unsigned type, const unsigned rgb,
                      GradientTypeT grad, const std::string &text,
                      int delay = GRADIENT_DELAY);
};

extern UserPalette *userPalette;

#endif  // GUI_USERPALETTE_H
