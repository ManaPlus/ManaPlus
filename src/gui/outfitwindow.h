/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
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

#ifndef OUTFITWINDOW_H
#define OUTFITWINDOW_H

#include "gui/widgets/window.h"

#include <guichan/actionlistener.hpp>
#include <guichan/mouselistener.hpp>

#ifdef __GNUC__
#define A_PURE  __attribute__ ((pure))
#else
#define A_PURE
#endif

const unsigned int OUTFITS_COUNT = 100;
const unsigned int OUTFIT_ITEM_COUNT = 16;

class Button;
class CheckBox;
class Item;
class Label;

class OutfitWindow : public Window, private gcn::ActionListener
{
    public:
        /**
         * Constructor.
         */
        OutfitWindow();

        /**
         * Destructor.
         */
        ~OutfitWindow();

        void action(const gcn::ActionEvent &event);

        void draw(gcn::Graphics *graphics);

        void mousePressed(gcn::MouseEvent &event);

        void mouseDragged(gcn::MouseEvent &event);

        void mouseReleased(gcn::MouseEvent &event);

        void load(const bool oldConfig = false);

        void setItemSelected(const int itemId)
        { mItemSelected = itemId; }

        void setItemSelected(Item *item);

        bool isItemSelected() const
        { return mItemSelected > 0; }

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

        std::string keyName(const int number) const;

        void clearCurrentOutfit();

    private:
        Button *mPreviousButton;
        Button *mNextButton;
        Button *mEquipButtom;
        Label *mCurrentLabel;
        CheckBox *mUnequipCheck;
        CheckBox *mAwayOutfitCheck;
        Label *mKeyLabel;

        int getIndexFromGrid(const int pointX, const int pointY) const;

        int mBoxWidth;
        int mBoxHeight;
        int mCursorPosX, mCursorPosY;
        int mGridWidth, mGridHeight;
        bool mItemClicked;
        Item *mItemMoved;

        void save();

        int mItems[OUTFITS_COUNT + 1][OUTFIT_ITEM_COUNT];
        unsigned char mItemColors[OUTFITS_COUNT + 1][OUTFIT_ITEM_COUNT];
        bool mItemsUnequip[OUTFITS_COUNT];
        int mItemSelected;
        unsigned char mItemColorSelected;

        int mCurrentOutfit;
        int mAwayOutfit;

        Image *mBackgroundImg;
        gcn::Color mBorderColor;
        gcn::Color mBackgroundColor;

        static float mAlpha;
};

extern OutfitWindow *outfitWindow;

#endif
