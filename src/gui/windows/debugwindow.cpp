/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "gui/windows/debugwindow.h"

#include "game.h"
#include "main.h"

#include "being/localplayer.h"

#include "particle/particle.h"

#include "gui/viewport.h"

#include "gui/windows/setupwindow.h"

#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/tabbedarea.h"

#include "resources/imagehelper.h"

#include "net/packetcounters.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"
#include "utils/timer.h"

#include "debug.h"

DebugWindow::DebugWindow() :
    // TRANSLATORS: debug window name
    Window(_("Debug"), false, nullptr, "debug.xml"),
    mTabs(new TabbedArea(this)),
    mMapWidget(new MapDebugTab(this)),
    mTargetWidget(new TargetDebugTab(this)),
    mNetWidget(new NetDebugTab(this))
{
    mTabs->postInit();
    setWindowName("Debug");
    if (setupWindow)
        setupWindow->registerWindowForReset(this);

    setResizable(true);
    setCloseButton(true);
    setSaveVisible(true);
    setStickyButtonLock(true);

    setDefaultSize(400, 300, ImageRect::CENTER);

    // TRANSLATORS: debug window tab
    mTabs->addTab(std::string(_("Map")), mMapWidget);
    // TRANSLATORS: debug window tab
    mTabs->addTab(std::string(_("Target")), mTargetWidget);
    // TRANSLATORS: debug window tab
    mTabs->addTab(std::string(_("Net")), mNetWidget);

    mTabs->setDimension(Rect(0, 0, 600, 300));

    const int w = mDimension.width;
    const int h = mDimension.height;
    mMapWidget->resize(w, h);
    mTargetWidget->resize(w, h);
    mNetWidget->resize(w, h);
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

void DebugWindow::postInit()
{
    add(mTabs);
}

void DebugWindow::slowLogic()
{
    BLOCK_START("DebugWindow::slowLogic")
    if (!isWindowVisible() || !mTabs)
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

void DebugWindow::draw(Graphics *g)
{
    BLOCK_START("DebugWindow::draw")
    Window::draw(g);

    if (player_node)
    {
        const Being *const target = player_node->getTarget();
        if (target)
        {
            target->draw(g, -target->getPixelX() + mapTileSize / 2
                + mDimension.width / 2, -target->getPixelY() + mapTileSize
                + mDimension.height / 2);
        }
    }
    BLOCK_END("DebugWindow::draw")
}

void DebugWindow::widgetResized(const Event &event)
{
    Window::widgetResized(event);

    mTabs->setDimension(Rect(0, 0,
        mDimension.width, mDimension.height));
}

#ifdef USE_PROFILER
void DebugWindow::logicChildren()
{
    BLOCK_START("DebugWindow::logicChildren")
    BasicContainer::logicChildren();
    BLOCK_END("DebugWindow::logicChildren")
}
#endif

MapDebugTab::MapDebugTab(const Widget2 *const widget) :
    DebugTab(widget),
    // TRANSLATORS: debug window label
    mMusicFileLabel(new Label(this, strprintf(_("Music:")))),
    // TRANSLATORS: debug window label
    mMapLabel(new Label(this, strprintf(_("Map:")))),
    // TRANSLATORS: debug window label
    mMinimapLabel(new Label(this, strprintf(_("Minimap:")))),
    mTileMouseLabel(new Label(this, strprintf("%s (%d, %d)",
        // TRANSLATORS: debug window label
        _("Cursor:"), 0, 0))),
    mParticleCountLabel(new Label(this, strprintf("%s %d",
        // TRANSLATORS: debug window label
        _("Particle count:"), 88888))),
    mMapActorCountLabel(new Label(this, strprintf("%s %d",
        // TRANSLATORS: debug window label
        _("Map actors count:"), 88888))),
    // TRANSLATORS: debug window label
    mXYLabel(new Label(this, strprintf("%s (?,?)", _("Player Position:")))),
    mTexturesLabel(nullptr),
    mUpdateTime(0),
#ifdef DEBUG_DRAW_CALLS
    mDrawCallsLabel(new Label(this, strprintf("%s %s",
        // TRANSLATORS: debug window label
        _("Draw calls:"), "?"))),
#endif
#ifdef DEBUG_BIND_TEXTURE
    mBindsLabel(new Label(this, strprintf("%s %s",
        // TRANSLATORS: debug window label
        _("Texture binds:"), "?"))),
#endif
    // TRANSLATORS: debug window label, frames per second
    mFPSLabel(new Label(this, strprintf(_("%d FPS"), 0))),
    // TRANSLATORS: debug window label, logic per second
    mLPSLabel(new Label(this, strprintf(_("%d LPS"), 0))),
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
            mFPSText = _("%d FPS (mobile OpenGL)");
            break;
        case RENDER_SDL2_DEFAULT:
            // TRANSLATORS: debug window label
            mFPSText = _("%d FPS (SDL2 default)");
            break;
    };
#else
    // TRANSLATORS: debug window label
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
#if defined (DEBUG_OPENGL_LEAKS) || defined(DEBUG_DRAW_CALLS) \
    || defined(DEBUG_BIND_TEXTURE)
    int n = 9;
#endif
#ifdef DEBUG_OPENGL_LEAKS
    mTexturesLabel = new Label(this, strprintf("%s %s",
        // TRANSLATORS: debug window label
        _("Textures count:"), "?"));
    place(0, n, mTexturesLabel, 2);
    n ++;
#endif
#ifdef DEBUG_DRAW_CALLS
    place(0, n, mDrawCallsLabel, 2);
    n ++;
#endif
#ifdef DEBUG_BIND_TEXTURE
    place(0, n, mBindsLabel, 2);
#endif
#endif
    place.getCell().matchColWidth(0, 0);
    place = h.getPlacer(0, 1);
    setDimension(Rect(0, 0, 600, 300));
}

void MapDebugTab::logic()
{
    BLOCK_START("MapDebugTab::logic")
    if (player_node)
    {
        // TRANSLATORS: debug window label
        mXYLabel->setCaption(strprintf("%s (%d, %d)", _("Player Position:"),
            player_node->getTileX(), player_node->getTileY()));
    }
    else
    {
        // TRANSLATORS: debug window label
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
            // TRANSLATORS: debug window label
            _("Cursor:"), mouseTileX, mouseTileY));

        // TRANSLATORS: debug window label
        mMusicFileLabel->setCaption(strprintf("%s %s", _("Music:"),
            map->getProperty("music").c_str()));
        // TRANSLATORS: debug window label
        mMinimapLabel->setCaption(strprintf("%s %s", _("Minimap:"),
            map->getProperty("minimap").c_str()));
        // TRANSLATORS: debug window label
        mMapLabel->setCaption(strprintf("%s %s", _("Map:"),
            map->getProperty("_realfilename").c_str()));


        if (mUpdateTime != cur_time)
        {
            mUpdateTime = cur_time;
            // TRANSLATORS: debug window label
            mParticleCountLabel->setCaption(strprintf(_("Particle count: %d"),
                Particle::particleCount));

            mMapActorCountLabel->setCaption(
                // TRANSLATORS: debug window label
                strprintf("%s %d", _("Map actors count:"),
                map->getActorsCount()));
#ifdef USE_OPENGL
#ifdef DEBUG_OPENGL_LEAKS
            mTexturesLabel->setCaption(strprintf("%s %d",
                // TRANSLATORS: debug window label
                _("Textures count:"), textures_count));
#endif
#ifdef DEBUG_DRAW_CALLS
            if (mainGraphics)
            {
                mDrawCallsLabel->setCaption(strprintf("%s %d",
                    // TRANSLATORS: debug window label
                    _("Draw calls:"), mainGraphics->getDrawCalls()));
            }
#endif
#ifdef DEBUG_BIND_TEXTURE
            if (mainGraphics)
            {
                mBindsLabel->setCaption(strprintf("%s %d",
                    // TRANSLATORS: debug window label
                    _("Texture binds:"), mainGraphics->getBinds()));
            }
#endif
#endif
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

        mMapActorCountLabel->setCaption(
            // TRANSLATORS: debug window label
            strprintf("%s ?", _("Map actors count:")));
    }

    mMapActorCountLabel->adjustSize();
    mParticleCountLabel->adjustSize();

    mFPSLabel->setCaption(strprintf(mFPSText.c_str(), fps));
    // TRANSLATORS: debug window label, logic per second
    mLPSLabel->setCaption(strprintf(_("%d LPS"), lps));
    BLOCK_END("MapDebugTab::logic")
}

TargetDebugTab::TargetDebugTab(const Widget2 *const widget) :
    DebugTab(widget),
    // TRANSLATORS: debug window label
    mTargetLabel(new Label(this, strprintf("%s ?", _("Target:")))),
    // TRANSLATORS: debug window label
    mTargetIdLabel(new Label(this, strprintf("%s ?     ", _("Target Id:")))),
    mTargetTypeLabel(new Label(this, strprintf(
        // TRANSLATORS: debug window label
        "%s ?     ", _("Target type:")))),
    // TRANSLATORS: debug window label
    mTargetLevelLabel(new Label(this, strprintf("%s ?", _("Target level:")))),
    // TRANSLATORS: debug window label
    mTargetRaceLabel(new Label(this, strprintf("%s ?", _("Target race:")))),
    // TRANSLATORS: debug window label
    mTargetPartyLabel(new Label(this, strprintf("%s ?", _("Target party:")))),
    // TRANSLATORS: debug window label
    mTargetGuildLabel(new Label(this, strprintf("%s ?", _("Target guild:")))),
    // TRANSLATORS: debug window label
    mAttackDelayLabel(new Label(this, strprintf("%s ?", _("Attack delay:")))),
    // TRANSLATORS: debug window label
    mMinHitLabel(new Label(this, strprintf("%s ?", _("Minimal hit:")))),
    // TRANSLATORS: debug window label
    mMaxHitLabel(new Label(this, strprintf("%s ?", _("Maximum hit:")))),
    // TRANSLATORS: debug window label
    mCriticalHitLabel(new Label(this, strprintf("%s ?", _("Critical hit:"))))
{
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mTargetLabel, 2);
    place(0, 1, mTargetIdLabel, 2);
    place(0, 2, mTargetTypeLabel, 2);
    place(0, 3, mTargetLevelLabel, 2);
    place(0, 4, mTargetRaceLabel, 2);
    place(0, 5, mAttackDelayLabel, 2);
    place(0, 6, mTargetPartyLabel, 2);
    place(0, 7, mTargetGuildLabel, 2);
    place(0, 8, mMinHitLabel, 2);
    place(0, 9, mMaxHitLabel, 2);
    place(0, 10, mCriticalHitLabel, 2);

    place.getCell().matchColWidth(0, 0);
    place = h.getPlacer(0, 1);
    setDimension(Rect(0, 0, 600, 300));
}

void TargetDebugTab::logic()
{
    BLOCK_START("TargetDebugTab::logic")
    if (player_node && player_node->getTarget())
    {
        const Being *const target = player_node->getTarget();

        // TRANSLATORS: debug window label
        mTargetLabel->setCaption(strprintf("%s %s (%d, %d)", _("Target:"),
            target->getName().c_str(), target->getTileX(),
            target->getTileY()));

        mTargetIdLabel->setCaption(strprintf("%s %d",
            // TRANSLATORS: debug window label
            _("Target Id:"), target->getId()));
        mTargetTypeLabel->setCaption(strprintf("%s %d",
            // TRANSLATORS: debug window label
            _("Target type:"), target->getSubType()));
        if (target->getLevel())
        {
            mTargetLevelLabel->setCaption(strprintf("%s %d",
                // TRANSLATORS: debug window label
                _("Target Level:"), target->getLevel()));
        }
        else
        {
            mTargetLevelLabel->setCaption(strprintf("%s ?",
                // TRANSLATORS: debug window label
                _("Target Level:")));
        }

        mTargetRaceLabel->setCaption(strprintf("%s %s",
            // TRANSLATORS: debug window label
            _("Target race:"), target->getRaceName().c_str()));

        // TRANSLATORS: debug window label
        mTargetPartyLabel->setCaption(strprintf("%s %s", _("Target Party:"),
            target->getPartyName().c_str()));

        // TRANSLATORS: debug window label
        mTargetGuildLabel->setCaption(strprintf("%s %s", _("Target Guild:"),
            target->getGuildName().c_str()));

        mMinHitLabel->setCaption(strprintf("%s %d",
            // TRANSLATORS: debug window label
            _("Minimal hit:"), target->getMinHit()));
        mMaxHitLabel->setCaption(strprintf("%s %d",
            // TRANSLATORS: debug window label
            _("Maximum hit:"), target->getMaxHit()));
        mCriticalHitLabel->setCaption(strprintf("%s %d",
            // TRANSLATORS: debug window label
            _("Critical hit:"), target->getCriticalHit()));

        const int delay = target->getAttackDelay();
        if (delay)
        {
            mAttackDelayLabel->setCaption(strprintf("%s %d",
                // TRANSLATORS: debug window label
                _("Attack delay:"), delay));
        }
        else
        {
            mAttackDelayLabel->setCaption(strprintf(
                // TRANSLATORS: debug window label
                "%s ?", _("Attack delay:")));
        }
    }
    else
    {
        // TRANSLATORS: debug window label
        mTargetLabel->setCaption(strprintf("%s ?", _("Target:")));
        // TRANSLATORS: debug window label
        mTargetIdLabel->setCaption(strprintf("%s ?", _("Target Id:")));
        // TRANSLATORS: debug window label
        mTargetTypeLabel->setCaption(strprintf("%s ?", _("Target type:")));
        // TRANSLATORS: debug window label
        mTargetLevelLabel->setCaption(strprintf("%s ?", _("Target Level:")));
        // TRANSLATORS: debug window label
        mTargetPartyLabel->setCaption(strprintf("%s ?", _("Target Party:")));
        // TRANSLATORS: debug window label
        mTargetGuildLabel->setCaption(strprintf("%s ?", _("Target Guild:")));
        // TRANSLATORS: debug window label
        mAttackDelayLabel->setCaption(strprintf("%s ?", _("Attack delay:")));
        // TRANSLATORS: debug window label
        mMinHitLabel->setCaption(strprintf("%s ?", _("Minimal hit:")));
        // TRANSLATORS: debug window label
        mMaxHitLabel->setCaption(strprintf("%s ?", _("Maximum hit:")));
        // TRANSLATORS: debug window label
        mCriticalHitLabel->setCaption(strprintf("%s ?", _("Critical hit:")));
    }

    mTargetLabel->adjustSize();
    mTargetIdLabel->adjustSize();
    mTargetTypeLabel->adjustSize();
    mTargetLevelLabel->adjustSize();
    mTargetPartyLabel->adjustSize();
    mTargetGuildLabel->adjustSize();
    mAttackDelayLabel->adjustSize();
    BLOCK_END("TargetDebugTab::logic")
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
    setDimension(Rect(0, 0, 600, 300));
}

void NetDebugTab::logic()
{
    BLOCK_START("NetDebugTab::logic")
    if (player_node)
    {
        // TRANSLATORS: debug window label
        mPingLabel->setCaption(strprintf(_("Ping: %s ms"),
            player_node->getPingTime().c_str()));
    }
    else
    {
        // TRANSLATORS: debug window label
        mPingLabel->setCaption(strprintf(_("Ping: %s ms"), "0"));
    }
    // TRANSLATORS: debug window label
    mInPackets1Label->setCaption(strprintf(_("In: %d bytes/s"),
        PacketCounters::getInBytes()));
    // TRANSLATORS: debug window label
    mOutPackets1Label->setCaption(strprintf(_("Out: %d bytes/s"),
        PacketCounters::getOutBytes()));
    BLOCK_END("NetDebugTab::logic")
}
