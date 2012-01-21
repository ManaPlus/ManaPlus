/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef TEST_TESTMAIN_H
#define TEST_TESTMAIN_H

#include "logger.h"

#include <string>

class TestMain
{
    public:
        TestMain();

        int exec();

    private:
        void initConfig();

        int readValue(int ver, int def = 0);

        int invokeTest(std::string test);

        int invokeTest3();

        int invokeTest4();

        int invokeTest7();

        int invokeSoftwareRenderTest(std::string test);

        int invokeFastOpenGLRenderTest(std::string test);

        int invokeSafeOpenGLRenderTest(std::string test);

        void testsMain();

        void writeConfig(int openGLMode, int rescale,
                         int sound, std::string info);

        Logger *log;

        std::ifstream file;
};

#endif // TEST_TESTMAIN_H
