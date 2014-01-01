/*
 *  The ManaPlus Client
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

#ifndef GUI_WIDGETS_MOUSEEVENT_H
#define GUI_WIDGETS_MOUSEEVENT_H

#include <guichan/mouseevent.hpp>
#include <guichan/widget.hpp>

class MouseEvent final : public gcn::MouseEvent
{
    public:
        MouseEvent(gcn::Widget* source, bool shiftPressed,
                   bool controlPressed, bool altPressed,
                   bool metaPressed, unsigned int type, unsigned int button,
                   int x, int y, int clickCount) :
            gcn::MouseEvent(source, shiftPressed, controlPressed,
                            altPressed, metaPressed, type, button, x, y,
                            clickCount)
        {
        }

        void setX(int n)
        { mX = n; }

        void setY(int n)
        { mY = n; }
};

#endif  // GUI_WIDGETS_MOUSEEVENT_H
