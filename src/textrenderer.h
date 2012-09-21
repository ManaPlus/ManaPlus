/*
 *  Text Renderer
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

#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "gui/theme.h"

/**
 * Class for text rendering. Used by the TextParticle, the Text and FlashText
 * objects and the Preview in the color dialog.
 */
class TextRenderer final
{
    public:
    /**
     * Renders a specified text.
     */
    static inline void renderText(gcn::Graphics *const graphics,
                                  const std::string &text,
                                  const int x, const int y,
                                  const gcn::Graphics::Alignment align,
                                  const gcn::Color &color,
                                  gcn::Font *const font,
                                  const bool outline = false,
                                  const bool shadow = false,
                                  const int alpha = 255)
    {
        graphics->setFont(font);

        // Text shadow
        if (shadow)
        {
            graphics->setColor(Theme::getThemeColor(Theme::SHADOW,
                                                    color.a / 2));
            if (outline)
                graphics->drawText(text, x + 2, y + 2, align);
            else
                graphics->drawText(text, x + 1, y + 1, align);
        }

        if (outline)
        {
            // Text outline
            graphics->setColor(Theme::getThemeColor(Theme::OUTLINE, alpha));
//            graphics->setColor(Theme::getThemeColor(Theme::OUTLINE, color.a));
            graphics->drawText(text, x + 1, y, align);
            graphics->drawText(text, x - 1, y, align);
            graphics->drawText(text, x, y + 1, align);
            graphics->drawText(text, x, y - 1, align);
        }

        graphics->setColor(color);
        graphics->drawText(text, x, y, align);
    }
};

#endif
