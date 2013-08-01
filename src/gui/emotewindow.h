/*
 *  The ManaPlus Client
 *  Copyright (C) 2013  The ManaPlus Developers
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

#ifndef GUI_EMOTEWINDOW_H
#define GUI_EMOTEWINDOW_H

#include "gui/widgets/window.h"

#include <guichan/mouselistener.hpp>

class EmotePage;
class ScrollArea;
class TabbedArea;

class EmoteWindow final : public Window
{
    public:
        EmoteWindow();

        A_DELETE_COPY(EmoteWindow)

        ~EmoteWindow();

        void show();

        void hide();

        std::string getSelectedEmote() const;

        void clearEmote();

        void addListeners(gcn::ActionListener *const listener);

    private:
        TabbedArea *mTabs;
        EmotePage *mEmotePage;
        ScrollArea *mScrollEmotePage;
};

extern EmoteWindow *emoteWindow;

#endif  // GUI_EMOTEWINDOW_H
