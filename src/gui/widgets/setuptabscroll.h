/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "localconsts.h"

class SetupItem;
class ScrollArea;
class VertContainer;

class SetupTabScroll : public SetupTab
{
    public:
        SetupTabScroll(const Widget2 *const widget);

        A_DELETE_COPY(SetupTabScroll)

        ~SetupTabScroll();

        void addControl(SetupItem *const widget);

        void addControl(SetupItem *const widget, const std::string &event);

        VertContainer *getContainer() const
        { return mContainer; }

        virtual void apply() override;

        virtual void cancel() override;

        virtual void externalUpdated() override;

        virtual void action(const gcn::ActionEvent &event A_UNUSED) override
        { }

        int getPreferredFirstItemSize()
        { return mPreferredFirstItemSize; }

    protected:
        VertContainer *mContainer;
        ScrollArea *mScroll;
        std::map<std::string, SetupItem*> mItems;
        std::set<SetupItem*> mAllItems;
        int mPreferredFirstItemSize;
};

#endif
