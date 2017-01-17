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

#include "gui/widgets/itemcontainer.h"

#include "dragdrop.h"

#include "being/playerinfo.h"

#include "gui/gui.h"
#include "gui/skin.h"
#include "gui/viewport.h"

#include "gui/fonts/font.h"

#include "gui/shortcut/itemshortcut.h"

#include "gui/popups/itempopup.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/inventorywindow.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/shortcutwindow.h"
#include "gui/windows/itemamountwindow.h"
#include "gui/windows/npcdialog.h"

#include "input/inputmanager.h"

#include "net/inventoryhandler.h"
#include "net/npchandler.h"
#include "net/serverfeatures.h"
#include "net/tradehandler.h"

#include "utils/delete2.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "render/vertexes/imagecollection.h"

#include "resources/iteminfo.h"

#include <algorithm>

#include "debug.h"

namespace
{
    class ItemIdPair final
    {
        public:
            A_DELETE_COPY(ItemIdPair)

            ItemIdPair(const int id, Item *const item) :
                mId(id), mItem(item)
            {
            }

            int mId;
            Item* mItem;
    };

    class SortItemAlphaFunctor final
    {
        public:
            A_DEFAULT_COPY(SortItemAlphaFunctor)

            bool operator() (const ItemIdPair *const pair1,
                             const ItemIdPair *const pair2) const
            {
                const Item *const item1 = pair1->mItem;
                const Item *const item2 = pair2->mItem;
                if (!item1 || !item2)
                    return false;

                const std::string name1 = item1->getInfo().getName(
                    item1->getColor());
                const std::string name2 = item2->getInfo().getName(
                    item2->getColor());
                if (name1 == name2)
                {
                    return item1->getInvIndex() <
                        item2->getInvIndex();
                }
                return (name1 < name2);
            }
    } itemAlphaInvSorter;

    class SortItemIdFunctor final
    {
        public:
            A_DEFAULT_COPY(SortItemIdFunctor)

            bool operator() (const ItemIdPair *const pair1,
                             const ItemIdPair *const pair2) const
            {
                if (!pair1->mItem || !pair2->mItem)
                    return false;

                const int id1 = pair1->mItem->getId();
                const int id2 = pair2->mItem->getId();
                if (id1 == id2)
                {
                    return pair1->mItem->getInvIndex() <
                        pair2->mItem->getInvIndex();
                }
                return id1 < id2;
            }
    } itemIdInvSorter;

    class SortItemWeightFunctor final
    {
        public:
            A_DEFAULT_COPY(SortItemWeightFunctor)

            bool operator() (const ItemIdPair *const pair1,
                             const ItemIdPair *const pair2) const
            {
                if (!pair1->mItem || !pair2->mItem)
                    return false;

                const int w1 = pair1->mItem->getInfo().getWeight();
                const int w2 = pair2->mItem->getInfo().getWeight();
                if (w1 == w2)
                {
                    const std::string name1 =
                        pair1->mItem->getInfo().getName();
                    const std::string name2 =
                        pair2->mItem->getInfo().getName();
                    if (name1 == name2)
                    {
                        return pair1->mItem->getInvIndex() <
                            pair2->mItem->getInvIndex();
                    }
                    return (name1 < name2);
                }
                return w1 < w2;
            }
    } itemWeightInvSorter;

    class SortItemAmountFunctor final
    {
        public:
            A_DEFAULT_COPY(SortItemAmountFunctor)

            bool operator() (const ItemIdPair *const pair1,
                             const ItemIdPair *const pair2) const
            {
                if (!pair1->mItem || !pair2->mItem)
                    return false;

                const int c1 = pair1->mItem->getQuantity();
                const int c2 = pair2->mItem->getQuantity();
                if (c1 == c2)
                {
                    const std::string name1 =
                        pair1->mItem->getInfo().getName();
                    const std::string name2 =
                        pair2->mItem->getInfo().getName();
                    if (name1 == name2)
                    {
                        return pair1->mItem->getInvIndex() <
                            pair2->mItem->getInvIndex();
                    }
                    return (name1 < name2);
                }
                return c1 < c2;
            }
    } itemAmountInvSorter;

    class SortItemTypeFunctor final
    {
        public:
            A_DEFAULT_COPY(SortItemTypeFunctor)

            bool operator() (const ItemIdPair *const pair1,
                             const ItemIdPair *const pair2) const
            {
                if (!pair1->mItem || !pair2->mItem)
                    return false;

                const ItemInfo &info1 = pair1->mItem->getInfo();
                const ItemInfo &info2 = pair2->mItem->getInfo();
                const ItemDbTypeT t1 = info1.getType();
                const ItemDbTypeT t2 = info2.getType();
                if (t1 == t2)
                {
                    const std::string name1 = info1.getName();
                    const std::string name2 = info2.getName();
                    if (name1 == name2)
                    {
                        return pair1->mItem->getInvIndex() <
                            pair2->mItem->getInvIndex();
                    }
                    return (name1 < name2);
                }
                return t1 < t2;
            }
    } itemTypeInvSorter;
}  // namespace

ItemContainer::ItemContainer(const Widget2 *const widget,
                             Inventory *const inventory,
                             const int maxColumns,
                             const ShowEmptyRows showEmptyRows,
                             const ForceQuantity forceQuantity) :
    Widget(widget),
    KeyListener(),
    MouseListener(),
    WidgetListener(),
    mInventory(inventory),
    mSelImg(Theme::getImageFromThemeXml("item_selection.xml", "")),
    mProtectedImg(Theme::getImageFromTheme("lock.png")),
    mCellBackgroundImg(Theme::getImageFromThemeXml("inventory_cell.xml", "")),
    mName(),
    mShowMatrix(nullptr),
    mSkin(theme ? theme->load("itemcontainer.xml", "") : nullptr),
    mVertexes(new ImageCollection),
    mEquipedColor(getThemeColor(ThemeColorId::ITEM_EQUIPPED)),
    mEquipedColor2(getThemeColor(ThemeColorId::ITEM_EQUIPPED_OUTLINE)),
    mUnEquipedColor(getThemeColor(ThemeColorId::ITEM_NOT_EQUIPPED)),
    mUnEquipedColor2(getThemeColor(ThemeColorId::ITEM_NOT_EQUIPPED_OUTLINE)),
    mSelectionListeners(),
    mGridColumns(1),
    mGridRows(1),
    mSelectedIndex(-1),
    mLastUsedSlot(-1),
    mTag(0),
    mSortType(0),
    mClicks(1),
    mBoxWidth(mSkin ? mSkin->getOption("boxWidth", 35) : 35),
    mBoxHeight(mSkin ? mSkin->getOption("boxHeight", 43) : 43),
    mEquippedTextPadding(mSkin ? mSkin->getOption(
                         "equippedTextPadding", 29) : 29),
    mPaddingItemX(mSkin ? mSkin->getOption("paddingItemX", 0) : 0),
    mPaddingItemY(mSkin ? mSkin->getOption("paddingItemY", 0) : 0),
    mMaxColumns(maxColumns),
    mSelectionStatus(SEL_NONE),
    mForceQuantity(forceQuantity),
    mShowEmptyRows(showEmptyRows),
    mDescItems(false)
{
    setFocusable(true);
    addKeyListener(this);
    addMouseListener(this);
    addWidgetListener(this);
    mAllowLogic = false;
}

ItemContainer::~ItemContainer()
{
    if (gui)
        gui->removeDragged(this);

    if (mSelImg)
    {
        mSelImg->decRef();
        mSelImg = nullptr;
    }
    if (mProtectedImg)
    {
        mProtectedImg->decRef();
        mProtectedImg = nullptr;
    }

    if (theme)
        theme->unload(mSkin);

    delete []mShowMatrix;
    delete2(mVertexes);
}

void ItemContainer::logic()
{
    BLOCK_START("ItemContainer::logic")
    Widget::logic();

    if (!mInventory)
    {
        BLOCK_END("ItemContainer::logic")
        return;
    }

    const int lastUsedSlot = mInventory->getLastUsedSlot();

    if (lastUsedSlot != mLastUsedSlot)
    {
        mLastUsedSlot = lastUsedSlot;
        adjustHeight();
    }
    BLOCK_END("ItemContainer::logic")
}

void ItemContainer::draw(Graphics *const graphics)
{
    if (!mInventory || !mShowMatrix)
        return;

    BLOCK_START("ItemContainer::draw")
    Font *const font = getFont();

    if (mCellBackgroundImg)
    {
        if (mRedraw || graphics->getRedraw())
        {
            mRedraw = false;
            mVertexes->clear();

            const int invSize = mInventory->getSize();
            const int maxRows = mShowEmptyRows == ShowEmptyRows_true ?
                std::max(invSize / mGridColumns, mGridRows) : mGridRows;
            const int maxColumns = mGridColumns > invSize ?
                invSize : mGridColumns;
            for (int j = 0; j < maxRows; j++)
            {
                const int intY0 = j * mBoxHeight;
                for (int i = 0; i < maxColumns; i++)
                {
                    int itemX = i * mBoxWidth;
                    int itemY = intY0;
                    graphics->calcTileCollection(mVertexes,
                        mCellBackgroundImg,
                        itemX + mPaddingItemX,
                        itemY + mPaddingItemY);
                }
            }
            graphics->finalize(mVertexes);
        }
        graphics->drawTileCollection(mVertexes);
    }

    for (int j = 0; j < mGridRows; j++)
    {
        const int intY0 = j * mBoxHeight;
        int itemIndex = j * mGridColumns - 1;
        for (int i = 0; i < mGridColumns; i++)
        {
            int itemX = i * mBoxWidth;
            int itemY = intY0;
            itemIndex ++;
            if (mShowMatrix[itemIndex] < 0)
                continue;

            const Item *const item = mInventory->getItem(
                mShowMatrix[itemIndex]);

            if (!item || item->getId() == 0)
                continue;

            Image *const image = item->getImage();
            if (image)
            {
                if (mShowMatrix[itemIndex] == mSelectedIndex)
                {
                    if (mSelImg)
                        graphics->drawImage(mSelImg, itemX, itemY);
                }
                image->setAlpha(1.0F);  // ensure the image if fully drawn...
                graphics->drawImage(image,
                    itemX + mPaddingItemX,
                    itemY + mPaddingItemY);
                if (mProtectedImg && PlayerInfo::isItemProtected(
                    item->getId()))
                {
                    graphics->drawImage(mProtectedImg,
                        itemX + mPaddingItemX,
                        itemY + mPaddingItemY);
                }
            }
        }
    }

    for (int j = 0; j < mGridRows; j++)
    {
        const int intY0 = j * mBoxHeight;
        int itemIndex = j * mGridColumns - 1;
        for (int i = 0; i < mGridColumns; i++)
        {
            int itemX = i * mBoxWidth;
            int itemY = intY0;
            itemIndex ++;
            if (mShowMatrix[itemIndex] < 0)
                continue;

            const Item *const item = mInventory->getItem(
                mShowMatrix[itemIndex]);

            if (!item || item->getId() == 0)
                continue;

            // Draw item caption
            std::string caption;
            if (item->getQuantity() > 1 ||
                mForceQuantity == ForceQuantity_true)
            {
                caption = toString(item->getQuantity());
            }
            else if (item->isEquipped() == Equipped_true)
            {
                // TRANSLATORS: Text under equipped items (should be small)
                caption = _("Eq.");
            }

            if (item->isEquipped() == Equipped_true)
            {
                font->drawString(graphics,
                    mEquipedColor, mEquipedColor2,
                    caption,
                    itemX + (mBoxWidth - font->getWidth(caption)) / 2,
                    itemY + mEquippedTextPadding);
            }
            else
            {
                font->drawString(graphics,
                    mUnEquipedColor, mUnEquipedColor2,
                    caption,
                    itemX + (mBoxWidth - font->getWidth(caption)) / 2,
                    itemY + mEquippedTextPadding);
            }
        }
    }
    BLOCK_END("ItemContainer::draw")
}

void ItemContainer::safeDraw(Graphics *const graphics)
{
    if (!mInventory || !mShowMatrix)
        return;

    BLOCK_START("ItemContainer::draw")
    Font *const font = getFont();

    if (mCellBackgroundImg)
    {
        const int invSize = mInventory->getSize();
        const int maxRows = mShowEmptyRows == ShowEmptyRows_true ?
            std::max(invSize / mGridColumns, mGridRows) : mGridRows;
        const int maxColumns = mGridColumns > invSize ?
            invSize : mGridColumns;
        for (int j = 0; j < maxRows; j++)
        {
            const int intY0 = j * mBoxHeight;
            for (int i = 0; i < maxColumns; i++)
            {
                int itemX = i * mBoxWidth;
                int itemY = intY0;
                graphics->drawImage(mCellBackgroundImg,
                    itemX + mPaddingItemX,
                    itemY + mPaddingItemY);
            }
        }
    }

    for (int j = 0; j < mGridRows; j++)
    {
        const int intY0 = j * mBoxHeight;
        int itemIndex = j * mGridColumns - 1;
        for (int i = 0; i < mGridColumns; i++)
        {
            int itemX = i * mBoxWidth;
            int itemY = intY0;
            itemIndex ++;
            if (mShowMatrix[itemIndex] < 0)
                continue;

            const Item *const item = mInventory->getItem(
                mShowMatrix[itemIndex]);

            if (!item || item->getId() == 0)
                continue;

            Image *const image = item->getImage();
            if (image)
            {
                if (mShowMatrix[itemIndex] == mSelectedIndex)
                {
                    if (mSelImg)
                        graphics->drawImage(mSelImg, itemX, itemY);
                }
                image->setAlpha(1.0F);  // ensure the image if fully drawn...
                graphics->drawImage(image,
                    itemX + mPaddingItemX,
                    itemY + mPaddingItemY);
                if (mProtectedImg && PlayerInfo::isItemProtected(
                    item->getId()))
                {
                    graphics->drawImage(mProtectedImg,
                        itemX + mPaddingItemX,
                        itemY + mPaddingItemY);
                }
            }
        }
    }

    for (int j = 0; j < mGridRows; j++)
    {
        const int intY0 = j * mBoxHeight;
        int itemIndex = j * mGridColumns - 1;
        for (int i = 0; i < mGridColumns; i++)
        {
            int itemX = i * mBoxWidth;
            int itemY = intY0;
            itemIndex ++;
            if (mShowMatrix[itemIndex] < 0)
                continue;

            const Item *const item = mInventory->getItem(
                mShowMatrix[itemIndex]);

            if (!item || item->getId() == 0)
                continue;

            // Draw item caption
            std::string caption;
            if (item->getQuantity() > 1 ||
                mForceQuantity == ForceQuantity_true)
            {
                caption = toString(item->getQuantity());
            }
            else if (item->isEquipped() == Equipped_true)
            {
                // TRANSLATORS: Text under equipped items (should be small)
                caption = _("Eq.");
            }

            if (item->isEquipped() == Equipped_true)
            {
                font->drawString(graphics,
                    mEquipedColor, mEquipedColor2,
                    caption,
                    itemX + (mBoxWidth - font->getWidth(caption)) / 2,
                    itemY + mEquippedTextPadding);
            }
            else
            {
                font->drawString(graphics,
                    mUnEquipedColor, mUnEquipedColor2,
                    caption,
                    itemX + (mBoxWidth - font->getWidth(caption)) / 2,
                    itemY + mEquippedTextPadding);
            }
        }
    }
    BLOCK_END("ItemContainer::draw")
}

void ItemContainer::selectNone()
{
    dragDrop.clear();

    setSelectedIndex(-1);
    mSelectionStatus = SEL_NONE;
/*
    if (outfitWindow)
        outfitWindow->setItemSelected(-1);
    if (shopWindow)
        shopWindow->setItemSelected(-1);
*/
}

void ItemContainer::setSelectedIndex(const int newIndex)
{
    if (mSelectedIndex != newIndex)
    {
        mSelectedIndex = newIndex;
        distributeValueChangedEvent();
    }
}

Item *ItemContainer::getSelectedItem() const
{
    if (mInventory)
        return mInventory->getItem(mSelectedIndex);
    else
        return nullptr;
}

void ItemContainer::distributeValueChangedEvent()
{
    FOR_EACH (SelectionListenerIterator, i, mSelectionListeners)
    {
        if (*i)
        {
            SelectionEvent event(this);
            (*i)->valueChanged(event);
        }
    }
}

void ItemContainer::keyPressed(KeyEvent &event A_UNUSED)
{
}

void ItemContainer::keyReleased(KeyEvent &event A_UNUSED)
{
}

void ItemContainer::mousePressed(MouseEvent &event)
{
    if (!mInventory)
        return;

    const MouseButtonT button = event.getButton();
    mClicks = event.getClickCount();

    if (button == MouseButton::LEFT || button == MouseButton::RIGHT)
    {
        event.consume();
        const int index = getSlotIndex(event.getX(), event.getY());
        if (index == Inventory::NO_SLOT_INDEX)
            return;

        Item *const item = mInventory->getItem(index);

        // put item name into chat window
        if (item && mDescItems && chatWindow)
            chatWindow->addItemText(item->getInfo().getName());

        DragDropSourceT src = DragDropSource::Empty;
        switch (mInventory->getType())
        {
            case InventoryType::Inventory:
                src = DragDropSource::Inventory;
                break;
            case InventoryType::Storage:
                src = DragDropSource::Storage;
                break;
            case InventoryType::Trade:
                src = DragDropSource::Trade;
                break;
            case InventoryType::Npc:
                src = DragDropSource::Npc;
                break;
            case InventoryType::Cart:
                src = DragDropSource::Cart;
                break;
            case InventoryType::Mail:
                src = DragDropSource::Mail;
                break;
            case InventoryType::Craft:
                src = DragDropSource::Craft;
                break;
            default:
            case InventoryType::Vending:
            case InventoryType::TypeEnd:
                break;
        }
        if (mSelectedIndex == index && mClicks != 2)
        {
            dragDrop.dragItem(item, src, index);
            dragDrop.select(item);
            mSelectionStatus = SEL_DESELECTING;
        }
        else if (item && item->getId())
        {
            if (index >= 0)
            {
                dragDrop.dragItem(item, src, index);
                dragDrop.select(item);
                setSelectedIndex(index);
                mSelectionStatus = SEL_SELECTING;

                if (itemShortcutWindow)
                {
                    const int num = itemShortcutWindow->getTabIndex();
                    if (num >= 0 && num < CAST_S32(SHORTCUT_TABS))
                    {
                        if (itemShortcut[num])
                            itemShortcut[num]->setItemSelected(item);
                    }
                }
                if (shopWindow)
                    shopWindow->setItemSelected(item->getId());
            }
        }
        else
        {
            dragDrop.deselect();
            selectNone();
        }
    }
}

void ItemContainer::mouseDragged(MouseEvent &event A_UNUSED)
{
    if (mSelectionStatus != SEL_NONE)
        mSelectionStatus = SEL_DRAGGING;
}

void ItemContainer::mouseReleased(MouseEvent &event)
{
    if (mClicks == 2 || !inventoryHandler || !tradeHandler)
        return;

    switch (mSelectionStatus)
    {
        case SEL_SELECTING:
            mSelectionStatus = SEL_SELECTED;
            break;
        case SEL_DESELECTING:
            selectNone();
            break;
        case SEL_DRAGGING:
            mSelectionStatus = SEL_SELECTED;
            break;
        case SEL_NONE:
        case SEL_SELECTED:
        default:
            break;
    };

    if (dragDrop.isEmpty())
    {
        const int index = getSlotIndex(event.getX(), event.getY());
        if (index == Inventory::NO_SLOT_INDEX)
            return;
        if (index == mSelectedIndex || mSelectedIndex == -1)
            return;
        inventoryHandler->moveItem(mSelectedIndex, index);
        selectNone();
    }
    else if (mInventory)
    {
        const DragDropSourceT src = dragDrop.getSource();
        DragDropSourceT dst = DragDropSource::Empty;
        switch (mInventory->getType())
        {
            case InventoryType::Inventory:
                dst = DragDropSource::Inventory;
                break;
            case InventoryType::Storage:
                dst = DragDropSource::Storage;
                break;
            case InventoryType::Trade:
                dst = DragDropSource::Trade;
                break;
            case InventoryType::Npc:
                dst = DragDropSource::Npc;
                break;
            case InventoryType::Mail:
                dst = DragDropSource::Mail;
                break;
            case InventoryType::Cart:
                dst = DragDropSource::Cart;
                break;
            case InventoryType::Craft:
                dst = DragDropSource::Craft;
                break;
            default:
            case InventoryType::Vending:
            case InventoryType::TypeEnd:
                break;
        }
        InventoryTypeT srcContainer = InventoryType::TypeEnd;
        InventoryTypeT dstContainer = InventoryType::TypeEnd;
        bool checkProtection(false);
        Inventory *inventory = nullptr;
        if (src == DragDropSource::Inventory
            && dst == DragDropSource::Storage)
        {
            srcContainer = InventoryType::Inventory;
            dstContainer = InventoryType::Storage;
            inventory = PlayerInfo::getInventory();
        }
        else if (src == DragDropSource::Storage
                 && dst == DragDropSource::Inventory)
        {
            srcContainer = InventoryType::Storage;
            dstContainer = InventoryType::Inventory;
            inventory = PlayerInfo::getStorageInventory();
        }
        if (src == DragDropSource::Inventory
            && dst == DragDropSource::Cart)
        {
            srcContainer = InventoryType::Inventory;
            dstContainer = InventoryType::Cart;
            inventory = PlayerInfo::getInventory();
        }
        if (src == DragDropSource::Inventory
            && dst == DragDropSource::Inventory)
        {
            if (!serverFeatures->haveCards())
                return;
            const int index = getSlotIndex(event.getX(), event.getY());
            if (index == Inventory::NO_SLOT_INDEX)
                return;
            if (index == mSelectedIndex || mSelectedIndex == -1)
                return;
            if (inventoryWindow)
                inventoryWindow->combineItems(index, mSelectedIndex);
            return;
        }
        else if (src == DragDropSource::Cart
                 && dst == DragDropSource::Inventory)
        {
            srcContainer = InventoryType::Cart;
            dstContainer = InventoryType::Inventory;
            inventory = PlayerInfo::getCartInventory();
        }
        else if (src == DragDropSource::Cart
                 && dst == DragDropSource::Storage)
        {
            srcContainer = InventoryType::Cart;
            dstContainer = InventoryType::Storage;
            inventory = PlayerInfo::getCartInventory();
        }
        else if (src == DragDropSource::Storage
                 && dst == DragDropSource::Cart)
        {
            srcContainer = InventoryType::Storage;
            dstContainer = InventoryType::Cart;
            inventory = PlayerInfo::getStorageInventory();
        }
        if (src == DragDropSource::Inventory
            && dst == DragDropSource::Trade)
        {
            checkProtection = true;
            inventory = PlayerInfo::getInventory();
        }
        else if (src == DragDropSource::Inventory
                 && (dst == DragDropSource::Npc
                 || dst == DragDropSource::Mail))
        {
            inventory = PlayerInfo::getInventory();
            if (inventory)
            {
                Item *const item = inventory->getItem(dragDrop.getTag());
                if (mInventory->addVirtualItem(
                    item,
                    getSlotByXY(event.getX(), event.getY()),
                    1))
                {
                    inventory->virtualRemove(item, 1);
                }
            }
            return;
        }
        else if (src == DragDropSource::Npc)
        {
            inventory = PlayerInfo::getInventory();
            if (dst == DragDropSource::Npc)
            {
                const Item *const item = mInventory->getItem(
                    dragDrop.getTag());
                const int index = getSlotByXY(event.getX(), event.getY());
                if (index == Inventory::NO_SLOT_INDEX)
                {
                    if (inventory)
                        inventory->virtualRestore(item, 1);
                    mInventory->removeItemAt(dragDrop.getTag());
                    return;
                }
                mInventory->removeItemAt(index);
                mInventory->setItem(index,
                    item->getId(),
                    item->getType(),
                    1,
                    1,
                    item->getColor(),
                    item->getIdentified(),
                    item->getDamaged(),
                    item->getFavorite(),
                    Equipm_false,
                    Equipped_false);
                Item *const item2 = mInventory->getItem(index);
                if (item2)
                    item2->setTag(item->getTag());
                mInventory->removeItemAt(dragDrop.getTag());
            }
            else
            {
                if (inventory)
                {
                    const Item *const item = inventory->getItem(
                        dragDrop.getTag());
                    if (item)
                    {
                        inventory->virtualRestore(item, 1);
                        mInventory->removeItemAt(dragDrop.getTag());
                    }
                }
                return;
            }
        }
        else if (src == DragDropSource::Inventory &&
                 dst == DragDropSource::Craft)
        {
            inventory = PlayerInfo::getInventory();
            if (inventory)
            {
                Item *const item = inventory->getItem(dragDrop.getTag());
                if (!item || item->isEquipped() == Equipped_true)
                    return;
                const int slot = getSlotByXY(event.getX(), event.getY());
                if (item->getQuantity() > 1
                    && !inputManager.isActionActive(InputAction::STOP_ATTACK))
                {
                    ItemAmountWindow::showWindow(
                        ItemAmountWindowUsage::CraftAdd,
                        npcHandler->getCurrentNpcDialog(),
                        item,
                        0,
                        slot);
                }
                else
                {
                    if (mInventory->addVirtualItem(
                        item,
                        slot,
                        1))
                    {
                        inventory->virtualRemove(item, 1);
                    }
                }
            }
            return;
        }
        else if (src == DragDropSource::Craft)
        {
            inventory = PlayerInfo::getInventory();
            if (dst == DragDropSource::Craft)
            {
                const Item *const item = mInventory->getItem(
                    dragDrop.getTag());
                const int index = getSlotByXY(event.getX(), event.getY());
                if (index == Inventory::NO_SLOT_INDEX)
                {
                    if (inventory)
                    {
                        inventory->virtualRestore(item,
                            item->getQuantity());
                        mInventory->removeItemAt(dragDrop.getTag());
                    }
                    return;
                }
                mInventory->moveItem(index, dragDrop.getTag());
            }
            else
            {
                if (inventory)
                {
                    const Item *const item = inventory->getItem(
                        dragDrop.getTag());
                    if (item)
                    {
                        inventory->virtualRestore(item,
                            item->getQuantity());
                        mInventory->removeItemAt(dragDrop.getTag());
                    }
                }
                return;
            }
        }
        else if (src == DragDropSource::Mail)
        {
            inventory = PlayerInfo::getInventory();
            if (inventory)
            {
                const Item *const item = inventory->getItem(dragDrop.getTag());
                if (item)
                    mInventory->removeItemAt(dragDrop.getTag());
            }
            return;
        }

        if (inventory)
        {
            const Item *const item = inventory->getItem(dragDrop.getTag());
            if (item)
            {
                if (srcContainer != InventoryType::TypeEnd)
                {   // inventory <--> storage, cart
                    inventoryHandler->moveItem2(srcContainer,
                        item->getInvIndex(),
                        item->getQuantity(),
                        dstContainer);
                }
                else
                {   // inventory --> trade
                    if (!checkProtection || !PlayerInfo::isItemProtected(
                        item->getId()))
                    {
                        tradeHandler->addItem(item, item->getQuantity());
                    }
                }
            }
        }
    }
}

void ItemContainer::mouseMoved(MouseEvent &event)
{
    if (!mInventory)
        return;

    const Item *const item = mInventory->getItem(
        getSlotIndex(event.getX(), event.getY()));

    if (item && viewport)
    {
        itemPopup->setItem(item, false);
        itemPopup->position(viewport->mMouseX, viewport->mMouseY);
    }
    else
    {
        itemPopup->setVisible(Visible_false);
    }
}

void ItemContainer::mouseExited(MouseEvent &event A_UNUSED)
{
    itemPopup->setVisible(Visible_false);
}

void ItemContainer::widgetResized(const Event &event A_UNUSED)
{
    updateSize();
}

void ItemContainer::updateSize()
{
    mGridColumns = std::min(mMaxColumns,
        std::max(1, mDimension.width / mBoxWidth));
    if (mGridColumns > mMaxColumns)
        mGridColumns = mMaxColumns;
    adjustHeight();
    mRedraw = true;
}

void ItemContainer::widgetMoved(const Event &event A_UNUSED)
{
    mRedraw = true;
}

void ItemContainer::adjustHeight()
{
    if (!mGridColumns)
        return;

    mGridRows = (mLastUsedSlot + 1) / mGridColumns;
    if (mGridRows == 0 || (mLastUsedSlot + 1) % mGridColumns > 0)
        ++mGridRows;

    const int invSize = mInventory->getSize();
    int maxRows = mShowEmptyRows == ShowEmptyRows_true ?
        std::max(invSize / mGridColumns, mGridRows) : mGridRows;

    if (mShowEmptyRows == ShowEmptyRows_true)
    {
        if (mGridColumns * maxRows < invSize)
            maxRows ++;
        mGridRows = maxRows;
    }

    setHeight(maxRows * mBoxHeight);
    updateMatrix();
}

void ItemContainer::updateMatrix()
{
    if (!mInventory)
        return;

    mRedraw = true;
    delete []mShowMatrix;
    mShowMatrix = new int[CAST_SIZE(mGridRows * mGridColumns)];

    std::vector<ItemIdPair*> sortedItems;
    int i = 0;
    int j = 0;

    std::string temp = mName;
    toLower(temp);

    const unsigned int invSize = mInventory->getSize();
    for (unsigned int idx = 0; idx < invSize; idx ++)
    {
        Item *const item = mInventory->getItem(idx);

        if (!item || item->getId() == 0 || !item->isHaveTag(mTag))
        {
            if (mShowEmptyRows == ShowEmptyRows_true)
                sortedItems.push_back(new ItemIdPair(idx, nullptr));
            continue;
        }

        if (!item->isHaveTag(mTag))
            continue;

        if (mName.empty())
        {
            sortedItems.push_back(new ItemIdPair(idx, item));
            continue;
        }
        std::string name = item->getInfo().getName();
        toLower(name);
        if (name.find(temp) != std::string::npos)
            sortedItems.push_back(new ItemIdPair(idx, item));
    }

    switch (mSortType)
    {
        case 0:
        default:
            break;
        case 1:
            std::sort(sortedItems.begin(), sortedItems.end(),
                itemAlphaInvSorter);
            break;
        case 2:
            std::sort(sortedItems.begin(), sortedItems.end(), itemIdInvSorter);
            break;
        case 3:
            std::sort(sortedItems.begin(), sortedItems.end(),
                itemWeightInvSorter);
            break;
        case 4:
            std::sort(sortedItems.begin(), sortedItems.end(),
                itemAmountInvSorter);
            break;
        case 5:
            std::sort(sortedItems.begin(), sortedItems.end(),
                itemTypeInvSorter);
            break;
    }

    int jMult = j * mGridColumns;
    const int maxSize = mGridRows * mGridColumns;
    FOR_EACH (std::vector<ItemIdPair*>::const_iterator, iter, sortedItems)
    {
        if (jMult >= maxSize)
            break;

        mShowMatrix[jMult + i] = (*iter)->mId;

        i ++;
        if (i >= mGridColumns)
        {
            i = 0;
            j ++;
            jMult += mGridColumns;
        }
    }

    for (int idx = j * mGridColumns + i; idx < maxSize; idx ++)
        mShowMatrix[idx] = -1;

    for (size_t idx = 0, sz = sortedItems.size(); idx < sz; idx ++)
        delete sortedItems[idx];
}

int ItemContainer::getSlotIndex(int x, int y) const
{
    if (!mShowMatrix)
        return Inventory::NO_SLOT_INDEX;

    if (x < mDimension.width && y < mDimension.height && x >= 0 && y >= 0)
    {
        if (x > mBoxWidth * mGridColumns)
            return Inventory::NO_SLOT_INDEX;
        const int idx = (y / mBoxHeight) * mGridColumns + (x / mBoxWidth);
        if (idx >= 0 && idx < mGridRows * mGridColumns
            && mShowMatrix[idx] >= 0)
        {
            return mShowMatrix[idx];
        }
    }

    return Inventory::NO_SLOT_INDEX;
}

int ItemContainer::getSlotByXY(int x, int y) const
{
    if (!mShowMatrix)
        return Inventory::NO_SLOT_INDEX;

    if (x < mDimension.width && y < mDimension.height && x >= 0 && y >= 0)
    {
        if (x > mBoxWidth * mGridColumns)
            return Inventory::NO_SLOT_INDEX;
        const int idx = (y / mBoxHeight) * mGridColumns + (x / mBoxWidth);
        if (idx >= 0 && idx < mGridRows * mGridColumns)
            return idx;
    }

    return Inventory::NO_SLOT_INDEX;
}

void ItemContainer::setFilter(const int tag)
{
    mTag = tag;
    updateMatrix();
}

void ItemContainer::setSortType(const int sortType)
{
    mSortType = sortType;
    updateMatrix();
}

void ItemContainer::setCellBackgroundImage(const std::string &xmlName)
{
    if (mCellBackgroundImg)
        mCellBackgroundImg->decRef();
    mCellBackgroundImg = Theme::getImageFromThemeXml(xmlName, "");
    mRedraw = true;
}

void ItemContainer::setMaxColumns(const int maxColumns)
{
    mMaxColumns = maxColumns;
    updateSize();
}
