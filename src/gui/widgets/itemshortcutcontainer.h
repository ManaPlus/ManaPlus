/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
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

#ifndef GUI_WIDGETS_ITEMSHORTCUTCONTAINER_H
#define GUI_WIDGETS_ITEMSHORTCUTCONTAINER_H

#include "gui/widgets/shortcutcontainer.h"

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
        ItemShortcutContainer(Widget2 *const widget,
                              const unsigned number);

        A_DELETE_COPY(ItemShortcutContainer)

        /**
         * Destructor.
         */
        ~ItemShortcutContainer() override final;

        /**
         * Draws the items.
         */
        void draw(Graphics *const graphics) override final A_NONNULL(2);

        void safeDraw(Graphics *const graphics) override final A_NONNULL(2);

        /**
         * Handles mouse when dragged.
         */
        void mouseDragged(MouseEvent &event) override final;

        /**
         * Handles mouse when pressed.
         */
        void mousePressed(MouseEvent &event) override final;

        /**
         * Handles mouse release.
         */
        void mouseReleased(MouseEvent &event) override final;

        void widgetHidden(const Event &event) override final;

        void mouseExited(MouseEvent &event) override final;

        void mouseMoved(MouseEvent &event) override final;

        void setSkin(const Widget2 *const widget,
                     Skin *const skin) override final;

    private:
        Color mEquipedColor;
        Color mEquipedColor2;
        Color mUnEquipedColor;
        Color mUnEquipedColor2;
        unsigned mNumber;
        int mKeyOffsetX;
        int mKeyOffsetY;
        bool mItemClicked;
};

#endif  // GUI_WIDGETS_ITEMSHORTCUTCONTAINER_H
