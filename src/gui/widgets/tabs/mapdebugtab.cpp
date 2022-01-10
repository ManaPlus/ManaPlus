/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "gui/widgets/tabs/mapdebugtab.h"

#include "game.h"

#include "being/localplayer.h"

#include "particle/particleengine.h"

#include "gui/viewport.h"

#include "gui/widgets/containerplacer.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"

#ifdef USE_OPENGL
#include "resources/imagehelper.h"
#endif  // USE_OPENGL

#include "resources/map/map.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"
#include "utils/timer.h"

#include "debug.h"

MapDebugTab::MapDebugTab(const Widget2 *const widget) :
    DebugTab(widget),
    // TRANSLATORS: debug window label
    mMusicFileLabel(new Label(this, _("Music:"))),
    // TRANSLATORS: debug window label
    mMapLabel(new Label(this, _("Map:"))),
    // TRANSLATORS: debug window label
    mMapNameLabel(new Label(this, _("Map name:"))),
    // TRANSLATORS: debug window label
    mMinimapLabel(new Label(this, _("Minimap:"))),
    mTileMouseLabel(new Label(this, strprintf("%s (%d, %d)",
        // TRANSLATORS: debug window label
        _("Cursor:"), 0, 0))),
    mParticleCountLabel(new Label(this, strprintf("%s %d",
        // TRANSLATORS: debug window label
        _("Particle count:"), 88888))),
    mMapActorCountLabel(new Label(this, strprintf("%s %d",
        // TRANSLATORS: debug window label
        _("Map actors count:"), 88888))),
#ifdef USE_OPENGL
    mMapAtlasCountLabel(new Label(this, strprintf("%s %d",
        // TRANSLATORS: debug window label
        _("Map atlas count:"), 88888))),
#endif  // USE_OPENGL
    // TRANSLATORS: debug window label
    mXYLabel(new Label(this, strprintf("%s (?,?)", _("Player Position:")))),
    mTexturesLabel(nullptr),
    mUpdateTime(0),
#ifdef DEBUG_DRAW_CALLS
    mDrawCallsLabel(new Label(this, strprintf("%s %s",
        // TRANSLATORS: debug window label
        _("Draw calls:"), "?"))),
#endif  // DEBUG_DRAW_CALLS
#ifdef DEBUG_BIND_TEXTURE
    mBindsLabel(new Label(this, strprintf("%s %s",
        // TRANSLATORS: debug window label
        _("Texture binds:"), "?"))),
#endif  // DEBUG_BIND_TEXTURE
    // TRANSLATORS: debug window label, frames per second
    mFPSLabel(new Label(this, strprintf(_("%d FPS"), 0))),
    mFPSText()
{
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

#ifdef USE_OPENGL
    switch (imageHelper->useOpenGL())
    {
        case RENDER_SOFTWARE:
            // TRANSLATORS: debug window label
            mFPSText = _("%d FPS (Software)");
            break;
        case RENDER_NORMAL_OPENGL:
        case RENDER_NULL:
        case RENDER_LAST:
        default:
            // TRANSLATORS: debug window label
            mFPSText = _("%d FPS (normal OpenGL)");
            break;
        case RENDER_SAFE_OPENGL:
            // TRANSLATORS: debug window label
            mFPSText = _("%d FPS (safe OpenGL)");
            break;
        case RENDER_GLES_OPENGL:
            // TRANSLATORS: debug window label
            mFPSText = _("%d FPS (mobile OpenGL ES)");
            break;
        case RENDER_GLES2_OPENGL:
            // TRANSLATORS: debug window label
            mFPSText = _("%d FPS (mobile OpenGL ES 2)");
            break;
        case RENDER_MODERN_OPENGL:
            // TRANSLATORS: debug window label
            mFPSText = _("%d FPS (modern OpenGL)");
            break;
        case RENDER_SDL2_DEFAULT:
            // TRANSLATORS: debug window label
            mFPSText = _("%d FPS (SDL2 default)");
            break;
    }
#else  // USE_OPENGL

    // TRANSLATORS: debug window label
    mFPSText = _("%d FPS (Software)");
#endif  // USE_OPENGL

    place(0, 0, mFPSLabel, 2, 1);
    place(0, 1, mMusicFileLabel, 2, 1);
    place(0, 2, mMapLabel, 2, 1);
    place(0, 3, mMapNameLabel, 2, 1);
    place(0, 4, mMinimapLabel, 2, 1);
    place(0, 5, mXYLabel, 2, 1);
    place(0, 6, mTileMouseLabel, 2, 1);
    place(0, 7, mParticleCountLabel, 2, 1);
    place(0, 8, mMapActorCountLabel, 2, 1);
#ifdef USE_OPENGL
    place(0, 9, mMapAtlasCountLabel, 2, 1);
#if defined (DEBUG_OPENGL_LEAKS) || defined(DEBUG_DRAW_CALLS) \
    || defined(DEBUG_BIND_TEXTURE)
    int n = 10;
#endif  // defined (DEBUG_OPENGL_LEAKS) || defined(DEBUG_DRAW_CALLS)
        // || defined(DEBUG_BIND_TEXTURE)
#ifdef DEBUG_OPENGL_LEAKS
    mTexturesLabel = new Label(this, strprintf("%s %s",
        // TRANSLATORS: debug window label
        _("Textures count:"), "?"));
    place(0, n, mTexturesLabel, 2, 1);
    n ++;
#endif  // DEBUG_OPENGL_LEAKS
#ifdef DEBUG_DRAW_CALLS
    place(0, n, mDrawCallsLabel, 2, 1);
    n ++;
#endif  // DEBUG_DRAW_CALLS
#ifdef DEBUG_BIND_TEXTURE
    place(0, n, mBindsLabel, 2, 1);
#endif  // DEBUG_BIND_TEXTURE
#endif  // USE_OPENGL

    place.getCell().matchColWidth(0, 0);
    place = h.getPlacer(0, 1);
    setDimension(Rect(0, 0, 600, 300));
}

void MapDebugTab::logic()
{
    BLOCK_START("MapDebugTab::logic")
    if (localPlayer != nullptr)
    {
        // TRANSLATORS: debug window label
        mXYLabel->setCaption(strprintf("%s (%d, %d)", _("Player Position:"),
            localPlayer->getTileX(), localPlayer->getTileY()));
    }
    else
    {
        // TRANSLATORS: debug window label
        mXYLabel->setCaption(strprintf("%s (?, ?)", _("Player Position:")));
    }

    Game *const game = Game::instance();
    const Map *const map = game != nullptr ? game->getCurrentMap() : nullptr;
    if (map != nullptr &&
        viewport != nullptr)
    {
          // Get the current mouse position
        const int mouseTileX = (viewport->mMouseX + viewport->getCameraX())
                         / map->getTileWidth();
        const int mouseTileY = (viewport->mMouseY + viewport->getCameraY())
                         / map->getTileHeight();
        mTileMouseLabel->setCaption(strprintf("%s (%d, %d)",
            // TRANSLATORS: debug window label
            _("Cursor:"), mouseTileX, mouseTileY));

        // TRANSLATORS: debug window label
        mMusicFileLabel->setCaption(strprintf("%s %s", _("Music:"),
            map->getProperty("music", std::string()).c_str()));
        // TRANSLATORS: debug window label
        mMinimapLabel->setCaption(strprintf("%s %s", _("Minimap:"),
            map->getProperty("minimap", std::string()).c_str()));
        // TRANSLATORS: debug window label
        mMapLabel->setCaption(strprintf("%s %s", _("Map:"),
            map->getProperty("_realfilename", std::string()).c_str()));
        // TRANSLATORS: debug window label
        mMapNameLabel->setCaption(strprintf("%s %s", _("Map name:"),
            map->getProperty("name", std::string()).c_str()));

        if (mUpdateTime != cur_time)
        {
            mUpdateTime = cur_time;
            // TRANSLATORS: debug window label
            mParticleCountLabel->setCaption(strprintf(_("Particle count: %d"),
                ParticleEngine::particleCount));

            mMapActorCountLabel->setCaption(
                // TRANSLATORS: debug window label
                strprintf("%s %d", _("Map actors count:"),
                map->getActorsCount()));
#ifdef USE_OPENGL
            mMapAtlasCountLabel->setCaption(
                // TRANSLATORS: debug window label
                strprintf("%s %d", _("Map atlas count:"),
                map->getAtlasCount()));
#ifdef DEBUG_OPENGL_LEAKS
            mTexturesLabel->setCaption(strprintf("%s %d",
                // TRANSLATORS: debug window label
                _("Textures count:"), textures_count));
#endif  // DEBUG_OPENGL_LEAKS
#ifdef DEBUG_DRAW_CALLS
            if (mainGraphics)
            {
                mDrawCallsLabel->setCaption(strprintf("%s %d",
                    // TRANSLATORS: debug window label
                    _("Draw calls:"), mainGraphics->getDrawCalls()));
            }
#endif  // DEBUG_DRAW_CALLS
#ifdef DEBUG_BIND_TEXTURE
            if (mainGraphics)
            {
                mBindsLabel->setCaption(strprintf("%s %d",
                    // TRANSLATORS: debug window label
                    _("Texture binds:"), mainGraphics->getBinds()));
            }
#endif  // DEBUG_BIND_TEXTURE
#endif  // USE_OPENGL
        }
    }
    else
    {
        // TRANSLATORS: debug window label
        mTileMouseLabel->setCaption(strprintf("%s (?, ?)", _("Cursor:")));
        // TRANSLATORS: debug window label
        mMusicFileLabel->setCaption(strprintf("%s ?", _("Music:")));
        // TRANSLATORS: debug window label
        mMinimapLabel->setCaption(strprintf("%s ?", _("Minimap:")));
        // TRANSLATORS: debug window label
        mMapLabel->setCaption(strprintf("%s ?", _("Map:")));
        // TRANSLATORS: debug window label
        mMapNameLabel->setCaption(strprintf("%s ?", _("Map name:")));
        mMapActorCountLabel->setCaption(
            // TRANSLATORS: debug window label
            strprintf("%s ?", _("Map actors count:")));
#ifdef USE_OPENGL
        mMapAtlasCountLabel->setCaption(
            // TRANSLATORS: debug window label
            strprintf("%s ?", _("Map atlas count:")));
#endif  // USE_OPENGL
    }

    mMapActorCountLabel->adjustSize();
    mParticleCountLabel->adjustSize();
#ifdef USE_OPENGL
    mMapAtlasCountLabel->adjustSize();
#endif  // USE_OPENGL

    mFPSLabel->setCaption(strprintf(mFPSText.c_str(), fps));
    BLOCK_END("MapDebugTab::logic")
}
