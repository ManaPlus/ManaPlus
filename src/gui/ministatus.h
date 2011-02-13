/*
 *  The Mana Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *
 *  This file is part of The Mana Client.
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

#ifndef MINISTATUS_H
#define MINISTATUS_H

#include "inventory.h"
#include "listener.h"

#include "gui/widgets/popup.h"
#include "gui/widgets/window.h"

#include <vector>

#ifdef __GNUC__
#define _UNUSED_  __attribute__ ((unused))
#else
#define _UNUSED_
#endif

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
class MiniStatusWindow : public Popup,
                         public InventoryListener,
                         public Mana::Listener
{
    public:
        MiniStatusWindow();

        ~MiniStatusWindow();

        /**
         * Sets one of the icons.
         */
        void setIcon(int index, AnimatedSprite *sprite);

        void eraseIcon(int index);

        void drawIcons(Graphics *graphics);

        void event(Channels channel, const Mana::Event &event);

        void updateStatus();

        void logic(); // Updates icons

        void draw(gcn::Graphics *graphics);

        void mouseMoved(gcn::MouseEvent &mouseEvent);

        void mousePressed(gcn::MouseEvent &event);

        void mouseExited(gcn::MouseEvent &event);

        void showBar(std::string name, bool isVisible);

        void updateBars();

        void slotsChanged(Inventory* inventory);

        std::list <ProgressBar*> &getBars()
        { return mBars; }

    private:
        bool isInBar(ProgressBar *bar, int x, int y) const;

        ProgressBar *createBar(float progress, int width, int height,
                               int color, std::string name,
                               std::string description);

        void loadBars();

        void saveBars();

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
        ProgressBar *mStatusBar;
        TextPopup *mTextPopup;
        StatusPopup *mStatusPopup;
        std::list <ProgressBar*> mBars;
        std::map <std::string, ProgressBar*> mBarNames;

        std::vector<AnimatedSprite *> mIcons;
};

extern MiniStatusWindow *miniStatusWindow;

#endif
