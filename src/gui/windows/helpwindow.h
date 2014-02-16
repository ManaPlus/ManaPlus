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

#ifndef GUI_WINDOWS_HELPWINDOW_H
#define GUI_WINDOWS_HELPWINDOW_H

#include "gui/widgets/linkhandler.h"
#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"

#include "localconsts.h"

#include <map>
#include <set>

class Button;
class BrowserBox;
class ScrollArea;

typedef std::set<std::string> HelpNames;
typedef HelpNames::const_iterator HelpNamesCIter;
typedef std::map<std::string, HelpNames> HelpTagsMap;

/**
 * The help window.
 */
class HelpWindow final : public Window,
                         public LinkHandler,
                         public ActionListener
{
    public:
        /**
         * Constructor.
         */
        HelpWindow();

        A_DELETE_COPY(HelpWindow)

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override final;

        /**
         * Handles link action.
         */
        void handleLink(const std::string &link,
                        MouseEvent *event A_UNUSED) override final;

        /**
         * Loads help in the dialog.
         */
        void loadHelp(const std::string &helpFile);

        /**
         * Seach for given text in tags.
         */
        void search(const std::string &text);

    private:
        void loadTags();

        void loadFile(std::string file);

        Button *mDYKButton;

        BrowserBox *mBrowserBox;
        ScrollArea *mScrollArea;
        HelpTagsMap mTagFileMap;
};

extern HelpWindow *helpWindow;

#endif  // GUI_WINDOWS_HELPWINDOW_H
