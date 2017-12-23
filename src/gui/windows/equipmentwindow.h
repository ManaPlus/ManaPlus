/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "utils/stringmap.h"
#include "utils/xml.h"

#include "listeners/actionlistener.h"

class Being;
class Button;
class Image;
class ImageSet;
class Item;
class PlayerBox;
class TabStrip;

struct EquipmentPage;

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
        EquipmentWindow(Equipment *const equipment,
                        Being *const being,
                        const bool foring);

        A_DELETE_COPY(EquipmentWindow)

        /**
         * Destructor.
         */
        ~EquipmentWindow();

        void postInit() override final;

        /**
         * Draws the equipment window.
         */
        void draw(Graphics *const graphics) override final A_NONNULL(2);

        void safeDraw(Graphics *const graphics) override final A_NONNULL(2);

        void action(const ActionEvent &event) override final;

        void mousePressed(MouseEvent& event) override final;

        const Item* getEquipment(const int i) const A_WARN_UNUSED
        {
            return mEquipment != nullptr ?
                mEquipment->getEquipment(i) : nullptr;
        }

        void setBeing(Being *const being);

        void updateBeing(Being *const being);

        void resetBeing(const Being *const being);

        void mouseExited(MouseEvent &event) override final;

        void mouseMoved(MouseEvent &event) override final;

        void mouseReleased(MouseEvent &event) override final;

        void recalcSize();

        static void prepareSlotNames();

    private:
        const Item *getItem(const int x, const int y) const A_WARN_UNUSED;

        void setSelected(const int index);

        void fillBoxes();

        void fillDefault();

        void updatePage();

        int addPage(const std::string &name);

        void addDefaultPage();

        void addBox(const int idx, int x, int y, const int imageIndex);

        void loadWindow(XmlNodeConstPtrConst windowNode);

        void loadPage(XmlNodeConstPtr node);

        void loadPlayerBox(XmlNodeConstPtr playerBoxNode, const int page);

        void loadSlot(XmlNodeConstPtr slotNode,
                      const ImageSet *const imageset,
                      const int page);

        static int parseSlotName(const std::string &name) A_WARN_UNUSED;

        static StringIntMap mSlotNames;

        Equipment *mEquipment;

        PlayerBox *mPlayerBox A_NONNULLPOINTER;
        Button *mUnequip A_NONNULLPOINTER;

        ImageSet *mImageSet;
        Being *mBeing;
        Image *mSlotBackground;
        Image *mSlotHighlightedBackground;
        ImageCollection *mVertexes A_NONNULLPOINTER;
        STD_VECTOR<EquipmentPage*> mPages;
        TabStrip *mTabs;
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
        int mYPadding;
        int mSelectedTab;
        bool mForing;
        bool mHaveDefaultPage;
};

extern EquipmentWindow *equipmentWindow;
extern EquipmentWindow *beingEquipmentWindow;

#endif  // GUI_WINDOWS_EQUIPMENTWINDOW_H
