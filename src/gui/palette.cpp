/*
 *  Configurable text colors
 *  Copyright (C) 2008  Douglas Boffey <dougaboffey@netscape.net>
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *
 *  This file is part of The Mana Client.
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

#include "palette.h"

#include "configuration.h"
#include "client.h"

#include "gui/gui.h"

#include "utils/gettext.h"
#include "utils/mathutils.h"
#include "utils/stringutils.h"

#include <math.h>

const gcn::Color Palette::BLACK = gcn::Color(0, 0, 0);
Palette::Palettes Palette::mInstances;

const gcn::Color Palette::RAINBOW_COLORS[7] = {
    gcn::Color(255, 0, 0),
    gcn::Color(255, 153, 0),
    gcn::Color(255, 255, 0),
    gcn::Color(0, 153, 0),
    gcn::Color(0, 204, 204),
    gcn::Color(51, 0, 153),
    gcn::Color(153, 0, 153)
};
/** Number of Elemets of RAINBOW_COLORS */
const int Palette::RAINBOW_COLOR_COUNT = 7;

Palette::Palette(int size) :
    mRainbowTime(tick_time),
    mColors(Colors(size))
{
    mInstances.insert(this);
}

Palette::~Palette()
{
    mInstances.erase(this);
}

const gcn::Color& Palette::getColor(char c, bool &valid)
{
    for (Colors::const_iterator col = mColors.begin(),
         colEnd = mColors.end(); col != colEnd; ++col)
    {
        if (col->ch == c)
        {
            valid = true;
            return col->color;
        }
    }
    valid = false;
    return BLACK;
}

void Palette::advanceGradients()
{
    Palettes::iterator it = mInstances.begin();
    Palettes::iterator it_end = mInstances.end();

    for (; it != it_end; it++)
        (*it)->advanceGradient();
}

void Palette::advanceGradient()
{
    if (get_elapsed_time(mRainbowTime) > 5)
    {
        int pos, colIndex, colVal, delay, numOfColors;
        // For slower systems, advance can be greater than one (advance > 1
        // skips advance-1 steps). Should make gradient look the same
        // independent of the framerate.
        int advance = get_elapsed_time(mRainbowTime) / 5;
        double startColVal, destColVal;

        for (size_t i = 0; i < mGradVector.size(); i++)
        {
            if (!mGradVector[i])
                continue;

            delay = mGradVector[i]->delay;

            if (mGradVector[i]->grad == PULSE)
                delay = delay / 20;

            numOfColors = (mGradVector[i]->grad == SPECTRUM ? 6 :
                           mGradVector[i]->grad == PULSE ? 127 :
                           RAINBOW_COLOR_COUNT);

            mGradVector[i]->gradientIndex =
                                    (mGradVector[i]->gradientIndex + advance) %
                                    (delay * numOfColors);

            pos = mGradVector[i]->gradientIndex % delay;
            if (delay)
                colIndex = mGradVector[i]->gradientIndex / delay;
            else
                colIndex = mGradVector[i]->gradientIndex;

            if (mGradVector[i]->grad == PULSE)
            {
                colVal = static_cast<int>(255.0 *
                        sin(M_PI * colIndex / numOfColors));

                const gcn::Color &col = mGradVector[i]->testColor;

                mGradVector[i]->color.r =
                    ((colVal * col.r) / 255) % (col.r + 1);
                mGradVector[i]->color.g =
                    ((colVal * col.g) / 255) % (col.g + 1);
                mGradVector[i]->color.b =
                    ((colVal * col.b) / 255) % (col.b + 1);
            }
            if (mGradVector[i]->grad == SPECTRUM)
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

                mGradVector[i]->color.r =
                        (colIndex == 0 || colIndex == 5) ? 255 :
                        (colIndex == 1 || colIndex == 4) ? colVal : 0;
                mGradVector[i]->color.g =
                        (colIndex == 1 || colIndex == 2) ? 255 :
                        (colIndex == 0 || colIndex == 3) ? colVal : 0;
                mGradVector[i]->color.b =
                        (colIndex == 3 || colIndex == 4) ? 255 :
                        (colIndex == 2 || colIndex == 5) ? colVal : 0;
            }
            else if (mGradVector[i]->grad == RAINBOW)
            {
                const gcn::Color &startCol = RAINBOW_COLORS[colIndex];
                const gcn::Color &destCol =
                        RAINBOW_COLORS[(colIndex + 1) % numOfColors];

                if (delay)
                    startColVal = (cos(M_PI * pos / delay) + 1) / 2;
                else
                    startColVal = 0;

                destColVal = 1 - startColVal;

                mGradVector[i]->color.r = static_cast<int>(startColVal
                        * startCol.r + destColVal * destCol.r);

                mGradVector[i]->color.g = static_cast<int>(startColVal
                        * startCol.g + destColVal * destCol.g);

                mGradVector[i]->color.b = static_cast<int>(startColVal
                        * startCol.b + destColVal * destCol.b);
            }
        }

        if (advance)
            mRainbowTime = tick_time;
    }
}

/*
gcn::Color Palette::produceHPColor(int hp, int maxHp, int alpha)
{
    float r1 = 255;
    float g1 = 255;
    float b1 = 255;
    float r2 = 255;
    float g2 = 255;
    float b2 = 255;

    float weight = 1.0f;

    int thresholdLevel = maxHp / 4;
    int thresholdProgress = hp % thresholdLevel;

    if (thresholdLevel)
        weight = 1 - ((float)thresholdProgress) / ((float)thresholdLevel);
    else
        weight = 0;

    if (hp < (thresholdLevel))
    {
        gcn::Color color1 = guiPalette->getColor(Palette::HPBAR_ONE_HALF);
        gcn::Color color2 = guiPalette->getColor(Palette::HPBAR_ONE_QUARTER);
        r1 = color1.r; r2 = color2.r;
        g1 = color1.g; g2 = color2.g;
        b1 = color1.b; b2 = color2.b;
    }
    else if (hp < (thresholdLevel*2))
    {
        gcn::Color color1 = guiPalette->getColor(Palette::HPBAR_THREE_QUARTERS);
        gcn::Color color2 = guiPalette->getColor(Palette::HPBAR_ONE_HALF);
        r1 = color1.r; r2 = color2.r;
        g1 = color1.g; g2 = color2.g;
        b1 = color1.b; b2 = color2.b;
    }
    else if (hp < thresholdLevel*3)
    {
        gcn::Color color1 = guiPalette->getColor(Palette::HPBAR_FULL);
        gcn::Color color2 = guiPalette->getColor(Palette::HPBAR_THREE_QUARTERS);
        r1 = color1.r; r2 = color2.r;
        g1 = color1.g; g2 = color2.g;
        b1 = color1.b; b2 = color2.b;
    }
    else
    {
       gcn::Color color1 = guiPalette->getColor(Palette::HPBAR_FULL);
       gcn::Color color2 = guiPalette->getColor(Palette::HPBAR_FULL);
       r1 = color1.r; r2 = color2.r;
       g1 = color1.g; g2 = color2.g;
       b1 = color1.b; b2 = color2.b;
    }

    // Safety checks
    if (weight > 1.0f) weight = 1.0f;
    if (weight < 0.0f) weight = 0.0f;

    // Do the color blend
    r1 = (int) weightedAverage(r1, r2,weight);
    g1 = (int) weightedAverage(g1, g2, weight);
    b1 = (int) weightedAverage(b1, b2, weight);

    // More safety checks
    if (r1 > 255) r1 = 255;
    if (g1 > 255) g1 = 255;
    if (b1 > 255) b1 = 255;

    return gcn::Color(r1, g1, b1, alpha);
}
*/

