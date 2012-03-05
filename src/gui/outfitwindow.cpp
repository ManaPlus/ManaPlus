/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
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

#include "gui/outfitwindow.h"

#include "configuration.h"
#include "equipment.h"
#include "graphics.h"
#include "inventory.h"
#include "item.h"
#include "localplayer.h"
#include "logger.h"
#include "playerinfo.h"

#include "gui/chatwindow.h"
#include "gui/theme.h"
#include "gui/viewport.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"

#include "net/inventoryhandler.h"
#include "net/net.h"

#include "resources/image.h"
#include "resources/resourcemanager.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <vector>

#include "debug.h"

float OutfitWindow::mAlpha = 1.0;

OutfitWindow::OutfitWindow():
    Window(_("Outfits"), false, nullptr, "outfits.xml"),
    mBoxWidth(33),
    mBoxHeight(33),
    mGridWidth(4),
    mGridHeight(3),
    mItemClicked(false),
    mItemMoved(nullptr),
    mItemSelected(-1),
    mItemColorSelected(1),
    mCurrentOutfit(0),
    mAwayOutfit(0)
{
    setWindowName("Outfits");
    setResizable(true);
    setCloseButton(true);
    setStickyButtonLock(true);

    setDefaultSize(250, 400, 150, 230);
    setMinWidth(145);
    setMinHeight(220);

    mBorderColor = Theme::getThemeColor(Theme::BORDER, 64);
    mBackgroundColor = Theme::getThemeColor(Theme::BACKGROUND, 32);

    mPreviousButton = new Button(_("<"), "previous", this);
    mNextButton = new Button(_(">"), "next", this);
    mCurrentLabel = new Label(strprintf(_("Outfit: %d"), 1));
    mCurrentLabel->setAlignment(gcn::Graphics::CENTER);
    mKeyLabel = new Label(strprintf(_("Key: %s"),
        keyName(mCurrentOutfit).c_str()));
    mKeyLabel->setAlignment(gcn::Graphics::CENTER);
    mUnequipCheck = new CheckBox(_("Unequip first"),
        serverConfig.getValueBool("OutfitUnequip0", true));

    mAwayOutfitCheck = new CheckBox(_("Away outfit"),
        serverConfig.getValue("OutfitAwayIndex", OUTFITS_COUNT - 1));

    mUnequipCheck->setActionEventId("unequip");
    mUnequipCheck->addActionListener(this);

    mAwayOutfitCheck->setActionEventId("away");
    mAwayOutfitCheck->addActionListener(this);

    place(0, 3, mKeyLabel, 4);
    place(0, 4, mPreviousButton, 1);
    place(1, 4, mCurrentLabel, 2);
    place(3, 4, mNextButton, 1);
    place(0, 5, mUnequipCheck, 4);
    place(0, 6, mAwayOutfitCheck, 4);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);
    layout.setColWidth(4, Layout::CENTER);

    loadWindowState();

    load();
}

OutfitWindow::~OutfitWindow()
{
    save();
}

void OutfitWindow::load(bool oldConfig)
{
    Configuration *cfg;
    if (oldConfig)
        cfg = &config;
    else
        cfg = &serverConfig;

    memset(mItems, -1, sizeof(mItems));
    memset(mItemColors, 1, sizeof(mItemColors));

    for (int o = 0; o < OUTFITS_COUNT; o++)
    {
        std::string outfit = cfg->getValue("Outfit" + toString(o), "-1");
        std::string buf;
        std::stringstream ss(outfit);

        std::vector<int> tokens;

        while (ss >> buf)
            tokens.push_back(atoi(buf.c_str()));

        for (int i = 0; i < static_cast<int>(tokens.size())
                && i < OUTFIT_ITEM_COUNT; i++)
        {
            mItems[o][i] = tokens[i];
        }


        outfit = cfg->getValue("OutfitColor" + toString(o), "1");
        std::stringstream ss2(outfit);

        tokens.clear();

        while (ss2 >> buf)
            tokens.push_back(atoi(buf.c_str()));

        for (int i = 0; i < static_cast<int>(tokens.size())
                && i < OUTFIT_ITEM_COUNT; i++)
        {
            mItemColors[o][i] = tokens[i];
        }

        mItemsUnequip[o] = cfg->getValueBool("OutfitUnequip" + toString(o),
                                             true);
    }
    mAwayOutfit = cfg->getValue("OutfitAwayIndex", OUTFITS_COUNT - 1);
    if (mAwayOutfit >= OUTFITS_COUNT)
        mAwayOutfit = OUTFITS_COUNT - 1;

    if (mAwayOutfitCheck)
        mAwayOutfitCheck->setSelected(mAwayOutfit == mCurrentOutfit);
}

void OutfitWindow::save()
{
    std::string outfitStr;
    std::string outfitColorsStr;
    for (int o = 0; o < OUTFITS_COUNT; o++)
    {
        bool good = false;
        for (int i = 0; i < OUTFIT_ITEM_COUNT; i++)
        {
            int res = mItems[o][i] ? mItems[o][i] : -1;
            if (res != -1)
                good = true;
            outfitStr += toString(res);
            if (i < OUTFIT_ITEM_COUNT - 1)
                outfitStr += " ";
            outfitColorsStr += toString(static_cast<int>(mItemColors[o][i]));
            if (i < OUTFIT_ITEM_COUNT - 1)
                outfitColorsStr += " ";
        }
        if (good)
        {
            serverConfig.setValue("Outfit" + toString(o), outfitStr);
            serverConfig.setValue("OutfitColor" + toString(o),
                outfitColorsStr);
        }
        else
        {
            serverConfig.deleteKey("Outfit" + toString(o));
            serverConfig.deleteKey("OutfitColor" + toString(o));
        }

        if (mItemsUnequip[o])
        {
            serverConfig.deleteKey("OutfitUnequip" + toString(o));
        }
        else
        {
            serverConfig.setValue("OutfitUnequip" + toString(o),
                mItemsUnequip[o]);
        }
        outfitStr = "";
        outfitColorsStr = "";
    }
    serverConfig.setValue("OutfitAwayIndex", mAwayOutfit);
}

void OutfitWindow::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "next")
    {
        next();
    }
    else if (event.getId() == "previous")
    {
        previous();
    }
    else if (event.getId() == "unequip")
    {
        if (mCurrentOutfit < OUTFITS_COUNT)
            mItemsUnequip[mCurrentOutfit] = mUnequipCheck->isSelected();
    }
    else if (event.getId() == "away")
    {
        mAwayOutfit = mCurrentOutfit;
        if (!mAwayOutfitCheck->isSelected())
            mAwayOutfitCheck->setSelected(true);
    }
}

void OutfitWindow::wearOutfit(int outfit, bool unwearEmpty, bool select)
{
    bool isEmpty = true;

    Item *item;
    if (outfit < 0 || outfit > OUTFITS_COUNT)
        return;

    for (int i = 0; i < OUTFIT_ITEM_COUNT; i++)
    {
        item = PlayerInfo::getInventory()->findItem(
            mItems[outfit][i], mItemColors[outfit][i]);
        if (item && !item->isEquipped() && item->getQuantity())
        {
            if (item->isEquipment())
            {
                Net::getInventoryHandler()->equipItem(item);
                isEmpty = false;
            }
        }
    }

    if ((!isEmpty || unwearEmpty) && outfit < OUTFITS_COUNT
        && mItemsUnequip[outfit])
    {
        unequipNotInOutfit(outfit);
    }
    if (select)
    {
        mCurrentOutfit = outfit;
        showCurrentOutfit();
    }
}

void OutfitWindow::copyOutfit(int outfit)
{
    copyOutfit(outfit, mCurrentOutfit);
}

void OutfitWindow::copyOutfit(int src, int dst)
{
    if (src < 0 || src > OUTFITS_COUNT
        || dst < 0 || dst > OUTFITS_COUNT)
    {
        return;
    }

    for (int i = 0; i < OUTFIT_ITEM_COUNT; i++)
    {
        mItems[dst][i] = mItems[src][i];
    }
}

void OutfitWindow::draw(gcn::Graphics *graphics)
{
    Window::draw(graphics);
    Graphics *g = static_cast<Graphics*>(graphics);

    for (int i = 0; i < OUTFIT_ITEM_COUNT; i++)
    {
        const int itemX = 10 + ((i % mGridWidth) * mBoxWidth);
        const int itemY = 25 + ((i / mGridWidth) * mBoxHeight);

        graphics->setColor(mBorderColor);
        graphics->drawRectangle(gcn::Rectangle(itemX, itemY, 32, 32));
        graphics->setColor(mBackgroundColor);
        graphics->fillRectangle(gcn::Rectangle(itemX, itemY, 32, 32));

        if (mItems[mCurrentOutfit][i] < 0)
            continue;

        bool foundItem = false;
        Inventory *inv = PlayerInfo::getInventory();
        if (inv)
        {
            Item *item = inv->findItem(mItems[mCurrentOutfit][i],
                mItemColors[mCurrentOutfit][i]);
            if (item)
            {
                // Draw item icon.
                Image* image = item->getImage();
                if (image)
                {
                    g->drawImage(image, itemX, itemY);
                    foundItem = true;
                }
            }
        }
        if (!foundItem)
        {
            Image *image = Item::getImage(mItems[mCurrentOutfit][i],
                mItemColors[mCurrentOutfit][i]);
            if (image)
                g->drawImage(image, itemX, itemY);
        }
    }
    if (mItemMoved)
    {
        // Draw the item image being dragged by the cursor.
        Image* image = mItemMoved->getImage();
        if (image)
        {
            const int tPosX = mCursorPosX - (image->mBounds.w / 2);
            const int tPosY = mCursorPosY - (image->mBounds.h / 2);

            g->drawImage(image, tPosX, tPosY);
        }
    }
}


void OutfitWindow::mouseDragged(gcn::MouseEvent &event)
{
    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        if (!mItemMoved && mItemClicked)
        {
            const int index = getIndexFromGrid(event.getX(), event.getY());
            if (index == -1)
            {
                Window::mouseDragged(event);
                return;
            }
            const int itemId = mItems[mCurrentOutfit][index];
            const int itemColor = mItemColors[mCurrentOutfit][index];
            if (itemId < 0)
            {
                Window::mouseDragged(event);
                return;
            }
            mMoved = false;
            event.consume();
            Inventory *inv = PlayerInfo::getInventory();
            if (inv)
            {
                Item *item = inv->findItem(itemId, itemColor);
                if (item)
                    mItemMoved = item;
                else
                    mItemMoved = nullptr;
                mItems[mCurrentOutfit][index] = -1;
            }
        }
        if (mItemMoved)
        {
            mCursorPosX = event.getX();
            mCursorPosY = event.getY();
        }
    }
    Window::mouseDragged(event);
}

void OutfitWindow::mousePressed(gcn::MouseEvent &event)
{
    const int index = getIndexFromGrid(event.getX(), event.getY());
    if (index == -1)
    {
        if (event.getButton() == gcn::MouseEvent::RIGHT && viewport)
        {
            viewport->showOutfitsPopup();
            event.consume();
        }
        else
        {
            Window::mousePressed(event);
        }
        return;
    }
    mMoved = false;
    event.consume();

    // Stores the selected item if there is one.
    if (isItemSelected())
    {
        mItems[mCurrentOutfit][index] = mItemSelected;
        mItemColors[mCurrentOutfit][index] = mItemColorSelected;
//        mItemSelected = -1;
    }
    else if (mItems[mCurrentOutfit][index])
    {
        mItemClicked = true;
    }
    Window::mousePressed(event);
}

void OutfitWindow::mouseReleased(gcn::MouseEvent &event)
{
    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
//        if (isItemSelected())
//            mItemSelected = -1;

        const int index = getIndexFromGrid(event.getX(), event.getY());
        if (index == -1)
        {
            mItemMoved = nullptr;
            Window::mouseReleased(event);
            return;
        }
        mMoved = false;
        event.consume();
        if (mItemMoved)
        {
            mItems[mCurrentOutfit][index] = mItemMoved->getId();
            mItemColors[mCurrentOutfit][index] = mItemMoved->getColor();
            mItemMoved = nullptr;
        }
        if (mItemClicked)
            mItemClicked = false;
    }
    Window::mouseReleased(event);
}

int OutfitWindow::getIndexFromGrid(int pointX, int pointY) const
{
    const gcn::Rectangle tRect = gcn::Rectangle(
        10, 25, mGridWidth * mBoxWidth, mGridHeight * mBoxHeight);
    if (!tRect.isPointInRect(pointX, pointY))
        return -1;
    const int index = (((pointY - 25) / mBoxHeight) * mGridWidth) +
        (pointX - 10) / mBoxWidth;
    if (index >= OUTFIT_ITEM_COUNT || index < 0)
        return -1;
    return index;
}

void OutfitWindow::unequipNotInOutfit(int outfit)
{
    // here we think that outfit is correct index

    Inventory *inventory = PlayerInfo::getInventory();
    if (!inventory)
        return;

    for (unsigned i = 0; i < inventory->getSize(); i++)
    {
        if (inventory->getItem(i) && inventory->getItem(i)->isEquipped())
        {
            bool found = false;
            for (unsigned f = 0; f < OUTFIT_ITEM_COUNT; f++)
            {
                if (inventory->getItem(i)->getId() == mItems[outfit][f])
                {
                    found = true;
                    break;
                }
            }
            if (!found)
                Net::getInventoryHandler()->unequipItem(inventory->getItem(i));
        }
    }
}

static const SDLKey numsTbl[] =
{
    SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9,
    SDLK_0, SDLK_MINUS, SDLK_EQUALS, SDLK_BACKSPACE, SDLK_INSERT, SDLK_HOME,
    SDLK_q, SDLK_w, SDLK_e, SDLK_r, SDLK_t, SDLK_y, SDLK_u, SDLK_i, SDLK_o,
    SDLK_p, SDLK_LEFTBRACKET, SDLK_RIGHTBRACKET, SDLK_BACKSLASH, SDLK_a,
    SDLK_s, SDLK_d, SDLK_f, SDLK_g, SDLK_h, SDLK_j, SDLK_k, SDLK_l,
    SDLK_SEMICOLON, SDLK_QUOTE, SDLK_z, SDLK_x, SDLK_c, SDLK_v, SDLK_b, SDLK_n,
    SDLK_m, SDLK_COMMA, SDLK_PERIOD, SDLK_SLASH
};

int OutfitWindow::keyToNumber(SDLKey key) const
{
    for (unsigned f = 0; f < sizeof(numsTbl) / sizeof(SDLKey); f ++)
    {
        if (numsTbl[f] == key)
            return f;
    }
    return -1;
}

SDLKey OutfitWindow::numberToKey(unsigned number) const
{
    if (number >= sizeof(numsTbl) / sizeof(SDLKey))
        return SDLK_UNKNOWN;
    return numsTbl[number];
}

std::string OutfitWindow::keyName(int number)
{
    return SDL_GetKeyName(numberToKey(number));
}

void OutfitWindow::next()
{
    if (mCurrentOutfit < (OUTFITS_COUNT - 1))
        mCurrentOutfit++;
    else
        mCurrentOutfit = 0;
    showCurrentOutfit();
}

void OutfitWindow::previous()
{
    if (mCurrentOutfit > 0)
        mCurrentOutfit--;
    else
        mCurrentOutfit = OUTFITS_COUNT - 1;
    showCurrentOutfit();
}

void OutfitWindow::showCurrentOutfit()
{
    mCurrentLabel->setCaption(strprintf(_("Outfit: %d"), mCurrentOutfit + 1));
    mUnequipCheck->setSelected(mItemsUnequip[mCurrentOutfit]);
    mKeyLabel->setCaption(strprintf(_("Key: %s"),
        keyName(mCurrentOutfit).c_str()));
    mAwayOutfitCheck->setSelected(mAwayOutfit == mCurrentOutfit);
}

void OutfitWindow::wearNextOutfit(bool all)
{
    bool fromStart = false;
    next();
    if (!all && mCurrentOutfit < OUTFITS_COUNT)
    {
        while (!mItemsUnequip[mCurrentOutfit])
        {
            next();
            if (mCurrentOutfit == 0)
            {
                if (!fromStart)
                    fromStart = true;
                else
                    return;
            }
        }
    }
    wearOutfit(mCurrentOutfit);
}

void OutfitWindow::wearPreviousOutfit(bool all)
{
    bool fromStart = false;
    previous();
    if (!all && mCurrentOutfit < OUTFITS_COUNT)
    {
        while (!mItemsUnequip[mCurrentOutfit])
        {
            previous();
            if (mCurrentOutfit == 0)
            {
                if (!fromStart)
                    fromStart = true;
                else
                    return;
            }
        }
    }
    wearOutfit(mCurrentOutfit);
}

void OutfitWindow::copyFromEquiped()
{
    copyFromEquiped(mCurrentOutfit);
}

void OutfitWindow::copyFromEquiped(int dst)
{
    Inventory *inventory = PlayerInfo::getInventory();
    if (!inventory)
        return;

    int outfitCell = 0;

    for (unsigned i = 0; i < inventory->getSize(); i++)
    {
        if (inventory->getItem(i) && inventory->getItem(i)->isEquipped())
        {
            mItems[dst][outfitCell] = inventory->getItem(i)->getId();
            mItemColors[dst][outfitCell++] = inventory->getItem(i)->getColor();
            if (outfitCell >= OUTFIT_ITEM_COUNT)
                break;
        }
    }
}

void OutfitWindow::wearAwayOutfit()
{
    copyFromEquiped(OUTFITS_COUNT);
    wearOutfit(mAwayOutfit, false);
}

void OutfitWindow::unwearAwayOutfit()
{
    wearOutfit(OUTFITS_COUNT);
}

void OutfitWindow::setItemSelected(Item *item)
{
    if (item)
    {
        mItemSelected = item->getId();
        mItemColorSelected = item->getColor();
    }
    else
    {
        mItemSelected = -1;
        mItemColorSelected = 1;
    }
}

void OutfitWindow::clearCurrentOutfit()
{
    for (unsigned f = 0; f < OUTFIT_ITEM_COUNT; f++)
    {
        mItems[mCurrentOutfit][f] = -1;
        mItemColors[mCurrentOutfit][f] = 1;
    }
}
