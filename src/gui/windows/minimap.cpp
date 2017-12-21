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

#include "gui/windows/minimap.h"

#include "actormanager.h"
#include "configuration.h"
#include "party.h"
#include "settings.h"

#include "being/localplayer.h"

#include "enums/resources/map/blockmask.h"

#include "fs/virtfs/fs.h"

#include "gui/popupmanager.h"
#include "gui/viewport.h"
#include "gui/userpalette.h"

#include "gui/popups/popupmenu.h"
#include "gui/popups/textpopup.h"

#include "gui/windows/setupwindow.h"

#include "resources/imagehelper.h"

#include "resources/image/image.h"

#include "resources/map/map.h"
#include "resources/map/metatile.h"

#include "resources/loaders/imageloader.h"

#include "utils/gettext.h"
#include "utils/foreach.h"
#include "utils/sdlcheckutils.h"
#include "utils/stdmove.h"

#include "debug.h"

Minimap *minimap = nullptr;
bool Minimap::mShow = true;

Minimap::Minimap() :
    // TRANSLATORS: mini map window name
    Window(_("Map"), Modal_false, nullptr, "map.xml"),
    mWidthProportion(0.5),
    mHeightProportion(0.5),
    mMapImage(nullptr),
    mMapOriginX(0),
    mMapOriginY(0),
    mCustomMapImage(false),
    mAutoResize(config.getBoolValue("autoresizeminimaps"))
{
    setWindowName("Minimap");
    mShow = config.getValueBool(getWindowName() + "Show", true);

    config.addListener("autoresizeminimaps", this);

    setDefaultSize(5, 25, 100, 100);
    // set this to false as the minimap window size is changed
    // depending on the map size
    setResizable(true);
    if (setupWindow != nullptr)
        setupWindow->registerWindowForReset(this);

    setDefaultVisible(true);
    setSaveVisible(true);

    setStickyButton(true);
    setSticky(false);

    loadWindowState();
    setVisible(fromBool(mShow, Visible), isSticky());
    enableVisibleSound(true);
}

Minimap::~Minimap()
{
    config.setValue(getWindowName() + "Show", mShow);
    config.removeListeners(this);
    CHECKLISTENERS
    deleteMapImage();
}

void Minimap::deleteMapImage()
{
    if (mMapImage != nullptr)
    {
        if (mCustomMapImage)
            delete mMapImage;
        else
            mMapImage->decRef();
        mMapImage = nullptr;
    }
}

void Minimap::setMap(const Map *const map)
{
    BLOCK_START("Minimap::setMap")
    std::string caption;

    if (map != nullptr)
        caption = map->getName();

    if (caption.empty())
    {
        // TRANSLATORS: mini map window name
        caption = _("Map");
    }

    setCaption(caption);
    deleteMapImage();

    if (map != nullptr)
    {
        if (config.getBoolValue("showExtMinimaps"))
        {
            SDL_Surface *const surface = MSDL_CreateRGBSurface(SDL_SWSURFACE,
                map->getWidth(), map->getHeight(), 32,
                0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000);
            if (surface == nullptr)
            {
                if (!isSticky())
                    setVisible(Visible_false);
                BLOCK_END("Minimap::setMap")
                return;
            }

            // I'm not sure if the locks are necessary since it's a SWSURFACE
            SDL_LockSurface(surface);
            int* data = static_cast<int*>(surface->pixels);
            if (data == nullptr)
            {
                if (!isSticky())
                    setVisible(Visible_false);
                BLOCK_END("Minimap::setMap")
                return;
            }
            const int size = surface->h * surface->w;
            const int mask = (BlockMask::WALL |
                BlockMask::AIR |
                BlockMask::WATER |
                BlockMask::PLAYERWALL);

            for (int ptr = 0; ptr < size; ptr ++)
            {
                *(data ++) = (map->mMetaTiles[ptr].blockmask & mask) != 0 ?
                    0x0 : 0x00ffffff;
            }

            SDL_UnlockSurface(surface);

            mMapImage = imageHelper->loadSurface(surface);
            mMapImage->setAlpha(settings.guiAlpha);
            mCustomMapImage = true;
            MSDL_FreeSurface(surface);
        }
        else
        {
            std::string tempname = pathJoin(paths.getStringValue("minimaps"),
                map->getFilename()).append(".png");

            std::string minimapName = map->getProperty("minimap",
                std::string());

            if (minimapName.empty() && VirtFs::exists(tempname))
                minimapName = tempname;

            if (minimapName.empty())
            {
                tempname = pathJoin("graphics/minimaps",
                    map->getFilename()).append(".png");
                if (VirtFs::exists(tempname))
                    minimapName = STD_MOVE(tempname);
            }

            if (!minimapName.empty())
                mMapImage = Loader::getImage(minimapName);
            else
                mMapImage = nullptr;
            mCustomMapImage = false;
        }
    }

    if ((mMapImage != nullptr) && (map != nullptr))
    {
        const int width = mMapImage->mBounds.w + 2 * getPadding();
        const int height = mMapImage->mBounds.h
            + getTitleBarHeight() + getPadding();
        const int mapWidth = mMapImage->mBounds.w < 100 ? width : 100;
        const int mapHeight = mMapImage->mBounds.h < 100 ? height : 100;
        const int minWidth = mapWidth > 310 ? 310 : mapWidth;
        const int minHeight = mapHeight > 220 ? 220 : mapHeight;

        setMinWidth(minWidth);
        setMinHeight(minHeight);

        mWidthProportion = static_cast<float>(
                mMapImage->mBounds.w) / static_cast<float>(map->getWidth());
        mHeightProportion = static_cast<float>(
                mMapImage->mBounds.h) / static_cast<float>(map->getHeight());

        setMaxWidth(width);
        setMaxHeight(height);
        if (mAutoResize)
        {
            setWidth(width);
            setHeight(height);
        }

        const Rect &rect = mDimension;
        setDefaultSize(rect.x, rect.y, rect.width, rect.height);
        resetToDefaultSize();

        if (mShow)
            setVisible(Visible_true);
    }
    else
    {
        if (!isSticky())
            setVisible(Visible_false);
    }
    BLOCK_END("Minimap::setMap")
}

void Minimap::toggle()
{
    setVisible(fromBool(!isWindowVisible(), Visible), isSticky());
    mShow = isWindowVisible();
}

void Minimap::draw(Graphics *const graphics)
{
    BLOCK_START("Minimap::draw")

    Window::draw(graphics);
    draw2(graphics);
}

void Minimap::safeDraw(Graphics *const graphics)
{
    BLOCK_START("Minimap::draw")

    Window::safeDraw(graphics);
    draw2(graphics);
}

void Minimap::draw2(Graphics *const graphics)
{
    if (userPalette == nullptr ||
        localPlayer == nullptr ||
        viewport == nullptr)
    {
        BLOCK_END("Minimap::draw")
        return;
    }

    const Rect a = getChildrenArea();

    graphics->pushClipArea(a);

    if (actorManager == nullptr)
    {
        BLOCK_END("Minimap::draw")
        return;
    }

    mMapOriginX = 0;
    mMapOriginY = 0;

    if (mMapImage != nullptr)
    {
        const SDL_Rect &rect = mMapImage->mBounds;
        const int w = rect.w;
        const int h = rect.h;
        if (w > a.width || h > a.height)
        {
            mMapOriginX = (a.width / 2) - (localPlayer->mPixelX +
                viewport->getCameraRelativeX() * mWidthProportion) / 32;

            mMapOriginY = (a.height / 2) - (localPlayer->mPixelY +
                viewport->getCameraRelativeY() * mHeightProportion) / 32;

            const int minOriginX = a.width - w;
            const int minOriginY = a.height - h;

            if (mMapOriginX < minOriginX)
                mMapOriginX = minOriginX;
            if (mMapOriginY < minOriginY)
                mMapOriginY = minOriginY;
            if (mMapOriginX > 0)
                mMapOriginX = 0;
            if (mMapOriginY > 0)
                mMapOriginY = 0;
        }

        graphics->drawImage(mMapImage, mMapOriginX, mMapOriginY);
    }

    const ActorSprites &actors = actorManager->getAll();
    FOR_EACH (ActorSpritesConstIterator, it, actors)
    {
        if (((*it) == nullptr) || (*it)->getType() == ActorType::FloorItem)
            continue;

        const Being *const being = static_cast<const Being *>(*it);
        if (being == nullptr)
            continue;

        int dotSize = 2;
        UserColorIdT type = UserColorId::PC;

        if (being == localPlayer)
        {
            type = UserColorId::SELF;
            dotSize = 3;
        }
        else if (being->isGM())
        {
            type = UserColorId::GM;
        }
        else if (being->getGuild() == localPlayer->getGuild()
                 || being->getGuildName() == localPlayer->getGuildName())
        {
            type = UserColorId::GUILD;
        }
        else
        {
            switch (being->getType())
            {
                case ActorType::Monster:
                    type = UserColorId::MONSTER;
                    break;

                case ActorType::Npc:
                    type = UserColorId::NPC;
                    break;

                case ActorType::Portal:
                    type = UserColorId::PORTAL_HIGHLIGHT;
                    break;

                case ActorType::Pet:
                    type = UserColorId::PET;
                    break;
                case ActorType::Mercenary:
                    type = UserColorId::MERCENARY;
                    break;

                case ActorType::Homunculus:
                    type = UserColorId::HOMUNCULUS;
                    break;

                case ActorType::SkillUnit:
                    type = UserColorId::SKILLUNIT;
                    break;
                case ActorType::Avatar:
                case ActorType::Unknown:
                case ActorType::Player:
                case ActorType::FloorItem:
                case ActorType::Elemental:
                default:
                    continue;
            }
        }

        if (userPalette != nullptr)
            graphics->setColor(userPalette->getColor(type, 255U));

        const int offsetHeight = CAST_S32(static_cast<float>(
                dotSize - 1) * mHeightProportion);
        const int offsetWidth = CAST_S32(static_cast<float>(
                dotSize - 1) * mWidthProportion);
        graphics->fillRectangle(Rect(
            (being->mPixelX * mWidthProportion) / 32
            + mMapOriginX - offsetWidth,
            (being->mPixelY * mHeightProportion) / 32
            + mMapOriginY - offsetHeight, dotSize, dotSize));
    }

    if (localPlayer->isInParty())
    {
        const Party *const party = localPlayer->getParty();
        if (party != nullptr)
        {
            const PartyMember *const m = party->getMember(
                localPlayer->getName());
            const Party::MemberList *const members = party->getMembers();
            if (m != nullptr)
            {
                const std::string curMap = m->getMap();
                Party::MemberList::const_iterator it = members->begin();
                const Party::MemberList::const_iterator
                    it_end = members->end();
                while (it != it_end)
                {
                    const PartyMember *const member = *it;
                    if ((member != nullptr) && member->getMap() == curMap
                        && member->getOnline() && member != m)
                    {
                        if (userPalette != nullptr)
                        {
                            graphics->setColor(userPalette->getColor(
                                UserColorId::PARTY, 255U));
                        }

                        const int offsetHeight = CAST_S32(
                            mHeightProportion);
                        const int offsetWidth = CAST_S32(
                            mWidthProportion);

                        graphics->fillRectangle(Rect(
                            CAST_S32(member->getX()
                            * mWidthProportion) + mMapOriginX - offsetWidth,
                            CAST_S32(member->getY()
                            * mHeightProportion) + mMapOriginY - offsetHeight,
                            2, 2));
                    }
                    ++ it;
                }
            }
        }
    }

    const int gw = graphics->getWidth();
    const int gh = graphics->getHeight();
    int x = (localPlayer->mPixelX - (gw / 2)
        + viewport->getCameraRelativeX())
        * mWidthProportion / 32 + mMapOriginX;
    int y = (localPlayer->mPixelY - (gh / 2)
        + viewport->getCameraRelativeY())
        * mHeightProportion / 32 + mMapOriginY;

    const int w = CAST_S32(static_cast<float>(
        gw) * mWidthProportion / 32);
    const int h = CAST_S32(static_cast<float>(
        gh) * mHeightProportion / 32);

    if (w <= a.width)
    {
        if (x < 0 && (w != 0))
            x = 0;
        if (x + w > a.width)
            x = a.width - w;
    }
    if (h <= a.height)
    {
        if (y < 0 && (h != 0))
            y = 0;
        if (y + h > a.height)
            y = a.height - h;
    }

    graphics->setColor(userPalette->getColor(UserColorId::PC, 255U));
    graphics->drawRectangle(Rect(x, y, w, h));
    graphics->popClipArea();
    BLOCK_END("Minimap::draw")
}

void Minimap::mousePressed(MouseEvent &event)
{
    if (event.getButton() == MouseButton::RIGHT)
        return;
    Window::mousePressed(event);
}

void Minimap::mouseReleased(MouseEvent &event)
{
    Window::mouseReleased(event);

    if ((localPlayer == nullptr) || (popupManager == nullptr))
        return;

    if (event.getButton() == MouseButton::LEFT)
    {
        int x = event.getX();
        int y = event.getY();
        screenToMap(x, y);

        localPlayer->navigateTo(x, y);
    }
    else if (event.getButton() == MouseButton::RIGHT)
    {
        int x = event.getX();
        int y = event.getY();
        screenToMap(x, y);
        popupMenu->showMapPopup(viewport->mMouseX,
            viewport->mMouseY,
            x, y,
            true);
    }
}

void Minimap::mouseMoved(MouseEvent &event)
{
    Window::mouseMoved(event);
    const int x = event.getX();
    const int y = event.getY();
    const Rect &rect = mDimension;
    textPopup->show(x + rect.x, y + rect.y, mCaption);
}

void Minimap::mouseExited(MouseEvent &event)
{
    Window::mouseExited(event);
    textPopup->hide();
}

void Minimap::screenToMap(int &x, int &y)
{
    const Rect a = getChildrenArea();
    x = (x - a.x - mMapOriginX + mWidthProportion) / mWidthProportion;
    y = (y - a.y - mMapOriginY + mHeightProportion) / mHeightProportion;
}

void Minimap::optionChanged(const std::string &name)
{
    if (name == "autoresizeminimaps")
        mAutoResize = config.getBoolValue("autoresizeminimaps");
}
