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

#include "resources/sdlmusic.h"

#include "debug.h"

#ifndef USE_SDL2
#define SDL_MIXER_COMPILEDVERSION \
    SDL_VERSIONNUM(SDL_MIXER_MAJOR_VERSION, \
    SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL)

#define SDL_MIXER_VERSION_ATLEAST(X, Y, Z) \
    (SDL_MIXER_COMPILEDVERSION >= SDL_VERSIONNUM(X, Y, Z))
#endif  // USE_SDL2

SDLMusic::SDLMusic(Mix_Music *const music,
                   SDL_RWops *const rw,
                   const std::string &name) :
    Resource(),
    mName(name),
    mMusic(music),
    mRw(rw)
{
}

SDLMusic::~SDLMusic()
{
    Mix_FreeMusic(mMusic);
#ifndef USE_SDL2
#if SDL_MIXER_VERSION_ATLEAST(1, 2, 12)
    if (mRw != nullptr)
    {
        SDL_RWclose(mRw);
        mRw = nullptr;
    }
#endif  // SDL_MIXER_VERSION_ATLEAST(1, 2, 12)
#endif  // USE_SDL2
}

bool SDLMusic::play(const int loops, const int fadeIn)
{
    if (fadeIn > 0)
        return Mix_FadeInMusicPos(mMusic, loops, fadeIn, 0.0) != 0;
    else
        return Mix_FadeInMusicPos(mMusic, loops, 0, 0.0) != 0;
}

int SDLMusic::calcMemoryLocal() const
{
    // +++ not used size of SDL_RWops
    return static_cast<int>(sizeof(SDLMusic)) +
        Resource::calcMemoryLocal();
}
