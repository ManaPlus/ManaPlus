/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef GUI_WIDGETS_PLAYERBOX_H
#define GUI_WIDGETS_PLAYERBOX_H

#include "gui/widgets/widget.h"

#include "listeners/mouselistener.h"

#include "resources/imagerect.h"

#include "localconsts.h"

class Being;
class Skin;

/**
 * A box showing a player character.
 *
 * \ingroup GUI
 */
class PlayerBox final : public Widget,
                        public MouseListener
{
    public:
        /**
         * Constructor. Takes the initial player character that this box should
         * display, which defaults to <code>NULL</code>.
         */
        PlayerBox(Widget2 *const widget,
                  Being *const being,
                  const std::string &skin,
                  const std::string &selectedSkin);

        PlayerBox(Widget2 *const widget,
                  const std::string &skin,
                  const std::string &selectedSkin);

        A_DELETE_COPY(PlayerBox)

        /**
         * Destructor.
         */
        ~PlayerBox() override final;

        void init(std::string name, std::string selectedName);

        /**
         * Sets a new player character to be displayed by this box. Setting the
         * player to <code>NULL</code> causes the box not to draw any
         * character.
         */
        void setPlayer(Being *being)
        { mBeing = being; }

        /**
         * Draws the scroll area.
         */
        void draw(Graphics *const graphics) override final A_NONNULL(2);

        void safeDraw(Graphics *const graphics) override final A_NONNULL(2);

        /**
         * Draws the background and border of the scroll area.
         */
        void drawFrame(Graphics *const graphics) override final A_NONNULL(2);

        void safeDrawFrame(Graphics *const graphics) override final
                           A_NONNULL(2);

        Being *getBeing() A_WARN_UNUSED
        { return mBeing; }

        void setSelected(bool b)
        { mSelected = b; }

        void mouseReleased(MouseEvent& event) override final;

    private:
        Being *mBeing;
        float mAlpha;
        ImageRect mBackground;
        ImageRect mSelectedBackground;
        Skin *mSkin;
        Skin *mSelectedSkin;
        int mOffsetX;
        int mOffsetY;
        bool mDrawBackground;
        bool mSelected;
};

#endif  // GUI_WIDGETS_PLAYERBOX_H
