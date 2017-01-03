/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

        void fromWidget() override final A_CONST;

        void toWidget() override final A_CONST;

        void action(const ActionEvent &event) override final A_CONST;

        void apply(const std::string &eventName) override final A_CONST;

        void cancel(const std::string &eventName A_UNUSED)
                    override final A_CONST;

        void externalUpdated(const std::string &eventName A_UNUSED)
                             override final A_CONST;

        void rereadValue() override final A_CONST;

        void save() override final A_CONST;

    protected:
        HorizontContainer *mHorizont;
        Button *mButton;
};

#endif  // GUI_WIDGETS_SETUPBUTTONITEM_H
