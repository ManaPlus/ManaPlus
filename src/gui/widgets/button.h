/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_BUTTON_H
#define GUI_WIDGETS_BUTTON_H

#include "gui/widgets/widget2.h"

#include <guichan/widgets/button.hpp>
#include <guichan/widgetlistener.hpp>

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
class Button final : public gcn::Button,
                     public Widget2,
                     public gcn::WidgetListener
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
               gcn::ActionListener *const listener);

        /**
         * Constructor, sets the caption of the button to the given string and
         * adds the given action listener.
         */
        Button(const Widget2 *const widget,
               const std::string &restrict caption,
               const std::string &restrict imageName,
               const int imageWidth, const int imageHeight,
               const std::string &actionEventId,
               gcn::ActionListener *const listener);

        /**
         * Constructor, sets the caption of the button to the given string and
         * adds the given action listener.
         */
        Button(const Widget2 *const widget,
               const std::string &restrict imageName,
               const int imageWidth, const int imageHeight,
               const std::string &restrict actionEventId,
               gcn::ActionListener *const listener);

        /**
         * Constructor, sets the caption of the button to the given string and
         * adds the given action listener.
         */
        Button(const Widget2 *const widget,
               const std::string &restrict imageName,
               const std::string &restrict caption,
               const std::string &restrict actionEventId,
               gcn::ActionListener *const listener);

        A_DELETE_COPY(Button)

        /**
         * Destructor.
         */
        ~Button();

        /**
         * Draws the button.
         */
        void draw(gcn::Graphics *graphics) override final;

        /**
         * Update the alpha value to the button components.
         */
        void updateAlpha();

        void mouseReleased(gcn::MouseEvent& mouseEvent) override final;

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

        void widgetResized(const gcn::Event &event) override final;

        void widgetMoved(const gcn::Event &event) override final;

        void loadImage(const std::string &imageName);

        void loadImageSet(const std::string &imageName);

        void adjustSize();

        void setCaption(const std::string& caption);

        void keyPressed(gcn::KeyEvent &keyEvent) override final;

        void keyReleased(gcn::KeyEvent &keyEvent) override final;

        bool isPressed2() const A_WARN_UNUSED;

        enum
        {
            BUTTON_STANDARD = 0,  // 0
            BUTTON_HIGHLIGHTED,   // 1
            BUTTON_PRESSED,       // 2
            BUTTON_DISABLED,      // 3
            BUTTON_COUNT          // 4 - Must be last.
        };

    private:
        void init();

        static Skin *button[BUTTON_COUNT];  /**< Button state graphics */
        static int mInstances;              /**< Number of button instances */
        static float mAlpha;

        std::string mDescription;
        ImageCollection *mVertexes2;
        gcn::Color mEnabledColor;
        gcn::Color mEnabledColor2;
        gcn::Color mDisabledColor;
        gcn::Color mDisabledColor2;
        gcn::Color mHighlightedColor;
        gcn::Color mHighlightedColor2;
        gcn::Color mPressedColor;
        gcn::Color mPressedColor2;
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
