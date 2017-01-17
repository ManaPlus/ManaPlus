/*
 *  The ManaPlus Client
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

#ifndef GUI_THEMEINFO_H
#define GUI_THEMEINFO_H

#include <string>

#include "localconsts.h"

struct ThemeInfo final
{
    ThemeInfo() :
        name(),
        copyright(),
        font(),
        boldFont(),
        particleFont(),
        helpFont(),
        secureFont(),
        npcFont(),
        japanFont(),
        chinaFont(),
        fontSize(0),
        npcfontSize(0),
        guiAlpha(0.0F)
    {
    }

    A_DELETE_COPY(ThemeInfo)

    std::string name;
    std::string copyright;
    std::string font;
    std::string boldFont;
    std::string particleFont;
    std::string helpFont;
    std::string secureFont;
    std::string npcFont;
    std::string japanFont;
    std::string chinaFont;
    int fontSize;
    int npcfontSize;
    float guiAlpha;
};

#endif  // GUI_THEMEINFO_H
