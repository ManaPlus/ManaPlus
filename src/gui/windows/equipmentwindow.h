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

#ifndef GUI_WINDOWS_EQUIPMENTWINDOW_H
#define GUI_WINDOWS_EQUIPMENTWINDOW_H

#include "equipment.h"
#include "localconsts.h"

#include "gui/widgets/window.h"

#include "utils/xml.h"

#include "listeners/actionlistener.h"

#include <vector>

class Being;
class Button;
class Image;
class ImageSet;
class Item;
class ItemPopup;
class PlayerBox;

struct EquipmentBox final
{
    EquipmentBox(const int x0, const int y0, Image *const img) :
        x(x0), y(y0), image(img)
    { }

    A_DELETE_COPY(EquipmentBox)

    int x;
    int y;
    Image *image;
};

/**
 * Equipment dialog.
 *
 * \ingroup Interface
 */
class EquipmentWindow final : public Window,
                              public ActionListener
{
    public:
        /**
         * Constructor.
         */
        EquipmentWindow(Equipment *const equipment, Being *const being,
                        const bool foring = false);

        A_DELETE_COPY(EquipmentWindow)

        /**
         * Destructor.
         */
        ~EquipmentWindow();

        void postInit() override final;

        /**
         * Draws the equipment window.
         */
        void draw(Graphics *graphics) override final;

        void action(const ActionEvent &event) override final;

        void mousePressed(MouseEvent& event) override final;

        const Item* getEquipment(const int i) const A_WARN_UNUSED
        { return mEquipment ? mEquipment->getEquipment(i) : nullptr; }

        void setBeing(Being *const being);

        void updateBeing(Being *const being);

        void resetBeing(const Being *const being);

        void mouseExited(MouseEvent &event) override final;

        void mouseMoved(MouseEvent &event) override final;

        void mouseReleased(MouseEvent &event) override final;

        void recalcSize();

    private:
        Item *getItem(const int x, const int y) const A_WARN_UNUSED;

        void setSelected(const int index);

        void fillBoxes();

        void fillDefault();

        void addBox(const int idx, int x, int y, const int imageIndex);

        void loadWindow(const XmlNodePtrConst windowNode);

        void loadPlayerBox(const XmlNodePtr playerBoxNode);

        void loadSlot(const XmlNodePtr slotNode,
                      const ImageSet *const imageset);

        static int parseSlotName(const std::string &name) A_WARN_UNUSED;

        Equipment *mEquipment;

        ItemPopup *mItemPopup;
        PlayerBox *mPlayerBox;
        Button *mUnequip;

        ImageSet *mImageSet;
        Being *mBeing;
        Image *mSlotBackground;
        Image *mSlotHighlightedBackground;
        ImageCollection *mVertexes;
        std::vector<EquipmentBox*> mBoxes;
        Color mHighlightColor;
        Color mBorderColor;
        Color mLabelsColor;
        Color mLabelsColor2;
        int mSelected; /**< Index of selected item. */
        int mItemPadding;
        int mBoxSize;
        int mButtonPadding;
        int mMinX;
        int mMinY;
        int mMaxX;
        int mMaxY;
        bool mForing;
};

extern EquipmentWindow *equipmentWindow;
extern EquipmentWindow *beingEquipmentWindow;

#endif  // GUI_WINDOWS_EQUIPMENTWINDOW_H
