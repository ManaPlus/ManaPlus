/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
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

#ifndef GUI_WINDOWS_OUTFITWINDOW_H
#define GUI_WINDOWS_OUTFITWINDOW_H

#include "enums/simpletypes/itemcolor.h"

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
        ~OutfitWindow() override final;

        void action(const ActionEvent &event) override final;

        void draw(Graphics *const graphics) override final A_NONNULL(2);

        void safeDraw(Graphics *const graphics) override final A_NONNULL(2);

        void mousePressed(MouseEvent &event) override final;

        void mouseDragged(MouseEvent &event) override final;

        void mouseReleased(MouseEvent &event) override final;

        void load();

        void wearOutfit(const int outfit,
                        const bool unwearEmpty,
                        const bool select);

        void copyOutfit(const int outfit);

        void copyOutfit(const int src, const int dst);

        void copyFromEquiped();

        void copyFromEquiped(const int dst);

        void unequipNotInOutfit(const int outfit) const;

        void next();

        void previous();

        void wearNextOutfit(const bool all);

        void wearPreviousOutfit(const bool all);

        void wearAwayOutfit();

        void unwearAwayOutfit();

        void showCurrentOutfit();

        static std::string keyName(const int number) A_WARN_UNUSED;

        void clearCurrentOutfit();

        std::string getOutfitString() const;

    private:
        int getIndexFromGrid(const int pointX,
                             const int pointY) const A_WARN_UNUSED;
        void save() const;

        Button *mPreviousButton A_NONNULLPOINTER;
        Button *mNextButton A_NONNULLPOINTER;
        Button *mEquipBottom A_NONNULLPOINTER;
        Label *mCurrentLabel A_NONNULLPOINTER;
        CheckBox *mUnequipCheck A_NONNULLPOINTER;
        CheckBox *mAwayOutfitCheck A_NONNULLPOINTER;
        Label *mKeyLabel A_NONNULLPOINTER;

        Color mBorderColor;

        int mCurrentOutfit;
        int mBoxWidth;
        int mBoxHeight;
        int mGridWidth;
        int mGridHeight;

        int mItems[OUTFITS_COUNT + 1][OUTFIT_ITEM_COUNT];
        int mAwayOutfit;

        ItemColor mItemColors[OUTFITS_COUNT + 1][OUTFIT_ITEM_COUNT];
        bool mItemClicked;
        bool mItemsUnequip[OUTFITS_COUNT];
};

extern OutfitWindow *outfitWindow;

#endif  // GUI_WINDOWS_OUTFITWINDOW_H
