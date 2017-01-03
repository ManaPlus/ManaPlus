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

#include "particle/textparticle.h"

#include "gui/theme.h"

#include "gui/fonts/font.h"

#include "render/graphics.h"

#include "debug.h"

TextParticle::TextParticle(const std::string &restrict text,
                           const Color *restrict const color,
                           Font *restrict const font,
                           const bool outline) :
    Particle(),
    mText(text),
    mTextFont(font),
    mColor(color),
    mTextWidth(mTextFont ? mTextFont->getWidth(mText) / 2 : 1),
    mOutline(outline)
{
    mType = ParticleType::Text;
}

void TextParticle::draw(Graphics *restrict const graphics,
                        const int offsetX,
                        const int offsetY) const restrict2
{
    BLOCK_START("TextParticle::draw")
    if (!isAlive())
    {
        BLOCK_END("TextParticle::draw")
        return;
    }

    const int screenX = CAST_S32(mPos.x) + offsetX;
    const int screenY = CAST_S32(mPos.y) - CAST_S32(mPos.z)
        + offsetY;

    float alpha = mAlpha * 255.0F;

    if (mFadeOut && mLifetimeLeft > -1 && mLifetimeLeft < mFadeOut)
    {
        alpha *= static_cast<float>(mLifetimeLeft)
                / static_cast<float>(mFadeOut);
    }

    if (mFadeIn && mLifetimePast < mFadeIn)
    {
        alpha *= static_cast<float>(mLifetimePast)
                / static_cast<float>(mFadeIn);
    }

    Color color = *mColor;
    color.a = CAST_U32(alpha);

    graphics->setColor(color);
    if (mOutline)
    {
        const Color &restrict color2 = theme->getColor(ThemeColorId::OUTLINE,
            CAST_S32(alpha));
        mTextFont->drawString(graphics,
            color, color2,
            mText,
            screenX - mTextWidth, screenY);
    }
    else
    {
        mTextFont->drawString(graphics,
            color, color,
            mText, screenX - mTextWidth, screenY);
    }
    BLOCK_END("TextParticle::draw")
    return;
}
