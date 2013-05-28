/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
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

#ifndef GUI_WIDGETS_DROPDOWN_H
#define GUI_WIDGETS_DROPDOWN_H

#include "gui/widgets/widget2.h"

#include <guichan/actionlistener.hpp>
#include <guichan/basiccontainer.hpp>
#include <guichan/focushandler.hpp>
#include <guichan/focuslistener.hpp>
#include <guichan/keylistener.hpp>
#include <guichan/listmodel.hpp>
#include <guichan/mouselistener.hpp>
#include <guichan/selectionlistener.hpp>
#include <guichan/widgets/scrollarea.hpp>

#include "localconsts.h"

class Image;
class ImageRect;
class ListBox;
class PopupList;
class ScrollArea;
class Skin;

/**
 * A drop down box from which you can select different values.
 *
 * A ListModel provides the contents of the drop down. To be able to use
 * DropDown you must give DropDown an implemented ListModel which represents
 * your list.
 */
class DropDown final : public gcn::ActionListener,
                       public gcn::BasicContainer,
                       public gcn::KeyListener,
                       public gcn::MouseListener,
                       public gcn::FocusListener,
                       public gcn::SelectionListener,
                       public Widget2
{
    public:
        /**
         * Contructor.
         *
         * @param listModel the ListModel to use.
         * @param scrollArea the ScrollArea to use.
         * @param listBox the listBox to use.
         * @see ListModel, ScrollArea, ListBox.
         */
        DropDown(const Widget2 *const widget,
                 gcn::ListModel *const listModel = nullptr,
                 bool extended = false,
                 bool modal = false,
                 gcn::ActionListener *const listener = nullptr,
                 const std::string &eventId = "");

        A_DELETE_COPY(DropDown)

        ~DropDown();

        /**
         * Update the alpha value to the graphic components.
         */
        void updateAlpha();

        void draw(gcn::Graphics *graphics) override;

        void drawFrame(gcn::Graphics *graphics) override;

        // Inherited from KeyListener

        void keyPressed(gcn::KeyEvent& keyEvent) override;

        // Inherited from MouseListener

        void mousePressed(gcn::MouseEvent& mouseEvent) override;

        void mouseReleased(gcn::MouseEvent& mouseEvent) override;

        void mouseDragged(gcn::MouseEvent& mouseEvent) override;

        void mouseWheelMovedUp(gcn::MouseEvent& mouseEvent) override;

        void mouseWheelMovedDown(gcn::MouseEvent& mouseEvent) override;

        void setSelectedString(std::string str);

        std::string getSelectedString() const A_WARN_UNUSED;

        void valueChanged(const gcn::SelectionEvent& event);

        void updateSelection();

        void adjustHeight();

        void dropDown();

        void foldUp();

        void hideDrop();

        int getSelected() const;

        void setSelected(int selected);

        void setListModel(gcn::ListModel *listModel);

        gcn::ListModel *getListModel();

        void addSelectionListener(SelectionListener* listener);

        void removeSelectionListener(SelectionListener* selectionListener);

        gcn::Rectangle getChildrenArea();

        void action(const gcn::ActionEvent &actionEvent);

        void distributeValueChangedEvent();

    protected:
        /**
         * Draws the button with the little down arrow.
         *
         * @param graphics a Graphics object to draw with.
         */
        void drawButton(gcn::Graphics *graphics);

        bool mExtended;
        PopupList *mPopup;
        gcn::Color mShadowColor;
        gcn::Color mHighlightColor;
        int mPadding;
        int mImagePadding;
        int mSpacing;
        gcn::Color mForegroundColor2;

        bool mDroppedDown;
        bool mPushed;
        int mFoldedUpHeight;
        bool mIsDragged;
        typedef std::list<SelectionListener*> SelectionListenerList;
        SelectionListenerList mSelectionListeners;
        typedef SelectionListenerList::iterator SelectionListenerIterator;

        // Add own Images.
        static int instances;
        static Image *buttons[2][2];
        static ImageRect skinRect;
        static float mAlpha;
        static Skin *mSkin;
};

#endif  // GUI_WIDGETS_DROPDOWN_H
