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

#ifndef GCN_CHECKBOX_HPP
#define GCN_CHECKBOX_HPP

#include <string>

#include "listeners/keylistener.h"
#include "listeners/mouselistener.h"

#include "gui/widgets/widget.h"

namespace gcn
{
    /**
     * An implementation of a check box where a user can select or deselect
     * the check box and where the status of the check box is displayed to the user.
     * A check box is capable of displaying a caption. 
     * 
     * If a check box's state changes an action event will be sent to all action 
     * listeners of the check box.
     */
    class CheckBox :
        public Widget,
        public MouseListener,
        public KeyListener
    {
    public:
        /**
         * Contructor.
         */
        explicit CheckBox(const Widget2 *const widget);

        /**
         * Constructor. The check box will be automatically resized
         * to fit the caption.
         *
         * @param caption The caption of the check box.
         * @param marked True if the check box is selected, false otherwise.
         */
        CheckBox(const Widget2 *const widget,
                 const std::string &caption,
                 bool selected = false);

        A_DELETE_COPY(CheckBox)

        /**
         * Destructor.
         */
        virtual ~CheckBox()
        { }

        /**
         * Checks if the check box is selected.
         *
         * @return True if the check box is selected, false otherwise.
         * @see setSelected
         */
        bool isSelected() const;

        /**
         * Sets the check box to be selected or not.
         *
         * @param selected True if the check box should be set as selected.
         * @see isSelected
         */
        void setSelected(bool selected);

        /**
         * Gets the caption of the check box.
         *
         * @return The caption of the check box.
         * @see setCaption
         */
        const std::string &getCaption() const;

        /**
         * Sets the caption of the check box. It's advisable to call
         * adjustSize after setting of the caption to adjust the
         * check box's size to fit the caption.
         *
         * @param caption The caption of the check box.
         * @see getCaption, adjustSize
         */
        void setCaption(const std::string& caption);

        /**
         * Adjusts the check box's size to fit the caption.
         */
        void adjustSize();

        // Inherited from KeyListener

        virtual void keyPressed(KeyEvent& keyEvent) override;

        // Inherited from MouseListener

        virtual void mouseClicked(MouseEvent& mouseEvent) override;

        virtual void mouseDragged(MouseEvent& mouseEvent) override;


    protected:
        /**
         * Toggles the check box between being selected and
         * not being selected.
         */
        virtual void toggleSelected();

        /**
         * True if the check box is selected, false otherwise.
         */
        bool mSelected;

        /**
         * Holds the caption of the check box.
         */
        std::string mCaption;
    };
}  // namespace gcn

#endif  // end GCN_CHECKBOX_HPP
