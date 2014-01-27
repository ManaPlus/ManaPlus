/*
 *  The ManaPlus Client
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

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef GCN_MOUSELISTENER_HPP
#define GCN_MOUSELISTENER_HPP

#include "guichan/mouseevent.hpp"

#include "localconsts.h"

namespace gcn
{
    /**
     * Interface for listening for mouse events from widgets.
     *
     * @see Widget::addMouseListener, Widget::removeMouseListener
     * @since 0.1.0
     */
    class MouseListener
    {
    public:
        /**
         * Destructor.
         */
        virtual ~MouseListener()
        { }

        /**
         * Called when the mouse has entered into the widget area.
         *
         * @param mouseEvent Describes the event.
         * @since 0.6.0
         */
        virtual void mouseEntered(MouseEvent& mouseEvent A_UNUSED)
        {
        }

        /**
         * Called when the mouse has exited the widget area.
         *
         * @param mouseEvent Describes the event.
         * @since 0.6.0
         */
        virtual void mouseExited(MouseEvent& mouseEvent A_UNUSED)
        {
        }

        /**
         * Called when a mouse button has been pressed on the widget area.
         *
         * NOTE: A mouse press is NOT equal to a mouse click.
         *       Use mouseClickMessage to check for mouse clicks.
         *
         * @param mouseEvent Describes the event.
         * @since 0.6.0
         */
        virtual void mousePressed(MouseEvent& mouseEvent A_UNUSED)
        {
        }

        /**
         * Called when a mouse button has been released on the widget area.
         *
         * @param mouseEvent Describes the event.
         * @since 0.6.0
         */
        virtual void mouseReleased(MouseEvent& mouseEvent A_UNUSED)
        {
        }

        /**
         * Called when a mouse button is pressed and released (clicked) on
         * the widget area.
         *
         * @param mouseEvent Describes the event.
         * @since 0.6.0
         */
        virtual void mouseClicked(MouseEvent& mouseEvent A_UNUSED)
        {
        }

        /**
         * Called when the mouse wheel has moved up on the widget area.
         *
         * @param mouseEvent Describes the event.
         * @since 0.6.0
         */
        virtual void mouseWheelMovedUp(MouseEvent& mouseEvent A_UNUSED)
        {
        }

        /**
         * Called when the mouse wheel has moved down on the widget area.
         *
         * @param mousEvent Describes the event.
         * @since 0.6.0
         */
        virtual void mouseWheelMovedDown(MouseEvent& mouseEvent A_UNUSED)
        {
        }

        /**
         * Called when the mouse has moved in the widget area and no mouse button
         * has been pressed (i.e no widget is being dragged).
         *
         * @param mouseEvent Describes the event.
         * @since 0.6.0
         */
        virtual void mouseMoved(MouseEvent& mouseEvent A_UNUSED)
        {
        }

        /**
         * Called when the mouse has moved and the mouse has previously been
         * pressed on the widget.
         *
         * @param mouseEvent Describes the event.
         * @since 0.6.0
         */
        virtual void mouseDragged(MouseEvent& mouseEvent A_UNUSED)
        {
        }

    protected:
        /**
         * Constructor.
         *
         * You should not be able to make an instance of MouseListener,
         * therefore its constructor is protected.
         */
        MouseListener()
        { }
    };
}  // namespace gcn

#endif  // end GCN_MOUSELISTENER_HPP
