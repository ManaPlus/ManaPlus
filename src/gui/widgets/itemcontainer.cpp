/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/widgets/itemcontainer.h"

#include "dragdrop.h"
#include "settings.h"

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
#include "gui/windows/maileditwindow.h"
#include "gui/windows/npcdialog.h"

#include "input/inputmanager.h"

#include "net/inventoryhandler.h"
#include "net/net.h"
#include "net/mail2handler.h"
#include "net/npchandler.h"
#include "net/tradehandler.h"

#include "utils/delete2.h"
#include "utils/foreach.h"
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
                if ((item1 == nullptr) || (item2 == nullptr))
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
                return name1 < name2;
            }
    } itemAlphaInvSorter;

    class SortItemIdFunctor final
    {
        public:
            A_DEFAULT_COPY(SortItemIdFunctor)

            bool operator() (const ItemIdPair *const pair1,
                             const ItemIdPair *const pair2) const
            {
                if ((pair1->mItem == nullptr) || (pair2->mItem == nullptr))
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
                if ((pair1->mItem == nullptr) || (pair2->mItem == nullptr))
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
                    return name1 < name2;
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
                if ((pair1->mItem == nullptr) || (pair2->mItem == nullptr))
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
                    return name1 < name2;
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
                if ((pair1->mItem == nullptr) || (pair2->mItem == nullptr))
                    return false;

                const ItemInfo &info1 = pair1->mItem->getInfo();
                const ItemInfo &info2 = pair2->mItem->getInfo();
                const ItemDbTypeT t1 = info1.getType();
                const ItemDbTypeT t2 = info2.getType();
                if (t1 == t2)
                {
                    const std::string &name1 = info1.getName();
                    const std::string &name2 = info2.getName();
                    if (name1 == name2)
                    {
                        return pair1->mItem->getInvIndex() <
                            pair2->mItem->getInvIndex();
                    }
                    return name1 < name2;
                }
                return t1 < t2;
            }
    } itemTypeInvSorter;

    class SortItemLevelFunctor final
    {
        public:
            A_DEFAULT_COPY(SortItemLevelFunctor)

        bool operator() (const ItemIdPair *const pair1,
                                     const ItemIdPair *const pair2) const
            {
                if ((pair1->mItem == nullptr) || (pair2->mItem == nullptr))
                    return false;
                const int w1 = pair1->mItem->getInfo().getReqLevel();
                const int w2 = pair2->mItem->getInfo().getReqLevel();
                const std::string name1 = pair1->mItem->getInfo().getName();
                const std::string name2 = pair2->mItem->getInfo().getName();
                if (w1 == w2)
                {
                    if (name1 == name2)
                    {
                        return pair1->mItem->getInvIndex() <
                            pair2->mItem->getInvIndex();
                    }
                    return name1 < name2;
                }
                return w1 < w2;
            }
    } itemLevelInvSorter;



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
    mSkin(theme != nullptr ? theme->load("itemcontainer.xml", "",
        true, Theme::getThemePath()) : nullptr),
    mVertexes(new ImageCollection),
    mEquipedColor(getThemeColor(ThemeColorId::ITEM_EQUIPPED, 255U)),
    mEquipedColor2(getThemeColor(ThemeColorId::ITEM_EQUIPPED_OUTLINE, 255U)),
    mUnEquipedColor(getThemeColor(ThemeColorId::ITEM_NOT_EQUIPPED, 255U)),
    mUnEquipedColor2(getThemeColor(ThemeColorId::ITEM_NOT_EQUIPPED_OUTLINE,
        255U)),
    mSelectionListeners(),
    mGridColumns(1),
    mGridRows(1),
    mDrawRows(1),
    mSelectedIndex(-1),
    mLastUsedSlot(-1),
    mTag(0),
    mSortType(0),
    mClicks(1),
    mBoxWidth(mSkin != nullptr ? mSkin->getOption("boxWidth", 35) : 35),
    mBoxHeight(mSkin != nullptr ? mSkin->getOption("boxHeight", 43) : 43),
    mEquippedTextPadding(mSkin != nullptr ? mSkin->getOption(
                         "equippedTextPadding", 29) : 29),
    mPaddingItemX(mSkin != nullptr ? mSkin->getOption("paddingItemX", 0) : 0),
    mPaddingItemY(mSkin != nullptr ? mSkin->getOption("paddingItemY", 0) : 0),
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
    if (gui != nullptr)
        gui->removeDragged(this);

    if (mSelImg != nullptr)
    {
        mSelImg->decRef();
        mSelImg = nullptr;
    }
    if (mProtectedImg != nullptr)
    {
        mProtectedImg->decRef();
        mProtectedImg = nullptr;
    }
    if (mCellBackgroundImg != nullptr)
    {
        mCellBackgroundImg->decRef();
        mCellBackgroundImg = nullptr;
    }

    if (theme != nullptr)
        theme->unload(mSkin);

    delete []mShowMatrix;
    delete2(mVertexes)
}

void ItemContainer::logic()
{
    BLOCK_START("ItemContainer::logic")
    Widget::logic();

    if (mInventory == nullptr)
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
    if ((mInventory == nullptr) || (mShowMatrix == nullptr))
        return;

    BLOCK_START("ItemContainer::draw")
    Font *const font = getFont();

    if (mCellBackgroundImg != nullptr)
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

    for (int j = 0; j < mDrawRows; j++)
    {
        const int intY0 = j * mBoxHeight;
        int itemIndex = j * mGridColumns - 1;
        for (int i = 0; i < mGridColumns; i++)
        {
            itemIndex ++;
            if (mShowMatrix[itemIndex] < 0)
                continue;

            const Item *const item = mInventory->getItem(
                mShowMatrix[itemIndex]);

            if ((item == nullptr) || item->getId() == 0)
                continue;

            Image *const image = item->getImage();
            if (image != nullptr)
            {
                int itemX = i * mBoxWidth;
                int itemY = intY0;
                if (mShowMatrix[itemIndex] == mSelectedIndex)
                {
                    if (mSelImg != nullptr)
                        graphics->drawImage(mSelImg, itemX, itemY);
                }
                image->setAlpha(1.0F);  // ensure the image if fully drawn...
                graphics->drawImage(image,
                    itemX + mPaddingItemX,
                    itemY + mPaddingItemY);
                if ((mProtectedImg != nullptr) && PlayerInfo::isItemProtected(
                    item->getId()))
                {
                    graphics->drawImage(mProtectedImg,
                        itemX + mPaddingItemX,
                        itemY + mPaddingItemY);
                }
            }
        }
    }

    for (int j = 0; j < mDrawRows; j++)
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

            if ((item == nullptr) || item->getId() == 0)
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
    if ((mInventory == nullptr) || (mShowMatrix == nullptr))
        return;

    BLOCK_START("ItemContainer::draw")
    Font *const font = getFont();

    if (mCellBackgroundImg != nullptr)
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

    for (int j = 0; j < mDrawRows; j++)
    {
        const int intY0 = j * mBoxHeight;
        int itemIndex = j * mGridColumns - 1;
        for (int i = 0; i < mGridColumns; i++)
        {
            itemIndex ++;
            if (mShowMatrix[itemIndex] < 0)
                continue;

            const Item *const item = mInventory->getItem(
                mShowMatrix[itemIndex]);

            if ((item == nullptr) || item->getId() == 0)
                continue;

            Image *const image = item->getImage();
            if (image != nullptr)
            {
                int itemX = i * mBoxWidth;
                int itemY = intY0;
                if (mShowMatrix[itemIndex] == mSelectedIndex)
                {
                    if (mSelImg != nullptr)
                        graphics->drawImage(mSelImg, itemX, itemY);
                }
                image->setAlpha(1.0F);  // ensure the image if fully drawn...
                graphics->drawImage(image,
                    itemX + mPaddingItemX,
                    itemY + mPaddingItemY);
                if ((mProtectedImg != nullptr) && PlayerInfo::isItemProtected(
                    item->getId()))
                {
                    graphics->drawImage(mProtectedImg,
                        itemX + mPaddingItemX,
                        itemY + mPaddingItemY);
                }
            }
        }
    }

    for (int j = 0; j < mDrawRows; j++)
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

            if ((item == nullptr) || item->getId() == 0)
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
    if (mInventory != nullptr)
        return mInventory->getItem(mSelectedIndex);
    return nullptr;
}

void ItemContainer::distributeValueChangedEvent()
{
    FOR_EACH (SelectionListenerIterator, i, mSelectionListeners)
    {
        if (*i != nullptr)
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
    if (mInventory == nullptr)
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
        if ((item != nullptr) && mDescItems && (chatWindow != nullptr))
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
            case InventoryType::MailEdit:
                src = DragDropSource::MailEdit;
                break;
            case InventoryType::MailView:
                src = DragDropSource::MailView;
                break;
            case InventoryType::Craft:
                src = DragDropSource::Craft;
                break;
            default:
            case InventoryType::Vending:
            case InventoryType::TypeEnd:
                break;
        }
        if (src == DragDropSource::MailView)
            return;
        if (mSelectedIndex == index && mClicks != 2)
        {
            dragDrop.dragItem(item, src, index);
            dragDrop.select(item);
            mSelectionStatus = SEL_DESELECTING;
        }
        else if ((item != nullptr) && (item->getId() != 0))
        {
            if (index >= 0)
            {
                dragDrop.dragItem(item, src, index);
                dragDrop.select(item);
                setSelectedIndex(index);
                mSelectionStatus = SEL_SELECTING;

                if (itemShortcutWindow != nullptr)
                {
                    const int num = itemShortcutWindow->getTabIndex();
                    if (num >= 0 && num < CAST_S32(SHORTCUT_TABS))
                    {
                        if (itemShortcut[num] != nullptr)
                            itemShortcut[num]->setItemSelected(item);
                    }
                }
                if (shopWindow != nullptr)
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
    if (mClicks == 2 ||
        inventoryHandler == nullptr ||
        tradeHandler == nullptr)
    {
        return;
    }

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
    }

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
    else if (mInventory != nullptr)
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
            case InventoryType::MailEdit:
                dst = DragDropSource::MailEdit;
                break;
            case InventoryType::MailView:
                dst = DragDropSource::MailView;
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
            if (Net::getNetworkType() == ServerType::TMWATHENA)
                return;
            const int index = getSlotIndex(event.getX(), event.getY());
            if (index == Inventory::NO_SLOT_INDEX)
                return;
            if (index == mSelectedIndex || mSelectedIndex == -1)
                return;
            if (inventoryWindow != nullptr)
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
        else if (src == DragDropSource::Inventory &&
                 dst == DragDropSource::Npc)
        {
            inventory = PlayerInfo::getInventory();
            if (inventory != nullptr)
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
        else if (src == DragDropSource::Inventory &&
                 dst == DragDropSource::MailEdit)
        {
            inventory = PlayerInfo::getInventory();
            if (inventory == nullptr)
                return;
            Item *const item = inventory->getItem(dragDrop.getTag());
            if (item == nullptr)
                return;
            if (settings.enableNewMailSystem)
            {
                if (item->getQuantity() > 1
                    && !inputManager.isActionActive(InputAction::STOP_ATTACK))
                {
                    ItemAmountWindow::showWindow(
                        ItemAmountWindowUsage::MailAdd,
                        mailEditWindow,
                        item,
                        0,
                        0);
                }
                else
                {
                    mail2Handler->addItem(item, 1);
                }
            }
            else
            {
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
                    if (inventory != nullptr)
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
                if (item2 != nullptr)
                    item2->setTag(item->getTag());
                mInventory->removeItemAt(dragDrop.getTag());
            }
            else
            {
                if (inventory != nullptr)
                {
                    const Item *const item = inventory->getItem(
                        dragDrop.getTag());
                    if (item != nullptr)
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
            if (inventory != nullptr)
            {
                Item *const item = inventory->getItem(dragDrop.getTag());
                if ((item == nullptr) || item->isEquipped() == Equipped_true)
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
                    if (inventory != nullptr)
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
                if (inventory != nullptr)
                {
                    const Item *const item = inventory->getItem(
                        dragDrop.getTag());
                    if (item != nullptr)
                    {
                        inventory->virtualRestore(item,
                            item->getQuantity());
                        mInventory->removeItemAt(dragDrop.getTag());
                    }
                }
                return;
            }
        }
        else if (src == DragDropSource::MailEdit)
        {
            if (event.getType() == MouseEventType::RELEASED2)
                return;
            if (settings.enableNewMailSystem)
            {
                if (mailEditWindow == nullptr)
                    return;
                inventory = mailEditWindow->getInventory();
                if (inventory == nullptr)
                    return;
                const Item *const item = inventory->getItem(dragDrop.getTag());
                if (item == nullptr)
                    return;
                mail2Handler->removeItem(item->getTag(),
                    item->getQuantity());
            }
            else
            {
                inventory = PlayerInfo::getInventory();
                if (inventory == nullptr)
                    return;
                const Item *const item = inventory->getItem(dragDrop.getTag());
                if (item == nullptr)
                    return;
                mInventory->removeItemAt(dragDrop.getTag());
            }
            return;
        }

        if (inventory != nullptr)
        {
            const Item *const item = inventory->getItem(dragDrop.getTag());
            if (item != nullptr)
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
    if (mInventory == nullptr)
        return;

    const Item *const item = mInventory->getItem(
        getSlotIndex(event.getX(), event.getY()));

    if ((item != nullptr) && (viewport != nullptr))
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
    if (mGridColumns == 0)
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

    if (mShowEmptyRows == ShowEmptyRows_false)
    {
        const int num = updateMatrix();
        mDrawRows = num / mGridColumns;
        if (mDrawRows == 0 || num % mGridColumns > 0)
            ++mDrawRows;

        maxRows = mDrawRows;
    }
    else
    {
        mDrawRows = mGridRows;
    }
    setHeight(maxRows * mBoxHeight);
}

int ItemContainer::updateMatrix()
{
    if (mInventory == nullptr)
        return 0;

    mRedraw = true;
    delete []mShowMatrix;
    mShowMatrix = new int[CAST_SIZE(mGridRows * mGridColumns)];

    STD_VECTOR<ItemIdPair*> sortedItems;
    int i = 0;
    int j = 0;

    std::string temp = mName;
    toLower(temp);

    const unsigned int invSize = mInventory->getSize();
    for (unsigned int idx = 0; idx < invSize; idx ++)
    {
        Item *const item = mInventory->getItem(idx);

        if (item == nullptr ||
            item->getId() == 0 ||
            !item->isHaveTag(mTag) ||
            item->getQuantity() == 0)
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
            std::sort(sortedItems.begin(), sortedItems.end(),
                itemIdInvSorter);
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
        case 6:
            std::sort(sortedItems.begin(), sortedItems.end(),
                itemLevelInvSorter);
            break;
    }

    int jMult = j * mGridColumns;
    const int maxSize = mGridRows * mGridColumns;
    FOR_EACH (STD_VECTOR<ItemIdPair*>::const_iterator, iter, sortedItems)
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

    const int num = CAST_S32(sortedItems.size());
    for (size_t idx = 0, sz = num; idx < sz; idx ++)
        delete sortedItems[idx];
    return num;
}

int ItemContainer::getSlotIndex(int x, int y) const
{
    if (mShowMatrix == nullptr)
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
    if (mShowMatrix == nullptr)
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
    adjustHeight();
}

void ItemContainer::setSortType(const int sortType)
{
    mSortType = sortType;
    updateMatrix();
}

void ItemContainer::setCellBackgroundImage(const std::string &xmlName)
{
    if (mCellBackgroundImg != nullptr)
        mCellBackgroundImg->decRef();
    mCellBackgroundImg = Theme::getImageFromThemeXml(xmlName, "");
    mRedraw = true;
}

void ItemContainer::setMaxColumns(const int maxColumns)
{
    mMaxColumns = maxColumns;
    updateSize();
}
