/*
 *  An effects manager
 *  Copyright (C) 2008  Fate <fate.tmw@googlemail.com>
 *  Copyright (C) 2008  Chuck Miller <shadowmil@gmail.com>
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

#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H

#include <string>
#include <vector>

#include "localconsts.h"

class Being;
class Particle;

class EffectManager final
{
    public:
        struct EffectDescription
        {
            EffectDescription(const int id0, const std::string &gfx0,
                              const std::string &sfx0,
                              const std::string &sprite0) :
                id(id0),
                gfx(gfx0),
                sfx(sfx0),
                sprite(sprite0)
            {
            }
            int id;
            std::string gfx;
            std::string sfx;
            std::string sprite;
        };

        EffectManager();

        A_DELETE_COPY(EffectManager)

        ~EffectManager();

        /**
         * Triggers a effect with the id, at
         * the specified being.
         */
        bool trigger(const int id, Being *const being, const int rotation = 0);

        Particle *triggerReturn(const int id, Being *const being,
                                const int rotation = 0);

        /**
         * Triggers a effect with the id, at
         * the specified x and y coordinate.
         */
        bool trigger(const int id, const int x, const int y,
                     const int rotation = 0);

   private:
        std::vector<EffectDescription> mEffects;
};

extern EffectManager *effectManager;

#endif  // EFFECTMANAGER_H
