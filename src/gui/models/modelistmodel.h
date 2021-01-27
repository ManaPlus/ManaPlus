/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef GUI_MODELS_MODELISTMODEL_H
#define GUI_MODELS_MODELISTMODEL_H

#include "gui/models/listmodel.h"

#include "utils/cast.h"
#include "utils/stringvector.h"

#include "localconsts.h"

class ModeListModel final : public ListModel
{
    public:
        ModeListModel();

        A_DELETE_COPY(ModeListModel)

        /**
         * Returns the number of elements in container.
         */
        int getNumberOfElements() override final
        { return CAST_S32(mVideoModes.size()); }

        /**
         * Returns element from container.
         */
        std::string getElementAt(int i) override final
        { return mVideoModes[CAST_SIZE(i)]; }

        /**
         * Returns the index corresponding to the given video mode.
         * E.g.: "800x600".
         * or -1 if not found.
         */
        int getIndexOf(const std::string &widthXHeightMode);

    private:
#ifndef ANDROID
        void addCustomMode(const std::string &mode);
#endif  // ANDROID

        StringVect mVideoModes;
};

#endif  // GUI_MODELS_MODELISTMODEL_H
