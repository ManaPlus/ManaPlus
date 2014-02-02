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

#ifndef GUI_WINDOWS_SHORTCUTWINDOW_H
#define GUI_WINDOWS_SHORTCUTWINDOW_H

#include "gui/widgets/window.h"

class ScrollArea;
class ShortcutContainer;
class TabbedArea;

/**
 * A window around a ShortcutContainer.
 *
 * \ingroup Interface
 */
class ShortcutWindow final : public Window
{
    public:
        /**
         * Constructor.
         */
        ShortcutWindow(const std::string &restrict title,
                       ShortcutContainer *restrict const content,
                       const std::string &restrict skinFile = "",
                       int width = 0, int height = 0);

        ShortcutWindow(const std::string &restrict title,
                       const std::string &restrict skinFile = "",
                       const int width = 0, const int height = 0);

        A_DELETE_COPY(ShortcutWindow)

        /**
         * Destructor.
         */
        ~ShortcutWindow();

        void addTab(const std::string &name, ShortcutContainer *const content);

        int getTabIndex() const A_WARN_UNUSED;

        void widgetHidden(const gcn::Event &event) override final;

        void widgetMoved(const gcn::Event& event) override final;

        void mousePressed(gcn::MouseEvent &event) override final;

        void mouseDragged(gcn::MouseEvent &event) override final;

        void nextTab();

        void prevTab();

#ifdef USE_PROFILER
        void logicChildren();
#endif

    private:
        ShortcutWindow();
        ShortcutContainer *mItems;

        ScrollArea *mScrollArea;
        TabbedArea *mTabs;
        std::vector<ShortcutContainer*> mPages;

        static int mBoxesWidth;
};

extern ShortcutWindow *itemShortcutWindow;
extern ShortcutWindow *emoteShortcutWindow;
extern ShortcutWindow *dropShortcutWindow;

#endif  // GUI_WINDOWS_SHORTCUTWINDOW_H
