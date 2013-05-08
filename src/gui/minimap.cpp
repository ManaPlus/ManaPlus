/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "gui/minimap.h"

#include "actorspritemanager.h"
#include "client.h"
#include "configuration.h"
#include "localplayer.h"
#include "party.h"

#include "gui/setup.h"
#include "gui/viewport.h"
#include "gui/textpopup.h"

#include "resources/image.h"
#include "resources/imagehelper.h"
#include "resources/resourcemanager.h"

#include "utils/gettext.h"

#include <guichan/font.hpp>

#include "debug.h"

bool Minimap::mShow = true;

Minimap::Minimap() :
    // TRANSLATORS: mini map window name
    Window(_("Map"), false, nullptr, "map.xml"),
    mMapImage(nullptr),
    mWidthProportion(0.5),
    mHeightProportion(0.5),
    mCustomMapImage(false),
    mMapOriginX(0),
    mMapOriginY(0),
    mTextPopup(new TextPopup),
    mAutoResize(config.getBoolValue("autoresizeminimaps"))
{
    setWindowName("Minimap");
    mShow = config.getValueBool(getWindowName() + "Show", true);

    config.addListener("autoresizeminimaps", this);

    setDefaultSize(5, 25, 100, 100);
    // set this to false as the minimap window size is changed
    // depending on the map size
    setResizable(true);
    setupWindow->registerWindowForReset(this);

    setDefaultVisible(true);
    setSaveVisible(true);

    setStickyButton(true);
    setSticky(false);

    loadWindowState();
    setVisible(mShow, isSticky());
    enableVisibleSound(true);
}

Minimap::~Minimap()
{
    config.setValue(getWindowName() + "Show", mShow);
    config.removeListeners(this);

    if (mMapImage)
    {
        if (mCustomMapImage)
            delete mMapImage;
        else
            mMapImage->decRef();
        mMapImage = nullptr;
    }
    delete mTextPopup;
    mTextPopup = nullptr;
}

void Minimap::setMap(const Map *const map)
{
    std::string caption;

    if (map)
        caption = map->getName();

    if (caption.empty())
    {
        // TRANSLATORS: mini map window name
        caption = _("Map");
    }

    setCaption(caption);

    // Adapt the image
    if (mMapImage)
    {
        if (mCustomMapImage)
            delete mMapImage;
        else
            mMapImage->decRef();
        mMapImage = nullptr;
    }

    if (map)
    {
        if (config.getBoolValue("showExtMinimaps"))
        {
            SDL_Surface *const surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                map->getWidth(), map->getHeight(), 32,
                0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000);
            if (!surface)
            {
                if (!isSticky())
                    setVisible(false);
                return;
            }

            // I'm not sure if the locks are necessary since it's a SWSURFACE
            SDL_LockSurface(surface);
            int* data = static_cast<int*>(surface->pixels);
            if (!data)
            {
                if (!isSticky())
                    setVisible(false);
                return;
            }
            const int size = surface->h * surface->w;
            const int mask = (Map::BLOCKMASK_WALL | Map::BLOCKMASK_AIR
                | Map::BLOCKMASK_WATER);

            for (int ptr = 0; ptr < size; ptr ++)
                *(data ++) = -!(map->mMetaTiles[ptr].blockmask & mask);

            SDL_UnlockSurface(surface);

            mMapImage = imageHelper->load(surface);
            mMapImage->setAlpha(Client::getGuiAlpha());
            mCustomMapImage = true;
            SDL_FreeSurface(surface);
        }
        else
        {
            std::string tempname = paths.getStringValue("minimaps").append(
                map->getFilename()).append(".png");
            ResourceManager *const resman = ResourceManager::getInstance();

            std::string minimapName = map->getProperty("minimap");

            if (minimapName.empty() && resman->exists(tempname))
                minimapName = tempname;

            if (minimapName.empty())
            {
                tempname = std::string("graphics/minimaps/").append(
                    map->getFilename()).append(".png");
                if (resman->exists(tempname))
                    minimapName = tempname;
            }

            mMapImage = resman->getImage(minimapName);
            mCustomMapImage = false;
        }
    }

    if (mMapImage && map)
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

        const gcn::Rectangle &rect = mDimension;
        setDefaultSize(rect.x, rect.y, rect.width, rect.height);
        resetToDefaultSize();

        if (mShow)
            setVisible(true);
    }
    else
    {
        if (!isSticky())
            setVisible(false);
    }
}

void Minimap::toggle()
{
    setVisible(!isWindowVisible(), isSticky());
    mShow = isWindowVisible();
}

void Minimap::draw(gcn::Graphics *graphics)
{
    BLOCK_START("Minimap::draw")
    Window::draw(graphics);

    if (!userPalette || !player_node || !viewport)
    {
        BLOCK_END("Minimap::draw")
        return;
    }

    Graphics *const graph = static_cast<Graphics*>(graphics);

    const gcn::Rectangle a = getChildrenArea();

    graphics->pushClipArea(a);

    if (!actorSpriteManager)
    {
        BLOCK_END("Minimap::draw")
        return;
    }

    mMapOriginX = 0;
    mMapOriginY = 0;

    if (mMapImage)
    {
        const SDL_Rect &rect = mMapImage->mBounds;
        const int w = rect.w;
        const int h = rect.h;
        if (w > a.width || h > a.height)
        {
            const Vector &p = player_node->getPosition();
            mMapOriginX = (a.width / 2) - (p.x + static_cast<float>(
                viewport->getCameraRelativeX()) * mWidthProportion) / 32;

            mMapOriginY = (a.height / 2) - (p.y + static_cast<float>(
                viewport->getCameraRelativeY()) * mHeightProportion) / 32;

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

        graph->drawImage(mMapImage, mMapOriginX, mMapOriginY);
    }

    const ActorSprites &actors = actorSpriteManager->getAll();
    FOR_EACH (ActorSpritesConstIterator, it, actors)
    {
        if (!(*it) || (*it)->getType() == ActorSprite::FLOOR_ITEM)
            continue;

        const Being *const being = static_cast<const Being *const>(*it);
        if (!being)
            continue;

        int dotSize = 2;
        int type = UserPalette::PC;

        if (being == player_node)
        {
            type = UserPalette::SELF;
            dotSize = 3;
        }
        else if (being->isGM())
        {
            type = UserPalette::GM;
        }
        else if (being->getGuild() == player_node->getGuild()
                 || being->getGuildName() == player_node->getGuildName())
        {
            type = UserPalette::GUILD;
        }
        else
        {
            switch (being->getType())
            {
                case ActorSprite::MONSTER:
                    type = UserPalette::MONSTER;
                    break;

                case ActorSprite::NPC:
                    type = UserPalette::NPC;
                    break;

                case ActorSprite::UNKNOWN:
                case ActorSprite::PLAYER:
                case ActorSprite::FLOOR_ITEM:
                case ActorSprite::PORTAL:
                case ActorSprite::PET:
                default:
                    continue;
            }
        }

        if (userPalette)
            graphics->setColor(userPalette->getColor(type));

        const int offsetHeight = static_cast<int>(static_cast<float>(
                dotSize - 1) * mHeightProportion);
        const int offsetWidth = static_cast<int>(static_cast<float>(
                dotSize - 1) * mWidthProportion);
        const Vector &pos = being->getPosition();

        graphics->fillRectangle(gcn::Rectangle(
            static_cast<float>(pos.x * mWidthProportion) / 32
            + mMapOriginX - offsetWidth,
            static_cast<float>(pos.y * mHeightProportion) / 32
            + mMapOriginY - offsetHeight, dotSize, dotSize));
    }

    if (player_node->isInParty())
    {
        const Party *const party = player_node->getParty();
        if (party)
        {
            const PartyMember *const m = party->getMember(
                player_node->getName());
            const Party::MemberList *const members = party->getMembers();
            if (m && members)
            {
                const std::string curMap = m->getMap();
                Party::MemberList::const_iterator it = members->begin();
                const Party::MemberList::const_iterator
                    it_end = members->end();
                while (it != it_end)
                {
                    const PartyMember *const member = *it;
                    if (member && member->getMap() == curMap
                        && member->getOnline() && member != m)
                    {
                        if (userPalette)
                        {
                            graphics->setColor(userPalette->getColor(
                                UserPalette::PARTY));
                        }

                        const int offsetHeight = static_cast<int>(
                            mHeightProportion);
                        const int offsetWidth = static_cast<int>(
                            mWidthProportion);

                        graphics->fillRectangle(gcn::Rectangle(
                            static_cast<int>(member->getX()
                            * mWidthProportion) + mMapOriginX - offsetWidth,
                            static_cast<int>(member->getY()
                            * mHeightProportion) + mMapOriginY - offsetHeight,
                            2, 2));
                    }
                    ++ it;
                }
            }
        }
    }

    const Vector &pos = player_node->getPosition();

    const int gw = graph->getWidth();
    const int gh = graph->getHeight();
    int x = static_cast<float>((pos.x - (gw / 2)
        + viewport->getCameraRelativeX())
        * mWidthProportion) / 32 + mMapOriginX;
    int y = static_cast<float>((pos.y - (gh / 2)
        + viewport->getCameraRelativeY())
        * mHeightProportion) / 32 + mMapOriginY;

    const int w = static_cast<int>(static_cast<float>(
        gw) * mWidthProportion / 32);
    const int h = static_cast<int>(static_cast<float>(
        gh) * mHeightProportion / 32);

    if (w <= a.width)
    {
        if (x < 0 && w)
            x = 0;
        if (x + w > a.width)
            x = a.width - w;
    }
    if (h <= a.height)
    {
        if (y < 0 && h)
            y = 0;
        if (y + h > a.height)
            y = a.height - h;
    }

    graphics->setColor(userPalette->getColor(UserPalette::PC));
    graphics->drawRectangle(gcn::Rectangle(x, y, w, h));
    graphics->popClipArea();
    BLOCK_END("Minimap::draw")
}

void Minimap::mouseReleased(gcn::MouseEvent &event)
{
    Window::mouseReleased(event);

    if (!player_node || !viewport)
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        int x = event.getX();
        int y = event.getY();
        screenToMap(x, y);

        player_node->navigateTo(x, y);
    }
    else if (event.getButton() == gcn::MouseEvent::RIGHT)
    {
        int x = event.getX();
        int y = event.getY();
        screenToMap(x, y);
        viewport->showMapPopup(x, y);
    }
}

void Minimap::mouseMoved(gcn::MouseEvent &event)
{
    Window::mouseMoved(event);
    const int x = event.getX();
    const int y = event.getY();
    const gcn::Rectangle &rect = mDimension;
    mTextPopup->show(x + rect.x, y + rect.y, mCaption);
}

void Minimap::mouseExited(gcn::MouseEvent &event)
{
    Window::mouseExited(event);
    mTextPopup->hide();
}

void Minimap::screenToMap(int &x, int &y)
{
    const gcn::Rectangle a = getChildrenArea();
    x = (x - a.x - mMapOriginX + mWidthProportion) / mWidthProportion;
    y = (y - a.y - mMapOriginY + mHeightProportion) / mHeightProportion;
}

void Minimap::optionChanged(const std::string &name)
{
    if (name == "autoresizeminimaps")
        mAutoResize = config.getBoolValue("autoresizeminimaps");
}
