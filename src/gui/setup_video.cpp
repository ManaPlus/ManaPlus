/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "gui/setup_video.h"

#include "configuration.h"
#include "game.h"
#include "graphics.h"
#include "localplayer.h"
#include "logger.h"
#include "main.h"
#include "particle.h"

#include "gui/gui.h"
#include "gui/okdialog.h"
#include "gui/textdialog.h"

#include "gui/widgets/checkbox.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/slider.h"
#include "gui/widgets/textfield.h"
#include "gui/widgets/dropdown.h"

#include "resources/image.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <guichan/key.hpp>
#include <guichan/listmodel.hpp>

#include <SDL.h>

#include <algorithm>
#include <string>
#include <vector>

#include "debug.h"

extern Graphics *mainGraphics;

/**
 * The list model for mode list.
 *
 * \ingroup Interface
 */
class ModeListModel : public gcn::ListModel
{
    public:
        /**
         * Constructor.
         */
        ModeListModel();

        /**
         * Destructor.
         */
        virtual ~ModeListModel()
        { }

        /**
         * Returns the number of elements in container.
         */
        int getNumberOfElements()
        { return static_cast<int>(mVideoModes.size()); }

        /**
         * Returns element from container.
         */
        std::string getElementAt(int i)
        { return mVideoModes[i]; }

        /**
         * Returns the index corresponding to the given video mode.
         * E.g.: "800x600".
         * or -1 if not found.
         */
        int getIndexOf(const std::string &widthXHeightMode);

    private:
        void addCustomMode(std::string mode);

        std::vector<std::string> mVideoModes;
};

bool modeSorter(std::string mode1, std::string mode2);

bool modeSorter(std::string mode1, std::string mode2)
{
    const int width1 = atoi(mode1.substr(0, mode1.find("x")).c_str());
    const int height1 = atoi(mode1.substr(mode1.find("x") + 1).c_str());
    if (!width1 || !height1)
        return false;

    const int width2 = atoi(mode2.substr(0, mode2.find("x")).c_str());
    const int height2 = atoi(mode2.substr(mode2.find("x") + 1).c_str());
    if (!width2 || !height2)
        return false;
    if (width1 != width2)
        return width1 < width2;

    if (height1 != height2)
        return height1 < height2;

    return false;
}

ModeListModel::ModeListModel()
{
    /* Get available fullscreen/hardware modes */
    SDL_Rect **modes = SDL_ListModes(nullptr, SDL_FULLSCREEN | SDL_HWSURFACE);

    /* Check which modes are available */
    if (modes == static_cast<SDL_Rect **>(nullptr))
    {
        logger->log1("No modes available");
    }
    else if (modes == reinterpret_cast<SDL_Rect **>(-1))
    {
        logger->log1("All resolutions available");
    }
    else
    {
        for (int i = 0; modes[i]; ++i)
        {
            const std::string modeString =
                toString(static_cast<int>(modes[i]->w)) + "x"
                    + toString(static_cast<int>(modes[i]->h));
            mVideoModes.push_back(modeString);
        }
    }
    addCustomMode("640x480");
    addCustomMode("800x600");
    addCustomMode("1024x768");
    addCustomMode("1280x1024");
    addCustomMode("1400x900");
    addCustomMode("1500x990");
    addCustomMode(toString(mainGraphics->mWidth) + "x"
        + toString(mainGraphics->mHeight));

    std::sort(mVideoModes.begin(), mVideoModes.end(), modeSorter);
    mVideoModes.push_back("custom");
}

void ModeListModel::addCustomMode(std::string mode)
{
    std::vector<std::string>::const_iterator it = mVideoModes.begin();
    std::vector<std::string>::const_iterator it_end = mVideoModes.end();
    while (it != it_end)
    {
        if (*it == mode)
            return;
        ++ it;
    }
    mVideoModes.push_back(mode);
}

int ModeListModel::getIndexOf(const std::string &widthXHeightMode)
{
    std::string currentMode("");
    for (int i = 0; i < getNumberOfElements(); i++)
    {
        currentMode = getElementAt(i);
        if (currentMode == widthXHeightMode)
            return i;
    }
    return -1;
}

const char *OPENGL_NAME[3] =
{
    N_("Software"),
    N_("Fast OpenGL"),
    N_("Safe OpenGL"),
};

class OpenGLListModel : public gcn::ListModel
{
public:
    virtual ~OpenGLListModel()
    { }

    virtual int getNumberOfElements()
    { return 3; }

    virtual std::string getElementAt(int i)
    {
        if (i >= getNumberOfElements() || i < 0)
            return _("???");

        return gettext(OPENGL_NAME[i]);
    }
};

static const char *speechModeToString(Being::Speech mode)
{
    switch (mode)
    {
        case Being::NO_SPEECH:
        default:
            return _("No text");
        case Being::TEXT_OVERHEAD:
            return _("Text");
        case Being::NO_NAME_IN_BUBBLE:
            return _("Bubbles, no names");
        case Being::NAME_IN_BUBBLE:
            return _("Bubbles with names");
    }
    return "";
}

const char *Setup_Video::overlayDetailToString(int detail)
{
    if (detail == -1)
        detail = config.getIntValue("OverlayDetail");

    switch (detail)
    {
        case 0:
            return _("off");
        case 1:
            return _("low");
        case 2:
            return _("high");
        default:
            return "";
    }
    return "";
}

const char *Setup_Video::particleDetailToString(int detail)
{
    if (detail == -1)
        detail = 3 - config.getIntValue("particleEmitterSkip");

    switch (detail)
    {
        case 0:
            return _("low");
        case 1:
            return _("medium");
        case 2:
            return _("high");
        case 3:
            return _("max");
        default:
            return "";
    }
    return "";
}

Setup_Video::Setup_Video():
    mFullScreenEnabled(config.getBoolValue("screen")),
    mOpenGLEnabled(config.getIntValue("opengl")),
    mCustomCursorEnabled(config.getBoolValue("customcursor")),
    mParticleEffectsEnabled(config.getBoolValue("particleeffects")),
    mPickupChatEnabled(config.getBoolValue("showpickupchat")),
    mPickupParticleEnabled(config.getBoolValue("showpickupparticle")),
    mOpacity(config.getFloatValue("guialpha")),
    mFps(config.getIntValue("fpslimit")),
    mAltFps(config.getIntValue("altfpslimit")),
    mSpeechMode(static_cast<Being::Speech>(
        config.getIntValue("speech"))),
    mModeListModel(new ModeListModel),
    mModeList(new ListBox(mModeListModel)),
    mFsCheckBox(new CheckBox(_("Full screen"), mFullScreenEnabled)),
    mCustomCursorCheckBox(new CheckBox(_("Custom cursor"),
                          mCustomCursorEnabled)),
    mParticleEffectsCheckBox(new CheckBox(_("Particle effects"),
                             mParticleEffectsEnabled)),
    mPickupNotifyLabel(new Label(_("Show pickup notification"))),
    // TRANSLATORS: Refers to "Show own name"
    mPickupChatCheckBox(new CheckBox(_("in chat"), mPickupChatEnabled)),
    // TRANSLATORS: Refers to "Show own name"
    mPickupParticleCheckBox(new CheckBox(_("as particle"),
                            mPickupParticleEnabled)),
    mSpeechSlider(new Slider(0, 3)),
    mSpeechLabel(new Label("")),
    mAlphaSlider(new Slider(0.1, 1.0)),
    mFpsCheckBox(new CheckBox(_("FPS limit:"))),
    mFpsSlider(new Slider(2, 160)),
    mFpsLabel(new Label),
    mAltFpsSlider(new Slider(2, 160)),
    mAltFpsLabel(new Label(_("Alt FPS limit: "))),
    mOverlayDetail(config.getIntValue("OverlayDetail")),
    mOverlayDetailSlider(new Slider(0, 2)),
    mOverlayDetailField(new Label),
    mParticleDetail(3 - config.getIntValue("particleEmitterSkip")),
    mParticleDetailSlider(new Slider(0, 3)),
    mParticleDetailField(new Label),
    mDialog(nullptr)
{
    setName(_("Video"));

    ScrollArea *scrollArea = new ScrollArea(mModeList);
    scrollArea->setWidth(150);
    scrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

    speechLabel = new Label(_("Overhead text"));
    alphaLabel = new Label(_("Gui opacity"));
    overlayDetailLabel = new Label(_("Ambient FX"));
    particleDetailLabel = new Label(_("Particle detail"));

    mOpenGLListModel = new OpenGLListModel;
    mOpenGLDropDown = new DropDown(mOpenGLListModel),
    mOpenGLDropDown->setSelected(mOpenGLEnabled);

    mModeList->setEnabled(true);

#ifndef USE_OPENGL
    mOpenGLDropDown->setSelected(0);
#endif

    mAlphaSlider->setValue(mOpacity);
    mAlphaSlider->setWidth(90);

    mFpsLabel->setCaption(mFps > 0 ? toString(mFps) : _("None"));
    mFpsLabel->setWidth(60);
    mAltFpsLabel->setCaption(_("Alt FPS limit: ") + (mAltFps > 0
                             ? toString(mAltFps) : _("None")));
    mAltFpsLabel->setWidth(150);
    mFpsSlider->setValue(mFps);
    mFpsSlider->setEnabled(mFps > 0);
    mAltFpsSlider->setValue(mAltFps);
    mAltFpsSlider->setEnabled(mAltFps > 0);
    mFpsCheckBox->setSelected(mFps > 0);

    // Pre-select the current video mode.
    std::string videoMode = toString(mainGraphics->mWidth) + "x"
        + toString(mainGraphics->mHeight);
    mModeList->setSelected(mModeListModel->getIndexOf(videoMode));

    mModeList->setActionEventId("videomode");
    mCustomCursorCheckBox->setActionEventId("customcursor");
    mParticleEffectsCheckBox->setActionEventId("particleeffects");
    mPickupChatCheckBox->setActionEventId("pickupchat");
    mPickupParticleCheckBox->setActionEventId("pickupparticle");
    mAlphaSlider->setActionEventId("guialpha");
    mFpsCheckBox->setActionEventId("fpslimitcheckbox");
    mSpeechSlider->setActionEventId("speech");
    mFpsSlider->setActionEventId("fpslimitslider");
    mAltFpsSlider->setActionEventId("altfpslimitslider");
    mOverlayDetailSlider->setActionEventId("overlaydetailslider");
    mOverlayDetailField->setActionEventId("overlaydetailfield");
    mParticleDetailSlider->setActionEventId("particledetailslider");
    mParticleDetailField->setActionEventId("particledetailfield");
    mOpenGLDropDown->setActionEventId("opengl");

    mModeList->addActionListener(this);
    mCustomCursorCheckBox->addActionListener(this);
    mParticleEffectsCheckBox->addActionListener(this);
    mPickupChatCheckBox->addActionListener(this);
    mPickupParticleCheckBox->addActionListener(this);
    mAlphaSlider->addActionListener(this);
    mFpsCheckBox->addActionListener(this);
    mSpeechSlider->addActionListener(this);
    mFpsSlider->addActionListener(this);
    mAltFpsSlider->addActionListener(this);
    mOverlayDetailSlider->addActionListener(this);
    mOverlayDetailField->addKeyListener(this);
    mParticleDetailSlider->addActionListener(this);
    mParticleDetailField->addKeyListener(this);
    mOpenGLDropDown->addActionListener(this);

    mSpeechLabel->setCaption(speechModeToString(mSpeechMode));
    mSpeechSlider->setValue(mSpeechMode);

    mOverlayDetailField->setCaption(overlayDetailToString(mOverlayDetail));
    mOverlayDetailSlider->setValue(mOverlayDetail);

    mParticleDetailField->setCaption(particleDetailToString(mParticleDetail));
    mParticleDetailSlider->setValue(mParticleDetail);

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, scrollArea, 1, 5).setPadding(2);
    place(0, 5, mOpenGLDropDown, 1);

//    place(0, 6, mHwAccelCheckBox, 6);

    place(1, 0, mFsCheckBox, 2);

    place(1, 1, mCustomCursorCheckBox, 3);

    place(1, 2, mParticleEffectsCheckBox, 2);

    place(1, 3, mPickupNotifyLabel, 4);

    place(1, 4, mPickupChatCheckBox, 1);
    place(2, 4, mPickupParticleCheckBox, 2);

    place(0, 6, mAlphaSlider);
    place(1, 6, alphaLabel, 3);

    place(0, 7, mFpsSlider);
    place(1, 7, mFpsCheckBox).setPadding(3);
    place(2, 7, mFpsLabel).setPadding(1);

    place(0, 8, mAltFpsSlider);
    place(1, 8, mAltFpsLabel).setPadding(3);

    place(0, 9, mSpeechSlider);
    place(1, 9, speechLabel);
    place(2, 9, mSpeechLabel, 3).setPadding(2);

    place(0, 10, mOverlayDetailSlider);
    place(1, 10, overlayDetailLabel);
    place(2, 10, mOverlayDetailField, 3).setPadding(2);

    place(0, 11, mParticleDetailSlider);
    place(1, 11, particleDetailLabel);
    place(2, 11, mParticleDetailField, 3).setPadding(2);

    int width = 600;

    if (config.getIntValue("screenwidth") >= 730)
        width += 100;

    setDimension(gcn::Rectangle(0, 0, width, 300));
}

Setup_Video::~Setup_Video()
{
    delete mModeListModel;
    mModeListModel = nullptr;
    delete mModeList;
    mModeList = nullptr;
    delete mOpenGLListModel;
    mOpenGLListModel = nullptr;
    delete mDialog;
    mDialog = nullptr;
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

#if defined(WIN32) || defined(__APPLE__)
        // checks for opengl usage
        if (!config.getIntValue("opengl"))
        {
#endif
            if (!mainGraphics->setFullscreen(fullscreen))
            {
                fullscreen = !fullscreen;
                if (!mainGraphics->setFullscreen(fullscreen))
                {
                    std::stringstream errorMsg;
                    if (fullscreen)
                    {
                        errorMsg << _("Failed to switch to windowed mode "
                            "and restoration of old mode also "
                            "failed!") << std::endl;
                    }
                    else
                    {
                        errorMsg << _("Failed to switch to fullscreen mode"
                            " and restoration of old mode also "
                            "failed!") << std::endl;
                    }
                    logger->error(errorMsg.str());
                }
            }
#if defined(WIN32) || defined(__APPLE__)
        }
        else
        {
            new OkDialog(_("Switching to Full Screen"),
                         _("Restart needed for changes to take effect."));
        }
#endif
        config.setValue("screen", fullscreen);
    }

    // OpenGL change
    if (mOpenGLDropDown->getSelected() != mOpenGLEnabled)
    {
        config.setValue("opengl", mOpenGLDropDown->getSelected());

        // OpenGL can currently only be changed by restarting, notify user.
        new OkDialog(_("Changing to OpenGL"),
                     _("Applying change to OpenGL requires restart."));
    }

    mFps = mFpsCheckBox->isSelected() ?
        static_cast<int>(mFpsSlider->getValue()) : 0;

    mAltFps = static_cast<int>(mAltFpsSlider->getValue());

    mFpsSlider->setEnabled(mFps > 0);

    mAltFpsSlider->setEnabled(mAltFps > 0);

    // FPS change
    config.setValue("fpslimit", mFps);
    config.setValue("altfpslimit", mAltFps);

    // We sync old and new values at apply time
    mFullScreenEnabled = config.getBoolValue("screen");
    mCustomCursorEnabled = config.getBoolValue("customcursor");
    mParticleEffectsEnabled = config.getBoolValue("particleeffects");

    mSpeechMode = static_cast<Being::Speech>(
        config.getIntValue("speech"));
    mOpacity = config.getFloatValue("guialpha");
    mOverlayDetail = config.getIntValue("OverlayDetail");
    mOpenGLEnabled = config.getIntValue("opengl");
    mPickupChatEnabled = config.getBoolValue("showpickupchat");
    mPickupParticleEnabled = config.getBoolValue("showpickupparticle");
}

void Setup_Video::cancel()
{
    mFpsCheckBox->setSelected(mFps > 0);
    mFsCheckBox->setSelected(mFullScreenEnabled);
    mOpenGLDropDown->setSelected(mOpenGLEnabled);
    mCustomCursorCheckBox->setSelected(mCustomCursorEnabled);
    mParticleEffectsCheckBox->setSelected(mParticleEffectsEnabled);
    mFpsSlider->setValue(mFps);
    mFpsSlider->setEnabled(mFps > 0);
    mAltFpsSlider->setValue(mAltFps);
    mAltFpsSlider->setEnabled(mAltFps > 0);
    mSpeechSlider->setValue(mSpeechMode);
    mAlphaSlider->setValue(mOpacity);
    mOverlayDetailSlider->setValue(mOverlayDetail);
    mParticleDetailSlider->setValue(mParticleDetail);
    mFpsLabel->setCaption(mFpsCheckBox->isSelected()
                          ? toString(mFps) : _("None"));
    mAltFpsLabel->setCaption(_("Alt FPS limit: ") + toString(mAltFps));

    config.setValue("screen", mFullScreenEnabled);

    // Set back to the current video mode.
    std::string videoMode = toString(mainGraphics->mWidth) + "x"
        + toString(mainGraphics->mHeight);
    mModeList->setSelected(mModeListModel->getIndexOf(videoMode));
    config.setValue("screenwidth", mainGraphics->mWidth);
    config.setValue("screenheight", mainGraphics->mHeight);

    config.setValue("customcursor", mCustomCursorEnabled);
    config.setValue("particleeffects", mParticleEffectsEnabled);
    config.setValue("speech", static_cast<int>(mSpeechMode));
    config.setValue("guialpha", mOpacity);
    Image::setEnableAlpha(mOpacity != 1.0f);
    config.setValue("opengl", mOpenGLEnabled);
    config.setValue("showpickupchat", mPickupChatEnabled);
    config.setValue("showpickupparticle", mPickupParticleEnabled);
}

void Setup_Video::action(const gcn::ActionEvent &event)
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
                mDialog = new TextDialog(
                    _("Custom resolution (example: 1024x768)"),
                    _("Enter new resolution:                "));
                mDialog->setActionEventId("videomode");
                mDialog->addActionListener(this);
                return;
            }
        }
        const int width = atoi(mode.substr(0, mode.find("x")).c_str());
        const int height = atoi(mode.substr(mode.find("x") + 1).c_str());
        if (!width || !height)
            return;

        // TODO: Find out why the drawing area doesn't resize without a restart.
        if (width != mainGraphics->mWidth || height != mainGraphics->mHeight)
        {
            if (width < mainGraphics->mWidth || height < mainGraphics->mHeight)
                new OkDialog(_("Screen Resolution Changed"),
                       _("Restart your client for the change to take effect.")
                       + std::string("\n") +
                _("Some windows may be moved to fit the lowered resolution."));
            else
                new OkDialog(_("Screen Resolution Changed"),
                     _("Restart your client for the change to take effect."));
        }

        config.setValue("oldscreen", config.getBoolValue("screen"));
        config.setValue("oldscreenwidth", mainGraphics->mWidth);
        config.setValue("oldscreenheight", mainGraphics->mHeight);
        config.setValue("screenwidth", width);
        config.setValue("screenheight", height);
    }
    if (id == "~videomode")
    {
        mDialog = nullptr;
    }
    else if (id == "guialpha")
    {
        config.setValue("guialpha", mAlphaSlider->getValue());
        Image::setEnableAlpha(config.getFloatValue("guialpha") != 1.0f);
    }
    else if (id == "customcursor")
    {
        config.setValue("customcursor", mCustomCursorCheckBox->isSelected());
    }
    else if (id == "particleeffects")
    {
        config.setValue("particleeffects",
                        mParticleEffectsCheckBox->isSelected());
        Particle::enabled = mParticleEffectsCheckBox->isSelected();

        if (Game::instance())
        {
            new OkDialog(_("Particle Effect Settings Changed."),
                         _("Changes will take effect on map change."));
        }
    }
    else if (id == "pickupchat")
    {
        config.setValue("showpickupchat", mPickupChatCheckBox->isSelected());
    }
    else if (id == "pickupparticle")
    {
        config.setValue("showpickupparticle",
                        mPickupParticleCheckBox->isSelected());
    }
    else if (id == "speech")
    {
        Being::Speech val = static_cast<Being::Speech>(
            mSpeechSlider->getValue());
        mSpeechLabel->setCaption(speechModeToString(val));
        mSpeechSlider->setValue(val);
        config.setValue("speech", static_cast<int>(val));
    }
    else if (id == "overlaydetailslider")
    {
        int val = static_cast<int>(mOverlayDetailSlider->getValue());
        mOverlayDetailField->setCaption(overlayDetailToString(val));
        config.setValue("OverlayDetail", val);
    }
    else if (id == "particledetailslider")
    {
        int val = static_cast<int>(mParticleDetailSlider->getValue());
        mParticleDetailField->setCaption(particleDetailToString(val));
        config.setValue("particleEmitterSkip", 3 - val);
        Particle::emitterSkip = 4 - val;
    }
    else if (id == "fpslimitcheckbox" || id == "fpslimitslider")
    {
        int tempFps = static_cast<int>(mFpsSlider->getValue());
        if (id == "fpslimitcheckbox" && !mFpsSlider->isEnabled())
            tempFps = 60;
        else
            tempFps = tempFps > 0 ? tempFps : 60;
        mFps = mFpsCheckBox->isSelected() ? tempFps : 0;
        const std::string text = mFps > 0 ? toString(mFps) : _("None");

        mFpsLabel->setCaption(text);
        mFpsSlider->setValue(mFps);
        mFpsSlider->setEnabled(mFps > 0);
    }
    else if (id == "altfpslimitslider")
    {
        int tempFps = static_cast<int>(mAltFpsSlider->getValue());
        tempFps = tempFps > 0 ? tempFps : static_cast<int>(
            mAltFpsSlider->getScaleStart());
        mAltFps = tempFps;
        const std::string text = mAltFps > 0 ? toString(mAltFps) : _("None");

        mAltFpsLabel->setCaption(_("Alt FPS limit: ") + text);
        mAltFpsSlider->setValue(mAltFps);
        mAltFpsSlider->setEnabled(mAltFps > 0);
    }
}

void Setup_Video::externalUpdated()
{
}