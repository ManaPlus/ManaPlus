/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2017  The ManaPlus Developers
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

#include "test/unittests.h"

#include "configuration.h"

#include "utils/env.h"
#include "utils/langs.h"

#include "debug.h"

TEST_CASE("Langs getLang", "")
{
    LangVect langs;

    config.setValue("lang", "C");
    langs = getLang();
    REQUIRE(langs.size() == 1);
    REQUIRE(langs[0] == "C");

    config.setValue("lang", "ru_RU");
    langs = getLang();
    REQUIRE(langs.size() == 2);
    REQUIRE(langs[0] == "ru_RU");
    REQUIRE(langs[1] == "ru");

    config.setValue("lang", "ru_RU.UTF-8");
    langs = getLang();
    REQUIRE(langs.size() == 2);
    REQUIRE(langs[0] == "ru_RU");
    REQUIRE(langs[1] == "ru");

    config.setValue("lang", "");

    setEnv("LANG", "C");
    langs = getLang();
    REQUIRE(langs.size() == 1);
    REQUIRE(langs[0] == "C");

    setEnv("LANG", "ru_RU");
    langs = getLang();
    REQUIRE(langs.size() == 2);
    REQUIRE(langs[0] == "ru_RU");
    REQUIRE(langs[1] == "ru");

    setEnv("LANG", "ru_RU.UTF-8");
    langs = getLang();
    REQUIRE(langs.size() == 2);
    REQUIRE(langs[0] == "ru_RU");
    REQUIRE(langs[1] == "ru");
}

TEST_CASE("Langs getLangSimple", "")
{
    config.setValue("lang", "C");
    REQUIRE(getLangSimple() == "C");

    config.setValue("lang", "ru_RU");
    REQUIRE(getLangSimple() == "ru_RU");

    config.setValue("lang", "ru_RU.UTF-8");
    REQUIRE(getLangSimple() == "ru_RU.UTF-8");

    config.setValue("lang", "");

    setEnv("LANG", "C");
    REQUIRE(getLangSimple() == "C");

    setEnv("LANG", "ru_RU");
    REQUIRE(getLangSimple() == "ru_RU");

    setEnv("LANG", "ru_RU.UTF-8");
    REQUIRE(getLangSimple() == "ru_RU.UTF-8");
}

TEST_CASE("Langs getLangShort", "")
{
    config.setValue("lang", "C");
    REQUIRE(getLangShort() == "C");

    config.setValue("lang", "ru_RU");
    REQUIRE(getLangShort() == "ru");

    config.setValue("lang", "ru_RU.UTF-8");
    REQUIRE(getLangShort() == "ru");

    config.setValue("lang", "");

    setEnv("LANG", "C");
    REQUIRE(getLangShort() == "C");

    setEnv("LANG", "ru_RU");
    REQUIRE(getLangShort() == "ru");

    setEnv("LANG", "ru_RU.UTF-8");
    REQUIRE(getLangShort() == "ru");
}
