/*
 *  The ManaPlus Client
 *  Copyright (C) 2013  The ManaPlus Developers
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

#include "utils/sdlcheckutils.h"

#ifdef DEBUG_SDL_SURFACES

#include "logger.h"

#include "utils/stringutils.h"

#include <map>

#include <SDL_image.h>
#include <SDL_ttf.h>

#include "debug.h"

// #define DEBUG_SURFACE_ALLOCATION 1

struct SurfaceObject
{
    SurfaceObject(const std::string name, const char *const file,
                  const unsigned int line) :
        mName(name),
        mAddFile(strprintf("%s:%u", file, line)),
        mRemoveFile(),
        mCnt(1)
    {
    }

    std::string mName;
    std::string mAddFile;
    std::string mRemoveFile;
    int mCnt;
};

std::map<SDL_Surface*, SurfaceObject*> mSurfaces;

static SDL_Surface *addSurface(const char *const name,
                               SDL_Surface *const surface,
                               const char *const file,
                               const unsigned line)
{
#ifdef DEBUG_SURFACE_ALLOCATION
    logger->log("add surface: %s %s:%u %p", name, file, line, static_cast<void*>(surface));
#endif
    std::map<SDL_Surface*, SurfaceObject*>::iterator
        it = mSurfaces.find(surface);
    if (it != mSurfaces.end())
    {
        SurfaceObject *const obj = (*it).second;
        if (obj)
        {   // found some time ago created surface
#ifdef DEBUG_SURFACE_ALLOCATION
            logger->log("adding existing surface: %p, count:%d\n"
                "was add %s\nwas deleted %s", surface, obj->mCnt,
                obj->mAddFile.c_str(), obj->mRemoveFile.c_str());
#endif
            obj->mCnt ++;
        }
    }
    else
    {   // creating surface object
        mSurfaces[surface] = new SurfaceObject(name, file, line);
    }
    return surface;
}

static void deleteSurface(const char *const name,
                          SDL_Surface *const surface,
                          const char *const file,
                          const unsigned line)
{
#ifdef DEBUG_SURFACE_ALLOCATION
    logger->log("delete surface: %s %s:%u %p", name, file, line, surface);
#endif
    std::map<SDL_Surface*, SurfaceObject*>::iterator
        it = mSurfaces.find(surface);
    if (it == mSurfaces.end())
    {
        logger->log("bad surface delete: %p at %s:%d", surface, file, line);
    }
    else
    {
        SurfaceObject *const obj = (*it).second;
        if (obj)
        {
            const int cnt = obj->mCnt;
#ifdef DEBUG_SURFACE_ALLOCATION
            logger->log("debug deleting surface: %p, count:%d\n"
                "was add %s\nwas deleted %s", surface, cnt,
                obj->mAddFile.c_str(), obj->mRemoveFile.c_str());
#endif
            if (cnt < 1)
            {   // surface was here but was deleted
                logger->log("deleting already deleted surface: %p at %s:%d\n"
                    "was add %s\nwas deleted %s", surface, file, line,
                    obj->mAddFile.c_str(), obj->mRemoveFile.c_str());
            }
            else if (cnt == 1)
            {
                mSurfaces.erase(surface);
                delete obj;
            }
            else
            {
                obj->mCnt --;
                obj->mRemoveFile = strprintf("%s:%u", file, line);
            }
        }
    }
}

SDL_Surface *FakeIMG_LoadPNG_RW(SDL_RWops *const src, const char *const file,
                                const unsigned line)
{
    return addSurface("IMG_LoadPNG_RW", IMG_LoadPNG_RW(src), file, line);
}

SDL_Surface *FakeIMG_Load(const char *name, const char *const file,
                          const unsigned line)
{
    return addSurface("IMG_Load", IMG_Load(name), file, line);
}

void FakeSDL_FreeSurface(SDL_Surface *const surface, const char *const file,
                         const unsigned line)
{
    deleteSurface("SDL_FreeSurface", surface, file, line);
    SDL_FreeSurface(surface);
}

SDL_Surface *FakeSDL_CreateRGBSurface(const uint32_t flags,
                                      const int width, const int height,
                                      const int depth, const uint32_t rMask,
                                      const uint32_t gMask,
                                      const uint32_t bMask,
                                      const uint32_t aMask,
                                      const char *const file,
                                      const unsigned line)
{
    return addSurface("SDL_CreateRGBSurface", SDL_CreateRGBSurface(flags,
        width, height, depth, rMask, gMask, bMask, aMask), file, line);
}

SDL_Surface *FakeSDL_ConvertSurface(SDL_Surface *const src,
                                    SDL_PixelFormat *const fmt,
                                    const uint32_t flags,
                                    const char *const file,
                                    const unsigned line)
{
    return addSurface("SDL_ConvertSurface", SDL_ConvertSurface(
        src, fmt, flags), file, line);
}

SDL_Surface *FakeTTF_RenderUTF8_Blended(_TTF_Font *const font,
                                        const char *const text,
                                        const SDL_Color fg,
                                        const char *const file,
                                        const unsigned line)
{
    return addSurface("TTF_RenderUTF8_Blended", TTF_RenderUTF8_Blended(
        font, text, fg), file, line);
}

#endif  // DEBUG_SDL_SURFACES
