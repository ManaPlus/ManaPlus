/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_SPELLSHORTCUTCONTAINER_H
#define GUI_WIDGETS_SPELLSHORTCUTCONTAINER_H

#include "gui/widgets/shortcutcontainer.h"

class SpellPopup;

/**
 * An item shortcut container. Used to quickly use items.
 *
 * \ingroup GUI
 */
class SpellShortcutContainer final : public ShortcutContainer
{
    public:
        /**
         * Constructor. Initializes the graphic.
         */
        explicit SpellShortcutContainer(const unsigned number);

        A_DELETE_COPY(SpellShortcutContainer)

        /**
         * Destructor.
         */
        ~SpellShortcutContainer();

        /**
         * Draws the items.
         */
        void draw(gcn::Graphics *graphics) override final;

        /**
         * Handles mouse when dragged.
         */
        void mouseDragged(gcn::MouseEvent &event) override final;

        /**
         * Handles mouse when pressed.
         */
        void mousePressed(gcn::MouseEvent &event) override final;

        /**
         * Handles mouse release.
         */
        void mouseReleased(gcn::MouseEvent &event) override final;

        void widgetHidden(const gcn::Event &event) override final;

        void mouseExited(gcn::MouseEvent &event) override final;

        void mouseMoved(gcn::MouseEvent &event) override final;

        void setWidget2(const Widget2 *const widget) override final;

        int getItemByIndex(const int index) const A_WARN_UNUSED;

    private:
        SpellPopup *mSpellPopup;
        unsigned int mNumber;
        bool mSpellClicked;
};

#endif  // GUI_WIDGETS_SPELLSHORTCUTCONTAINER_H
