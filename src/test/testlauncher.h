/*
 *  The ManaPlus Client
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

#ifndef TEST_TESTLAUNCHER_H
#define TEST_TESTLAUNCHER_H

#include "main.h"

#ifdef USE_OPENGL

#include <fstream>
#include <string>

#ifdef WIN32
#include <sys/time.h>
#endif

#include "localconsts.h"

class TestLauncher final
{
    public:
        explicit TestLauncher(std::string test);

        A_DELETE_COPY(TestLauncher)

        ~TestLauncher();

        int exec();

        int calcFps(const timeval *const start, const timeval *const end,
                    const int calls) const;

        int testBackend() const;

        int testSound() const;

        int testRescale() const;

        int testFps();

        int testInternal();

        int testDye();

        int testVideoDetection();

        int testBatches();

    private:
        std::string mTest;

        std::ofstream file;
};

#endif
#endif  // TEST_TESTLAUNCHER_H
