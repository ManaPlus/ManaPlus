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

#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SDL_mixer.h>

#include "listeners/configlistener.h"

#include <string>

#include "localconsts.h"

class SDLMusic;

/** SoundManager
 *
 * \ingroup CORE
 */
class SoundManager final : public ConfigListener
{
    public:
        SoundManager();

        A_DELETE_COPY(SoundManager)

        ~SoundManager();

        void optionChanged(const std::string &value) override final;

        /**
         * Installs the sound engine.
         */
        void init();

        /**
         * Removes all sound functionalities.
         */
        void close();

        /**
         * Starts background music.
         *
         * @param fileName The name of the music file.
         */
        void playMusic(const std::string &fileName);

        /**
         * Stops currently running background music track.
         */
        void stopMusic();

        /**
         * Fades in background music.
         *
         * @param fileName The name of the music file.
         * @param ms       Duration of fade-in effect (ms)
         */
//        void fadeInMusic(const std::string &fileName, const int ms = 1000);

        /**
         * Fades out currently running background music track.
         *
         * @param ms   Duration of fade-out effect (ms)
         */
        void fadeOutMusic(const int ms = 1000);

        /**
         * Fades out a background music and play a new one.
         *
         * @param fileName The name of the music file.
         * @param ms       Duration of fade-out effect (ms)
         */
        void fadeOutAndPlayMusic(const std::string &fileName,
                                 const int ms = 1000);

        static int getMaxVolume() A_WARN_UNUSED
        { return MIX_MAX_VOLUME; }

        void setMusicVolume(const int volume);

        void setSfxVolume(const int volume);

        /**
         * Plays an item.
         *
         * @param path The resource path to the sound file.
         */
        void playSfx(const std::string &path, const int x = 0,
                     const int y = 0) const;

        /**
         * Plays an item for gui.
         *
         * @param path The resource path to the sound file.
         */
        void playGuiSfx(const std::string &path);

        void playGuiSound(const std::string &name);

        void changeAudio();

        void volumeOff() const;

        void volumeRestore() const;

        std::string getCurrentMusicFile() const A_WARN_UNUSED
        { return mCurrentMusicFile; }

        /**
         * The sound logic.
         * Currently used to check whether the music file can be freed after
         * a fade out, and whether new music has to be played.
         */
        void logic();

        void shutdown();

    private:
        /** Logs various info about sound device. */
        static void info();

        /** Halts and frees currently playing music. */
        void haltMusic();

        /**
         * When calling fadeOutAndPlayMusic(),
         * the music file below will then be played
         */
        std::string mNextMusicFile;

        bool mInstalled;

        int mSfxVolume;
        int mMusicVolume;

        std::string mCurrentMusicFile;
        SDLMusic *mMusic;
        int mGuiChannel;
        bool mPlayBattle;
        bool mPlayGui;
        bool mPlayMusic;
        bool mFadeoutMusic;
        bool mCacheSounds;
};

extern SoundManager soundManager;

#endif  // SOUNDMANAGER_H
