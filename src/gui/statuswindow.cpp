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

#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/vertcontainer.h"

#include "net/net.h"
#include "net/playerhandler.h"
#include "net/gamehandler.h"

#include "utils/gettext.h"
#include "utils/mathutils.h"

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

        A_DELETE_COPY(AttrDisplay)

        ~AttrDisplay();

        virtual std::string update();

        virtual Type getType() const
        { return UNKNOWN; }

        std::string getValue() const
        {
            if (!mValue)
                return "-";
            else
                return mValue->getCaption();
        }

    protected:
        AttrDisplay(const Widget2 *const widget,
                    const int id, const std::string &name);

        const int mId;
        const std::string mName;

        LayoutHelper *mLayout;
        Label *mLabel;
        Label *mValue;
};

class DerDisplay final : public AttrDisplay
{
    public:
        DerDisplay(const Widget2 *const widget,
                   const int id, const std::string &name);

        A_DELETE_COPY(DerDisplay)

        virtual Type getType() const
        { return DERIVED; }
};

class ChangeDisplay final : public AttrDisplay, gcn::ActionListener
{
    public:
        ChangeDisplay(const Widget2 *const widget,
                      const int id, const std::string &name);

        A_DELETE_COPY(ChangeDisplay)

        std::string update();

        virtual Type getType() const
        { return CHANGEABLE; }

        void setPointsNeeded(int needed);

        void action(const gcn::ActionEvent &event);

    private:
        int mNeeded;

        Label *mPoints;
        Button *mDec;
        Button *mInc;
};

StatusWindow::StatusWindow() :
    Window(player_node ? player_node->getName() :
        "?", false, nullptr, "status.xml"),
    gcn::ActionListener(),
    mLvlLabel(new Label(strprintf(_("Level: %d"), 0))),
    mMoneyLabel(new Label(strprintf(_("Money: %s"), ""))),
    mHpLabel(new Label(_("HP:"))),
    mMpLabel(nullptr),
    mXpLabel(new Label(_("Exp:"))),
    mHpBar(nullptr),
    mMpBar(nullptr),
    mXpBar(nullptr),
    mJobLvlLabel(nullptr),
    mJobLabel(nullptr),
    mJobBar(nullptr),
    mAttrCont(new VertContainer(this,32)),
    mAttrScroll(new ScrollArea(mAttrCont, false)),
    mDAttrCont(new VertContainer(this, 32)),
    mDAttrScroll(new ScrollArea(mDAttrCont, false)),
    mCharacterPointsLabel(new Label("C")),
    mCorrectionPointsLabel(nullptr),
    mCopyButton(new Button(this, _("Copy to chat"), "copy", this))
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

    if (player_node && !player_node->getRaceName().empty())
    {
        setCaption(strprintf("%s (%s)", player_node->getName().c_str(),
            player_node->getRaceName().c_str()));
    }

    int max = PlayerInfo::getAttribute(PlayerInfo::MAX_HP);
    if (!max)
        max = 1;

    mHpBar = new ProgressBar(max ?
            static_cast<float>(PlayerInfo::getAttribute(PlayerInfo::HP))
            / static_cast<float>(max):
            static_cast<float>(0), 80, 0, Theme::PROG_HP);

    max = PlayerInfo::getAttribute(PlayerInfo::EXP_NEEDED);
    mXpBar = new ProgressBar(max ?
            static_cast<float>(PlayerInfo::getAttribute(PlayerInfo::EXP))
            / static_cast<float>(max):
            static_cast<float>(0), 80, 0, Theme::PROG_EXP);

    const bool magicBar = Net::getGameHandler()->canUseMagicBar();
    const int job = Net::getPlayerHandler()->getJobLocation()
              && serverConfig.getValueBool("showJob", false);

    if (magicBar)
    {
        max = PlayerInfo::getAttribute(PlayerInfo::MAX_MP);
        mMpLabel = new Label(_("MP:"));
        mMpBar = new ProgressBar(max ? static_cast<float>(
            PlayerInfo::getAttribute(PlayerInfo::MAX_MP))
            / static_cast<float>(max) : static_cast<float>(0),
            80, 0, Net::getPlayerHandler()->canUseMagic() ?
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
        mJobBar = new ProgressBar(0.0f, 80, 0, Theme::PROG_JOB);

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

    mAttrScroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mAttrScroll->setVerticalScrollPolicy(ScrollArea::SHOW_AUTO);
    place(0, 3, mAttrScroll, 5, 3);

    mDAttrScroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mDAttrScroll->setVerticalScrollPolicy(ScrollArea::SHOW_AUTO);
    place(6, 3, mDAttrScroll, 5, 3);

    getLayout().setRowHeight(3, Layout::AUTO_SET);

    place(0, 6, mCharacterPointsLabel, 5);


    place(0, 5, mCopyButton);

    if (Net::getPlayerHandler()->canCorrectAttributes())
    {
        mCorrectionPointsLabel = new Label("C");
        place(0, 7, mCorrectionPointsLabel, 5);
    }

    loadWindowState();
    enableVisibleSound(true);

    // Update bars
    updateHPBar(mHpBar, true);
    if (magicBar)
        updateMPBar(mMpBar, true);
    updateXPBar(mXpBar, false);

    mMoneyLabel->setCaption(strprintf(_("Money: %s"), Units::formatCurrency(
        PlayerInfo::getAttribute(PlayerInfo::MONEY)).c_str()));
    mMoneyLabel->adjustSize();
    mCharacterPointsLabel->setCaption(strprintf(_("Character points: %d"),
        PlayerInfo::getAttribute(PlayerInfo::CHAR_POINTS)));
    mCharacterPointsLabel->adjustSize();

    if (player_node && player_node->isGM())
    {
        mLvlLabel->setCaption(strprintf(_("Level: %d (GM %d)"),
            PlayerInfo::getAttribute(PlayerInfo::LEVEL),
            player_node->getGMLevel()));
    }
    else
    {
        mLvlLabel->setCaption(strprintf(_("Level: %d"),
            PlayerInfo::getAttribute(PlayerInfo::LEVEL)));
    }
    mLvlLabel->adjustSize();
}

void StatusWindow::processEvent(Channels channel A_UNUSED,
                                const DepricatedEvent &event)
{
    static bool blocked = false;
    if (blocked)
        return;

    if (event.getName() == EVENT_UPDATEATTRIBUTE)
    {
        switch (event.getInt("id"))
        {
            case PlayerInfo::HP:
            case PlayerInfo::MAX_HP:
                updateHPBar(mHpBar, true);
                break;

            case PlayerInfo::MP:
            case PlayerInfo::MAX_MP:
                updateMPBar(mMpBar, true);
                break;

            case PlayerInfo::EXP:
            case PlayerInfo::EXP_NEEDED:
                updateXPBar(mXpBar, false);
                break;

            case PlayerInfo::MONEY:
                mMoneyLabel->setCaption(strprintf(_("Money: %s"),
                    Units::formatCurrency(event.getInt("newValue")).c_str()));
                mMoneyLabel->adjustSize();
                break;

            case PlayerInfo::CHAR_POINTS:
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

            case PlayerInfo::CORR_POINTS:
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

            case PlayerInfo::LEVEL:
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
        const int id = event.getInt("id");
        if (id == Net::getPlayerHandler()->getJobLocation())
        {
            if (mJobLvlLabel)
            {
                int lvl = PlayerInfo::getStatBase(
                    static_cast<PlayerInfo::Attribute>(id));

                const int oldExp = event.getInt("oldValue1");
                const std::pair<int, int> exp = PlayerInfo::getStatExperience(
                    static_cast<PlayerInfo::Attribute>(id));

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
                        PlayerInfo::setStatBase(
                            static_cast<PlayerInfo::Attribute>(id), lvl);
                        blocked = false;
                    }
                }

                if (exp.first < oldExp && exp.second >= 20000)
                {   // possible job level up. but server broken and don't send
                    // new job exp limit, we fixing it
                    lvl ++;
                    blocked = true;
                    PlayerInfo::setStatExperience(
                        static_cast<PlayerInfo::Attribute>(id),
                        exp.first, 20000 + lvl * 150);
                    PlayerInfo::setStatBase(
                        static_cast<PlayerInfo::Attribute>(id), lvl);
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
            const Attrs::const_iterator it = mAttrs.find(id);
            if (it != mAttrs.end() && it->second)
            {
                if (it->second)
                    it->second->update();
            }
        }
    }
}

void StatusWindow::setPointsNeeded(const int id, const int needed)
{
    const Attrs::const_iterator it = mAttrs.find(id);

    if (it != mAttrs.end())
    {
        AttrDisplay *const disp = it->second;
        if (disp && disp->getType() == AttrDisplay::CHANGEABLE)
            static_cast<ChangeDisplay*>(disp)->setPointsNeeded(needed);
    }
}

void StatusWindow::addAttribute(const int id, const std::string &name,
                                const bool modifiable,
                                const std::string &description A_UNUSED)
{
    AttrDisplay *disp;

    if (modifiable)
    {
        disp = new ChangeDisplay(this, id, name);
        mAttrCont->add1(disp);
    }
    else
    {
        disp = new DerDisplay(this, id, name);
        mDAttrCont->add1(disp);
    }
    mAttrs[id] = disp;
}

void StatusWindow::clearAttributes()
{
    mAttrCont->clear();
    mDAttrCont->clear();
    for (Attrs::iterator it = mAttrs.begin(), it_end = mAttrs.end();
         it != it_end; ++ it)
    {
        delete (*it).second;
    }
    mAttrs.clear();
}

void StatusWindow::updateHPBar(ProgressBar *const bar, const bool showMax)
{
    if (!bar)
        return;

    if (showMax)
    {
        bar->setText(toString(PlayerInfo::getAttribute(PlayerInfo::HP)) +
            "/" + toString(PlayerInfo::getAttribute(PlayerInfo::MAX_HP)));
    }
    else
    {
        bar->setText(toString(PlayerInfo::getAttribute(PlayerInfo::HP)));
    }

    float prog = 1.0;

    if (PlayerInfo::getAttribute(PlayerInfo::MAX_HP) > 0)
    {
        prog = static_cast<float>(PlayerInfo::getAttribute(PlayerInfo::HP))
            / static_cast<float>(PlayerInfo::getAttribute(PlayerInfo::MAX_HP));
    }
    bar->setProgress(prog);
}

void StatusWindow::updateMPBar(ProgressBar *const bar, const bool showMax)
{
    if (!bar)
        return;

    if (showMax)
    {
        bar->setText(toString(PlayerInfo::getAttribute(PlayerInfo::MP)) +
            "/" + toString(PlayerInfo::getAttribute(PlayerInfo::MAX_MP)));
    }
    else
    {
        bar->setText(toString(PlayerInfo::getAttribute(PlayerInfo::MP)));
    }

    float prog = 1.0f;

    if (PlayerInfo::getAttribute(PlayerInfo::MAX_MP) > 0)
    {
        if (PlayerInfo::getAttribute(PlayerInfo::MAX_MP))
        {
            prog = static_cast<float>(PlayerInfo::getAttribute(PlayerInfo::MP))
                / static_cast<float>(PlayerInfo::getAttribute(
                PlayerInfo::MAX_MP));
        }
        else
        {
            prog = static_cast<float>(PlayerInfo::getAttribute(
                PlayerInfo::MP));
        }
    }

    if (Net::getPlayerHandler()->canUseMagic())
        bar->setProgressPalette(Theme::PROG_MP);
    else
        bar->setProgressPalette(Theme::PROG_NO_MP);

    bar->setProgress(prog);
}

void StatusWindow::updateProgressBar(ProgressBar *const bar, const int value,
                                     const int max, const bool percent)
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
        const float progress = static_cast<float>(value)
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

void StatusWindow::updateXPBar(ProgressBar *const bar, const bool percent)
{
    if (!bar)
        return;

    updateProgressBar(bar, PlayerInfo::getAttribute(PlayerInfo::EXP),
        PlayerInfo::getAttribute(PlayerInfo::EXP_NEEDED), percent);
}

void StatusWindow::updateJobBar(ProgressBar *const bar, const bool percent)
{
    if (!bar)
        return;

    const std::pair<int, int> exp =  PlayerInfo::getStatExperience(
        Net::getPlayerHandler()->getJobLocation());
    updateProgressBar(bar, exp.first, exp.second, percent);
}

void StatusWindow::updateProgressBar(ProgressBar *const bar, const int id,
                                     const bool percent) const
{
    const std::pair<int, int> exp =  PlayerInfo::getStatExperience(
        static_cast<PlayerInfo::Attribute>(id));
    updateProgressBar(bar, exp.first, exp.second, percent);
}

void StatusWindow::updateWeightBar(ProgressBar *const bar)
{
    if (!bar)
        return;

    if (PlayerInfo::getAttribute(PlayerInfo::MAX_WEIGHT) == 0)
    {
        bar->setText(_("Max"));
        bar->setProgress(1.0);
    }
    else
    {
        const float progress = static_cast<float>(PlayerInfo::getAttribute(
            PlayerInfo::TOTAL_WEIGHT)) / static_cast<float>(
            PlayerInfo::getAttribute(PlayerInfo::MAX_WEIGHT));

        bar->setText(strprintf("%s/%s", Units::formatWeight(
            PlayerInfo::getAttribute(PlayerInfo::TOTAL_WEIGHT)).c_str(),
            Units::formatWeight(PlayerInfo::getAttribute(
            PlayerInfo::MAX_WEIGHT)).c_str()));

        bar->setProgress(progress);
    }
}

void StatusWindow::updateMoneyBar(ProgressBar *const bar)
{
    if (!bar)
        return;

    const int money = PlayerInfo::getAttribute(PlayerInfo::MONEY);
    bar->setText(Units::formatCurrency(money).c_str());
    if (money > 0)
    {
        const float progress = static_cast<float>(money)
            / static_cast<float>(1000000000);
        bar->setProgress(progress);
    }
    else
    {
        bar->setProgress(1.0);
    }
}

void StatusWindow::updateArrowsBar(ProgressBar *const bar)
{
    if (!bar || !equipmentWindow)
        return;

    const Item *const item = equipmentWindow->getEquipment(
        Equipment::EQUIP_PROJECTILE_SLOT);

    if (item && item->getQuantity() > 0)
        bar->setText(toString(item->getQuantity()));
    else
        bar->setText("0");
}

void StatusWindow::updateInvSlotsBar(ProgressBar *const bar)
{
    if (!bar)
        return;

    const Inventory *const inv = PlayerInfo::getInventory();
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

std::string StatusWindow::translateLetter(const char *const letters)
{
    char buf[2];
    char *const str = gettext(letters);
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

void StatusWindow::updateStatusBar(ProgressBar *const bar,
                                   const bool percent A_UNUSED)
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
        const Attrs::const_iterator it_end = mAttrs.end();
        std::string str;
        while (it != it_end)
        {
            const ChangeDisplay *const attr = dynamic_cast<ChangeDisplay*>(
                (*it).second);
            if (attr)
                str += attr->getValue() + " ";
            ++ it;
        }
        chatWindow->addInputText(str);
    }
}

AttrDisplay::AttrDisplay(const Widget2 *const widget,
                         const int id, const std::string &name) :
    Container(widget),
    mId(id),
    mName(name),
    mLayout(new LayoutHelper(this)),
    mLabel(new Label(name)),
    mValue(new Label("1 "))
{
    setSize(100, 32);

    mLabel->setAlignment(Graphics::CENTER);
    mValue->setAlignment(Graphics::CENTER);
}

AttrDisplay::~AttrDisplay()
{
    delete mLayout;
    mLayout = nullptr;
}

std::string AttrDisplay::update()
{
    const int base = PlayerInfo::getStatBase(
        static_cast<PlayerInfo::Attribute>(mId));
    const int bonus = PlayerInfo::getStatMod(
        static_cast<PlayerInfo::Attribute>(mId));
    std::string value = toString(base + bonus);
    if (bonus)
        value += strprintf("=%d%+d", base, bonus);
    mValue->setCaption(value);
    return mName;
}

DerDisplay::DerDisplay(const Widget2 *const widget,
                      const int id, const std::string &name) :
    AttrDisplay(widget, id, name)
{
//    LayoutHelper h(this);
    ContainerPlacer place = mLayout->getPlacer(0, 0);

    place(0, 0, mLabel, 3);
    place(3, 0, mValue, 2);

    update();
}

ChangeDisplay::ChangeDisplay(const Widget2 *const widget,
                             const int id, const std::string &name) :
    AttrDisplay(widget, id, name),
    gcn::ActionListener(),
    mNeeded(1),
    mPoints(new Label(_("Max"))),
    mDec(nullptr),
    mInc(new Button(this, _("+"), "inc", this))
{
    // Do the layout
    ContainerPlacer place = mLayout->getPlacer(0, 0);

    place(0, 0, mLabel, 3);
    place(4, 0, mValue, 2);
    place(6, 0, mInc);
    place(7, 0, mPoints);

    if (Net::getPlayerHandler()->canCorrectAttributes())
    {
        mDec = new Button(this, _("-"), "dec", this);
        mDec->setWidth(mInc->getWidth());

        place(3, 0, mDec);
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
        mDec->setEnabled(PlayerInfo::getAttribute(PlayerInfo::CORR_POINTS));

    mInc->setEnabled(PlayerInfo::getAttribute(PlayerInfo::CHAR_POINTS)
        >= mNeeded && mNeeded > 0);

    return AttrDisplay::update();
}

void ChangeDisplay::setPointsNeeded(const int needed)
{
    mNeeded = needed;

    update();
}

void ChangeDisplay::action(const gcn::ActionEvent &event)
{
    if (Net::getPlayerHandler()->canCorrectAttributes() &&
        event.getSource() == mDec)
    {
        const int newcorpoints = PlayerInfo::getAttribute(
            PlayerInfo::CORR_POINTS) - 1;
        PlayerInfo::setAttribute(PlayerInfo::CORR_POINTS, newcorpoints);

        const int newpoints = PlayerInfo::getAttribute(
            PlayerInfo::CHAR_POINTS) + 1;
        PlayerInfo::setAttribute(PlayerInfo::CHAR_POINTS, newpoints);

        const int newbase = PlayerInfo::getStatBase(
            static_cast<PlayerInfo::Attribute>(mId)) - 1;
        PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(
            mId), newbase);

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

        const int newpoints = PlayerInfo::getAttribute(
            PlayerInfo::CHAR_POINTS) - cnt;
        PlayerInfo::setAttribute(PlayerInfo::CHAR_POINTS, newpoints);

        const int newbase = PlayerInfo::getStatBase(
            static_cast<PlayerInfo::Attribute>(mId)) + cnt;
        PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(
            mId), newbase);

        for (unsigned f = 0; f < mInc->getClickCount(); f ++)
        {
            Net::getPlayerHandler()->increaseAttribute(mId);
            if (cnt != 1)
                SDL_Delay(100);
        }
    }
}
