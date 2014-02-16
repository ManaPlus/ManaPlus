/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#ifndef GUI_WINDOWS_EMOTEWINDOW_H
#define GUI_WINDOWS_EMOTEWINDOW_H

#include "gui/widgets/window.h"

class ColorModel;
class ColorPage;
class EmotePage;
class ImageSet;
class ListBox;
class NamesModel;
class ScrollArea;
class TabbedArea;

class EmoteWindow final : public Window
{
    public:
        EmoteWindow();

        A_DELETE_COPY(EmoteWindow)

        ~EmoteWindow();

        void postInit() override final;

        void show();

        void hide();

        std::string getSelectedEmote() const;

        void clearEmote();

        std::string getSelectedColor() const;

        void clearColor();

        std::string getSelectedFont() const;

        void clearFont();

        void addListeners(ActionListener *const listener);

        void widgetResized(const Event &event) override final;

        void widgetMoved(const Event &event) override final;

    private:
        TabbedArea *mTabs;
        EmotePage *mEmotePage;
        ColorModel *mColorModel;
        ColorPage *mColorPage;
        ScrollArea *mScrollColorPage;
        NamesModel *mFontModel;
        ListBox *mFontPage;
        ScrollArea *mScrollFontPage;
        ImageSet *mImageSet;
};

extern EmoteWindow *emoteWindow;

#endif  // GUI_WINDOWS_EMOTEWINDOW_H
