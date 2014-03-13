/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
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

#ifndef GUI_WINDOWS_OUTFITWINDOW_H
#define GUI_WINDOWS_OUTFITWINDOW_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"

const unsigned int OUTFITS_COUNT = 100;
const unsigned int OUTFIT_ITEM_COUNT = 16;

class Button;
class CheckBox;
class Label;

class OutfitWindow final : public Window,
                           private ActionListener
{
    public:
        /**
         * Constructor.
         */
        OutfitWindow();

        A_DELETE_COPY(OutfitWindow)

        /**
         * Destructor.
         */
        ~OutfitWindow();

        void action(const ActionEvent &event) override final;

        void draw(Graphics *graphics) override final;

        void mousePressed(MouseEvent &event) override final;

        void mouseDragged(MouseEvent &event) override final;

        void mouseReleased(MouseEvent &event) override final;

        void load(const bool oldConfig = false);

        void wearOutfit(const int outfit, const bool unwearEmpty = true,
                        const bool select = false);

        void copyOutfit(const int outfit);

        void copyOutfit(const int src, const int dst);

        void copyFromEquiped();

        void copyFromEquiped(const int dst);

        void unequipNotInOutfit(const int outfit) const;

        void next();

        void previous();

        void wearNextOutfit(const bool all = false);

        void wearPreviousOutfit(const bool all = false);

        void wearAwayOutfit();

        void unwearAwayOutfit();

        void showCurrentOutfit();

        static std::string keyName(const int number) A_WARN_UNUSED;

        void clearCurrentOutfit();

    private:
        int getIndexFromGrid(const int pointX,
                             const int pointY) const A_WARN_UNUSED;
        void save() const;

        Button *mPreviousButton;
        Button *mNextButton;
        Button *mEquipButtom;
        Label *mCurrentLabel;
        CheckBox *mUnequipCheck;
        CheckBox *mAwayOutfitCheck;
        Label *mKeyLabel;

        Color mBorderColor;
        Color mBackgroundColor;

        int mCurrentOutfit;
        int mBoxWidth;
        int mBoxHeight;
        int mGridWidth;
        int mGridHeight;

        int mItems[OUTFITS_COUNT + 1][OUTFIT_ITEM_COUNT];
        int mAwayOutfit;

        unsigned char mItemColors[OUTFITS_COUNT + 1][OUTFIT_ITEM_COUNT];
        bool mItemClicked;
        bool mItemsUnequip[OUTFITS_COUNT];
};

extern OutfitWindow *outfitWindow;

#endif  // GUI_WINDOWS_OUTFITWINDOW_H
