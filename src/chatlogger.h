/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
 *  Copyright (C) 2009-2010  Andrei Karas
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

#ifndef CHATLOGGER_H
#define CHATLOGGER_H

#include <fstream>
#include <list>

#include "localconsts.h"

class ChatLogger final
{
    public:
        /**
         * Constructor.
         */
        ChatLogger();

        A_DELETE_COPY(ChatLogger)

        /**
         * Destructor, closes log file.
         */
        ~ChatLogger();

        /**
         * Enters a message in the log. The message will be timestamped.
         */
        void log(std::string str);

        void log(std::string name, std::string str);

        void loadLast(std::string name, std::list<std::string> &list,
                      const unsigned n) const;

        std::string getDir() const A_WARN_UNUSED;

        static std::string secureName(std::string &str);

        void setServerName(const std::string &serverName);

        void setBaseLogDir(const std::string &logDir)
        { mBaseLogDir = logDir; }

        void clear();

    private:
        /**
         * Sets the file to log to and opens it
         */
        void setLogFile(const std::string &logFilename);

        void setLogDir(const std::string &logDir);

        static void writeTo(std::ofstream &file, const std::string &str);

        std::ofstream mLogFile;
        std::string mLogDir;
        std::string mBaseLogDir;
        std::string mServerName;
        std::string mLogFileName;
};

extern ChatLogger *chatLogger;

#endif  // CHATLOGGER_H
