/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "gui/widgets/tabs/setup_video.h"

#include "gui/windowmanager.h"

#include "gui/windows/okdialog.h"
#include "gui/windows/textdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/slider.h"
#include "gui/widgets/dropdown.h"

#include "utils/delete2.h"

#if defined(USE_OPENGL) && !defined(ANDROID) && !defined(__APPLE__)
#include "graphicsmanager.h"

#include "test/testmain.h"
#endif  // defined(USE_OPENGL) && !defined(ANDROID) && !defined(__APPLE__)

#if defined(ANDROID) || defined(__APPLE__) || !defined(USE_OPENGL)
#include "configuration.h"
#endif  // defined(ANDROID) || defined(__APPLE__) || !defined(USE_OPENGL)

#if defined(ANDROID) || defined(__APPLE__)
#include "utils/stringutils.h"
#endif  // defined(ANDROID) || defined(__APPLE__)

#include "gui/models/modelistmodel.h"
#include "gui/models/opengllistmodel.h"

#include <sstream>

#include "debug.h"

extern Graphics *mainGraphics;

Setup_Video::Setup_Video(const Widget2 *const widget) :
    SetupTab(widget),
    KeyListener(),
    mFullScreenEnabled(config.getBoolValue("screen")),
    mOpenGLEnabled(intToRenderType(config.getIntValue("opengl"))),
    mFps(config.getIntValue("fpslimit")),
    mAltFps(config.getIntValue("altfpslimit")),
    mModeListModel(new ModeListModel),
    mOpenGLListModel(new OpenGLListModel),
    mModeList(CREATEWIDGETR(ListBox, widget, mModeListModel, "")),
    // TRANSLATORS: video settings checkbox
    mFsCheckBox(new CheckBox(this, _("Full screen"), mFullScreenEnabled)),
    mOpenGLDropDown(new DropDown(widget, mOpenGLListModel)),
    // TRANSLATORS: video settings checkbox
    mFpsCheckBox(new CheckBox(this, _("FPS limit:"))),
    mFpsSlider(new Slider(this, 2.0, 160.0, 1.0)),
    mFpsLabel(new Label(this)),
    mAltFpsSlider(new Slider(this, 2.0, 160.0, 1.0)),
    // TRANSLATORS: video settings label
    mAltFpsLabel(new Label(this, _("Alt FPS limit: "))),
#if !defined(ANDROID) && !defined(__APPLE__) && !defined(__native_client__)
    // TRANSLATORS: video settings button
    mDetectButton(new Button(this, _("Detect best mode"), "detect", this)),
#endif  // !defined(ANDROID) && !defined(__APPLE__) &&
        // !defined(__native_client__)
    mDialog(nullptr),
    mCustomCursorEnabled(config.getBoolValue("customcursor")),
    mEnableResize(config.getBoolValue("enableresize")),
    mNoFrame(config.getBoolValue("noframe")),
    mCustomCursorCheckBox(new CheckBox(this,
#ifdef ANDROID
        // TRANSLATORS: video settings checkbox
        _("Show cursor"),
#else  // ANDROID
        // TRANSLATORS: video settings checkbox
        _("Custom cursor"),
#endif  // ANDROID
        mCustomCursorEnabled)),
    // TRANSLATORS: video settings checkbox
    mEnableResizeCheckBox(new CheckBox(this, _("Enable resize"),
                          mEnableResize)),
    // TRANSLATORS: video settings checkbox
    mNoFrameCheckBox(new CheckBox(this, _("No frame"), mNoFrame))
{
    // TRANSLATORS: video settings tab name
    setName(_("Video"));

    ScrollArea *const scrollArea = new ScrollArea(this, mModeList,
        Opaque_true, "setup_video_background.xml");
    scrollArea->setWidth(150);
    scrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);

    mOpenGLDropDown->setSelected(renderToIndex[mOpenGLEnabled]);

    mModeList->setEnabled(true);

    // TRANSLATORS: video settings label
    mFpsLabel->setCaption(mFps > 0 ? toString(mFps) : _("None"));
    mFpsLabel->setWidth(60);
    // TRANSLATORS: video settings label
    mAltFpsLabel->setCaption(_("Alt FPS limit: ") + (mAltFps > 0 ?
        // TRANSLATORS: video settings label value
        toString(mAltFps) : _("None")));
    mAltFpsLabel->setWidth(150);
    mFpsSlider->setEnabled(mFps > 0);
    mFpsSlider->setValue(mFps);
    mAltFpsSlider->setEnabled(mAltFps > 0);
    mAltFpsSlider->setValue(mAltFps);
    mFpsCheckBox->setSelected(mFps > 0);

    // Pre-select the current video mode.
    const std::string videoMode = toString(
        mainGraphics->mActualWidth).append("x").append(
        toString(mainGraphics->mActualHeight));
    mModeList->setSelected(mModeListModel->getIndexOf(videoMode));

    mModeList->setActionEventId("videomode");
    mCustomCursorCheckBox->setActionEventId("customcursor");
    mFpsCheckBox->setActionEventId("fpslimitcheckbox");
    mFpsSlider->setActionEventId("fpslimitslider");
    mAltFpsSlider->setActionEventId("altfpslimitslider");
    mOpenGLDropDown->setActionEventId("opengl");
    mEnableResizeCheckBox->setActionEventId("enableresize");
    mNoFrameCheckBox->setActionEventId("noframe");

    mModeList->addActionListener(this);
    mCustomCursorCheckBox->addActionListener(this);
    mFpsCheckBox->addActionListener(this);
    mFpsSlider->addActionListener(this);
    mAltFpsSlider->addActionListener(this);
    mOpenGLDropDown->addActionListener(this);
    mEnableResizeCheckBox->addActionListener(this);
    mNoFrameCheckBox->addActionListener(this);

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, scrollArea, 1, 5).setPadding(2);
    place(0, 5, mOpenGLDropDown, 1);

    place(1, 0, mFsCheckBox, 2);

    place(1, 1, mCustomCursorCheckBox, 3);

    place(1, 2, mEnableResizeCheckBox, 2);
    place(1, 3, mNoFrameCheckBox, 2);

    place(0, 6, mFpsSlider);
    place(1, 6, mFpsCheckBox).setPadding(3);
    place(2, 6, mFpsLabel).setPadding(1);

    place(0, 7, mAltFpsSlider);
    place(1, 7, mAltFpsLabel).setPadding(3);

#if !defined(ANDROID) && !defined(__APPLE__) && !defined(__native_client__)
    place(0, 8, mDetectButton);
#else  // !defined(ANDROID) && !defined(__APPLE__) &&
       // !defined(__native_client__)
    mNoFrameCheckBox->setEnabled(false);
    mEnableResizeCheckBox->setEnabled(false);
#ifndef __native_client__
    mFsCheckBox->setEnabled(false);
#endif  // __native_client__
#endif  // !defined(ANDROID) && !defined(__APPLE__) &&
        // !defined(__native_client__)

    int width = 600;

    if (config.getIntValue("screenwidth") >= 730)
        width += 100;

    setDimension(Rect(0, 0, width, 300));
}

Setup_Video::~Setup_Video()
{
    delete2(mModeListModel);
    delete2(mModeList);
    delete2(mOpenGLListModel);
    delete2(mDialog);
}

void Setup_Video::apply()
{
    // Full screen changes
    bool fullscreen = mFsCheckBox->isSelected();
    if (fullscreen != config.getBoolValue("screen"))
    {
        /* The OpenGL test is only necessary on Windows, since switching
         * to/from full screen works fine on Linux. On Windows we'd have to
         * reinitialize the OpenGL state and reload all textures.
         *
         * See http://libsdl.org/cgi/docwiki.cgi/SDL_SetVideoMode
         */

#if defined(WIN32) || defined(__APPLE__) || defined(ANDROID)
        // checks for opengl usage
        if (intToRenderType(config.getIntValue("opengl")) == RENDER_SOFTWARE)
        {
#endif  // defined(WIN32) || defined(__APPLE__) || defined(ANDROID)
            if (!WindowManager::setFullScreen(fullscreen))
            {
                fullscreen = !fullscreen;
                if (!WindowManager::setFullScreen(fullscreen))
                {
                    std::stringstream errorMsg;
                    if (fullscreen)
                    {
                        // TRANSLATORS: video error message
                        errorMsg << _("Failed to switch to windowed mode "
                            "and restoration of old mode also "
                            "failed!") << std::endl;
                    }
                    else
                    {
                        // TRANSLATORS: video error message
                        errorMsg << _("Failed to switch to fullscreen mode"
                            " and restoration of old mode also "
                            "failed!") << std::endl;
                    }
                    logger->safeError(errorMsg.str());
                }
            }
#if defined(WIN32) || defined(__APPLE__) || defined(ANDROID)
        }
        else
        {
            CREATEWIDGET(OkDialog,
                // TRANSLATORS: video settings warning
                _("Switching to Full Screen"),
                // TRANSLATORS: video settings warning
                _("Restart needed for changes to take effect."),
                // TRANSLATORS: ok dialog button
                _("OK"),
                DialogType::OK,
                Modal_true,
                ShowCenter_true,
                nullptr,
                260);
        }
#endif  // defined(WIN32) || defined(__APPLE__) || defined(ANDROID)

        config.setValue("screen", fullscreen);
    }

    const int sel = mOpenGLDropDown->getSelected();
    RenderType mode = RENDER_SOFTWARE;
    if (sel >= 0 && CAST_U32(sel) < sizeof(indexToRender))
        mode = indexToRender[mOpenGLDropDown->getSelected()];

    // OpenGL change
    if (mode != mOpenGLEnabled)
    {
        config.setValue("opengl", CAST_S32(mode));

        // OpenGL can currently only be changed by restarting, notify user.
        CREATEWIDGET(OkDialog,
            // TRANSLATORS: video settings warning
            _("Changing to OpenGL"),
            // TRANSLATORS: video settings warning
            _("Applying change to OpenGL requires restart."),
            // TRANSLATORS: ok dialog button
            _("OK"),
            DialogType::OK,
            Modal_true,
            ShowCenter_true,
            nullptr,
            260);
    }

    mFps = mFpsCheckBox->isSelected() ?
        CAST_S32(mFpsSlider->getValue()) : 0;

    mAltFps = CAST_S32(mAltFpsSlider->getValue());

    mFpsSlider->setEnabled(mFps > 0);

    mAltFpsSlider->setEnabled(mAltFps > 0);

    // FPS change
    config.setValue("fpslimit", mFps);
    config.setValue("altfpslimit", mAltFps);

    // We sync old and new values at apply time
    mFullScreenEnabled = config.getBoolValue("screen");
    mCustomCursorEnabled = config.getBoolValue("customcursor");

    mOpenGLEnabled = intToRenderType(config.getIntValue("opengl"));
    mEnableResize = config.getBoolValue("enableresize");
    mNoFrame = config.getBoolValue("noframe");
}

void Setup_Video::cancel()
{
    mFpsCheckBox->setSelected(mFps > 0);
    mFsCheckBox->setSelected(mFullScreenEnabled);
    mOpenGLDropDown->setSelected(renderToIndex[mOpenGLEnabled]);
    mCustomCursorCheckBox->setSelected(mCustomCursorEnabled);
    mFpsSlider->setEnabled(mFps > 0);
    mFpsSlider->setValue(mFps);
    mAltFpsSlider->setEnabled(mAltFps > 0);
    mAltFpsSlider->setValue(mAltFps);
    mFpsLabel->setCaption(mFpsCheckBox->isSelected()
    // TRANSLATORS: video settings label
                          ? toString(mFps) : _("None"));
    // TRANSLATORS: video settings label
    mAltFpsLabel->setCaption(_("Alt FPS limit: ") + toString(mAltFps));
    mEnableResizeCheckBox->setSelected(mEnableResize);
    mNoFrameCheckBox->setSelected(mNoFrame);

    config.setValue("screen", mFullScreenEnabled);

    // Set back to the current video mode.
    std::string videoMode = toString(mainGraphics->mActualWidth).append("x")
        .append(toString(mainGraphics->mActualHeight));
    mModeList->setSelected(mModeListModel->getIndexOf(videoMode));
    config.setValue("screenwidth", mainGraphics->mActualWidth);
    config.setValue("screenheight", mainGraphics->mActualHeight);

    config.setValue("customcursor", mCustomCursorEnabled);
    config.setValue("opengl", CAST_S32(mOpenGLEnabled));
    config.setValue("enableresize", mEnableResize);
    config.setValue("noframe", mNoFrame);
}

void Setup_Video::action(const ActionEvent &event)
{
    const std::string &id = event.getId();

    if (id == "videomode")
    {
        std::string mode = mModeListModel->getElementAt(
            mModeList->getSelected());

        if (mode == "custom")
        {
            if (mDialog)
            {
                mode = mDialog->getText();
                mDialog = nullptr;
            }
            else
            {
                CREATEWIDGETV(mDialog, TextDialog,
                    // TRANSLATORS: resolution question dialog
                    _("Custom resolution (example: 1024x768)"),
                    // TRANSLATORS: resolution question dialog
                    _("Enter new resolution:                "));
                mDialog->setActionEventId("videomode");
                mDialog->addActionListener(this);
                return;
            }
        }
        const int width = atoi(mode.substr(0, mode.find('x')).c_str());
        const int height = atoi(mode.substr(mode.find('x') + 1).c_str());
        if (!width || !height)
            return;

        if (width != mainGraphics->mActualWidth
            || height != mainGraphics->mActualHeight)
        {
#if defined(WIN32) || defined(__APPLE__) || defined(ANDROID)
            if (intToRenderType(config.getIntValue("opengl"))
                == RENDER_SOFTWARE)
            {
                WindowManager::doResizeVideo(width, height, false);
            }
            else
            {
                if (width < mainGraphics->mActualWidth
                    || height < mainGraphics->mActualHeight)
                {
                    CREATEWIDGET(OkDialog,
                        // TRANSLATORS: video settings warning
                        _("Screen Resolution Changed"),
                        // TRANSLATORS: video settings warning
                       _("Restart your client for the change to take effect.")
                        // TRANSLATORS: video settings warning
                       + std::string("\n") + _("Some windows may be moved to "
                        "fit the lowered resolution."),
                        // TRANSLATORS: ok dialog button
                        _("OK"),
                        DialogType::OK,
                        Modal_true,
                        ShowCenter_true,
                        nullptr,
                        260);
                }
                else
                {
                    CREATEWIDGET(OkDialog,
                        // TRANSLATORS: video settings warning
                        _("Screen Resolution Changed"),
                        // TRANSLATORS: video settings warning
                        _("Restart your client for the change"
                        " to take effect."),
                        // TRANSLATORS: ok dialog button
                        _("OK"),
                        DialogType::OK,
                        Modal_true,
                        ShowCenter_true,
                        nullptr,
                        260);
                }
            }
#else  // defined(WIN32) || defined(__APPLE__) || defined(ANDROID)

            mainGraphics->setWindowSize(width, height);
            WindowManager::doResizeVideo(width, height, false);
#endif  // defined(WIN32) || defined(__APPLE__) || defined(ANDROID)
        }

        config.setValue("oldscreen", config.getBoolValue("screen"));
        config.setValue("oldscreenwidth", mainGraphics->mActualWidth);
        config.setValue("oldscreenheight", mainGraphics->mActualHeight);
        config.setValue("screenwidth", width);
        config.setValue("screenheight", height);
    }
    if (id == "~videomode")
    {
        mDialog = nullptr;
    }
    else if (id == "customcursor")
    {
        config.setValue("customcursor", mCustomCursorCheckBox->isSelected());
    }
    else if (id == "fpslimitcheckbox" || id == "fpslimitslider")
    {
        int tempFps = CAST_S32(mFpsSlider->getValue());
        if (id == "fpslimitcheckbox" && !mFpsSlider->isEnabled())
            tempFps = 60;
        else
            tempFps = tempFps > 0 ? tempFps : 60;
        mFps = mFpsCheckBox->isSelected() ? tempFps : 0;
        // TRANSLATORS: video settings label
        const std::string text = mFps > 0 ? toString(mFps) : _("None");

        mFpsLabel->setCaption(text);
        mFpsSlider->setEnabled(mFps > 0);
        mFpsSlider->setValue(mFps);
    }
    else if (id == "altfpslimitslider")
    {
        int tempFps = CAST_S32(mAltFpsSlider->getValue());
        tempFps = tempFps > 0 ? tempFps : CAST_S32(
            mAltFpsSlider->getScaleStart());
        mAltFps = tempFps;
        // TRANSLATORS: video settings label
        const std::string text = mAltFps > 0 ? toString(mAltFps) : _("None");

        // TRANSLATORS: video settings label
        mAltFpsLabel->setCaption(_("Alt FPS limit: ") + text);
        mAltFpsSlider->setEnabled(mAltFps > 0);
        mAltFpsSlider->setValue(mAltFps);
    }
    else if (id == "enableresize")
    {
        config.setValue("enableresize", mEnableResizeCheckBox->isSelected());
    }
    else if (id == "noframe")
    {
        config.setValue("noframe", mNoFrameCheckBox->isSelected());
    }
#if defined(USE_OPENGL) && !defined(ANDROID) && !defined(__APPLE__)
    else if (id == "detect")
    {
        TestMain *test = graphicsManager.startDetection();
        if (test)
        {
            Configuration &conf = test->getConfig();
            const int val = conf.getValueInt("opengl", -1);
            if (val >= 0 && CAST_U32(val)
                < sizeof(renderToIndex) / sizeof(int))
            {
                mOpenGLDropDown->setSelected(renderToIndex[val]);
            }
            config.setValue("textureSize",
                conf.getValue("textureSize", "1024,1024,1024,1024,1024,1024"));
            config.setValue("testInfo", conf.getValue("testInfo", ""));
            delete test;
        }
    }
#endif  // defined(USE_OPENGL) && !defined(ANDROID) && !defined(__APPLE__)
}
