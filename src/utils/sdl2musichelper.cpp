/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifdef USE_SDL2

#include "utils/sdl2musichelper.h"

#include "debug.h"

#ifndef SDL_MIXER_COMPILEDVERSION
#define SDL_MIXER_COMPILEDVERSION \
    SDL_VERSIONNUM(SDL_MIXER_MAJOR_VERSION, \
    SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL)
#endif  // SDL_MIXER_COMPILEDVERSION
#ifndef SDL_MIXER_VERSION_ATLEAST
#define SDL_MIXER_VERSION_ATLEAST(X, Y, Z) \
    (SDL_MIXER_COMPILEDVERSION >= SDL_VERSIONNUM(X, Y, Z))
#endif  // SDL_MIXER_VERSION_ATLEAST

int SDL::MixOpenAudio(const int frequency,
                      const uint16_t format,
                      const int nchannels,
                      const int chunksize)
{
#if SDL_MIXER_VERSION_ATLEAST(2, 0, 2)
    return Mix_OpenAudioDevice(frequency,
        format,
        nchannels,
        chunksize,
        nullptr,
        SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_CHANNELS_CHANGE);
#else  // SDL_MIXER_VERSION_ATLEAST(2, 0, 2)

    return Mix_OpenAudio(frequency,
        format,
        nchannels,
        chunksize);
#endif  // SDL_MIXER_VERSION_ATLEAST(2, 0, 2)
}

Mix_Music *SDL::LoadMUSOgg_RW(SDL_RWops *const rw)
{
    return Mix_LoadMUSType_RW(rw, MUS_OGG, 1);
}

#endif  // USE_SDL2
