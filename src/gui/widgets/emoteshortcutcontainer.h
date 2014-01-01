/*
 *  Extended support for activating emotes
 *  Copyright (C) 2009  Aethyra Development Team
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

#ifndef GUI_WIDGETS_EMOTESHORTCUTCONTAINER_H
#define GUI_WIDGETS_EMOTESHORTCUTCONTAINER_H

#include "gui/widgets/shortcutcontainer.h"

#include "resources/db/emotedb.h"

#include <vector>

class TextPopup;

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
        EmoteShortcutContainer();

        A_DELETE_COPY(EmoteShortcutContainer)

        /**
         * Destructor.
         */
        ~EmoteShortcutContainer();

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

        void mouseMoved(gcn::MouseEvent &event) override final;

        void mouseExited(gcn::MouseEvent &event) override final;

        void widgetHidden(const gcn::Event &event) override final;

        void setWidget2(const Widget2 *const widget) override final;

    private:
        std::vector<const EmoteSprite*> mEmoteImg;

        TextPopup *mEmotePopup;
        bool mEmoteClicked;
        unsigned char mEmoteMoved;
};

#endif  // GUI_WIDGETS_EMOTESHORTCUTCONTAINER_H
