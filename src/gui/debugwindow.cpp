/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "gui/debugwindow.h"

#include "client.h"
#include "game.h"
#include "localplayer.h"
#include "main.h"
#include "map.h"
#include "particle.h"

#include "gui/setup.h"
#include "gui/setup_video.h"
#include "gui/viewport.h"

#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/chattab.h"

#include "resources/image.h"

#include "net/packetcounters.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

DebugWindow::DebugWindow():
    Window(_("Debug"))
{
    setWindowName("Debug");
    if (setupWindow)
        setupWindow->registerWindowForReset(this);

    mUpdateTime = 0;
    setResizable(true);
    setCloseButton(true);
    setSaveVisible(true);
    setDefaultSize(500, 150, ImageRect::CENTER);

#ifdef USE_OPENGL
    switch (Image::getLoadAsOpenGL())
    {
        case 0:
            mFPSText = _("%d FPS (Software)");
            break;
        case 1:
        default:
            mFPSText = _("%d FPS (fast OpenGL)");
            break;
        case 2:
            mFPSText = _("%d FPS (old OpenGL)");
            break;
    };
#else
    mFPSText = _("%d FPS (Software)");
#endif

    mFPSLabel = new Label(strprintf(_("%d FPS"), 0));
    mMusicFileLabel = new Label(strprintf(_("Music:")));
    mMapLabel = new Label(strprintf(_("Map:")));
    mMinimapLabel = new Label(strprintf(_("Minimap:")));
    mTileMouseLabel = new Label(strprintf(_("Cursor: (%d, %d)"), 0, 0));
    mParticleCountLabel = new Label(strprintf(_("Particle count: %d"), 88888));
    mMapActorCountLabel = new Label(strprintf(
        _("Map actors count: %d"), 88888));

    mPingLabel = new Label("                ");
    mInPackets1Label = new Label("                ");
    mOutPackets1Label = new Label("                ");

    mXYLabel = new Label(strprintf("%s (?,?)", _("Player Position:")));
    mTargetLabel = new Label(strprintf("%s ?", _("Target:")));
    mTargetIdLabel = new Label(strprintf("%s ?     ", _("Target Id:")));
    mTargetLevelLabel = new Label(strprintf("%s ?", _("Target Level:")));
    mTargetPartyLabel = new Label(strprintf("%s ?", _("Target Party:")));
    mTargetGuildLabel = new Label(strprintf("%s ?", _("Target Guild:")));

    place(0, 0, mFPSLabel, 3);
    place(4, 0, mTileMouseLabel, 2);
    place(0, 1, mMusicFileLabel, 3);
    place(4, 1, mParticleCountLabel, 2);
    place(4, 2, mMapActorCountLabel, 2);
    place(0, 2, mMapLabel, 4);
    place(0, 3, mMinimapLabel, 4);
    place(0, 4, mXYLabel, 4);
    place(4, 3, mPingLabel, 2);
    place(4, 4, mInPackets1Label, 2);
    place(4, 5, mOutPackets1Label, 2);
    place(0, 5, mTargetLabel, 4);
    place(0, 6, mTargetIdLabel, 4);
    place(0, 7, mTargetLevelLabel, 4);
    place(0, 8, mTargetPartyLabel, 4);
    place(0, 9, mTargetGuildLabel, 4);

    loadWindowState();
}

void DebugWindow::logic()
{
    if (!isVisible())
        return;

    mFPSLabel->setCaption(strprintf(mFPSText.c_str(), fps));

    if (player_node)
    {
        mXYLabel->setCaption(strprintf("%s (%d, %d)", _("Player Position:"),
            player_node->getTileX(), player_node->getTileY()));
    }
    else
    {
        mXYLabel->setCaption(strprintf("%s (?, ?)", _("Player Position:")));
    }

    if (player_node && player_node->getTarget())
    {
        Being *target = player_node->getTarget();

        mTargetLabel->setCaption(strprintf("%s %s (%d, %d)", _("Target:"),
            target->getName().c_str(), target->getTileX(),
            target->getTileY()));

        mTargetIdLabel->setCaption(strprintf("%s %d",
            _("Target Id:"), target->getId()));
        if (target->getLevel())
        {
            mTargetLevelLabel->setCaption(strprintf("%s %d",
                _("Target Level:"), target->getLevel()));
        }
        else
        {
            mTargetLevelLabel->setCaption(strprintf("%s ?",
                _("Target Level:")));
        }

        mTargetPartyLabel->setCaption(strprintf("%s %s", _("Target Party:"),
            target->getPartyName().c_str()));

        mTargetGuildLabel->setCaption(strprintf("%s %s", _("Target Guild:"),
            target->getGuildName().c_str()));
    }
    else
    {
        mTargetLabel->setCaption(strprintf("%s ?", _("Target:")));
        mTargetIdLabel->setCaption(strprintf("%s ?", _("Target Id:")));
        mTargetLevelLabel->setCaption(strprintf("%s ?", _("Target Level:")));
        mTargetPartyLabel->setCaption(strprintf("%s ?", _("Target Party:")));
        mTargetGuildLabel->setCaption(strprintf("%s ?", _("Target Guild:")));
    }

    const Map *map = Game::instance()->getCurrentMap();
    if (map && viewport)
    {
          // Get the current mouse position
        int mouseTileX = (viewport->getMouseX() + viewport->getCameraX())
                         / map->getTileWidth();
        int mouseTileY = (viewport->getMouseY() + viewport->getCameraY())
                         / map->getTileHeight();
        mTileMouseLabel->setCaption(strprintf("%s (%d, %d)",
            _("Cursor:"), mouseTileX, mouseTileY));

        mMusicFileLabel->setCaption(strprintf("%s %s", _("Music:"),
            map->getProperty("music").c_str()));
        mMinimapLabel->setCaption(strprintf("%s %s", _("Minimap:"),
            map->getProperty("minimap").c_str()));
        mMapLabel->setCaption(strprintf("%s %s", _("Map:"),
            map->getProperty("_realfilename").c_str()));


        if (mUpdateTime != cur_time)
        {
            mUpdateTime = cur_time;
            mParticleCountLabel->setCaption(strprintf(_("Particle count: %d"),
                                            Particle::particleCount));

            mMapActorCountLabel->setCaption(
                strprintf("%s %d", _("Map actors count:"),
                map->getActorsCount()));
        }
    }
    else
    {
        mTileMouseLabel->setCaption(strprintf("%s (?, ?)", _("Cursor:")));

        mMusicFileLabel->setCaption(strprintf("%s ?", _("Music:")));
        mMinimapLabel->setCaption(strprintf("%s ?", _("Minimap:")));
        mMapLabel->setCaption(strprintf("%s ?", _("Map:")));

        mMapActorCountLabel->setCaption(
            strprintf("%s ?", _("Map actors count:")));
    }

    mMapActorCountLabel->adjustSize();
    mParticleCountLabel->adjustSize();

    if (player_node && player_node->getPingTime() != 0)
    {
        mPingLabel->setCaption(strprintf(_("Ping: %d ms"),
            player_node->getPingTime()));
    }
    else
    {
        mPingLabel->setCaption(_("Ping: ? ms"));
    }

    mInPackets1Label->setCaption(strprintf(_("In: %d bytes/s"),
        PacketCounters::getInBytes()));
    mOutPackets1Label->setCaption(strprintf(_("Out: %d bytes/s"),
        PacketCounters::getOutBytes()));

    if (player_node)
        player_node->tryPingRequest();
}

void DebugWindow::draw(gcn::Graphics *g)
{
    Window::draw(g);

    if (player_node)
    {
        Being *target = player_node->getTarget();
        if (target)
        {
            Graphics *g2 = static_cast<Graphics*>(g);
            target->draw(g2, -target->getPixelX() + 16 + getWidth() / 2,
                         -target->getPixelY() + 32 + getHeight() / 2);
        }
    }
}
