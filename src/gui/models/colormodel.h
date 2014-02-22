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

#ifndef GUI_MODELS_COLORMODEL_H
#define GUI_MODELS_COLORMODEL_H

#include "utils/stringvector.h"

#include "gui/models/listmodel.h"

#include "localconsts.h"

class Color;
class Widget2;

struct ColorPair
{
    ColorPair(const Color* c1, const Color* c2) :
        color1(c1),
        color2(c2)
    {
    }

    const Color* color1;
    const Color* color2;
};

class ColorModel : public ListModel
{
    public:
        ColorModel();

        A_DELETE_COPY(ColorModel)

        virtual ~ColorModel();

        virtual int getNumberOfElements() override final A_WARN_UNUSED;

        virtual std::string getElementAt(int i) override final A_WARN_UNUSED;

        virtual const ColorPair *getColorAt(const int i) const A_WARN_UNUSED;

        StringVect &getNames() A_WARN_UNUSED
        { return mNames; }

        size_t size() A_WARN_UNUSED
        { return mNames.size(); }

        void add(const std::string &name, const Color *const color1,
                 const Color *const color2);

        static ColorModel *createDefault(const Widget2 *const widget);

    protected:
        StringVect mNames;
        std::vector<ColorPair> mColors;
};

#endif  // GUI_MODELS_COLORMODEL_H
