/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "gui/windows/statuswindow.h"

#include "configuration.h"
#include "gamemodifiers.h"
#include "settings.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "enums/gui/layouttype.h"

#include "gui/windows/chatwindow.h"

#include "gui/windows/equipmentwindow.h"
#include "gui/windows/setupwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/statspage.h"
#include "gui/widgets/statspagebasic.h"
#include "gui/widgets/tabbedarea.h"
#include "gui/widgets/windowcontainer.h"

#include "net/inventoryhandler.h"
#include "net/playerhandler.h"

#include "resources/db/groupdb.h"
#include "resources/db/unitsdb.h"
#include "resources/db/statdb.h"

#include "resources/item/item.h"

#include "utils/delete2.h"
#include "utils/dtor.h"
#include "utils/foreach.h"
#include "utils/gettext.h"

#include "debug.h"

StatusWindow *statusWindow = nullptr;

StatusWindow::StatusWindow() :
    Window(localPlayer != nullptr ? localPlayer->getName() :
        "?", Modal_false, nullptr, "status.xml"),
    ActionListener(),
    AttributeListener(),
    StatListener(),
    mPages(),
    mTabs(CREATEWIDGETR(TabbedArea, this)),
    // TRANSLATORS: status window label
    mLvlLabel(new Label(this, strprintf(_("Level: %d"), 0))),
    // TRANSLATORS: status window label
    mMoneyLabel(new Label(this, strprintf(_("Money: %s"), ""))),
    // TRANSLATORS: status window label
    mHpLabel(new Label(this, _("HP:"))),
    mMpLabel(nullptr),
    // TRANSLATORS: status window label
    mXpLabel(new Label(this, _("Exp:"))),
    mHpBar(nullptr),
    mMpBar(nullptr),
    mXpBar(nullptr),
    mJobLvlLabel(nullptr),
    mJobLabel(nullptr),
    mJobBar(nullptr),
    mBasicStatsPage(new StatsPageBasic(this)),
    // TRANSLATORS: status window button
    mCopyButton(new Button(this, _("Copy to chat"), "copy", this))
{
    setWindowName("Status");
    if (setupWindow != nullptr)
        setupWindow->registerWindowForReset(this);
    setResizable(true);
    setCloseButton(true);
    setSaveVisible(true);
    setStickyButtonLock(true);
    setDefaultSize((windowContainer->getWidth() - 480) / 2,
        (windowContainer->getHeight() - 500) / 2, 480, 500);

    mTabs->setSelectable(false);
    mTabs->getWidgetContainer()->setSelectable(false);
    mTabs->getTabContainer()->setSelectable(false);

    if ((localPlayer != nullptr) && !localPlayer->getRaceName().empty())
    {
        setCaption(strprintf("%s (%s)", localPlayer->getName().c_str(),
            localPlayer->getRaceName().c_str()));
    }

    int max = PlayerInfo::getAttribute(Attributes::PLAYER_MAX_HP);
    if (max == 0)
        max = 1;

    mHpBar = new ProgressBar(this,
        static_cast<float>(PlayerInfo::getAttribute(Attributes::PLAYER_HP))
        / static_cast<float>(max),
        80,
        0,
        ProgressColorId::PROG_HP,
        "hpprogressbar.xml", "hpprogressbar_fill.xml");
    mHpBar->setColor(getThemeColor(ThemeColorId::HP_BAR),
        getThemeColor(ThemeColorId::HP_BAR_OUTLINE));
    mHpBar->setSelectable(false);

    const int64_t maxExp = PlayerInfo::getAttribute64(
        Attributes::PLAYER_EXP_NEEDED);
    mXpBar = new ProgressBar(this,
        maxExp != 0 ?
        static_cast<float>(PlayerInfo::getAttribute64(
        Attributes::PLAYER_EXP)) /
        static_cast<float>(maxExp) : static_cast<float>(0),
        80,
        0,
        ProgressColorId::PROG_EXP,
        "xpprogressbar.xml", "xpprogressbar_fill.xml");
    mXpBar->setColor(getThemeColor(ThemeColorId::XP_BAR),
        getThemeColor(ThemeColorId::XP_BAR_OUTLINE));
    mXpBar->setSelectable(false);

    const bool job = serverConfig.getValueBool("showJob", true);

    max = PlayerInfo::getAttribute(Attributes::PLAYER_MAX_MP);
    // TRANSLATORS: status window label
    mMpLabel = new Label(this, _("MP:"));
    const bool useMagic = playerHandler->canUseMagic();
    mMpBar = new ProgressBar(this,
        max != 0 ? static_cast<float>(PlayerInfo::getAttribute(
        Attributes::PLAYER_MAX_MP)) / static_cast<float>(max)
        : static_cast<float>(0),
        80,
        0,
        useMagic ? ProgressColorId::PROG_MP : ProgressColorId::PROG_NO_MP,
        useMagic ? "mpprogressbar.xml" : "nompprogressbar.xml",
        useMagic ? "mpprogressbar_fill.xml" : "nompprogressbar_fill.xml");
    mMpBar->setSelectable(false);
    if (useMagic)
    {
        mMpBar->setColor(getThemeColor(ThemeColorId::MP_BAR),
            getThemeColor(ThemeColorId::MP_BAR_OUTLINE));
    }
    else
    {
        mMpBar->setColor(getThemeColor(ThemeColorId::NO_MP_BAR),
            getThemeColor(ThemeColorId::NO_MP_BAR_OUTLINE));
    }

    place(0, 0, mLvlLabel, 3);
    place(0, 1, mHpLabel).setPadding(3);
    place(1, 1, mHpBar, 4);
    place(5, 1, mXpLabel).setPadding(3);
    place(6, 1, mXpBar, 5);
    place(0, 2, mMpLabel).setPadding(3);
    // 5, 2 and 6, 2 Job Progress Bar
    if (job)
        place(1, 2, mMpBar, 4);
    else
        place(1, 2, mMpBar, 10);

    if (job)
    {
        // TRANSLATORS: status window label
        mJobLvlLabel = new Label(this, strprintf(_("Job: %d"), 0));
        // TRANSLATORS: status window label
        mJobLabel = new Label(this, _("Job:"));
        mJobBar = new ProgressBar(this, 0.0F, 80, 0, ProgressColorId::PROG_JOB,
            "jobprogressbar.xml", "jobprogressbar_fill.xml");
        mJobBar->setColor(getThemeColor(ThemeColorId::JOB_BAR),
            getThemeColor(ThemeColorId::JOB_BAR_OUTLINE));
        mJobBar->setSelectable(false);

        place(3, 0, mJobLvlLabel, 3);
        place(5, 2, mJobLabel).setPadding(3);
        place(6, 2, mJobBar, 5);
        place(6, 0, mMoneyLabel, 3);
    }
    else
    {
        mJobLvlLabel = nullptr;
        mJobLabel = nullptr;
        mJobBar = nullptr;
        place(3, 0, mMoneyLabel, 3);
    }

    place(0, 3, mTabs, 11, 3);

    getLayout().setRowHeight(3, LayoutType::SET);

    place(0, 5, mCopyButton);

    loadWindowState();
    enableVisibleSound(true);

    // Update bars
    updateHPBar(mHpBar, true);
    updateMPBar(mMpBar, true);
    updateXPBar(mXpBar, false);

    // TRANSLATORS: status window label
    mMoneyLabel->setCaption(strprintf(_("Money: %s"),
        UnitsDb::formatCurrency(PlayerInfo::getAttribute(
        Attributes::MONEY)).c_str()));
    mMoneyLabel->adjustSize();

    updateLevelLabel();
    addTabs();
}

StatusWindow::~StatusWindow()
{
    delete2(mBasicStatsPage);
    delete_all(mPages);
}

void StatusWindow::addTabs()
{
    // TRANSLATORS: status window tab name
    addTabBasic(_("Basic"));
    const STD_VECTOR<std::string> &pages = StatDb::getPages();
    FOR_EACH(STD_VECTOR<std::string>::const_iterator, it, pages)
    {
        addTab(*it);
    }
    mTabs->adjustSize();
}

void StatusWindow::addTab(const std::string &name)
{
    StatsPage *const page = new StatsPage(this, name);
    mTabs->addTab(name,
        page);
    mPages.push_back(page);
}

void StatusWindow::addTabBasic(const std::string &name)
{
    mTabs->addTab(name,
        mBasicStatsPage);
}

void StatusWindow::updateLevelLabel()
{
    if (localPlayer == nullptr)
        return;

    const int groupId = localPlayer->getGroupId();
    const std::string &name = GroupDb::getName(groupId);
    if (!name.empty())
    {
        // TRANSLATORS: status window label
        mLvlLabel->setCaption(strprintf(_("Level: %d (%s %d)"),
            PlayerInfo::getAttribute(Attributes::PLAYER_BASE_LEVEL),
            name.c_str(),
            groupId));
    }
    else
    {
        // TRANSLATORS: status window label
        mLvlLabel->setCaption(strprintf(_("Level: %d"),
            PlayerInfo::getAttribute(Attributes::PLAYER_BASE_LEVEL)));
    }
    mLvlLabel->adjustSize();
}

void StatusWindow::statChanged(const AttributesT id,
                               const int oldVal1,
                               const int oldVal2 A_UNUSED)
{
    static bool blocked = false;
    if (blocked)
        return;

    if (id == Attributes::PLAYER_JOB)
    {
        if (mJobLvlLabel != nullptr)
        {
            int lvl = PlayerInfo::getStatBase(id);
            const int oldExp = oldVal1;
            const std::pair<int, int> exp = PlayerInfo::getStatExperience(id);

            if (lvl == 0)
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

            // TRANSLATORS: status window label
            mJobLvlLabel->setCaption(strprintf(_("Job: %d"), lvl));
            mJobLvlLabel->adjustSize();

            updateJobBar(mJobBar, false);
        }
    }
    else
    {
        updateMPBar(mMpBar, true);
    }
}

void StatusWindow::attributeChanged(const AttributesT id,
                                    const int64_t oldVal A_UNUSED,
                                    const int64_t newVal)
{
    PRAGMA45(GCC diagnostic push)
    PRAGMA45(GCC diagnostic ignored "-Wswitch-enum")
    switch (id)
    {
        case Attributes::PLAYER_HP:
        case Attributes::PLAYER_MAX_HP:
            updateHPBar(mHpBar, true);
            break;

        case Attributes::PLAYER_MP:
        case Attributes::PLAYER_MAX_MP:
            updateMPBar(mMpBar, true);
            break;

        case Attributes::PLAYER_EXP:
        case Attributes::PLAYER_EXP_NEEDED:
            updateXPBar(mXpBar, false);
            break;

        case Attributes::MONEY:
            // TRANSLATORS: status window label
            mMoneyLabel->setCaption(strprintf(_("Money: %s"),
                UnitsDb::formatCurrency(newVal).c_str()));
            mMoneyLabel->adjustSize();
            break;

        case Attributes::PLAYER_BASE_LEVEL:
            // TRANSLATORS: status window label
            mLvlLabel->setCaption(strprintf(_("Level: %d"),
                CAST_S32(newVal)));
            mLvlLabel->adjustSize();
            break;

        default:
            break;
    }
    PRAGMA45(GCC diagnostic pop)
}

void StatusWindow::setPointsNeeded(const AttributesT id,
                                   const int needed)
{
    mBasicStatsPage->setPointsNeeded(id, needed);
}

void StatusWindow::updateHPBar(ProgressBar *const bar, const bool showMax)
{
    if (bar == nullptr)
        return;

    const int hp = PlayerInfo::getAttribute(Attributes::PLAYER_HP);
    const int maxHp = PlayerInfo::getAttribute(Attributes::PLAYER_MAX_HP);
    if (showMax)
        bar->setText(toString(hp).append("/").append(toString(maxHp)));
    else
        bar->setText(toString(hp));

    float prog = 1.0;
    if (maxHp > 0)
        prog = static_cast<float>(hp) / static_cast<float>(maxHp);
    bar->setProgress(prog);
}

void StatusWindow::updateMPBar(ProgressBar *const bar,
                               const bool showMax) const
{
    if (bar == nullptr)
        return;

    const int mp = PlayerInfo::getAttribute(Attributes::PLAYER_MP);
    const int maxMp = PlayerInfo::getAttribute(Attributes::PLAYER_MAX_MP);
    if (showMax)
        bar->setText(toString(mp).append("/").append(toString(maxMp)));
    else
        bar->setText(toString(mp));

    float prog = 1.0F;
    if (maxMp > 0)
        prog = static_cast<float>(mp) / static_cast<float>(maxMp);

    if (playerHandler->canUseMagic())
    {
        bar->setColor(getThemeColor(ThemeColorId::MP_BAR),
            getThemeColor(ThemeColorId::MP_BAR_OUTLINE));
        bar->setProgressPalette(ProgressColorId::PROG_MP);
    }
    else
    {
        bar->setColor(getThemeColor(ThemeColorId::NO_MP_BAR),
            getThemeColor(ThemeColorId::NO_MP_BAR_OUTLINE));
        bar->setProgressPalette(ProgressColorId::PROG_NO_MP);
    }

    bar->setProgress(prog);
}

void StatusWindow::updateProgressBar(ProgressBar *const bar,
                                     const int64_t value,
                                     const int64_t max,
                                     const bool percent)
{
    if (bar == nullptr)
        return;

    if (max == 0)
    {
        // TRANSLATORS: status bar label
        bar->setText(_("Max"));
        bar->setProgress(1);
        bar->setText(toString(CAST_U64(value)));
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
            bar->setText(toString(
                CAST_U64(value)).append(
                "/").append(toString(
                CAST_U64(max))));
        }
        bar->setProgress(progress);
    }
}

void StatusWindow::updateXPBar(ProgressBar *const bar, const bool percent)
{
    if (bar == nullptr)
        return;

    updateProgressBar(bar, PlayerInfo::getAttribute(Attributes::PLAYER_EXP),
        PlayerInfo::getAttribute(Attributes::PLAYER_EXP_NEEDED), percent);
}

void StatusWindow::updateJobBar(ProgressBar *const bar, const bool percent)
{
    if (bar == nullptr)
        return;

    const std::pair<int, int> exp =  PlayerInfo::getStatExperience(
        Attributes::PLAYER_JOB);
    updateProgressBar(bar, exp.first, exp.second, percent);
}

void StatusWindow::updateProgressBar(ProgressBar *const bar,
                                     const AttributesT id,
                                     const bool percent)
{
    const std::pair<int, int> exp =  PlayerInfo::getStatExperience(id);
    updateProgressBar(bar, exp.first, exp.second, percent);
}

void StatusWindow::updateWeightBar(ProgressBar *const bar)
{
    if (bar == nullptr)
        return;

    if (PlayerInfo::getAttribute(Attributes::MAX_WEIGHT) == 0)
    {
        // TRANSLATORS: status bar label
        bar->setText(_("Max"));
        bar->setProgress(1.0);
    }
    else
    {
        const int totalWeight = PlayerInfo::getAttribute(
            Attributes::TOTAL_WEIGHT);
        const int maxWeight = PlayerInfo::getAttribute(Attributes::MAX_WEIGHT);
        float progress = 1.0F;
        if (maxWeight != 0)
        {
            progress = static_cast<float>(totalWeight)
                / static_cast<float>(maxWeight);
        }
        bar->setText(strprintf("%s/%s",
            UnitsDb::formatWeight(totalWeight).c_str(),
            UnitsDb::formatWeight(maxWeight).c_str()));
        bar->setProgress(progress);
    }
}

void StatusWindow::updateMoneyBar(ProgressBar *const bar)
{
    if (bar == nullptr)
        return;

    const int money = PlayerInfo::getAttribute(Attributes::MONEY);
    bar->setText(UnitsDb::formatCurrency(money));
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
    if ((bar == nullptr) || (equipmentWindow == nullptr))
        return;

    const Item *const item = equipmentWindow->getEquipment(
        inventoryHandler->getProjectileSlot());

    if ((item != nullptr) && item->getQuantity() > 0)
        bar->setText(toString(item->getQuantity()));
    else
        bar->setText("0");
}

void StatusWindow::updateInvSlotsBar(ProgressBar *const bar)
{
    if (bar == nullptr)
        return;

    const Inventory *const inv = PlayerInfo::getInventory();
    if (inv == nullptr)
        return;

    const int usedSlots = inv->getNumberOfSlotsUsed();
    const int maxSlots = inv->getSize();

    if (maxSlots != 0)
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
    if ((str == nullptr) || strlen(str) != 3)
        return letters;

    buf[0] = str[1];
    buf[1] = 0;
    return std::string(buf);
}

std::string StatusWindow::translateLetter2(const std::string &letters)
{
    if (letters.size() < 5)
        return "";

    return std::string(gettext(letters.substr(1, 1).c_str()));
}

void StatusWindow::updateStatusBar(ProgressBar *const bar,
                                   const bool percent A_UNUSED) const
{
    if (bar == nullptr)
        return;
    bar->setText(translateLetter2(GameModifiers::getMoveTypeString())
        .append(translateLetter2(GameModifiers::getCrazyMoveTypeString()))
        .append(translateLetter2(GameModifiers::getMoveToTargetTypeString()))
        .append(translateLetter2(GameModifiers::getFollowModeString()))
        .append(" ").append(translateLetter2(
        GameModifiers::getAttackWeaponTypeString()))
        .append(translateLetter2(GameModifiers::getAttackTypeString()))
        .append(translateLetter2(GameModifiers::getMagicAttackTypeString()))
        .append(translateLetter2(GameModifiers::getPvpAttackTypeString()))
        .append(" ").append(translateLetter2(
        GameModifiers::getQuickDropCounterString()))
        .append(translateLetter2(GameModifiers::getPickUpTypeString()))
        .append(" ").append(translateLetter2(
        GameModifiers::getMapDrawTypeString()))
        .append(" ").append(translateLetter2(
        GameModifiers::getImitationModeString()))
        .append(translateLetter2(GameModifiers::getCameraModeString()))
        .append(translateLetter2(GameModifiers::getAwayModeString()))
        .append(translateLetter2(GameModifiers::getTargetingTypeString())));

    bar->setProgress(50);
    if (settings.disableGameModifiers)
        bar->setBackgroundColor(getThemeColor(ThemeColorId::STATUSBAR_ON));
    else
        bar->setBackgroundColor(getThemeColor(ThemeColorId::STATUSBAR_OFF));
}

void StatusWindow::action(const ActionEvent &event)
{
    if (chatWindow == nullptr)
        return;

    if (event.getId() == "copy")
    {
        chatWindow->addInputText(mBasicStatsPage->getStatsStr());
    }
}
