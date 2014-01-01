/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
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

#ifndef PARTICLE_PARTICLEEMITTERPROP_H
#define PARTICLE_PARTICLEEMITTERPROP_H

#include <cmath>
#include <cstdlib>

#include "localconsts.h"

/**
 * Returns a random numeric value that is larger than or equal min and smaller
 * than max
 */

enum ChangeFunc
{
    FUNC_NONE = 0,
    FUNC_SINE,
    FUNC_SAW,
    FUNC_TRIANGLE,
    FUNC_SQUARE
};

template <typename T> struct ParticleEmitterProp final
{
    ParticleEmitterProp():
        minVal(0), maxVal(0), changeFunc(FUNC_NONE),
        changeAmplitude(0), changePeriod(0), changePhase(0)
    {
    }

    void set(const T min, const T max)
    {
        minVal = min;
        maxVal = max;
    }

    void set(const T val)
    {
        set(val, val);
    }

    void setFunction(ChangeFunc func, T amplitude,
                     const int period, const int phase)
    {
        changeFunc = func;
        changeAmplitude = amplitude;
        changePeriod = period;
        if (!changePeriod)
            changePeriod = 1;
        changePhase = phase;
    }

    T value(int tick) const
    {
        tick += changePhase;
        T val = static_cast<T>(minVal + (maxVal - minVal)
            * (rand() / (static_cast<double>(RAND_MAX) + 1)));

        switch (changeFunc)
        {
            case FUNC_SINE:
                val += static_cast<T>(std::sin(M_PI * 2 * (static_cast<double>(
                    tick % changePeriod) / static_cast<double>(
                    changePeriod)))) * changeAmplitude;
                break;
            case FUNC_SAW:
                val += static_cast<T>(changeAmplitude * (static_cast<double>(
                    tick % changePeriod) / static_cast<double>(
                    changePeriod))) * 2 - changeAmplitude;
                break;
            case FUNC_TRIANGLE:
                if ((tick % changePeriod) * 2 < changePeriod)
                {
                    val += changeAmplitude - static_cast<T>((
                        tick % changePeriod) / static_cast<double>(
                        changePeriod)) * changeAmplitude * 4;
                }
                else
                {
                    val += changeAmplitude * -3 + static_cast<T>((
                        tick % changePeriod) / static_cast<double>(
                        changePeriod)) * changeAmplitude * 4;
                    // I have no idea why this works but it does
                }
                break;
            case FUNC_SQUARE:
                if ((tick % changePeriod) * 2 < changePeriod)
                    val += changeAmplitude;
                else
                    val -= changeAmplitude;
                break;
            case FUNC_NONE:
            default:
                // nothing
                break;
        }

        return val;
    }

    T minVal;
    T maxVal;

    ChangeFunc changeFunc;
    T changeAmplitude;
    int changePeriod;
    int changePhase;
};

#endif  // PARTICLE_PARTICLEEMITTERPROP_H
