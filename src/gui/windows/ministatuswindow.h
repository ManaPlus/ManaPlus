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

#ifndef GUI_WINDOWS_MINISTATUSWINDOW_H
#define GUI_WINDOWS_MINISTATUSWINDOW_H

#include "listeners/arrowslistener.h"
#include "listeners/attributelistener.h"
#include "listeners/inventorylistener.h"
#include "listeners/statlistener.h"
#include "listeners/updatestatuslistener.h"

#include "gui/widgets/window.h"

class AnimatedSprite;
class Graphics;
class Inventory;
class ProgressBar;
class StatusPopup;

/**
 * The player mini-status dialog.
 *
 * \ingroup Interface
 */
class MiniStatusWindow final : public Window,
                               public InventoryListener,
                               public AttributeListener,
                               public StatListener,
                               public ArrowsListener,
                               public UpdateStatusListener
{
    public:
        MiniStatusWindow();

        A_DELETE_COPY(MiniStatusWindow)

        ~MiniStatusWindow() override final;

        /**
         * Sets one of the icons.
         */
        void setIcon(const int index, AnimatedSprite *const sprite);

        void eraseIcon(const int index);

        void drawIcons(Graphics *const graphics) A_NONNULL(2);

        void updateStatus() override final;

        void logic() override final;

        void draw(Graphics *const graphics) override final A_NONNULL(2);

        void safeDraw(Graphics *const graphics) override final A_NONNULL(2);

        void mouseMoved(MouseEvent &event) override final;

        void mousePressed(MouseEvent &event) override final;

        void mouseExited(MouseEvent &event) override final;

        void showBar(const std::string &name, const Visible visible);

        void updateBars();

        void slotsChanged(const Inventory *const inventory) override final;

        STD_VECTOR <ProgressBar*> &getBars() A_WARN_UNUSED
        { return mBars; }

        Rect getChildrenArea() override final A_WARN_UNUSED;

        void attributeChanged(const AttributesT id,
                              const int64_t oldVal,
                              const int64_t newVal) override final;

        void statChanged(const AttributesT id,
                         const int oldVal1,
                         const int oldVal2) override final;

        void arrowsChanged() override final;

#ifdef USE_PROFILER
        void logicChildren();
#endif  // USE_PROFILER

    private:
        bool isInBar(ProgressBar *bar, int x, int y) const;

        ProgressBar *createBar(const float progress,
                               const int width,
                               const int height,
                               const ThemeColorIdT textColor,
                               const ProgressColorIdT backColor,
                               const std::string &restrict skin,
                               const std::string &restrict skinFill,
                               const std::string &restrict name,
                               const std::string &restrict description)
                               A_WARN_UNUSED;

        void loadBars();

        void saveBars() const;

        STD_VECTOR <ProgressBar*> mBars;
        std::map <std::string, ProgressBar*> mBarNames;
        STD_VECTOR<AnimatedSprite *> mIcons;

        /*
         * Mini Status Bars
         */
        ProgressBar *mHpBar;
        ProgressBar *mMpBar;
        ProgressBar *mXpBar;
        ProgressBar *mJobBar;
        ProgressBar *mWeightBar;
        ProgressBar *mInvSlotsBar;
        ProgressBar *mMoneyBar;
        ProgressBar *mArrowsBar;
        ProgressBar *mStatusBar;
        StatusPopup *mStatusPopup;

        int mSpacing;
        int mIconPadding;
        int mIconSpacing;
        int mMaxX;
};

extern MiniStatusWindow *miniStatusWindow;

#endif  // GUI_WINDOWS_MINISTATUSWINDOW_H
