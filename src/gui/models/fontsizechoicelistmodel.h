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

#ifndef GUI_MODELS_FONTSIZECHOICELISTMODEL_H
#define GUI_MODELS_FONTSIZECHOICELISTMODEL_H

#include "gui/models/listmodel.h"

#include "utils/gettext.h"

#include "localconsts.h"

const int maxFontSizes = 16;

const char *SIZE_NAME[maxFontSizes] =
{
    // TRANSLATORS: font size
    N_("Very small (8)"),
    // TRANSLATORS: font size
    N_("Very small (9)"),
    // TRANSLATORS: font size
    N_("Tiny (10)"),
    // TRANSLATORS: font size
    N_("Small (11)"),
    // TRANSLATORS: font size
    N_("Medium (12)"),
    // TRANSLATORS: font size
    N_("Normal (13)"),
    // TRANSLATORS: font size
    N_("Large (14)"),
    // TRANSLATORS: font size
    N_("Large (15)"),
    // TRANSLATORS: font size
    N_("Large (16)"),
    // TRANSLATORS: font size
    N_("Big (17)"),
    // TRANSLATORS: font size
    N_("Big (18)"),
    // TRANSLATORS: font size
    N_("Big (19)"),
    // TRANSLATORS: font size
    N_("Very big (20)"),
    // TRANSLATORS: font size
    N_("Very big (21)"),
    // TRANSLATORS: font size
    N_("Very big (22)"),
    // TRANSLATORS: font size
    N_("Huge (23)")
};

class FontSizeChoiceListModel final : public ListModel
{
    public:
        ~FontSizeChoiceListModel()
        { }

        int getNumberOfElements() override final A_WARN_UNUSED
        { return maxFontSizes; }

        std::string getElementAt(int i) override final A_WARN_UNUSED
        {
            if (i >= getNumberOfElements() || i < 0)
                return "???";

            return gettext(SIZE_NAME[i]);
        }
};

#endif  // GUI_MODELS_FONTSIZECHOICELISTMODEL_H
