/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "sound.h"

#include "configuration.h"
#include "localplayer.h"
#include "logger.h"

#include "resources/music.h"
#include "resources/resourcemanager.h"
#include "resources/soundeffect.h"

#include <SDL.h>

#include "debug.h"

/**
 * This will be set to true, when a music can be freed after a fade out
 * Currently used by fadeOutCallBack()
 */
static bool sFadingOutEnded = false;

/**
 * Callback used at end of fadeout.
 * It is called by Mix_MusicFadeFinished().
 */
static void fadeOutCallBack()
{
    sFadingOutEnded = true;
}

Sound::Sound():
    mInstalled(false),
    mSfxVolume(100),
    mMusicVolume(60),
    mMusic(nullptr),
    mPlayBattle(false),
    mPlayGui(false),
    mPlayMusic(false),
    mGuiChannel(-1)
{
    // This set up our callback function used to
    // handle fade outs endings.
    sFadingOutEnded = false;
    Mix_HookMusicFinished(fadeOutCallBack);
}

Sound::~Sound()
{
    config.removeListeners(this);

    // Unlink the callback function.
    Mix_HookMusicFinished(nullptr);
}

void Sound::optionChanged(const std::string &value)
{
    if (value == "playBattleSound")
        mPlayBattle = config.getBoolValue("playBattleSound");
    else if (value == "playGuiSound")
        mPlayGui = config.getBoolValue("playGuiSound");
    else if (value == "playMusic")
        mPlayMusic = config.getBoolValue("playMusic");
    else if (value == "sfxVolume")
        setSfxVolume(config.getIntValue("sfxVolume"));
    else if (value == "musicVolume")
        setMusicVolume(config.getIntValue("musicVolume"));
}

void Sound::init()
{
    // Don't initialize sound engine twice
    if (mInstalled)
        return;

    logger->log1("Sound::init() Initializing sound...");

    mPlayBattle = config.getBoolValue("playBattleSound");
    mPlayGui = config.getBoolValue("playGuiSound");
    mPlayMusic = config.getBoolValue("playMusic");
    config.addListener("playBattleSound", this);
    config.addListener("playGuiSound", this);
    config.addListener("playMusic", this);
    config.addListener("sfxVolume", this);
    config.addListener("musicVolume", this);

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) == -1)
    {
        logger->log1("Sound::init() Failed to initialize audio subsystem");
        return;
    }

    const size_t audioBuffer = 4096;
    int channels = config.getIntValue("audioChannels");
    switch (channels)
    {
        case 3:
            channels = 4;
            break;
        case 4:
            channels = 6;
            break;
        default:
            break;
    }

    const int res = Mix_OpenAudio(config.getIntValue("audioFrequency"),
        MIX_DEFAULT_FORMAT, channels, audioBuffer);

    if (res < 0)
    {
        logger->log("Sound::init Could not initialize audio: %s",
                    Mix_GetError());
        if (Mix_OpenAudio(22010, MIX_DEFAULT_FORMAT, 2, audioBuffer) < 0)
            return;
        logger->log("Fallback to stereo audio");
    }

    Mix_AllocateChannels(16);
    Mix_VolumeMusic(mMusicVolume);
    Mix_Volume(-1, mSfxVolume);

    info();

    mInstalled = true;

    if (!mCurrentMusicFile.empty() && mPlayMusic)
        playMusic(mCurrentMusicFile);
}

void Sound::info()
{
    SDL_version compiledVersion;
    const SDL_version *linkedVersion;
    char driver[40] = "Unknown";
    const char *format = "Unknown";
    int rate = 0;
    uint16_t audioFormat = 0;
    int channels = 0;

    MIX_VERSION(&compiledVersion);
    linkedVersion = Mix_Linked_Version();

    SDL_AudioDriverName(driver, 40);

    Mix_QuerySpec(&rate, &audioFormat, &channels);
    switch (audioFormat)
    {
        case AUDIO_U8: format = "U8"; break;
        case AUDIO_S8: format = "S8"; break;
        case AUDIO_U16LSB: format = "U16LSB"; break;
        case AUDIO_S16LSB: format = "S16LSB"; break;
        case AUDIO_U16MSB: format = "U16MSB"; break;
        case AUDIO_S16MSB: format = "S16MSB"; break;
        default: break;
    }

    logger->log("Sound::info() SDL_mixer: %i.%i.%i (compiled)",
            compiledVersion.major,
            compiledVersion.minor,
            compiledVersion.patch);
    if (linkedVersion)
    {
        logger->log("Sound::info() SDL_mixer: %i.%i.%i (linked)",
            linkedVersion->major,
            linkedVersion->minor,
            linkedVersion->patch);
    }
    else
    {
        logger->log1("Sound::info() SDL_mixer: unknown");
    }
    logger->log("Sound::info() Driver: %s", driver);
    logger->log("Sound::info() Format: %s", format);
    logger->log("Sound::info() Rate: %i", rate);
    logger->log("Sound::info() Channels: %i", channels);
}

void Sound::setMusicVolume(int volume)
{
    mMusicVolume = volume;

    if (mInstalled)
        Mix_VolumeMusic(mMusicVolume);
}

void Sound::setSfxVolume(int volume)
{
    mSfxVolume = volume;

    if (mInstalled)
        Mix_Volume(-1, mSfxVolume);
}

static Music *loadMusic(const std::string &fileName)
{
    ResourceManager *resman = ResourceManager::getInstance();
    return resman->getMusic(paths.getStringValue("music") + fileName);
}

void Sound::playMusic(const std::string &fileName)
{
    mCurrentMusicFile = fileName;

    if (!mInstalled || !mPlayMusic)
        return;

    haltMusic();

    if (!fileName.empty())
    {
        mMusic = loadMusic(fileName);
        if (mMusic)
            mMusic->play();
    }
}

void Sound::stopMusic()
{
    haltMusic();
}

void Sound::fadeInMusic(const std::string &fileName, int ms)
{
    mCurrentMusicFile = fileName;

    if (!mInstalled || !mPlayMusic)
        return;

    haltMusic();

    if (!fileName.empty())
    {
        mMusic = loadMusic(fileName);
        if (mMusic)
            mMusic->play(-1, ms);
    }
}

void Sound::fadeOutMusic(int ms)
{
    mCurrentMusicFile.clear();

    if (!mInstalled)
        return;

    logger->log("Sound::fadeOutMusic() Fading-out (%i ms)", ms);

    if (mMusic)
    {
        Mix_FadeOutMusic(ms);
        // Note: The fadeOutCallBack handler will take care about freeing
        // the music file at fade out ending.
    }
    else
    {
        sFadingOutEnded = true;
    }
}

void Sound::fadeOutAndPlayMusic(const std::string &fileName, int ms)
{
    mNextMusicFile = fileName;
    fadeOutMusic(ms);
}

void Sound::logic()
{
    if (sFadingOutEnded)
    {
        if (mMusic)
        {
            mMusic->decRef();
            mMusic = nullptr;
        }
        sFadingOutEnded = false;

        if (!mNextMusicFile.empty())
        {
            playMusic(mNextMusicFile);
            mNextMusicFile.clear();
        }
    }
}

void Sound::playSfx(const std::string &path, int x, int y)
{
    if (!mInstalled || path.empty() || !mPlayBattle)
        return;

    std::string tmpPath;
    if (!path.compare(0, 4, "sfx/"))
        tmpPath = path;
    else
        tmpPath = paths.getValue("sfx", "sfx/") + path;
    ResourceManager *resman = ResourceManager::getInstance();
    SoundEffect *sample = resman->getSoundEffect(tmpPath);
    if (sample)
    {
        logger->log("Sound::playSfx() Playing: %s", path.c_str());
        int vol = 120;
        if (player_node && (x > 0 || y > 0))
        {
            int dx = player_node->getTileX() - x;
            int dy = player_node->getTileY() - y;
            if (dx < 0)
                dx = -dx;
            if (dy < 0)
                dy = -dy;
            int dist = dx > dy ? dx : dy;
            if (dist * 8 > vol)
                return;

            vol -= dist * 8;
        }
        sample->play(0, vol);
    }
}

void Sound::playGuiSound(const std::string &name)
{
    playGuiSfx(branding.getStringValue("systemsounds")
        + config.getStringValue(name) + ".ogg");
}

void Sound::playGuiSfx(const std::string &path)
{
    if (!mInstalled || path.empty() || !mPlayGui)
        return;

    std::string tmpPath;
    if (!path.compare(0, 4, "sfx/"))
        tmpPath = path;
    else
        tmpPath = paths.getValue("sfx", "sfx/") + path;
    ResourceManager *resman = ResourceManager::getInstance();
    SoundEffect *sample = resman->getSoundEffect(tmpPath);
    if (sample)
    {
        logger->log("Sound::playGuiSfx() Playing: %s", path.c_str());
        int ret = sample->play(0, 120, mGuiChannel);
        if (ret != -1)
            mGuiChannel = ret;
    }
}

void Sound::close()
{
    if (!mInstalled)
        return;

    haltMusic();
    logger->log1("Sound::close() Shutting down sound...");
    Mix_CloseAudio();

    mInstalled = false;
}

void Sound::haltMusic()
{
    if (!mMusic)
        return;

    Mix_HaltMusic();
    if (mMusic)
    {
        mMusic->decRef();
        mMusic = nullptr;
    }
}

void Sound::changeAudio()
{
    if (mInstalled)
        close();
    else
        init();
}

void Sound::volumeOff()
{
    if (mInstalled)
    {
        Mix_VolumeMusic(0);
        Mix_Volume(-1, 0);
    }
}

void Sound::volumeRestore()
{
    if (mInstalled)
    {
        Mix_VolumeMusic(mMusicVolume);
        Mix_Volume(-1, mSfxVolume);
    }
}
