/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_TABS_NETDEBUGTAB_H
#define GUI_WIDGETS_TABS_NETDEBUGTAB_H

#include "gui/widgets/tabs/debugtab.h"

class Label;

class NetDebugTab final : public DebugTab
{
    friend class DebugWindow;

    public:
        explicit NetDebugTab(const Widget2 *const widget);

        A_DELETE_COPY(NetDebugTab)

        void logic() override final;

    private:
        Label *mPingLabel A_NONNULLPOINTER;
        Label *mInPackets1Label A_NONNULLPOINTER;
        Label *mOutPackets1Label A_NONNULLPOINTER;
};

#endif  // GUI_WIDGETS_TABS_NETDEBUGTAB_H
