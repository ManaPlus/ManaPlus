/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
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

#ifndef GUI_WIDGETS_DROPDOWN_H
#define GUI_WIDGETS_DROPDOWN_H

#include "gui/widgets/basiccontainer.h"

#include "listeners/actionlistener.h"
#include "listeners/focuslistener.h"
#include "listeners/keylistener.h"
#include "listeners/mouselistener.h"
#include "listeners/selectionlistener.h"

#include "localconsts.h"

class Image;
class ListModel;
class PopupList;
class Skin;

/**
 * A drop down box from which you can select different values.
 *
 * A ListModel provides the contents of the drop down. To be able to use
 * DropDown you must give DropDown an implemented ListModel which represents
 * your list.
 */
class DropDown final : public ActionListener,
                       public BasicContainer,
                       public KeyListener,
                       public MouseListener,
                       public FocusListener,
                       public SelectionListener
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
                 ListModel *const listModel,
                 const bool extended = false,
                 const bool modal = false,
                 ActionListener *const listener = nullptr,
                 const std::string &eventId = "");

        A_DELETE_COPY(DropDown)

        ~DropDown();

        /**
         * Update the alpha value to the graphic components.
         */
        void updateAlpha();

        void draw(Graphics *graphics) override final;

        void drawFrame(Graphics *graphics) override final;

        // Inherited from KeyListener

        void keyPressed(KeyEvent& keyEvent) override final;

        // Inherited from MouseListener

        void mousePressed(MouseEvent& mouseEvent) override final;

        void mouseReleased(MouseEvent& mouseEvent) override final;

        void mouseDragged(MouseEvent& mouseEvent) override final;

        void mouseWheelMovedUp(MouseEvent& mouseEvent) override final;

        void mouseWheelMovedDown(MouseEvent& mouseEvent) override final;

        void setSelectedString(const std::string &str);

        std::string getSelectedString() const A_WARN_UNUSED;

        void valueChanged(const SelectionEvent& event) override final;

        void updateSelection();

        void adjustHeight();

        void dropDown();

        void foldUp();

        void hideDrop(bool event = true);

        int getSelected() const;

        void setSelected(int selected);

        void setListModel(ListModel *const listModel);

        ListModel *getListModel();

        void addSelectionListener(SelectionListener* listener);

        void removeSelectionListener(SelectionListener* selectionListener);

        Rect getChildrenArea() override;

        void action(const ActionEvent &actionEvent) override;

        void distributeValueChangedEvent();

    protected:
        /**
         * Draws the button with the little down arrow.
         *
         * @param graphics a Graphics object to draw with.
         */
        void drawButton(Graphics *graphics);

        PopupList *mPopup;
        Color mShadowColor;
        Color mHighlightColor;
        int mPadding;
        int mImagePadding;
        int mSpacing;
        int mFoldedUpHeight;

        typedef std::list<SelectionListener*> SelectionListenerList;
        SelectionListenerList mSelectionListeners;
        typedef SelectionListenerList::iterator SelectionListenerIterator;

        bool mExtended;
        bool mDroppedDown;
        bool mPushed;
        bool mIsDragged;

        // Add own Images.
        static int instances;
        static Image *buttons[2][2];
        static ImageRect skinRect;
        static float mAlpha;
        static Skin *mSkin;
};

#endif  // GUI_WIDGETS_DROPDOWN_H
