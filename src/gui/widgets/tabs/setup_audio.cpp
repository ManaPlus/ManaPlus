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

#include "gui/widgets/tabs/setup_audio.h"

#include "resources/map/map.h"

#include "configuration.h"
#include "soundmanager.h"

#include "gui/viewport.h"

#include "gui/models/soundsmodel.h"

#include "gui/widgets/containerplacer.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"

#include "utils/delete2.h"

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
    new SetupItemLabel(_("Basic settings"), "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable Audio"), "", "sound", this, "soundEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable music"), "",
        "playMusic", this, "playMusicEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable game sfx"), "",
        "playBattleSound", this, "playBattleSoundEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable gui sfx"), "",
        "playGuiSound", this, "playGuiSoundEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemSlider(_("Sfx volume"), "", "sfxVolume",
        this, "sfxVolumeEvent", 0, SoundManager::getMaxVolume(), 1,
        150,
        OnTheFly_true,
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemSlider(_("Music volume"), "", "musicVolume",
        this, "musicVolumeEvent", 0, SoundManager::getMaxVolume(), 1,
        150,
        OnTheFly_true,
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable music fade out"), "",
        "fadeoutmusic", this, "fadeoutmusicEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemIntTextField(_("Audio frequency"), "",
        "audioFrequency", this, "audioFrequencyEvent", 14000, 192000,
        MainConfig_true);

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
        "audioChannels", 1, 4, 1, mChannelsList,
        OnTheFly_false,
        MainConfig_true,
        DoNotAlign_false);

    // TRANSLATORS: settings option
    new SetupItemIntTextField(_("Parallel number of sounds"), "",
        "parallelAudioChannels", this, "parallelAudioChannelsEvent", 1, 1000,
        MainConfig_true);


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Sound effects"), "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Information dialog sound"), "",
        "soundinfo", this, "soundinfoEvent", mSoundModel,
        150, OnTheFly_false, MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Request dialog sound"), "",
        "soundrequest", this, "soundrequestEvent", mSoundModel,
        150, OnTheFly_false, MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Whisper message sound"), "",
        "soundwhisper", this, "soundwhisperEvent", mSoundModel,
        150, OnTheFly_false, MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Guild / Party message sound"), "",
        "soundguild", this, "soundguildEvent", mSoundModel,
        150, OnTheFly_false, MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Highlight message sound"), "",
        "soundhighlight", this, "soundhighlightEvent", mSoundModel,
        150, OnTheFly_false, MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Global message sound"), "",
        "soundglobal", this, "soundglobalEvent", mSoundModel,
        150, OnTheFly_false, MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Error message sound"), "",
        "sounderror", this, "sounderrorEvent", mSoundModel,
        150, OnTheFly_false, MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Trade request sound"), "",
        "soundtrade", this, "soundtradeEvent", mSoundModel,
        150, OnTheFly_false, MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Show window sound"), "",
        "soundshowwindow", this, "soundshowwindowEvent", mSoundModel,
        150, OnTheFly_false, MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemSound(_("Hide window sound"), "",
        "soundhidewindow", this, "soundhidewindowEvent", mSoundModel,
        150, OnTheFly_false, MainConfig_true);

    // TRANSLATORS: settings group
    new SetupItemLabel(_("Other"), "", this,
        Separator_true);

#ifdef USE_MUMBLE
    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable mumble voice chat"), "",
        "enableMumble", this, "enableMumbleEvent",
        MainConfig_true);
#endif  // USE_MUMBLE

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Download music"), "",
        "download-music", this, "download-musicEvent",
        MainConfig_true);

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
        if (config.getBoolValue("playMusic"))
        {
            if (viewport != nullptr)
            {  // in game
                const Map *const map = viewport->getMap();
                if (map != nullptr)
                {
                    soundManager.playMusic(map->getMusicFile(),
                        SkipError_false);
                }
            }
            else
            {  // not in game
                soundManager.playMusic(branding.getValue(
                    "loginMusic",
                    "keprohm.ogg"),
                    SkipError_true);
            }
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
