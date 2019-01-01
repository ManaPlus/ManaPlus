/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_STATSPAGEBASIC_H
#define GUI_WIDGETS_STATSPAGEBASIC_H

#include "gui/widgets/attrs/attrdisplay.h"

#include "listeners/attributelistener.h"
#include "listeners/statlistener.h"

#include "localconsts.h"

class Label;
class ScrollArea;
class VertContainer;

class StatsPageBasic final : public Container,
                             public WidgetListener,
                             public AttributeListener,
                             public StatListener
{
    public:
        explicit StatsPageBasic(const Widget2 *const widget);

        A_DELETE_COPY(StatsPageBasic)

        void widgetResized(const Event &event) override final;

        void attributeChanged(const AttributesT id,
                              const int64_t oldVal,
                              const int64_t newVal) override final;

        void statChanged(const AttributesT id,
                         const int oldVal1,
                         const int oldVal2) override final;

        void setPointsNeeded(const AttributesT id,
                             const int needed);

        std::string getStatsStr() const;

    private:
        Attrs mAttrs;
        VertContainer *mAttrCont A_NONNULLPOINTER;
        ScrollArea *mAttrScroll A_NONNULLPOINTER;
        Label *mCharacterPointsLabel A_NONNULLPOINTER;
};

#endif  // GUI_WIDGETS_STATSPAGEBASIC_H
