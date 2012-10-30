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

#ifndef BUTTON_H
#define BUTTON_H

#include "gui/widgets/widget2.h"

#include <guichan/widgets/button.hpp>
#include <guichan/mouseevent.hpp>
#include <guichan/widgetlistener.hpp>

#include "localconsts.h"

class GraphicsVertexes;
class Image;
class ImageSet;
class ImageRect;
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
        Button(const Widget2 *const widget);

        /**
         * Constructor, sets the caption of the button to the given string and
         * adds the given action listener.
         */
        Button(const Widget2 *const widget,
               const std::string &caption, const std::string &actionEventId,
               gcn::ActionListener *const listener);

        /**
         * Constructor, sets the caption of the button to the given string and
         * adds the given action listener.
         */
        Button(const Widget2 *const widget,
               const std::string &caption, const std::string &imageName,
               const int imageWidth, const int imageHeight,
               const std::string &actionEventId,
               gcn::ActionListener *const listener);

        /**
         * Constructor, sets the caption of the button to the given string and
         * adds the given action listener.
         */
        Button(const Widget2 *const widget, const std::string &imageName,
               const int imageWidth, const int imageHeight,
               const std::string &actionEventId,
               gcn::ActionListener *const listener);

        A_DELETE_COPY(Button)

        /**
         * Destructor.
         */
        ~Button();

        /**
         * Draws the button.
         */
        void draw(gcn::Graphics *graphics) override;

        /**
         * Update the alpha value to the button components.
         */
        void updateAlpha();

        virtual void mouseReleased(gcn::MouseEvent& mouseEvent) override;

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

        void widgetResized(const gcn::Event &event) override;

        void widgetMoved(const gcn::Event &event) override;

        void loadImage(const std::string &imageName);

        void adjustSize();

        void setCaption(const std::string& caption);

        void keyPressed(gcn::KeyEvent &keyEvent) override;

        void keyReleased(gcn::KeyEvent &keyEvent) override;

        bool isPressed2() const A_WARN_UNUSED;

    private:
        void init();

        static Skin *button[4];    /**< Button state graphics */
        static int mInstances;     /**< Number of button instances */
        static float mAlpha;

        std::string mDescription;
        unsigned mClickCount;
        int mTag;
        GraphicsVertexes *mVertexes;
        bool mRedraw;
        int mMode;
        int mXOffset;
        int mYOffset;
        gcn::Color mEnabledColor;
        gcn::Color mDisabledColor;
        gcn::Color mHighlightedColor;
        gcn::Color mPressedColor;
        Image **mImages;
        ImageSet *mImageSet;
        int mImageWidth;
        int mImageHeight;
        bool mStick;
        bool mPressed;
};

#endif
