/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "gui/windows/inventorywindow.h"

#include "configuration.h"

#include "being/playerinfo.h"

#include "const/sound.h"

#include "enums/gui/layouttype.h"

#include "input/inputmanager.h"

#include "gui/gui.h"

#include "gui/fonts/font.h"

#include "gui/models/sortlistmodelinv.h"

#include "gui/popups/itempopup.h"
#include "gui/popups/popupmenu.h"
#include "gui/popups/textpopup.h"

#include "gui/windows/confirmdialog.h"
#include "gui/windows/itemamountwindow.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/setupwindow.h"
#include "gui/windows/tradewindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/itemcontainer.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/tabstrip.h"
#include "gui/widgets/textfield.h"
#include "gui/widgets/windowcontainer.h"

#include "listeners/insertcardlistener.h"

#include "net/npchandler.h"

#include "resources/iteminfo.h"

#include "resources/db/unitsdb.h"

#include "resources/item/item.h"

#include "utils/delete2.h"
#include "utils/foreach.h"

#include "debug.h"

InventoryWindow *inventoryWindow = nullptr;
InventoryWindow *storageWindow = nullptr;
InventoryWindow *cartWindow = nullptr;
InventoryWindow::WindowList InventoryWindow::invInstances;
InsertCardListener insertCardListener;

InventoryWindow::InventoryWindow(Inventory *const inventory) :
    Window("Inventory", Modal_false, nullptr, "inventory.xml"),
    ActionListener(),
    KeyListener(),
    SelectionListener(),
    InventoryListener(),
    AttributeListener(),
    mInventory(inventory),
    mItems(new ItemContainer(this, mInventory, 100000,
        ShowEmptyRows_false, ForceQuantity_false)),
    mUseButton(nullptr),
    mDropButton(nullptr),
    mOutfitButton(nullptr),
    mShopButton(nullptr),
    mCartButton(nullptr),
    mEquipmentButton(nullptr),
    mStoreButton(nullptr),
    mRetrieveButton(nullptr),
    mInvCloseButton(nullptr),
    mWeightBar(nullptr),
    mSlotsBar(new ProgressBar(this, 0.0F, 100, 0,
        ProgressColorId::PROG_INVY_SLOTS,
        "slotsprogressbar.xml", "slotsprogressbar_fill.xml")),
    mFilter(nullptr),
    mSortModel(new SortListModelInv),
    mSortDropDown(new DropDown(this, mSortModel, false,
        Modal_false, this, "sort")),
    mNameFilter(new TextField(this, "", LoseFocusOnTab_true,
        this, "namefilter", true)),
    mSortDropDownCell(nullptr),
    mNameFilterCell(nullptr),
    mFilterCell(nullptr),
    mSlotsBarCell(nullptr),
    mSplit(false),
    mCompactMode(false)
{
    mSlotsBar->setColor(getThemeColor(ThemeColorId::SLOTS_BAR, 255U),
        getThemeColor(ThemeColorId::SLOTS_BAR_OUTLINE, 255U));

    if (inventory != nullptr)
    {
        setCaption(gettext(inventory->getName().c_str()));
        setWindowName(inventory->getName());
        switch (inventory->getType())
        {
            case InventoryType::Inventory:
            case InventoryType::Trade:
            case InventoryType::Npc:
            case InventoryType::Vending:
            case InventoryType::MailEdit:
            case InventoryType::MailView:
            case InventoryType::Craft:
            case InventoryType::TypeEnd:
            default:
                mSortDropDown->setSelected(config.getIntValue(
                    "inventorySortOrder"));
                break;
            case InventoryType::Storage:
                mSortDropDown->setSelected(config.getIntValue(
                    "storageSortOrder"));
                break;
            case InventoryType::Cart:
                mSortDropDown->setSelected(config.getIntValue(
                    "cartSortOrder"));
                break;
        }
    }
    else
    {
        // TRANSLATORS: inventory window name
        setCaption(_("Inventory"));
        setWindowName("Inventory");
        mSortDropDown->setSelected(0);
    }

    if ((setupWindow != nullptr) &&
        (inventory != nullptr) &&
        inventory->getType() != InventoryType::Storage)
    {
        setupWindow->registerWindowForReset(this);
    }

    setResizable(true);
    setCloseButton(true);
    setSaveVisible(true);
    setStickyButtonLock(true);

    if (mainGraphics->mWidth > 600)
        setDefaultSize(450, 310, ImagePosition::CENTER, 0, 0);
    else
        setDefaultSize(387, 307, ImagePosition::CENTER, 0, 0);
    setMinWidth(310);
    setMinHeight(179);
    addKeyListener(this);

    mItems->addSelectionListener(this);

    const int size = config.getIntValue("fontSize");
    mFilter = new TabStrip(this, "filter_" + getWindowName(), size + 16, 0);
    mFilter->addActionListener(this);
    mFilter->setActionEventId("tag_");
    mFilter->setSelectable(false);

    StringVect tags = ItemDB::getTags();
    const size_t sz = tags.size();
    for (size_t f = 0; f < sz; f ++)
        mFilter->addButton(tags[f], tags[f], false);

    if (mInventory == nullptr)
    {
        invInstances.push_back(this);
        return;
    }

    ScrollArea *const invenScroll = new ScrollArea(this, mItems,
        fromBool(getOptionBool("showbackground", false), Opaque),
        "inventory_background.xml");
    invenScroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);

    switch (mInventory->getType())
    {
        case InventoryType::Inventory:
        {
            // TRANSLATORS: inventory button
            const std::string equip = _("Equip");
            // TRANSLATORS: inventory button
            const std::string use = _("Use");
            // TRANSLATORS: inventory button
            const std::string unequip = _("Unequip");

            std::string longestUseString = getFont()->getWidth(equip) >
                getFont()->getWidth(use) ? equip : use;

            if (getFont()->getWidth(longestUseString) <
                getFont()->getWidth(unequip))
            {
                longestUseString = unequip;
            }

            mUseButton = new Button(this,
                longestUseString,
                "use",
                BUTTON_SKIN,
                this);
            mDropButton = new Button(this,
                // TRANSLATORS: inventory button
                _("Drop..."),
                "drop",
                BUTTON_SKIN,
                this);
            mOutfitButton = new Button(this,
                // TRANSLATORS: inventory outfits button
                _("O"),
                "outfit",
                BUTTON_SKIN,
                this);
            mCartButton = new Button(this,
                // TRANSLATORS: inventory cart button
                _("C"),
                "cart",
                BUTTON_SKIN,
                this);
            mShopButton = new Button(this,
                // TRANSLATORS: inventory shop button
                _("S"),
                "shop",
                BUTTON_SKIN,
                this);
            mEquipmentButton = new Button(this,
                // TRANSLATORS: inventory equipment button
                _("E"),
                "equipment",
                BUTTON_SKIN,
                this);
            mWeightBar = new ProgressBar(this, 0.0F, 100, 0,
                ProgressColorId::PROG_WEIGHT,
                "weightprogressbar.xml", "weightprogressbar_fill.xml");
            mWeightBar->setColor(getThemeColor(ThemeColorId::WEIGHT_BAR, 255U),
                getThemeColor(ThemeColorId::WEIGHT_BAR_OUTLINE, 255U));

            // TRANSLATORS: outfits button tooltip
            mOutfitButton->setDescription(_("Outfits"));
            // TRANSLATORS: cart button tooltip
            mCartButton->setDescription(_("Cart"));
            // TRANSLATORS: shop button tooltip
            mShopButton->setDescription(_("Shop"));
            // TRANSLATORS: equipment button tooltip
            mEquipmentButton->setDescription(_("Equipment"));

            place(0, 0, mWeightBar, 4, 1);
            mSlotsBarCell = &place(4, 0, mSlotsBar, 4, 1);
            mSortDropDownCell = &place(8, 0, mSortDropDown, 3, 1);

            mFilterCell = &place(0, 1, mFilter, 10, 1).setPadding(3);
            mNameFilterCell = &place(8, 1, mNameFilter, 3, 1);

            place(0, 2, invenScroll, 11, 1).setPadding(3);
            place(0, 3, mUseButton, 1, 1);
            place(1, 3, mDropButton, 1, 1);
            ContainerPlacer placer = getPlacer(10, 3);
            placer(0, 0, mShopButton, 1, 1);
            placer(1, 0, mOutfitButton, 1, 1);
            placer(2, 0, mCartButton, 1, 1);
            placer(3, 0, mEquipmentButton, 1, 1);

            updateWeight();
            break;
        }

        case InventoryType::Storage:
        {
            mStoreButton = new Button(this,
                // TRANSLATORS: storage button
                _("Store"),
                "store",
                BUTTON_SKIN,
                this);
            mRetrieveButton = new Button(this,
                // TRANSLATORS: storage button
                _("Retrieve"),
                "retrieve",
                BUTTON_SKIN,
                this);
            mInvCloseButton = new Button(this,
                // TRANSLATORS: storage button
                _("Close"),
                "close",
                BUTTON_SKIN,
                this);

            mSlotsBarCell = &place(0, 0, mSlotsBar, 6, 1);
            mSortDropDownCell = &place(6, 0, mSortDropDown, 1, 1);

            mFilterCell = &place(0, 1, mFilter, 7, 1).setPadding(3);
            mNameFilterCell = &place(6, 1, mNameFilter, 1, 1);

            place(0, 2, invenScroll, 7, 4);
            place(0, 6, mStoreButton, 1, 1);
            place(1, 6, mRetrieveButton, 1, 1);
            place(6, 6, mInvCloseButton, 1, 1);
            break;
        }

        case InventoryType::Cart:
        {
            mStoreButton = new Button(this,
                // TRANSLATORS: storage button
                _("Store"),
                "store",
                BUTTON_SKIN,
                this);
            mRetrieveButton = new Button(this,
                // TRANSLATORS: storage button
                _("Retrieve"),
                "retrieve",
                BUTTON_SKIN,
                this);
            mInvCloseButton = new Button(this,
                // TRANSLATORS: storage button
                _("Close"),
                "close",
                BUTTON_SKIN,
                this);

            mWeightBar = new ProgressBar(this, 0.0F, 100, 0,
                ProgressColorId::PROG_WEIGHT,
                "weightprogressbar.xml", "weightprogressbar_fill.xml");
            mWeightBar->setColor(getThemeColor(ThemeColorId::WEIGHT_BAR, 255U),
                getThemeColor(ThemeColorId::WEIGHT_BAR_OUTLINE, 255U));

            mSlotsBarCell = &place(3, 0, mSlotsBar, 3, 1);
            mSortDropDownCell = &place(6, 0, mSortDropDown, 1, 1);

            mFilterCell = &place(0, 1, mFilter, 7, 1).setPadding(3);
            mNameFilterCell = &place(6, 1, mNameFilter, 1, 1);

            place(0, 0, mWeightBar, 3, 1);
            place(0, 2, invenScroll, 7, 4);
            place(0, 6, mStoreButton, 1, 1);
            place(1, 6, mRetrieveButton, 1, 1);
            place(6, 6, mInvCloseButton, 1, 1);
            break;
        }

        default:
        case InventoryType::Trade:
        case InventoryType::Npc:
        case InventoryType::Vending:
        case InventoryType::MailEdit:
        case InventoryType::MailView:
        case InventoryType::Craft:
        case InventoryType::TypeEnd:
            break;
    }

    Layout &layout = getLayout();
    layout.setRowHeight(2, LayoutType::SET);

    mInventory->addInventoyListener(this);

    invInstances.push_back(this);

    if (inventory->isMainInventory())
    {
        updateDropButton();
    }
    else
    {
        if (!invInstances.empty())
            invInstances.front()->updateDropButton();
    }

    loadWindowState();
    enableVisibleSound(true);
}

void InventoryWindow::postInit()
{
    Window::postInit();
    slotsChanged(mInventory);

    mItems->setSortType(mSortDropDown->getSelected());
    widgetResized(Event(nullptr));
    if (mInventory != nullptr &&
        mInventory->getType() == InventoryType::Storage)
    {
        setVisible(Visible_true);
    }
}

InventoryWindow::~InventoryWindow()
{
    invInstances.remove(this);
    if (mInventory != nullptr)
        mInventory->removeInventoyListener(this);
    if (!invInstances.empty())
        invInstances.front()->updateDropButton();

    mSortDropDown->hideDrop(false);
    delete2(mSortModel)
}

void InventoryWindow::storeSortOrder() const
{
    if (mInventory != nullptr)
    {
        switch (mInventory->getType())
        {
            case InventoryType::Inventory:
            case InventoryType::Trade:
            case InventoryType::Npc:
            case InventoryType::Vending:
            case InventoryType::MailEdit:
            case InventoryType::MailView:
            case InventoryType::Craft:
            case InventoryType::TypeEnd:
            default:
                config.setValue("inventorySortOrder",
                    mSortDropDown->getSelected());
                break;
            case InventoryType::Storage:
                config.setValue("storageSortOrder",
                    mSortDropDown->getSelected());
                break;
            case InventoryType::Cart:
                config.setValue("cartSortOrder",
                    mSortDropDown->getSelected());
                break;
        }
    }
}

void InventoryWindow::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "outfit")
    {
        inputManager.executeAction(InputAction::WINDOW_OUTFIT);
    }
    else if (eventId == "shop")
    {
        inputManager.executeAction(InputAction::WINDOW_SHOP);
    }
    else if (eventId == "equipment")
    {
        inputManager.executeAction(InputAction::WINDOW_EQUIPMENT);
    }
    else if (eventId == "cart")
    {
        inputManager.executeAction(InputAction::WINDOW_CART);
    }
    else if (eventId == "close")
    {
        close();
    }
    else if (eventId == "store")
    {
        if (inventoryWindow == nullptr || !inventoryWindow->isWindowVisible())
            return;

        Item *const item = inventoryWindow->getSelectedItem();

        if (item == nullptr)
            return;

        if (storageWindow != nullptr)
        {
            ItemAmountWindow::showWindow(ItemAmountWindowUsage::StoreAdd,
                this,
                item,
                0,
                0);
        }
        else if ((cartWindow != nullptr) && cartWindow->isWindowVisible())
        {
            ItemAmountWindow::showWindow(ItemAmountWindowUsage::CartAdd,
                this,
                item,
                0,
                0);
        }
    }
    else if (eventId == "sort")
    {
        mItems->setSortType(mSortDropDown->getSelected());
        storeSortOrder();
        return;
    }
    else if (eventId == "namefilter")
    {
        mItems->setName(mNameFilter->getText());
        mItems->updateMatrix();
    }
    else if (eventId.find("tag_") == 0U)
    {
        std::string tagName = event.getId().substr(4);
        mItems->setFilter(ItemDB::getTagId(tagName));
        return;
    }

    Item *const item = mItems->getSelectedItem();

    if (item == nullptr)
        return;

    if (eventId == "use")
    {
        PlayerInfo::useEquipItem(item, 0, Sfx_true);
    }
    if (eventId == "equip")
    {
        PlayerInfo::useEquipItem2(item, 0, Sfx_true);
    }
    else if (eventId == "drop")
    {
        if (isStorageActive())
        {
            inventoryHandler->moveItem2(InventoryType::Inventory,
                item->getInvIndex(),
                item->getQuantity(),
                InventoryType::Storage);
        }
        else if ((cartWindow != nullptr) && cartWindow->isWindowVisible())
        {
            inventoryHandler->moveItem2(InventoryType::Inventory,
                item->getInvIndex(),
                item->getQuantity(),
                InventoryType::Cart);
        }
        else
        {
            if (PlayerInfo::isItemProtected(item->getId()))
                return;

            if (inputManager.isActionActive(InputAction::STOP_ATTACK))
            {
                PlayerInfo::dropItem(item, item->getQuantity(), Sfx_true);
            }
            else
            {
                ItemAmountWindow::showWindow(ItemAmountWindowUsage::ItemDrop,
                    this,
                    item,
                    0,
                    0);
            }
        }
    }
    else if (eventId == "split")
    {
        ItemAmountWindow::showWindow(ItemAmountWindowUsage::ItemSplit,
            this,
            item,
            item->getQuantity() - 1,
            9);
    }
    else if (eventId == "retrieve")
    {
        if (storageWindow != nullptr)
        {
            ItemAmountWindow::showWindow(ItemAmountWindowUsage::StoreRemove,
                this,
                item,
                0,
                0);
        }
        else if ((cartWindow != nullptr) && cartWindow->isWindowVisible())
        {
            ItemAmountWindow::showWindow(ItemAmountWindowUsage::CartRemove,
                this,
                item,
                0,
                0);
        }
    }
}

Item *InventoryWindow::getSelectedItem() const
{
    return mItems->getSelectedItem();
}

void InventoryWindow::unselectItem()
{
    mItems->selectNone();
}

void InventoryWindow::widgetHidden(const Event &event)
{
    Window::widgetHidden(event);
    if (itemPopup != nullptr)
        itemPopup->setVisible(Visible_false);
}

void InventoryWindow::mouseClicked(MouseEvent &event)
{
    Window::mouseClicked(event);

    const int clicks = event.getClickCount();

    if (clicks == 2 && (gui != nullptr))
        gui->resetClickCount();

    const bool mod = (isStorageActive() &&
        inputManager.isActionActive(InputAction::STOP_ATTACK));

    const bool mod2 = (tradeWindow != nullptr &&
        tradeWindow->isWindowVisible() &&
        inputManager.isActionActive(InputAction::STOP_ATTACK));

    if (mInventory != nullptr)
    {
        if (!mod && !mod2 && event.getButton() == MouseButton::RIGHT)
        {
            Item *const item = mItems->getSelectedItem();

            if (item == nullptr)
                return;

            /* Convert relative to the window coordinates to absolute screen
             * coordinates.
             */
            if (popupMenu != nullptr)
            {
                const int mx = event.getX() + getX();
                const int my = event.getY() + getY();

                popupMenu->showPopup(this,
                    mx, my,
                    item,
                    mInventory->getType());
            }
        }
    }
    else
    {
        return;
    }

    if (event.getButton() == MouseButton::LEFT ||
        event.getButton() == MouseButton::RIGHT)
    {
        Item *const item = mItems->getSelectedItem();

        if (item == nullptr)
            return;

        if (mod)
        {
            if (mInventory->isMainInventory())
            {
                if (event.getButton() == MouseButton::RIGHT)
                {
                    ItemAmountWindow::showWindow(
                        ItemAmountWindowUsage::StoreAdd,
                        inventoryWindow,
                        item,
                        0,
                        0);
                }
                else
                {
                    inventoryHandler->moveItem2(InventoryType::Inventory,
                        item->getInvIndex(),
                        item->getQuantity(),
                        InventoryType::Storage);
                }
            }
            else
            {
                if (event.getButton() == MouseButton::RIGHT)
                {
                    ItemAmountWindow::showWindow(
                        ItemAmountWindowUsage::StoreRemove,
                        inventoryWindow,
                        item,
                        0,
                        0);
                }
                else
                {
                    inventoryHandler->moveItem2(InventoryType::Storage,
                        item->getInvIndex(),
                        item->getQuantity(),
                        InventoryType::Inventory);
                }
            }
        }
        else if (mod2 && mInventory->isMainInventory())
        {
            if (PlayerInfo::isItemProtected(item->getId()))
                return;
            if (event.getButton() == MouseButton::RIGHT)
            {
                ItemAmountWindow::showWindow(ItemAmountWindowUsage::TradeAdd,
                    tradeWindow,
                    item,
                    0,
                    0);
            }
            else
            {
                if (tradeWindow != nullptr)
                    tradeWindow->tradeItem(item, item->getQuantity(), true);
            }
        }
        else if (clicks == 2)
        {
            if (mInventory->isMainInventory())
            {
                if (isStorageActive())
                {
                    ItemAmountWindow::showWindow(
                        ItemAmountWindowUsage::StoreAdd,
                        inventoryWindow,
                        item,
                        0,
                        0);
                }
                else if (tradeWindow != nullptr &&
                         tradeWindow->isWindowVisible())
                {
                    if (PlayerInfo::isItemProtected(item->getId()))
                        return;
                    ItemAmountWindow::showWindow(
                        ItemAmountWindowUsage::TradeAdd,
                        tradeWindow,
                        item,
                        0,
                        0);
                }
                else
                {
                    PlayerInfo::useEquipItem(item, 0, Sfx_true);
                }
            }
            else
            {
                if (isStorageActive())
                {
                    ItemAmountWindow::showWindow(
                        ItemAmountWindowUsage::StoreRemove,
                        inventoryWindow,
                        item,
                        0,
                        0);
                }
            }
        }
    }
}

void InventoryWindow::mouseMoved(MouseEvent &event)
{
    Window::mouseMoved(event);
    if (textPopup == nullptr)
        return;

    const Widget *const src = event.getSource();
    if (src == nullptr)
    {
        textPopup->hide();
        return;
    }
    const int x = event.getX();
    const int y = event.getY();
    const Rect &rect = mDimension;
    if (src == mSlotsBar || src == mWeightBar)
    {
        // TRANSLATORS: money label
        textPopup->show(rect.x + x, rect.y + y, strprintf(_("Money: %s"),
            UnitsDb::formatCurrency(PlayerInfo::getAttribute(
            Attributes::MONEY)).c_str()));
    }
    else
    {
        const Button *const btn = dynamic_cast<const Button *>(src);
        if (btn == nullptr)
        {
            textPopup->hide();
            return;
        }
        const std::string text = btn->getDescription();
        if (!text.empty())
            textPopup->show(x + rect.x, y + rect.y, text);
    }
}

void InventoryWindow::mouseExited(MouseEvent &event A_UNUSED)
{
    textPopup->hide();
}

void InventoryWindow::keyPressed(KeyEvent &event)
{
    if (event.getActionId() == InputAction::GUI_MOD)
        mSplit = true;
}

void InventoryWindow::keyReleased(KeyEvent &event)
{
    if (event.getActionId() == InputAction::GUI_MOD)
        mSplit = false;
}

void InventoryWindow::valueChanged(const SelectionEvent &event A_UNUSED)
{
    if ((mInventory == nullptr) || !mInventory->isMainInventory())
        return;

    Item *const item = mItems->getSelectedItem();

    if (mSplit && (item != nullptr) && inventoryHandler->
        canSplit(mItems->getSelectedItem()))
    {
        ItemAmountWindow::showWindow(ItemAmountWindowUsage::ItemSplit,
            this,
            item,
            item->getQuantity() - 1,
            0);
    }
    updateButtons(item);
}

void InventoryWindow::updateButtons(const Item *item)
{
    if ((mInventory == nullptr) || !mInventory->isMainInventory())
        return;

    const Item *const selectedItem = mItems->getSelectedItem();
    if ((item != nullptr) && selectedItem != item)
        return;

    if (item == nullptr)
        item = selectedItem;

    if ((item == nullptr) || item->getQuantity() == 0)
    {
        if (mUseButton != nullptr)
            mUseButton->setEnabled(false);
        if (mDropButton != nullptr)
            mDropButton->setEnabled(false);
        return;
    }

    if (mDropButton != nullptr)
        mDropButton->setEnabled(true);

    if (mUseButton != nullptr)
    {
        const ItemInfo &info = item->getInfo();
        const std::string &str = (item->isEquipment() == Equipm_true
            && item->isEquipped() == Equipped_true)
            ? info.getUseButton2() : info.getUseButton();
        if (str.empty())
        {
            mUseButton->setEnabled(false);
            // TRANSLATORS: default use button name
            mUseButton->setCaption(_("Use"));
        }
        else
        {
            mUseButton->setEnabled(true);
            mUseButton->setCaption(str);
        }
    }

    updateDropButton();
}

void InventoryWindow::close()
{
    if (mInventory == nullptr)
    {
        Window::close();
        return;
    }

    switch (mInventory->getType())
    {
        case InventoryType::Inventory:
        case InventoryType::Cart:
            setVisible(Visible_false);
            break;

        case InventoryType::Storage:
            if (inventoryHandler != nullptr)
            {
                inventoryHandler->closeStorage();
                inventoryHandler->forgotStorage();
            }
            scheduleDelete();
            break;

        default:
        case InventoryType::Trade:
        case InventoryType::Npc:
        case InventoryType::Vending:
        case InventoryType::MailEdit:
        case InventoryType::MailView:
        case InventoryType::Craft:
        case InventoryType::TypeEnd:
            break;
    }
}

void InventoryWindow::updateWeight()
{
    if ((mInventory == nullptr) || (mWeightBar == nullptr))
        return;
    const InventoryTypeT type = mInventory->getType();
    if (type != InventoryType::Inventory &&
        type != InventoryType::Cart)
    {
        return;
    }

    const bool isInv = type == InventoryType::Inventory;
    const int total = PlayerInfo::getAttribute(isInv
        ? Attributes::TOTAL_WEIGHT : Attributes::CART_TOTAL_WEIGHT);
    const int max = PlayerInfo::getAttribute(isInv
        ? Attributes::MAX_WEIGHT : Attributes::CART_MAX_WEIGHT);

    if (max <= 0)
        return;

    // Adjust progress bar
    mWeightBar->setProgress(static_cast<float>(total)
        / static_cast<float>(max));
    mWeightBar->setText(strprintf("%s/%s",
        UnitsDb::formatWeight(total).c_str(),
        UnitsDb::formatWeight(max).c_str()));
}

void InventoryWindow::slotsChanged(const Inventory *const inventory)
{
    if (inventory == mInventory)
    {
        const int usedSlots = mInventory->getNumberOfSlotsUsed();
        const int maxSlots = mInventory->getSize();

        if (maxSlots != 0)
        {
            mSlotsBar->setProgress(static_cast<float>(usedSlots)
                / static_cast<float>(maxSlots));
        }

        mSlotsBar->setText(strprintf("%d/%d", usedSlots, maxSlots));
        mItems->updateMatrix();
    }
}

void InventoryWindow::updateDropButton()
{
    if (mDropButton == nullptr)
        return;

    if (isStorageActive() ||
        (cartWindow != nullptr && cartWindow->isWindowVisible()))
    {
        // TRANSLATORS: inventory button
        mDropButton->setCaption(_("Store"));
    }
    else
    {
        const Item *const item = mItems->getSelectedItem();
        if ((item != nullptr) && item->getQuantity() > 1)
        {
            // TRANSLATORS: inventory button
            mDropButton->setCaption(_("Drop..."));
        }
        else
        {
            // TRANSLATORS: inventory button
            mDropButton->setCaption(_("Drop"));
        }
    }
}

bool InventoryWindow::isInputFocused() const
{
    return (mNameFilter != nullptr) && mNameFilter->isFocused();
}

bool InventoryWindow::isAnyInputFocused()
{
    FOR_EACH (WindowList::const_iterator, it, invInstances)
    {
        if (((*it) != nullptr) && (*it)->isInputFocused())
            return true;
    }
    return false;
}

InventoryWindow *InventoryWindow::getFirstVisible()
{
    std::set<Widget*> list;
    FOR_EACH (WindowList::const_iterator, it, invInstances)
    {
        if (((*it) != nullptr) && (*it)->isWindowVisible())
            list.insert(*it);
    }
    return dynamic_cast<InventoryWindow*>(
        windowContainer->findFirstWidget(list));
}

void InventoryWindow::nextTab()
{
    const InventoryWindow *const window = getFirstVisible();
    if (window != nullptr)
        window->mFilter->nextTab();
}

void InventoryWindow::prevTab()
{
    const InventoryWindow *const window = getFirstVisible();
    if (window != nullptr)
        window->mFilter->prevTab();
}

void InventoryWindow::widgetResized(const Event &event)
{
    Window::widgetResized(event);

    if (mInventory == nullptr)
        return;
    const InventoryTypeT type = mInventory->getType();
    if (type != InventoryType::Inventory &&
        type != InventoryType::Cart)
    {
        return;
    }

    if (getWidth() < 600)
    {
        if (!mCompactMode)
        {
            mNameFilter->setVisible(Visible_false);
            mNameFilterCell->setType(LayoutCell::NONE);
            mFilterCell->setWidth(mFilterCell->getWidth() + 3);
            mCompactMode = true;
        }
    }
    else if (mCompactMode)
    {
        mNameFilter->setVisible(Visible_true);
        mNameFilterCell->setType(LayoutCell::WIDGET);
        mFilterCell->setWidth(mFilterCell->getWidth() - 3);
        mCompactMode = false;
    }
}

void InventoryWindow::setVisible(Visible visible)
{
    if (visible == Visible_false)
        mSortDropDown->hideDrop(true);
    Window::setVisible(visible);
}

void InventoryWindow::unsetInventory()
{
    if (mInventory != nullptr)
    {
        mInventory->removeInventoyListener(this);
        if (mItems != nullptr)
            mItems->unsetInventory();
    }
    mInventory = nullptr;
}

void InventoryWindow::attributeChanged(const AttributesT id,
                                       const int64_t oldVal A_UNUSED,
                                       const int64_t newVal A_UNUSED)
{
    if (id == Attributes::TOTAL_WEIGHT
        || id == Attributes::MAX_WEIGHT
        || id == Attributes::CART_TOTAL_WEIGHT
        || id == Attributes::CART_MAX_WEIGHT)
    {
        updateWeight();
    }
}

void InventoryWindow::combineItems(const int index1,
                                   const int index2)
{
    if (mInventory == nullptr)
        return;
    const Item *item1 = mInventory->getItem(index1);
    if (item1 == nullptr)
        return;
    const Item *item2 = mInventory->getItem(index2);
    if (item2 == nullptr)
        return;

    if (item1->getType() != ItemType::Card)
    {
        const Item *tmpItem = item1;
        item1 = item2;
        item2 = tmpItem;
    }

    ConfirmDialog *const confirmDlg = CREATEWIDGETR(ConfirmDialog,
        // TRANSLATORS: question dialog title
        _("Insert card request"),
        // TRANSLATORS: question dialog message
        strprintf(_("Insert %s into %s?"),
        item1->getName().c_str(),
        item2->getName().c_str()),
        SOUND_REQUEST,
        false,
        Modal_true,
        nullptr);
    insertCardListener.itemIndex = item2->getInvIndex();
    insertCardListener.cardIndex = item1->getInvIndex();
    confirmDlg->addActionListener(&insertCardListener);
}

void InventoryWindow::moveItemToCraft(const int craftSlot)
{
    if (npcHandler == nullptr)
        return;

    Item *const item = mItems->getSelectedItem();
    if (item == nullptr)
        return;

    NpcDialog *const dialog = npcHandler->getCurrentNpcDialog();
    if ((dialog != nullptr) &&
        dialog->getInputState() == NpcInputState::ITEM_CRAFT)
    {
        if (item->getQuantity() > 1
            && !inputManager.isActionActive(InputAction::STOP_ATTACK))
        {
            ItemAmountWindow::showWindow(ItemAmountWindowUsage::CraftAdd,
                npcHandler->getCurrentNpcDialog(),
                item,
                0,
                craftSlot);
        }
        else
        {
            dialog->addCraftItem(item, 1, craftSlot);
        }
    }
}
