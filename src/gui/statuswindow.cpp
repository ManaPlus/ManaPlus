/*
 *  The Mana Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *
 *  This file is part of The Mana Client.
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

#include "gui/statuswindow.h"

#include "configuration.h"
#include "event.h"
#include "localplayer.h"
#include "playerinfo.h"
#include "units.h"
#include "viewport.h"

#include "gui/setup.h"
#include "gui/theme.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/vertcontainer.h"
#include "gui/widgets/windowcontainer.h"

#include "net/net.h"
#include "net/playerhandler.h"
#include "net/gamehandler.h"

#include "utils/gettext.h"
#include "utils/mathutils.h"
#include "utils/stringutils.h"

class AttrDisplay : public Container
{
    public:
        enum Type
        {
            DERIVED = 0,
            CHANGEABLE,
            UNKNOWN
        };

        ~AttrDisplay();

        virtual std::string update();

        virtual Type getType()
        { return UNKNOWN; }

    protected:
        AttrDisplay(int id, const std::string &name);

        const int mId;
        const std::string mName;

        LayoutHelper *mLayout;
        Label *mLabel;
        Label *mValue;
};

class DerDisplay : public AttrDisplay
{
    public:
        DerDisplay(int id, const std::string &name);

        virtual Type getType()
        { return DERIVED; }
};

class ChangeDisplay : public AttrDisplay, gcn::ActionListener
{
    public:
        ChangeDisplay(int id, const std::string &name);

        std::string update();

        virtual Type getType()
        { return CHANGEABLE; }

        void setPointsNeeded(int needed);

    private:
        void action(const gcn::ActionEvent &event);

        int mNeeded;

        Label *mPoints;
        Button *mDec;
        Button *mInc;
};

StatusWindow::StatusWindow():
    Window(player_node ? player_node->getName() : "?")
{
    listen(CHANNEL_ATTRIBUTES);

    setWindowName("Status");
    setupWindow->registerWindowForReset(this);
    setResizable(true);
    setCloseButton(true);
    setSaveVisible(true);
    setDefaultSize((windowContainer->getWidth() - 365) / 2,
                   (windowContainer->getHeight() - 255) / 2, 365, 275);

    // ----------------------
    // Status Part
    // ----------------------

    mLvlLabel = new Label(strprintf(_("Level: %d"), 0));
    mMoneyLabel = new Label(strprintf(_("Money: %s"), ""));

    int max = PlayerInfo::getAttribute(MAX_HP);
    if (!max)
        max = 1;

    mHpLabel = new Label(_("HP:"));
    mHpBar = new ProgressBar(max ?
            static_cast<float>(PlayerInfo::getAttribute(HP))
            / static_cast<float>(max):
            static_cast<float>(0), 80, 15, Theme::PROG_HP);

    max = PlayerInfo::getAttribute(EXP_NEEDED);
    mXpLabel = new Label(_("Exp:"));
    mXpBar = new ProgressBar(max ?
            static_cast<float>(PlayerInfo::getAttribute(EXP))
            / static_cast<float>(max):
            static_cast<float>(0), 80, 15, Theme::PROG_EXP);

    bool magicBar = Net::getGameHandler()->canUseMagicBar();

    int job = Net::getPlayerHandler()->getJobLocation()
              && serverConfig.getValueBool("showJob", false);

    if (magicBar)
    {
        max = PlayerInfo::getAttribute(MAX_MP);
        mMpLabel = new Label(_("MP:"));
        mMpBar = new ProgressBar(max ?
                static_cast<float>(PlayerInfo::getAttribute(MAX_MP))
                / static_cast<float>(max) : static_cast<float>(0),
                80, 15, Net::getPlayerHandler()->canUseMagic() ?
                Theme::PROG_MP : Theme::PROG_NO_MP);
    }
    else
    {
        mMpLabel = 0;
        mMpBar = 0;
    }

    place(0, 0, mLvlLabel, 3);
    // 5, 0 Job Level
    place(8, 0, mMoneyLabel, 3);
    place(0, 1, mHpLabel).setPadding(3);
    place(1, 1, mHpBar, 4);
    place(5, 1, mXpLabel).setPadding(3);
    place(6, 1, mXpBar, 5);
    if (magicBar)
    {
        place(0, 2, mMpLabel).setPadding(3);
        // 5, 2 and 6, 2 Job Progress Bar
        if (job)
            place(1, 2, mMpBar, 4);
        else
            place(1, 2, mMpBar, 10);
    }

    if (job)
    {
        mJobLvlLabel = new Label(strprintf(_("Job: %d"), 0));
        mJobLabel = new Label(_("Job:"));
        mJobBar = new ProgressBar(0.0f, 80, 15, Theme::PROG_JOB);

        place(5, 0, mJobLvlLabel, 3);
        place(5, 2, mJobLabel).setPadding(3);
        place(6, 2, mJobBar, 5);
    }
    else
    {
        mJobLvlLabel = 0;
        mJobLabel = 0;
        mJobBar = 0;
    }

    // ----------------------
    // Stats Part
    // ----------------------

    mAttrCont = new VertContainer(32);
    mAttrScroll = new ScrollArea(mAttrCont);
    mAttrScroll->setOpaque(false);
    mAttrScroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mAttrScroll->setVerticalScrollPolicy(ScrollArea::SHOW_AUTO);
    place(0, 3, mAttrScroll, 5, 3);

    mDAttrCont = new VertContainer(32);
    mDAttrScroll = new ScrollArea(mDAttrCont);
    mDAttrScroll->setOpaque(false);
    mDAttrScroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mDAttrScroll->setVerticalScrollPolicy(ScrollArea::SHOW_AUTO);
    place(6, 3, mDAttrScroll, 5, 3);

    getLayout().setRowHeight(3, Layout::AUTO_SET);

    mCharacterPointsLabel = new Label("C");
    place(0, 6, mCharacterPointsLabel, 5);

    if (Net::getPlayerHandler()->canCorrectAttributes())
    {
        mCorrectionPointsLabel = new Label("C");
        place(0, 7, mCorrectionPointsLabel, 5);
    }

    loadWindowState();

    // Update bars
    updateHPBar(mHpBar, true);
    if (magicBar)
        updateMPBar(mMpBar, true);
    updateXPBar(mXpBar, false);

    mMoneyLabel->setCaption(strprintf(_("Money: %s"),
        Units::formatCurrency(PlayerInfo::getAttribute(MONEY)).c_str()));
    mMoneyLabel->adjustSize();
    mCharacterPointsLabel->setCaption(strprintf(_("Character points: %d"),
                                      PlayerInfo::getAttribute(CHAR_POINTS)));
    mCharacterPointsLabel->adjustSize();

    if (player_node && player_node->isGM())
    {
        mLvlLabel->setCaption(strprintf(_("Level: %d (GM %d)"),
            PlayerInfo::getAttribute(LEVEL), player_node->getGMLevel()));
    }
    else
    {
        mLvlLabel->setCaption(strprintf(_("Level: %d"),
            PlayerInfo::getAttribute(LEVEL)));
    }
    mLvlLabel->adjustSize();
}

void StatusWindow::event(Channels channel _UNUSED_,
                         const Mana::Event &event)
{
    if (event.getName() == EVENT_UPDATEATTRIBUTE)
    {
        switch(event.getInt("id"))
        {
            case HP: case MAX_HP:
                updateHPBar(mHpBar, true);
                break;

            case MP: case MAX_MP:
                updateMPBar(mMpBar, true);
                break;

            case EXP: case EXP_NEEDED:
                updateXPBar(mXpBar, false);
                break;

            case MONEY:
                mMoneyLabel->setCaption(strprintf(_("Money: %s"),
                    Units::formatCurrency(event.getInt("newValue")).c_str()));
                mMoneyLabel->adjustSize();
                break;

            case CHAR_POINTS:
                mCharacterPointsLabel->setCaption(strprintf(
                    _("Character points: %d"), event.getInt("newValue")));

                mCharacterPointsLabel->adjustSize();
                // Update all attributes
                for (Attrs::iterator it = mAttrs.begin();
                     it != mAttrs.end(); ++it)
                {
                    if (it->second)
                        it->second->update();
                }
            break;

            case CORR_POINTS:
                mCorrectionPointsLabel->setCaption(strprintf(
                    _("Correction points: %d"), event.getInt("newValue")));
                mCorrectionPointsLabel->adjustSize();
                // Update all attributes
                for (Attrs::iterator it = mAttrs.begin();
                     it != mAttrs.end(); ++it)
                {
                    if (it->second)
                        it->second->update();
                }
            break;

            case LEVEL:
                mLvlLabel->setCaption(strprintf(_("Level: %d"),
                                      event.getInt("newValue")));
                mLvlLabel->adjustSize();
            break;

            default:
                break;
        }
    }
    else if (event.getName() == EVENT_UPDATESTAT)
    {
        int id = event.getInt("id");
        if (id == Net::getPlayerHandler()->getJobLocation())
        {
            if (mJobLvlLabel)
            {
                int lvl = PlayerInfo::getStatBase(id);
                if (!lvl)
                {
                    // possible server corrupted and dont send job level,
                    // then we fixing it :)
                    std::pair<int, int> exp
                        = PlayerInfo::getStatExperience(id);
                    if (exp.second < 20000)
                    {
                        lvl = 0;
                    }
                    else
                    {
                        lvl = (exp.second - 20000) / 150;
                        PlayerInfo::setStatBase(id, lvl);
                    }
                }

                mJobLvlLabel->setCaption(strprintf(_("Job: %d"), lvl));
                mJobLvlLabel->adjustSize();

                updateProgressBar(mJobBar, id, false);
            }
        }
        else
        {
            updateMPBar(mMpBar, true);
            Attrs::iterator it = mAttrs.find(id);
            if (it != mAttrs.end() && it->second)
            {
                if (it->second)
                    it->second->update();
            }
        }
    }
}

void StatusWindow::setPointsNeeded(int id, int needed)
{
    Attrs::iterator it = mAttrs.find(id);

    if (it != mAttrs.end())
    {
        AttrDisplay *disp = it->second;
        if (disp && disp->getType() == AttrDisplay::CHANGEABLE)
            static_cast<ChangeDisplay*>(disp)->setPointsNeeded(needed);
    }
}

void StatusWindow::addAttribute(int id, const std::string &name,
                                bool modifiable,
                                const std::string &description _UNUSED_)
{
    AttrDisplay *disp;

    if (modifiable)
    {
        disp = new ChangeDisplay(id, name);
        mAttrCont->add(disp);
    }
    else
    {
        disp = new DerDisplay(id, name);
        mDAttrCont->add(disp);
    }

    mAttrs[id] = disp;
}

void StatusWindow::updateHPBar(ProgressBar *bar, bool showMax)
{
    if (!bar)
        return;

    if (showMax)
        bar->setText(toString(PlayerInfo::getAttribute(HP)) +
                    "/" + toString(PlayerInfo::getAttribute(MAX_HP)));
    else
        bar->setText(toString(PlayerInfo::getAttribute(HP)));

    float prog = 1.0;

    if (PlayerInfo::getAttribute(MAX_HP) > 0)
    {
        prog = static_cast<float>(PlayerInfo::getAttribute(HP))
            / static_cast<float>(PlayerInfo::getAttribute(MAX_HP));
    }
    bar->setProgress(prog);
}

void StatusWindow::updateMPBar(ProgressBar *bar, bool showMax)
{
    if (!bar)
        return;

    if (showMax)
    {
        bar->setText(toString(PlayerInfo::getAttribute(MP)) +
                    "/" + toString(PlayerInfo::getAttribute(MAX_MP)));
    }
    else
    {
        bar->setText(toString(PlayerInfo::getAttribute(MP)));
    }

    float prog = 1.0f;

    if (PlayerInfo::getAttribute(MAX_MP) > 0)
    {
        if (PlayerInfo::getAttribute(MAX_MP))
        {
            prog = static_cast<float>(PlayerInfo::getAttribute(MP))
                    / static_cast<float>(PlayerInfo::getAttribute(MAX_MP));
        }
        else
        {
            prog = static_cast<float>(PlayerInfo::getAttribute(MP));
        }
    }

    if (Net::getPlayerHandler()->canUseMagic())
        bar->setProgressPalette(Theme::PROG_MP);
    else
        bar->setProgressPalette(Theme::PROG_NO_MP);

    bar->setProgress(prog);
}

void StatusWindow::updateProgressBar(ProgressBar *bar, int value, int max,
                                     bool percent)
{
    if (!bar)
        return;

    if (max == 0)
    {
        bar->setText(_("Max"));
        bar->setProgress(1.0);
    }
    else
    {
        float progress = static_cast<float>(value)
                         / static_cast<float>(max);

        if (percent)
            bar->setText(strprintf("%2.5f", 100 * progress) + "%");
        else
            bar->setText(toString(value) + "/" + toString(max));

        bar->setProgress(progress);
    }
}

void StatusWindow::updateXPBar(ProgressBar *bar, bool percent)
{
    if (!bar)
        return;

    updateProgressBar(bar, PlayerInfo::getAttribute(EXP),
                      PlayerInfo::getAttribute(EXP_NEEDED), percent);
}

void StatusWindow::updateJobBar(ProgressBar *bar, bool percent)
{
    if (!bar)
        return;

    std::pair<int, int> exp =  PlayerInfo::getStatExperience(
        Net::getPlayerHandler()->getJobLocation());
    updateProgressBar(bar, exp.first, exp.second, percent);
}

void StatusWindow::updateProgressBar(ProgressBar *bar, int id, bool percent)
{
    std::pair<int, int> exp =  PlayerInfo::getStatExperience(id);
    updateProgressBar(bar, exp.first, exp.second, percent);
}

void StatusWindow::updateStatusBar(ProgressBar *bar, bool percent _UNUSED_)
{
    if (!player_node || !viewport)
        return;

    std::string str;

    switch (player_node->getInvertDirection())
    {
        case 0:
            str = "D";
            break;
        case 1:
            str = "I";
            break;
        case 2:
            str = "c";
            break;
        case 3:
            str = "C";
            break;
        case 4:
            str = "d";
            break;
        default:
            str = "?";
            break;
    }

    if (player_node->getCrazyMoveType() < 10)
        str += toString(player_node->getCrazyMoveType());
    else
    {
        switch (player_node->getCrazyMoveType())
        {
            case 10:
                str += "a";
                break;
            default:
                str += "?";
                break;
        }
    }

    switch (player_node->getMoveToTargetType())
    {
        case 0:
            str += "0";
            break;
        case 1:
            str += "1";
            break;
        case 2:
            str += "2";
            break;
        case 3:
            str += "3";
            break;
        case 4:
            str += "5";
            break;
        case 5:
            str += "7";
            break;
        case 6:
            str += "A";
            break;
        default:
            str += "?";
            break;
    }

    switch (player_node->getFollowMode())
    {
        case 0:
            str += "D";
            break;
        case 1:
            str += "R";
            break;
        case 2:
            str += "M";
            break;
        case 3:
            str += "P";
            break;
        default:
            str += "?";
            break;
    }

    str += " ";
    switch (player_node->getAttackWeaponType())
    {
        case 1:
            str += "D";
            break;
        case 2:
            str += "s";
            break;
        case 3:
            str += "S";
            break;
        default:
            str += "?";
            break;
    }

    switch (player_node->getAttackType())
    {
        case 0:
            str += "D";
            break;
        case 1:
            str += "G";
            break;
        case 2:
            str += "A";
            break;
        case 3:
            str += "d";
            break;
        default:
            str += "?";
            break;
    }

    switch (player_node->getMagicAttackType())
    {
        case 0:
            str += "f";
            break;
        case 1:
            str += "c";
            break;
        case 2:
            str += "I";
            break;
        case 3:
            str += "F";
            break;
        case 4:
            str += "U";
            break;
        default:
            str += "?";
            break;
    }

    str += " " + toString(player_node->getQuickDropCounter());

    switch (player_node->getPickUpType())
    {
        case 0:
            str += "S";
            break;
        case 1:
            str += "D";
            break;
        case 2:
            str += "F";
            break;
        case 3:
            str += "3";
            break;
        case 4:
            str += "g";
            break;
        case 5:
            str += "G";
            break;
        case 6:
            str += "A";
            break;
        default:
            str += "?";
            break;
    }

    switch (viewport->getDebugPath())
    {
        case 0:
            str += " N";
            break;
        case 1:
            str += " D";
            break;
        case 2:
            str += " u";
            break;
        case 3:
            str += " U";
            break;
        case 4:
            str += " e";
            break;
        case 5:
            str += " b";
            break;
        default:
            str += " ?";
            break;
    }

    switch (player_node->getImitationMode())
    {
        case 0:
            str += " D";
            break;
        case 1:
            str += " O";
            break;
        default:
            str += " ?";
            break;
    }

    switch (viewport->getCameraMode())
    {
        case 0:
            str += "G";
            break;
        case 1:
            str += "F";
            break;
        case 2:
            str += "D";
            break;
        default:
            str += "?";
            break;
    }

    switch (player_node->getAwayMode())
    {
        case 0:
            str += "O";
            break;
        case 1:
            str += "A";
            break;
        default:
            str += "?";
            break;
    }

    bar->setText(str);
    bar->setProgress(50);
    if (player_node->getDisableGameModifiers())
    {
        gcn::Color col;
        col.r = 100;
        col.g = 100;
        col.b = 100;
//        bar->setColor(new gcn::Color(100, 100, 100));
        bar->setColor(col);
    }
    else
    {
        gcn::Color col;
        col.r = 255;
        col.g = 255;
        col.b = 0;
//        bar->setColor(new gcn::Color(255, 255, 0));
        bar->setColor(col);
    }
}

AttrDisplay::AttrDisplay(int id, const std::string &name):
        mId(id),
        mName(name)
{
    setSize(100, 32);
    mLabel = new Label(name);
    mValue = new Label("1 ");

    mLabel->setAlignment(Graphics::CENTER);
    mValue->setAlignment(Graphics::CENTER);

    mLayout = new LayoutHelper(this);
}

AttrDisplay::~AttrDisplay()
{
    delete mLayout;
}

std::string AttrDisplay::update()
{
    int base = PlayerInfo::getStatBase(mId);
    int bonus = PlayerInfo::getStatMod(mId);
    std::string value = toString(base + bonus);
    if (bonus)
        value += strprintf("=%d%+d", base, bonus);
    mValue->setCaption(value);
    return mName;
}

DerDisplay::DerDisplay(int id, const std::string &name):
        AttrDisplay(id, name)
{
    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = mLayout->getPlacer(0, 0);

    place(0, 0, mLabel, 3);
    place(3, 0, mValue, 2);

    update();
}

ChangeDisplay::ChangeDisplay(int id, const std::string &name):
        AttrDisplay(id, name), mNeeded(1)
{
    mPoints = new Label(_("Max"));
    mInc = new Button(_("+"), "inc", this);

    // Do the layout
    ContainerPlacer place = mLayout->getPlacer(0, 0);

    place(0, 0, mLabel, 3);
    place(4, 0, mValue, 2);
    place(6, 0, mInc);
    place(7, 0, mPoints);

    if (Net::getPlayerHandler()->canCorrectAttributes())
    {
        mDec = new Button(_("-"), "dec", this);
        mDec->setWidth(mInc->getWidth());

        place(3, 0, mDec);
    }
    else
    {
        mDec = 0;
    }

    update();
}

std::string ChangeDisplay::update()
{
    if (mNeeded > 0)
        mPoints->setCaption(toString(mNeeded));
    else
        mPoints->setCaption(_("Max"));

    if (mDec)
        mDec->setEnabled(PlayerInfo::getAttribute(CORR_POINTS));

    mInc->setEnabled(PlayerInfo::getAttribute(CHAR_POINTS) >= mNeeded &&
                     mNeeded > 0);

    return AttrDisplay::update();
}

void ChangeDisplay::setPointsNeeded(int needed)
{
    mNeeded = needed;

    update();
}

void ChangeDisplay::action(const gcn::ActionEvent &event)
{
    if (Net::getPlayerHandler()->canCorrectAttributes() &&
        event.getSource() == mDec)
    {
        int newcorpoints = PlayerInfo::getAttribute(CORR_POINTS) - 1;
        PlayerInfo::setAttribute(CORR_POINTS, newcorpoints);

        int newpoints = PlayerInfo::getAttribute(CHAR_POINTS) + 1;
        PlayerInfo::setAttribute(CHAR_POINTS, newpoints);

        int newbase = PlayerInfo::getStatBase(mId) - 1;
        PlayerInfo::setStatBase(mId, newbase);

        Net::getPlayerHandler()->decreaseAttribute(mId);
    }
    else if (event.getSource() == mInc)
    {
        int cnt = 1;
        if (config.getBoolValue("quickStats"))
        {
            cnt = mInc->getClickCount();
            if (cnt > 10)
                cnt = 10;
        }

        int newpoints = PlayerInfo::getAttribute(CHAR_POINTS) - cnt;
        PlayerInfo::setAttribute(CHAR_POINTS, newpoints);

        int newbase = PlayerInfo::getStatBase(mId) + cnt;
        PlayerInfo::setStatBase(mId, newbase);

        for (unsigned f = 0; f < mInc->getClickCount(); f ++)
        {
            Net::getPlayerHandler()->increaseAttribute(mId);
            if (cnt != 1)
                SDL_Delay(100);
        }
    }
}
