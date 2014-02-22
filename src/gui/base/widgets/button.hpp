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

#ifndef GCN_BUTTON_HPP
#define GCN_BUTTON_HPP

#include <string>

#include "listeners/focuslistener.h"
#include "listeners/keylistener.h"
#include "listeners/mouselistener.h"

#include "gui/widgets/widget.h"

#include "render/graphics.h"

namespace gcn
{
    /**
     * An implementation of a regular clickable button. A button is capable of
     * displaying a caption. 
     *
     * If a button is clicked an action event will be sent to all action listener's 
     * of the button.
     *
     * @see ImageButton
     */
    class Button : public Widget,
                   public MouseListener,
                   public KeyListener,
                   public FocusListener
    {
    public:
        /**
         * Constructor.
         */
        explicit Button(const Widget2 *const widget);

        /**
         * Constructor. The button will be automatically resized
         * to fit the caption.
         *
         * @param caption The caption of the button.
         */
        Button(const Widget2 *const widget,
               const std::string& caption);

        A_DELETE_COPY(Button)

        /**
         * Sets the caption of the button. It's advisable to call
         * adjustSize after setting of the caption to adjust the
         * button's size to fit the caption.
         *
         * @param caption The caption of the button.
         * @see getCaption, adjustSize
         */
        void setCaption(const std::string& caption);

        /**
         * Gets the caption of the button.
         *
         * @return The caption of the button.
         */
        const std::string& getCaption() const;

        /**
         * Sets the alignment of the caption. The alignment is relative
         * to the center of the button.
         *
         * @param alignment The alignment of the caption.
         * @see getAlignment, Graphics
         */
        void setAlignment(Graphics::Alignment alignment);

        /**
         * Gets the alignment of the caption.
         *
         * @return The alignment of the caption.
         * @see setAlignment, Graphics
         */
        Graphics::Alignment getAlignment() const;

        /**
         * Sets the spacing between the border of the button and its caption.
         *
         * @param spacing The default value for spacing is 4 and can be changed 
         *                using this method.
         * @see getSpacing
         */
        void setSpacing(unsigned int spacing);

        /**
         * Gets the spacing between the border of the button and its caption.
         *
         * @return spacing.
         * @see setSpacing
         */
        unsigned int getSpacing() const;

        /**
         * Adjusts the button's size to fit the caption.
         */
        void adjustSize();


        // Inherited from FocusListener

        virtual void focusLost(const Event& event);

        // Inherited from MouseListener

        virtual void mousePressed(MouseEvent& mouseEvent) override;

        virtual void mouseEntered(MouseEvent& mouseEvent) override;

        virtual void mouseExited(MouseEvent& mouseEvent) override;

        virtual void mouseDragged(MouseEvent& mouseEvent) override;

    protected:
        /**
         * Checks if the button is pressed. Convenient method to use
         * when overloading the draw method of the button.
         *
         * @return True if the button is pressed, false otherwise.
         */
        bool isPressed() const;

        /**
         * Holds the caption of the button.
         */
        std::string mCaption;

        /**
         * True if the mouse is ontop of the button, false otherwise.
         */
        bool mHasMouse;

        /**
         * True if a key has been pressed, false otherwise.
         */
        bool mKeyPressed;

        /**
         * True if a mouse has been pressed, false otherwise.
         */
        bool mMousePressed;

        /**
         * Holds the alignment of the caption.
         */
        Graphics::Alignment mAlignment;

        /**
         * Holds the spacing between the border and the caption.
         */
        unsigned int mSpacing;
    };
}  // namespace gcn

#endif  // end GCN_BUTTON_HPP
