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

#ifndef GUI_WIDGETS_TABS_SETUP_VIDEO_H
#define GUI_WIDGETS_TABS_SETUP_VIDEO_H

#include "gui/widgets/tabs/setuptab.h"

#include <guichan/keylistener.hpp>

class Button;
class CheckBox;
class DropDown;
class Label;
class ListBox;
class ModeListModel;
class OpenGLListModel;
class Slider;
class TextDialog;

class Setup_Video final : public SetupTab, public gcn::KeyListener
{
    public:
        explicit Setup_Video(const Widget2 *const widget);

        A_DELETE_COPY(Setup_Video)

        ~Setup_Video();

        void apply() override final;

        void cancel() override final;

        void action(const gcn::ActionEvent &event) override final;

    private:
        bool mFullScreenEnabled;
        RenderType mOpenGLEnabled;
        int mFps;
        int mAltFps;
        ModeListModel *mModeListModel;
        OpenGLListModel *mOpenGLListModel;
        ListBox *mModeList;
        CheckBox *mFsCheckBox;
        DropDown *mOpenGLDropDown;
        CheckBox *mFpsCheckBox;
        Slider *mFpsSlider;
        Label *mFpsLabel;
        Slider *mAltFpsSlider;
        Label *mAltFpsLabel;
#if !defined(ANDROID) && !defined(__APPLE__)
        Button *mDetectButton;
#endif
        TextDialog *mDialog;
        bool mCustomCursorEnabled;
        bool mEnableResize;
        bool mNoFrame;
        CheckBox *mCustomCursorCheckBox;
        CheckBox *mEnableResizeCheckBox;
        CheckBox *mNoFrameCheckBox;
};

#endif  // GUI_WIDGETS_TABS_SETUP_VIDEO_H
