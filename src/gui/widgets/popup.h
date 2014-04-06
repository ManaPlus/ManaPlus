/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
 *  Copyright (C) 2009  Aethyra Development Team
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

#ifndef GUI_WIDGETS_POPUP_H
#define GUI_WIDGETS_POPUP_H

#include "gui/widgets/container.h"

#include "listeners/mouselistener.h"
#include "listeners/widgetlistener.h"

class ImageCollection;
class Skin;
class WindowContainer;

/**
 * A light version of the Window class. Particularly suited for popup type
 * functionality that doesn't need to be resized or moved around by the mouse
 * once created, but only needs to display some simple content, like a static
 * message.
 *
 * Popups, in general, shouldn't also need to update their content once
 * created, although this is not an explicit requirement to use the popup
 * class.
 *
 * \ingroup GUI
 */
class Popup : public Container,
              public MouseListener,
              public WidgetListener
{
    public:
        /**
         * Constructor. Initializes the title to the given text and hooks
         * itself into the popup container.
         *
         * @param name    A human readable name for the popup. Only useful for
         *                debugging purposes.
         * @param skin    The location where the Popup's skin XML can be found.
         */
        explicit Popup(const std::string &name = "",
                       std::string skin = "");

        A_DELETE_COPY(Popup)

        /**
         * Destructor. Deletes all the added widgets.
         */
        virtual ~Popup();

        /**
         * Sets the window container to be used by new popups.
         */
        static void setWindowContainer(WindowContainer *const windowContainer);

        /**
         * Draws the popup.
         */
        void draw(Graphics *graphics) override;

        /**
         * Sets the size of this popup.
         */
        void setContentSize(int width, int height);

        /**
         * Sets the location relative to the given widget.
         */
        void setLocationRelativeTo(const Widget *const widget);

        void mouseMoved(MouseEvent &event) override;

        /**
         * Sets the minimum width of the popup.
         */
        void setMinWidth(const int width);

        int getMinWidth() const A_WARN_UNUSED
        { return mMinWidth; }

        /**
         * Sets the minimum height of the popup.
         */
        void setMinHeight(const int height);

        int getMinHeight() const A_WARN_UNUSED
        { return mMinHeight; }

        /**
         * Sets the maximum width of the popup.
         */
        void setMaxWidth(const int width);

        int getMaxWidth() const A_WARN_UNUSED
        { return mMaxWidth; }

        /**
         * Sets the minimum height of the popup.
         */
        void setMaxHeight(const int height);

        int getMaxHeight() const A_WARN_UNUSED
        { return mMaxHeight; }

        /**
         * Gets the padding of the popup. The padding is the distance between
         * the popup border and the content.
         *
         * @return The padding of the popup.
         * @see setPadding
         */
        int getPadding() const A_WARN_UNUSED
        { return mPadding; }

        void setPadding(int padding)
        { mPadding = padding; }

        /**
         * Sets the name of the popup. This is only useful for debug purposes.
         */
        void setPopupName(const std::string &name)
        { mPopupName = name; }

        const std::string &getPopupName() const
        { return mPopupName; }

        /**
         * Schedule this popup for deletion. It will be deleted at the start
         * of the next logic update.
         */
        void scheduleDelete();

        // Inherited from BasicContainer

        virtual Rect getChildrenArea() override;

        /**
         * Sets the location to display the popup. Tries to horizontally center
         * the popup and provide a vertical buffer between the given point and
         * the popup. Prevents the popup from extending off-screen, if
         * possible.
         */
        void position(const int x, const int y);

        void hide();

        void widgetResized(const Event &event) override;

        void widgetMoved(const Event &event) override final;

        bool isPopupVisible() const
        { return mVisible; }

        virtual void postInit()
        {
        }

    protected:
        int mPadding;                 /**< Holds the padding of the popup. */
        Skin *mSkin;                  /**< Skin in use by this popup */

    private:
        std::string mPopupName;       /**< Name of the popup */
        int mMinWidth;                /**< Minimum popup width */
        int mMinHeight;               /**< Minimum popup height */
        int mMaxWidth;                /**< Maximum popup width */
        int mMaxHeight;               /**< Maximum popup height */

        ImageCollection *mVertexes;
};

#endif  // GUI_WIDGETS_POPUP_H
