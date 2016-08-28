/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_TABS_DEBUGWINDOWTABS_H
#define GUI_WIDGETS_TABS_DEBUGWINDOWTABS_H

#include "gui/widgets/container.h"

class Label;

class DebugTab notfinal : public Container
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
        Label *mMusicFileLabel A_NONNULLPOINTER;
        Label *mMapLabel A_NONNULLPOINTER;
        Label *mMapNameLabel A_NONNULLPOINTER;
        Label *mMinimapLabel A_NONNULLPOINTER;
        Label *mTileMouseLabel A_NONNULLPOINTER;
        Label *mParticleCountLabel A_NONNULLPOINTER;
        Label *mMapActorCountLabel A_NONNULLPOINTER;
        Label *mXYLabel A_NONNULLPOINTER;
        Label *mTexturesLabel A_NONNULLPOINTER;
        time_t mUpdateTime;
#ifdef DEBUG_DRAW_CALLS
        Label *mDrawCallsLabel A_NONNULLPOINTER;
#endif
#ifdef DEBUG_BIND_TEXTURE
        Label *mBindsLabel A_NONNULLPOINTER;
#endif
        Label *mFPSLabel A_NONNULLPOINTER;
        Label *mLPSLabel A_NONNULLPOINTER;
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
        Label *mTargetLabel A_NONNULLPOINTER;
        Label *mTargetIdLabel A_NONNULLPOINTER;
        Label *mTargetTypeLabel A_NONNULLPOINTER;
        Label *mTargetLevelLabel A_NONNULLPOINTER;
        Label *mTargetRaceLabel A_NONNULLPOINTER;
        Label *mTargetPartyLabel A_NONNULLPOINTER;
        Label *mTargetGuildLabel A_NONNULLPOINTER;
        Label *mAttackDelayLabel A_NONNULLPOINTER;
        Label *mMinHitLabel A_NONNULLPOINTER;
        Label *mMaxHitLabel A_NONNULLPOINTER;
        Label *mCriticalHitLabel A_NONNULLPOINTER;
        Label *mKarmaLabel A_NONNULLPOINTER;
        Label *mMannerLabel A_NONNULLPOINTER;
        Label *mEffectsLabel A_NONNULLPOINTER;
};

class NetDebugTab final : public DebugTab
{
    friend class DebugWindow;

    public:
        explicit NetDebugTab(const Widget2 *const widget);

        A_DELETE_COPY(NetDebugTab)

        void logic() override final;

    private:
        Label *mPingLabel A_NONNULLPOINTER;
        Label *mInPackets1Label A_NONNULLPOINTER;
        Label *mOutPackets1Label A_NONNULLPOINTER;
};

#endif  // GUI_WIDGETS_TABS_DEBUGWINDOWTABS_H
