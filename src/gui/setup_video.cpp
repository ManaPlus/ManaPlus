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
#include "log.h"
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

#include <string>
#include <vector>

#include "debug.h"

extern Graphics *graphics;

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
        std::vector<std::string> mVideoModes;
};

ModeListModel::ModeListModel()
{
    /* Get available fullscreen/hardware modes */
    SDL_Rect **modes = SDL_ListModes(NULL, SDL_FULLSCREEN | SDL_HWSURFACE);

    /* Check which modes are available */
    if (modes == static_cast<SDL_Rect **>(0))
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
    mVideoModes.push_back("custom");
}

int ModeListModel::getIndexOf(const std::string &widthXHeightMode)
{
    std::string currentMode = "";
    for (int i = 0; i < getNumberOfElements(); i++)
    {
        currentMode = getElementAt(i);
        if (currentMode == widthXHeightMode)
            return i;
    }
    return -1;
}

const char *SIZE_NAME[6] =
{
    N_("Tiny (10)"),
    N_("Small (11)"),
    N_("Medium (12)"),
    N_("Large (13)"),
    N_("Big (14)"),
    N_("Huge (15)"),
};

class FontSizeChoiceListModel : public gcn::ListModel
{
public:
    virtual ~FontSizeChoiceListModel()
    { }

    virtual int getNumberOfElements()
    { return 6; }

    virtual std::string getElementAt(int i)
    {
        if (i >= getNumberOfElements() || i < 0)
            return _("???");

        return gettext(SIZE_NAME[i]);
    }
};

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
    mHwAccelEnabled(config.getBoolValue("hwaccel")),
    mCustomCursorEnabled(config.getBoolValue("customcursor")),
    mVisibleNamesEnabled(config.getBoolValue("visiblenames")),
    mParticleEffectsEnabled(config.getBoolValue("particleeffects")),
    mNPCLogEnabled(config.getBoolValue("logNpcInGui")),
    mPickupChatEnabled(config.getBoolValue("showpickupchat")),
    mPickupParticleEnabled(config.getBoolValue("showpickupparticle")),
    mOpacity(config.getFloatValue("guialpha")),
    mFps(config.getIntValue("fpslimit")),
    mAltFps(config.getIntValue("altfpslimit")),
    mHideShieldSprite(config.getBoolValue("hideShield")),
    mLowTraffic(config.getBoolValue("lowTraffic")),
    mSyncPlayerMove(config.getBoolValue("syncPlayerMove")),
    mDrawHotKeys(config.getBoolValue("drawHotKeys")),
    mDrawPath(config.getBoolValue("drawPath")),
    mShowJob(serverConfig.getBoolValue("showJob")),
    mAlphaCache(config.getBoolValue("alphaCache")),
    mShowBackground(config.getBoolValue("showBackground")),
    mSpeechMode(static_cast<Being::Speech>(
        config.getIntValue("speech"))),
    mModeListModel(new ModeListModel),
    mModeList(new ListBox(mModeListModel)),
    mFsCheckBox(new CheckBox(_("Full screen"), mFullScreenEnabled)),
    mHwAccelCheckBox(new CheckBox(_("Hw acceleration"), mHwAccelEnabled)),
    mCustomCursorCheckBox(new CheckBox(_("Custom cursor"),
                          mCustomCursorEnabled)),
    mVisibleNamesCheckBox(new CheckBox(_("Visible names"),
                          mVisibleNamesEnabled)),
    mParticleEffectsCheckBox(new CheckBox(_("Particle effects"),
                             mParticleEffectsEnabled)),
    mNPCLogCheckBox(new CheckBox(_("Log NPC dialogue"), mNPCLogEnabled)),
    mPickupNotifyLabel(new Label(_("Show pickup notification"))),
    // TRANSLATORS: Refers to "Show own name"
    mPickupChatCheckBox(new CheckBox(_("in chat"), mPickupChatEnabled)),
    // TRANSLATORS: Refers to "Show own name"
    mPickupParticleCheckBox(new CheckBox(_("as particle"),
                            mPickupParticleEnabled)),
    mHideShieldSpriteCheckBox(new CheckBox(_("Hide shield sprite"),
                              mHideShieldSprite)),
    mLowTrafficCheckBox(new CheckBox(_("Low traffic mode"),
                        mLowTraffic)),
    mSyncPlayerMoveCheckBox(new CheckBox(_("Sync player move"),
                            mSyncPlayerMove)),
    mDrawHotKeysCheckBox(new CheckBox(_("Draw hotkeys on map"),
                         mDrawHotKeys)),
    mDrawPathCheckBox(new CheckBox(_("Draw path"), mDrawPath)),
    mShowJobCheckBox(new CheckBox(_("Show job"), mShowJob)),
    mAlphaCacheCheckBox(new CheckBox(_("Enable opacity cache"), mAlphaCache)),
    mShowBackgroundCheckBox(new CheckBox(_("Show background"),
                            mShowBackground)),
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
    mFontSize(config.getIntValue("fontSize")),
    mDialog(0)
{
    setName(_("Video"));

    ScrollArea *scrollArea = new ScrollArea(mModeList);
    scrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

    speechLabel = new Label(_("Overhead text"));
    alphaLabel = new Label(_("Gui opacity"));
    overlayDetailLabel = new Label(_("Ambient FX"));
    particleDetailLabel = new Label(_("Particle detail"));
    fontSizeLabel = new Label(_("Font size"));

    mOpenGLListModel = new OpenGLListModel;
    mOpenGLDropDown = new DropDown(mOpenGLListModel),
    mOpenGLDropDown->setSelected(mOpenGLEnabled);

    mFontSizeListModel = new FontSizeChoiceListModel;
    mFontSizeDropDown = new DropDown(mFontSizeListModel);

    mModeList->setEnabled(true);

#ifndef USE_OPENGL
    mOpenGLDropDown->setSelected(0);
#endif

    mAlphaSlider->setValue(mOpacity);
    mAlphaSlider->setWidth(90);
    mAlphaCacheCheckBox->setEnabled(mOpenGLDropDown->getSelected() == 0);

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
    std::string videoMode = toString(graphics->getWidth()) + "x"
                            + toString(graphics->getHeight());
    mModeList->setSelected(mModeListModel->getIndexOf(videoMode));

    mModeList->setActionEventId("videomode");
    mCustomCursorCheckBox->setActionEventId("customcursor");
    mVisibleNamesCheckBox->setActionEventId("visiblenames");
    mParticleEffectsCheckBox->setActionEventId("particleeffects");
    mPickupChatCheckBox->setActionEventId("pickupchat");
    mPickupParticleCheckBox->setActionEventId("pickupparticle");
    mNPCLogCheckBox->setActionEventId("lognpc");
    mAlphaSlider->setActionEventId("guialpha");
    mFpsCheckBox->setActionEventId("fpslimitcheckbox");
    mSpeechSlider->setActionEventId("speech");
    mFpsSlider->setActionEventId("fpslimitslider");
    mAltFpsSlider->setActionEventId("altfpslimitslider");
    mOverlayDetailSlider->setActionEventId("overlaydetailslider");
    mOverlayDetailField->setActionEventId("overlaydetailfield");
    mParticleDetailSlider->setActionEventId("particledetailslider");
    mParticleDetailField->setActionEventId("particledetailfield");
    mHideShieldSpriteCheckBox->setActionEventId("hideshieldsprite1");
    mLowTrafficCheckBox->setActionEventId("lowTraffic1");
    mSyncPlayerMoveCheckBox->setActionEventId("syncPlayerMove1");
    mDrawHotKeysCheckBox->setActionEventId("drawHotKeys");
    mDrawPathCheckBox->setActionEventId("drawPath1");
    mShowJobCheckBox->setActionEventId("showJob");
    mAlphaCacheCheckBox->setActionEventId("alphaCache");
    mOpenGLDropDown->setActionEventId("opengl");

    mModeList->addActionListener(this);
    mCustomCursorCheckBox->addActionListener(this);
    mVisibleNamesCheckBox->addActionListener(this);
    mParticleEffectsCheckBox->addActionListener(this);
    mPickupChatCheckBox->addActionListener(this);
    mPickupParticleCheckBox->addActionListener(this);
    mNPCLogCheckBox->addActionListener(this);
    mAlphaSlider->addActionListener(this);
    mFpsCheckBox->addActionListener(this);
    mSpeechSlider->addActionListener(this);
    mFpsSlider->addActionListener(this);
    mAltFpsSlider->addActionListener(this);
    mOverlayDetailSlider->addActionListener(this);
    mOverlayDetailField->addKeyListener(this);
    mParticleDetailSlider->addActionListener(this);
    mParticleDetailField->addKeyListener(this);
    mHideShieldSpriteCheckBox->addKeyListener(this);
    mLowTrafficCheckBox->addKeyListener(this);
    mSyncPlayerMoveCheckBox->addKeyListener(this);
    mDrawHotKeysCheckBox->addKeyListener(this);
    mDrawPathCheckBox->addKeyListener(this);
    mShowJobCheckBox->addKeyListener(this);
    mOpenGLDropDown->addActionListener(this);

    mAlphaCacheCheckBox->addKeyListener(this);

    mSpeechLabel->setCaption(speechModeToString(mSpeechMode));
    mSpeechSlider->setValue(mSpeechMode);

    mOverlayDetailField->setCaption(overlayDetailToString(mOverlayDetail));
    mOverlayDetailSlider->setValue(mOverlayDetail);

    mParticleDetailField->setCaption(particleDetailToString(mParticleDetail));
    mParticleDetailSlider->setValue(mParticleDetail);

    mFontSizeDropDown->setSelected(mFontSize - 10);
    mFontSizeDropDown->adjustHeight();

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, scrollArea, 1, 5).setPadding(2);
    place(1, 0, mFsCheckBox, 2);
    place(3, 0, mOpenGLDropDown, 1);

    place(1, 1, mCustomCursorCheckBox, 3);
    place(3, 1, mHwAccelCheckBox, 1);

    place(1, 2, mVisibleNamesCheckBox, 3);

    place(3, 2, mAlphaCacheCheckBox, 4);

    place(1, 3, mParticleEffectsCheckBox, 2);

    place(1, 4, mPickupNotifyLabel, 4);

    place(1, 5, mPickupChatCheckBox, 1);
    place(2, 5, mPickupParticleCheckBox, 2);

    place(0, 6, fontSizeLabel, 3);
    place(1, 6, mFontSizeDropDown, 1);

    place(0, 7, mAlphaSlider);
    place(1, 7, alphaLabel, 3);

    place(0, 8, mFpsSlider);
    place(1, 8, mFpsCheckBox).setPadding(3);
    place(2, 8, mFpsLabel).setPadding(1);

    place(0, 9, mAltFpsSlider);
    place(1, 9, mAltFpsLabel).setPadding(3);

    place(0, 10, mSpeechSlider);
    place(1, 10, speechLabel);
    place(2, 10, mSpeechLabel, 3).setPadding(2);

    place(0, 11, mOverlayDetailSlider);
    place(1, 11, overlayDetailLabel);
    place(2, 11, mOverlayDetailField, 3).setPadding(2);

    place(0, 12, mParticleDetailSlider);
    place(1, 12, particleDetailLabel);
    place(2, 12, mParticleDetailField, 3).setPadding(2);

    place(3, 7, mHideShieldSpriteCheckBox);
    place(3, 8, mLowTrafficCheckBox);
    place(0, 13, mShowBackgroundCheckBox);
    place(0, 14, mSyncPlayerMoveCheckBox);
    place(0, 15, mDrawHotKeysCheckBox);
    place(2, 13, mDrawPathCheckBox, 2);
    place(2, 14, mShowJobCheckBox, 2);
    place(2, 15, mNPCLogCheckBox, 2);

    int width = 600;

    if (config.getIntValue("screenwidth") >= 730)
        width += 100;

    setDimension(gcn::Rectangle(0, 0, width, 300));
}

Setup_Video::~Setup_Video()
{
    delete mModeListModel;
    mModeListModel = 0;
    delete mModeList;
    mModeList = 0;
    delete mFontSizeListModel;
    mFontSizeListModel = 0;
    delete mOpenGLListModel;
    mOpenGLListModel = 0;
    delete mDialog;
    mDialog = 0;
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
            if (!graphics->setFullscreen(fullscreen))
            {
                fullscreen = !fullscreen;
                if (!graphics->setFullscreen(fullscreen))
                {
                    std::stringstream errorMessage;
                    if (fullscreen)
                    {
                        errorMessage << _("Failed to switch to windowed mode "
                                          "and restoration of old mode also "
                                          "failed!") << std::endl;
                    }
                    else
                    {
                        errorMessage << _("Failed to switch to fullscreen mode"
                                          " and restoration of old mode also "
                                          "failed!") << std::endl;
                    }
                    logger->error(errorMessage.str());
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

    config.setValue("hwaccel", mHwAccelCheckBox->isSelected());

    // FPS change
    config.setValue("fpslimit", mFps);
    config.setValue("altfpslimit", mAltFps);

    if (config.getIntValue("fontSize")
        != static_cast<int>(mFontSizeDropDown->getSelected()) + 10)
    {
        config.setValue("fontSize", mFontSizeDropDown->getSelected() + 10);
        gui->updateFonts();
    }

    config.setValue("hideShield", mHideShieldSpriteCheckBox->isSelected());
    config.setValue("lowTraffic", mLowTrafficCheckBox->isSelected());
    config.setValue("syncPlayerMove", mSyncPlayerMoveCheckBox->isSelected());
    config.setValue("drawHotKeys", mDrawHotKeysCheckBox->isSelected());
    config.setValue("drawPath", mDrawPathCheckBox->isSelected());
    serverConfig.setValue("showJob", mShowJobCheckBox->isSelected());
    config.setValue("alphaCache", mAlphaCacheCheckBox->isSelected());
    config.setValue("showBackground", mShowBackgroundCheckBox->isSelected());

    // We sync old and new values at apply time
    mFullScreenEnabled = config.getBoolValue("screen");
    mCustomCursorEnabled = config.getBoolValue("customcursor");
    mVisibleNamesEnabled = config.getBoolValue("visiblenames");
    mParticleEffectsEnabled = config.getBoolValue("particleeffects");
    mNPCLogEnabled = config.getBoolValue("logNpcInGui");
    mHideShieldSprite = config.getBoolValue("hideShield");
    mLowTraffic = config.getBoolValue("lowTraffic");
    mSyncPlayerMove = config.getBoolValue("syncPlayerMove");
    mDrawHotKeys = config.getBoolValue("drawHotKeys");
    mDrawPath = config.getBoolValue("drawPath");
    mShowJob = serverConfig.getBoolValue("showJob");
    mAlphaCache = config.getBoolValue("alphaCache");
    mShowBackground = config.getBoolValue("showBackground");

    mSpeechMode = static_cast<Being::Speech>(
        config.getIntValue("speech"));
    mOpacity = config.getFloatValue("guialpha");
    mOverlayDetail = config.getIntValue("OverlayDetail");
    mOpenGLEnabled = config.getIntValue("opengl");
    mHwAccelEnabled = config.getBoolValue("hwaccel");
    mPickupChatEnabled = config.getBoolValue("showpickupchat");
    mPickupParticleEnabled = config.getBoolValue("showpickupparticle");
}

void Setup_Video::cancel()
{
    mFpsCheckBox->setSelected(mFps > 0);
    mFsCheckBox->setSelected(mFullScreenEnabled);
    mOpenGLDropDown->setSelected(mOpenGLEnabled);
    mHwAccelCheckBox->setSelected(mHwAccelEnabled);
    mCustomCursorCheckBox->setSelected(mCustomCursorEnabled);
    mVisibleNamesCheckBox->setSelected(mVisibleNamesEnabled);
    mParticleEffectsCheckBox->setSelected(mParticleEffectsEnabled);
    mFpsSlider->setValue(mFps);
    mFpsSlider->setEnabled(mFps > 0);
    mAltFpsSlider->setValue(mAltFps);
    mAltFpsSlider->setEnabled(mAltFps > 0);
    mSpeechSlider->setValue(mSpeechMode);
    mNPCLogCheckBox->setSelected(mNPCLogEnabled);
    mHideShieldSpriteCheckBox->setSelected(mHideShieldSprite);
    mLowTrafficCheckBox->setSelected(mLowTraffic);
    mSyncPlayerMoveCheckBox->setSelected(mSyncPlayerMove);
    mDrawHotKeysCheckBox->setSelected(mDrawHotKeys);
    mDrawPathCheckBox->setSelected(mDrawPath);
    mShowJobCheckBox->setSelected(mShowJob);
    mAlphaCacheCheckBox->setSelected(mAlphaCache);
    mShowBackgroundCheckBox->setSelected(mShowBackground);
    mAlphaSlider->setValue(mOpacity);
    mOverlayDetailSlider->setValue(mOverlayDetail);
    mParticleDetailSlider->setValue(mParticleDetail);
    mFpsLabel->setCaption(mFpsCheckBox->isSelected()
                          ? toString(mFps) : _("None"));
    mAltFpsLabel->setCaption(_("Alt FPS limit: ") + toString(mAltFps));

    config.setValue("screen", mFullScreenEnabled);

    // Set back to the current video mode.
    std::string videoMode = toString(graphics->getWidth()) + "x"
                            + toString(graphics->getHeight());
    mModeList->setSelected(mModeListModel->getIndexOf(videoMode));
    config.setValue("screenwidth", graphics->getWidth());
    config.setValue("screenheight", graphics->getHeight());

    config.setValue("customcursor", mCustomCursorEnabled);
    config.setValue("visiblenames", mVisibleNamesEnabled);
    config.setValue("particleeffects", mParticleEffectsEnabled);
    config.setValue("speech", static_cast<int>(mSpeechMode));
    config.setValue("logNpcInGui", mNPCLogEnabled);
    config.setValue("hideShield", mHideShieldSprite);
    config.setValue("lowTraffic", mLowTraffic);
    config.setValue("syncPlayerMove", mSyncPlayerMove);
    config.setValue("drawHotKeys", mDrawHotKeys);
    config.setValue("drawPath", mDrawPath);
    serverConfig.setValue("showJob", mShowJob);
    config.setValue("alphaCache", mAlphaCache);
    config.setValue("showBackground", mShowBackground);
    config.setValue("guialpha", mOpacity);
    Image::setEnableAlpha(mOpacity != 1.0f);
    config.setValue("opengl", mOpenGLEnabled);
    config.setValue("hwaccel", mHwAccelEnabled);
    config.setValue("showpickupchat", mPickupChatEnabled);
    config.setValue("showpickupparticle", mPickupParticleEnabled);
    mAlphaCacheCheckBox->setEnabled(mOpenGLDropDown->getSelected() == 0);
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
                mDialog = 0;
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
        if (width != graphics->getWidth() || height != graphics->getHeight())
        {
            if (width < graphics->getWidth() || height < graphics->getHeight())
                new OkDialog(_("Screen Resolution Changed"),
                       _("Restart your client for the change to take effect.")
                       + std::string("\n") +
                _("Some windows may be moved to fit the lowered resolution."));
            else
                new OkDialog(_("Screen Resolution Changed"),
                     _("Restart your client for the change to take effect."));
        }

        config.setValue("screenwidth", width);
        config.setValue("screenheight", height);
    }
    if (id == "~videomode")
    {
        mDialog = 0;
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
    else if (id == "visiblenames")
    {
        config.setValue("visiblenames", mVisibleNamesCheckBox->isSelected());
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
    else if (id == "lognpc")
    {
        config.setValue("logNpcInGui", mNPCLogCheckBox->isSelected());
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
        int fps = static_cast<int>(mFpsSlider->getValue());
        if (id == "fpslimitcheckbox" && !mFpsSlider->isEnabled())
            fps = 60;
        else
            fps = fps > 0 ? fps : 60;
        mFps = mFpsCheckBox->isSelected() ? fps : 0;
        const std::string text = mFps > 0 ? toString(mFps) : _("None");

        mFpsLabel->setCaption(text);
        mFpsSlider->setValue(mFps);
        mFpsSlider->setEnabled(mFps > 0);
    }
    else if (id == "altfpslimitslider")
    {
        int fps = static_cast<int>(mAltFpsSlider->getValue());
        fps = fps > 0 ? fps : static_cast<int>(mAltFpsSlider->getScaleStart());
        mAltFps = fps;
        const std::string text = mAltFps > 0 ? toString(mAltFps) : _("None");

        mAltFpsLabel->setCaption(_("Alt FPS limit: ") + text);
        mAltFpsSlider->setValue(mAltFps);
        mAltFpsSlider->setEnabled(mAltFps > 0);
    }
    else if (id == "opengl")
    {
        bool isSoftware = (mOpenGLDropDown->getSelected() == 0);
        mAlphaCacheCheckBox->setEnabled(isSoftware);
    }
}

void Setup_Video::externalUpdated()
{
    mShowJob = serverConfig.getBoolValue("showJob");
    mShowJobCheckBox->setSelected(mShowJob);
}