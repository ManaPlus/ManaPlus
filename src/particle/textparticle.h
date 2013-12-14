/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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
                     const gcn::Color *const color,
                     gcn::Font *const font, const bool outline = false);

        A_DELETE_COPY(TextParticle)

        /**
         * Draws the particle image.
         */
        void draw(Graphics *const graphics,
                  const int offsetX, const int offsetY) const override final;

        // hack to improve text visibility
        int getPixelY() const override final A_WARN_UNUSED
        { return static_cast<int>(mPos.y + mPos.z); }

        // hack to improve text visibility (for sorting only)
        int getSortPixelY() const override final A_WARN_UNUSED
        { return static_cast<int>(mPos.y + mPos.z); }

    private:
        std::string mText;             /**< Text of the particle. */
        gcn::Font *mTextFont;          /**< Font used for drawing the text. */
        const gcn::Color *mColor;      /**< Color used for drawing the text. */
        int mTextWidth;
        bool mOutline;                 /**< Make the text better readable */
};

#endif  // PARTICLE_TEXTPARTICLE_H
