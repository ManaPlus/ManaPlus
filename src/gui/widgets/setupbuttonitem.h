/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_SETUPBUTTONITEM_H
#define GUI_WIDGETS_SETUPBUTTONITEM_H

#include "gui/widgets/setupitem.h"

class Button;
class HorizontContainer;
class SetupTabScroll;

class SetupButtonItem final : public SetupItem
{
    public:
        SetupButtonItem(const std::string &restrict text,
                        const std::string &restrict description,
                        const std::string &restrict actionEventId,
                        SetupTabScroll *restrict const parent,
                        const std::string &restrict eventName,
                        ActionListener *const listener);

        A_DELETE_COPY(SetupButtonItem)

        ~SetupButtonItem();

        void createControls();

        void fromWidget() final A_CONST;

        void toWidget() final A_CONST;

        void action(const ActionEvent &event) final A_CONST;

        void apply(const std::string &eventName) final A_CONST;

        void cancel(const std::string &eventName A_UNUSED)
                    final A_CONST;

        void externalUpdated(const std::string &eventName A_UNUSED)
                             final A_CONST;

        void rereadValue() final A_CONST;

        void save() final A_CONST;

    protected:
        HorizontContainer *mHorizont;
        Button *mButton;
};

#endif  // GUI_WIDGETS_SETUPBUTTONITEM_H
