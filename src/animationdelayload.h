/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#ifndef ANIMATIONDELAYLOAD_H
#define ANIMATIONDELAYLOAD_H

#include <string>

#include "localconsts.h"

class AnimatedSprite;

class AnimationDelayLoad final
{
    public:
        AnimationDelayLoad(const std::string &fileName,
                           const int variant, AnimatedSprite *const sprite);

        A_DELETE_COPY(AnimationDelayLoad)

        ~AnimationDelayLoad();

        void clearSprite();

        void load();

        void setAction(std::string action)
        { mAction = action; }

    private:
        std::string mFileName;
        int mVariant;
        AnimatedSprite *mSprite;
        std::string mAction;
};

#endif  // ANIMATIONDELAYLOAD_H
