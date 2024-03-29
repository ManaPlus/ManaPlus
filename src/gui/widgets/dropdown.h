/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "enums/simpletypes/modal.h"

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
        DropDown(const Widget2 *const widget,
                 ListModel *const listModel,
                 const bool extended,
                 const Modal modal,
                 ActionListener *const listener,
                 const std::string &eventId);

        A_DELETE_COPY(DropDown)

        ~DropDown() override final;

        /**
         * Update the alpha value to the graphic components.
         */
        void updateAlpha();

        void draw(Graphics *const graphics) override final A_NONNULL(2);

        void safeDraw(Graphics *const graphics) override final A_NONNULL(2);

        void drawFrame(Graphics *const graphics) override final A_NONNULL(2);

        void safeDrawFrame(Graphics *const graphics) override final
                           A_NONNULL(2);

        // Inherited from KeyListener

        void keyPressed(KeyEvent& event) override final;

        // Inherited from MouseListener

        void mousePressed(MouseEvent& event) override final;

        void mouseReleased(MouseEvent& event) override final;

        void mouseDragged(MouseEvent& event) override final;

        void mouseWheelMovedUp(MouseEvent& event) override final;

        void mouseWheelMovedDown(MouseEvent& event) override final;

        void setSelectedString(const std::string &str);

        std::string getSelectedString() const A_WARN_UNUSED;

        void valueChanged(const SelectionEvent& event) override final;

        void updateSelection();

        void adjustHeight();

        void dropDown();

        void foldUp();

        void hideDrop(bool event);

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
        void drawButton(Graphics *const graphics) A_NONNULL(2);

        PopupList *mPopup A_NONNULLPOINTER;
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
