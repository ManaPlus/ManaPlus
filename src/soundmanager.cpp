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

#include "soundmanager.h"

#include "configuration.h"

#include "being/localplayer.h"

#include "resources/sdlmusic.h"
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

SoundManager::SoundManager():
    mNextMusicFile(),
    mInstalled(false),
    mSfxVolume(100),
    mMusicVolume(60),
    mCurrentMusicFile(),
    mMusic(nullptr),
    mGuiChannel(-1),
    mPlayBattle(false),
    mPlayGui(false),
    mPlayMusic(false),
    mFadeoutMusic(true),
    mCacheSounds(true)
{
    // This set up our callback function used to
    // handle fade outs endings.
    sFadingOutEnded = false;
    Mix_HookMusicFinished(&fadeOutCallBack);
}

SoundManager::~SoundManager()
{
}

void SoundManager::shutdown()
{
    config.removeListeners(this);

    // Unlink the callback function.
    Mix_HookMusicFinished(nullptr);

    CHECKLISTENERS
}

void SoundManager::optionChanged(const std::string &value)
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
    else if (value == "fadeoutmusic")
        mFadeoutMusic = config.getIntValue("fadeoutmusic");
    else if (value == "uselonglivesounds")
        mCacheSounds = config.getIntValue("uselonglivesounds");
}

void SoundManager::init()
{
    // Don't initialize sound engine twice
    if (mInstalled)
        return;

    logger->log1("SoundManager::init() Initializing sound...");

    mPlayBattle = config.getBoolValue("playBattleSound");
    mPlayGui = config.getBoolValue("playGuiSound");
    mPlayMusic = config.getBoolValue("playMusic");
    mFadeoutMusic = config.getBoolValue("fadeoutmusic");
    mMusicVolume = config.getIntValue("musicVolume");
    mSfxVolume = config.getIntValue("sfxVolume");
    mCacheSounds = config.getIntValue("uselonglivesounds");

    config.addListener("playBattleSound", this);
    config.addListener("playGuiSound", this);
    config.addListener("playMusic", this);
    config.addListener("sfxVolume", this);
    config.addListener("musicVolume", this);
    config.addListener("fadeoutmusic", this);
    config.addListener("uselonglivesounds", this);

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) == -1)
    {
        logger->log1("SoundManager::init() Failed to "
            "initialize audio subsystem");
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
        logger->log("SoundManager::init Could not initialize audio: %s",
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

void SoundManager::info()
{
    SDL_version compiledVersion;
    const char *format = "Unknown";
    int rate = 0;
    uint16_t audioFormat = 0;
    int channels = 0;

    MIX_VERSION(&compiledVersion);
    const SDL_version *const linkedVersion = Mix_Linked_Version();

#ifdef USE_SDL2
    const char *driver = SDL_GetCurrentAudioDriver();
#else
    char driver[40] = "Unknown";
    SDL_AudioDriverName(driver, 40);
#endif

    Mix_QuerySpec(&rate, &audioFormat, &channels);
    switch (audioFormat)
    {
        case AUDIO_U8:
            format = "U8";
            break;
        case AUDIO_S8:
            format = "S8"; break;
        case AUDIO_U16LSB:
            format = "U16LSB";
            break;
        case AUDIO_S16LSB:
            format = "S16LSB";
            break;
        case AUDIO_U16MSB:
            format = "U16MSB";
            break;
        case AUDIO_S16MSB:
            format = "S16MSB";
            break;
        default: break;
    }

    logger->log("SoundManager::info() SDL_mixer: %i.%i.%i (compiled)",
            compiledVersion.major,
            compiledVersion.minor,
            compiledVersion.patch);
    if (linkedVersion)
    {
        logger->log("SoundManager::info() SDL_mixer: %i.%i.%i (linked)",
            linkedVersion->major,
            linkedVersion->minor,
            linkedVersion->patch);
    }
    else
    {
        logger->log1("SoundManager::info() SDL_mixer: unknown");
    }
    logger->log("SoundManager::info() Driver: %s", driver);
    logger->log("SoundManager::info() Format: %s", format);
    logger->log("SoundManager::info() Rate: %i", rate);
    logger->log("SoundManager::info() Channels: %i", channels);
}

void SoundManager::setMusicVolume(const int volume)
{
    mMusicVolume = volume;

    if (mInstalled)
        Mix_VolumeMusic(mMusicVolume);
}

void SoundManager::setSfxVolume(const int volume)
{
    mSfxVolume = volume;

    if (mInstalled)
        Mix_Volume(-1, mSfxVolume);
}

static SDLMusic *loadMusic(const std::string &fileName)
{
    ResourceManager *const resman = ResourceManager::getInstance();
    return resman->getMusic(paths.getStringValue("music").append(fileName));
}

void SoundManager::playMusic(const std::string &fileName)
{
    if (!mInstalled || !mPlayMusic)
        return;

    if (mCurrentMusicFile == fileName)
        return;

    mCurrentMusicFile = fileName;

    haltMusic();

    if (!fileName.empty())
    {
        mMusic = loadMusic(fileName);
        if (mMusic)
            mMusic->play();
    }
}

void SoundManager::stopMusic()
{
    haltMusic();
}

/*
void SoundManager::fadeInMusic(const std::string &fileName, const int ms)
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
*/

void SoundManager::fadeOutMusic(const int ms)
{
    if (!mPlayMusic)
        return;

    mCurrentMusicFile.clear();

    if (!mInstalled)
        return;

    logger->log("SoundManager::fadeOutMusic() Fading-out (%i ms)", ms);

    if (mMusic && mFadeoutMusic)
    {
        Mix_FadeOutMusic(ms);
        // Note: The fadeOutCallBack handler will take care about freeing
        // the music file at fade out ending.
    }
    else
    {
        sFadingOutEnded = true;
        if (!mFadeoutMusic)
            haltMusic();
    }
}

void SoundManager::fadeOutAndPlayMusic(const std::string &fileName,
                                       const int ms)
{
    if (!mPlayMusic)
        return;

    mNextMusicFile = fileName;
    fadeOutMusic(ms);
}

void SoundManager::logic()
{
    BLOCK_START("SoundManager::logic")
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
    BLOCK_END("SoundManager::logic")
}

void SoundManager::playSfx(const std::string &path,
                           const int x, const int y) const
{
    if (!mInstalled || path.empty() || !mPlayBattle)
        return;

    std::string tmpPath;
    if (!path.compare(0, 4, "sfx/"))
        tmpPath = path;
    else
        tmpPath = paths.getValue("sfx", "sfx/").append(path);
    ResourceManager *const resman = ResourceManager::getInstance();
    SoundEffect *const sample = resman->getSoundEffect(tmpPath);
    if (sample)
    {
        logger->log("SoundManager::playSfx() Playing: %s", path.c_str());
        int vol = 120;
        if (player_node && (x > 0 || y > 0))
        {
            int dx = player_node->getTileX() - x;
            int dy = player_node->getTileY() - y;
            if (dx < 0)
                dx = -dx;
            if (dy < 0)
                dy = -dy;
            const int dist = dx > dy ? dx : dy;
            if (dist * 8 > vol)
                return;

            vol -= dist * 8;
        }
        sample->play(0, vol);
        if (!mCacheSounds)
            sample->decRef();
    }
}

void SoundManager::playGuiSound(const std::string &name)
{
    playGuiSfx(branding.getStringValue("systemsounds").append(
        config.getStringValue(name)).append(".ogg"));
}

void SoundManager::playGuiSfx(const std::string &path)
{
    if (!mInstalled || path.empty() || !mPlayGui)
        return;

    std::string tmpPath;
    if (!path.compare(0, 4, "sfx/"))
        tmpPath = path;
    else
        tmpPath = paths.getValue("sfx", "sfx/").append(path);
    ResourceManager *const resman = ResourceManager::getInstance();
    SoundEffect *const sample = resman->getSoundEffect(tmpPath);
    if (sample)
    {
        logger->log("SoundManager::playGuiSfx() Playing: %s", path.c_str());
        const int ret = sample->play(0, 120, mGuiChannel);
        if (ret != -1)
            mGuiChannel = ret;
        if (!mCacheSounds)
            sample->decRef();
    }
}

void SoundManager::close()
{
    if (!mInstalled)
        return;

    haltMusic();
    logger->log1("SoundManager::close() Shutting down sound...");
    Mix_CloseAudio();

    mInstalled = false;
}

void SoundManager::haltMusic()
{
    if (!mMusic)
        return;

    Mix_HaltMusic();
    mMusic->decRef();
    mMusic = nullptr;
    mCurrentMusicFile.clear();
}

void SoundManager::changeAudio()
{
    if (mInstalled)
        close();
    else
        init();
}

void SoundManager::volumeOff() const
{
    if (mInstalled)
    {
        Mix_VolumeMusic(0);
        Mix_Volume(-1, 0);
    }
}

void SoundManager::volumeRestore() const
{
    if (mInstalled)
    {
        Mix_VolumeMusic(mMusicVolume);
        Mix_Volume(-1, mSfxVolume);
    }
}
