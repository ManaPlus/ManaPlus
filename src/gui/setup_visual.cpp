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
#include "gui/widgets/scrollarea.h"

#include "client.h"

#include "utils/gettext.h"

#include "debug.h"

Setup_Visual::Setup_Visual()
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

    new SetupItemCheckBox(_("Grab mouse and keyboard input"),
        "", "grabinput", this, "grabinputEvent");

    new SetupItemCheckBox(_("Blurring textures (OpenGL)"),
        "", "blur", this, "blurEvent");

    new SetupItemSlider(_("Gui opacity"), "", "guialpha",
        this, "guialphaEvent", 0.1, 1.0, 150, true);

    mSpeachList = new SetupItemNames();
    mSpeachList->push_back(_("No text"));
    mSpeachList->push_back(_("Text"));
    mSpeachList->push_back(_("Bubbles, no names"));
    mSpeachList->push_back(_("Bubbles with names"));
    new SetupItemSlider2(_("Overhead text"), "", "speech", this,
        "speechEvent", 0, 3, mSpeachList);

    mAmbientFxList = new SetupItemNames();
    mAmbientFxList->push_back(_("off"));
    mAmbientFxList->push_back(_("low"));
    mAmbientFxList->push_back(_("high"));
    new SetupItemSlider2(_("Ambient FX"), "", "OverlayDetail", this,
        "OverlayDetailEvent", 0, 2, mAmbientFxList);

    new SetupItemCheckBox(_("Particle effects"), "",
        "particleeffects", this, "particleeffectsEvent");

    mParticleList = new SetupItemNames();
    mParticleList->push_back(_("low"));
    mParticleList->push_back(_("medium"));
    mParticleList->push_back(_("high"));
    mParticleList->push_back(_("max"));
    (new SetupItemSlider2(_("Particle detail"), "", "particleEmitterSkip",
        this, "particleEmitterSkipEvent", 0, 3,
        mParticleList, true))->setInvertValue(3);

    new SetupItemLabel(_("Other"), "", this);

    new SetupItemSlider(_("Gamma"), "", "gamma",
        this, "gammeEvent", 1, 20, 350, true);

    mVSyncList = new SetupItemNames();
    mVSyncList->push_back(_("default"));
    mVSyncList->push_back(_("off"));
    mVSyncList->push_back(_("on"));
    new SetupItemSlider2(_("Vsync"), "", "vsync", this,
        "vsyncEvent", 0, 2, mVSyncList);

#if defined(WIN32) || defined(__APPLE__)
    new SetupItemCheckBox(_("Center game window"),
        "", "centerwindow", this, "centerwindowEvent");
#endif

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
    delete mVSyncList;
    mVSyncList = nullptr;
}

void Setup_Visual::apply()
{
    SetupTabScroll::apply();
    Client::applyGrabMode();
}
