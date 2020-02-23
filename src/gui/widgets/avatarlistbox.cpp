/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "actormanager.h"
#include "configuration.h"

#include "being/localplayer.h"

#include "enums/resources/map/mapitemtype.h"

#include "gui/gui.h"
#include "gui/popupmanager.h"
#include "gui/skin.h"
#include "gui/viewport.h"

#include "gui/fonts/font.h"

#include "gui/popups/popupmenu.h"

#include "gui/models/avatarlistmodel.h"

#include "gui/windows/chatwindow.h"

#include "render/vertexes/imagecollection.h"

#include "resources/image/image.h"

#include "resources/map/map.h"

#include "debug.h"

int AvatarListBox::instances = 0;
Image *AvatarListBox::onlineIcon = nullptr;
Image *AvatarListBox::offlineIcon = nullptr;

AvatarListBox::AvatarListBox(const Widget2 *const widget,
                             AvatarListModel *const model) :
    ListBox(widget, model, "avatarlistbox.xml"),
    mImagePadding(mSkin != nullptr ? mSkin->getOption("imagePadding") : 0),
    mShowGender(config.getBoolValue("showgender")),
    mShowLevel(config.getBoolValue("showlevel"))
{
    instances++;

    if (instances == 1)
    {
        onlineIcon = Theme::getImageFromThemeXml("circle-on.xml", "");
        offlineIcon = Theme::getImageFromThemeXml("circle-off.xml", "");
    }

    setWidth(200);

    config.addListener("showgender", this);
    config.addListener("showlevel", this);

    mForegroundColor = getThemeColor(ThemeColorId::TEXT, 255U);
    mForegroundColor2 = getThemeColor(ThemeColorId::TEXT_OUTLINE, 255U);
}

AvatarListBox::~AvatarListBox()
{
    config.removeListeners(this);
    CHECKLISTENERS

    instances--;

    if (instances == 0)
    {
        if (onlineIcon != nullptr)
        {
            onlineIcon->decRef();
            onlineIcon = nullptr;
        }
        if (offlineIcon != nullptr)
        {
            offlineIcon->decRef();
            offlineIcon = nullptr;
        }
    }
}

void AvatarListBox::draw(Graphics *const graphics)
{
    BLOCK_START("AvatarListBox::draw")
    if ((mListModel == nullptr) || (localPlayer == nullptr))
    {
        BLOCK_END("AvatarListBox::draw")
        return;
    }

    const Widget *const parent = mParent;
    if (parent == nullptr)
        return;

    AvatarListModel *const model = static_cast<AvatarListModel *>(
        mListModel);
    updateAlpha();

    Font *const font = getFont();
    const int fontHeight = font->getHeight();
    const int parentWidth = parent->getWidth();
    const std::string &name = localPlayer->getName();

    // Draw the list elements
    ImageCollection vertexes;
    const int num = model->getNumberOfElements();
    for (int i = 0, y = 0;
         i < num;
         ++i, y += fontHeight)
    {
        const Avatar *const a = model->getAvatarAt(i);
        if (a == nullptr)
            continue;

        const MapItemType::Type type = static_cast<MapItemType::Type>(
            a->getType());
        if (type != MapItemType::SEPARATOR)
        {
            // Draw online status
            const Image *const icon = a->getOnline()
                ? onlineIcon : offlineIcon;
            if (icon != nullptr)
            {
                graphics->calcTileCollection(&vertexes, icon,
                    mImagePadding, y + mPadding);
            }
        }
    }

    graphics->finalize(&vertexes);
    graphics->drawTileCollection(&vertexes);

    for (int i = 0, y = 0;
         i < num;
         ++i, y += fontHeight)
    {
        const Avatar *const a = model->getAvatarAt(i);
        if (a == nullptr)
            continue;

        const MapItemType::Type type = static_cast<MapItemType::Type>(
            a->getType());
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
            if (a->getMaxHp() != 0)
            {
                const bool isPoison = a->getPoison();
                const ProgressColorIdT themeColor = (isPoison
                    ? ProgressColorId::PROG_HP_POISON
                    : ProgressColorId::PROG_HP);
                Color color = Theme::getProgressColor(
                    themeColor, static_cast<float>(a->getHp())
                    / static_cast<float>(a->getMaxHp()));
                color.a = 80;
                graphics->setColor(color);
                graphics->fillRectangle(Rect(mPadding, y + mPadding,
                    parentWidth * a->getHp() / a->getMaxHp()
                    - 2 * mPadding, fontHeight));
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

            const ProgressColorIdT themeColor = (a->getPoison()
                ? ProgressColorId::PROG_HP_POISON : ProgressColorId::PROG_HP);
            Color color = Theme::getProgressColor(themeColor, 1);
            color.a = 80;
            graphics->setColor(color);
            graphics->fillRectangle(Rect(mPadding, y + mPadding,
                parentWidth * a->getDamageHp() / 1024
                - 2 * mPadding, fontHeight));

            if (a->getLevel() > 1)
            {
                graphics->setColor(mForegroundColor);
                int minHp = 40 + ((a->getLevel() - 1) * 5);
                if (minHp < 0)
                    minHp = 40;

                graphics->drawLine(parentWidth * minHp / 1024
                    + mPadding, y + mPadding,
                    parentWidth * minHp / 1024, y + fontHeight);
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
                text.append(strprintf(" [%d,%d %s]", a->getX(), a->getY(),
                    a->getMap().c_str()));
            }
            else
            {
                text.append(strprintf(" [%s]", a->getMap().c_str()));
            }
        }

        if (graphics->getSecure())
        {
            if (mShowGender)
            {
                switch (a->getGender())
                {
                    case Gender::FEMALE:
                        text.append(" \u2640 ");
                        break;
                    case Gender::MALE:
                        text.append(" \u2642 ");
                        break;
                    default:
                    case Gender::UNSPECIFIED:
                    case Gender::HIDDEN:
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
                    case Gender::FEMALE:
                        text.append(strprintf(" \u2640 %s",
                            a->getAdditionString().c_str()));
                        break;
                    case Gender::MALE:
                        text.append(strprintf(" \u2642 %s",
                            a->getAdditionString().c_str()));
                        break;
                    default:
                    case Gender::UNSPECIFIED:
                    case Gender::HIDDEN:
                        break;
                }
            }
            else
            {
                text.append(a->getAdditionString());
            }
        }

        // Draw Name
        if (a->getDisplayBold())
        {
            if (type == MapItemType::SEPARATOR)
            {
                boldFont->drawString(graphics,
                    mForegroundColor,
                    mForegroundColor,
                    text,
                    mImagePadding + mPadding,
                    y + mPadding);
            }
            else
            {
                boldFont->drawString(graphics,
                    mForegroundColor,
                    mForegroundColor,
                    text,
                    15 + mImagePadding + mPadding,
                    y + mPadding);
            }
        }
        else
        {
            if (type == MapItemType::SEPARATOR)
            {
                font->drawString(graphics,
                    mForegroundColor,
                    mForegroundColor,
                    text,
                    mImagePadding + mPadding,
                    y + mPadding);
            }
            else
            {
                font->drawString(graphics,
                    mForegroundColor,
                    mForegroundColor,
                    text,
                    15 + mImagePadding + mPadding,
                    y + mPadding);
            }
        }
    }

    setWidth(parentWidth - 10);
    BLOCK_END("AvatarListBox::draw")
}

void AvatarListBox::safeDraw(Graphics *const graphics)
{
    BLOCK_START("AvatarListBox::draw")
    if ((mListModel == nullptr) || (localPlayer == nullptr))
    {
        BLOCK_END("AvatarListBox::draw")
        return;
    }

    const Widget *const parent = mParent;
    if (parent == nullptr)
        return;

    AvatarListModel *const model = static_cast<AvatarListModel *>(
        mListModel);
    updateAlpha();

    Font *const font = getFont();
    const int fontHeight = font->getHeight();
    const int parentWidth = parent->getWidth();
    const std::string &name = localPlayer->getName();

    // Draw the list elements
    const int num = model->getNumberOfElements();
    for (int i = 0, y = 0;
         i < num;
         ++i, y += fontHeight)
    {
        const Avatar *const a = model->getAvatarAt(i);
        if (a == nullptr)
            continue;

        const MapItemType::Type type = static_cast<MapItemType::Type>(
            a->getType());
        if (type != MapItemType::SEPARATOR)
        {
            // Draw online status
            const Image *const icon = a->getOnline()
                ? onlineIcon : offlineIcon;
            if (icon != nullptr)
                graphics->drawImage(icon, mImagePadding, y + mPadding);
        }
    }

    for (int i = 0, y = 0;
         i < model->getNumberOfElements();
         ++i, y += fontHeight)
    {
        const Avatar *const a = model->getAvatarAt(i);
        if (a == nullptr)
            continue;

        const MapItemType::Type type = static_cast<MapItemType::Type>(
            a->getType());
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
            if (a->getMaxHp() != 0)
            {
                const bool isPoison = a->getPoison();
                const ProgressColorIdT themeColor = (isPoison
                    ? ProgressColorId::PROG_HP_POISON
                    : ProgressColorId::PROG_HP);
                Color color = Theme::getProgressColor(
                    themeColor, static_cast<float>(a->getHp())
                    / static_cast<float>(a->getMaxHp()));
                color.a = 80;
                graphics->setColor(color);
                graphics->fillRectangle(Rect(mPadding, y + mPadding,
                    parentWidth * a->getHp() / a->getMaxHp()
                    - 2 * mPadding, fontHeight));
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

            const ProgressColorIdT themeColor = (a->getPoison()
                ? ProgressColorId::PROG_HP_POISON : ProgressColorId::PROG_HP);
            Color color = Theme::getProgressColor(themeColor, 1);
            color.a = 80;
            graphics->setColor(color);
            graphics->fillRectangle(Rect(mPadding, y + mPadding,
                parentWidth * a->getDamageHp() / 1024
                - 2 * mPadding, fontHeight));

            if (a->getLevel() > 1)
            {
                graphics->setColor(mForegroundColor);
                int minHp = 40 + ((a->getLevel() - 1) * 5);
                if (minHp < 0)
                    minHp = 40;

                graphics->drawLine(parentWidth * minHp / 1024
                    + mPadding, y + mPadding,
                    parentWidth * minHp / 1024, y + fontHeight);
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
                text.append(strprintf(" [%d,%d %s]", a->getX(), a->getY(),
                    a->getMap().c_str()));
            }
            else
            {
                text.append(strprintf(" [%s]", a->getMap().c_str()));
            }
        }

        if (graphics->getSecure())
        {
            if (mShowGender)
            {
                switch (a->getGender())
                {
                    case Gender::FEMALE:
                        text.append(" \u2640 ");
                        break;
                    case Gender::MALE:
                        text.append(" \u2642 ");
                        break;
                    default:
                    case Gender::UNSPECIFIED:
                    case Gender::HIDDEN:
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
                    case Gender::FEMALE:
                        text.append(strprintf(" \u2640 %s",
                            a->getAdditionString().c_str()));
                        break;
                    case Gender::MALE:
                        text.append(strprintf(" \u2642 %s",
                            a->getAdditionString().c_str()));
                        break;
                    default:
                    case Gender::UNSPECIFIED:
                    case Gender::HIDDEN:
                        break;
                }
            }
            else
            {
                text.append(a->getAdditionString());
            }
        }

        // Draw Name
        if (a->getDisplayBold())
        {
            if (type == MapItemType::SEPARATOR)
            {
                boldFont->drawString(graphics,
                    mForegroundColor,
                    mForegroundColor,
                    text,
                    mImagePadding + mPadding,
                    y + mPadding);
            }
            else
            {
                boldFont->drawString(graphics,
                    mForegroundColor,
                    mForegroundColor,
                    text,
                    15 + mImagePadding + mPadding,
                    y + mPadding);
            }
        }
        else
        {
            if (type == MapItemType::SEPARATOR)
            {
                font->drawString(graphics,
                    mForegroundColor,
                    mForegroundColor,
                    text,
                    mImagePadding + mPadding,
                    y + mPadding);
            }
            else
            {
                font->drawString(graphics,
                    mForegroundColor,
                    mForegroundColor,
                    text,
                    15 + mImagePadding + mPadding,
                    y + mPadding);
            }
        }
    }

    setWidth(parentWidth - 10);
    BLOCK_END("AvatarListBox::draw")
}

void AvatarListBox::mousePressed(MouseEvent &event)
{
    if (actorManager == nullptr ||
        localPlayer == nullptr ||
        popupManager == nullptr)
    {
        return;
    }

    const int height = getFont()->getHeight();
    if (height == 0)
        return;

    const int y = (event.getY() - mPadding) / height;
    if ((mListModel == nullptr) || y > mListModel->getNumberOfElements())
        return;

    setSelected(y);
    distributeActionEvent();
    const int selected = getSelected();
    AvatarListModel *const model = static_cast<AvatarListModel *>(
        mListModel);
    if (model == nullptr)
        return;
    const Avatar *ava = model->getAvatarAt(selected);
    if (ava == nullptr)
        return;

    const MapItemType::Type type = static_cast<MapItemType::Type>(
        ava->getType());

    event.consume();
    const MouseButtonT eventButton = event.getButton();
    if (eventButton == MouseButton::LEFT)
    {
#ifdef TMWA_SUPPORT
        if (type == MapItemType::EMPTY)
        {
            const Being *const being = actorManager->findBeingByName(
                ava->getName(), ActorType::Player);
            if (being != nullptr)
                actorManager->heal(being);
        }
        else
#endif  // TMWA_SUPPORT
        {
            localPlayer->navigateTo(ava->getX(), ava->getY());
        }
    }
    else if (eventButton == MouseButton::RIGHT)
    {
        switch (type)
        {
            case MapItemType::EMPTY:
            {
                const Avatar *const avatar = model->getAvatarAt(selected);
                if (avatar != nullptr)
                {
                    const Being *const being = actorManager->findBeingByName(
                        avatar->getName(), ActorType::Player);
                    if (being != nullptr)
                    {
                        popupMenu->showPopup(viewport->mMouseX,
                            viewport->mMouseY,
                            being);
                    }
                    else
                    {
                        popupMenu->showPlayerPopup(avatar->getName());
                    }
                }
                break;
            }
            case MapItemType::ATTACK:
            case MapItemType::PRIORITY:
            case MapItemType::IGNORE_:
            {
                std::string name;
                if (model->getAvatarAt(selected)->getLevel() == 0)
                    name.clear();
                else
                    name = model->getAvatarAt(selected)->getName();

                popupMenu->showAttackMonsterPopup(viewport->mMouseX,
                    viewport->mMouseY,
                    name,
                    model->getAvatarAt(selected)->getType());
                break;
            }
            case MapItemType::PICKUP:
            case MapItemType::NOPICKUP:
            {
                std::string name;
                if (model->getAvatarAt(selected)->getLevel() == 0)
                    name.clear();
                else
                    name = model->getAvatarAt(selected)->getName();

                popupMenu->showPickupItemPopup(viewport->mMouseX,
                    viewport->mMouseY,
                    name);
                break;
            }
            case MapItemType::HOME:
            case MapItemType::ROAD:
            case MapItemType::CROSS:
            case MapItemType::ARROW_UP:
            case MapItemType::ARROW_DOWN:
            case MapItemType::ARROW_LEFT:
            case MapItemType::ARROW_RIGHT:
            case MapItemType::PORTAL:
            case MapItemType::MUSIC:
            case MapItemType::SEPARATOR:
            {
                break;
            }
            default:
            {
                const Map *const map = viewport->getMap();
                ava = model->getAvatarAt(selected);
                if ((map != nullptr) && (ava != nullptr))
                {
                    MapItem *const mapItem = map->findPortalXY(
                        ava->getX(), ava->getY());
                    popupMenu->showPopup(viewport->mMouseX,
                        viewport->mMouseY,
                        mapItem);
                }
                break;
            }
        }
    }
    else if (eventButton == MouseButton::MIDDLE)
    {
        if (type == MapItemType::EMPTY && (chatWindow != nullptr))
        {
            const std::string &name = model->getAvatarAt(selected)->getName();
            const WhisperTab *const tab = chatWindow->addWhisperTab(
                name, name, true);
            if (tab != nullptr)
                chatWindow->saveState();
        }
    }
}

void AvatarListBox::mouseReleased(MouseEvent &event A_UNUSED)
{
}

void AvatarListBox::optionChanged(const std::string &value)
{
    if (value == "showgender")
        mShowGender = config.getBoolValue("showgender");
    else if (value == "showlevel")
        mShowLevel = config.getBoolValue("showlevel");
}
