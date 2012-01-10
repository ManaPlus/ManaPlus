/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "main.h"

#include "utils/gettext.h"

#include "client.h"
#include "logger.h"

#include <getopt.h>
#include <iostream>
#include <physfs.h>

#include "utils/stringutils.h"
#include "utils/xml.h"

#ifdef __MINGW32__
#include <windows.h>
#endif

#include "debug.h"

char *selfName = nullptr;

static void printHelp()
{
    using std::endl;

    std::cout
        << _("manaplus [options] [manaplus-file]") << endl << endl
        << _("[manaplus-file] : The manaplus file is an XML file (.manaplus)")
        << endl
        << _("                  used to set custom parameters") << endl
        << _("                  to the manaplus client.")
        << endl << endl
        << _("Options:") << endl
        << _("  -l --log-file       : Log file to use") << endl
        << _("  -L --chat-log-dir   : Chat log dir to use") << endl
        << _("  -v --version        : Display the version") << endl
        << _("  -h --help           : Display this help") << endl
        << _("  -C --config-dir     : Configuration directory to use") << endl
        << _("  -U --username       : Login with this username") << endl
        << _("  -P --password       : Login with this password") << endl
        << _("  -c --character      : Login with this character") << endl
        << _("  -s --server         : Login server name or IP") << endl
        << _("  -p --port           : Login server port") << endl
        << _("     --update-host    : Use this update host") << endl
        << _("  -D --default        : Choose default character server and "
                                     "character") << endl
        << _("  -u --skip-update    : Skip the update downloads") << endl
        << _("  -d --data           : Directory to load game "
             "data from") << endl
        << _("  -L --localdata-dir  : Directory to use as local data"
             " directory") << endl
        << _("     --screenshot-dir : Directory to store screenshots") << endl
        << _("     --safemode       : Start game in safe mode") << endl
        << _("  -T --tests          : Start testing drivers and "
                                     "auto configuring") << endl
#ifdef USE_OPENGL
        << _("     --no-opengl      : Disable OpenGL for this session") << endl
#endif
        ;
}

static void printVersion()
{
    std::cout << strprintf("ManaPlus client %s", FULL_VERSION) << std::endl;
}

static void parseOptions(int argc, char *argv[], Client::Options &options)
{
    const char *optstring = "hvud:U:P:Dc:p:l:L:C:s:t:T";

    const struct option long_options[] =
    {
        { "config-dir",     required_argument, 0, 'C' },
        { "data",           required_argument, 0, 'd' },
        { "default",        no_argument,       0, 'D' },
        { "password",       required_argument, 0, 'P' },
        { "character",      required_argument, 0, 'c' },
        { "help",           no_argument,       0, 'h' },
        { "localdata-dir",  required_argument, 0, 'L' },
        { "update-host",    required_argument, 0, 'H' },
        { "port",           required_argument, 0, 'p' },
        { "server",         required_argument, 0, 's' },
        { "skip-update",    no_argument,       0, 'u' },
        { "username",       required_argument, 0, 'U' },
        { "no-opengl",      no_argument,       0, 'O' },
        { "chat-log-dir",   required_argument, 0, 'l' },
        { "version",        no_argument,       0, 'v' },
        { "log-file",       required_argument, 0, 'l' },
        { "chat-log-dir",   required_argument, 0, 'L' },
        { "screenshot-dir", required_argument, 0, 'i' },
        { "safemode",       no_argument,       0, 'm' },
        { "tests",          no_argument,       0, 'T' },
        { "test",           required_argument, 0, 't' },
        { nullptr,          0,                 0, 0 }
    };

    while (optind < argc)
    {
        int result = getopt_long(argc, argv, optstring, long_options, nullptr);

        if (result == -1)
            break;

        switch (result)
        {
            case 'C':
                options.configDir = optarg;
                break;
            case 'd':
                options.dataPath = optarg;
                break;
            case 'D':
                options.chooseDefault = true;
                break;
            case '?': // Unknown option
            case ':': // Missing argument
            case 'h':
                options.printHelp = true;
                break;
            case 'H':
                if (checkPath(optarg))
                    options.updateHost = optarg;
                else
                    options.updateHost = "";
                break;
            case 'c':
                options.character = optarg;
                break;
            case 'P':
                options.password = optarg;
                break;
            case 's':
                options.serverName = optarg;
                break;
            case 'p':
                options.serverPort = static_cast<short>(atoi(optarg));
                break;
            case 'u':
                options.skipUpdate = true;
                break;
            case 'U':
                options.username = optarg;
                break;
            case 'v':
                options.printVersion = true;
                break;
            case 'S':
                options.localDataDir = optarg;
                break;
            case 'O':
                options.noOpenGL = true;
                break;
            case 'l':
                options.logFileName = std::string(optarg);
                break;
            case 'L':
                options.chatLogDir = std::string(optarg);
            case 'i':
                options.screenshotDir = optarg;
                break;
            case 'm':
                options.safeMode = true;
                break;
            case 'T':
                options.testMode = true;
                options.test = "";
                break;
            case 't':
                options.testMode = true;
                options.test = std::string(optarg);
                break;
            default:
                break;
        }
    }

    // when there are still options left use the last
    // one as branding file
    if (optind < argc)
    {
        options.brandingPath = argv[optind];
    }
}

#ifdef WIN32
extern "C" char const *_nl_locale_name_default(void);
#endif

int main(int argc, char *argv[])
{
#if defined(__MINGW32__)
    // load mingw crash handler. Won't fail if dll is not present.
    // may load libray from current dir, it may not same as program dir
    LoadLibrary("exchndl.dll");
#endif

    selfName = argv[0];

    // Parse command line options
    Client::Options options;
    parseOptions(argc, argv, options);

    if (options.printHelp)
    {
        printHelp();
        return 0;
    }
    else if (options.printVersion)
    {
        printVersion();
        return 0;
    }

    // Initialize PhysicsFS
    if (!PHYSFS_init(argv[0]))
    {
        std::cout << "Error while initializing PhysFS: "
                  << PHYSFS_getLastError() << std::endl;
        return 1;
    }

    atexit((void(*)()) PHYSFS_deinit);

    XML::initXML();

#ifdef WIN32
    SetCurrentDirectory(PHYSFS_getBaseDir());
#endif
    Client client(options);
    if (!options.testMode)
    {
        client.gameInit();
        return client.gameExec();
    }
    else
    {
        client.testsInit();
        return client.testsExec();
    }
}
