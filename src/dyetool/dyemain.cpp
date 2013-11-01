/*
 *  The ManaPlus Client
 *  Copyright (C) 2013  The ManaPlus Developers
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

#include "logger.h"

#include "graphicsmanager.h"
#include "sdlshared.h"

#include "resources/image.h"
#include "resources/imagehelper.h"
#include "resources/imagewriter.h"
#include "resources/resourcemanager.h"
#include "resources/sdlimagehelper.h"

#ifdef USE_SDL2
#include "resources/surfaceimagehelper.h"
#endif

#include "utils/gettext.h"
#include "utils/physfstools.h"

#include <iostream>

#include <SDL.h>

#include "debug.h"

int serverVersion = 0;

static void printHelp()
{
    std::cout << _("dyecmd srcfile dyestring dstfile")
        << std::endl << std::endl;
}

int main(int argc, char **argv)
{
    if (argc != 4)
        printHelp();

    logger = new Logger;
    logger->setLogToStandardOut(false);

    PhysFs::init(argv[0]);
    SDL_Init(SDL_INIT_VIDEO);

    graphicsManager.createWindow(10, 10, 0, SDL_ANYFORMAT);

#ifdef USE_SDL2
    imageHelper = new SurfaceImageHelper;
#else
    imageHelper = new SDLImageHelper;
#endif

    ResourceManager *resman = new ResourceManager;
    resman->setWriteDir(".");
    resman->addToSearchPath(".", false);
    resman->addToSearchPath("/", false);
    Image *image = resman->getImage(std::string(
        argv[1]).append("|").append(argv[2]));
    if (!image)
    {
        printf("Error loading image\n");
        return 1;
    }
    ImageWriter::writePNG(image->getSDLSurface(), argv[3]);
    return 0;
}
