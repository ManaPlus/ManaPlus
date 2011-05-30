/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "guichan/widgets/button.hpp"
#include "guichan/mouseevent.hpp"
#include "guichan/widgetlistener.hpp"

class GraphicsVertexes;
class ImageRect;

/**
 * Button widget. Same as the Guichan button but with custom look.
 *
 * \ingroup GUI
 */
class Button : public gcn::Button, public gcn::WidgetListener
{
    public:
        /**
         * Default constructor.
         */
        Button();

        /**
         * Constructor, sets the caption of the button to the given string and
         * adds the given action listener.
         */
        Button(const std::string &caption, const std::string &actionEventId,
               gcn::ActionListener *listener);

        /**
         * Destructor.
         */
        ~Button();

        /**
         * Draws the button.
         */
        void draw(gcn::Graphics *graphics);

        /**
         * Update the alpha value to the button components.
         */
        void updateAlpha();

        virtual void mouseReleased(gcn::MouseEvent& mouseEvent);

        void setDescription(std::string text)
        { mDescription = text; }

        std::string getDescription()
        { return mDescription; }

        unsigned getClickCount()
        { return mClickCount; }

        void setTag(int tag)
        { mTag = tag; }

        int getTag()
        { return mTag; }

        void widgetResized(const gcn::Event &event);

        void widgetMoved(const gcn::Event &event);

    private:
        void init();

        static ImageRect button[4];    /**< Button state graphics */
        static int mInstances;         /**< Number of button instances */
        static float mAlpha;

        std::string mDescription;
        unsigned mClickCount;
        int mTag;
        GraphicsVertexes *mVertexes;
        bool mRedraw;
        int mMode;
        int mXOffset;
        int mYOffset;
};

#endif
