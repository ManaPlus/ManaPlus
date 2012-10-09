/*
 *  Configurable text colors
 *  Copyright (C) 2008  Douglas Boffey <dougaboffey@netscape.net>
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef PALETTE_H
#define PALETTE_H

#include "logger.h"
#include "utils/stringutils.h"

#include <guichan/color.hpp>

#include <cstdlib>
#include <map>
#include <string>
#include <set>
#include <vector>

// Default Gradient Delay
#define GRADIENT_DELAY 40

/**
 * Class controlling the game's color palette.
 */
class Palette
{
    public:
        /** Black Color Constant */
        static const gcn::Color BLACK;

        /** Colors can be static or can alter over time. */
        enum GradientType
        {
            STATIC = 0,
            PULSE,
            SPECTRUM,
            RAINBOW
        };

        A_DELETE_COPY(Palette);

        /**
         * Returns the color associated with a character, if it exists. Returns
         * Palette::BLACK if the character is not found.
         *
         * @param c character requested
         * @param valid indicate whether character is known
         *
         * @return the requested color or Palette::BLACK
         */
        const gcn::Color &getColor(const char c, bool &valid);

        /**
         * Gets the color associated with the type. Sets the alpha channel
         * before returning.
         *
         * @param type the color type requested
         * @param alpha alpha channel to use
         *
         * @return the requested color
         */
        inline const gcn::Color &getColor(int type, int alpha = 255)
        {
            if (type >= static_cast<signed>(mColors.size()))
            {
                logger->log("incorrect color request type: %d from %u",
                    type, static_cast<unsigned int>(mColors.size()));
                type = 0;
            }
            gcn::Color* col = &mColors[type].color;
            col->a = alpha;
            return *col;
        }

        inline const gcn::Color &getColorWithAlpha(int type)
        {
            gcn::Color* col = &mColors[type].color;
            col->a = mColors[type].delay;
            return *col;
        }

        /**
         * Gets the GradientType associated with the specified type.
         *
         * @param type the color type of the color
         *
         * @return the gradient type of the color with the given index
         */
        inline GradientType getGradientType(int type) const
        { return mColors[type].grad; }

        /**
         * Get the character used by the specified color.
         *
         * @param type the color type of the color
         *
         * @return the color char of the color with the given index
         */
        inline char getColorChar(int type) const
        { return mColors[type].ch; }

        /**
         * Gets the gradient delay for the specified type.
         *
         * @param type the color type of the color
         *
         * @return the gradient delay of the color with the given index
         */
        inline int getGradientDelay(int type) const
        { return mColors[type].delay; }

        /**
         * Updates all colors, that are non-static.
         */
        static void advanceGradients();

        static gcn::Color produceHPColor(int hp, int maxHp, int alpha = 255);

    protected:
        /** Colors used for the rainbow gradient */
        static const gcn::Color RAINBOW_COLORS[];
        static const int RAINBOW_COLOR_COUNT;

        /** Time tick, that gradient-type colors were updated the last time. */
        int mRainbowTime;

        typedef std::set<Palette*> Palettes;
        static Palettes mInstances;

        /**
         * Constructor
         */
        Palette(const int size);

        /**
         * Destructor
         */
        ~Palette();

        void advanceGradient();

        struct ColorElem
        {
            ColorElem() :
                type(0),
                color(0),
                testColor(0),
                committedColor(0),
                text(),
                ch(0),
                grad(STATIC),
                committedGrad(STATIC),
                gradientIndex(0),
                delay(0),
                committedDelay(0)
            {
            }

            int type;
            gcn::Color color;
            gcn::Color testColor;
            gcn::Color committedColor;
            std::string text;
            char ch;
            GradientType grad;
            GradientType committedGrad;
            int gradientIndex;
            int delay;
            int committedDelay;

            void set(const int type0, const gcn::Color &color0,
                     const GradientType grad0, const int delay0)
            {
                type = type0;
                color = color0;
                testColor = color0;
                grad = grad0;
                delay = delay0;
                gradientIndex = rand();
            }

            inline int getRGB() const
            {
                return (committedColor.r << 16) | (committedColor.g << 8) |
                        committedColor.b;
            }
        };
        typedef std::vector<ColorElem> Colors;
        typedef std::map<unsigned char, int> CharColors;
        /** Vector containing the colors. */
        Colors mColors;
        CharColors mCharColors;
        std::vector<ColorElem*> mGradVector;
};

#endif
