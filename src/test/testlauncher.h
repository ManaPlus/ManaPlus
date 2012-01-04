/*
 *  The ManaPlus Client
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

#ifndef TEST_TESTLAUNCHER_H
#define TEST_TESTLAUNCHER_H

#include "logger.h"

#include <string>
#include <sys/time.h>

class TestLauncher
{
    public:
        TestLauncher(std::string test);

        ~TestLauncher();

        int exec();

        int calcFps(timeval *start, timeval *end, int calls);

        int testBackend();

        int testSound();

        int testRescale();

        int testFps();

    private:
        std::string mTest;

        std::ofstream file;
};

#endif // TEST_TESTLAUNCHER_H
