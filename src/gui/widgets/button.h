/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef GUI_WIDGETS_BUTTON_H
#define GUI_WIDGETS_BUTTON_H

#include "gui/widgets/widget.h"

#include "listeners/focuslistener.h"
#include "listeners/keylistener.h"
#include "listeners/mouselistener.h"
#include "listeners/widgetlistener.h"

#include "localconsts.h"

class Image;
class ImageCollection;
class ImageSet;
class Skin;

const std::string BUTTON_PLAY = "buttonplay.png";

/**
 * Button widget. Same as the Guichan button but with custom look.
 *
 * \ingroup GUI
 */
class Button final : public Widget,
                     public MouseListener,
                     public KeyListener,
                     public FocusListener,
                     public WidgetListener
{
    public:
        /**
         * Default constructor.
         */
        explicit Button(const Widget2 *const widget);

        /**
         * Constructor, sets the caption of the button to the given string and
         * adds the given action listener.
         */
        Button(const Widget2 *const widget,
               const std::string &restrict caption,
               const std::string &restrict actionEventId,
               ActionListener *const listener);

        /**
         * Constructor, sets the caption of the button to the given string and
         * adds the given action listener.
         */
        Button(const Widget2 *const widget,
               const std::string &restrict caption,
               const std::string &restrict imageName,
               const int imageWidth, const int imageHeight,
               const std::string &actionEventId,
               ActionListener *const listener);

        /**
         * Constructor, sets the caption of the button to the given string and
         * adds the given action listener.
         */
        Button(const Widget2 *const widget,
               const std::string &restrict imageName,
               const int imageWidth, const int imageHeight,
               const std::string &restrict actionEventId,
               ActionListener *const listener);

        /**
         * Constructor, sets the caption of the button to the given string and
         * adds the given action listener.
         */
        Button(const Widget2 *const widget,
               const std::string &restrict imageName,
               const std::string &restrict caption,
               const std::string &restrict actionEventId,
               ActionListener *const listener);

        A_DELETE_COPY(Button)

        /**
         * Destructor.
         */
        ~Button();

        /**
         * Draws the button.
         */
        void draw(Graphics *graphics) override final;

        /**
         * Update the alpha value to the button components.
         */
        static void updateAlpha();

        void mouseReleased(MouseEvent& event) override final;

        void setDescription(std::string text)
        { mDescription = text; }

        std::string getDescription() const A_WARN_UNUSED
        { return mDescription; }

        unsigned getClickCount() const A_WARN_UNUSED
        { return mClickCount; }

        void setTag(int tag)
        { mTag = tag; }

        int getTag() const A_WARN_UNUSED
        { return mTag; }

        void setStick(bool b)
        { mStick = b; }

        void setPressed(bool b)
        { mPressed = b; }

        void widgetResized(const Event &event) override final;

        void widgetMoved(const Event &event) override final;

        void loadImage(const std::string &imageName);

        void loadImageSet(const std::string &imageName);

        void adjustSize();

        void keyPressed(KeyEvent &event) override final;

        void keyReleased(KeyEvent &event) override final;

        bool isPressed2() const A_WARN_UNUSED;

        /**
         * Sets the caption of the button. It's advisable to call
         * adjustSize after setting of the caption to adjust the
         * button's size to fit the caption.
         *
         * @param caption The caption of the button.
         * @see getCaption, adjustSize
         */
        void setCaption(const std::string& caption)
        { mCaption = caption; }

        /**
         * Gets the caption of the button.
         *
         * @return The caption of the button.
         */
        const std::string& getCaption() const
        { return mCaption; }

        /**
         * Sets the alignment of the caption. The alignment is relative
         * to the center of the button.
         *
         * @param alignment The alignment of the caption.
         * @see getAlignment, Graphics
         */
        void setAlignment(Graphics::Alignment alignment)
        { mAlignment = alignment; }

        /**
         * Gets the alignment of the caption.
         *
         * @return The alignment of the caption.
         * @see setAlignment, Graphics
         */
        Graphics::Alignment getAlignment() const
        { return mAlignment; }

        /**
         * Sets the spacing between the border of the button and its caption.
         *
         * @param spacing The default value for spacing is 4 and can be changed 
         *                using this method.
         * @see getSpacing
         */
        void setSpacing(unsigned int spacing)
        { mSpacing = spacing; }

        /**
         * Gets the spacing between the border of the button and its caption.
         *
         * @return spacing.
         * @see setSpacing
         */
        unsigned int getSpacing() const
        { return mSpacing; }

        void focusLost(const Event& event) override final;

        void mousePressed(MouseEvent& event) override final;

        void mouseEntered(MouseEvent& event) override final;

        void mouseExited(MouseEvent& event) override final;

        void mouseDragged(MouseEvent& event) override final;

        enum
        {
            BUTTON_STANDARD = 0,  // 0
            BUTTON_HIGHLIGHTED,   // 1
            BUTTON_PRESSED,       // 2
            BUTTON_DISABLED,      // 3
            BUTTON_COUNT          // 4 - Must be last.
        };

    private:
        /**
         * Checks if the button is pressed. Convenient method to use
         * when overloading the draw method of the button.
         *
         * @return True if the button is pressed, false otherwise.
         */
        bool isPressed() const;

        void init();

        static Skin *button[BUTTON_COUNT];  /**< Button state graphics */
        static int mInstances;              /**< Number of button instances */
        static float mAlpha;

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

        std::string mDescription;
        ImageCollection *mVertexes2;
        Color mEnabledColor;
        Color mEnabledColor2;
        Color mDisabledColor;
        Color mDisabledColor2;
        Color mHighlightedColor;
        Color mHighlightedColor2;
        Color mPressedColor;
        Color mPressedColor2;
        Image **mImages;
        ImageSet *mImageSet;
        unsigned mClickCount;
        int mTag;
        int mMode;
        int mXOffset;
        int mYOffset;
        int mImageWidth;
        int mImageHeight;
        bool mRedraw;
        bool mStick;
        bool mPressed;
};

#endif  // GUI_WIDGETS_BUTTON_H
