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

#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include "gui/widgets/container.h"
#include "gui/widgets/window.h"

class Container;
class DebugWindow;
class Label;
class Tab;
class TabbedArea;

class DebugTab : public Container
{
    friend class DebugWindow;

    public:
        DebugTab() :
            Container()
        { }

        void logic() = 0;

        void resize(const int x, const int y)
        { setDimension(gcn::Rectangle(0, 0, x, y)); }

    private:
};

class MapDebugTab final : public DebugTab
{
    friend class DebugWindow;

    public:
        MapDebugTab();

        void logic();

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
        Label *mFPSLabel;
        Label *mLPSLabel;
        std::string mFPSText;
};

class TargetDebugTab final : public DebugTab
{
    friend class DebugWindow;

    public:
        TargetDebugTab();

        void logic();

    private:
        Label *mTargetLabel;
        Label *mTargetIdLabel;
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
        NetDebugTab();

        void logic();

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

        ~DebugWindow();

        /**
         * Logic (updates components' size and infos)
         */
        void slowLogic();

        void draw(gcn::Graphics *g);

        void setPing(int pingTime);

        void widgetResized(const gcn::Event &event);

    private:
        TabbedArea *mTabs;
        MapDebugTab *mMapWidget;
        TargetDebugTab *mTargetWidget;
        NetDebugTab *mNetWidget;
};

extern DebugWindow *debugWindow;

#endif
