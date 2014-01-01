/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#include "utils/stringutils.h"

#include "debug.h"

int mCpuFlags = 0;

void Cpu::detect()
{
#if defined(__GNUC__) && (GCC_VERSION >= 40800) && !defined(ANDROID)
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
    printFlags();
#elif defined(__linux__) || defined(__linux)
    FILE *file = fopen("/proc/cpuinfo", "r");
    char buf[1001];
    if (!file)
        return;
    while (fgets(buf, 1000, file))
    {
        std::string str = buf;
        if (findFirst(str, "flags"))
        {
            size_t idx = str.find(":");
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
            }
            fclose(file);
            printFlags();
            return;
        }
    }
    fclose(file);
    logger->log("cpu features was not detected");
#endif
}

void Cpu::printFlags()
{
    std::string str("CPU features:");
    if (mCpuFlags & FEATURE_MMX)
        str.append(" mmx");
    if (mCpuFlags & FEATURE_SSE)
        str.append(" sse");
    if (mCpuFlags & FEATURE_SSE2)
        str.append(" sse2");
    if (mCpuFlags & FEATURE_SSSE3)
        str.append(" ssse3");
    if (mCpuFlags & FEATURE_SSE4)
        str.append(" sse4");
    if (mCpuFlags & FEATURE_SSE42)
        str.append(" sse4_2");
    logger->log(str);
}
