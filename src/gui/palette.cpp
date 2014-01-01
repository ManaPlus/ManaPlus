/*
 *  Configurable text colors
 *  Copyright (C) 2008  Douglas Boffey <dougaboffey@netscape.net>
 *  Copyright (C) 2009  The Mana World Development Team
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

#include "gui/palette.h"

#include "utils/timer.h"

#include <cmath>

#include "debug.h"

const gcn::Color Palette::BLACK = gcn::Color(0, 0, 0);
Palette::Palettes Palette::mInstances;

const gcn::Color Palette::RAINBOW_COLORS[7] =
{
    gcn::Color(255, 0, 0),
    gcn::Color(255, 153, 0),
    gcn::Color(255, 255, 0),
    gcn::Color(0, 153, 0),
    gcn::Color(0, 204, 204),
    gcn::Color(51, 0, 153),
    gcn::Color(153, 0, 153)
};

const int Palette::RAINBOW_COLOR_COUNT = 7;

Palette::Palette(const int size) :
    mRainbowTime(tick_time),
    mColors(Colors(size)),
    mCharColors(),
    mGradVector()
{
    mInstances.insert(this);
}

Palette::~Palette()
{
    mInstances.erase(this);
}

const gcn::Color& Palette::getCharColor(const signed char c, bool &valid) const
{
    const CharColors::const_iterator it = mCharColors.find(c);
    if (it != mCharColors.end())
    {
        valid = true;
        return mColors[(*it).second].color;
    }

    valid = false;
    return BLACK;
}

int Palette::getIdByChar(const signed char c, bool &valid) const
{
    const CharColors::const_iterator it = mCharColors.find(c);
    if (it != mCharColors.end())
    {
        valid = true;
        return (*it).second;
    }

    valid = false;
    return 0;
}

void Palette::advanceGradients()
{
    FOR_EACH (Palettes::const_iterator, it, mInstances)
        (*it)->advanceGradient();
}

void Palette::advanceGradient()
{
    const int time = get_elapsed_time(mRainbowTime);
    if (time > 5)
    {
        // For slower systems, advance can be greater than one (advance > 1
        // skips advance-1 steps). Should make gradient look the same
        // independent of the framerate.
        const int advance = time / 5;

        for (size_t i = 0, sz = mGradVector.size(); i < sz; i++)
        {
            ColorElem *const elem = mGradVector[i];
            if (!elem)
                continue;

            int delay = elem->delay;
            const GradientType &grad = elem->grad;

            if (grad == PULSE)
                delay = delay / 20;

            const int numOfColors = (elem->grad == SPECTRUM ? 6 :
                grad == PULSE ? 127 :
                RAINBOW_COLOR_COUNT);

            elem->gradientIndex = (elem->gradientIndex + advance)
                % (delay * numOfColors);

            const int gradIndex = elem->gradientIndex;
            const int pos = delay ? (gradIndex % delay) : gradIndex;
            int colIndex;
            if (delay)
                colIndex = gradIndex / delay;
            else
                colIndex = gradIndex;

            gcn::Color &color = elem->color;
            int colVal;

            if (grad == PULSE)
            {
                colVal = static_cast<int>(255.0 *
                        sin(M_PI * colIndex / numOfColors));

                const gcn::Color &col = elem->testColor;

                color.r = ((colVal * col.r) / 255) % (col.r + 1);
                color.g = ((colVal * col.g) / 255) % (col.g + 1);
                color.b = ((colVal * col.b) / 255) % (col.b + 1);
            }
            else if (grad == SPECTRUM)
            {
                if (colIndex % 2)
                { // falling curve
                    if (delay)
                    {
                        colVal = static_cast<int>(255.0 *
                             (cos(M_PI * pos / delay) + 1) / 2);
                    }
                    else
                    {
                        colVal = static_cast<int>(255.0 *
                             (cos(M_PI * pos) + 1) / 2);
                    }
                }
                else
                { // ascending curve
                    if (delay)
                    {
                        colVal = static_cast<int>(255.0 * (cos(M_PI *
                            (delay - pos) / delay) + 1) / 2);
                    }
                    else
                    {
                        colVal = static_cast<int>(255.0 * (cos(M_PI *
                            (delay - pos)) + 1) / 2);
                    }
                }

                color.r = (colIndex == 0 || colIndex == 5) ? 255 :
                    (colIndex == 1 || colIndex == 4) ? colVal : 0;
                color.g = (colIndex == 1 || colIndex == 2) ? 255 :
                    (colIndex == 0 || colIndex == 3) ? colVal : 0;
                color.b = (colIndex == 3 || colIndex == 4) ? 255 :
                    (colIndex == 2 || colIndex == 5) ? colVal : 0;
            }
            else if (elem->grad == RAINBOW)
            {
                const gcn::Color &startCol = RAINBOW_COLORS[colIndex];
                const gcn::Color &destCol =
                        RAINBOW_COLORS[(colIndex + 1) % numOfColors];
                double startColVal;
                double destColVal;

                if (delay)
                    startColVal = (cos(M_PI * pos / delay) + 1) / 2;
                else
                    startColVal = 0;

                destColVal = 1 - startColVal;

                color.r = static_cast<int>(startColVal
                    * startCol.r + destColVal * destCol.r);

                color.g = static_cast<int>(startColVal
                    * startCol.g + destColVal * destCol.g);

                color.b = static_cast<int>(startColVal
                    * startCol.b + destColVal * destCol.b);
            }
        }

        if (advance)
            mRainbowTime = tick_time;
    }
}
