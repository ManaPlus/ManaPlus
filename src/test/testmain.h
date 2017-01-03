/*
 *  The ManaPlus Client
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

#ifndef TEST_TESTMAIN_H
#define TEST_TESTMAIN_H

#ifdef USE_OPENGL

#include "configuration.h"

#include "render/renderers.h"

class Logger;

class TestMain final
{
    public:
        TestMain();

        A_DELETE_COPY(TestMain)

        ~TestMain();

        int exec(const bool testAudio = true);

        static int readValue(const int ver, int def);

        Configuration &getConfig()
        { return mConfig; }

    private:
        void initConfig();

        int invokeTest(const std::string &test);

        int invokeTest3();

        int invokeTest4();

        int invokeTest7();

        int invokeSoftwareRenderTest(const std::string &test);

        int invokeNormalOpenGLRenderTest(const std::string &test);

        int invokeNormalOpenBatchTest(const std::string &test);

        int invokeModernOpenGLRenderTest(const std::string &test);

        int invokeModernOpenBatchTest(const std::string &test);

        int invokeMobileOpenBatchTest(const std::string &test);

        int invokeSafeOpenBatchTest(const std::string &test);

        int invokeSafeOpenGLRenderTest(const std::string &test);

        void testsMain();

        void writeConfig(const RenderType openGLMode,
                         const int rescale,
                         const int sound,
                         const std::string &info,
                         const int batchSize,
                         const std::string &textureSize,
                         const int detectMode);

        int readValue2(const int ver);

        Logger *log;

        Configuration mConfig;
};

#endif  // USE_OPENGL
#endif  // TEST_TESTMAIN_H
