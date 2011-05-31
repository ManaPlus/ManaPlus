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

#ifndef SOUND_H
#define SOUND_H

#include <SDL_mixer.h>

#include "configlistener.h"

#include <string>

/** Sound engine
 *
 * \ingroup CORE
 */
class Sound : public ConfigListener
{
    public:
        Sound();
        ~Sound();

        void optionChanged(const std::string &value);

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
         * @param path The full path to the music file.
         */
        void playMusic(const std::string &path);

        /**
         * Stops currently running background music track.
         */
        void stopMusic();

        /**
         * Fades in background music.
         *
         * @param path The full path to the music file.
         * @param ms   Duration of fade-in effect (ms)
         */
        void fadeInMusic(const std::string &path, int ms = 1000);

        /**
         * Fades out currently running background music track.
         *
         * @param ms   Duration of fade-out effect (ms)
         */
        void fadeOutMusic(int ms = 1000);

        /**
         * Fades out a background music and play a new one.
         *
         * @param path The full path to the fade in music file.
         * @param ms   Duration of fade-out effect (ms)
         */
        void fadeOutAndPlayMusic(const std::string &path, int ms = 1000);

        int getMaxVolume() const
        { return MIX_MAX_VOLUME; }

        void setMusicVolume(int volume);
        void setSfxVolume(int volume);

        /**
         * Plays an item.
         *
         * @param path The resource path to the sound file.
         */
        void playSfx(const std::string &path, int x = 0, int y = 0);

        /**
         * Plays an item for gui.
         *
         * @param path The resource path to the sound file.
         */
        void playGuiSfx(const std::string &path);

        void changeAudio();

        void volumeOff();

        void volumeRestore();

        std::string getCurrentMusicFile()
        { return mCurrentMusicFile; }

        /**
         * The sound logic.
         * Currently used to check whether the music file can be freed after
         * a fade out, and whether new music has to be played.
         */
        void logic();

    private:
        /** Logs various info about sound device. */
        void info();

        /** Halts and frees currently playing music. */
        void haltMusic();

        /**
         * When calling fadeOutAndPlayMusic(),
         * the music file below will then be played
         */
        std::string mNextMusicPath;

        bool mInstalled;

        int mSfxVolume;
        int mMusicVolume;

        std::string mCurrentMusicFile;
        Mix_Music *mMusic;
        bool mPlayBattle;
        bool mPlayGui;
        bool mPlayMusic;
};

extern Sound sound;

#endif
