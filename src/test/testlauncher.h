/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#ifndef TEST_TESTLAUNCHER_H
#define TEST_TESTLAUNCHER_H

#ifdef USE_OPENGL

#include <fstream>

#ifdef WIN32
#include <sys/time.h>
#endif  // WIN32

#ifdef __native_client__
#include <sys/time.h>
#endif  // __native_client__

#include "localconsts.h"

class TestLauncher final
{
    public:
        explicit TestLauncher(std::string test);

        A_DELETE_COPY(TestLauncher)

        ~TestLauncher();

        int exec();

        int calcFps(const timeval &start,
                    const timeval &end,
                    const int calls) const;

        int testBackend() const;

        int testSound() const;

        int testRescale() const;

        int testFps();

        int testFps2();

        int testFps3();

        int testInternal();

        int testDye();

        int testVideoDetection();

        int testBatches();

        int testTextures();

        int testDraw();

        int testDyeSSpeed();

        int testDyeASpeed();

        int testStackSpeed();

    private:
        std::string mTest;

        std::ofstream file;
};

#endif  // USE_OPENGL
#endif  // TEST_TESTLAUNCHER_H
