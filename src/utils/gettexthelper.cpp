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

#include "utils/gettexthelper.h"

#if defined(ENABLE_NLS) || defined(ENABLE_CUSTOMNLS)
#include "configuration.h"
#endif  // defined(ENABLE_NLS) || defined(ENABLE_CUSTOMNLS)

#ifdef ENABLE_NLS
#include "client.h"
#include "logger.h"

#include "utils/physfstools.h"

#include <libintl.h>

#ifdef WIN32
#include <string>
extern "C" char const *_nl_locale_name_default(void);
#endif  // WIN32
#elif defined(ENABLE_CUSTOMNLS)
#include "utils/translation/podict.h"
#ifdef __native_client__
#include "utils/naclmessages.h"
#endif  // __native_client__
#endif  // ENABLE_NLS

#if defined(ENABLE_NLS) || defined(ENABLE_CUSTOMNLS) && !defined(WIN32)
#include "utils/env.h"
#endif  // defined(ENABLE_NLS) || defined(ENABLE_CUSTOMNLS) && !defined(WIN32)

#include "debug.h"

#if defined(ENABLE_NLS) || defined(ENABLE_CUSTOMNLS)
static std::string setLangEnv()
{
    std::string lang = config.getStringValue("lang");
#if defined(ENABLE_NLS) && defined(WIN32)
    if (lang.empty())
        lang = std::string(_nl_locale_name_default());
#elif defined(ENABLE_CUSTOMNLS) && defined(__native_client__)
    if (lang.empty())
    {
        NaclMessageHandle *handle = naclRegisterMessageHandler(
            "get-uilanguage");
        naclPostMessage("get-uilanguage", "");
        lang = naclWaitForMessage(handle);
    }
#endif  // defined(ENABLE_NLS) && defined(WIN32)

    if (!lang.empty())
    {
#ifdef WIN32
        putenv(const_cast<char*>(("LANG=" + lang).c_str()));
        putenv(const_cast<char*>(("LANGUAGE=" + lang).c_str()));
#else  // WIN32

        if (!lang.empty())
        {
            setEnv("LANG", lang.c_str());
            setEnv("LANGUAGE", lang.c_str());
        }
#endif  // WIN32
    }

    return lang;
}
#endif  // defined(ENABLE_NLS) || defined(ENABLE_CUSTOMNLS)

void GettextHelper::initLang()
{
#ifdef ENABLE_NLS
    const std::string lang = setLangEnv();
#ifdef WIN32
    // mingw doesn't like LOCALEDIR to be defined for some reason
    if (lang != "C")
        bindTextDomain("translations/");
#else  // WIN32
#ifdef ANDROID
#ifdef USE_SDL2
    bindTextDomain((std::string(getenv("APPDIR")).append("/locale")).c_str());
#else  // USE_SDL2

    bindTextDomain((std::string(PhysFs::getBaseDir()).append(
        "/locale")).c_str());
#endif  // USE_SDL2
#else  // ANDROID
#ifdef ENABLE_PORTABLE
    bindTextDomain((std::string(PhysFs::getBaseDir()).append(
        "../locale/")).c_str());
#else  // ENABLE_PORTABLE
#ifdef __APPLE__
    bindTextDomain((std::string(PhysFs::getBaseDir())
        .append("ManaPlus.app/Contents/Resources/locale/")).c_str());
#else  // __APPLE__

    bindTextDomain(LOCALEDIR);
#endif  // __APPLE__
#endif  // ENABLE_PORTABLE
#endif  // ANDROID
#endif  // WIN32

    char *locale = setlocale(LC_MESSAGES, lang.c_str());
    if (locale)
    {
        logger->log("locale: %s", locale);
    }
    else
    {
        locale = setlocale(LC_MESSAGES, (lang + ".utf8").c_str());
        if (locale)
            logger->log("locale: %s", locale);
        else
            logger->log("locale empty");
    }
    bind_textdomain_codeset("manaplus", "UTF-8");
    textdomain("manaplus");
#elif defined(ENABLE_CUSTOMNLS)
    mainTranslator = new PoDict("en");
    setLangEnv();
#endif  // ENABLE_NLS
}

#ifdef ENABLE_NLS
void GettextHelper::bindTextDomain(const char *const path)
{
    const char *const dir = bindtextdomain("manaplus", path);
    if (dir)
        logger->log("bindtextdomain: %s", dir);
    else
        logger->log("bindtextdomain failed");
}
#else  // ENABLE_NLS

void GettextHelper::bindTextDomain(const char *const path A_UNUSED)
{
}
#endif  // ENABLE_NLS
