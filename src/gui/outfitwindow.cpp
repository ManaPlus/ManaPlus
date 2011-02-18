/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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
#include "log.h"
#include "playerinfo.h"

#include "gui/chat.h"
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

float OutfitWindow::mAlpha = 1.0;

OutfitWindow::OutfitWindow():
    Window(_("Outfits")),
    mBoxWidth(33),
    mBoxHeight(33),
    mGridWidth(4),
    mGridHeight(3),
    mItemClicked(false),
    mItemMoved(NULL),
    mItemSelected(-1),
    mCurrentOutfit(0),
    mAwayOutfit(0)
{
    setWindowName("Outfits");
    setResizable(true);
    setCloseButton(true);
    setDefaultSize(250, 400, 150, 230);
    setMinWidth(145);
    setMinHeight(220);

    addMouseListener(this);

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
    for (int o = 0; o < OUTFITS_COUNT; o++)
    {
        for (int i = 0; i < OUTFIT_ITEM_COUNT; i++)
        {
            outfitStr += mItems[o][i] ? toString(mItems[o][i]) : toString(-1);
            if (i < OUTFIT_ITEM_COUNT - 1)
                outfitStr += " ";
        }
        serverConfig.setValue("Outfit" + toString(o), outfitStr);
        serverConfig.setValue("OutfitUnequip" + toString(o), mItemsUnequip[o]);
        outfitStr = "";
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
        item = PlayerInfo::getInventory()->findItem(mItems[outfit][i]);
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

        graphics->setColor(gcn::Color(0, 0, 0, 64));
        graphics->drawRectangle(gcn::Rectangle(itemX, itemY, 32, 32));
        graphics->setColor(gcn::Color(255, 255, 255, 32));
        graphics->fillRectangle(gcn::Rectangle(itemX, itemY, 32, 32));

        if (mItems[mCurrentOutfit][i] < 0)
            continue;

        bool foundItem = false;
        Inventory *inv = PlayerInfo::getInventory();
        if (inv)
        {
            Item *item = inv->findItem(mItems[mCurrentOutfit][i]);
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
            Image *image = Item::getImage(mItems[mCurrentOutfit][i]);
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
            const int tPosX = mCursorPosX - (image->getWidth() / 2);
            const int tPosY = mCursorPosY - (image->getHeight() / 2);

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
                Item *item = inv->findItem(itemId);
                if (item)
                    mItemMoved = item;
                else
                    mItemMoved = 0;
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
            viewport->showOutfitsPopup();
        Window::mousePressed(event);
        return;
    }
    mMoved = false;
    event.consume();

    // Stores the selected item if there is one.
    if (isItemSelected())
    {
        mItems[mCurrentOutfit][index] = mItemSelected;
        mItemSelected = -1;
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
        if (isItemSelected())
            mItemSelected = -1;

        const int index = getIndexFromGrid(event.getX(), event.getY());
        if (index == -1)
        {
            mItemMoved = NULL;
            Window::mouseReleased(event);
            return;
        }
        mMoved = false;
        event.consume();
        if (mItemMoved)
        {
            mItems[mCurrentOutfit][index] = mItemMoved->getId();
            mItemMoved = NULL;
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

int OutfitWindow::keyToNumber(SDLKey key)
{
    int outfitNum = -1;
    switch (key)
    {
        case SDLK_1:
        case SDLK_2:
        case SDLK_3:
        case SDLK_4:
        case SDLK_5:
        case SDLK_6:
        case SDLK_7:
        case SDLK_8:
        case SDLK_9:
            outfitNum = key - SDLK_1;
            break;

        case SDLK_0:
            outfitNum = 9;
            break;

        case SDLK_MINUS:
            outfitNum = 10;
            break;

        case SDLK_EQUALS:
            outfitNum = 11;
            break;

        case SDLK_BACKSPACE:
            outfitNum = 12;
            break;

        case SDLK_INSERT:
            outfitNum = 13;
            break;

        case SDLK_HOME:
            outfitNum = 14;
            break;

        case SDLK_q:
            outfitNum = 15;
            break;

        case SDLK_w:
            outfitNum = 16;
            break;

        case SDLK_e:
            outfitNum = 17;
            break;

        case SDLK_r:
            outfitNum = 18;
            break;

        case SDLK_t:
            outfitNum = 19;
            break;

        case SDLK_y:
            outfitNum = 20;
            break;

        case SDLK_u:
            outfitNum = 21;
            break;

        case SDLK_i:
            outfitNum = 22;
            break;

        case SDLK_o:
            outfitNum = 23;
            break;

        case SDLK_p:
            outfitNum = 24;
            break;

        case SDLK_LEFTBRACKET:
            outfitNum = 25;
            break;

        case SDLK_RIGHTBRACKET:
            outfitNum = 26;
            break;

        case SDLK_BACKSLASH:
            outfitNum = 27;
            break;

        case SDLK_a:
            outfitNum = 28;
            break;

        case SDLK_s:
            outfitNum = 29;
            break;

        case SDLK_d:
            outfitNum = 30;
            break;

        case SDLK_f:
            outfitNum = 31;
            break;

        case SDLK_g:
            outfitNum = 32;
            break;

        case SDLK_h:
            outfitNum = 33;
            break;

        case SDLK_j:
            outfitNum = 34;
            break;

        case SDLK_k:
            outfitNum = 35;
            break;

        case SDLK_l:
            outfitNum = 36;
            break;

        case SDLK_SEMICOLON:
            outfitNum = 37;
            break;

        case SDLK_QUOTE:
            outfitNum = 38;
            break;

        case SDLK_z:
            outfitNum = 39;
            break;


        case SDLK_x:
            outfitNum = 40;
            break;

        case SDLK_c:
            outfitNum = 41;
            break;

        case SDLK_v:
            outfitNum = 42;
            break;

        case SDLK_b:
            outfitNum = 43;
            break;

        case SDLK_n:
            outfitNum = 44;
            break;

        case SDLK_m:
            outfitNum = 45;
            break;

        case SDLK_COMMA:
            outfitNum = 46;
            break;

        case SDLK_PERIOD:
            outfitNum = 47;
            break;

        case SDLK_SLASH:
            outfitNum = 48;
            break;

        default:
            break;
    }

    return outfitNum;
}

SDLKey OutfitWindow::numberToKey(int number)
{
    SDLKey key = SDLK_UNKNOWN;
    switch (number)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            key = static_cast<SDLKey>(
                static_cast<unsigned int>(SDLK_1) + number);
            break;

        case 9:
            key = SDLK_0;
            break;

        case 10:
            key = SDLK_MINUS;
            break;

        case 11:
            key = SDLK_EQUALS;
            break;

        case 12:
            key = SDLK_BACKSPACE;
            break;

        case 13:
            key = SDLK_INSERT;
            break;

        case 14:
            key = SDLK_HOME;
            break;

        case 15:
            key = SDLK_q;
            break;

        case 16:
            key = SDLK_w;
            break;

        case 17:
            key = SDLK_e;
            break;

        case 18:
            key = SDLK_r;
            break;

        case 19:
            key = SDLK_t;
            break;

        case 20:
            key = SDLK_y;
            break;

        case 21:
            key = SDLK_u;
            break;

        case 22:
            key = SDLK_i;
            break;

        case 23:
            key = SDLK_o;
            break;

        case 24:
            key = SDLK_p;
            break;

        case 25:
            key = SDLK_LEFTBRACKET;
            break;

        case 26:
            key = SDLK_RIGHTBRACKET;
            break;

        case 27:
            key = SDLK_BACKSLASH;
            break;

        case 28:
            key = SDLK_a;
            break;

        case 29:
            key = SDLK_s;
            break;

        case 30:
            key = SDLK_d;
            break;

        case 31:
            key = SDLK_f;
            break;

        case 32:
            key = SDLK_g;
            break;

        case 33:
            key = SDLK_h;
            break;

        case 34:
            key = SDLK_j;
            break;

        case 35:
            key = SDLK_k;
            break;

        case 36:
            key = SDLK_l;
            break;

        case 37:
            key = SDLK_SEMICOLON;
            break;

        case 38:
            key = SDLK_QUOTE;
            break;

        case 39:
            key = SDLK_z;
            break;


        case 40:
            key = SDLK_x;
            break;

        case 41:
            key = SDLK_c;
            break;

        case 42:
            key = SDLK_v;
            break;

        case 43:
            key = SDLK_b;
            break;

        case 44:
            key = SDLK_n;
            break;

        case 45:
            key = SDLK_m;
            break;

        case 46:
            key = SDLK_COMMA;
            break;

        case 47:
            key = SDLK_PERIOD;
            break;

        case 48:
            key = SDLK_SLASH;
            break;

        default:
            break;
    }

    return key;
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
            mItems[dst][outfitCell++] = inventory->getItem(i)->getId();
            if (outfitCell > 8)
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
