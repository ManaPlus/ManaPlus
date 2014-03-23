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

#ifndef GUI_WINDOWS_MINISTATUSWINDOW_H
#define GUI_WINDOWS_MINISTATUSWINDOW_H

#include "inventory.h"

#include "listeners/depricatedlistener.h"

#include "gui/widgets/window.h"

#include <vector>

class AnimatedSprite;
class Graphics;
class ProgressBar;
class StatusPopup;
class TextPopup;

/**
 * The player mini-status dialog.
 *
 * \ingroup Interface
 */
class MiniStatusWindow final : public Window,
                               public InventoryListener,
                               public DepricatedListener
{
    public:
        MiniStatusWindow();

        A_DELETE_COPY(MiniStatusWindow)

        ~MiniStatusWindow();

        /**
         * Sets one of the icons.
         */
        void setIcon(const int index, AnimatedSprite *const sprite);

        void eraseIcon(const int index);

        void drawIcons(Graphics *const graphics);

        void processEvent(const Channels channel,
                          const DepricatedEvent &event) override final;

        void updateStatus();

        void logic() override final;

        void draw(Graphics *graphics) override final;

        void mouseMoved(MouseEvent &event) override final;

        void mousePressed(MouseEvent &event) override final;

        void mouseExited(MouseEvent &event) override final;

        void showBar(const std::string &name, const bool visible);

        void updateBars();

        void updateArrows();

        void slotsChanged(Inventory *const inventory) override final;

        std::vector <ProgressBar*> &getBars() A_WARN_UNUSED
        { return mBars; }

        Rect getChildrenArea() override final A_WARN_UNUSED;

#ifdef USE_PROFILER
        void logicChildren();
#endif

    private:
        bool isInBar(ProgressBar *bar, int x, int y) const;

        ProgressBar *createBar(const float progress,
                               const int width, const int height,
                               const int textColor, const int backColor,
                               const std::string &restrict skin,
                               const std::string &restrict skinFill,
                               const std::string &restrict name,
                               const std::string &restrict description)
                               A_WARN_UNUSED;

        void loadBars();

        void saveBars() const;

        std::vector <ProgressBar*> mBars;
        std::map <std::string, ProgressBar*> mBarNames;
        std::vector<AnimatedSprite *> mIcons;

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
        TextPopup *mTextPopup;
        StatusPopup *mStatusPopup;

        int mSpacing;
        int mIconPadding;
        int mIconSpacing;
        int mMaxX;
};

extern MiniStatusWindow *miniStatusWindow;

#endif  // GUI_WINDOWS_MINISTATUSWINDOW_H
