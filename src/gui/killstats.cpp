/*
 *  The Mana World
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *
 *  This file is part of The Mana World.
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gui/killstats.h"

#include <math.h>
#include "guichan/widgets/label.hpp"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/chattab.h"
#include "gui/chatwindow.h"

#include "actorspritemanager.h"
#include "event.h"
#include "localplayer.h"
#include "playerinfo.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

KillStats::KillStats():
    Window(_("Kill stats")), mKillCounter(0), mExpCounter(0),
    mKillTCounter(0), mExpTCounter(0), mKillTimer(0),
    m1minExpTime(0), m1minExpNum(0), m1minSpeed(0),
    m5minExpTime(0), m5minExpNum(0), m5minSpeed(0),
    m15minExpTime(0), m15minExpNum(0), m15minSpeed(0),
    mJackoSpawnTime(0), mValidateJackoTime(0), mJackoId(0),
    mIsJackoAlive(false), mIsJackoMustSpawn(true),
    mIsJackoSpawnTimeUnknown(true)
{
    setWindowName("Kill stats");
    setCloseButton(true);
    setResizable(true);
    setDefaultSize(250, 250, 350, 300);

    listen(Mana::CHANNEL_ATTRIBUTES);
    int xp(PlayerInfo::getAttribute(EXP));
    int xpNextLevel(PlayerInfo::getAttribute(EXP_NEEDED));

    mResetButton = new Button(_("Reset stats"), "reset", this);
    mTimerButton = new Button(_("Reset timer"), "timer", this);
    if (!xpNextLevel)
        xpNextLevel = 1;

    mLine1 = new Label(_("Level: ") + toString(player_node->getLevel())
        + " at " + toString(static_cast<float>(xp)
        / static_cast<float>(xpNextLevel) * 100.0f) + "%");

    mLine2 = new Label(_("Exp: ") + toString(xp) + "/" +
        toString(xpNextLevel) + _(" Left: ") +
        toString(xpNextLevel-xp));
    mLine3 = new Label("1% = " + toString(xpNextLevel / 100) +
            _(" exp, Avg Mob for 1%: ?"));
    mLine4 = new Label(_("Kills: ?, Total Exp: ?"));
    mLine5 = new Label(_("Avg Exp: ?, No. of Avg mob to next level: ?"));
    mLine6 = new Label(_("Kills/Min: ?, Exp/Min: ?"));

    mExpSpeed1Label = new Label(_("Exp speed per 1 min: ?"));
    mExpTime1Label = new Label(_("Time for next level per 1 min: ?"));
    mExpSpeed5Label = new Label(_("Exp speed per 5 min: ?"));
    mExpTime5Label = new Label(_("Time for next level per 5 min: ?"));
    mExpSpeed15Label = new Label(_("Exp speed per 15 min: ?"));
    mExpTime15Label = new Label(_("Time for Next level per 15 min: ?"));

    mLastKillExpLabel = new Label(_("Last kill exp: ?"));
    mTimeBeforeJackoLabel = new Label(_("Time before jacko spawn: ?"));

    place(0, 0, mLine1, 6).setPadding(0);
    place(0, 1, mLine2, 6).setPadding(0);
    place(0, 2, mLine3, 6).setPadding(0);
    place(0, 3, mLine4, 6).setPadding(0);
    place(0, 4, mLine5, 6).setPadding(0);
    place(0, 5, mLine6, 6).setPadding(0);

    place(0, 6, mLastKillExpLabel, 6).setPadding(0);
    place(0, 7, mTimeBeforeJackoLabel, 6).setPadding(0);
    place(0, 8, mExpSpeed1Label, 6).setPadding(0);
    place(0, 9, mExpTime1Label, 6).setPadding(0);
    place(0, 10, mExpSpeed5Label, 6).setPadding(0);
    place(0, 11, mExpTime5Label, 6).setPadding(0);
    place(0, 12, mExpSpeed15Label, 6).setPadding(0);
    place(0, 13, mExpTime15Label, 6).setPadding(0);

    place(5, 12, mTimerButton).setPadding(0);
    place(5, 13, mResetButton).setPadding(0);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);

    loadWindowState();

}

KillStats::~KillStats()
{
}

void KillStats::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "reset")
    {
        mKillCounter = 0;
        mExpCounter = 0;
        mLine3->setCaption("1% = " + toString(
            PlayerInfo::getAttribute(EXP_NEEDED) / 100) +
            " exp, Avg Mob for 1%: ?");
        mLine4->setCaption(_("Kills: ?, Total Exp: ?"));
        mLine5->setCaption(_("Avg Exp: ?, No. of Avg mob to next level: ?"));

        m1minExpTime = 0;
        m1minExpNum = 0;
        m1minSpeed = 0;
        m5minExpTime = 0;
        m5minExpNum = 0;
        m5minSpeed = 0;
        m15minExpTime = 0;
        m15minExpNum = 0;
        m15minSpeed = 0;
    }
    else if (event.getId() == "timer")
    {
        mKillTimer = 0;
        mKillTCounter = 0;
        mExpTCounter = 0;
        mLine6->setCaption(_("Kills/Min: ?, Exp/Min: ?"));

        m1minExpTime = 0;
        m1minExpNum = 0;
        m1minSpeed = 0;
        m5minExpTime = 0;
        m5minExpNum = 0;
        m5minSpeed = 0;
        m15minExpTime = 0;
        m15minExpNum = 0;
        m15minSpeed = 0;
    }
}

void KillStats::gainXp(int xp)
{
    if (xp == PlayerInfo::getAttribute(EXP_NEEDED))
        xp = 0;
    else if (!xp)
        return;

    mKillCounter++;
    mKillTCounter++;

    mExpCounter = mExpCounter + xp;
    mExpTCounter = mExpTCounter + xp;
    if (!mKillCounter)
        mKillCounter = 1;

    float AvgExp = static_cast<float>(mExpCounter / mKillCounter);
    int xpNextLevel(PlayerInfo::getAttribute(EXP_NEEDED));

    if (mKillTimer == 0)
        mKillTimer = cur_time;

    if (!xpNextLevel)
        xpNextLevel = 1;

    double timeDiff = difftime(cur_time, mKillTimer) / 60;

    if (timeDiff <= 0.001)
        timeDiff = 1;

    mLine1->setCaption("Level: " + toString(player_node->getLevel()) + " at " +
        toString(static_cast<float>(PlayerInfo::getAttribute(EXP))
        / static_cast<float>(xpNextLevel) * 100.0f) + "%");

    mLine2->setCaption("Exp: " + toString(PlayerInfo::getAttribute(EXP)) + "/"
        + toString(xpNextLevel) + " Left: "
        + toString(xpNextLevel - PlayerInfo::getAttribute(EXP)));
    if (AvgExp >= 0.001f && AvgExp <= 0.001f)
    {
        mLine3->setCaption("1% = " + toString(xpNextLevel / 100)
            + " exp, Avg Mob for 1%: ?");

        mLine5->setCaption("Avg Exp: " + toString(AvgExp) +
            ", No. of Avg mob to next level: ?");
    }
    else
    {
        mLine3->setCaption("1% = " + toString(xpNextLevel / 100)
            + " exp, Avg Mob for 1%: " +
            toString((static_cast<float>(xpNextLevel) / 100) / AvgExp));

        mLine5->setCaption("Avg Exp: " + toString(AvgExp) +
            ", No. of Avg mob to next level: " +
            toString(static_cast<float>(xpNextLevel -
            PlayerInfo::getAttribute(EXP)) / AvgExp));
    }
    mLine4->setCaption("Kills: " + toString(mKillCounter) +
        ", Total Exp: " + toString(mExpCounter));

    mLine6->setCaption("Kills/Min: " + toString(mKillTCounter / timeDiff) +
        ", Exp/Min: " + toString(mExpTCounter / timeDiff));

    mLastKillExpLabel->setCaption("Last Kill Exp: " + toString(xp));

    recalcStats();
    update();
}

void KillStats::recalcStats()
{
    int curTime = cur_time;

    // Need Update Exp Counter
    if (curTime - m1minExpTime > 60)
    {
        int newExp = PlayerInfo::getAttribute(EXP);
        if (m1minExpTime != 0)
            m1minSpeed = newExp - m1minExpNum;
        else
            m1minSpeed = 0;
        m1minExpTime = curTime;
        m1minExpNum = newExp;
    }

    if (curTime - m5minExpTime > 60*5)
    {
        int newExp = PlayerInfo::getAttribute(EXP);
        if (m5minExpTime != 0)
            m5minSpeed = newExp - m5minExpNum;
        else
            m5minSpeed = 0;
        m5minExpTime = curTime;
        m5minExpNum = newExp;
    }

    if (curTime - m15minExpTime > 60*15)
    {
        int newExp = PlayerInfo::getAttribute(EXP);
        if (m15minExpTime != 0)
            m15minSpeed = newExp - m15minExpNum;
        else
            m15minSpeed = 0;
        m15minExpTime = curTime;
        m15minExpNum = newExp;
    }
    validateJacko();
}

void KillStats::update()
{
    mExpSpeed1Label->setCaption(
            strprintf(_("Exp Speed per 1 min: %d"), m1minSpeed));
    mExpSpeed1Label->adjustSize();

    if (m1minSpeed != 0)
    {
        mExpTime1Label->setCaption(strprintf(_("  Time For Next Level: %f"),
            static_cast<float>((PlayerInfo::getAttribute(EXP_NEEDED)
            - PlayerInfo::getAttribute(EXP)) / m1minSpeed)));
    }
    else
    {
        mExpTime1Label->setCaption(_("  Time For Next Level: ?"));
    }
    mExpTime1Label->adjustSize();

    mExpSpeed5Label->setCaption(
    strprintf(_("Exp Speed per 5 min: %d"), m5minSpeed / 5));
    mExpSpeed5Label->adjustSize();

    if (m5minSpeed != 0)
    {
        mExpTime5Label->setCaption(strprintf(_("  Time For Next Level: %f"),
            static_cast<float>((PlayerInfo::getAttribute(EXP_NEEDED)
            - PlayerInfo::getAttribute(EXP)) / m5minSpeed * 5)));
    }
    else
    {
        mExpTime5Label->setCaption(_("  Time For Next Level: ?"));
    }
    mExpTime5Label->adjustSize();

    mExpSpeed15Label->setCaption(
        strprintf(_("Exp Speed per 15 min: %d"), m15minSpeed / 15));
    mExpSpeed15Label->adjustSize();

    if (m15minSpeed != 0)
    {
        mExpTime15Label->setCaption(strprintf(_("  Time For Next Level: %f"),
            static_cast<float>((PlayerInfo::getAttribute(EXP_NEEDED)
            - PlayerInfo::getAttribute(EXP)) / m15minSpeed * 15)));
    }
    else
    {
        mExpTime15Label->setCaption(_("  Time For Next Level: ?"));
    }

    validateJacko();
    updateJackoLabel();
}
void KillStats::draw(gcn::Graphics *g)
{
    update();

    Window::draw(g);
}

void KillStats::updateJackoLabel()
{
    if (mIsJackoAlive)
    {
        mTimeBeforeJackoLabel->setCaption(
            _("Time before jacko spawn: jacko alive"));
    }
    else if (mIsJackoSpawnTimeUnknown && mJackoSpawnTime != 0)
    {
        mTimeBeforeJackoLabel->setCaption(_("Time before jacko spawn: ")
            + toString(mJackoSpawnTime - cur_time) + _("?"));
    }
    else if (mIsJackoMustSpawn)
    {
        mTimeBeforeJackoLabel->setCaption(
            _("Time before jacko spawn: jacko spawning"));
    }
    else
    {
        mTimeBeforeJackoLabel->setCaption(_("Time before jacko spawn: ")
            + toString(mJackoSpawnTime - cur_time));
    }
}

void KillStats::jackoDead(int id)
{
    if (id == mJackoId && mIsJackoAlive)
    {
        mIsJackoAlive = false;
        mJackoSpawnTime =  cur_time + 60*4;
        mIsJackoSpawnTimeUnknown = false;
        updateJackoLabel();
    }
}

void KillStats::addLog(std::string str)
{
    if (debugChatTab)
        debugChatTab->chatLog(str, BY_SERVER);
}

void KillStats::jackoAlive(int id)
{
    if (!mIsJackoAlive)
    {
        mJackoId = id;
        mIsJackoAlive = true;
        mIsJackoMustSpawn = false;
        mJackoSpawnTime = 0;
        mIsJackoSpawnTimeUnknown = false;
        updateJackoLabel();
    }
}

void KillStats::validateJacko()
{
    if (!actorSpriteManager || !player_node)
        return;

    Map *currentMap = Game::instance()->getCurrentMap();
    if (currentMap)
    {
        if (currentMap->getProperty("_realfilename") == "018-1"
            || currentMap->getProperty("_realfilename") == "maps/018-1.tmx")
        {
            if (player_node->getTileX() >= 167
                && player_node->getTileX() <= 175
                && player_node->getTileY() >= 21
                && player_node->getTileY() <= 46)
            {
                Being *dstBeing = actorSpriteManager->findBeingByName(
                    "Jack O", Being::MONSTER);
                if (mIsJackoAlive && !dstBeing)
                {
                    mIsJackoAlive = false;
                    mJackoSpawnTime =  cur_time + 60*4;
                    mIsJackoSpawnTimeUnknown = true;
                }
            }
        }

        if (!mIsJackoAlive && cur_time > mJackoSpawnTime + 15)
            mIsJackoMustSpawn = true;
    }
}

void KillStats::event(Mana::Channels channel _UNUSED_,
                      const Mana::Event &event)
{
    if (event.getName() == Mana::EVENT_UPDATEATTRIBUTE)
    {
        int id = event.getInt("id");
        if (id == EXP || id == EXP_NEEDED)
        {
            gainXp(event.getInt("newValue") - event.getInt("oldValue"));
//            update();
        }
    }
}
