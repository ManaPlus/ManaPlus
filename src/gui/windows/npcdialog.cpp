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

#include "gui/windows/npcdialog.h"

#include "actormanager.h"
#include "configuration.h"
#include "settings.h"
#include "soundmanager.h"

#include "const/sound.h"

#include "being/being.h"
#include "being/playerinfo.h"

#include "enums/gui/layouttype.h"

#include "gui/gui.h"
#include "gui/viewport.h"

#include "gui/fonts/font.h"

#include "gui/popups/popupmenu.h"

#include "gui/windows/cutinwindow.h"
#include "gui/windows/inventorywindow.h"

#include "gui/widgets/browserbox.h"
#include "gui/widgets/button.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/icon.h"
#include "gui/widgets/inttextfield.h"
#include "gui/widgets/itemcontainer.h"
#include "gui/widgets/itemlinkhandler.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/extendedlistbox.h"
#include "gui/widgets/playerbox.h"
#include "gui/widgets/scrollarea.h"

#include "resources/npcdialoginfo.h"

#include "resources/db/avatardb.h"
#include "resources/db/npcdb.h"
#include "resources/db/npcdialogdb.h"

#include "resources/inventory/complexinventory.h"

#include "resources/item/complexitem.h"

#include "resources/loaders/imageloader.h"

#include "net/npchandler.h"
#include "net/packetlimiter.h"

#include "utils/copynpaste.h"
#include "utils/delete2.h"
#include "utils/foreach.h"
#include "utils/gettext.h"

#include <sstream>

#include "debug.h"

// TRANSLATORS: npc dialog button
#define CAPTION_WAITING _("Stop waiting")
// TRANSLATORS: npc dialog button
#define CAPTION_NEXT _("Next")
// TRANSLATORS: npc dialog button
#define CAPTION_CLOSE _("Close")
// TRANSLATORS: npc dialog button
#define CAPTION_SUBMIT _("Submit")

NpcDialog::DialogList NpcDialog::instances;
NpcDialogs NpcDialog::mNpcDialogs;

typedef STD_VECTOR<Image *>::iterator ImageVectorIter;

NpcDialog::NpcDialog(const BeingId npcId) :
    // TRANSLATORS: npc dialog name
    Window(_("NPC"), Modal_false, nullptr, "npc.xml"),
    ActionListener(),
    mNpcId(npcId),
    mDefaultInt(0),
    mDefaultString(),
    mTextBox(new BrowserBox(this, Opaque_true,
        "browserbox.xml")),
    mScrollArea(new ScrollArea(this, mTextBox,
        fromBool(getOptionBool("showtextbackground"), Opaque),
        "npc_textbackground.xml")),
    mText(),
    mNewText(),
    mItems(),
    mImages(),
    mItemList(CREATEWIDGETR(ExtendedListBox,
        this, this, "extendedlistbox.xml", 13)),
    mListScrollArea(new ScrollArea(this, mItemList,
        fromBool(getOptionBool("showlistbackground"), Opaque),
        "npc_listbackground.xml")),
    mSkinContainer(new Container(this)),
    mSkinScrollArea(new ScrollArea(this, mSkinContainer,
        fromBool(getOptionBool("showlistbackground"), Opaque),
        "npc_listbackground.xml")),
    mItemLinkHandler(new ItemLinkHandler),
    mTextField(new TextField(this, std::string(), LoseFocusOnTab_true,
        nullptr, std::string(), false)),
    mIntField(new IntTextField(this, 0, 0, 0, Enable_true, 0)),
    // TRANSLATORS: npc dialog button
    mPlusButton(new Button(this, _("+"), "inc", this)),
    // TRANSLATORS: npc dialog button
    mMinusButton(new Button(this, _("-"), "dec", this)),
    // TRANSLATORS: npc dialog button
    mClearButton(new Button(this, _("Clear"), "clear", this)),
    mButton(new Button(this, "", "ok", this)),
    // TRANSLATORS: npc dialog button
    mButton2(new Button(this, _("Close"), "close", this)),
    // TRANSLATORS: npc dialog button
    mButton3(new Button(this, _("Add"), "add", this)),
    // TRANSLATORS: npc dialog button
    mResetButton(new Button(this, _("Reset"), "reset", this)),
    mInventory(new Inventory(InventoryType::Npc, 1)),
    mComplexInventory(new ComplexInventory(InventoryType::Craft, 1)),
    mItemContainer(new ItemContainer(this, mInventory,
        10000, ShowEmptyRows_true, ForceQuantity_false)),
    mItemScrollArea(new ScrollArea(this, mItemContainer,
        fromBool(getOptionBool("showitemsbackground"), Opaque),
        "npc_listbackground.xml")),
    mInputState(NpcInputState::NONE),
    mActionState(NpcActionState::WAIT),
    mSkinControls(),
    mSkinName(),
    mPlayerBox(new PlayerBox(nullptr, std::string(), std::string())),
    mAvatarBeing(nullptr),
    mDialogInfo(nullptr),
    mLastNextTime(0),
    mCameraMode(-1),
    mCameraX(0),
    mCameraY(0),
    mShowAvatar(false),
    mLogInteraction(config.getBoolValue("logNpcInGui"))
{
    // Basic Window Setup
    setWindowName("NpcText");
    setResizable(true);
    setFocusable(true);
    setStickyButtonLock(true);

    setMinWidth(200);
    setMinHeight(150);

    setDefaultSize(300, 578, ImagePosition::LOWER_LEFT);

    mPlayerBox->setWidth(70);
    mPlayerBox->setHeight(100);

    // Setup output text box
    mTextBox->setOpaque(Opaque_false);
    mTextBox->setMaxRow(config.getIntValue("ChatLogLength"));
    mTextBox->setLinkHandler(mItemLinkHandler);
    mTextBox->setProcessVars(true);
    mTextBox->setFont(gui->getNpcFont());
    mTextBox->setEnableKeys(true);
    mTextBox->setEnableTabs(true);
    mTextBox->setEnableImages(true);

    mScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mScrollArea->setVerticalScrollPolicy(ScrollArea::SHOW_ALWAYS);

    // Setup listbox
    mItemList->setWrappingEnabled(true);
    mItemList->setActionEventId("ok");
    mItemList->addActionListener(this);
    mItemList->setDistributeMousePressed(false);
    mItemList->setFont(gui->getNpcFont());
    if (gui->getNpcFont()->getHeight() < 20)
        mItemList->setRowHeight(20);
    else
        mItemList->setRowHeight(gui->getNpcFont()->getHeight());

    setContentSize(260, 175);
    mListScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mItemScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mSkinScrollArea->setScrollPolicy(ScrollArea::SHOW_NEVER,
        ScrollArea::SHOW_NEVER);
    mItemList->setVisible(Visible_true);
    mTextField->setVisible(Visible_true);
    mIntField->setVisible(Visible_true);

    const Font *const fnt = mButton->getFont();
    int width = std::max(fnt->getWidth(CAPTION_WAITING),
        fnt->getWidth(CAPTION_NEXT));
    width = std::max(width, fnt->getWidth(CAPTION_CLOSE));
    width = std::max(width, fnt->getWidth(CAPTION_SUBMIT));
    mButton->setWidth(8 + width);

    // Place widgets
    buildLayout();

    center();
    loadWindowState();

    instances.push_back(this);
}

void NpcDialog::postInit()
{
    Window::postInit();
    setVisible(Visible_true);
    requestFocus();
    enableVisibleSound(true);
    soundManager.playGuiSound(SOUND_SHOW_WINDOW);

    if (actorManager != nullptr)
    {
        const Being *const being = actorManager->findBeing(mNpcId);
        if (being != nullptr)
        {
            showAvatar(NPCDB::getAvatarFor(fromInt(
                being->getSubType(), BeingTypeId)));
            setCaption(being->getName());
        }
    }

    config.addListener("logNpcInGui", this);
}

NpcDialog::~NpcDialog()
{
    config.removeListeners(this);
    CHECKLISTENERS
    clearLayout();

    if (mPlayerBox != nullptr)
    {
        delete mPlayerBox->getBeing();
        delete mPlayerBox;
    }

    deleteSkinControls();

    delete2(mTextBox);
    delete2(mClearButton);
    delete2(mButton);
    delete2(mButton2);
    delete2(mButton3);
    delete2(mScrollArea);
    delete2(mItemList);
    delete2(mTextField);
    delete2(mIntField);
    delete2(mResetButton);
    delete2(mPlusButton);
    delete2(mMinusButton);
    delete2(mItemLinkHandler);
    delete2(mItemContainer);
    delete2(mInventory);
    delete2(mComplexInventory);
    delete2(mItemScrollArea);
    delete2(mListScrollArea);
    delete2(mSkinScrollArea);

    FOR_EACH (ImageVectorIter, it, mImages)
    {
        if (*it != nullptr)
            (*it)->decRef();
    }

    mImages.clear();

    instances.remove(this);
}

void NpcDialog::addText(const std::string &text, const bool save)
{
    if (save || mLogInteraction)
    {
        if (mText.size() > 5000)
            mText.clear();

        mNewText.append(text);
        mTextBox->addRow(text,
            false);
    }
    mScrollArea->setVerticalScrollAmount(mScrollArea->getVerticalMaxScroll());
    mActionState = NpcActionState::WAIT;
    buildLayout();
}

void NpcDialog::showNextButton()
{
    mActionState = NpcActionState::NEXT;
    buildLayout();
}

void NpcDialog::showCloseButton()
{
    mActionState = NpcActionState::CLOSE;
    buildLayout();
}

void NpcDialog::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "ok")
    {
        if (mActionState == NpcActionState::NEXT)
        {
            if (!PacketLimiter::limitPackets(PacketType::PACKET_NPC_NEXT))
                return;

            nextDialog();
            addText(std::string(), false);
        }
        else if (mActionState == NpcActionState::CLOSE
                 || mActionState == NpcActionState::WAIT)
        {
            if (cutInWindow != nullptr)
                cutInWindow->hide();
            closeDialog();
        }
        else if (mActionState == NpcActionState::INPUT)
        {
            std::string printText;  // Text that will get printed
                                    // in the textbox
            switch (mInputState)
            {
                case NpcInputState::LIST:
                {
                    if (mDialogInfo != nullptr)
                        return;
                    if (gui != nullptr)
                        gui->resetClickCount();
                    const int selectedIndex = mItemList->getSelected();

                    if (selectedIndex >= CAST_S32(mItems.size())
                        || selectedIndex < 0
                        || !PacketLimiter::limitPackets(
                        PacketType::PACKET_NPC_INPUT))
                    {
                        return;
                    }
                    unsigned char choice = CAST_U8(
                        selectedIndex + 1);
                    printText = mItems[selectedIndex];

                    npcHandler->listInput(mNpcId, choice);
                    break;
                }
                case NpcInputState::STRING:
                {
                    if (!PacketLimiter::limitPackets(
                        PacketType::PACKET_NPC_INPUT))
                    {
                        return;
                    }
                    printText = mTextField->getText();
                    npcHandler->stringInput(mNpcId, printText);
                    break;
                }
                case NpcInputState::INTEGER:
                {
                    if (!PacketLimiter::limitPackets(
                        PacketType::PACKET_NPC_INPUT))
                    {
                        return;
                    }
                    printText = strprintf("%d", mIntField->getValue());
                    npcHandler->integerInput(
                        mNpcId, mIntField->getValue());
                    break;
                }
                case NpcInputState::ITEM:
                {
                    restoreVirtuals();
                    if (!PacketLimiter::limitPackets(
                        PacketType::PACKET_NPC_INPUT))
                    {
                        return;
                    }

                    std::string str;
                    const int sz = mInventory->getSize();
                    if (sz == 0)
                    {
                        str = "0,0";
                    }
                    else
                    {
                        const Item *item = mInventory->getItem(0);
                        if (item != nullptr)
                        {
                            str = strprintf("%d,%d", item->getId(),
                                toInt(item->getColor(), int));
                        }
                        else
                        {
                            str = "0,0";
                        }
                        for (int f = 1; f < sz; f ++)
                        {
                            str.append(";");
                            item = mInventory->getItem(f);
                            if (item != nullptr)
                            {
                                str.append(strprintf("%d,%d", item->getId(),
                                    toInt(item->getColor(), int)));
                            }
                            else
                            {
                                str.append("0,0");
                            }
                        }
                    }

                    // need send selected item
                    npcHandler->stringInput(mNpcId, str);
                    mInventory->clear();
                    break;
                }
                case NpcInputState::ITEM_INDEX:
                {
                    restoreVirtuals();
                    if (!PacketLimiter::limitPackets(
                        PacketType::PACKET_NPC_INPUT))
                    {
                        return;
                    }

                    std::string str;
                    const int sz = mInventory->getSize();
                    if (sz == 0)
                    {
                        str = "-1";
                    }
                    else
                    {
                        const Item *item = mInventory->getItem(0);
                        if (item != nullptr)
                        {
                            str = strprintf("%d", item->getTag());
                        }
                        else
                        {
                            str = "-1";
                        }
                        for (int f = 1; f < sz; f ++)
                        {
                            str.append(";");
                            item = mInventory->getItem(f);
                            if (item != nullptr)
                                str.append(strprintf("%d", item->getTag()));
                            else
                                str.append("-1");
                        }
                    }

                    // need send selected item
                    npcHandler->stringInput(mNpcId, str);
                    mInventory->clear();
                    break;
                }
                case NpcInputState::ITEM_CRAFT:
                {
                    restoreVirtuals();
                    if (!PacketLimiter::limitPackets(
                        PacketType::PACKET_NPC_INPUT))
                    {
                        return;
                    }

                    std::string str;
                    const int sz = mComplexInventory->getSize();
                    if (sz == 0)
                    {
                        str.clear();
                    }
                    else
                    {
                        const ComplexItem *item = dynamic_cast<ComplexItem*>(
                            mComplexInventory->getItem(0));
                        str = complexItemToStr(item);
                        for (int f = 1; f < sz; f ++)
                        {
                            str.append("|");
                            item = dynamic_cast<ComplexItem*>(
                                mComplexInventory->getItem(f));
                            str.append(complexItemToStr(item));
                        }
                    }

                    // need send selected item
                    npcHandler->stringInput(mNpcId, str);
                    mInventory->clear();
                    break;
                }

                case NpcInputState::NONE:
                default:
                    break;
            }
            if (mInputState != NpcInputState::ITEM &&
                mInputState != NpcInputState::ITEM_INDEX &&
                mInputState != NpcInputState::ITEM_CRAFT)
            {
                // addText will auto remove the input layout
                addText(strprintf("> \"%s\"", printText.c_str()), false);
            }
            mNewText.clear();
        }

        if (!mLogInteraction)
            mTextBox->clearRows();
    }
    else if (eventId == "reset")
    {
        switch (mInputState)
        {
            case NpcInputState::STRING:
                mTextField->setText(mDefaultString);
                break;
            case NpcInputState::INTEGER:
                mIntField->setValue(mDefaultInt);
                break;
            case NpcInputState::ITEM:
            case NpcInputState::ITEM_INDEX:
                mInventory->clear();
                break;
            case NpcInputState::ITEM_CRAFT:
                mComplexInventory->clear();
                break;
            case NpcInputState::NONE:
            case NpcInputState::LIST:
            default:
                break;
        }
    }
    else if (eventId == "inc")
    {
        mIntField->setValue(mIntField->getValue() + 1);
    }
    else if (eventId == "dec")
    {
        mIntField->setValue(mIntField->getValue() - 1);
    }
    else if (eventId == "clear")
    {
        switch (mInputState)
        {
            case NpcInputState::ITEM:
            case NpcInputState::ITEM_INDEX:
                mInventory->clear();
                break;
            case NpcInputState::ITEM_CRAFT:
                mComplexInventory->clear();
                break;
            case NpcInputState::STRING:
            case NpcInputState::INTEGER:
            case NpcInputState::LIST:
            case NpcInputState::NONE:
            default:
                clearRows();
                break;
        }
    }
    else if (eventId == "close")
    {
        restoreVirtuals();
        if (mActionState == NpcActionState::INPUT)
        {
            switch (mInputState)
            {
                case NpcInputState::ITEM:
                    npcHandler->stringInput(mNpcId, "0,0");
                    break;
                case NpcInputState::ITEM_INDEX:
                    npcHandler->stringInput(mNpcId, "-1");
                    break;
                case NpcInputState::ITEM_CRAFT:
                    npcHandler->stringInput(mNpcId, "");
                    break;
                case NpcInputState::STRING:
                case NpcInputState::INTEGER:
                case NpcInputState::NONE:
                case NpcInputState::LIST:
                default:
                    npcHandler->listInput(mNpcId, 255);
                    break;
            }
            if (cutInWindow != nullptr)
                cutInWindow->hide();
            closeDialog();
        }
    }
    else if (eventId == "add")
    {
        if (inventoryWindow != nullptr)
        {
            Item *const item = inventoryWindow->getSelectedItem();
            Inventory *const inventory = PlayerInfo::getInventory();
            if (inventory != nullptr)
            {
                if (mInputState == NpcInputState::ITEM_CRAFT)
                {
                    if (mComplexInventory->addVirtualItem(item, -1, 1))
                        inventory->virtualRemove(item, 1);
                }
                else
                {
                    if (mInventory->addVirtualItem(item, -1, 1))
                        inventory->virtualRemove(item, 1);
                }
            }
        }
    }
    else if (eventId.find("skin_") == 0)
    {
        const std::string cmd = eventId.substr(5);
        std::string printText;
        int cnt = 0;
        FOR_EACH (StringVectCIter, it, mItems)
        {
            if (cmd == *it)
            {
                npcHandler->listInput(mNpcId, CAST_U8(cnt + 1));
                printText = mItems[cnt];

                if (mInputState != NpcInputState::ITEM &&
                    mInputState != NpcInputState::ITEM_INDEX &&
                    mInputState != NpcInputState::ITEM_CRAFT)
                {
                    // addText will auto remove the input layout
                    addText(strprintf("> \"%s\"", printText.c_str()), false);
                }
                mNewText.clear();
                break;
            }
            cnt ++;
        }
    }
}

void NpcDialog::nextDialog()
{
    npcHandler->nextDialog(mNpcId);
}

void NpcDialog::closeDialog()
{
    restoreCamera();
    npcHandler->closeDialog(mNpcId);
}

int NpcDialog::getNumberOfElements()
{
    return CAST_S32(mItems.size());
}

std::string NpcDialog::getElementAt(int i)
{
    return mItems[i];
}

const Image *NpcDialog::getImageAt(int i)
{
    return mImages[i];
}

void NpcDialog::choiceRequest()
{
    mItems.clear();
    FOR_EACH (ImageVectorIter, it, mImages)
    {
        if (*it != nullptr)
            (*it)->decRef();
    }
    mImages.clear();
    mActionState = NpcActionState::INPUT;
    mInputState = NpcInputState::LIST;
    buildLayout();
}

void NpcDialog::addChoice(const std::string &choice)
{
    mItems.push_back(choice);
    mImages.push_back(nullptr);
}

void NpcDialog::parseListItems(const std::string &itemString)
{
    std::istringstream iss(itemString);
    std::string tmp;
    const std::string path = paths.getStringValue("guiIcons");
    while (getline(iss, tmp, ':'))
    {
        if (tmp.empty())
            continue;
        const size_t pos = tmp.find('|');
        if (pos == std::string::npos)
        {
            mItems.push_back(tmp);
            mImages.push_back(nullptr);
        }
        else
        {
            mItems.push_back(tmp.substr(pos + 1));
            Image *const img = Loader::getImage(pathJoin(path,
                std::string(tmp.substr(0, pos)).append(".png")));
            mImages.push_back(img);
        }
    }

    if (!mItems.empty())
    {
        mItemList->setSelected(0);
        mItemList->requestFocus();
    }
    else
    {
        mItemList->setSelected(-1);
    }
}

void NpcDialog::refocus()
{
    if (!mItems.empty())
        mItemList->refocus();
}

void NpcDialog::textRequest(const std::string &defaultText)
{
    mActionState = NpcActionState::INPUT;
    mInputState = NpcInputState::STRING;
    mDefaultString = defaultText;
    mTextField->setText(defaultText);

    buildLayout();
}

bool NpcDialog::isTextInputFocused() const
{
    return mTextField->isFocused();
}

bool NpcDialog::isInputFocused() const
{
    return mTextField->isFocused() || mIntField->isFocused()
        || mItemList->isFocused();
}

bool NpcDialog::isAnyInputFocused()
{
    FOR_EACH (DialogList::const_iterator, it, instances)
    {
        if (((*it) != nullptr) && (*it)->isInputFocused())
            return true;
    }

    return false;
}

void NpcDialog::integerRequest(const int defaultValue,
                               const int min,
                               const int max)
{
    mActionState = NpcActionState::INPUT;
    mInputState = NpcInputState::INTEGER;
    mDefaultInt = defaultValue;
    mIntField->setRange(min, max);
    mIntField->setValue(defaultValue);
    buildLayout();
}

void NpcDialog::itemRequest(const int size)
{
    mActionState = NpcActionState::INPUT;
    mInputState = NpcInputState::ITEM;
    mInventory->resize(size);
    buildLayout();
}

void NpcDialog::itemIndexRequest(const int size)
{
    mActionState = NpcActionState::INPUT;
    mInputState = NpcInputState::ITEM_INDEX;
    mInventory->resize(size);
    buildLayout();
}

void NpcDialog::itemCraftRequest(const int size)
{
    mActionState = NpcActionState::INPUT;
    mInputState = NpcInputState::ITEM_CRAFT;
    mComplexInventory->resize(size);
    buildLayout();
}

void NpcDialog::move(const int amount)
{
    if (mActionState != NpcActionState::INPUT)
        return;

    switch (mInputState)
    {
        case NpcInputState::INTEGER:
            mIntField->setValue(mIntField->getValue() + amount);
            break;
        case NpcInputState::LIST:
            mItemList->setSelected(mItemList->getSelected() - amount);
            break;
        case NpcInputState::NONE:
        case NpcInputState::STRING:
        case NpcInputState::ITEM:
        case NpcInputState::ITEM_INDEX:
        case NpcInputState::ITEM_CRAFT:
        default:
            break;
    }
}

void NpcDialog::setVisible(Visible visible)
{
    Window::setVisible(visible);

    if (visible == Visible_false)
        scheduleDelete();
}

void NpcDialog::optionChanged(const std::string &name)
{
    if (name == "logNpcInGui")
        mLogInteraction = config.getBoolValue("logNpcInGui");
}

NpcDialog *NpcDialog::getActive()
{
    if (instances.size() == 1)
        return instances.front();

    FOR_EACH (DialogList::const_iterator, it, instances)
    {
        if (((*it) != nullptr) && (*it)->isFocused())
            return (*it);
    }

    return nullptr;
}

void NpcDialog::closeAll()
{
    FOR_EACH (DialogList::const_iterator, it, instances)
    {
        if (*it != nullptr)
            (*it)->close();
    }
}

void NpcDialog::placeNormalControls()
{
    if (mShowAvatar)
    {
        place(0, 0, mPlayerBox, 1, 1);
        place(1, 0, mScrollArea, 5, 3);
        place(4, 3, mClearButton, 1, 1);
        place(5, 3, mButton, 1, 1);
    }
    else
    {
        place(0, 0, mScrollArea, 5, 3);
        place(3, 3, mClearButton, 1, 1);
        place(4, 3, mButton, 1, 1);
    }
}

void NpcDialog::placeMenuControls()
{
    if (mShowAvatar)
    {
        place(0, 0, mPlayerBox, 1, 1);
        place(1, 0, mScrollArea, 6, 3);
        place(0, 3, mListScrollArea, 7, 3);
        place(1, 6, mButton2, 2, 1);
        place(3, 6, mClearButton, 2, 1);
        place(5, 6, mButton, 2, 1);
    }
    else
    {
        place(0, 0, mScrollArea, 6, 3);
        place(0, 3, mListScrollArea, 6, 3);
        place(0, 6, mButton2, 2, 1);
        place(2, 6, mClearButton, 2, 1);
        place(4, 6, mButton, 2, 1);
    }
}

void NpcDialog::placeSkinControls()
{
    createSkinControls();
    if ((mDialogInfo != nullptr) && mDialogInfo->hideText)
    {
        if (mShowAvatar)
        {
            place(0, 0, mPlayerBox, 1, 1);
            place(1, 0, mSkinScrollArea, 7, 3);
            place(1, 3, mButton2, 2, 1);
        }
        else
        {
            place(0, 0, mSkinScrollArea, 6, 3);
            place(0, 3, mButton2, 2, 1);
        }
    }
    else
    {
        if (mShowAvatar)
        {
            place(0, 0, mPlayerBox, 1, 1);
            place(1, 0, mScrollArea, 6, 3);
            place(0, 3, mSkinScrollArea, 7, 3);
            place(1, 6, mButton2, 2, 1);
        }
        else
        {
            place(0, 0, mScrollArea, 6, 3);
            place(0, 3, mSkinScrollArea, 6, 3);
            place(0, 6, mButton2, 2, 1);
        }
    }
}

void NpcDialog::placeTextInputControls()
{
    if (mShowAvatar)
    {
        place(0, 0, mPlayerBox, 1, 1);
        place(1, 0, mScrollArea, 6, 3);
        place(1, 3, mTextField, 6, 1);
        place(1, 4, mResetButton, 2, 1);
        place(3, 4, mClearButton, 2, 1);
        place(5, 4, mButton, 2, 1);
    }
    else
    {
        place(0, 0, mScrollArea, 6, 3);
        place(0, 3, mTextField, 6, 1);
        place(0, 4, mResetButton, 2, 1);
        place(2, 4, mClearButton, 2, 1);
        place(4, 4, mButton, 2, 1);
    }
}

void NpcDialog::placeIntInputControls()
{
    if (mShowAvatar)
    {
        place(0, 0, mPlayerBox, 1, 1);
        place(1, 0, mScrollArea, 6, 3);
        place(1, 3, mMinusButton, 1, 1);
        place(2, 3, mIntField, 4, 1);
        place(6, 3, mPlusButton, 1, 1);
        place(1, 4, mResetButton, 2, 1);
        place(3, 4, mClearButton, 2, 1);
        place(5, 4, mButton, 2, 1);
    }
    else
    {
        place(0, 0, mScrollArea, 6, 3);
        place(0, 3, mMinusButton, 1, 1);
        place(1, 3, mIntField, 4, 1);
        place(5, 3, mPlusButton, 1, 1);
        place(0, 4, mResetButton, 2, 1);
        place(2, 4, mClearButton, 2, 1);
        place(4, 4, mButton, 2, 1);
    }
}

void NpcDialog::placeItemInputControls()
{
    if (mDialogInfo != nullptr)
    {
        mItemContainer->setCellBackgroundImage(mDialogInfo->inventory.cell);
        mItemContainer->setMaxColumns(mDialogInfo->inventory.columns);
    }
    else
    {
        mItemContainer->setCellBackgroundImage("inventory_cell.xml");
        mItemContainer->setMaxColumns(10000);
    }

    if (mInputState == NpcInputState::ITEM_CRAFT)
        mItemContainer->setInventory(mComplexInventory);
    else
        mItemContainer->setInventory(mInventory);

    if ((mDialogInfo != nullptr) && mDialogInfo->hideText)
    {
        if (mShowAvatar)
        {
            place(0, 0, mPlayerBox, 1, 1);
            place(1, 0, mItemScrollArea, 7, 3);
            place(1, 3, mButton3, 2, 1);
            place(3, 3, mClearButton, 2, 1);
            place(5, 3, mButton, 2, 1);
        }
        else
        {
            place(0, 0, mItemScrollArea, 6, 3);
            place(0, 3, mButton3, 2, 1);
            place(2, 3, mClearButton, 2, 1);
            place(4, 3, mButton, 2, 1);
        }
    }
    else
    {
        if (mShowAvatar)
        {
            place(0, 0, mPlayerBox, 1, 1);
            place(1, 0, mScrollArea, 6, 3);
            place(0, 3, mItemScrollArea, 7, 3);
            place(1, 6, mButton3, 2, 1);
            place(3, 6, mClearButton, 2, 1);
            place(5, 6, mButton, 2, 1);
        }
        else
        {
            place(0, 0, mScrollArea, 6, 3);
            place(0, 3, mItemScrollArea, 6, 3);
            place(0, 6, mButton3, 2, 1);
            place(2, 6, mClearButton, 2, 1);
            place(4, 6, mButton, 2, 1);
        }
    }
}

void NpcDialog::buildLayout()
{
    clearLayout();

    if (mActionState != NpcActionState::INPUT)
    {
        if (mActionState == NpcActionState::WAIT)
            mButton->setCaption(CAPTION_WAITING);
        else if (mActionState == NpcActionState::NEXT)
            mButton->setCaption(CAPTION_NEXT);
        else if (mActionState == NpcActionState::CLOSE)
            mButton->setCaption(CAPTION_CLOSE);
        placeNormalControls();
    }
    else if (mInputState != NpcInputState::NONE)
    {
        mButton->setCaption(CAPTION_SUBMIT);
        switch (mInputState)
        {
            case NpcInputState::LIST:
                if (mDialogInfo == nullptr)
                    placeMenuControls();
                else
                    placeSkinControls();
                mItemList->setSelected(-1);
                break;

            case NpcInputState::STRING:
                placeTextInputControls();
                break;

            case NpcInputState::INTEGER:
                placeIntInputControls();
                break;

            case NpcInputState::ITEM:
            case NpcInputState::ITEM_INDEX:
            case NpcInputState::ITEM_CRAFT:
                placeItemInputControls();
                break;

            case NpcInputState::NONE:
            default:
                break;
        }
    }

    Layout &layout = getLayout();
    layout.setRowHeight(1, LayoutType::SET);
    redraw();
    mScrollArea->setVerticalScrollAmount(mScrollArea->getVerticalMaxScroll());
}

void NpcDialog::saveCamera()
{
    if ((viewport == nullptr) || mCameraMode >= 0)
        return;

    mCameraMode = CAST_S32(settings.cameraMode);
    mCameraX = viewport->getCameraRelativeX();
    mCameraY = viewport->getCameraRelativeY();
}

void NpcDialog::restoreCamera()
{
    if ((viewport == nullptr) || mCameraMode == -1)
        return;

    if (CAST_S32(settings.cameraMode) != mCameraMode)
        viewport->toggleCameraMode();
    if (mCameraMode != 0)
    {
        viewport->setCameraRelativeX(mCameraX);
        viewport->setCameraRelativeY(mCameraY);
    }
    mCameraMode = -1;
}

void NpcDialog::showAvatar(const BeingTypeId avatarId)
{
    const bool needShow = (avatarId != BeingTypeId_zero);
    if (needShow)
    {
        delete mAvatarBeing;
        mAvatarBeing = Being::createBeing(BeingId_zero,
            ActorType::Avatar,
            avatarId,
            nullptr);
        mPlayerBox->setPlayer(mAvatarBeing);
        if (!mAvatarBeing->mSprites.empty())
        {
            mAvatarBeing->logic();
            const BeingInfo *const info = AvatarDB::get(avatarId);
            const int pad2 = 2 * mPadding;
            int width = 0;
            if (info != nullptr)
            {
                width = info->getWidth();
                mPlayerBox->setWidth(width + pad2);
                mPlayerBox->setHeight(info->getHeight() + pad2);
            }
            const Sprite *const sprite = mAvatarBeing->mSprites[0];
            if ((sprite != nullptr) && (width == 0))
            {
                mPlayerBox->setWidth(sprite->getWidth() + pad2);
                mPlayerBox->setHeight(sprite->getHeight() + pad2);
            }
        }
    }
    else
    {
        delete2(mAvatarBeing)
        mPlayerBox->setPlayer(nullptr);
    }
    if (needShow != mShowAvatar)
    {
        mShowAvatar = needShow;
        buildLayout();
    }
    else
    {
        mShowAvatar = needShow;
    }
}

void NpcDialog::setAvatarDirection(const uint8_t direction)
{
    Being *const being = mPlayerBox->getBeing();
    if (being != nullptr)
        being->setDirection(direction);
}

void NpcDialog::setAvatarAction(const int actionId)
{
    Being *const being = mPlayerBox->getBeing();
    if (being != nullptr)
        being->setAction(static_cast<BeingActionT>(actionId), 0);
}

void NpcDialog::logic()
{
    BLOCK_START("NpcDialog::logic")
    Window::logic();
    if (mShowAvatar && (mAvatarBeing != nullptr))
    {
        mAvatarBeing->logic();
        if (mPlayerBox->getWidth() < CAST_S32(3 * getPadding()))
        {
            const Sprite *const sprite = mAvatarBeing->mSprites[0];
            if (sprite != nullptr)
            {
                mPlayerBox->setWidth(sprite->getWidth() + 2 * getPadding());
                mPlayerBox->setHeight(sprite->getHeight() + 2 * getPadding());
                buildLayout();
            }
        }
    }
    BLOCK_END("NpcDialog::logic")
}

void NpcDialog::clearRows()
{
    mTextBox->clearRows();
}

void NpcDialog::clearDialogs()
{
    NpcDialogs::iterator it = mNpcDialogs.begin();
    const NpcDialogs::iterator it_end = mNpcDialogs.end();
    while (it != it_end)
    {
        delete (*it).second;
        ++ it;
    }
    mNpcDialogs.clear();
}

void NpcDialog::mousePressed(MouseEvent &event)
{
    Window::mousePressed(event);
    if (event.getButton() == MouseButton::RIGHT
        && event.getSource() == mTextBox)
    {
        event.consume();
        if (popupMenu != nullptr)
        {
            popupMenu->showNpcDialogPopup(mNpcId,
                viewport->mMouseX,
                viewport->mMouseY);
        }
    }
}

void NpcDialog::copyToClipboard(const int x, const int y) const
{
    std::string str = mTextBox->getTextAtPos(x, y);
    sendBuffer(str);
}

void NpcDialog::setSkin(const std::string &skin)
{
    if (skin.empty())
    {
        mSkinName = skin;
        mDialogInfo = nullptr;
        return;
    }
    const NpcDialogInfo *const dialog = NpcDialogDB::getDialog(skin);
    if (dialog == nullptr)
    {
        logger->log("Error: creating controls for not existing npc dialog %s",
            skin.c_str());
        return;
    }
    mSkinName = skin;
    mDialogInfo = dialog;
}

void NpcDialog::deleteSkinControls()
{
    mSkinContainer->removeControls();
}

void NpcDialog::createSkinControls()
{
    deleteSkinControls();

    if (mDialogInfo == nullptr)
        return;

    FOR_EACH (STD_VECTOR<NpcImageInfo*>::const_iterator,
        it,
        mDialogInfo->menu.images)
    {
        const NpcImageInfo *const info = *it;
        Image *const image = Theme::getImageFromTheme(info->name);
        if (image != nullptr)
        {
            Icon *const icon = new Icon(this, image, AutoRelease_true);
            icon->setPosition(info->x, info->y);
            mSkinContainer->add(icon);
        }
    }
    FOR_EACH (STD_VECTOR<NpcTextInfo*>::const_iterator,
        it,
        mDialogInfo->menu.texts)
    {
        const NpcTextInfo *const info = *it;
        BrowserBox *box = new BrowserBox(this,
            Opaque_true,
            "browserbox.xml");
        box->setOpaque(Opaque_false);
        box->setMaxRow(config.getIntValue("ChatLogLength"));
        box->setLinkHandler(mItemLinkHandler);
        box->setProcessVars(true);
        box->setFont(gui->getNpcFont());
        box->setEnableKeys(true);
        box->setEnableTabs(true);
        box->setPosition(info->x, info->y);
        mSkinContainer->add(box);
        box->setWidth(info->width);
        box->setHeight(info->height);
        StringVect parts;
        splitToStringVector(parts, info->text, '\n');
        FOR_EACH (StringVectCIter, it2, parts)
        {
            box->addRow(*it2,
                false);
        }
    }
    FOR_EACH (STD_VECTOR<NpcButtonInfo*>::const_iterator,
        it,
        mDialogInfo->menu.buttons)
    {
        const NpcButtonInfo *const info = *it;
        Button *const button = new Button(this);
        button->setCaption(info->name);
        button->setActionEventId("skin_" + info->value);
        button->addActionListener(this);
        button->setPosition(info->x, info->y);
        if (!info->image.empty())
        {
            button->setImageWidth(info->imageWidth);
            button->setImageHeight(info->imageHeight);
            button->loadImageSet(info->image);
        }
        mSkinContainer->add(button);
        button->adjustSize();
    }
}

void NpcDialog::restoreVirtuals()
{
    Inventory *const inventory = PlayerInfo::getInventory();
    if (inventory != nullptr)
        inventory->restoreVirtuals();
}

std::string NpcDialog::complexItemToStr(const ComplexItem *const item)
{
    std::string str;
    if (item != nullptr)
    {
        const STD_VECTOR<Item*> &items = item->getChilds();
        const size_t sz = items.size();
        if (sz == 0u)
            return str;

        const Item *item2 = items[0];

        str = strprintf("%d,%d",
            item2->getInvIndex(),
            item2->getQuantity());
        for (size_t f = 1; f < sz; f ++)
        {
            str.append(";");
            item2 = items[f];
            str.append(strprintf("%d,%d",
                item2->getInvIndex(),
                item2->getQuantity()));
        }
    }
    else
    {
        str.clear();
    }
    return str;
}

void NpcDialog::addCraftItem(Item *const item,
                             const int amount,
                             const int slot)
{
    if (mInputState != NpcInputState::ITEM_CRAFT)
        return;

    Inventory *const inventory = PlayerInfo::getInventory();

    if (inventory == nullptr)
        return;

    if (mComplexInventory->addVirtualItem(
        item,
        slot,
        amount))
    {
        inventory->virtualRemove(item, amount);
    }
}
