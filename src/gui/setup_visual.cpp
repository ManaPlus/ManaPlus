/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  Andrei Karas
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

#include "gui/setup_visual.h"

#include "gui/widgets/layouthelper.h"
#include "gui/widgets/namesmodel.h"
#include "gui/widgets/scrollarea.h"

#include "client.h"

#include "utils/gettext.h"

#include "debug.h"

static const int speachListSize = 4;

static const char *const speachList[] =
{
    N_("No text"),
    N_("Text"),
    N_("Bubbles, no names"),
    N_("Bubbles with names")
};

static const int ambientFxListSize = 3;

static const char *const ambientFxList[] =
{
    N_("off"),
    N_("low"),
    N_("high")
};

static const int particleTypeListSize = 3;

static const char *const particleTypeList[] =
{
    N_("best quality"),
    N_("normal"),
    N_("best perfomance")
};

static const int vSyncListSize = 3;

static const char *const vSyncList[] =
{
    N_("default"),
    N_("off"),
    N_("on")
};

Setup_Visual::Setup_Visual(const Widget2 *const widget) :
    SetupTabScroll(widget),
    mSpeachList(new NamesModel),
    mAmbientFxList(new NamesModel),
    mParticleList(new SetupItemNames),
    mParticleTypeList(new NamesModel),
    mVSyncList(new NamesModel)
{
    setName(_("Visual"));
    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);
    place(0, 0, mScroll, 10, 10);

    mPreferredFirstItemSize = 150;

    new SetupItemLabel(_("Notifications"), "", this);

    new SetupItemCheckBox(_("Show pickup notifications in chat"), "",
        "showpickupchat", this, "showpickupchatEvent");

    new SetupItemCheckBox(_("Show pickup notifications as particle effects"),
        "", "showpickupparticle", this, "showpickupparticleEvent");

    new SetupItemLabel(_("Effects"), "", this);

#ifndef ANDROID
    new SetupItemCheckBox(_("Grab mouse and keyboard input"),
        "", "grabinput", this, "grabinputEvent");
#endif

    new SetupItemCheckBox(_("Blurring textures (OpenGL)"),
        "", "blur", this, "blurEvent");

    new SetupItemSlider(_("Gui opacity"), "", "guialpha",
        this, "guialphaEvent", 0.1, 1.0, 150, true);

    mSpeachList->fillFromArray(&speachList[0], speachListSize);
    new SetupItemDropDown(_("Overhead text"), "", "speech", this,
        "speechEvent", mSpeachList, 200);

    mAmbientFxList->fillFromArray(&ambientFxList[0], ambientFxListSize);
    new SetupItemDropDown(_("Ambient FX"), "", "OverlayDetail", this,
        "OverlayDetailEvent", mAmbientFxList, 100);

    new SetupItemCheckBox(_("Particle effects"), "",
        "particleeffects", this, "particleeffectsEvent");

    mParticleList->push_back(_("low"));
    mParticleList->push_back(_("medium"));
    mParticleList->push_back(_("high"));
    mParticleList->push_back(_("max"));
    (new SetupItemSlider2(_("Particle detail"), "", "particleEmitterSkip",
        this, "particleEmitterSkipEvent", 0, 3,
        mParticleList, true))->setInvertValue(3);

    mParticleTypeList->fillFromArray(&particleTypeList[0],
        particleTypeListSize);
    new SetupItemDropDown(_("Particle physics"), "", "particleFastPhysics",
        this, "particleFastPhysicsEvent", mParticleTypeList, 200);


    new SetupItemLabel(_("Gamma"), "", this);

    new SetupItemCheckBox(_("Enable gamma control"),
        "", "enableGamma", this, "enableGammaEvent");

    new SetupItemSlider(_("Gamma"), "", "gamma",
        this, "gammeEvent", 1, 20, 350, true);


    new SetupItemLabel(_("Other"), "", this);

    mVSyncList->fillFromArray(&vSyncList[0], vSyncListSize);
    new SetupItemDropDown(_("Vsync"), "", "vsync", this,
        "vsyncEvent", mVSyncList, 100);

#if defined(WIN32) || defined(__APPLE__)
    new SetupItemCheckBox(_("Center game window"),
        "", "centerwindow", this, "centerwindowEvent");
#endif

    new SetupItemCheckBox(_("Allow screensaver to run"),
        "", "allowscreensaver", this, "allowscreensaverEvent");

    setDimension(gcn::Rectangle(0, 0, 550, 350));
}

Setup_Visual::~Setup_Visual()
{
    delete mSpeachList;
    mSpeachList = nullptr;
    delete mAmbientFxList;
    mAmbientFxList = nullptr;
    delete mParticleList;
    mParticleList = nullptr;
    delete mParticleTypeList;
    mParticleTypeList = nullptr;
    delete mVSyncList;
    mVSyncList = nullptr;
}

void Setup_Visual::apply()
{
    SetupTabScroll::apply();
    Client::applyGrabMode();
}
