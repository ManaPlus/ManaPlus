/*
 *  The ManaPlus Client
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

#ifndef GUI_WIDGETS_SETUPQUICKITEM_H
#define GUI_WIDGETS_SETUPQUICKITEM_H

#include "gui/widgets/setupitem.h"

#include "listeners/gamemodifierlistener.h"

class Button;
class HorizontContainer;
class Label;
class SetupTabScroll;

typedef std::string (*ModifierGetFuncPtr)();
typedef void (*ModifierChangeFuncPtr)(const bool forward);

class SetupQuickItem final : public SetupItem,
                             public GameModifierListener
{
    public:
        SetupQuickItem(const std::string &restrict description,
                       SetupTabScroll *restrict const parent,
                       const std::string &restrict eventName,
                       ModifierGetFuncPtr getFunc,
                       ModifierChangeFuncPtr changeFunc);

        A_DELETE_COPY(SetupQuickItem)

        ~SetupQuickItem() override final;

        void createControls();

        void fromWidget() override final;

        void toWidget() override final;

        void action(const ActionEvent &event) override final;

        void apply(const std::string &eventName) override final;

        void cancel(const std::string &eventName A_UNUSED) override final;

        void externalUpdated(const std::string &eventName A_UNUSED)
                             override final;

        void rereadValue() override final;

        void save() override final;

        void gameModifiersChanged() override final;

    protected:
        HorizontContainer *mHorizont;
        Button *mButton;
        Label *mLabel;
        ModifierGetFuncPtr mGetFunc;
        ModifierChangeFuncPtr mChangeFunc;
};

#endif  // GUI_WIDGETS_SETUPQUICKITEM_H
