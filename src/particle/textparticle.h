/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef PARTICLE_TEXTPARTICLE_H
#define PARTICLE_TEXTPARTICLE_H

#include "particle/particle.h"

class TextParticle final : public Particle
{
    public:
        /**
         * Constructor.
         */
        TextParticle(const std::string &text,
                     const Color *restrict const color,
                     Font *restrict const font,
                     const bool outline = false) A_NONNULL(3, 4);

        A_DELETE_COPY(TextParticle)

        /**
         * Draws the particle image.
         */
        void draw(Graphics *restrict const graphics,
                  const int offsetX,
                  const int offsetY) const
                  restrict2 override final A_NONNULL(2);

        // hack to improve text visibility
        int getPixelY() const restrict2 override final A_WARN_UNUSED
        { return CAST_S32(mPos.y + mPos.z); }

        // hack to improve text visibility (for sorting only)
        int getSortPixelY() const restrict2 override final A_WARN_UNUSED
        { return CAST_S32(mPos.y + mPos.z); }

    private:
        /**< Text of the particle. */
        std::string mText;
        /**< Font used for drawing the text. */
        Font *restrict mTextFont A_NONNULLPOINTER;
        /**< Color used for drawing the text. */
        const Color *restrict mColor A_NONNULLPOINTER;
        int mTextWidth;
        /**< Make the text better readable */
        bool mOutline;
};

#endif  // PARTICLE_TEXTPARTICLE_H
