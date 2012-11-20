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

#include "gui/debugwindow.h"

#include "client.h"
#include "game.h"
#include "localplayer.h"
#include "main.h"
#include "particle.h"

#include "gui/setup.h"
#include "gui/setup_video.h"
#include "gui/viewport.h"

#include "gui/widgets/chattab.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"

#include "resources/imagehelper.h"

#include "net/packetcounters.h"

#include "utils/gettext.h"

#include "debug.h"

DebugWindow::DebugWindow():
    Window(_("Debug"), false, nullptr, "debug.xml"),
    mTabs(new TabbedArea(this)),
    mMapWidget(new MapDebugTab(this)),
    mTargetWidget(new TargetDebugTab(this)),
    mNetWidget(new NetDebugTab(this))
{
    setWindowName("Debug");
    if (setupWindow)
        setupWindow->registerWindowForReset(this);

    setResizable(true);
    setCloseButton(true);
    setSaveVisible(true);
    setStickyButtonLock(true);

    setDefaultSize(400, 300, ImageRect::CENTER);

    mTabs->addTab(std::string(_("Map")), mMapWidget);
    mTabs->addTab(std::string(_("Target")), mTargetWidget);
    mTabs->addTab(std::string(_("Net")), mNetWidget);

    mTabs->setDimension(gcn::Rectangle(0, 0, 600, 300));
    add(mTabs);

    mMapWidget->resize(getWidth(), getHeight());
    mTargetWidget->resize(getWidth(), getHeight());
    mNetWidget->resize(getWidth(), getHeight());
    loadWindowState();
    enableVisibleSound(true);
}

DebugWindow::~DebugWindow()
{
    delete mMapWidget;
    mMapWidget = nullptr;
    delete mTargetWidget;
    mTargetWidget = nullptr;
    delete mNetWidget;
    mNetWidget = nullptr;
}

void DebugWindow::slowLogic()
{
    BLOCK_START("DebugWindow::slowLogic")
    if (!isVisible() || !mTabs)
    {
        BLOCK_END("DebugWindow::slowLogic")
        return;
    }

    switch (mTabs->getSelectedTabIndex())
    {
        default:
        case 0:
            mMapWidget->logic();
            break;
        case 1:
            mTargetWidget->logic();
            break;
        case 2:
            mNetWidget->logic();
            break;
    }

    if (player_node)
        player_node->tryPingRequest();
    BLOCK_END("DebugWindow::slowLogic")
}

void DebugWindow::draw(gcn::Graphics *g)
{
    BLOCK_START("DebugWindow::draw")
    Window::draw(g);

    if (player_node)
    {
        const Being *const target = player_node->getTarget();
        if (target)
        {
            Graphics *const g2 = static_cast<Graphics*>(g);
            target->draw(g2, -target->getPixelX() + 16 + getWidth() / 2,
                         -target->getPixelY() + 32 + getHeight() / 2);
        }
    }
    BLOCK_END("DebugWindow::draw")
}

void DebugWindow::widgetResized(const gcn::Event &event)
{
    Window::widgetResized(event);

    mTabs->setDimension(gcn::Rectangle(0, 0, getWidth(), getHeight()));
}

MapDebugTab::MapDebugTab(const Widget2 *const widget) :
    DebugTab(widget),
    mMusicFileLabel(new Label(this, strprintf(_("Music:")))),
    mMapLabel(new Label(this, strprintf(_("Map:")))),
    mMinimapLabel(new Label(this, strprintf(_("Minimap:")))),
    mTileMouseLabel(new Label(this, strprintf("%s (%d, %d)",
        _("Cursor:"), 0, 0))),
    mParticleCountLabel(new Label(this, strprintf("%s %d",
        _("Particle count:"), 88888))),
    mMapActorCountLabel(new Label(this, strprintf("%s %d",
        _("Map actors count:"), 88888))),
    mXYLabel(new Label(this, strprintf("%s (?,?)", _("Player Position:")))),
    mTexturesLabel(nullptr),
    mUpdateTime(0),
#ifdef DEBUG_DRAW_CALLS
    mDrawCallsLabel(new Label(this, strprintf("%s %s",
        _("Draw calls:"), "?"))),
#endif
    mFPSLabel(new Label(this, strprintf(_("%d FPS"), 0))),
    mLPSLabel(new Label(this, strprintf(_("%d LPS"), 0)))
{
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

#ifdef USE_OPENGL
    switch (imageHelper->useOpenGL())
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
        case 3:
            mFPSText = _("%d FPS (mobile OpenGL)");
            break;
    };
#else
    mFPSText = _("%d FPS (Software)");
#endif

    place(0, 0, mFPSLabel, 2);
    place(0, 1, mLPSLabel, 2);
    place(0, 2, mMusicFileLabel, 2);
    place(0, 3, mMapLabel, 2);
    place(0, 4, mMinimapLabel, 2);
    place(0, 5, mXYLabel, 2);
    place(0, 6, mTileMouseLabel, 2);
    place(0, 7, mParticleCountLabel, 2);
    place(0, 8, mMapActorCountLabel, 2);
#ifdef USE_OPENGL
    int n = 9;
#ifdef DEBUG_OPENGL_LEAKS
    mTexturesLabel = new Label(this, strprintf("%s %s",
        _("Textures count:"), "?"));
    place(0, n, mTexturesLabel, 2);
    n ++;
#endif
#ifdef DEBUG_DRAW_CALLS
    place(0, n, mDrawCallsLabel, 2);
#endif
#endif
    place.getCell().matchColWidth(0, 0);
    place = h.getPlacer(0, 1);
    setDimension(gcn::Rectangle(0, 0, 600, 300));
}

void MapDebugTab::logic()
{
    if (player_node)
    {
        mXYLabel->setCaption(strprintf("%s (%d, %d)", _("Player Position:"),
            player_node->getTileX(), player_node->getTileY()));
    }
    else
    {
        mXYLabel->setCaption(strprintf("%s (?, ?)", _("Player Position:")));
    }

    const Map *const map = Game::instance()->getCurrentMap();
    if (map && viewport)
    {
          // Get the current mouse position
        const int mouseTileX = (viewport->getMouseX() + viewport->getCameraX())
                         / map->getTileWidth();
        const int mouseTileY = (viewport->getMouseY() + viewport->getCameraY())
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
#ifdef USE_OPENGL
#ifdef DEBUG_OPENGL_LEAKS
            mTexturesLabel->setCaption(strprintf("%s %d",
                _("Textures count:"), textures_count));
#endif
#ifdef DEBUG_DRAW_CALLS
            if (mainGraphics)
            {
                mDrawCallsLabel->setCaption(strprintf("%s %d",
                    _("Draw calls:"), mainGraphics->getDrawCalls()));
            }
#endif
#endif
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

    mFPSLabel->setCaption(strprintf(mFPSText.c_str(), fps));
    mLPSLabel->setCaption(strprintf(_("%d LPS"), lps));
}

TargetDebugTab::TargetDebugTab(const Widget2 *const widget) :
    DebugTab(widget),
    mTargetLabel(new Label(this, strprintf("%s ?", _("Target:")))),
    mTargetIdLabel(new Label(this, strprintf("%s ?     ", _("Target Id:")))),
    mTargetLevelLabel(new Label(this, strprintf("%s ?", _("Target level:")))),
    mTargetRaceLabel(new Label(this, strprintf("%s ?", _("Target race:")))),
    mTargetPartyLabel(new Label(this, strprintf("%s ?", _("Target party:")))),
    mTargetGuildLabel(new Label(this, strprintf("%s ?", _("Target guild:")))),
    mAttackDelayLabel(new Label(this, strprintf("%s ?", _("Attack delay:")))),
    mMinHitLabel(new Label(this, strprintf("%s ?", _("Minimal hit:")))),
    mMaxHitLabel(new Label(this, strprintf("%s ?", _("Maximum hit:")))),
    mCriticalHitLabel(new Label(this, strprintf("%s ?", _("Critical hit:"))))
{
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mTargetLabel, 2);
    place(0, 1, mTargetIdLabel, 2);
    place(0, 2, mTargetLevelLabel, 2);
    place(0, 3, mTargetRaceLabel, 2);
    place(0, 4, mAttackDelayLabel, 2);
    place(0, 5, mTargetPartyLabel, 2);
    place(0, 6, mTargetGuildLabel, 2);
    place(0, 7, mMinHitLabel, 2);
    place(0, 8, mMaxHitLabel, 2);
    place(0, 9, mCriticalHitLabel, 2);

    place.getCell().matchColWidth(0, 0);
    place = h.getPlacer(0, 1);
    setDimension(gcn::Rectangle(0, 0, 600, 300));
}

void TargetDebugTab::logic()
{
    if (player_node && player_node->getTarget())
    {
        const Being *const target = player_node->getTarget();

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

        mTargetRaceLabel->setCaption(strprintf("%s %s",
            _("Target race:"), target->getRaceName().c_str()));

        mTargetPartyLabel->setCaption(strprintf("%s %s", _("Target Party:"),
            target->getPartyName().c_str()));

        mTargetGuildLabel->setCaption(strprintf("%s %s", _("Target Guild:"),
            target->getGuildName().c_str()));

        mMinHitLabel->setCaption(strprintf("%s %d",
            _("Minimal hit:"), target->getMinHit()));
        mMaxHitLabel->setCaption(strprintf("%s %d",
            _("Maximum hit:"), target->getMaxHit()));
        mCriticalHitLabel->setCaption(strprintf("%s %d",
            _("Critical hit:"), target->getCriticalHit()));

        const int delay = target->getAttackDelay();
        if (delay)
        {
            mAttackDelayLabel->setCaption(strprintf("%s %d",
                _("Attack delay:"), delay));
        }
        else
        {
            mAttackDelayLabel->setCaption(strprintf(
                "%s ?", _("Attack delay:")));
        }
    }
    else
    {
        mTargetLabel->setCaption(strprintf("%s ?", _("Target:")));
        mTargetIdLabel->setCaption(strprintf("%s ?", _("Target Id:")));
        mTargetLevelLabel->setCaption(strprintf("%s ?", _("Target Level:")));
        mTargetPartyLabel->setCaption(strprintf("%s ?", _("Target Party:")));
        mTargetGuildLabel->setCaption(strprintf("%s ?", _("Target Guild:")));
        mAttackDelayLabel->setCaption(strprintf("%s ?", _("Attack delay:")));
        mMinHitLabel->setCaption(strprintf("%s ?", _("Minimal hit:")));
        mMaxHitLabel->setCaption(strprintf("%s ?", _("Maximum hit:")));
        mCriticalHitLabel->setCaption(strprintf("%s ?", _("Critical hit:")));
    }

    mTargetLabel->adjustSize();
    mTargetIdLabel->adjustSize();
    mTargetLevelLabel->adjustSize();
    mTargetPartyLabel->adjustSize();
    mTargetGuildLabel->adjustSize();
    mAttackDelayLabel->adjustSize();
}

NetDebugTab::NetDebugTab(const Widget2 *const widget) :
    DebugTab(widget),
    mPingLabel(new Label(this, "                ")),
    mInPackets1Label(new Label(this, "                ")),
    mOutPackets1Label(new Label(this, "                "))
{
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mPingLabel, 2);
    place(0, 1, mInPackets1Label, 2);
    place(0, 2, mOutPackets1Label, 2);

    place.getCell().matchColWidth(0, 0);
    place = h.getPlacer(0, 1);
    setDimension(gcn::Rectangle(0, 0, 600, 300));
}

void NetDebugTab::logic()
{
    if (player_node && player_node->getPingTime() != 0)
    {
        mPingLabel->setCaption(strprintf(_("Ping: %s ms"),
            toString(static_cast<int>(player_node->getPingTime())).c_str()));
    }
    else
    {
        mPingLabel->setCaption(strprintf(_("Ping: %s ms"), "?"));
    }

    mInPackets1Label->setCaption(strprintf(_("In: %d bytes/s"),
        PacketCounters::getInBytes()));
    mOutPackets1Label->setCaption(strprintf(_("Out: %d bytes/s"),
        PacketCounters::getOutBytes()));
}
