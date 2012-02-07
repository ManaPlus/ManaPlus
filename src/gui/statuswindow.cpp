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

#include "gui/statuswindow.h"

#include "chatwindow.h"
#include "configuration.h"
#include "event.h"
#include "equipment.h"
#include "inventory.h"
#include "item.h"
#include "localplayer.h"
#include "playerinfo.h"
#include "units.h"

#include "gui/equipmentwindow.h"
#include "gui/setup.h"
#include "gui/theme.h"
#include "gui/viewport.h"

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

#include "debug.h"

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

        virtual Type getType() const
        { return UNKNOWN; }

        std::string getValue()
        {
            if (!mValue)
                return "-";
            else
                return mValue->getCaption();
        }

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

        virtual Type getType() const
        { return DERIVED; }
};

class ChangeDisplay : public AttrDisplay, gcn::ActionListener
{
    public:
        ChangeDisplay(int id, const std::string &name);

        std::string update();

        virtual Type getType() const
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
    Window(player_node ? player_node->getName() :
        "?", false, nullptr, "status.xml")
{
    listen(CHANNEL_ATTRIBUTES);

    setWindowName("Status");
    setupWindow->registerWindowForReset(this);
    setResizable(true);
    setCloseButton(true);
    setSaveVisible(true);
    setStickyButtonLock(true);
    setDefaultSize((windowContainer->getWidth() - 480) / 2,
                   (windowContainer->getHeight() - 500) / 2, 480, 500);

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
        mMpLabel = nullptr;
        mMpBar = nullptr;
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
        mJobLvlLabel = nullptr;
        mJobLabel = nullptr;
        mJobBar = nullptr;
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

    mCopyButton = new Button(_("Copy to chat"), "copy", this);

    place(0, 5, mCopyButton);

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

void StatusWindow::processEvent(Channels channel A_UNUSED,
                                const Event &event)
{
    static bool blocked = false;
    if (blocked)
        return;

    if (event.getName() == EVENT_UPDATEATTRIBUTE)
    {
        switch (event.getInt("id"))
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
                for (Attrs::const_iterator it = mAttrs.begin();
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
                for (Attrs::const_iterator it = mAttrs.begin();
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

                int oldExp = event.getInt("oldValue1");
                std::pair<int, int> exp
                    = PlayerInfo::getStatExperience(id);

                if (!lvl)
                {
                    // possible server broken and don't send job level,
                    // then we fixing it :)
                    if (exp.second < 20000)
                    {
                        lvl = 0;
                    }
                    else
                    {
                        lvl = (exp.second - 20000) / 150;
                        blocked = true;
                        PlayerInfo::setStatBase(id, lvl);
                        blocked = false;
                    }
                }

                if (exp.first < oldExp && exp.second >= 20000)
                {   // possible job level up. but server broken and don't send
                    // new job exp limit, we fixing it
                    lvl ++;
                    blocked = true;
                    PlayerInfo::setStatExperience(
                        id, exp.first, 20000 + lvl * 150);
                    PlayerInfo::setStatBase(id, lvl);
                    blocked = false;
                }

                mJobLvlLabel->setCaption(strprintf(_("Job: %d"), lvl));
                mJobLvlLabel->adjustSize();

                updateProgressBar(mJobBar, id, false);
            }
        }
        else
        {
            updateMPBar(mMpBar, true);
            Attrs::const_iterator it = mAttrs.find(id);
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
    Attrs::const_iterator it = mAttrs.find(id);

    if (it != mAttrs.end())
    {
        AttrDisplay *disp = it->second;
        if (disp && disp->getType() == AttrDisplay::CHANGEABLE)
            static_cast<ChangeDisplay*>(disp)->setPointsNeeded(needed);
    }
}

void StatusWindow::addAttribute(int id, const std::string &name,
                                bool modifiable,
                                const std::string &description A_UNUSED)
{
    AttrDisplay *disp;

    if (modifiable)
    {
        disp = new ChangeDisplay(id, name);
        mAttrCont->add1(disp);
    }
    else
    {
        disp = new DerDisplay(id, name);
        mDAttrCont->add1(disp);
    }
    mAttrs[id] = disp;
}

void StatusWindow::clearAttributes()
{
    mAttrCont->clear();
    mDAttrCont->clear();
    Attrs::iterator it = mAttrs.begin();
    Attrs::iterator it_end = mAttrs.end();
    for (; it != it_end; ++ it)
        delete (*it).second;
    mAttrs.clear();
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
        bar->setProgress(1);
        bar->setText(toString(value));
    }
    else
    {
        float progress = static_cast<float>(value)
                         / static_cast<float>(max);

        if (percent)
        {
            bar->setText(strprintf("%2.5f%%",
                static_cast<double>(100 * progress)));
        }
        else
        {
            bar->setText(toString(value) + "/" + toString(max));
        }

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

void StatusWindow::updateWeightBar(ProgressBar *bar)
{
    if (!bar)
        return;

    if (PlayerInfo::getAttribute(MAX_WEIGHT) == 0)
    {
        bar->setText(_("Max"));
        bar->setProgress(1.0);
    }
    else
    {
        float progress = static_cast<float>(
            PlayerInfo::getAttribute(TOTAL_WEIGHT))
            / static_cast<float>(PlayerInfo::getAttribute(MAX_WEIGHT));

        bar->setText(strprintf("%s/%s", Units::formatWeight(
            PlayerInfo::getAttribute(TOTAL_WEIGHT)).c_str(),
            Units::formatWeight(PlayerInfo::getAttribute(
            MAX_WEIGHT)).c_str()));

        bar->setProgress(progress);
    }
}

void StatusWindow::updateMoneyBar(ProgressBar *bar)
{
    if (!bar)
        return;

    int money = PlayerInfo::getAttribute(MONEY);
    bar->setText(Units::formatCurrency(money).c_str());
    if (money > 0)
    {
        float progress = static_cast<float>(money)
            / static_cast<float>(1000000000);
        bar->setProgress(progress);
    }
    else
    {
        bar->setProgress(1.0);
    }
}

void StatusWindow::updateArrowsBar(ProgressBar *bar)
{
    if (!bar || !equipmentWindow)
        return;

    Item *item = equipmentWindow->getEquipment(
        Equipment::EQUIP_PROJECTILE_SLOT);

    if (item && item->getQuantity() > 0)
        bar->setText(toString(item->getQuantity()));
    else
        bar->setText("0");
}

void StatusWindow::updateInvSlotsBar(ProgressBar *bar)
{
    if (!bar)
        return;

    Inventory *inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    const int usedSlots = inv->getNumberOfSlotsUsed();
    const int maxSlots = inv->getSize();

    if (maxSlots)
    {
        bar->setProgress(static_cast<float>(usedSlots)
            / static_cast<float>(maxSlots));
    }

    bar->setText(strprintf("%d", usedSlots));
}

std::string StatusWindow::translateLetter(const char* letters)
{
    char buf[2];
    char *str = gettext(letters);
    if (strlen(str) != 3)
        return letters;

    buf[0] = str[1];
    buf[1] = 0;
    return std::string(buf);
}

std::string StatusWindow::translateLetter2(std::string letters)
{
    if (letters.size() < 5)
        return "";

    return std::string(gettext(letters.substr(1, 1).c_str()));
}

void StatusWindow::updateStatusBar(ProgressBar *bar, bool percent A_UNUSED)
{
    if (!player_node || !viewport)
        return;

    bar->setText(translateLetter2(player_node->getInvertDirectionString())
        += translateLetter2(player_node->getCrazyMoveTypeString())
        += translateLetter2(player_node->getMoveToTargetTypeString())
        += translateLetter2(player_node->getFollowModeString())
        += " " + translateLetter2(player_node->getAttackWeaponTypeString())
        += translateLetter2(player_node->getAttackTypeString())
        += translateLetter2(player_node->getMagicAttackString())
        += translateLetter2(player_node->getPvpAttackString())
        += " " + translateLetter2(player_node->getQuickDropCounterString())
        += translateLetter2(player_node->getPickUpTypeString())
        += " " + translateLetter2(player_node->getDebugPathString())
        += " " + translateLetter2(player_node->getImitationModeString())
        += translateLetter2(player_node->getCameraModeString())
        += translateLetter2(player_node->getAwayModeString()));

    bar->setProgress(50);
    if (player_node->getDisableGameModifiers())
    {
        gcn::Color col;
        col.r = 100;
        col.g = 100;
        col.b = 100;
        bar->setColor(col);
    }
    else
    {
        gcn::Color col;
        col.r = 255;
        col.g = 255;
        col.b = 0;
        bar->setColor(col);
    }
}

void StatusWindow::action(const gcn::ActionEvent &event)
{
    if (!chatWindow)
        return;

    if (event.getId() == "copy")
    {
        Attrs::const_iterator it = mAttrs.begin();
        Attrs::const_iterator it_end = mAttrs.end();
        std::string str;
        while (it != it_end)
        {
            ChangeDisplay *attr = dynamic_cast<ChangeDisplay*>((*it).second);
            if (attr)
                str += attr->getValue() + " ";
            ++ it;
        }
        chatWindow->addInputText(str);
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
    mLayout = nullptr;
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
        mDec = nullptr;
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
