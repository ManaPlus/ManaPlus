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

#include "utils/fuzzer.h"

#ifdef USE_FUZZER

#include "client.h"
#include "logger.h"

#include "utils/stringutils.h"

#include "debug.h"

namespace
{
    Logger *fuzz = nullptr;
    int fuzzRand = 50;
}  // namespace

void Fuzzer::init()
{
    fuzz = new Logger;
    fuzz->setLogFile(client->getLocalDataDirectory() + "/fuzzer.log");
    unsigned int sr = time(nullptr);

    fuzz->log("Srand: %u", sr);
    srand(sr);
}

bool Fuzzer::conditionTerminate(const char *const name)
{
    if ((rand() % 100) <= fuzzRand)
    {
        fuzz->log("deleted: %s", name);
        return true;
    }
    fuzz->log("passed: %s", name);
    return false;
}

#endif
