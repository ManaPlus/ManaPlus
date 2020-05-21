/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "gui/widgets/tabs/setup_perfomance.h"

#include "gui/models/namesmodel.h"

#include "gui/widgets/containerplacer.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/setupitem.h"

#include "utils/delete2.h"
#include "utils/gettext.h"
#ifdef USE_SDL2
#include "configuration.h"
#include "utils/sdlhelper.h"
#endif  // USE_SDL2

#include "debug.h"

static const int texturesListSize = 5;

static const char *const texturesList[] =
{
    // TRANSLATORS: texture compression type
    N_("No"),
    "s3tc",
    "fxt1",
    "ARB",
    "bptc"
};

Setup_Perfomance::Setup_Perfomance(const Widget2 *const widget) :
    SetupTabScroll(widget),
#ifdef USE_SDL2
    mSdlDriversList(new NamesModel),
#endif  // USE_SDL2
    mTexturesList(new NamesModel)
{
    // TRANSLATORS: settings tab name
    setName(_("Performance"));

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);
    place(0, 0, mScroll, 10, 10);

#ifdef USE_SDL2
    StringVect sdlDriversList;
    SDL::getRenderers(sdlDriversList,
        config.getStringValue("sdlDriver"));
    sdlDriversList.insert(sdlDriversList.begin(),
        // TRANSLATORS: sdl driver name
        N_("default"));
#endif  // USE_SDL2

    // TRANSLATORS: settings option
    new SetupItemLabel(_("Better performance (enable for better performance)"),
        "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Auto adjust performance"), "",
        "adjustPerfomance", this, "adjustPerfomanceEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Hw acceleration"), "",
        "hwaccel", this, "hwaccelEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable opacity cache (Software, can "
        "use much memory)"), "", "alphaCache", this, "alphaCacheEvent",
        MainConfig_true);

#ifndef USE_SDL2
    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable map reduce (Software)"), "",
        "enableMapReduce", this, "enableMapReduceEvent",
        MainConfig_true);
#endif  // USE_SDL2

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable compound sprite delay (Software)"), "",
        "enableCompoundSpriteDelay", this, "enableCompoundSpriteDelayEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable delayed images load (OpenGL)"), "",
        "enableDelayedAnimations", this, "enableDelayedAnimationsEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable texture sampler (OpenGL)"), "",
        "useTextureSampler", this, "useTextureSamplerEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable OpenGL context creation"),
        "", "openglContext", this, "openglContextEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable OpenGL direct state access"),
        "", "enableDSA", this, "enableDSAEvent",
        MainConfig_true);


    // TRANSLATORS: settings option
    new SetupItemLabel(_("Better quality (disable for better performance)"),
        "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable alpha channel fix (Software, can "
        "be very slow)"), "Can slow down drawing", "enableAlphaFix",
        this, "enableAlphaFixEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show beings transparency"), "",
        "beingopacity", this, "beingopacityEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable reorder sprites (need for mods support)."),
        "", "enableReorderSprites", this, "enableReorderSpritesEvent",
        MainConfig_true);


#ifndef USE_SDL2
    // TRANSLATORS: settings option
    new SetupItemLabel(_("Small memory (enable for lower memory usage)"),
         "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Disable advanced beings caching (Software)"), "",
        "disableAdvBeingCaching", this, "disableAdvBeingCachingEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Disable beings caching (Software)"), "",
        "disableBeingCaching", this, "disableBeingCachingEvent",
        MainConfig_true);
#endif  // USE_SDL2


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Different options (enable or disable can "
        "improve performance)"), "", this,
        Separator_true);

#ifdef USE_SDL2
    mSdlDriversList->fillFromVector(sdlDriversList);
    new SetupItemDropDownStr(
        // TRANSLATORS: settings option
        _("Try first sdl driver (only for SDL2 default mode)"),
        "", "sdlDriver", this, "sdlDriverEvent", mSdlDriversList, 100,
        MainConfig_true);
#endif  // USE_SDL2

    mTexturesList->fillFromArray(&texturesList[0], texturesListSize);
    // TRANSLATORS: settings option
    new SetupItemDropDown(_("Enable texture compression (OpenGL)"), "",
        "compresstextures", this, "compresstexturesEvent", mTexturesList, 100,
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable rectangular texture extension (OpenGL)"),
        "", "rectangulartextures", this, "rectangulartexturesEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Use new texture internal format (OpenGL)"),
        "", "newtextures", this, "newtexturesEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable texture atlases (OpenGL)"), "",
        "useAtlases", this, "useAtlasesEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Cache all sprites per map (can use "
        "additional memory)"), "", "uselonglivesprites", this,
        "uselonglivespritesEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Cache all sounds (can use additional memory)"),
        "", "uselonglivesounds", this,
        "uselonglivesoundsEvent",
        MainConfig_true);

    setDimension(Rect(0, 0, 550, 350));
}

Setup_Perfomance::~Setup_Perfomance()
{
    delete2(mTexturesList)
#ifdef USE_SDL2
    delete2(mSdlDriversList)
#endif  // USE_SDL2
}
