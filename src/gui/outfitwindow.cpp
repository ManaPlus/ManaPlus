/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
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

#include "gui/outfitwindow.h"

#include "configuration.h"
#include "emoteshortcut.h"
#include "equipment.h"
#include "game.h"
#include "inputmanager.h"
#include "inventory.h"
#include "item.h"
#include "keyboardconfig.h"
#include "localplayer.h"
#include "playerinfo.h"

#include "gui/chatwindow.h"
#include "gui/inventorywindow.h"
#include "gui/viewport.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"

#include "resources/image.h"
#include "resources/resourcemanager.h"

#include "utils/gettext.h"

#include <vector>

#include "debug.h"

float OutfitWindow::mAlpha = 1.0;

OutfitWindow::OutfitWindow():
    Window(_("Outfits"), false, nullptr, "outfits.xml"),
    gcn::ActionListener(),
    mPreviousButton(new Button(this, _("<"), "previous", this)),
    mNextButton(new Button(this, _(">"), "next", this)),
    mEquipButtom(new Button(this, _("Equip"), "equip", this)),
    mCurrentLabel(new Label(this, strprintf(_("Outfit: %d"), 1))),
    mUnequipCheck(new CheckBox(this, _("Unequip first"),
        serverConfig.getValueBool("OutfitUnequip0", true))),
    mAwayOutfitCheck(new CheckBox(this, _("Away outfit"),
        serverConfig.getValue("OutfitAwayIndex", OUTFITS_COUNT - 1))),
    mCurrentOutfit(0),
    mKeyLabel(new Label(this, strprintf(_("Key: %s"),
        keyName(mCurrentOutfit).c_str()))),
    mBoxWidth(33),
    mBoxHeight(33),
    mCursorPosX(0),
    mCursorPosY(0),
    mGridWidth(4),
    mGridHeight(4),
    mItemClicked(false),
    mItemMoved(nullptr),
    mItemSelected(-1),
    mItemColorSelected(1),
    mAwayOutfit(0),
    mBorderColor(getThemeColor(Theme::BORDER, 64)),
    mBackgroundColor(getThemeColor(Theme::BACKGROUND, 32))
{
    setWindowName("Outfits");
    setResizable(true);
    setCloseButton(true);
    setStickyButtonLock(true);

    setDefaultSize(250, 400, 150, 290);
    setMinWidth(145);
    setMinHeight(220);

    mCurrentLabel->setAlignment(gcn::Graphics::CENTER);
    mKeyLabel->setAlignment(gcn::Graphics::CENTER);

    mUnequipCheck->setActionEventId("unequip");
    mUnequipCheck->addActionListener(this);

    mAwayOutfitCheck->setActionEventId("away");
    mAwayOutfitCheck->addActionListener(this);

    place(1, 3, mEquipButtom, 2);
    place(0, 4, mKeyLabel, 4);
    place(0, 5, mPreviousButton, 1);
    place(1, 5, mCurrentLabel, 2);
    place(3, 5, mNextButton, 1);
    place(0, 6, mUnequipCheck, 4);
    place(0, 7, mAwayOutfitCheck, 4);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);
    layout.setColWidth(4, Layout::CENTER);

    loadWindowState();

    enableVisibleSound(true);
    load();
}

OutfitWindow::~OutfitWindow()
{
    save();
}

void OutfitWindow::load(const bool oldConfig)
{
    const Configuration *cfg;
    if (oldConfig)
        cfg = &config;
    else
        cfg = &serverConfig;

    memset(mItems, -1, sizeof(mItems));
    memset(mItemColors, 1, sizeof(mItemColors));

    for (unsigned o = 0; o < OUTFITS_COUNT; o++)
    {
        std::string outfit = cfg->getValue("Outfit" + toString(o), "-1");
        std::string buf;
        std::stringstream ss(outfit);

        std::vector<int> tokens;

        while (ss >> buf)
            tokens.push_back(atoi(buf.c_str()));

        for (size_t i = 0, sz = tokens.size();
             i < sz && i < OUTFIT_ITEM_COUNT; i++)
        {
            mItems[o][i] = tokens[i];
        }

        outfit = cfg->getValue("OutfitColor" + toString(o), "1");
        std::stringstream ss2(outfit);

        tokens.clear();

        std::vector<unsigned char> tokens2;
        while (ss2 >> buf)
            tokens2.push_back(static_cast<unsigned char>(atoi(buf.c_str())));

        for (size_t i = 0, sz = tokens2.size();
             i < sz && i < OUTFIT_ITEM_COUNT; i++)
        {
            mItemColors[o][i] = tokens2[i];
        }

        mItemsUnequip[o] = cfg->getValueBool("OutfitUnequip" + toString(o),
                                             true);
    }
    mAwayOutfit = cfg->getValue("OutfitAwayIndex", OUTFITS_COUNT - 1);
    if (mAwayOutfit >= static_cast<int>(OUTFITS_COUNT))
        mAwayOutfit = static_cast<int>(OUTFITS_COUNT) - 1;

    if (mAwayOutfitCheck)
        mAwayOutfitCheck->setSelected(mAwayOutfit == mCurrentOutfit);
}

void OutfitWindow::save()
{
    std::string outfitStr;
    std::string outfitColorsStr;
    for (unsigned o = 0; o < OUTFITS_COUNT; o++)
    {
        bool good = false;
        for (unsigned i = 0; i < OUTFIT_ITEM_COUNT; i++)
        {
            const int res = mItems[o][i] ? mItems[o][i] : -1;
            if (res != -1)
                good = true;
            outfitStr.append(toString(res));
            if (i < OUTFIT_ITEM_COUNT - 1)
                outfitStr.append(" ");
            outfitColorsStr.append(toString(static_cast<int>(
                mItemColors[o][i])));
            if (i < OUTFIT_ITEM_COUNT - 1)
                outfitColorsStr.append(" ");
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
        outfitStr.clear();
        outfitColorsStr.clear();
    }
    serverConfig.setValue("OutfitAwayIndex", mAwayOutfit);
}

void OutfitWindow::action(const gcn::ActionEvent &event)
{
    const std::string eventId = event.getId();
    if (eventId == "next")
    {
        next();
    }
    else if (eventId == "previous")
    {
        previous();
    }
    else if (eventId == "unequip")
    {
        if (mCurrentOutfit < static_cast<int>(OUTFITS_COUNT))
            mItemsUnequip[mCurrentOutfit] = mUnequipCheck->isSelected();
    }
    else if (eventId == "equip")
    {
        wearOutfit(mCurrentOutfit);
        if (Game::instance())
            Game::instance()->setValidSpeed();
    }
    else if (eventId == "away")
    {
        mAwayOutfit = mCurrentOutfit;
        if (!mAwayOutfitCheck->isSelected())
            mAwayOutfitCheck->setSelected(true);
    }
}

void OutfitWindow::wearOutfit(const int outfit, const bool unwearEmpty,
                              const bool select)
{
    bool isEmpty = true;

    if (outfit < 0 || outfit > static_cast<int>(OUTFITS_COUNT))
        return;

    for (unsigned i = 0; i < OUTFIT_ITEM_COUNT; i++)
    {
        const Item *const item = PlayerInfo::getInventory()->findItem(
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

    if ((!isEmpty || unwearEmpty) && outfit < static_cast<int>(OUTFITS_COUNT)
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

void OutfitWindow::copyOutfit(const int outfit)
{
    copyOutfit(outfit, mCurrentOutfit);
}

void OutfitWindow::copyOutfit(const int src, const int dst)
{
    if (src < 0 || src > static_cast<int>(OUTFITS_COUNT)
        || dst < 0 || dst > static_cast<int>(OUTFITS_COUNT))
    {
        return;
    }

    for (unsigned int i = 0; i < OUTFIT_ITEM_COUNT; i++)
        mItems[dst][i] = mItems[src][i];
}

void OutfitWindow::draw(gcn::Graphics *graphics)
{
    BLOCK_START("OutfitWindow::draw")
    Window::draw(graphics);
    Graphics *const g = static_cast<Graphics*>(graphics);

    for (unsigned int i = 0; i < OUTFIT_ITEM_COUNT; i++)
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
        const Inventory *const inv = PlayerInfo::getInventory();
        if (inv)
        {
            const Item *const item = inv->findItem(mItems[mCurrentOutfit][i],
                mItemColors[mCurrentOutfit][i]);
            if (item)
            {
                // Draw item icon.
                const Image *const image = item->getImage();
                if (image)
                {
                    g->drawImage(image, itemX, itemY);
                    foundItem = true;
                }
            }
        }
        if (!foundItem)
        {
            Image *const image = Item::getImage(mItems[mCurrentOutfit][i],
                mItemColors[mCurrentOutfit][i]);
            if (image)
            {
                g->drawImage(image, itemX, itemY);
                image->decRef();
            }
        }
    }
    if (mItemMoved)
    {
        // Draw the item image being dragged by the cursor.
        const Image *const image = mItemMoved->getImage();
        if (image)
        {
            const int tPosX = mCursorPosX - (image->mBounds.w / 2);
            const int tPosY = mCursorPosY - (image->mBounds.h / 2);

            g->drawImage(image, tPosX, tPosY);
        }
    }
    BLOCK_END("OutfitWindow::draw")
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
            const unsigned char itemColor = mItemColors[mCurrentOutfit][index];
            if (itemId < 0)
            {
                Window::mouseDragged(event);
                return;
            }
            mMoved = false;
            event.consume();
            const Inventory *const inv = PlayerInfo::getInventory();
            if (inv)
            {
                Item *const item = inv->findItem(itemId, itemColor);
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

        if (inventoryWindow)
            inventoryWindow->unselectItem();
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

int OutfitWindow::getIndexFromGrid(const int pointX, const int pointY) const
{
    const gcn::Rectangle tRect = gcn::Rectangle(
        10, 25, mGridWidth * mBoxWidth, mGridHeight * mBoxHeight);
    if (!tRect.isPointInRect(pointX, pointY))
        return -1;
    const int index = (((pointY - 25) / mBoxHeight) * mGridWidth) +
        (pointX - 10) / mBoxWidth;
    if (index >= static_cast<int>(OUTFIT_ITEM_COUNT) || index < 0)
        return -1;
    return index;
}

void OutfitWindow::unequipNotInOutfit(const int outfit) const
{
    // here we think that outfit is correct index

    const Inventory *const inventory = PlayerInfo::getInventory();
    if (!inventory)
        return;

    for (unsigned i = 0; i < inventory->getSize(); i++)
    {
        const Item *const item = inventory->getItem(i);
        if (item && item->isEquipped())
        {
            bool found = false;
            for (unsigned f = 0; f < OUTFIT_ITEM_COUNT; f++)
            {
                if (item->getId() == mItems[outfit][f])
                {
                    found = true;
                    break;
                }
            }
            if (!found)
                Net::getInventoryHandler()->unequipItem(item);
        }
    }
}

std::string OutfitWindow::keyName(const int number) const
{
    if (number < 0 || number >= SHORTCUT_EMOTES)
        return "";
    return inputManager.getKeyStringLong(static_cast<int>(
        Input::KEY_EMOTE_1) + number);
}

void OutfitWindow::next()
{
    if (mCurrentOutfit < (static_cast<int>(OUTFITS_COUNT) - 1))
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
    if (mCurrentOutfit < static_cast<int>(OUTFITS_COUNT))
        mUnequipCheck->setSelected(mItemsUnequip[mCurrentOutfit]);
    else
        mUnequipCheck->setSelected(false);
    mKeyLabel->setCaption(strprintf(_("Key: %s"),
        keyName(mCurrentOutfit).c_str()));
    mAwayOutfitCheck->setSelected(mAwayOutfit == mCurrentOutfit);
}

void OutfitWindow::wearNextOutfit(const bool all)
{
    next();
    if (!all && mCurrentOutfit < static_cast<int>(OUTFITS_COUNT))
    {
        bool fromStart = false;
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

void OutfitWindow::wearPreviousOutfit(const bool all)
{
    previous();
    if (!all && mCurrentOutfit < static_cast<int>(OUTFITS_COUNT))
    {
        bool fromStart = false;
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

void OutfitWindow::copyFromEquiped(const int dst)
{
    const Inventory *const inventory = PlayerInfo::getInventory();
    if (!inventory)
        return;

    int outfitCell = 0;

    for (unsigned i = 0, sz = inventory->getSize(); i < sz; i++)
    {
        const Item *const item = inventory->getItem(i);
        if (item && item->isEquipped())
        {
            mItems[dst][outfitCell] = item->getId();
            mItemColors[dst][outfitCell++] = item->getColor();
            if (outfitCell >= static_cast<int>(OUTFIT_ITEM_COUNT))
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

void OutfitWindow::setItemSelected(const Item *const item)
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
