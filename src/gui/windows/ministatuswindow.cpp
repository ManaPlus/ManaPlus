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

#include "gui/windows/ministatuswindow.h"

#include "animatedsprite.h"
#include "configuration.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "gui/viewport.h"

#include "gui/popups/textpopup.h"

#include "gui/popups/statuspopup.h"

#include "gui/windows/statuswindow.h"

#include "gui/widgets/progressbar.h"

#include "net/net.h"
#include "net/playerhandler.h"
#include "net/gamehandler.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include "debug.h"

extern volatile int tick_time;

typedef std::vector <ProgressBar*>::const_iterator ProgressBarVectorCIter;

MiniStatusWindow::MiniStatusWindow() :
    Popup("MiniStatus", "ministatus.xml"),
    InventoryListener(),
    mBars(),
    mBarNames(),
    mIcons(),
    // TRANSLATORS: status bar name
    mHpBar(createBar(0, 100, 0, Theme::HP_BAR, Theme::PROG_HP,
        "hpprogressbar.xml", "hpprogressbar_fill.xml",
        "hp bar", _("health bar"))),
    mMpBar(Net::getGameHandler()->canUseMagicBar()
        ? createBar(0, 100, 0, Net::getPlayerHandler()->canUseMagic()
        ? Theme::MP_BAR : Theme::NO_MP_BAR,
        Net::getPlayerHandler()->canUseMagic()
        ? Theme::PROG_MP : Theme::PROG_NO_MP,
        Net::getPlayerHandler()->canUseMagic()
        ? "mpprogressbar.xml" : "nompprogressbar.xml",
        Net::getPlayerHandler()->canUseMagic()
        ? "mpprogressbar_fill.xml" : "nompprogressbar_fill.xml",
        // TRANSLATORS: status bar name
        "mp bar", _("mana bar")) : nullptr),
    mXpBar(createBar(0, 100, 0, Theme::XP_BAR, Theme::PROG_EXP,
        "xpprogressbar.xml", "xpprogressbar_fill.xml",
        // TRANSLATORS: status bar name
        "xp bar", _("experience bar"))),
    mJobBar(nullptr),
    mWeightBar(createBar(0, 140, 0, Theme::WEIGHT_BAR, Theme::PROG_WEIGHT,
        "weightprogressbar.xml", "weightprogressbar_fill.xml",
        // TRANSLATORS: status bar name
        "weight bar", _("weight bar"))),
    mInvSlotsBar(createBar(0, 45, 0,
        Theme::SLOTS_BAR, Theme::PROG_INVY_SLOTS,
        "slotsprogressbar.xml", "slotsprogressbar_fill.xml",
        "inventory slots bar",
        // TRANSLATORS: status bar name
        _("inventory slots bar"))),
    mMoneyBar(createBar(0, 130, 0, Theme::MONEY_BAR, Theme::PROG_MONEY,
        "moneyprogressbar.xml", "moneyprogressbar_fill.xml",
        // TRANSLATORS: status bar name
        "money bar", _("money bar"))),
    mArrowsBar(createBar(0, 50, 0, Theme::ARROWS_BAR, Theme::PROG_ARROWS,
        "arrowsprogressbar.xml", "arrowsprogressbar_fill.xml",
        // TRANSLATORS: status bar name
        "arrows bar", _("arrows bar"))),
    mStatusBar(createBar(100, (config.getIntValue("fontSize") > 16
        ? 250 : 165), 0, Theme::STATUS_BAR, Theme::PROG_STATUS,
        "statusprogressbar.xml", "statusprogressbar_fill.xml",
        // TRANSLATORS: status bar name
        "status bar", _("status bar"))),
    mTextPopup(new TextPopup),
    mStatusPopup(new StatusPopup),
    mSpacing(mSkin ? mSkin->getOption("spacing", 3) : 3),
    mIconPadding(mSkin ? mSkin->getOption("iconPadding", 3) : 3),
    mIconSpacing(mSkin ? mSkin->getOption("iconSpacing", 2) : 2),
    mMaxX(0)
{
    mTextPopup->postInit();
    mStatusPopup->postInit();

    listen(CHANNEL_ATTRIBUTES);

    StatusWindow::updateHPBar(mHpBar);

    if (Net::getGameHandler()->canUseMagicBar())
        StatusWindow::updateMPBar(mMpBar);

    const int job = Net::getPlayerHandler()->getJobLocation()
        && serverConfig.getValueBool("showJob", true);

    StatusWindow::updateXPBar(mXpBar);

    if (job)
    {
        mJobBar = createBar(0, 100, 0, Theme::JOB_BAR, Theme::PROG_JOB,
            // TRANSLATORS: status bar name
            "jobprogressbar.xml", "jobprogressbar_fill.xml",
            "job bar", _("job bar"));
        StatusWindow::updateJobBar(mJobBar);
    }

    loadBars();
    updateBars();

    setVisible(config.getValueBool(getPopupName() + "Visible", true));
    addMouseListener(this);
    Inventory *const inv = PlayerInfo::getInventory();
    if (inv)
        inv->addInventoyListener(this);

    StatusWindow::updateMoneyBar(mMoneyBar);
    StatusWindow::updateArrowsBar(mArrowsBar);
    updateStatus();
}

MiniStatusWindow::~MiniStatusWindow()
{
    delete mTextPopup;
    mTextPopup = nullptr;
    delete mStatusPopup;
    mStatusPopup = nullptr;
    delete_all(mIcons);
    mIcons.clear();

    Inventory *const inv = PlayerInfo::getInventory();
    if (inv)
        inv->removeInventoyListener(this);

    FOR_EACH (ProgressBarVectorCIter, it, mBars)
    {
        ProgressBar *bar = *it;
        if (!bar)
            continue;
        if (!bar->isVisible())
            delete bar;
    }
    mBars.clear();
}

ProgressBar *MiniStatusWindow::createBar(const float progress,
                                         const int width, const int height,
                                         const int textColor,
                                         const int backColor,
                                         const std::string &restrict skin,
                                         const std::string &restrict skinFill,
                                         const std::string &restrict name,
                                         const std::string &restrict
                                         description)
{
    ProgressBar *const bar = new ProgressBar(this,
        progress, width, height, backColor, skin, skinFill);
    bar->setActionEventId(name);
    bar->setId(description);
    bar->setColor(Theme::getThemeColor(textColor),
        Theme::getThemeColor(textColor + 1));
    mBars.push_back(bar);
    mBarNames[name] = bar;
    return bar;
}

void MiniStatusWindow::updateBars()
{
    int x = 0;
    const ProgressBar *lastBar = nullptr;
    FOR_EACH (ProgressBarVectorCIter, it, mBars)
        safeRemove(*it);

    FOR_EACH (ProgressBarVectorCIter, it, mBars)
    {
        ProgressBar *const bar = *it;
        if (!bar)
            continue;
        if (bar->isVisible())
        {
            bar->setPosition(x, 0);
            add(bar);
            x += bar->getWidth() + mSpacing;
            lastBar = bar;
        }
    }

    if (lastBar)
    {
        setContentSize(lastBar->getX() + lastBar->getWidth(),
            lastBar->getY() + lastBar->getHeight());
    }
    mMaxX = x;
}

void MiniStatusWindow::setIcon(const int index, AnimatedSprite *const sprite)
{
    if (index >= static_cast<int>(mIcons.size()))
        mIcons.resize(index + 1, nullptr);

    delete mIcons[index];
    mIcons[index] = sprite;
}

void MiniStatusWindow::eraseIcon(const int index)
{
    if (index < static_cast<int>(mIcons.size()))
    {
        delete mIcons[index];
        mIcons.erase(mIcons.begin() + index);
    }
}

void MiniStatusWindow::drawIcons(Graphics *const graphics)
{
    // Draw icons
    int icon_x = mMaxX + mIconPadding;
    for (size_t i = 0, sz = mIcons.size(); i < sz; i ++)
    {
        const AnimatedSprite *const icon = mIcons[i];
        if (icon)
        {
            icon->draw(graphics, icon_x, mIconPadding);
            icon_x += mIconSpacing + icon->getWidth();
        }
    }
}

void MiniStatusWindow::processEvent(const Channels channel A_UNUSED,
                                    const DepricatedEvent &event)
{
    if (event.getName() == EVENT_UPDATEATTRIBUTE)
    {
        const int id = event.getInt("id");
        if (id == PlayerInfo::HP || id == PlayerInfo::MAX_HP)
        {
            StatusWindow::updateHPBar(mHpBar);
        }
        else if (id == PlayerInfo::MP || id == PlayerInfo::MAX_MP)
        {
            StatusWindow::updateMPBar(mMpBar);
        }
        else if (id == PlayerInfo::EXP || id == PlayerInfo::EXP_NEEDED)
        {
            StatusWindow::updateXPBar(mXpBar);
        }
        else if (id == PlayerInfo::TOTAL_WEIGHT
                 || id == PlayerInfo::MAX_WEIGHT)
        {
            StatusWindow::updateWeightBar(mWeightBar);
        }
        else if (id == PlayerInfo::MONEY)
        {
            StatusWindow::updateMoneyBar(mMoneyBar);
        }
    }
    else if (event.getName() == EVENT_UPDATESTAT)
    {
        StatusWindow::updateMPBar(mMpBar);
        StatusWindow::updateJobBar(mJobBar);
    }
}

void MiniStatusWindow::updateStatus()
{
    StatusWindow::updateStatusBar(mStatusBar);
    if (mStatusPopup && mStatusPopup->isPopupVisible())
        mStatusPopup->update();
}

void MiniStatusWindow::logic()
{
    BLOCK_START("MiniStatusWindow::logic")
    Popup::logic();

    for (size_t i = 0, sz = mIcons.size(); i < sz; i++)
    {
        AnimatedSprite *const icon = mIcons[i];
        if (icon)
            icon->update(tick_time * 10);
    }
    BLOCK_END("MiniStatusWindow::logic")
}

void MiniStatusWindow::draw(Graphics *graphics)
{
    BLOCK_START("MiniStatusWindow::draw")
    drawChildren(graphics);
    BLOCK_END("MiniStatusWindow::draw")
}

void MiniStatusWindow::mouseMoved(MouseEvent &event)
{
    Popup::mouseMoved(event);

    const int x = event.getX();
    const int y = event.getY();

    const Rect &rect = mDimension;
    if (event.getSource() == mStatusBar)
    {
        mStatusPopup->view(x + rect.x, y + rect.y);
        mTextPopup->hide();
    }
    else if (event.getSource() == mXpBar)
    {
        std::string level;
        if (player_node && player_node->isGM())
        {
            // TRANSLATORS: status bar label
            level = strprintf(_("Level: %d (GM %d)"),
                PlayerInfo::getAttribute(PlayerInfo::LEVEL),
                player_node->getGMLevel());
        }
        else
        {
            // TRANSLATORS: status bar label
            level = strprintf(_("Level: %d"),
                PlayerInfo::getAttribute(PlayerInfo::LEVEL));
        }

        const int exp = PlayerInfo::getAttribute(PlayerInfo::EXP);
        const int expNeed = PlayerInfo::getAttribute(PlayerInfo::EXP_NEEDED);
        if (exp > expNeed)
        {
            mTextPopup->show(x + rect.x, y + rect.y, level, strprintf("%d/%d",
                exp, expNeed));
        }
        else
        {
            mTextPopup->show(x + rect.x, y + rect.y, level, strprintf("%d/%d",
                exp, expNeed),
                // TRANSLATORS: status bar label
                strprintf("%s: %d", _("Need"), expNeed - exp));
        }
        mStatusPopup->hide();
    }
    else if (event.getSource() == mHpBar)
    {
        mTextPopup->show(x + rect.x, y + rect.y, event.getSource()->getId(),
            strprintf("%d/%d", PlayerInfo::getAttribute(PlayerInfo::HP),
            PlayerInfo::getAttribute(PlayerInfo::MAX_HP)));
        mStatusPopup->hide();
    }
    else if (event.getSource() == mMpBar)
    {
        mTextPopup->show(x + rect.x, y + rect.y, event.getSource()->getId(),
            strprintf("%d/%d", PlayerInfo::getAttribute(PlayerInfo::MP),
            PlayerInfo::getAttribute(PlayerInfo::MAX_MP)));
        mStatusPopup->hide();
    }
    else if (event.getSource() == mJobBar)
    {
        const std::pair<int, int> exp =  PlayerInfo::getStatExperience(
            Net::getPlayerHandler()->getJobLocation());

        // TRANSLATORS: job bar label
        const std::string level = strprintf(_("Job level: %d"),
            PlayerInfo::getStatBase(
            Net::getPlayerHandler()->getJobLocation()));

        if (exp.first > exp.second)
        {
            mTextPopup->show(x + rect.x, y + rect.y, level,
                strprintf("%d/%d", exp.first, exp.second));
        }
        else
        {
            mTextPopup->show(x + rect.x, y + rect.y, level,
                strprintf("%d/%d", exp.first, exp.second),
                // TRANSLATORS: status bar label
                strprintf("%s: %d", _("Need"), exp.second - exp.first));
        }
        mStatusPopup->hide();
    }
    else if (event.getSource() == mWeightBar)
    {
        mTextPopup->show(x + rect.x, y + rect.y, event.getSource()->getId(),
            strprintf("%d/%d", PlayerInfo::getAttribute(
            PlayerInfo::TOTAL_WEIGHT),
            PlayerInfo::getAttribute(PlayerInfo::MAX_WEIGHT)));
        mStatusPopup->hide();
    }
    else if (event.getSource() == mInvSlotsBar)
    {
        const Inventory *const inv = PlayerInfo::getInventory();
        if (inv)
        {
            const int usedSlots = inv->getNumberOfSlotsUsed();
            const int maxSlots = inv->getSize();
            mTextPopup->show(x + rect.x, y + rect.y,
                event.getSource()->getId(),
                strprintf("%d/%d", usedSlots, maxSlots));
        }
        mStatusPopup->hide();
    }
    else if (event.getSource() == mMoneyBar)
    {
        mTextPopup->show(x + rect.x, y + rect.y,
            event.getSource()->getId(),
            toString(PlayerInfo::getAttribute(PlayerInfo::MONEY)));
    }
    else
    {
        mTextPopup->hide();
        mStatusPopup->hide();
    }
}

void MiniStatusWindow::mousePressed(MouseEvent &event)
{
    if (!viewport)
        return;

    if (event.getButton() == MouseEvent::RIGHT)
    {
        const ProgressBar *const bar = dynamic_cast<ProgressBar*>(
            event.getSource());
        if (!bar)
            return;
        if (viewport)
        {
            viewport->showPopup(getX() + event.getX(),
                getY() + event.getY(), bar);
        }
    }
}

void MiniStatusWindow::mouseExited(MouseEvent &event)
{
    Popup::mouseExited(event);

    mTextPopup->hide();
    mStatusPopup->hide();
}

void MiniStatusWindow::showBar(const std::string &name, const bool visible)
{
    ProgressBar *const bar = mBarNames[name];
    if (!bar)
        return;
    bar->setVisible(visible);
    updateBars();
    saveBars();
}

void MiniStatusWindow::loadBars()
{
    if (!config.getIntValue("ministatussaved"))
    {
        if (mWeightBar)
            mWeightBar->setVisible(false);
        if (mInvSlotsBar)
            mInvSlotsBar->setVisible(false);
        if (mMoneyBar)
            mMoneyBar->setVisible(false);
        if (mArrowsBar)
            mArrowsBar->setVisible(false);
        if (mStatusBar)
            mStatusBar->setVisible(false);
        if (mJobBar)
            mJobBar->setVisible(true);
        return;
    }

    for (int f = 0; f < 10; f ++)
    {
        const std::string str = config.getValue(
            "ministatus" + toString(f), "");
        if (str == "")
            continue;
        ProgressBar *const bar = mBarNames[str];
        if (!bar)
            continue;
        bar->setVisible(false);
    }
}

void MiniStatusWindow::saveBars() const
{
    int i = 0;
    FOR_EACH (ProgressBarVectorCIter, it, mBars)
    {
        const ProgressBar *const bar = *it;
        if (!bar->isVisible())
        {
            config.setValue("ministatus" + toString(i),
                bar->getActionEventId());
            i ++;
        }
    }
    for (int f = i; f < 10; f ++)
        config.deleteKey("ministatus" + toString(f));

    config.setValue("ministatussaved", true);
}

void MiniStatusWindow::slotsChanged(Inventory *const inventory)
{
    if (!inventory)
        return;

    if (inventory->getType() == Inventory::INVENTORY)
        StatusWindow::updateInvSlotsBar(mInvSlotsBar);
}

void MiniStatusWindow::updateArrows()
{
    StatusWindow::updateArrowsBar(mArrowsBar);
}

Rect MiniStatusWindow::getChildrenArea()
{
    const int padding = mPadding;
    const int padding2 = padding * 2;
    const Rect &rect = mDimension;
    return Rect(padding, padding,
        rect.width - padding2,
        rect.height - padding2);
}

#ifdef USE_PROFILER
void MiniStatusWindow::logicChildren()
{
    BLOCK_START("MiniStatusWindow::logicChildren")
    BasicContainer::logicChildren();
    BLOCK_END("MiniStatusWindow::logicChildren")
}
#endif
