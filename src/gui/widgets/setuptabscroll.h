/*
 *  The ManaPlus Client
 *  Copyright (C) 2011  The ManaPlus Developers
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

#ifndef GUI_SETUPTABSCROLL_H
#define GUI_SETUPTABSCROLL_H

#include "gui/widgets/setuptab.h"

#include <map>
#include <set>

#ifdef __GNUC__
#define A_UNUSED  __attribute__ ((unused))
#else
#define A_UNUSED
#endif

class SetupItem;
class ScrollArea;
class VertContainer;

class SetupTabScroll : public SetupTab
{
    public:
        SetupTabScroll();

        ~SetupTabScroll();

        void addControl(SetupItem *widget);

        void addControl(SetupItem *widget, std::string event);

        VertContainer *getContainer()
        { return mContainer; }

        virtual void apply();

        virtual void cancel();

        virtual void externalUpdated();

        virtual void action(const gcn::ActionEvent &event A_UNUSED)
        { }

    protected:
        VertContainer *mContainer;
        ScrollArea *mScroll;
        std::map<std::string, SetupItem*> mItems;
        std::set<SetupItem*> mAllItems;
};

#endif
