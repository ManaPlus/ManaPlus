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

#ifndef MINISTATUSWINDOW_H
#define MINISTATUSWINDOW_H

#include "inventory.h"
#include "listener.h"

#include "gui/widgets/popup.h"

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
class MiniStatusWindow final : public Popup,
                               public InventoryListener,
                               public Listener
{
    public:
        MiniStatusWindow();

        ~MiniStatusWindow();

        /**
         * Sets one of the icons.
         */
        void setIcon(const int index, AnimatedSprite *const sprite);

        void eraseIcon(const int index);

        void drawIcons(Graphics *const graphics);

        void processEvent(Channels channel,
                          const DepricatedEvent &event) override;

        void updateStatus();

        void logic() override; // Updates icons

        void draw(gcn::Graphics *graphics) override;

        void mouseMoved(gcn::MouseEvent &mouseEvent) override;

        void mousePressed(gcn::MouseEvent &event) override;

        void mouseExited(gcn::MouseEvent &event) override;

        void showBar(const std::string &name, const bool visible);

        void updateBars();

        void updateArrows();

        void slotsChanged(Inventory *const inventory);

        std::vector <ProgressBar*> &getBars()
        { return mBars; }

    private:
        bool isInBar(ProgressBar *bar, int x, int y) const;

        ProgressBar *createBar(const float progress, const int width,
                               const int height, const int color,
                               std::string name, std::string description);

        void loadBars();

        void saveBars();

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
};

extern MiniStatusWindow *miniStatusWindow;

#endif
