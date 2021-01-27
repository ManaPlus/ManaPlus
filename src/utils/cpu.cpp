/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "utils/cpu.h"

#include "logger.h"

#if (defined(__amd64__) || defined(__i386__)) && defined(__GNUC__) \
    && (GCC_VERSION >= 40800) && !defined(ANDROID)
// nothing
#elif defined(__linux__) || defined(__linux)
#include "utils/foreach.h"
#include "utils/stringutils.h"
#endif  // (defined(__amd64__) || defined(__i386__)) && defined(__GNUC__)
        // && (GCC_VERSION >= 40800) && !defined(ANDROID)

#ifdef USE_SDL2
#include <SDL_cpuinfo.h>
#endif  // USE_SDL2

#include "debug.h"

namespace
{
    uint32_t mCpuFlags;
}  // namespace

void Cpu::detect()
{
#if (defined(__amd64__) || defined(__i386__)) && defined(__GNUC__) \
    && (GCC_VERSION >= 40800) && !defined(ANDROID)
    __builtin_cpu_init();
    if (__builtin_cpu_supports ("mmx"))
        mCpuFlags |= FEATURE_MMX;
    if (__builtin_cpu_supports ("sse"))
        mCpuFlags |= FEATURE_SSE;
    if (__builtin_cpu_supports ("sse2"))
        mCpuFlags |= FEATURE_SSE2;
    if (__builtin_cpu_supports ("ssse3"))
        mCpuFlags |= FEATURE_SSSE3;
    if (__builtin_cpu_supports ("sse4.1"))
        mCpuFlags |= FEATURE_SSE4;
    if (__builtin_cpu_supports ("sse4.2"))
        mCpuFlags |= FEATURE_SSE42;
    if (__builtin_cpu_supports ("avx"))
        mCpuFlags |= FEATURE_AVX;
    if (__builtin_cpu_supports ("avx2"))
        mCpuFlags |= FEATURE_AVX2;
    printFlags();
#elif defined(__linux__) || defined(__linux)
    FILE *file = fopen("/proc/cpuinfo", "r");
    char buf[1001];
    if (file == nullptr)
        return;
    while (fgets(buf, 1000, file) != nullptr)
    {
        std::string str = buf;
        if (findFirst(str, "flags"))
        {
            size_t idx = str.find(':');
            if (idx == std::string::npos)
                continue;
            str = str.substr(idx + 1);
            trim(str);
            StringVect vect;
            splitToStringVector(vect, str, ' ');
            FOR_EACH (StringVectCIter, it, vect)
            {
                const std::string &flag = *it;
                if (flag == "mmx")
                    mCpuFlags |= FEATURE_MMX;
                else if (flag == "sse")
                    mCpuFlags |= FEATURE_SSE;
                else if (flag == "sse2")
                    mCpuFlags |= FEATURE_SSE2;
                else if (flag == "ssse3")
                    mCpuFlags |= FEATURE_SSSE3;
                else if (flag == "sse4_1")
                    mCpuFlags |= FEATURE_SSE4;
                else if (flag == "sse4_2")
                    mCpuFlags |= FEATURE_SSE42;
                else if (flag == "avx")
                    mCpuFlags |= FEATURE_AVX;
                else if (flag == "avx2")
                    mCpuFlags |= FEATURE_AVX2;
            }
            fclose(file);
            printFlags();
            return;
        }
    }
    fclose(file);
    if (logger != nullptr)
        logger->log("cpu features was not detected");
#else  // OTHER

#ifdef USE_SDL2
    if (SDL_HasMMX())
        mCpuFlags |= FEATURE_MMX;
    if (SDL_HasSSE())
        mCpuFlags |= FEATURE_SSE;
    if (SDL_HasSSE2())
        mCpuFlags |= FEATURE_SSE2;
    if (SDL_HasSSE3())
        mCpuFlags |= FEATURE_SSSE3;
    if (SDL_HasSSE41())
        mCpuFlags |= FEATURE_SSE4;
    if (SDL_HasSSE42())
        mCpuFlags |= FEATURE_SSE42;

#if SDL_VERSION_ATLEAST(2, 0, 2)
    if (SDL_HasAVX())
        mCpuFlags |= FEATURE_AVX;
#endif  // SDL_VERSION_ATLEAST(2, 0, 2)
#if SDL_VERSION_ATLEAST(2, 0, 4)
    if (SDL_HasAVX2())
        mCpuFlags |= FEATURE_AVX2;
#endif  // SDL_VERSION_ATLEAST(2, 0, 4)

    printFlags();
#else  // USE_SDL2

    if (logger)
        logger->log("cpu features not supported");
#endif  // USE_SDL2
#endif  // (defined(__amd64__) || defined(__i386__)) && defined(__GNUC__)
        // && (GCC_VERSION >= 40800) && !defined(ANDROID)

#if SDL_VERSION_ATLEAST(2, 0, 1)
    logger->log("System ram size: %d", SDL_GetSystemRAM());
#endif  // SDL_VERSION_ATLEAST(2, 0, 1)
}

void Cpu::printFlags()
{
    if (logger == nullptr)
        return;
    std::string str("CPU features:");
    if ((mCpuFlags & FEATURE_MMX) != 0U)
        str.append(" mmx");
    if ((mCpuFlags & FEATURE_SSE) != 0U)
        str.append(" sse");
    if ((mCpuFlags & FEATURE_SSE2) != 0U)
        str.append(" sse2");
    if ((mCpuFlags & FEATURE_SSSE3) != 0U)
        str.append(" ssse3");
    if ((mCpuFlags & FEATURE_SSE4) != 0U)
        str.append(" sse4");
    if ((mCpuFlags & FEATURE_SSE42) != 0U)
        str.append(" sse4_2");
    if ((mCpuFlags & FEATURE_AVX) != 0U)
        str.append(" avx");
    if ((mCpuFlags & FEATURE_AVX2) != 0U)
        str.append(" avx2");
    logger->log(str);
}

uint32_t Cpu::getFlags()
{
    return mCpuFlags;
}
