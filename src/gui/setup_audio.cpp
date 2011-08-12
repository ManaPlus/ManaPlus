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

#include "gui/setup_audio.h"

#include "configuration.h"
#include "logger.h"
#include "sound.h"

#include "gui/okdialog.h"
#include "gui/viewport.h"

#include "gui/widgets/checkbox.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/slider.h"

#include "utils/gettext.h"

#include "debug.h"

Setup_Audio::Setup_Audio():
    mMusicVolume(config.getIntValue("musicVolume")),
    mSfxVolume(config.getIntValue("sfxVolume")),
    mAudioEnabled(config.getBoolValue("sound")),
    mGameSoundEnabled(config.getBoolValue("playBattleSound")),
    mGuiSoundEnabled(config.getBoolValue("playGuiSound")),
    mMusicEnabled(config.getBoolValue("playMusic")),
    mMumbleEnabled(config.getBoolValue("enableMumble")),
    mDownloadEnabled(config.getBoolValue("download-music")),
    mAudioCheckBox(new CheckBox(_("Enable Audio"), mAudioEnabled)),
    mGameSoundCheckBox(new CheckBox(_("Enable game sfx"), mGameSoundEnabled)),
    mGuiSoundCheckBox(new CheckBox(_("Enable gui sfx"), mGuiSoundEnabled)),
    mMusicCheckBox(new CheckBox(_("Enable music"), mMusicEnabled)),
    mMumbleCheckBox(new CheckBox(_("Enable mumble voice chat"),
                    mMumbleEnabled)),
    mDownloadMusicCheckBox(new CheckBox(_("Download music"),
                           mDownloadEnabled)),
    mSfxSlider(new Slider(0, sound.getMaxVolume())),
    mMusicSlider(new Slider(0, sound.getMaxVolume()))
{
    setName(_("Audio"));
    setDimension(gcn::Rectangle(0, 0, 250, 200));

    gcn::Label *sfxLabel = new Label(_("Sfx volume"));
    gcn::Label *musicLabel = new Label(_("Music volume"));

    mSfxSlider->setActionEventId("sfx");
    mMusicSlider->setActionEventId("music");

    mSfxSlider->addActionListener(this);
    mMusicSlider->addActionListener(this);

    mAudioCheckBox->setPosition(10, 10);

    mSfxSlider->setValue(mSfxVolume);
    mMusicSlider->setValue(mMusicVolume);

    mSfxSlider->setWidth(90);
    mMusicSlider->setWidth(90);

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mAudioCheckBox);
    place(0, 1, mMusicCheckBox);
    place(0, 2, mGameSoundCheckBox);
    place(0, 3, mGuiSoundCheckBox);
    place(0, 4, mSfxSlider);
    place(1, 4, sfxLabel);
    place(0, 5, mMusicSlider);
    place(1, 5, musicLabel);
    place(0, 6, mMumbleCheckBox);
    place(0, 7, mDownloadMusicCheckBox);

    setDimension(gcn::Rectangle(0, 0, 365, 280));
}

void Setup_Audio::apply()
{
    mAudioEnabled = mAudioCheckBox->isSelected();
    mGameSoundEnabled = mGameSoundCheckBox->isSelected();
    mGuiSoundEnabled = mGuiSoundCheckBox->isSelected();
    mMusicEnabled = mMusicCheckBox->isSelected();
    mMumbleEnabled = mMumbleCheckBox->isSelected();
    mDownloadEnabled = mDownloadMusicCheckBox->isSelected();
    mSfxVolume = config.getIntValue("sfxVolume");
    mMusicVolume = config.getIntValue("musicVolume");

    config.setValue("sound", mAudioEnabled);
    config.setValue("playBattleSound", mGameSoundEnabled);
    config.setValue("playGuiSound", mGuiSoundEnabled);

    config.setValue("enableMumble", mMumbleEnabled);

    // Display a message if user has selected to download music,
    // And if downloadmusic is not already enabled
    if (mDownloadEnabled && !config.getBoolValue("download-music"))
    {
        new OkDialog(_("Notice"), _("You may have to restart your client "
                     "if you want to download new music"));
    }
    config.setValue("download-music", mDownloadEnabled);

    if (mAudioEnabled)
    {
        try
        {
            sound.init();
        }
        catch (const char *err)
        {
            new OkDialog(_("Sound Engine"), err);
            logger->log("Warning: %s", err);
        }
        if (mMusicEnabled)
        {
            if (viewport && !config.getBoolValue("playMusic"))
            {
                Map *map = viewport->getMap();
                if (map)
                {
                    config.setValue("playMusic", mMusicEnabled);
                    sound.playMusic(map->getMusicFile());
                }
            }
        }
        else if (config.getBoolValue("playMusic"))
        {
            sound.stopMusic();
        }

    }
    else
    {
        sound.close();
    }

    config.setValue("playMusic", mMusicEnabled);
}

void Setup_Audio::cancel()
{
    mAudioCheckBox->setSelected(mAudioEnabled);
    mGameSoundCheckBox->setSelected(mGameSoundEnabled);
    mGuiSoundCheckBox->setSelected(mGuiSoundEnabled);
    mMusicCheckBox->setSelected(mMusicEnabled);
    mMumbleCheckBox->setSelected(mMumbleEnabled);
    mDownloadMusicCheckBox->setSelected(mDownloadEnabled);

    sound.setSfxVolume(mSfxVolume);
    mSfxSlider->setValue(mSfxVolume);

    sound.setMusicVolume(mMusicVolume);
    mMusicSlider->setValue(mMusicVolume);

    config.setValue("sound", mAudioEnabled);
    config.setValue("playBattleSound", mGameSoundEnabled);
    config.setValue("playGuiSound", mGuiSoundEnabled);
    config.setValue("enableMumble", mMumbleEnabled);
    config.setValue("download-music", mDownloadEnabled);
    config.setValue("sfxVolume", mSfxVolume);
    config.setValue("musicVolume", mMusicVolume);
    config.setValue("playMusic", mMusicEnabled);
}

void Setup_Audio::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "sfx")
    {
        config.setValueInt("sfxVolume",
            static_cast<int>(mSfxSlider->getValue()));
        sound.setSfxVolume(static_cast<int>(mSfxSlider->getValue()));
    }
    else if (event.getId() == "music")
    {
        config.setValueInt("musicVolume",
            static_cast<int>(mMusicSlider->getValue()));
        sound.setMusicVolume(static_cast<int>(mMusicSlider->getValue()));
    }
}
