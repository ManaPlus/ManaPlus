/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2018  The ManaPlus Developers
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

#ifndef GUI_MODELS_COLORMODEL_H
#define GUI_MODELS_COLORMODEL_H

#include "utils/stringvector.h"

#include "gui/colorpair.h"

#include "gui/models/listmodel.h"

#include "localconsts.h"

class Color;
class Widget2;

class ColorModel final : public ListModel
{
    public:
        ColorModel();

        A_DELETE_COPY(ColorModel)

        ~ColorModel() override final;

        int getNumberOfElements() override final A_WARN_UNUSED;

        std::string getElementAt(int i) override final A_WARN_UNUSED;

        const ColorPair *getColorAt(const int i)
                                    const RETURNS_NONNULL A_WARN_UNUSED;

        StringVect &getNames() A_WARN_UNUSED
        { return mNames; }

        size_t size() A_WARN_UNUSED
        { return mNames.size(); }

        void add(const std::string &name,
                 const Color *const color1,
                 const Color *const color2);

        static ColorModel *createDefault(const Widget2 *const widget)
                                         RETURNS_NONNULL;

    protected:
        StringVect mNames;
        STD_VECTOR<ColorPair> mColors;
};

#endif  // GUI_MODELS_COLORMODEL_H
