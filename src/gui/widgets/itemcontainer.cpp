/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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
#include "inventory.h"
#include "item.h"
#include "itemshortcut.h"

#include "being/playerinfo.h"

#include "gui/gui.h"
#include "gui/viewport.h"

#include "gui/popups/itempopup.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/shortcutwindow.h"

#include "net/net.h"
#include "net/inventoryhandler.h"
#include "net/tradehandler.h"

#include "utils/gettext.h"

#include "resources/image.h"

#include "gui/font.h"
#include "listeners/selectionlistener.h"

#include <algorithm>

#include "debug.h"

namespace
{
    class ItemIdPair final
    {
        public:
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
            bool operator() (const ItemIdPair *const pair1,
                             const ItemIdPair *const pair2) const
            {
                if (!pair1 || !pair2)
                    return false;

                return (pair1->mItem->getInfo().getName()
                        < pair2->mItem->getInfo().getName());
            }
    } itemAlphaInvSorter;

    class SortItemIdFunctor final
    {
        public:
            bool operator() (const ItemIdPair *const pair1,
                             const ItemIdPair *const pair2) const
            {
                if (!pair1 || !pair2)
                    return false;

                return pair1->mItem->getId() < pair2->mItem->getId();
            }
    } itemIdInvSorter;

    class SortItemWeightFunctor final
    {
        public:
            bool operator() (const ItemIdPair *const pair1,
                             const ItemIdPair *const pair2) const
            {
                if (!pair1 || !pair2)
                    return false;

                const int w1 = pair1->mItem->getInfo().getWeight();
                const int w2 = pair2->mItem->getInfo().getWeight();
                if (w1 == w2)
                {
                    return (pair1->mItem->getInfo().getName()
                            < pair2->mItem->getInfo().getName());
                }
                return w1 < w2;
            }
    } itemWeightInvSorter;

    class SortItemAmountFunctor final
    {
        public:
            bool operator() (const ItemIdPair *const pair1,
                             const ItemIdPair *const pair2) const
            {
                if (!pair1 || !pair2)
                    return false;

                const int c1 = pair1->mItem->getQuantity();
                const int c2 = pair2->mItem->getQuantity();
                if (c1 == c2)
                {
                    return (pair1->mItem->getInfo().getName()
                            < pair2->mItem->getInfo().getName());
                }
                return c1 < c2;
            }
    } itemAmountInvSorter;

    class SortItemTypeFunctor final
    {
        public:
            bool operator() (const ItemIdPair *const pair1,
                             const ItemIdPair *const pair2) const
            {
                if (!pair1 || !pair2)
                    return false;

                const int t1 = pair1->mItem->getInfo().getType();
                const int t2 = pair2->mItem->getInfo().getType();
                if (t1 == t2)
                {
                    return (pair1->mItem->getInfo().getName()
                            < pair2->mItem->getInfo().getName());
                }
                return t1 < t2;
            }
    } itemTypeInvSorter;
}  // namespace

ItemContainer::ItemContainer(const Widget2 *const widget,
                             Inventory *const inventory,
                             const bool forceQuantity) :
    Widget(widget),
    KeyListener(),
    MouseListener(),
    WidgetListener(),
    mInventory(inventory),
    mSelImg(Theme::getImageFromThemeXml("item_selection.xml", "")),
    mProtectedImg(Theme::getImageFromTheme("lock.png")),
    mName(),
    mItemPopup(new ItemPopup),
    mShowMatrix(nullptr),
    mSkin(Theme::instance() ? Theme::instance()->load(
          "itemcontainer.xml", "") : nullptr),
    mEquipedColor(getThemeColor(Theme::ITEM_EQUIPPED)),
    mEquipedColor2(getThemeColor(Theme::ITEM_EQUIPPED_OUTLINE)),
    mUnEquipedColor(getThemeColor(Theme::ITEM_NOT_EQUIPPED)),
    mUnEquipedColor2(getThemeColor(Theme::ITEM_NOT_EQUIPPED_OUTLINE)),
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
    mSelectionStatus(SEL_NONE),
    mForceQuantity(forceQuantity),
    mDescItems(false)
{
    mItemPopup->postInit();
    setFocusable(true);
    addKeyListener(this);
    addMouseListener(this);
    addWidgetListener(this);
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

    if (Theme::instance())
        Theme::instance()->unload(mSkin);

    delete mItemPopup;
    mItemPopup = nullptr;
    delete []mShowMatrix;
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

void ItemContainer::draw(Graphics *graphics)
{
    if (!mInventory || !mShowMatrix)
        return;

    BLOCK_START("ItemContainer::draw")
    Font *const font = getFont();

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
            if (item->getQuantity() > 1 || mForceQuantity)
            {
                caption = toString(item->getQuantity());
            }
            else if (item->isEquipped())
            {
                // TRANSLATORS: Text under equipped items (should be small)
                caption = _("Eq.");
            }

            if (item->isEquipped())
                graphics->setColorAll(mEquipedColor, mEquipedColor2);
            else
                graphics->setColorAll(mUnEquipedColor, mUnEquipedColor2);

            font->drawString(graphics, caption,
                itemX + (mBoxWidth - font->getWidth(caption)) / 2,
                itemY + mEquippedTextPadding);
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

void ItemContainer::hidePopup()
{
    if (mItemPopup)
        mItemPopup->setVisible(false);
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

    const int button = event.getButton();
    mClicks = event.getClickCount();

    if (button == MouseEvent::LEFT || button == MouseEvent::RIGHT)
    {
        const int index = getSlotIndex(event.getX(), event.getY());
        if (index == Inventory::NO_SLOT_INDEX)
            return;

        Item *const item = mInventory->getItem(index);

        // put item name into chat window
        if (item && mDescItems && chatWindow)
            chatWindow->addItemText(item->getInfo().getName());

        DragDropSource src = DRAGDROP_SOURCE_EMPTY;
        switch (mInventory->getType())
        {
            case Inventory::INVENTORY:
                src = DRAGDROP_SOURCE_INVENTORY;
                break;
            case Inventory::STORAGE:
                src = DRAGDROP_SOURCE_STORAGE;
                break;
            case Inventory::TRADE:
                src = DRAGDROP_SOURCE_TRADE;
                break;
            case Inventory::NPC:
                src = DRAGDROP_SOURCE_NPC;
                break;
            default:
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
                    if (num >= 0 && num < static_cast<int>(SHORTCUT_TABS))
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
    if (mClicks == 2)
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
        Net::getInventoryHandler()->moveItem(mSelectedIndex, index);
        selectNone();
    }
    else
    {
        const DragDropSource src = dragDrop.getSource();
        DragDropSource dst = DRAGDROP_SOURCE_EMPTY;
        switch (mInventory->getType())
        {
            case Inventory::INVENTORY:
                dst = DRAGDROP_SOURCE_INVENTORY;
                break;
            case Inventory::STORAGE:
                dst = DRAGDROP_SOURCE_STORAGE;
                break;
            case Inventory::TRADE:
                dst = DRAGDROP_SOURCE_TRADE;
                break;
            case Inventory::NPC:
                dst = DRAGDROP_SOURCE_NPC;
                break;
            default:
                break;
        }
        int srcContainer = -1;
        int dstContainer = -1;
        bool checkProtection(false);
        Inventory *inventory = nullptr;
        if (src == DRAGDROP_SOURCE_INVENTORY
            && dst == DRAGDROP_SOURCE_STORAGE)
        {
            srcContainer = Inventory::INVENTORY;
            dstContainer = Inventory::STORAGE;
            inventory = PlayerInfo::getInventory();
        }
        else if (src == DRAGDROP_SOURCE_STORAGE
                 && dst == DRAGDROP_SOURCE_INVENTORY)
        {
            srcContainer = Inventory::STORAGE;
            dstContainer = Inventory::INVENTORY;
            inventory = PlayerInfo::getStorageInventory();
        }
        if (src == DRAGDROP_SOURCE_INVENTORY
            && dst == DRAGDROP_SOURCE_TRADE)
        {
            checkProtection = true;
            inventory = PlayerInfo::getInventory();
        }
        else if (src == DRAGDROP_SOURCE_INVENTORY
                 && dst == DRAGDROP_SOURCE_NPC)
        {
            inventory = PlayerInfo::getInventory();
            const Item *const item = inventory->getItem(dragDrop.getTag());
            if (item && !PlayerInfo::isItemProtected(item->getId()))
                mInventory->addItem(item->getId(), 1, 1, item->getColor());
            return;
        }
        else if (src == DRAGDROP_SOURCE_NPC)
        {
            inventory = PlayerInfo::getInventory();
            const Item *const item = inventory->getItem(dragDrop.getTag());
            if (item)
                mInventory->removeItemAt(dragDrop.getTag());
            return;
        }

        if (inventory)
        {
            const Item *const item = inventory->getItem(dragDrop.getTag());
            if (item)
            {
                if (srcContainer != -1)
                {   // inventory <--> storage
                    Net::getInventoryHandler()->moveItem2(srcContainer,
                        item->getInvIndex(), item->getQuantity(),
                        dstContainer);
                }
                else
                {   // inventory --> trade
                    if (!checkProtection || !PlayerInfo::isItemProtected(
                        item->getId()))
                    {
                        Net::getTradeHandler()->addItem(item,
                            item->getQuantity());
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
        mItemPopup->setItem(item);
        mItemPopup->position(viewport->getMouseX(), viewport->getMouseY());
    }
    else
    {
        mItemPopup->setVisible(false);
    }
}

void ItemContainer::mouseExited(MouseEvent &event A_UNUSED)
{
    mItemPopup->setVisible(false);
}

void ItemContainer::widgetResized(const Event &event A_UNUSED)
{
    mGridColumns = std::max(1, mDimension.width / mBoxWidth);
    adjustHeight();
}

void ItemContainer::adjustHeight()
{
    if (!mGridColumns)
        return;

    mGridRows = (mLastUsedSlot + 1) / mGridColumns;
    if (mGridRows == 0 || (mLastUsedSlot + 1) % mGridColumns > 0)
        ++mGridRows;

    setHeight(mGridRows * mBoxHeight);

    updateMatrix();
}

void ItemContainer::updateMatrix()
{
    if (!mInventory)
        return;

    delete []mShowMatrix;
    mShowMatrix = new int[mGridRows * mGridColumns];

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

int ItemContainer::getSlotIndex(const int x, const int y) const
{
    if (!mShowMatrix)
        return Inventory::NO_SLOT_INDEX;

    if (x < mDimension.width && y < mDimension.height && x >= 0 && y >= 0)
    {
        const int idx = (y / mBoxHeight) * mGridColumns + (x / mBoxWidth);
        if (idx >= 0 && idx < mGridRows * mGridColumns
            && mShowMatrix[idx] >= 0)
        {
            return mShowMatrix[idx];
        }
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
