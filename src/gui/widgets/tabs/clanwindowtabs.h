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

#ifndef GUI_WIDGETS_TABS_CLANWINDOWTABS_H
#define GUI_WIDGETS_TABS_CLANWINDOWTABS_H

#include "gui/widgets/container.h"

class Label;

class InfoClanTab final : public Container
{
    public:
        explicit InfoClanTab(const Widget2 *const widget);

        A_DELETE_COPY(InfoClanTab)

        void resetClan();

        void updateClan();

    private:
        Label *mNameLabel A_NONNULLPOINTER;
        Label *mMasterLabel A_NONNULLPOINTER;
        Label *mMapLabel A_NONNULLPOINTER;
};

class StatsClanTab final : public Container
{
    public:
        explicit StatsClanTab(const Widget2 *const widget);

        A_DELETE_COPY(StatsClanTab)

        void clearLabels();

        void resetClan();

        void updateClan();

    private:
        STD_VECTOR<Label*> mLabels;
};

class RelationClanTab final : public Container
{
    public:
        explicit RelationClanTab(const Widget2 *const widget);

        A_DELETE_COPY(RelationClanTab)

        void clearLabels();

        void resetClan();

        void updateClan(const STD_VECTOR<std::string> &restrict names);

    private:
        STD_VECTOR<Label*> mLabels;
};

#endif  // GUI_WIDGETS_TABS_CLANWINDOWTABS_H
