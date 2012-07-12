/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#include "gui/widgets/avatarlistbox.h"

#include "actorspritemanager.h"
#include "configuration.h"
#include "graphics.h"
#include "guild.h"
#include "localplayer.h"
#include "maplayer.h"

#include "gui/chatwindow.h"
#include "gui/gui.h"
#include "gui/palette.h"
#include "gui/sdlfont.h"
#include "gui/theme.h"
#include "gui/viewport.h"

#include "resources/image.h"
#include "resources/resourcemanager.h"

#include "utils/stringutils.h"

#include <guichan/font.hpp>

#include "debug.h"

int AvatarListBox::instances = 0;
Image *AvatarListBox::onlineIcon = nullptr;
Image *AvatarListBox::offlineIcon = nullptr;

AvatarListBox::AvatarListBox(AvatarListModel *model):
    ListBox(model),
    mShowGender(false),
    mShowLevel(false)
{
    instances++;

    if (instances == 1)
    {
        onlineIcon = Theme::getImageFromThemeXml("circle-on.xml");
        offlineIcon = Theme::getImageFromThemeXml("circle-off.xml");
    }

    setWidth(200);

    mShowGender = config.getBoolValue("showgender");
    mShowLevel = config.getBoolValue("showlevel");

    config.addListener("showgender", this);
    config.addListener("showlevel", this);

    mHighlightColor = Theme::getThemeColor(Theme::HIGHLIGHT);
    setForegroundColor(Theme::getThemeColor(Theme::TEXT));
}

AvatarListBox::~AvatarListBox()
{
    config.removeListeners(this);

    instances--;

    if (instances == 0)
    {
        if (onlineIcon)
        {
            onlineIcon->decRef();
            onlineIcon = nullptr;
        }
        if (offlineIcon)
        {
            offlineIcon->decRef();
            offlineIcon = nullptr;
        }
    }
}

void AvatarListBox::draw(gcn::Graphics *gcnGraphics)
{
    if (!mListModel || !player_node)
        return;

    AvatarListModel *model = static_cast<AvatarListModel*>(mListModel);
//    Guild *guild = dynamic_cast<Guild*>(model);

    updateAlpha();

    Graphics *graphics = static_cast<Graphics*>(gcnGraphics);

    mHighlightColor.a = static_cast<int>(mAlpha * 255.0f);
    graphics->setColor(mHighlightColor);
    graphics->setFont(getFont());

    const int fontHeight = getFont()->getHeight();

    Widget *parent = mParent;

    const std::string name = player_node->getName();

    // Draw the list elements
    graphics->setColor(getForegroundColor());
    for (int i = 0, y = 0;
         i < model->getNumberOfElements();
         ++i, y += fontHeight)
    {
        Avatar *a = model->getAvatarAt(i);
        if (!a)
            continue;

        if (a->getType() != MapItem::SEPARATOR)
        {
            // Draw online status
            Image *icon = a->getOnline() ? onlineIcon : offlineIcon;
            if (icon)
                graphics->drawImage(icon, 2, y + 1);
        }

        if (a->getDisplayBold())
            graphics->setFont(boldFont);

        std::string text;

        if (a->getMaxHp() > 0)
        {
            if (mShowLevel && a->getLevel() > 1)
            {
                text = strprintf("%s %d/%d (%d)", a->getComplexName().c_str(),
                                 a->getHp(), a->getMaxHp(), a->getLevel());
            }
            else
            {
                text = strprintf("%s %d/%d", a->getComplexName().c_str(),
                                 a->getHp(), a->getMaxHp());
            }
            if (parent && a->getMaxHp())
            {
                gcn::Color color = Theme::getProgressColor(
                        Theme::PROG_HP, static_cast<float>(a->getHp())
                        / static_cast<float>(a->getMaxHp()));
                color.a = 80;
                graphics->setColor(color);

                graphics->fillRectangle(gcn::Rectangle(0, y,
                    parent->getWidth() * a->getHp() / a->getMaxHp(),
                    fontHeight));
            }
        }
        else if (a->getDamageHp() != 0 && a->getName() != name)
        {
            if (mShowLevel && a->getLevel() > 1)
            {
                text = strprintf("%s -%d (%d)", a->getComplexName().c_str(),
                                 a->getDamageHp(), a->getLevel());
            }
            else
            {
                text = strprintf("%s -%d", a->getComplexName().c_str(),
                                 a->getDamageHp());
            }

            if (parent)
            {
//                int diff;
//                if (a->getDamageHp() > 1024)
//                    diff = 0;
//                else
//                    diff = 1024 - a->getDamageHp();
                gcn::Color color = Theme::getProgressColor(Theme::PROG_HP,
                        1);
//                        0 / 1024);
/*
                if (a->getDamageHp() >= 400)
                {
                }
                else
                {
//                    int intens = 1024/(400 - a->getDamageHp());
                    int intens = a->getDamageHp() / 1024;
                    if (intens > 1)
                        intens = 1;
                    color = Theme::getProgressColor(Theme::PROG_HP,
                        intens);
                }
*/

                color.a = 80;
                graphics->setColor(color);
                graphics->fillRectangle(gcn::Rectangle(0, y,
                    parent->getWidth() * a->getDamageHp() / 1024,
                    fontHeight));

                if (a->getLevel() > 1)
                {
                    graphics->setColor(getForegroundColor());
                    int minHp = 40 + ((a->getLevel() - 1) * 5);
                    if (minHp < 0)
                        minHp = 40;

                    graphics->drawLine(parent->getWidth()*minHp / 1024, y,
                        parent->getWidth() * minHp / 1024, y + fontHeight);
                }
            }
        }
        else
        {
            if (mShowLevel && a->getLevel() > 1)
            {
                text = strprintf("%s (%d)", a->getComplexName().c_str(),
                                 a->getLevel());
            }
            else
            {
                text = a->getComplexName();
            }
        }

        if (!a->getMap().empty())
        {
            if (a->getX() != -1)
            {
                text += strprintf(" [%d,%d %s]", a->getX(), a->getY(),
                                  a->getMap().c_str());
            }
            else
            {
                text += strprintf(" [%s]", a->getMap().c_str());
            }
        }

        if (graphics->getSecure())
        {
            if (mShowGender)
            {
                switch (a->getGender())
                {
                    case GENDER_FEMALE:
                        text += strprintf(" \u2640 ");
                        break;
                    case GENDER_MALE:
                        text += strprintf(" \u2642 ");
                        break;
                    default:
                        break;
                }
            }
        }
        else
        {
            if (mShowGender)
            {
                switch (a->getGender())
                {
                    case GENDER_FEMALE:
                        text += strprintf(" \u2640 %s",
                            a->getAdditionString().c_str());
                        break;
                    case GENDER_MALE:
                        text += strprintf(" \u2642 %s",
                            a->getAdditionString().c_str());
                        break;
                    default:
                        break;
                }
            }
            else
            {
                text += a->getAdditionString();
            }
        }

        graphics->setColor(getForegroundColor());

        // Draw Name
        if (a->getType() == MapItem::SEPARATOR)
            graphics->drawText(text, 2, y);
        else
            graphics->drawText(text, 15, y);

        if (a->getDisplayBold())
            graphics->setFont(getFont());
    }

    setWidth(parent->getWidth() - 10);
}

void AvatarListBox::mousePressed(gcn::MouseEvent &event)
{
    if (!actorSpriteManager || !player_node || !viewport
        || !getFont()->getHeight())
    {
        return;
    }

    int y = event.getY() / getFont()->getHeight();
    if (!mListModel || y > mListModel->getNumberOfElements())
        return;

    setSelected(y);
    distributeActionEvent();
    int selected = getSelected();
    AvatarListModel *model = static_cast<AvatarListModel*>(mListModel);
    if (!model)
        return;
    Avatar *ava = model->getAvatarAt(selected);
    if (!ava)
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        if (ava->getType() == AVATAR_PLAYER)
        {
            Being* being = actorSpriteManager->findBeingByName(ava->getName(),
                        Being::PLAYER);
            if (being)
                actorSpriteManager->heal(being);
        }
        else
        {
            player_node->navigateTo(ava->getX(), ava->getY());
        }
    }
    else if (event.getButton() == gcn::MouseEvent::RIGHT)
    {
        switch (ava->getType())
        {
            case AVATAR_PLAYER:
            {
                Avatar *avatar = model->getAvatarAt(selected);
                if (avatar)
                {
                    Being* being = actorSpriteManager->findBeingByName(
                        avatar->getName(), Being::PLAYER);
                    if (being)
                        viewport->showPopup(being);
                    else
                        viewport->showPlayerPopup(avatar->getName());
                }
                break;
            }
            case MapItem::ATTACK:
            case MapItem::PRIORITY:
            case MapItem::IGNORE_:
            {
                std::string name;
                if (model->getAvatarAt(selected)->getLevel() == 0)
                    name = "";
                else
                    name = model->getAvatarAt(selected)->getName();

                viewport->showAttackMonsterPopup(name,
                    model->getAvatarAt(selected)->getType());
                break;
            }
            case MapItem::PICKUP:
            case MapItem::NOPICKUP:
            {
                std::string name;
                if (model->getAvatarAt(selected)->getLevel() == 0)
                    name = "";
                else
                    name = model->getAvatarAt(selected)->getName();

                viewport->showPickupItemPopup(name);
                break;
            }
            default:
            {
                Map *map = viewport->getMap();
                ava = model->getAvatarAt(selected);
                if (map && ava)
                {
                    MapItem *mapItem = map->findPortalXY(
                        ava->getX(), ava->getY());
                    viewport->showPopup(mapItem);
                }
                break;
            }
        }
    }
    else if (event.getButton() == gcn::MouseEvent::MIDDLE)
    {
        if (ava->getType() == AVATAR_PLAYER && chatWindow)
        {
            ChatTab *tab = chatWindow->addWhisperTab(
                model->getAvatarAt(selected)->getName(), true);
            if (chatWindow && tab)
                chatWindow->saveState();
        }
    }
}

void AvatarListBox::optionChanged(const std::string &value)
{
    if (value == "showgender")
        mShowGender = config.getBoolValue("showgender");
    else if (value == "showlevel")
        mShowLevel = config.getBoolValue("showlevel");
}
