/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2012  The ManaPlus developers
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

#include "gui/killstats.h"

#include <math.h>
#include <guichan/widgets/label.hpp>

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/chattab.h"
#include "gui/chatwindow.h"

#include "actorspritemanager.h"
#include "depricatedevent.h"
#include "localplayer.h"
#include "playerinfo.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

KillStats::KillStats():
    Window(_("Kill stats"), false, nullptr, "killstats.xml"),
    mKillCounter(0), mExpCounter(0),
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
    setSaveVisible(true);
    setStickyButtonLock(true);
    setDefaultSize(250, 250, 350, 300);

    listen(CHANNEL_ATTRIBUTES);
    int xp(PlayerInfo::getAttribute(EXP));
    int xpNextLevel(PlayerInfo::getAttribute(EXP_NEEDED));

    mResetButton = new Button(_("Reset stats"), "reset", this);
    mTimerButton = new Button(_("Reset timer"), "timer", this);
    if (!xpNextLevel)
        xpNextLevel = 1;

    mLine1 = new Label(strprintf(_("Level: %d at %f%%"),
        player_node->getLevel(), static_cast<double>(xp)
        / static_cast<double>(xpNextLevel) * 100.0));

    mLine2 = new Label(strprintf(_("Exp: %d/%d Left: %d"),
        xp, xpNextLevel, xpNextLevel - xp));

    mLine3 = new Label(strprintf(_("1%% = %d exp, avg mob for 1%%: %s"),
        xpNextLevel / 100, "?"));

    mLine4 = new Label(strprintf(_("Kills: %s, total exp: %s"), "?", "?"));
    mLine5 = new Label(strprintf(_("Avg Exp: %s"), "?"));
    mLine6 = new Label(strprintf(_("No. of avg mob to next level: %s"), "?"));
    mLine7 = new Label(strprintf(_("Kills/Min: %s, Exp/Min: %s"), "?", "?"));

    mExpSpeed1Label = new Label(strprintf(ngettext("Exp speed per %d min: %s",
        "Exp speed per %d min: %s", 1), 1, "?"));
    mExpTime1Label = new Label(strprintf(ngettext(
        "Time for next level per %d min: %s",
        "Time for next level per %d min: %s", 1), 1, "?"));
    mExpSpeed5Label = new Label(strprintf(ngettext("Exp speed per %d min: %s",
        "Exp speed per %d min: %s", 5), 5, "?"));
    mExpTime5Label = new Label(strprintf(ngettext(
        "Time for next level per %d min: %s",
        "Time for next level per %d min: %s", 5), 5, "?"));
    mExpSpeed15Label = new Label(strprintf(ngettext("Exp speed per %d min: %s",
        "Exp speed per %d min: %s", 15), 15, "?"));
    mExpTime15Label = new Label(strprintf(ngettext(
        "Time for next level per %d min: %s",
        "Time for next level per %d min: %s", 15), 15, "?"));

    mLastKillExpLabel = new Label(strprintf("%s ?", _("Last kill exp:")));
    mTimeBeforeJackoLabel = new Label(strprintf(
        "%s ?", _("Time before jacko spawn:")));

    place(0, 0, mLine1, 6).setPadding(0);
    place(0, 1, mLine2, 6).setPadding(0);
    place(0, 2, mLine3, 6).setPadding(0);
    place(0, 3, mLine4, 6).setPadding(0);
    place(0, 4, mLine5, 6).setPadding(0);
    place(0, 5, mLine6, 6).setPadding(0);
    place(0, 6, mLine7, 6).setPadding(0);

    place(0, 7, mLastKillExpLabel, 6).setPadding(0);
    place(0, 8, mTimeBeforeJackoLabel, 6).setPadding(0);
    place(0, 9, mExpSpeed1Label, 6).setPadding(0);
    place(0, 10, mExpTime1Label, 6).setPadding(0);
    place(0, 11, mExpSpeed5Label, 6).setPadding(0);
    place(0, 12, mExpTime5Label, 6).setPadding(0);
    place(0, 13, mExpSpeed15Label, 6).setPadding(0);
    place(0, 14, mExpTime15Label, 6).setPadding(0);

    place(5, 13, mTimerButton).setPadding(0);
    place(5, 14, mResetButton).setPadding(0);

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
        mLine3->setCaption(strprintf("1%% = %d exp, avg mob for 1%%: %s",
            PlayerInfo::getAttribute(EXP_NEEDED) / 100, "?"));
        mLine4->setCaption(strprintf(_("Kills: %s, total exp: %s"), "?", "?"));
        mLine5->setCaption(strprintf(_("Avg Exp: %s"), "?"));
        mLine6->setCaption(strprintf(
            _("No. of avg mob to next level: %s"), "?"));

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
        mLine7->setCaption(strprintf(
            _("Kills/Min: %s, Exp/Min: %s"), "?", "?"));

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

    mLine1->setCaption(strprintf(_("Level: %d at %f%%"),
        player_node->getLevel(), static_cast<double>(
        PlayerInfo::getAttribute(EXP)) / static_cast<double>(
        xpNextLevel) * 100.0));

    mLine2->setCaption(strprintf(_("Exp: %d/%d Left: %d"),
        PlayerInfo::getAttribute(EXP), xpNextLevel,
        xpNextLevel - PlayerInfo::getAttribute(EXP)));

    if (AvgExp >= 0.001f && AvgExp <= 0.001f)
    {
        mLine3->setCaption(strprintf(_("1%% = %d exp, avg mob for 1%%: %s"),
            xpNextLevel / 100, "?"));

        mLine5->setCaption(strprintf(_("Avg Exp: %s"),
            toString(AvgExp).c_str()));

        mLine6->setCaption(strprintf(
            _("No. of avg mob to next level: %s"), "?"));
    }
    else
    {
        mLine3->setCaption(strprintf(_("1%% = %d exp, avg mob for 1%%: %s"),
            xpNextLevel / 100, toString((static_cast<float>(
            xpNextLevel) / 100) / AvgExp).c_str()));

        mLine5->setCaption(strprintf(_("Avg Exp: %s"),
            toString(AvgExp).c_str()));

        mLine6->setCaption(strprintf(_("No. of avg mob to next level: %s"),
            toString(static_cast<float>(xpNextLevel
            - PlayerInfo::getAttribute(EXP)) / AvgExp).c_str()));
    }
    mLine4->setCaption(strprintf(_("Kills: %s, total exp: %s"),
        toString(mKillCounter).c_str(), toString(mExpCounter).c_str()));

    mLine7->setCaption(strprintf(_("Kills/Min: %s, Exp/Min: %s"),
        toString(mKillTCounter / timeDiff).c_str(),
        toString(mExpTCounter / timeDiff).c_str()));

    mLastKillExpLabel->setCaption(strprintf("%s %d", _("Last kill exp:"), xp));

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
    mExpSpeed1Label->setCaption(strprintf(ngettext("Exp speed per %d min: %s",
        "Exp speed per %d min: %s", 1), 1, toString(m1minSpeed).c_str()));

    if (m1minSpeed != 0)
    {
        mExpTime1Label->setCaption(strprintf(_("  Time for next level: %s"),
            toString(static_cast<float>((PlayerInfo::getAttribute(EXP_NEEDED)
            - PlayerInfo::getAttribute(EXP)) / m1minSpeed)).c_str()));
    }
    else
    {
        mExpTime1Label->setCaption(strprintf(
            _("  Time for next level: %s"), "?"));
    }
    mExpTime1Label->adjustSize();

    mExpSpeed5Label->setCaption(strprintf(ngettext("Exp speed per %d min: %s",
        "Exp speed per %d min: %s", 5), 5, toString(m5minSpeed / 5).c_str()));
    mExpSpeed5Label->adjustSize();

    if (m5minSpeed != 0)
    {
        mExpTime5Label->setCaption(strprintf(_("  Time for next level: %s"),
            toString(static_cast<float>((PlayerInfo::getAttribute(EXP_NEEDED)
            - PlayerInfo::getAttribute(EXP)) / m5minSpeed * 5)).c_str()));
    }
    else
    {
        mExpTime5Label->setCaption(strprintf(
            _("  Time for next level: %s"), "?"));
    }
    mExpTime5Label->adjustSize();


    mExpSpeed15Label->setCaption(strprintf(ngettext("Exp speed per %d min: %s",
        "Exp speed per %d min: %s", 15), 15, toString(
        m15minSpeed / 15).c_str()));
    mExpSpeed15Label->adjustSize();

    if (m15minSpeed != 0)
    {
        mExpTime15Label->setCaption(strprintf(_("  Time for next level: %s"),
            toString(static_cast<float>((PlayerInfo::getAttribute(EXP_NEEDED)
            - PlayerInfo::getAttribute(EXP)) / m15minSpeed * 15)).c_str()));
    }
    else
    {
        mExpTime15Label->setCaption(strprintf(
            _("  Time for next level: %s"), "?"));
    }

    validateJacko();
    updateJackoLabel();
}
void KillStats::draw(gcn::Graphics *g)
{
//    update();

    Window::draw(g);
}

void KillStats::updateJackoLabel()
{
    if (mIsJackoAlive)
    {
        mTimeBeforeJackoLabel->setCaption(strprintf("%s jacko alive",
            _("Time before jacko spawn:")));
    }
    else if (mIsJackoSpawnTimeUnknown && mJackoSpawnTime != 0)
    {
        // TRANSLATORS: Example: Time before jacko spawn: 10?
        mTimeBeforeJackoLabel->setCaption(strprintf(
            _("%s %d?"), _("Time before jacko spawn:"),
            mJackoSpawnTime - cur_time));
    }
    else if (mIsJackoMustSpawn)
    {
        mTimeBeforeJackoLabel->setCaption(strprintf("%s %s",
            _("Time before jacko spawn:"), _("jacko spawning")));
    }
    else
    {
        mTimeBeforeJackoLabel->setCaption(strprintf("%s %d",
            _("Time before jacko spawn:"), mJackoSpawnTime - cur_time));
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

void KillStats::processEvent(Channels channel A_UNUSED,
                             const DepricatedEvent &event)
{
    if (event.getName() == EVENT_UPDATEATTRIBUTE)
    {
        int id = event.getInt("id");
        if (id == EXP || id == EXP_NEEDED)
        {
            gainXp(event.getInt("newValue") - event.getInt("oldValue"));
//            update();
        }
    }
}
