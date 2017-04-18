/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2017  The ManaPlus Developers
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

#include "catch.hpp"
#include "configuration.h"
#include "configmanager.h"
#include "dirs.h"
#include "logger.h"

#include "listeners/configlistener.h"

#include "utils/delete2.h"

#include "debug.h"

namespace
{
    int mCalled = 0;

    class TestConfigListener : public ConfigListener
    {
        void optionChanged(const std::string &name) override final
        {
            if (name == "testkey123")
                mCalled ++;
        }
    } testListener;
}  // namespace

TEST_CASE("configuration tests", "configuration")
{
    logger = new Logger();

    Dirs::initRootDir();
    Dirs::initHomeDir();

    ConfigManager::initConfiguration();

    SECTION("configuration undefined")
    {
        const char *const key = "nonsetvalue";
        REQUIRE(config.getValue(key, "not set") == "not set");
        REQUIRE(config.getValue(key, 12345) == 12345);
        REQUIRE(config.getValue(key, 12345U) == 12345U);
        REQUIRE(config.getValueInt(key, 12345) == 12345);
        REQUIRE(config.getValueBool(key, false) == false);
        REQUIRE(config.getValueBool(key, true) == true);
        REQUIRE(config.getValue(key, -12345) == -12345);
        REQUIRE(config.getValue(key, 12.345) > 12.3);

        REQUIRE(config.getIntValue(key) == 0);
        REQUIRE(config.getFloatValue(key) >= 0.0f);
        REQUIRE(config.getStringValue(key).empty());
        REQUIRE(config.getBoolValue(key) == false);
    }

    SECTION("configuration getint default")
    {
        const char *const key = "sfxVolume";
        REQUIRE(config.getIntValue(key) == 100);
        REQUIRE(config.getFloatValue(key) >= 100.0f);
        REQUIRE(config.getStringValue(key) == "100");
        REQUIRE(config.getBoolValue(key) == true);
    }

    SECTION("configuration getfloat default")
    {
        const char *const key = "guialpha";
        REQUIRE(config.getIntValue(key) == 0);
        REQUIRE(config.getFloatValue(key) >= 0.8f);
        REQUIRE(config.getStringValue(key).substr(0, 3) == "0.8");
        REQUIRE(config.getBoolValue(key) == false);
    }

    SECTION("configuration getstring default")
    {
        const char *const key = "soundwhisper";
        REQUIRE(config.getIntValue(key) == 0);
        REQUIRE(config.getFloatValue(key) >= 0.0f);
        REQUIRE(config.getStringValue(key) == "newmessage");
        REQUIRE(config.getBoolValue(key) == true);
    }

    SECTION("configuration getbool default1")
    {
        const char *const key = "showgender";
        REQUIRE(config.getIntValue(key) == 1);
        REQUIRE(config.getFloatValue(key) >= 1.0f);
        REQUIRE(config.getStringValue(key) == "1");
        REQUIRE(config.getBoolValue(key) == true);
    }

    SECTION("configuration getbool default2")
    {
        const char *const key = "showlevel";
        REQUIRE(config.getIntValue(key) == 0);
        REQUIRE(config.getFloatValue(key) >= 0.0f);
        REQUIRE(config.getStringValue(key) == "0");
        REQUIRE(config.getBoolValue(key) == false);
    }

    SECTION("configuration getint set")
    {
        const char *const key = "sfxVolume";
        config.setValue(key, 50);
        REQUIRE(config.getIntValue(key) == 50);
        REQUIRE(config.getFloatValue(key) >= 50.0f);
        REQUIRE(config.getStringValue(key) == "50");
        REQUIRE(config.getBoolValue(key) == true);

        REQUIRE(config.getValue(key, "not set") == "50");
        REQUIRE(config.getValue(key, 12345) == 50);
        REQUIRE(config.getValue(key, 12345U) == 50U);
        REQUIRE(config.getValueInt(key, 12345) == 50);
        REQUIRE(config.getValueBool(key, false) == true);
        REQUIRE(config.getValueBool(key, true) == true);
        REQUIRE(config.getValue(key, -12345) == 50);
        REQUIRE(config.getValue(key, 12.345) >= 50.0);
    }

    SECTION("configuration getfloat set")
    {
        const char *const key = "guialpha";
        config.setValue(key, 50.5);
        REQUIRE(config.getIntValue(key) == 50);
        REQUIRE(config.getFloatValue(key) >= 50.4f);
        REQUIRE(config.getStringValue(key).substr(0, 2) == "50");
        REQUIRE(config.getBoolValue(key) == true);

        REQUIRE(config.getValue(key, "not set").substr(0, 2) == "50");
        REQUIRE(config.getValue(key, 12345) == 50);
        REQUIRE(config.getValue(key, 12345U) == 50U);
        REQUIRE(config.getValueInt(key, 12345) == 50);
        REQUIRE(config.getValueBool(key, false) == true);
        REQUIRE(config.getValueBool(key, true) == true);
        REQUIRE(config.getValue(key, -12345) == 50);
        REQUIRE(config.getValue(key, 12.345) >= 50.4);
    }

    SECTION("configuration getstring set")
    {
        const char *const key = "soundwhisper";
        config.setValue(key, "test line");
        REQUIRE(config.getIntValue(key) == 0);
        REQUIRE(config.getFloatValue(key) >= 0.0f);
        REQUIRE(config.getStringValue(key) == "test line");
        REQUIRE(config.getBoolValue(key) == false);

        REQUIRE(config.getValue(key, "not set") == "test line");
        REQUIRE(config.getValue(key, 12345) == 0);
        REQUIRE(config.getValue(key, 12345U) == 0U);
        REQUIRE(config.getValueInt(key, 12345) == 0);
        REQUIRE(config.getValueBool(key, false) == false);
        REQUIRE(config.getValueBool(key, true) == false);
        REQUIRE(config.getValue(key, -12345) == 0);
        REQUIRE(config.getValue(key, 12.345) >= 0.0);
    }

    SECTION("configuration getbool set1")
    {
        const char *const key = "showgender";
        config.setValue(key, true);
        REQUIRE(config.getIntValue(key) == 1);
        REQUIRE(config.getFloatValue(key) >= 1.0f);
        REQUIRE(config.getStringValue(key) == "1");
        REQUIRE(config.getBoolValue(key) == true);

        REQUIRE(config.getValue(key, "not set") == "1");
        REQUIRE(config.getValue(key, 12345) == 1);
        REQUIRE(config.getValue(key, 12345U) == 1U);
        REQUIRE(config.getValueInt(key, 12345) == 1);
        REQUIRE(config.getValueBool(key, false) == true);
        REQUIRE(config.getValueBool(key, true) == true);
        REQUIRE(config.getValue(key, -12345) == 1);
        REQUIRE(config.getValue(key, 12.345) >= 1.0);
    }

    SECTION("configuration getbool set2")
    {
        const char *const key = "showgender";
        config.setValue(key, false);
        REQUIRE(config.getIntValue(key) == 0);
        REQUIRE(config.getFloatValue(key) >= 0.0f);
        REQUIRE(config.getStringValue(key) == "0");
        REQUIRE(config.getBoolValue(key) == false);

        REQUIRE(config.getValue(key, "not set") == "0");
        REQUIRE(config.getValue(key, 12345) == 0);
        REQUIRE(config.getValue(key, 12345U) == 0U);
        REQUIRE(config.getValueInt(key, 12345) == 0);
        REQUIRE(config.getValueBool(key, false) == false);
        REQUIRE(config.getValueBool(key, true) == false);
        REQUIRE(config.getValue(key, -12345) == 0);
        REQUIRE(config.getValue(key, 12.345) >= 0.0);
    }

    SECTION("configuration deletekey")
    {
        const char *const key = "testkey123";
        config.setValue(key, 123);
        REQUIRE(config.getValueInt(key, 12345) == 123);
        config.deleteKey(key);
        REQUIRE(config.getValueInt(key, 12345) == 12345);
    }

    SECTION("configuration addlistener")
    {
        const char *const key = "testkey123";
        REQUIRE(mCalled == 0);
        config.addListener(key, &testListener);
        REQUIRE(mCalled == 0);
        config.setValue(key, 123);
        REQUIRE(mCalled == 1);
        REQUIRE(config.getValueInt(key, 12345) == 123);
        REQUIRE(mCalled == 1);
        config.setValue(key, 123);
        REQUIRE(mCalled == 2);
        config.setSilent(key, true);
        REQUIRE(mCalled == 2);
        REQUIRE(config.getBoolValue(key) == true);
        REQUIRE(mCalled == 2);
        config.setSilent(key, false);
        REQUIRE(mCalled == 2);
        REQUIRE(config.getBoolValue(key) == false);
        REQUIRE(mCalled == 2);
        config.removeListener(key, &testListener);
    }

    SECTION("configuration incvalue")
    {
        const char *const key = "testkey123";
        config.setValue(key, 10);
        REQUIRE(config.getValueInt(key, 12345) == 10);
        config.incValue(key);
        REQUIRE(config.getValueInt(key, 12345) == 11);
    }

    SECTION("configuration resetintvalue")
    {
        const char *const key = "sfxVolume";
        config.setValue(key, 20);
        REQUIRE(config.getIntValue(key) == 20);
        REQUIRE(config.getFloatValue(key) >= 20.0f);
        REQUIRE(config.getStringValue(key) == "20");
        REQUIRE(config.getBoolValue(key) == true);
        config.resetIntValue(key);
        REQUIRE(config.getIntValue(key) == 100);
        REQUIRE(config.getFloatValue(key) >= 100.0f);
        REQUIRE(config.getStringValue(key) == "100");
        REQUIRE(config.getBoolValue(key) == true);
    }

    SECTION("configuration resetboolvalue1")
    {
        const char *const key = "showgender";
        config.setValue(key, false);
        REQUIRE(config.getIntValue(key) == 0);
        REQUIRE(config.getFloatValue(key) >= 0.0f);
        REQUIRE(config.getStringValue(key) == "0");
        REQUIRE(config.getBoolValue(key) == false);

        config.resetBoolValue(key);
        REQUIRE(config.getIntValue(key) == 1);
        REQUIRE(config.getFloatValue(key) >= 1.0f);
        REQUIRE(config.getStringValue(key) == "1");
        REQUIRE(config.getBoolValue(key) == true);
    }

    SECTION("configuration resetboolvalue2")
    {
        const char *const key = "showlevel";
        config.setValue(key, true);
        REQUIRE(config.getIntValue(key) == 1);
        REQUIRE(config.getFloatValue(key) >= 1.0f);
        REQUIRE(config.getStringValue(key) == "1");
        REQUIRE(config.getBoolValue(key) == true);

        config.resetBoolValue(key);
        REQUIRE(config.getIntValue(key) == 0);
        REQUIRE(config.getFloatValue(key) >= 0.0f);
        REQUIRE(config.getStringValue(key) == "0");
        REQUIRE(config.getBoolValue(key) == false);
    }

    delete2(logger);
}
