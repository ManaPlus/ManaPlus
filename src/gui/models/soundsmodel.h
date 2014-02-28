/*
 *  The ManaPlus Client
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

#ifndef GUI_MODELS_SOUNDSMODEL_H
#define GUI_MODELS_SOUNDSMODEL_H

#include "gui/theme.h"

#include "gui/models/namesmodel.h"

#include "utils/gettext.h"

#include "localconsts.h"

class SoundsModel final : public NamesModel
{
    public:
        SoundsModel() :
            NamesModel()
        {
            mNames.push_back(gettext("(no sound)"));
            Theme::fillSoundsList(mNames);
        }

        ~SoundsModel()
        { }
};

#endif  // GUI_MODELS_SOUNDSMODEL_H
