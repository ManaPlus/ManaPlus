/*
 *  The ManaPlus Client
 *  Copyright (C) 2012  The ManaPlus Developers
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

#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include <set>
#include <string>

class Graphics;

class GraphicsManager
{
    public:
        GraphicsManager();

        virtual ~GraphicsManager();

        void initGraphics(bool noOpenGL);

        void updateExtensions(const char *extensions);

        bool supportExtension(const std::string ext);

        void updateTextureFormat();

        void logString(const char *format, int num);

        void setVideoMode();

    private:
        std::set<std::string> mExtensions;
};

extern GraphicsManager graphicsManager;

#endif
