/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef GUI_WIDGETS_TABS_SETUPTAB_H
#define GUI_WIDGETS_TABS_SETUPTAB_H

#include "gui/widgets/container.h"

#include "listeners/actionlistener.h"
#include "listeners/widgetlistener.h"

#include "localconsts.h"

/**
 * A container for the contents of a tab in the setup window.
 */
class SetupTab notfinal : public Container,
                          public ActionListener,
                          public WidgetListener
{
    public:
        A_DELETE_COPY(SetupTab)

        const std::string &getName() const noexcept2 A_WARN_UNUSED
        { return mName; }

        /**
         * Called when the Apply button is pressed in the setup window.
         */
        virtual void apply() = 0;

        /**
         * Called when the Cancel button is pressed in the setup window.
         */
        virtual void cancel() = 0;

        virtual void externalUpdated();

        virtual void externalUnloaded();

    protected:
        explicit SetupTab(const Widget2 *const widget);

        /**
         * Sets the name displayed on the tab. Should be set in the
         * constructor of a subclass.
         */
        void setName(const std::string &name) noexcept2
        { mName = name; }

    private:
        std::string mName;
};

#endif  // GUI_WIDGETS_TABS_SETUPTAB_H
