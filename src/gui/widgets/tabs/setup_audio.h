/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef GUI_WIDGETS_TABS_SETUP_AUDIO_H
#define GUI_WIDGETS_TABS_SETUP_AUDIO_H

#include "gui/widgets/setupitem.h"

#include "gui/widgets/tabs/setuptabscroll.h"

class Setup_Audio final : public SetupTabScroll
{
    public:
        explicit Setup_Audio(const Widget2 *const widget);

        A_DELETE_COPY(Setup_Audio)

        ~Setup_Audio();

        void apply() override final;

    private:
        ListModel *mSoundModel;

        SetupItemNames *mChannelsList;
};

#endif  // GUI_WIDGETS_TABS_SETUP_AUDIO_H
