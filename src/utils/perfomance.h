/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef UTILS_PERFOMANCE_H
#define UTILS_PERFOMANCE_H

#ifdef USE_PROFILER
#include "utils/vector.h"

#include <string>
#include <sstream>
#include <list>
#include <set>

#include "localconsts.h"

#define PROFILER_START() Perfomance::start();
#define PROFILER_END() Perfomance::flush();
#define BLOCK_START(name) Perfomance::blockStart(name);
#define BLOCK_END(name) Perfomance::blockEnd(name);
#define FUNC_BLOCK(name, id) Perfomance::Func PerfomanceFunc##id(name);

namespace Perfomance
{
    void start();

    void init(const std::string &path);

    void clear();

    void blockStart(const std::string &name);

    void blockEnd(const std::string &name);

    void flush();

    class Func final
    {
        public:
            explicit Func(const std::string &str) :
                name(str)
            {
                blockStart(str);
            }

            A_DELETE_COPY(Func)

            ~Func()
            {
                blockEnd(name);
            }

            std::string name;
    };
}  // namespace Perfomance

#else  // USE_PROFILER

#define PROFILER_START()
#define PROFILER_END()
#define BLOCK_START(name)
#define BLOCK_END(name)
#define FUNC_BLOCK(name, id)

#endif  // USE_PROFILER
#endif  // UTILS_PERFOMANCE_H
