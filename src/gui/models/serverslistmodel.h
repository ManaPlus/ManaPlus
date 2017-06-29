/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef GUI_MODELS_SERVERSLISTMODEL_H
#define GUI_MODELS_SERVERSLISTMODEL_H

#include "gui/gui.h"

#include "gui/fonts/font.h"

#include "gui/windows/serverdialog.h"

#include "gui/models/listmodel.h"

#include "utils/cast.h"
#include "utils/gettext.h"

/**
 * Server and Port List Model
 */
class ServersListModel final : public ListModel
{
    public:
        typedef std::pair<int, std::string> VersionString;

        ServersListModel(ServerInfos *const servers,
                         ServerDialog *const parent) :
            mServers(servers),
            mVersionStrings(servers != nullptr ? servers->size() : 0,
                VersionString(0, "")),
            mParent(parent)
        {
        }

        A_DELETE_COPY(ServersListModel)

        /**
         * Used to get number of line in the list
         */
        int getNumberOfElements() override final A_WARN_UNUSED
        {
            MutexLocker lock = mParent->lock();
            return CAST_S32(mServers->size());
        }

        /**
         * Used to get an element from the list
         */
        std::string getElementAt(int elementIndex)
                                 override final A_WARN_UNUSED
        {
            MutexLocker lock = mParent->lock();
            const ServerInfo &server = mServers->at(elementIndex);
            std::string myServer;
            if (server.freeType == ServerFreeType::NonFree)
            {
                // TRANSLATORS: server license comment
                myServer.append(_("(NON FREE)"));
                myServer.append(" ");
            }
            else if (server.freeType == ServerFreeType::Unknown)
            {
                // TRANSLATORS: server license comment
                myServer.append(_("(UNKNOWN)"));
                myServer.append(" ");
            }
            myServer.append(server.hostname);
            return myServer;
        }

        /**
         * Used to get the corresponding Server struct
         */
        const ServerInfo &getServer(const int elementIndex) const A_WARN_UNUSED
        { return mServers->at(elementIndex); }

        void setVersionString(const int index, const std::string &version)
        {
            if (index < 0 || index >= CAST_S32(mVersionStrings.size()))
            return;

            if (version.empty() || (gui == nullptr))
            {
                mVersionStrings[index] = VersionString(0, "");
            }
            else
            {
                mVersionStrings[index] = VersionString(
                    gui->getFont()->getWidth(version), version);
            }
        }

    private:
        typedef std::vector<VersionString> VersionStrings;

        ServerInfos *mServers;
        VersionStrings mVersionStrings;
        ServerDialog *mParent;
};

#endif  // GUI_MODELS_SERVERSLISTMODEL_H
