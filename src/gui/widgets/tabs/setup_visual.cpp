/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "gui/widgets/tabs/setup_visual.h"

#include "gui/windowmanager.h"

#include "gui/models/namesmodel.h"

#include "gui/widgets/containerplacer.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"

#include "utils/delete2.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

static const int speachListSize = 4;

static const char *const speachList[] =
{
    // TRANSLATORS: speach type
    N_("No text"),
    // TRANSLATORS: speach type
    N_("Text"),
    // TRANSLATORS: speach type
    N_("Bubbles, no names"),
    // TRANSLATORS: speach type
    N_("Bubbles with names")
};

static const int ambientFxListSize = 3;

static const char *const ambientFxList[] =
{
    // TRANSLATORS: ambient effect type
    N_("off"),
    // TRANSLATORS: ambient effect type
    N_("low"),
    // TRANSLATORS: ambient effect type
    N_("high")
};

static const int particleTypeListSize = 3;

static const char *const particleTypeList[] =
{
    // TRANSLATORS: patricle effects type
    N_("best quality"),
    // TRANSLATORS: patricle effects type
    N_("normal"),
    // TRANSLATORS: patricle effects type
    N_("best performance")
};

static const int vSyncListSize = 3;

static const char *const vSyncList[] =
{
    // TRANSLATORS: vsync type
    N_("default"),
    // TRANSLATORS: vsync type
    N_("off"),
    // TRANSLATORS: vsync type
    N_("on")
};

Setup_Visual::Setup_Visual(const Widget2 *const widget) :
    SetupTabScroll(widget),
    mSpeachList(new NamesModel),
    mAmbientFxList(new NamesModel),
    mParticleList(new SetupItemNames),
    mParticleTypeList(new NamesModel),
    mVSyncList(new NamesModel),
    mScaleList(new NamesModel)
{
    // TRANSLATORS: settings tab name
    setName(_("Visual"));
    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);
    place(0, 0, mScroll, 10, 10);

    mPreferredFirstItemSize = 150;

    // TRANSLATORS: settings option
    new SetupItemLabel(_("Scale"), "", this,
        Separator_true);

    for (int f = 1; f <= 7; f ++)
    {
        // TRANSLATORS: particle details
        mScaleList->add(toString(f) + "x");
    }
    // TRANSLATORS: settings option
    new SetupItemSliderInt(_("Scale"), "",
        "scale", this, "scaleEvent", mScaleList, 1,
        150, OnTheFly_false, MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemLabel(_("Notifications"), "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show pickup notifications in chat"), "",
        "showpickupchat", this, "showpickupchatEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show pickup notifications as particle effects"),
        "", "showpickupparticle", this, "showpickupparticleEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemLabel(_("Effects"), "", this,
        Separator_true);

#ifndef ANDROID
    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Grab mouse and keyboard input"),
        "", "grabinput", this, "grabinputEvent",
        MainConfig_true);
#endif  // ANDROID

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Blurring textures (OpenGL)"),
        "", "blur", this, "blurEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemSlider(_("Gui opacity"), "", "guialpha",
        this, "guialphaEvent", 0.1, 1.01, 0.1, 150,
        OnTheFly_true,
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable gui opacity"),
        "", "enableGuiOpacity", this, "enableGuiOpacityEvent",
        MainConfig_true);

    mSpeachList->fillFromArray(&speachList[0], speachListSize);
    // TRANSLATORS: settings option
    new SetupItemDropDown(_("Overhead text"), "", "speech", this,
        "speechEvent", mSpeachList, 200,
        MainConfig_true);

    mAmbientFxList->fillFromArray(&ambientFxList[0], ambientFxListSize);
    // TRANSLATORS: settings option
    new SetupItemDropDown(_("Ambient FX"), "", "OverlayDetail", this,
        "OverlayDetailEvent", mAmbientFxList, 100,
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show particle effects"), "",
        "particleeffects", this, "particleeffectsEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show particle effects from maps"), "",
        "mapparticleeffects", this, "mapparticleeffectsEvent",
        MainConfig_true);

    // TRANSLATORS: particle details
    mParticleList->push_back(_("low"));
    // TRANSLATORS: particle details
    mParticleList->push_back(_("medium"));
    // TRANSLATORS: particle details
    mParticleList->push_back(_("high"));
    // TRANSLATORS: particle details
    mParticleList->push_back(_("max"));
    // TRANSLATORS: settings option
    (new SetupItemSlider2(_("Particle detail"), "", "particleEmitterSkip",
        this, "particleEmitterSkipEvent", 0, 3, 1, mParticleList,
        OnTheFly_true,
        MainConfig_true,
        DoNotAlign_false))->setInvertValue(3);

    mParticleTypeList->fillFromArray(&particleTypeList[0],
        particleTypeListSize);
    // TRANSLATORS: settings option
    new SetupItemDropDown(_("Particle physics"), "", "particleFastPhysics",
        this, "particleFastPhysicsEvent", mParticleTypeList, 200,
        MainConfig_true);


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Gamma"), "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable gamma control"),
        "", "enableGamma", this, "enableGammaEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemSlider(_("Gamma"), "", "gamma",
        this, "gammeEvent", 1, 20, 1, 350,
        OnTheFly_true,
        MainConfig_true);


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Other"), "", this,
        Separator_true);

    mVSyncList->fillFromArray(&vSyncList[0], vSyncListSize);
    // TRANSLATORS: settings option
    new SetupItemDropDown(_("Vsync"), "", "vsync", this,
        "vsyncEvent", mVSyncList, 100,
        MainConfig_true);

#if defined(WIN32) || defined(__APPLE__)
    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Center game window"),
        "", "centerwindow", this, "centerwindowEvent",
        MainConfig_true);
#endif  // defined(WIN32) || defined(__APPLE__)

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Allow screensaver to run"),
        "", "allowscreensaver", this, "allowscreensaverEvent",
        MainConfig_true);


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Screenshots"), "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Add water mark into screenshots"),
        "", "addwatermark", this, "addwatermarkEvent",
        MainConfig_true);

    setDimension(Rect(0, 0, 550, 350));
}

Setup_Visual::~Setup_Visual()
{
    delete2(mSpeachList);
    delete2(mAmbientFxList);
    delete2(mParticleList);
    delete2(mParticleTypeList);
    delete2(mVSyncList);
    delete2(mScaleList);
}

void Setup_Visual::apply()
{
    SetupTabScroll::apply();
    WindowManager::applyGrabMode();
#ifndef WIN32
    WindowManager::applyScale();
#endif  // WIN32
}
