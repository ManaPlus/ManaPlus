/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

        ~EmoteWindow() override final;

        void postInit() override final;

        void show();

        void hide();

        std::string getSelectedEmote() const;

        void clearEmote();

        std::string getSelectedColor() const;

        void clearColor();

        std::string getSelectedFont() const;

        int getSelectedTextIndex() const;

        void clearFont();

        void clearText();

        void addListeners(ActionListener *const listener);

        void widgetResized(const Event &event) override final;

        void widgetMoved(const Event &event) override final;

    private:
        TabbedArea *mTabs A_NONNULLPOINTER;
        EmotePage *mEmotePage A_NONNULLPOINTER;
        ColorModel *mColorModel A_NONNULLPOINTER;
        ColorPage *mColorPage A_NONNULLPOINTER;
        ScrollArea *mScrollColorPage A_NONNULLPOINTER;
        NamesModel *mFontModel A_NONNULLPOINTER;
        ListBox *mFontPage A_NONNULLPOINTER;
        ScrollArea *mScrollFontPage A_NONNULLPOINTER;
        NamesModel *mTextModel A_NONNULLPOINTER;
        ListBox *mTextPage A_NONNULLPOINTER;
        ScrollArea *mScrollTextPage A_NONNULLPOINTER;
        ImageSet *mImageSet;
};

extern EmoteWindow *emoteWindow;

#endif  // GUI_WINDOWS_EMOTEWINDOW_H
