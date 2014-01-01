/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef GUI_WIDGETS_ITEMSHORTCUTCONTAINER_H
#define GUI_WIDGETS_ITEMSHORTCUTCONTAINER_H

#include "gui/widgets/shortcutcontainer.h"

class ItemPopup;
class SpellPopup;

/**
 * An item shortcut container. Used to quickly use items.
 *
 * \ingroup GUI
 */
class ItemShortcutContainer final : public ShortcutContainer
{
    public:
        /**
         * Constructor. Initializes the graphic.
         */
        explicit ItemShortcutContainer(const unsigned number);

        A_DELETE_COPY(ItemShortcutContainer)

        /**
         * Destructor.
         */
        ~ItemShortcutContainer();

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

        void setWidget2(const Widget2 *const widget);

    private:
        bool mItemClicked;
        unsigned mNumber;

        ItemPopup *mItemPopup;
        SpellPopup *mSpellPopup;
        gcn::Color mEquipedColor;
        gcn::Color mEquipedColor2;
        gcn::Color mUnEquipedColor;
        gcn::Color mUnEquipedColor2;
};

#endif  // GUI_WIDGETS_ITEMSHORTCUTCONTAINER_H
