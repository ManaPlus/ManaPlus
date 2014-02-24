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

#ifndef GUI_WINDOWS_DEBUGWINDOW_H
#define GUI_WINDOWS_DEBUGWINDOW_H

#include "gui/widgets/container.h"
#include "gui/widgets/window.h"

class Label;
class TabbedArea;

class DebugTab : public Container
{
    friend class DebugWindow;

    public:
        A_DELETE_COPY(DebugTab)

        void logic() override
        {
        }

        void resize(const int x, const int y)
        { setDimension(Rect(0, 0, x, y)); }

    protected:
        explicit DebugTab(const Widget2 *const widget) :
            Container(widget)
        { }
};

class MapDebugTab final : public DebugTab
{
    friend class DebugWindow;

    public:
        explicit MapDebugTab(const Widget2 *const widget);

        A_DELETE_COPY(MapDebugTab)

        void logic() override final;

    private:
        Label *mMusicFileLabel;
        Label *mMapLabel;
        Label *mMinimapLabel;
        Label *mTileMouseLabel;
        Label *mParticleCountLabel;
        Label *mMapActorCountLabel;
        Label *mXYLabel;
        Label *mTexturesLabel;
        int mUpdateTime;
#ifdef DEBUG_DRAW_CALLS
        Label *mDrawCallsLabel;
#endif
#ifdef DEBUG_BIND_TEXTURE
        Label *mBindsLabel;
#endif
        Label *mFPSLabel;
        Label *mLPSLabel;
        std::string mFPSText;
};

class TargetDebugTab final : public DebugTab
{
    friend class DebugWindow;

    public:
        explicit TargetDebugTab(const Widget2 *const widget);

        A_DELETE_COPY(TargetDebugTab)

        void logic() override final;

    private:
        Label *mTargetLabel;
        Label *mTargetIdLabel;
        Label *mTargetTypeLabel;
        Label *mTargetLevelLabel;
        Label *mTargetRaceLabel;
        Label *mTargetPartyLabel;
        Label *mTargetGuildLabel;
        Label *mAttackDelayLabel;
        Label *mMinHitLabel;
        Label *mMaxHitLabel;
        Label *mCriticalHitLabel;
};

class NetDebugTab final : public DebugTab
{
    friend class DebugWindow;

    public:
        explicit NetDebugTab(const Widget2 *const widget);

        A_DELETE_COPY(NetDebugTab)

        void logic() override final;

    private:
        Label *mPingLabel;
        Label *mInPackets1Label;
        Label *mOutPackets1Label;
};

/**
 * The debug window.
 *
 * \ingroup Interface
 */
class DebugWindow final : public Window
{
    public:
        /**
         * Constructor.
         */
        DebugWindow();

        A_DELETE_COPY(DebugWindow)

        ~DebugWindow();

        void postInit() override final;

        /**
         * Logic (updates components' size and infos)
         */
        void slowLogic();

        void draw(Graphics *g) override final;

        void setPing(int pingTime);

        void widgetResized(const Event &event) override final;

#ifdef USE_PROFILER
        void logicChildren();
#endif

    private:
        TabbedArea *mTabs;
        MapDebugTab *mMapWidget;
        TargetDebugTab *mTargetWidget;
        NetDebugTab *mNetWidget;
};

extern DebugWindow *debugWindow;

#endif  // GUI_WINDOWS_DEBUGWINDOW_H
