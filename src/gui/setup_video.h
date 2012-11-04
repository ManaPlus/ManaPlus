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

#ifndef GUI_SETUP_VIDEO_H
#define GUI_SETUP_VIDEO_H

#include "being.h"

#include "gui/widgets/setuptab.h"

#include <guichan/actionlistener.hpp>
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
        Setup_Video(const Widget2 *const widget);

        A_DELETE_COPY(Setup_Video)

        ~Setup_Video();

        void apply();

        void cancel();

        void action(const gcn::ActionEvent &event) override;

        virtual void externalUpdated();

    private:
        bool mFullScreenEnabled;
        int mOpenGLEnabled;
        bool mCustomCursorEnabled;
        int mFps;
        int mAltFps;
        bool mEnableResize;
        bool mNoFrame;

        ModeListModel *mModeListModel;

        OpenGLListModel *mOpenGLListModel;

        Label *scrollRadiusLabel;
        Label *scrollLazinessLabel;

        ListBox *mModeList;
        CheckBox *mFsCheckBox;
        DropDown *mOpenGLDropDown;
        CheckBox *mCustomCursorCheckBox;

        CheckBox *mEnableResizeCheckBox;
        CheckBox *mNoFrameCheckBox;

        CheckBox *mFpsCheckBox;
        Slider *mFpsSlider;
        Label *mFpsLabel;
        Slider *mAltFpsSlider;
        Label *mAltFpsLabel;

#ifndef ANDROID
        Button *mDetectButton;
#endif
        TextDialog *mDialog;
};

#endif
