/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  Andrei Karas
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

#ifndef GUI_WIDGETS_TABS_SETUP_VISUAL_H
#define GUI_WIDGETS_TABS_SETUP_VISUAL_H

#include "gui/widgets/setupitem.h"

class NamesModel;

class Setup_Visual final : public SetupTabScroll
{
    public:
        explicit Setup_Visual(const Widget2 *const widget);

        A_DELETE_COPY(Setup_Visual)

        ~Setup_Visual();

        void apply() override final;

    private:
        NamesModel *mSpeachList;
        NamesModel *mAmbientFxList;
        SetupItemNames *mParticleList;
        NamesModel *mParticleTypeList;
        NamesModel *mVSyncList;
        NamesModel *mScaleList;
};

#endif  // GUI_WIDGETS_TABS_SETUP_VISUAL_H
