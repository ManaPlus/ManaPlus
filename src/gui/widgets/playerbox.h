/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef PLAYERBOX_H
#define PLAYERBOX_H

#include <guichan/widgets/scrollarea.hpp>

#include "localconsts.h"

class Being;
class ImageRect;

/**
 * A box showing a player character.
 *
 * \ingroup GUI
 */
class PlayerBox : public gcn::ScrollArea
{
    public:
        /**
         * Constructor. Takes the initial player character that this box should
         * display, which defaults to <code>NULL</code>.
         */
        PlayerBox(Being *being = nullptr);

        /**
         * Destructor.
         */
        ~PlayerBox();

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
        void draw(gcn::Graphics *graphics);

        /**
         * Draws the background and border of the scroll area.
         */
        void drawFrame(gcn::Graphics *graphics);

        Being *getBeing()
        { return mBeing; }

    private:
        Being *mBeing; /**< The character used for display */

        static float mAlpha;
        static int instances;
        static ImageRect background;
};

#endif
