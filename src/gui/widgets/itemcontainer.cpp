/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "gui/widgets/itemcontainer.h"

#include "inventory.h"
#include "item.h"
#include "itemshortcut.h"
#include "dropshortcut.h"

#include "gui/chatwindow.h"
#include "gui/gui.h"
#include "gui/itempopup.h"
#include "gui/outfitwindow.h"
#include "gui/shopwindow.h"
#include "gui/shortcutwindow.h"
#include "gui/sdlinput.h"
#include "gui/viewport.h"

#include "net/net.h"
#include "net/inventoryhandler.h"

#include "utils/gettext.h"

#include "resources/image.h"

#include <guichan/font.hpp>
#include <guichan/mouseinput.hpp>
#include <guichan/selectionlistener.hpp>

#include "debug.h"


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
} itemAlphaSorter;

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
} itemIdSorter;

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
} itemWeightSorter;

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
} itemAmountSorter;

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
} itemTypeSorter;

ItemContainer::ItemContainer(const Widget2 *const widget,
                             Inventory *const inventory,
                             const bool forceQuantity) :
    gcn::Widget(),
    Widget2(widget),
    gcn::KeyListener(),
    gcn::MouseListener(),
    gcn::WidgetListener(),
    mInventory(inventory),
    mGridColumns(1),
    mGridRows(1),
    mSelImg(Theme::getImageFromThemeXml("item_selection.xml", "")),
    mSelectedIndex(-1),
    mHighlightedIndex(-1),
    mLastUsedSlot(-1),
    mSelectionStatus(SEL_NONE),
    mForceQuantity(forceQuantity),
    mSwapItems(false),
    mDescItems(false),
    mDragPosX(0),
    mDragPosY(0),
    mTag(0),
    mSortType(0),
    mItemPopup(new ItemPopup),
    mShowMatrix(nullptr),
    mClicks(1),
    mSkin(Theme::instance() ? Theme::instance()->load(
          "itemcontainer.xml", "") : nullptr),
    mBoxWidth(mSkin ? mSkin->getOption("boxWidth", 35) : 35),
    mBoxHeight(mSkin ? mSkin->getOption("boxHeight", 43) : 43),
    mEquippedTextPadding(mSkin ? mSkin->getOption(
                         "equippedTextPadding", 29) : 29),
    mPaddingItemX(mSkin ? mSkin->getOption("paddingItemX", 0) : 0),
    mPaddingItemY(mSkin ? mSkin->getOption("paddingItemY", 0) : 0),
    mEquipedColor(getThemeColor(Theme::ITEM_EQUIPPED)),
    mUnEquipedColor(getThemeColor(Theme::ITEM_NOT_EQUIPPED))
{
    setFocusable(true);
    addKeyListener(this);
    addMouseListener(this);
    addWidgetListener(this);
}

ItemContainer::~ItemContainer()
{
    if (mSelImg)
    {
        mSelImg->decRef();
        mSelImg = nullptr;
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
    gcn::Widget::logic();

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

void ItemContainer::draw(gcn::Graphics *graphics)
{
    if (!mInventory || !mShowMatrix)
        return;

    BLOCK_START("ItemContainer::draw")
    Graphics *const g = static_cast<Graphics *const>(graphics);
    gcn::Font *const font = getFont();
    g->setFont(font);

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
                    if (mSelectionStatus == SEL_DRAGGING)
                    {
                        // Reposition the coords to that of the cursor.
                        itemX = mDragPosX - (mBoxWidth / 2);
                        itemY = mDragPosY - (mBoxHeight / 2);
                    }
                    else
                    {
                        // Draw selection border image.
                        if (mSelImg)
                            g->drawImage(mSelImg, itemX, itemY);
                    }
                }
                image->setAlpha(1.0f); // ensure the image if fully drawn...
                g->drawImage(image, itemX + mPaddingItemX,
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
                g->setColor(mEquipedColor);
            else
                g->setColor(mUnEquipedColor);

            font->drawString(g, caption,
                itemX + (mBoxWidth - font->getWidth(caption)) / 2,
                itemY + mEquippedTextPadding);
        }
    }
    BLOCK_END("ItemContainer::draw")
}

void ItemContainer::selectNone()
{
    setSelectedIndex(-1);
    mSelectionStatus = SEL_NONE;
    if (outfitWindow)
        outfitWindow->setItemSelected(-1);
    if (shopWindow)
        shopWindow->setItemSelected(-1);
//    if (skillDialog)
//        skillDialog->setItemSelected(-1);
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
    for (SelectionListenerIterator i = mSelectionListeners.begin(),
         i_end = mSelectionListeners.end();
         i != i_end; ++i)
    {
        if (*i)
        {
            gcn::SelectionEvent event(this);
            (*i)->valueChanged(event);
        }
    }
}

void ItemContainer::hidePopup()
{
    if (mItemPopup)
        mItemPopup->setVisible(false);
}

void ItemContainer::keyPressed(gcn::KeyEvent &event A_UNUSED)
{
}

void ItemContainer::keyReleased(gcn::KeyEvent &event A_UNUSED)
{
}

void ItemContainer::mousePressed(gcn::MouseEvent &event)
{
    if (!mInventory)
        return;

    const int button = event.getButton();
    mClicks = event.getClickCount();

    if (button == gcn::MouseEvent::LEFT || button == gcn::MouseEvent::RIGHT)
    {
        const int index = getSlotIndex(event.getX(), event.getY());
        if (index == Inventory::NO_SLOT_INDEX)
            return;

        Item *const item = mInventory->getItem(index);

        // put item name into chat window
        if (item && mDescItems && chatWindow)
            chatWindow->addItemText(item->getInfo().getName());

        if (mSelectedIndex == index && mClicks != 2)
        {
            mSelectionStatus = SEL_DESELECTING;
        }
        else if (item && item->getId())
        {
            setSelectedIndex(index);
            mSelectionStatus = SEL_SELECTING;

            const int num = itemShortcutWindow->getTabIndex();
            if (num >= 0 && num < static_cast<int>(SHORTCUT_TABS))
            {
                if (itemShortcut[num])
                    itemShortcut[num]->setItemSelected(item);
            }
            if (dropShortcut)
                dropShortcut->setItemSelected(item);
            if (item->isEquipment() && outfitWindow)
                outfitWindow->setItemSelected(item);
            if (shopWindow)
                shopWindow->setItemSelected(item->getId());
        }
        else
        {
            selectNone();
        }
    }
}

void ItemContainer::mouseDragged(gcn::MouseEvent &event)
{
    if (mSelectionStatus != SEL_NONE)
    {
        mSelectionStatus = SEL_DRAGGING;
        mDragPosX = event.getX();
        mDragPosY = event.getY();
    }
}

void ItemContainer::mouseReleased(gcn::MouseEvent &event)
{
    if (mClicks == 2)
        return;

    switch (mSelectionStatus)
    {
        case SEL_SELECTING:
            mSelectionStatus = SEL_SELECTED;
            return;
        case SEL_DESELECTING:
            selectNone();
            return;
        case SEL_DRAGGING:
            mSelectionStatus = SEL_SELECTED;
            break;
        case SEL_NONE:
        case SEL_SELECTED:
        default:
            return;
    };

    const int index = getSlotIndex(event.getX(), event.getY());
    if (index == Inventory::NO_SLOT_INDEX)
        return;
    if (index == mSelectedIndex || mSelectedIndex == -1)
        return;
    Net::getInventoryHandler()->moveItem(mSelectedIndex, index);
    selectNone();
}


// Show ItemTooltip
void ItemContainer::mouseMoved(gcn::MouseEvent &event)
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

// Hide ItemTooltip
void ItemContainer::mouseExited(gcn::MouseEvent &event A_UNUSED)
{
    mItemPopup->setVisible(false);
}

void ItemContainer::widgetResized(const gcn::Event &event A_UNUSED)
{
    mGridColumns = std::max(1, getWidth() / mBoxWidth);
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

    for (unsigned idx = 0; idx < mInventory->getSize(); idx ++)
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
            std::sort(sortedItems.begin(), sortedItems.end(), itemAlphaSorter);
            break;
        case 2:
            std::sort(sortedItems.begin(), sortedItems.end(), itemIdSorter);
            break;
        case 3:
            std::sort(sortedItems.begin(), sortedItems.end(),
                itemWeightSorter);
            break;
        case 4:
            std::sort(sortedItems.begin(), sortedItems.end(),
                itemAmountSorter);
            break;
        case 5:
            std::sort(sortedItems.begin(), sortedItems.end(), itemTypeSorter);
            break;
    }

    int jMult = j * mGridColumns;
    const int maxSize = mGridRows * mGridColumns;
    for (std::vector<ItemIdPair*>::const_iterator iter = sortedItems.begin(),
         iter_end = sortedItems.end(); iter != iter_end; ++ iter)
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

    for (int idx = j * mGridColumns + i;
         idx < maxSize; idx ++)
    {
        mShowMatrix[idx] = -1;
    }

    for (size_t idx = 0, sz = sortedItems.size(); idx < sz; idx ++)
        delete sortedItems[idx];
}

int ItemContainer::getSlotIndex(const int x, const int y) const
{
    if (!mShowMatrix)
        return Inventory::NO_SLOT_INDEX;

    if (x < getWidth() && y < getHeight())
    {
        const int idx = (y / mBoxHeight) * mGridColumns + (x / mBoxWidth);
        if (idx < mGridRows * mGridColumns && mShowMatrix[idx] >= 0)
            return mShowMatrix[idx];
    }

    return Inventory::NO_SLOT_INDEX;
}

void ItemContainer::keyAction()
{
    // If there is no highlight then return.
    if (mHighlightedIndex == -1)
        return;

    // If the highlight is on the selected item, then deselect it.
    if (mHighlightedIndex == mSelectedIndex)
    {
         selectNone();
    }
    // Check and swap items if necessary.
    else if (mSwapItems && mSelectedIndex != -1 && mHighlightedIndex != -1)
    {
        Net::getInventoryHandler()->moveItem(
            mSelectedIndex, mHighlightedIndex);
        setSelectedIndex(mHighlightedIndex);
    }
    // If the highlight is on an item then select it.
    else if (mHighlightedIndex != -1)
    {
        setSelectedIndex(mHighlightedIndex);
        mSelectionStatus = SEL_SELECTED;
    }
    // If the highlight is on a blank space then move it.
    else if (mSelectedIndex != -1)
    {
        Net::getInventoryHandler()->moveItem(
            mSelectedIndex, mHighlightedIndex);
        selectNone();
    }
}

void ItemContainer::moveHighlight(const Direction direction)
{
    if (mHighlightedIndex == -1)
    {
        if (mSelectedIndex != -1)
            mHighlightedIndex = mSelectedIndex;
        else
            mHighlightedIndex = 0;
        return;
    }

    switch (direction)
    {
        case Left:
            if (mHighlightedIndex % mGridColumns == 0)
                mHighlightedIndex += mGridColumns;
            mHighlightedIndex--;
            break;
        case Right:
            if ((mHighlightedIndex % mGridColumns) ==
                (mGridColumns - 1))
            {
                mHighlightedIndex -= mGridColumns;
            }
            mHighlightedIndex++;
            break;
        case Up:
            if (mHighlightedIndex / mGridColumns == 0)
                mHighlightedIndex += (mGridColumns * mGridRows);
            mHighlightedIndex -= mGridColumns;
            break;
        case Down:
            if ((mHighlightedIndex / mGridColumns) ==
                (mGridRows - 1))
            {
                mHighlightedIndex -= (mGridColumns * mGridRows);
            }
            mHighlightedIndex += mGridColumns;
            break;
        default:
            logger->log("warning moveHighlight unknown direction:"
                        + toString(static_cast<unsigned>(direction)));
            break;
    }
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
