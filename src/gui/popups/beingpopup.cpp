/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "gui/popups/beingpopup.h"

#include "being/being.h"
#include "being/homunculusinfo.h"
#include "being/petinfo.h"
#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "gui/gui.h"

#include "gui/fonts/font.h"

#include "gui/widgets/label.h"

#include "resources/chatobject.h"

#include "resources/db/groupdb.h"

#include "utils/gettext.h"
#include "utils/foreach.h"
#include "utils/stringutils.h"

#include "debug.h"

BeingPopup *beingPopup = nullptr;

BeingPopup::BeingPopup() :
    Popup("BeingPopup", "beingpopup.xml"),
    mBeingName(new Label(this, "A")),
    mLabels()
{
    // Being Name
    mBeingName->setFont(boldFont);
    mBeingName->setPosition(0, 0);

    const int fontHeight = mBeingName->getHeight();
    setMinHeight(fontHeight);
    addLabels(fontHeight);
}

BeingPopup::~BeingPopup()
{
}

void BeingPopup::postInit()
{
    Popup::postInit();
    add(mBeingName);
    FOR_EACH (STD_VECTOR<Label*>::iterator, it, mLabels)
    {
        add(*it);
    }
}

void BeingPopup::addLabels(const int fontHeight)
{
    for (int f = 0; f < 11; f ++)
    {
        Label *const label = new Label(this, "A");
        label->setPosition(0, fontHeight * (f + 1));
        label->setForegroundColorAll(
            getThemeColor(ThemeColorId::POPUP, 255U),
            getThemeColor(ThemeColorId::POPUP_OUTLINE, 255U));
        mLabels.push_back(label);
    }
}

void BeingPopup::show(const int x, const int y, Being *const b)
{
    if (b == nullptr)
    {
        setVisible(Visible_false);
        return;
    }

    int num = 0;
    Label *ptr = nullptr;
    b->updateComment();

    if (b->getType() == ActorType::Npc && b->getComment().empty())
    {
        setVisible(Visible_false);
        return;
    }

    mBeingName->setCaption(b->getName() + b->getGenderSignWithSpace());
    if (gui != nullptr)
    {
        if (playerRelations.isGoodName(b))
            mBeingName->setFont(boldFont);
        else
            mBeingName->setFont(gui->getSecureFont());
    }
    if (b->isAdvanced())
    {
        mBeingName->setForegroundColorAll(
            getThemeColor(ThemeColorId::PLAYER_ADVANCED, 255U),
            getThemeColor(ThemeColorId::PLAYER_ADVANCED_OUTLINE, 255U));
    }
    else
    {
        mBeingName->setForegroundColorAll(
            getThemeColor(ThemeColorId::POPUP, 255U),
            getThemeColor(ThemeColorId::POPUP_OUTLINE, 255U));
    }

    mBeingName->adjustSize();
    FOR_EACH (STD_VECTOR<Label*>::iterator, it, mLabels)
    {
        (*it)->setCaption(std::string());
    }

    const ActorTypeT type = b->getType();
    if (type == ActorType::Pet)
    {
        const PetInfo *const info = PlayerInfo::getPet();
        if ((info != nullptr) && info->id == b->getId())
        {
            ptr = mLabels[num];
            // TRANSLATORS: being popup label
            ptr->setCaption(strprintf(_("Hungry: %d"),
                info->hungry));
            ptr->adjustSize();
            num ++;
            ptr = mLabels[num];
            // TRANSLATORS: being popup label
            ptr->setCaption(strprintf(_("Intimacy: %d"),
                info->intimacy));
            ptr->adjustSize();
            num ++;
        }
    }
    else if (type == ActorType::Homunculus)
    {
        const HomunculusInfo *const info = PlayerInfo::getHomunculus();
        if ((info != nullptr) && info->id == b->getId())
        {
            ptr = mLabels[num];
            // TRANSLATORS: being popup label
            ptr->setCaption(strprintf(_("Hungry: %d"),
                info->hungry));
            ptr->adjustSize();
            num ++;
            ptr = mLabels[num];
            // TRANSLATORS: being popup label
            ptr->setCaption(strprintf(_("Intimacy: %d"),
                info->intimacy));
            ptr->adjustSize();
            num ++;
        }
    }
    else
    {
        const int groupId = b->getGroupId();
        const std::string &groupName = GroupDb::getLongName(groupId);
        if (!groupName.empty())
        {
            ptr = mLabels[num];
            // TRANSLATORS: being popup label
            ptr->setCaption(strprintf(_("Group: %s (%d)"),
                groupName.c_str(),
                groupId));
            ptr->adjustSize();
            num ++;
        }

        if (!(b->getPartyName().empty()))
        {
            ptr = mLabels[num];
            // TRANSLATORS: being popup label
            ptr->setCaption(strprintf(_("Party: %s"),
                b->getPartyName().c_str()));
            ptr->adjustSize();
            num ++;
        }

        if (!(b->getGuildName().empty()))
        {
            ptr = mLabels[num];
            // TRANSLATORS: being popup label
            ptr->setCaption(strprintf(_("Guild: %s"),
                b->getGuildName().c_str()));
            ptr->adjustSize();
            num ++;
        }

        if (b->getPvpRank() > 0)
        {
            ptr = mLabels[num];
            // TRANSLATORS: being popup label
            ptr->setCaption(strprintf(_("Pvp rank: %u"),
                b->getPvpRank()));
            ptr->adjustSize();
            num ++;
        }

        if (!b->getBuyBoard().empty())
        {
            ptr = mLabels[num];
            // TRANSLATORS: being popup label
            ptr->setCaption(strprintf(_("Buy shop: %s"),
                b->getBuyBoard().c_str()));
            ptr->adjustSize();
            num ++;
        }

        if (!b->getSellBoard().empty())
        {
            ptr = mLabels[num];
            // TRANSLATORS: being popup label
            ptr->setCaption(strprintf(_("Sell shop: %s"),
                b->getSellBoard().c_str()));
            ptr->adjustSize();
            num ++;
        }

        if (!b->getComment().empty())
        {
            ptr = mLabels[num];
            // TRANSLATORS: being popup label
            ptr->setCaption(strprintf(_("Comment: %s"),
                b->getComment().c_str()));
            ptr->adjustSize();
            num ++;
        }

        const std::string effects = b->getStatusEffectsString();
        if (!effects.empty())
        {
            ptr = mLabels[num];
            // TRANSLATORS: being popup label
            ptr->setCaption(strprintf(_("Effects: %s"),
                effects.c_str()));
            ptr->adjustSize();
            num ++;
        }

        const ChatObject *const chat = b->getChat();
        if (chat != nullptr)
        {
            ptr = mLabels[num];
            // TRANSLATORS: being popup label
            ptr->setCaption(strprintf(_("Chat room: %s"),
                chat->title.c_str()));
            ptr->adjustSize();
            num ++;
        }
    }

    const int level = b->getLevel();
    if (level > 1)
    {
        ptr = mLabels[num];
        // TRANSLATORS: being popup label
        ptr->setCaption(strprintf(_("Level: %d"),
            level));
        ptr->adjustSize();
        num ++;
    }

    const int maxHp = b->getMaxHP();
    if (maxHp > 0)
    {
        int hp = b->getHP();
        if (hp == 0)
            hp = maxHp - b->getDamageTaken();
        if (hp > 0)
        {
            ptr = mLabels[num];
            // TRANSLATORS: being popup label
            ptr->setCaption(strprintf(_("Hp: %d/%d"),
                hp,
                maxHp));
            ptr->adjustSize();
            num ++;
        }
    }

    ptr = mLabels[num];
    // TRANSLATORS: being popup label
    ptr->setCaption(strprintf(_("Particles: %u"),
        CAST_U32(b->getParticlesCount())));
    ptr->adjustSize();
    num ++;

    const size_t sz = mLabels.size();
    for (size_t f = num; f < sz; f ++)
    {
        mLabels[f]->setCaption(std::string());
    }

    int minWidth = mBeingName->getWidth();
    const int height1 = getFont()->getHeight();
    int height = height1;
    FOR_EACH (STD_VECTOR<Label*>::iterator, it, mLabels)
    {
        const Label *const label = *it;
        if (label != nullptr)
        {
            if (label->getWidth() > minWidth)
                minWidth = label->getWidth();
            if (!label->getCaption().empty())
                height += height1;
        }
    }

    setContentSize(minWidth, height);
    position(x, y);
}

#ifdef USE_PROFILER
void BeingPopup::logic()
{
    BLOCK_START("BeingPopup::logic")
    logicChildren();
    BLOCK_END("BeingPopup::logic")
}
#endif  // USE_PROFILER
