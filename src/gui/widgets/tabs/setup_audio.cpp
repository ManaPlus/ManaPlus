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

#include "gui/widgets/tabs/setup_audio.h"

#include "main.h"
#include "map.h"

#include "configuration.h"
#include "soundmanager.h"

#include "gui/viewport.h"

#include "gui/models/soundsmodel.h"

#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "debug.h"

Setup_Audio::Setup_Audio(const Widget2 *const widget) :
    SetupTabScroll(widget),
    mSoundModel(new SoundsModel),
    mChannelsList(new SetupItemNames)
{
    // TRANSLATORS: audio tab in settings
    setName(_("Audio"));

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);
    place(0, 0, mScroll, 10, 10);

    // TRANSLATORS: settings option
    new SetupItemLabel(_("Basic settings"), "", this);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable Audio"), "", "sound", this, "soundEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable music"), "",
        "playMusic", this, "playMusicEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable game sfx"), "",
        "playBattleSound", this, "playBattleSoundEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable gui sfx"), "",
        "playGuiSound", this, "playGuiSoundEvent");

    // TRANSLATORS: settings option
    new SetupItemSlider(_("Sfx volume"), "", "sfxVolume",
        this, "sfxVolumeEvent", 0, soundManager.getMaxVolume(), 1,
        150, true, true);

    // TRANSLATORS: settings option
    new SetupItemSlider(_("Music volume"), "", "musicVolume",
        this, "musicVolumeEvent", 0, soundManager.getMaxVolume(), 1,
        150, true, true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable music fade out"), "",
        "fadeoutmusic", this, "fadeoutmusicEvent");

    // TRANSLATORS: settings option
    new SetupItemIntTextField(_("Audio frequency"), "",
        "audioFrequency", this, "audioFrequencyEvent", 14000, 192000);

    // TRANSLATORS: audio type
    mChannelsList->push_back(_("mono"));
    // TRANSLATORS: audio type
    mChannelsList->push_back(_("stereo"));
    // TRANSLATORS: audio type
    mChannelsList->push_back(_("surround"));
    // TRANSLATORS: audio type
    mChannelsList->push_back(_("surround+center+lfe"));
    // TRANSLATORS: settings option
    new SetupItemSlider2(_("Audio channels"), "", "audioChannels", this,
        "audioChannels", 1, 4, 1, mChannelsList, false, true, false);


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Sound effects"), "", this);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Information dialog sound"), "",
        "soundinfo", this, "soundinfoEvent", mSoundModel);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Request dialog sound"), "",
        "soundrequest", this, "soundrequestEvent", mSoundModel);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Whisper message sound"), "",
        "soundwhisper", this, "soundwhisperEvent", mSoundModel);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Guild / Party message sound"), "",
        "soundguild", this, "soundguildEvent", mSoundModel);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Highlight message sound"), "",
        "soundhighlight", this, "soundhighlightEvent", mSoundModel);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Global message sound"), "",
        "soundglobal", this, "soundglobalEvent", mSoundModel);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Error message sound"), "",
        "sounderror", this, "sounderrorEvent", mSoundModel);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Trade request sound"), "",
        "soundtrade", this, "soundtradeEvent", mSoundModel);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Show window sound"), "",
        "soundshowwindow", this, "soundshowwindowEvent", mSoundModel);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Hide window sound"), "",
        "soundhidewindow", this, "soundhidewindowEvent", mSoundModel);

    // TRANSLATORS: settings group
    new SetupItemLabel(_("Other"), "", this);

#ifdef USE_MUMBLE
    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable mumble voice chat"), "",
        "enableMumble", this, "enableMumbleEvent");
#endif

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Download music"), "",
        "download-music", this, "download-musicEvent");

    setDimension(Rect(0, 0, 550, 350));
}

Setup_Audio::~Setup_Audio()
{
    delete2(mSoundModel);
    delete2(mChannelsList);
}

void Setup_Audio::apply()
{
    SetupTabScroll::apply();
    if (config.getBoolValue("sound"))
    {
        soundManager.init();
        if (viewport && config.getBoolValue("playMusic"))
        {
            const Map *const map = viewport->getMap();
            if (map)
                soundManager.playMusic(map->getMusicFile());
        }
        else
        {
            soundManager.stopMusic();
        }
    }
    else
    {
        soundManager.close();
    }
}
