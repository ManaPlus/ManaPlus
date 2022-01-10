/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Douglas Boffey <dougaboffey@netscape.net>
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef GUI_WIDGETS_TABS_SETUP_COLORS_H
#define GUI_WIDGETS_TABS_SETUP_COLORS_H

#include "gui/widgets/tabs/setuptab.h"

#include "listeners/selectionlistener.h"

class BrowserBox;
class Label;
class ListBox;
class ScrollArea;
class Slider;
class TextField;
class TextPreview;

class Setup_Colors final : public SetupTab,
                           public SelectionListener
{
    public:
        explicit Setup_Colors(const Widget2 *const widget);

        A_DELETE_COPY(Setup_Colors)

        ~Setup_Colors() override final;

        void apply() override final;

        void cancel() override final;

        void action(const ActionEvent &event) override final;

        void valueChanged(const SelectionEvent &event) override final;

    private:
        static const char *const rawmsg;

        ListBox *mColorBox A_NONNULLPOINTER;
        ScrollArea *mScroll A_NONNULLPOINTER;
        BrowserBox *mPreview A_NONNULLPOINTER;
        TextPreview *mTextPreview A_NONNULLPOINTER;
        ScrollArea *mPreviewBox A_NONNULLPOINTER;
        int mSelected;

        Label *mGradTypeLabel A_NONNULLPOINTER;
        Slider *mGradTypeSlider A_NONNULLPOINTER;
        Label *mGradTypeText A_NONNULLPOINTER;

        Label *mGradDelayLabel A_NONNULLPOINTER;
        Slider *mGradDelaySlider A_NONNULLPOINTER;
        TextField *mGradDelayText A_NONNULLPOINTER;

        Label *mRedLabel A_NONNULLPOINTER;
        Slider *mRedSlider A_NONNULLPOINTER;
        TextField *mRedText A_NONNULLPOINTER;

        Label *mGreenLabel A_NONNULLPOINTER;
        Slider *mGreenSlider A_NONNULLPOINTER;
        TextField *mGreenText A_NONNULLPOINTER;

        Label *mBlueLabel A_NONNULLPOINTER;
        Slider *mBlueSlider A_NONNULLPOINTER;
        TextField *mBlueText A_NONNULLPOINTER;

        static void setEntry(Slider *const s, TextField *const t,
                             const int value);
        void updateColor() const;
        void updateGradType();
};

#endif  // GUI_WIDGETS_TABS_SETUP_COLORS_H
