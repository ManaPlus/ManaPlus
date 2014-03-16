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

#ifndef GUI_WINDOWS_STATUSWINDOW_H
#define GUI_WINDOWS_STATUSWINDOW_H

#include "listeners/depricatedlistener.h"

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"

#include <map>

class AttrDisplay;
class Button;
class Label;
class ProgressBar;
class ScrollArea;
class VertContainer;

/**
 * The player status dialog.
 *
 * \ingroup Interface
 */
class StatusWindow final : public Window,
                           public ActionListener,
                           public DepricatedListener
{
    public:
        /**
         * Constructor.
         */
        StatusWindow();

        A_DELETE_COPY(StatusWindow)

        void processEvent(const Channels channel,
                          const DepricatedEvent &event) override;

        void setPointsNeeded(const int id, const int needed);

        void addAttribute(const int id, const std::string &restrict name,
                          const std::string &restrict shortName = "",
                          const bool modifiable = false);

        static void updateHPBar(ProgressBar *const bar,
                                const bool showMax = false);
        void updateMPBar(ProgressBar *bar, const bool showMax = false) const;
        static void updateJobBar(ProgressBar *const bar,
                                 const bool percent = true);
        static void updateXPBar(ProgressBar *const bar,
                                const bool percent = true);
        static void updateWeightBar(ProgressBar *const bar);
        static void updateInvSlotsBar(ProgressBar *const bar);
        static void updateMoneyBar(ProgressBar *const bar);
        static void updateArrowsBar(ProgressBar *const bar);
        void updateStatusBar(ProgressBar *const bar,
                             const bool percent = true) const;
        static void updateProgressBar(ProgressBar *const bar, const int value,
                                      const int max, const bool percent);
        static void updateProgressBar(ProgressBar *const bar,
                                      const int id,
                                      const bool percent = true);

        void action(const ActionEvent &event) override;

        void clearAttributes();

    private:
        static std::string translateLetter(const char *const letters);
        static std::string translateLetter2(std::string letters);

        /**
         * Status Part
         */
        Label *mLvlLabel;
        Label *mMoneyLabel;
        Label *mHpLabel;
        Label *mMpLabel;
        Label *mXpLabel;
        ProgressBar *mHpBar;
        ProgressBar *mMpBar;
        ProgressBar *mXpBar;

        Label *mJobLvlLabel;
        Label *mJobLabel;
        ProgressBar *mJobBar;

        VertContainer *mAttrCont;
        ScrollArea *mAttrScroll;
        VertContainer *mDAttrCont;
        ScrollArea *mDAttrScroll;

        Label *mCharacterPointsLabel;
        Label *mCorrectionPointsLabel;
        Button *mCopyButton;

        typedef std::map<int, AttrDisplay*> Attrs;
        Attrs mAttrs;
};

extern StatusWindow *statusWindow;

#endif  // GUI_WINDOWS_STATUSWINDOW_H
