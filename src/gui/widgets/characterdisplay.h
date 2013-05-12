/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#ifndef WIDGETS_CHARACTER_DISPLAY_H
#define WIDGETS_CHARACTER_DISPLAY_H

#include "gui/widgets/container.h"
#include "gui/widgets/playerbox.h"

#include "net/charserverhandler.h"
#include "net/net.h"

#include <guichan/widgetlistener.hpp>

class Button;
class CharSelectDialog;
class Label;
class PlayerBox;
class TextPopup;

class CharacterDisplay final : public Container,
                               public gcn::MouseListener,
                               public gcn::WidgetListener
{
    public:
        CharacterDisplay(const Widget2 *const widget,
                         CharSelectDialog *const charSelectDialog);

        A_DELETE_COPY(CharacterDisplay)

        virtual ~CharacterDisplay();

        void setCharacter(Net::Character *const character);

        Net::Character *getCharacter() const
        { return mCharacter; }

        void requestFocus();

        void setActive(const bool active);

        bool isSelectFocused() const
        { return false; }

        bool isDeleteFocused() const
        { return false; }

        void focusSelect()
        { }

        void focusDelete()
        { }

        void setSelect(bool b)
        { mPlayerBox->setSelected(b); }

        void widgetHidden(const gcn::Event &event) override;

        void mouseExited(gcn::MouseEvent &event) override;

        void mouseMoved(gcn::MouseEvent &event) override;

    private:
        void update();

        Net::Character *mCharacter;
        PlayerBox *mPlayerBox;
        Label *mName;
        TextPopup *mPopup;
};

#endif
