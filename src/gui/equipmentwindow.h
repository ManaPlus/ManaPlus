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

#ifndef EQUIPMENTWINDOW_H
#define EQUIPMENTWINDOW_H

#include "equipment.h"
#include "guichanfwd.h"
#include "localconsts.h"

#include "gui/widgets/window.h"

#include "utils/xml.h"

#include "resources/image.h"

#include <guichan/actionlistener.hpp>

#include <vector>

class Being;
class Inventory;
class Image;
class ImageSet;
class Item;
class ItemPopup;
class PlayerBox;

struct EquipmentBox
{
    EquipmentBox(int x0, int y0, Image *img) :
        x(x0), y(y0), image(img)
    { }

    int x;
    int y;
    Image *image;
};

/**
 * Equipment dialog.
 *
 * \ingroup Interface
 */
class EquipmentWindow : public Window, public gcn::ActionListener
{
    public:
        /**
         * Constructor.
         */
        EquipmentWindow(Equipment *equipment, Being *being,
                        bool mCloseOnHide = false);

        /**
         * Destructor.
         */
        ~EquipmentWindow();

        /**
         * Draws the equipment window.
         */
        void draw(gcn::Graphics *graphics);

        void action(const gcn::ActionEvent &event);

        void mousePressed(gcn::MouseEvent& mouseEvent);

        Item* getEquipment(int i)
        { return mEquipment ? mEquipment->getEquipment(i) : nullptr; }

        void setBeing(Being *being);

        void updateBeing(Being *being);

        void resetBeing(Being *being);

    private:
        void mouseExited(gcn::MouseEvent &event);

        void mouseMoved(gcn::MouseEvent &event);

        Item *getItem(int x, int y) const;

        void setSelected(int index);

        void fillBoxes();

        void fillDefault();

        void addBox(int idx, int x, int y, int imageIndex);

        void loadWindow(XmlNodePtr windowNode);

        void loadPlayerBox(XmlNodePtr playerBoxNode);

        void loadSlot(XmlNodePtr slotNode, ImageSet *imageset);

        int parseSlotName(std::string name);

        Equipment *mEquipment;

        ItemPopup *mItemPopup;
        PlayerBox *mPlayerBox;
        gcn::Button *mUnequip;

        int mSelected; /**< Index of selected item. */
        bool mForing;
        ImageSet *mImageSet;
        Being *mBeing;
        std::vector<EquipmentBox*> mBoxes;
        gcn::Color mHighlightColor;
        gcn::Color mBorderColor;
};

extern EquipmentWindow *equipmentWindow;
extern EquipmentWindow *beingEquipmentWindow;

#endif
