/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  Aethyra Development Team
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

#ifndef GUI_WIDGETS_EMOTESHORTCUTCONTAINER_H
#define GUI_WIDGETS_EMOTESHORTCUTCONTAINER_H

#include "gui/widgets/shortcutcontainer.h"

struct EmoteSprite;

/**
 * An emote shortcut container. Used to quickly use emoticons.
 *
 * \ingroup GUI
 */
class EmoteShortcutContainer final : public ShortcutContainer
{
    public:
        /**
         * Constructor. Initializes the graphic.
         */
        explicit EmoteShortcutContainer(Widget2 *restrict const widget);

        A_DELETE_COPY(EmoteShortcutContainer)

        /**
         * Destructor.
         */
        ~EmoteShortcutContainer();

        /**
         * Draws the items.
         */
        void draw(Graphics *restrict graphics)
                  restrict2 override final A_NONNULL(2);

        void safeDraw(Graphics *restrict graphics)
                      restrict2 override final A_NONNULL(2);

        /**
         * Handles mouse when dragged.
         */
        void mouseDragged(MouseEvent &restrict event)
                          restrict2 override final A_CONST;

        /**
         * Handles mouse when pressed.
         */
        void mousePressed(MouseEvent &restrict event) restrict2 override final;

        /**
         * Handles mouse release.
         */
        void mouseReleased(MouseEvent &restrict event)
                           restrict2 override final;

        void mouseMoved(MouseEvent &restrict event) restrict2 override final;

        void mouseExited(MouseEvent &restrict event) restrict2 override final;

        void widgetHidden(const Event &restrict event)
                          restrict2 override final;

        void setWidget2(const Widget2 *restrict const widget)
                        restrict2 override final;

    private:
        std::vector<const EmoteSprite*> mEmoteImg;

        bool mEmoteClicked;
        unsigned char mEmoteMoved;
};

#endif  // GUI_WIDGETS_EMOTESHORTCUTCONTAINER_H
