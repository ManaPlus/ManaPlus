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

#include "particle/textparticle.h"

#include "render/graphics.h"

#include "gui/theme.h"

#include <guichan/color.hpp>
#include <guichan/font.hpp>

#include "debug.h"

TextParticle::TextParticle(const std::string &text,
                           const gcn::Color *const color,
                           gcn::Font *const font, const bool outline) :
    Particle(),
    mText(text),
    mTextFont(font),
    mColor(color),
    mTextWidth(mTextFont ? mTextFont->getWidth(mText) / 2 : 1),
    mOutline(outline)
{
}

void TextParticle::draw(Graphics *const graphics,
                        const int offsetX, const int offsetY) const
{
    if (!mColor || !mTextFont)
        return;

    BLOCK_START("TextParticle::draw")
    if (!isAlive())
    {
        BLOCK_END("TextParticle::draw")
        return;
    }

    const int screenX = static_cast<int>(mPos.x) + offsetX;
    const int screenY = static_cast<int>(mPos.y) - static_cast<int>(mPos.z)
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

    gcn::Color color = *mColor;
    color.a = static_cast<int>(alpha);

    graphics->setColor(color);
    if (mOutline)
    {
        graphics->setColor2(Theme::getThemeColor(
            Theme::OUTLINE, static_cast<int>(alpha)));
    }
    mTextFont->drawString(graphics, mText, screenX - mTextWidth, screenY);
    BLOCK_END("TextParticle::draw")
    return;
}
