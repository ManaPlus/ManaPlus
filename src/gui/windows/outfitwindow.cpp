/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/windows/outfitwindow.h"

#include "configuration.h"
#include "dragdrop.h"
#include "game.h"

#include "being/playerinfo.h"

#include "const/emoteshortcut.h"

#include "enums/gui/layouttype.h"

#include "input/inputactionoperators.h"
#include "input/inputmanager.h"

#include "gui/viewport.h"

#include "gui/popups/popupmenu.h"

#include "gui/windows/setupwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"

#include "utils/gettext.h"

#include "resources/inventory/inventory.h"

#include <sstream>

#include "debug.h"

OutfitWindow *outfitWindow = nullptr;

OutfitWindow::OutfitWindow() :
    // TRANSLATORS: outfits window name
    Window(_("Outfits"), Modal_false, nullptr, "outfits.xml"),
    ActionListener(),
    // TRANSLATORS: outfits window button
    mPreviousButton(new Button(this, _("<"), "previous", BUTTON_SKIN, this)),
    // TRANSLATORS: outfits window button
    mNextButton(new Button(this, _(">"), "next", BUTTON_SKIN, this)),
    // TRANSLATORS: outfits window button
    mEquipBottom(new Button(this, _("Equip"), "equip", BUTTON_SKIN, this)),
    // TRANSLATORS: outfits window label
    mCurrentLabel(new Label(this, strprintf(_("Outfit: %d"), 1))),
    // TRANSLATORS: outfits window checkbox
    mUnequipCheck(new CheckBox(this, _("Unequip first"),
        serverConfig.getValueBool("OutfitUnequip0", true),
        nullptr, std::string())),
    // TRANSLATORS: outfits window checkbox
    mAwayOutfitCheck(new CheckBox(this, _("Away outfit"),
        serverConfig.getValue("OutfitAwayIndex", OUTFITS_COUNT - 1) != 0u,
        nullptr, std::string())),
    // TRANSLATORS: outfits window label
    mKeyLabel(new Label(this, strprintf(_("Key: %s"),
        keyName(0).c_str()))),
    mBorderColor(getThemeColor(ThemeColorId::BORDER, 64)),
    mCurrentOutfit(0),
    mBoxWidth(33),
    mBoxHeight(33),
    mGridWidth(4),
    mGridHeight(4),
    mItems(),
    mAwayOutfit(0),
    mItemColors(),
    mItemClicked(false),
    mItemsUnequip()
{
    setWindowName("Outfits");
    setResizable(true);
    setCloseButton(true);
    setStickyButtonLock(true);

    mBackgroundColor = getThemeColor(ThemeColorId::BACKGROUND, 32);

    setDefaultSize(250, 400, 150, 290);
    setMinWidth(145);
    setMinHeight(220);

    if (setupWindow != nullptr)
        setupWindow->registerWindowForReset(this);

    mCurrentLabel->setAlignment(Graphics::CENTER);
    mKeyLabel->setAlignment(Graphics::CENTER);

    mUnequipCheck->setActionEventId("unequip");
    mUnequipCheck->addActionListener(this);

    mAwayOutfitCheck->setActionEventId("away");
    mAwayOutfitCheck->addActionListener(this);

    place(1, 3, mEquipBottom, 2, 1);
    place(0, 4, mKeyLabel, 4, 1);
    place(0, 5, mPreviousButton, 1, 1);
    place(1, 5, mCurrentLabel, 2, 1);
    place(3, 5, mNextButton, 1, 1);
    place(0, 6, mUnequipCheck, 4, 1);
    place(0, 7, mAwayOutfitCheck, 4, 1);

    Layout &layout = getLayout();
    layout.setRowHeight(0, LayoutType::SET);
    layout.setColWidth(4, Layout::CENTER);

    loadWindowState();

    enableVisibleSound(true);
    load();
}

OutfitWindow::~OutfitWindow()
{
    save();
}

void OutfitWindow::load()
{
    const Configuration *cfg = &serverConfig;

    memset(mItems, -1, sizeof(mItems));
    memset(mItemColors, 1, sizeof(mItemColors));

    for (unsigned o = 0; o < OUTFITS_COUNT; o++)
    {
        std::string outfit = cfg->getValue("Outfit" + toString(o), "-1");
        std::string buf;
        std::stringstream ss(outfit);

        STD_VECTOR<int> tokens;

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

        STD_VECTOR<unsigned char> tokens2;
        while (ss2 >> buf)
            tokens2.push_back(CAST_U8(atoi(buf.c_str())));

        for (size_t i = 0, sz = tokens2.size();
             i < sz && i < OUTFIT_ITEM_COUNT; i++)
        {
            mItemColors[o][i] = fromInt(tokens2[i], ItemColor);
        }

        mItemsUnequip[o] = cfg->getValueBool("OutfitUnequip" + toString(o),
                                             true);
    }
    mAwayOutfit = cfg->getValue("OutfitAwayIndex", OUTFITS_COUNT - 1);
    if (mAwayOutfit >= CAST_S32(OUTFITS_COUNT))
        mAwayOutfit = CAST_S32(OUTFITS_COUNT) - 1;

    if (mAwayOutfitCheck != nullptr)
        mAwayOutfitCheck->setSelected(mAwayOutfit == mCurrentOutfit);
}

void OutfitWindow::save() const
{
    std::string outfitStr;
    std::string outfitColorsStr;
    for (unsigned o = 0; o < OUTFITS_COUNT; o++)
    {
        bool good = false;
        for (unsigned i = 0; i < OUTFIT_ITEM_COUNT; i++)
        {
            const int val = mItems[o][i];
            const int res = val != 0 ? val : -1;
            if (res != -1)
                good = true;
            outfitStr.append(toString(res));
            if (i < OUTFIT_ITEM_COUNT - 1)
                outfitStr.append(" ");
            outfitColorsStr.append(toString(CAST_S32(
                toInt(mItemColors[o][i], int))));
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

void OutfitWindow::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
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
        if (mCurrentOutfit >= 0 && mCurrentOutfit < CAST_S32(
            OUTFITS_COUNT))
        {
            mItemsUnequip[mCurrentOutfit] = mUnequipCheck->isSelected();
        }
    }
    else if (eventId == "equip")
    {
        wearOutfit(mCurrentOutfit,
            true,
            false);
        if (Game::instance() != nullptr)
            Game::instance()->setValidSpeed();
    }
    else if (eventId == "away")
    {
        mAwayOutfit = mCurrentOutfit;
        if (!mAwayOutfitCheck->isSelected())
            mAwayOutfitCheck->setSelected(true);
    }
}

void OutfitWindow::wearOutfit(const int outfit,
                              const bool unwearEmpty,
                              const bool select)
{
    bool isEmpty = true;

    if (outfit < 0 || outfit > CAST_S32(OUTFITS_COUNT))
        return;

    for (unsigned i = 0; i < OUTFIT_ITEM_COUNT; i++)
    {
        const Item *const item = PlayerInfo::getInventory()->findItem(
            mItems[outfit][i],
            mItemColors[outfit][i]);
        if ((item != nullptr)
            && item->isEquipped() == Equipped_false
            && (item->getQuantity() != 0))
        {
            if (item->isEquipment() == Equipm_true)
            {
                PlayerInfo::equipItem(item, Sfx_false);
                isEmpty = false;
            }
        }
    }

    if ((!isEmpty || unwearEmpty) && outfit < CAST_S32(OUTFITS_COUNT)
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
    if (src < 0 || src > CAST_S32(OUTFITS_COUNT)
        || dst < 0 || dst > CAST_S32(OUTFITS_COUNT))
    {
        return;
    }

    for (unsigned int i = 0; i < OUTFIT_ITEM_COUNT; i++)
        mItems[dst][i] = mItems[src][i];
    save();
}

void OutfitWindow::draw(Graphics *const graphics)
{
    BLOCK_START("OutfitWindow::draw")
    Window::draw(graphics);

    if (mCurrentOutfit < 0 || mCurrentOutfit
        >= static_cast<signed int>(OUTFITS_COUNT))
    {
        return;
    }

    for (unsigned int i = 0; i < OUTFIT_ITEM_COUNT; i++)
    {
        const int itemX = mPadding + ((i % mGridWidth) * mBoxWidth);
        const int itemY = mPadding + mTitleBarHeight
            + ((i / CAST_U32(mGridWidth)) * mBoxHeight);

        graphics->setColor(mBorderColor);
        graphics->drawRectangle(Rect(itemX, itemY, 32, 32));
        graphics->setColor(mBackgroundColor);
        graphics->fillRectangle(Rect(itemX, itemY, 32, 32));

        if (mItems[mCurrentOutfit][i] < 0)
            continue;

        bool foundItem = false;
        const Inventory *const inv = PlayerInfo::getInventory();
        if (inv != nullptr)
        {
            const Item *const item = inv->findItem(mItems[mCurrentOutfit][i],
                mItemColors[mCurrentOutfit][i]);
            if (item != nullptr)
            {
                // Draw item icon.
                const Image *const image = item->getImage();
                if (image != nullptr)
                {
                    graphics->drawImage(image, itemX, itemY);
                    foundItem = true;
                }
            }
        }
        if (!foundItem)
        {
            Image *const image = Item::getImage(mItems[mCurrentOutfit][i],
                mItemColors[mCurrentOutfit][i]);
            if (image != nullptr)
            {
                graphics->drawImage(image, itemX, itemY);
                image->decRef();
            }
        }
    }
    BLOCK_END("OutfitWindow::draw")
}

void OutfitWindow::safeDraw(Graphics *const graphics)
{
    BLOCK_START("OutfitWindow::draw")
    Window::safeDraw(graphics);

    if (mCurrentOutfit < 0 || mCurrentOutfit
        >= static_cast<signed int>(OUTFITS_COUNT))
    {
        return;
    }

    for (unsigned int i = 0; i < OUTFIT_ITEM_COUNT; i++)
    {
        const int itemX = mPadding + ((i % mGridWidth) * mBoxWidth);
        const int itemY = mPadding + mTitleBarHeight
            + ((i / CAST_U32(mGridWidth)) * mBoxHeight);

        graphics->setColor(mBorderColor);
        graphics->drawRectangle(Rect(itemX, itemY, 32, 32));
        graphics->setColor(mBackgroundColor);
        graphics->fillRectangle(Rect(itemX, itemY, 32, 32));

        if (mItems[mCurrentOutfit][i] < 0)
            continue;

        bool foundItem = false;
        const Inventory *const inv = PlayerInfo::getInventory();
        if (inv != nullptr)
        {
            const Item *const item = inv->findItem(mItems[mCurrentOutfit][i],
                mItemColors[mCurrentOutfit][i]);
            if (item != nullptr)
            {
                // Draw item icon.
                const Image *const image = item->getImage();
                if (image != nullptr)
                {
                    graphics->drawImage(image, itemX, itemY);
                    foundItem = true;
                }
            }
        }
        if (!foundItem)
        {
            Image *const image = Item::getImage(mItems[mCurrentOutfit][i],
                mItemColors[mCurrentOutfit][i]);
            if (image != nullptr)
            {
                graphics->drawImage(image, itemX, itemY);
                image->decRef();
            }
        }
    }
    BLOCK_END("OutfitWindow::draw")
}

void OutfitWindow::mouseDragged(MouseEvent &event)
{
    if (event.getButton() == MouseButton::LEFT)
    {
        if (dragDrop.isEmpty() && mItemClicked)
        {
            if (mCurrentOutfit < 0 || mCurrentOutfit
                >= static_cast<signed int>(OUTFITS_COUNT))
            {
                Window::mouseDragged(event);
                return;
            }

            const int index = getIndexFromGrid(event.getX(), event.getY());
            if (index == -1)
            {
                Window::mouseDragged(event);
                return;
            }
            const int itemId = mItems[mCurrentOutfit][index];
            const ItemColor itemColor = mItemColors[mCurrentOutfit][index];
            if (itemId < 0)
            {
                Window::mouseDragged(event);
                return;
            }
            mMoved = false;
            event.consume();
            const Inventory *const inv = PlayerInfo::getInventory();
            if (inv != nullptr)
            {
                Item *const item = inv->findItem(itemId, itemColor);
                if (item != nullptr)
                    dragDrop.dragItem(item, DragDropSource::Outfit, 0);
                else
                    dragDrop.clear();
                mItems[mCurrentOutfit][index] = -1;
            }
        }
    }
    Window::mouseDragged(event);
}

void OutfitWindow::mousePressed(MouseEvent &event)
{
    const int index = getIndexFromGrid(event.getX(), event.getY());
    if (event.getButton() == MouseButton::RIGHT && (popupMenu != nullptr))
    {
        popupMenu->showOutfitsWindowPopup(viewport->mMouseX,
            viewport->mMouseY);
        event.consume();
        return;
    }
    else if (index == -1)
    {
        Window::mousePressed(event);
        return;
    }
    mMoved = false;
    event.consume();

    if (mItems[mCurrentOutfit][index] > 0)
    {
        mItemClicked = true;
    }
    else
    {
        if (dragDrop.isSelected())
        {
            mItems[mCurrentOutfit][index] = dragDrop.getSelected();
            mItemColors[mCurrentOutfit][index] = dragDrop.getSelectedColor();
            dragDrop.deselect();
            save();
        }
    }

    Window::mousePressed(event);
}

void OutfitWindow::mouseReleased(MouseEvent &event)
{
    if (event.getButton() == MouseButton::LEFT)
    {
        if (mCurrentOutfit < 0 || mCurrentOutfit
            >= static_cast<signed int>(OUTFITS_COUNT))
        {
            return;
        }
        const int index = getIndexFromGrid(event.getX(), event.getY());
        if (index == -1)
        {
            dragDrop.clear();
            Window::mouseReleased(event);
            return;
        }
        mMoved = false;
        event.consume();
        if (!dragDrop.isEmpty())
        {
            if (dragDrop.isSourceItemContainer())
            {
                mItems[mCurrentOutfit][index] = dragDrop.getItem();
                mItemColors[mCurrentOutfit][index] = dragDrop.getItemColor();
                dragDrop.clear();
                dragDrop.deselect();
                save();
            }
        }
        if (mItemClicked)
            mItemClicked = false;
    }
    Window::mouseReleased(event);
}

int OutfitWindow::getIndexFromGrid(const int pointX, const int pointY) const
{
    const Rect tRect = Rect(mPadding, mTitleBarHeight,
        mGridWidth * mBoxWidth, mGridHeight * mBoxHeight);
    if (!tRect.isPointInRect(pointX, pointY))
        return -1;
    const int index = (((pointY - mTitleBarHeight) / mBoxHeight) * mGridWidth)
        + (pointX - mPadding) / mBoxWidth;
    if (index >= CAST_S32(OUTFIT_ITEM_COUNT) || index < 0)
        return -1;
    return index;
}

void OutfitWindow::unequipNotInOutfit(const int outfit) const
{
    // here we think that outfit is correct index

    const Inventory *const inventory = PlayerInfo::getInventory();
    if (inventory == nullptr)
        return;

    const unsigned int invSize = inventory->getSize();
    for (unsigned i = 0; i < invSize; i++)
    {
        const Item *const item = inventory->getItem(i);
        if ((item != nullptr) && item->isEquipped() == Equipped_true)
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
                PlayerInfo::unequipItem(item, Sfx_false);
        }
    }
}

std::string OutfitWindow::keyName(const int number)
{
    if (number < 0 || number >= SHORTCUT_EMOTES)
        return "";
    return inputManager.getKeyStringLong(InputAction::OUTFIT_1 + number);
}

void OutfitWindow::next()
{
    if (mCurrentOutfit < (CAST_S32(OUTFITS_COUNT) - 1))
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
    // TRANSLATORS: outfits window label
    mCurrentLabel->setCaption(strprintf(_("Outfit: %d"), mCurrentOutfit + 1));
    if (mCurrentOutfit < CAST_S32(OUTFITS_COUNT))
        mUnequipCheck->setSelected(mItemsUnequip[mCurrentOutfit]);
    else
        mUnequipCheck->setSelected(false);
    // TRANSLATORS: outfits window label
    mKeyLabel->setCaption(strprintf(_("Key: %s"),
        keyName(mCurrentOutfit).c_str()));
    mAwayOutfitCheck->setSelected(mAwayOutfit == mCurrentOutfit);
}

void OutfitWindow::wearNextOutfit(const bool all)
{
    next();
    if (!all && mCurrentOutfit >= 0 && mCurrentOutfit
        < CAST_S32(OUTFITS_COUNT))
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
    wearOutfit(mCurrentOutfit,
        true,
        false);
}

void OutfitWindow::wearPreviousOutfit(const bool all)
{
    previous();
    if (!all && mCurrentOutfit >= 0 && mCurrentOutfit
        < CAST_S32(OUTFITS_COUNT))
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
    wearOutfit(mCurrentOutfit,
        true,
        false);
}

void OutfitWindow::copyFromEquiped()
{
    copyFromEquiped(mCurrentOutfit);
}

void OutfitWindow::copyFromEquiped(const int dst)
{
    const Inventory *const inventory = PlayerInfo::getInventory();
    if (inventory == nullptr)
        return;

    int outfitCell = 0;
    for (unsigned i = 0, sz = inventory->getSize(); i < sz; i++)
    {
        const Item *const item = inventory->getItem(i);
        if ((item != nullptr) && item->isEquipped() == Equipped_true)
        {
            mItems[dst][outfitCell] = item->getId();
            mItemColors[dst][outfitCell++] = item->getColor();
            if (outfitCell >= CAST_S32(OUTFIT_ITEM_COUNT))
                break;
        }
    }
    save();
}

void OutfitWindow::wearAwayOutfit()
{
    copyFromEquiped(OUTFITS_COUNT);
    wearOutfit(mAwayOutfit,
        false,
        false);
}

void OutfitWindow::unwearAwayOutfit()
{
    wearOutfit(OUTFITS_COUNT,
        true,
        false);
}

void OutfitWindow::clearCurrentOutfit()
{
    if (mCurrentOutfit < 0 || mCurrentOutfit
        >= static_cast<signed int>(OUTFITS_COUNT))
    {
        return;
    }
    for (unsigned f = 0; f < OUTFIT_ITEM_COUNT; f++)
    {
        mItems[mCurrentOutfit][f] = -1;
        mItemColors[mCurrentOutfit][f] = ItemColor_one;
    }
    save();
}

std::string OutfitWindow::getOutfitString() const
{
    std::string str;
    for (unsigned int i = 0; i < OUTFIT_ITEM_COUNT; i++)
    {
        const int id = mItems[mCurrentOutfit][i];
        if (id < 0)
            continue;

        const ItemColor color = mItemColors[mCurrentOutfit][i];
        STD_VECTOR<int> ids;
        ids.push_back(id);
        ids.push_back(CAST_S32(color));

        const std::string name = ItemDB::getNamesStr(ids);
        if (name.empty())
            continue;
        str.append("[");
        str.append(name);
        str.append("] ");
    }
    return str;
}
