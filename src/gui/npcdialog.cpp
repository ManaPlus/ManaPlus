/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "gui/npcdialog.h"

#include "actorspritemanager.h"
#include "being.h"
#include "configuration.h"
#include "client.h"
#include "soundmanager.h"

#include "gui/gui.h"
#include "gui/sdlfont.h"
#include "gui/setup.h"
#include "gui/viewport.h"

#include "gui/widgets/browserbox.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/inttextfield.h"
#include "gui/widgets/itemlinkhandler.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/extendedlistbox.h"
#include "gui/widgets/playerbox.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/textbox.h"

#include "resources/npcdb.h"
#include "resources/resourcemanager.h"

#include "net/net.h"
#include "net/npchandler.h"

#include "utils/gettext.h"

#include <guichan/font.hpp>

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

typedef std::vector<Image *>::iterator ImageVectorIter;

NpcDialog::NpcDialog(const int npcId) :
    // TRANSLATORS: npc dialog name
    Window(_("NPC"), false, nullptr, "npc.xml"),
    gcn::ActionListener(),
    mNpcId(npcId),
    mLogInteraction(config.getBoolValue("logNpcInGui")),
    mDefaultInt(0),
    mDefaultString(),
    mTextBox(new BrowserBox(this, BrowserBox::AUTO_WRAP)),
    mScrollArea(new ScrollArea(mTextBox,
        getOptionBool("showtextbackground"), "npc_textbackground.xml")),
    mText(),
    mNewText(),
    mItemList(new ExtendedListBox(this, this, "extendedlistbox.xml")),
    mListScrollArea(new ScrollArea(mItemList,
        getOptionBool("showlistbackground"), "npc_listbackground.xml")),
    mItems(),
    mImages(),
    mItemLinkHandler(new ItemLinkHandler),
    mTextField(new TextField(this, "")),
    mIntField(new IntTextField(this)),
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
    mResetButton(new Button(this, _("Reset"), "reset", this)),
    mInputState(NPC_INPUT_NONE),
    mActionState(NPC_ACTION_WAIT),
    mLastNextTime(0),
    mCameraMode(-1),
    mCameraX(0),
    mCameraY(0),
    mPlayerBox(new PlayerBox(nullptr)),
    mAvatarBeing(nullptr),
    mShowAvatar(false)
{
    // Basic Window Setup
    setWindowName("NpcText");
    setResizable(true);
    setFocusable(true);
    setStickyButtonLock(true);

    setMinWidth(200);
    setMinHeight(150);

    setDefaultSize(300, 578, ImageRect::LOWER_LEFT);

    mPlayerBox->setWidth(70);
    mPlayerBox->setHeight(100);

    // Setup output text box
    mTextBox->setOpaque(false);
    mTextBox->setMaxRow(config.getIntValue("ChatLogLength"));
    mTextBox->setLinkHandler(mItemLinkHandler);
    mTextBox->setFont(gui->getNpcFont());

    mScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mScrollArea->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);

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
    mListScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mItemList->setVisible(true);
    mTextField->setVisible(true);
    mIntField->setVisible(true);

    const gcn::Font *const fnt = mButton->getFont();
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
    setVisible(true);
    requestFocus();
    enableVisibleSound(true);
    soundManager.playGuiSound(SOUND_SHOW_WINDOW);

    if (actorSpriteManager)
    {
        const Being *const being = actorSpriteManager->findBeing(mNpcId);
        if (being)
            showAvatar(NPCDB::getAvatarFor(being->getSubType()));
    }

    config.addListener("logNpcInGui", this);
}

NpcDialog::~NpcDialog()
{
    config.removeListener("logNpcInGui", this);

    clearLayout();

    if (mPlayerBox)
    {
        delete mPlayerBox->getBeing();
        delete mPlayerBox;
    }

    delete mTextBox;
    mTextBox = nullptr;
    delete mClearButton;
    mClearButton = nullptr;
    delete mButton;
    mButton = nullptr;
    delete mButton2;
    mButton2 = nullptr;

    // These might not actually be in the layout, so lets be safe
    delete mScrollArea;
    mScrollArea = nullptr;
    delete mItemList;
    mItemList = nullptr;
    delete mTextField;
    mTextField = nullptr;
    delete mIntField;
    mIntField = nullptr;
    delete mResetButton;
    mResetButton = nullptr;
    delete mPlusButton;
    mPlusButton = nullptr;
    delete mMinusButton;
    mMinusButton = nullptr;
    delete mItemLinkHandler;
    mItemLinkHandler = nullptr;

    delete mListScrollArea;
    mListScrollArea = nullptr;

    FOR_EACH (ImageVectorIter, it, mImages)
    {
        if (*it)
            (*it)->decRef();
    }

    mImages.clear();

    instances.remove(this);
}

/*
void NpcDialog::setText(const std::string &text)
{
    mText = text;

    mTextBox->setTextWrapped(mText, mScrollArea->getWidth() - 15);
}
*/

void NpcDialog::addText(const std::string &text, const bool save)
{
    if (save || mLogInteraction)
    {
        if (mText.size() > 5000)
            mText.clear();

        mNewText.append(text);
        mTextBox->addRow(text);
//        setText(mText + text + "\n");
    }
    mScrollArea->setVerticalScrollAmount(mScrollArea->getVerticalMaxScroll());
    mActionState = NPC_ACTION_WAIT;
    buildLayout();
}

void NpcDialog::showNextButton()
{
    mActionState = NPC_ACTION_NEXT;
    buildLayout();
}

void NpcDialog::showCloseButton()
{
    mActionState = NPC_ACTION_CLOSE;
    buildLayout();
}

void NpcDialog::action(const gcn::ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "ok")
    {
        if (mActionState == NPC_ACTION_NEXT)
        {
            if (!Client::limitPackets(PACKET_NPC_NEXT))
                return;

            nextDialog();
            addText(std::string(), false);
        }
        else if (mActionState == NPC_ACTION_CLOSE
                 || mActionState == NPC_ACTION_WAIT)
        {
            closeDialog();
        }
        else if (mActionState == NPC_ACTION_INPUT)
        {
            std::string printText("");  // Text that will get printed
                                        // in the textbox

            if (mInputState == NPC_INPUT_LIST)
            {
                if (gui)
                    gui->resetClickCount();
                const int selectedIndex = mItemList->getSelected();

                if (selectedIndex >= static_cast<int>(mItems.size())
                    || selectedIndex < 0
                    || !Client::limitPackets(PACKET_NPC_INPUT))
                {
                    return;
                }
                unsigned char choice = static_cast<unsigned char>(
                    selectedIndex + 1);
                printText = mItems[selectedIndex];

                Net::getNpcHandler()->listInput(mNpcId, choice);
            }
            else if (mInputState == NPC_INPUT_STRING)
            {
                if (!Client::limitPackets(PACKET_NPC_INPUT))
                    return;
                printText = mTextField->getText();
                Net::getNpcHandler()->stringInput(mNpcId, printText);
            }
            else if (mInputState == NPC_INPUT_INTEGER)
            {
                if (!Client::limitPackets(PACKET_NPC_INPUT))
                    return;
                printText = strprintf("%d", mIntField->getValue());
                Net::getNpcHandler()->integerInput(
                    mNpcId, mIntField->getValue());
            }
            // addText will auto remove the input layout
            addText(strprintf("> \"%s\"", printText.c_str()), false);

            mNewText.clear();
        }

        if (!mLogInteraction)
            mTextBox->clearRows();
    }
    else if (eventId == "reset")
    {
        if (mInputState == NPC_INPUT_STRING)
            mTextField->setText(mDefaultString);
        else if (mInputState == NPC_INPUT_INTEGER)
            mIntField->setValue(mDefaultInt);
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
        clearRows();
    }
    else if (eventId == "close")
    {
        if (mActionState == NPC_ACTION_INPUT)
        {
            Net::getNpcHandler()->listInput(mNpcId, 255);
            closeDialog();
        }
    }
}

void NpcDialog::nextDialog()
{
    Net::getNpcHandler()->nextDialog(mNpcId);
}

void NpcDialog::closeDialog()
{
    restoreCamera();
    Net::getNpcHandler()->closeDialog(mNpcId);
}

int NpcDialog::getNumberOfElements()
{
    return static_cast<int>(mItems.size());
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
        if (*it)
            (*it)->decRef();
    }
    mImages.clear();
    mActionState = NPC_ACTION_INPUT;
    mInputState = NPC_INPUT_LIST;
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
    ResourceManager *const resman = ResourceManager::getInstance();

    std::string tmp;
    while (getline(iss, tmp, ':'))
    {
        const size_t pos = tmp.find("|");
        if (pos == std::string::npos)
        {
            mItems.push_back(tmp);
            mImages.push_back(nullptr);
        }
        else
        {
            mItems.push_back(tmp.substr(pos + 1));
            Image *const img = resman->getImage(
                paths.getStringValue("guiIcons")
                .append(tmp.substr(0, pos)).append(".png"));
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
    mActionState = NPC_ACTION_INPUT;
    mInputState = NPC_INPUT_STRING;
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
        if ((*it) && (*it)->isInputFocused())
            return true;
    }

    return false;
}

void NpcDialog::integerRequest(const int defaultValue, const int min,
                               const int max)
{
    mActionState = NPC_ACTION_INPUT;
    mInputState = NPC_INPUT_INTEGER;
    mDefaultInt = defaultValue;
    mIntField->setRange(min, max);
    mIntField->setValue(defaultValue);
    buildLayout();
}

void NpcDialog::move(const int amount)
{
    if (mActionState != NPC_ACTION_INPUT)
        return;

    switch (mInputState)
    {
        case NPC_INPUT_INTEGER:
            mIntField->setValue(mIntField->getValue() + amount);
            break;
        case NPC_INPUT_LIST:
            mItemList->setSelected(mItemList->getSelected() - amount);
            break;
        case NPC_INPUT_NONE:
        case NPC_INPUT_STRING:
        default:
            break;
    }
}

void NpcDialog::setVisible(bool visible)
{
    Window::setVisible(visible);

    if (!visible)
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
        if ((*it) && (*it)->isFocused())
            return (*it);
    }

    return nullptr;
}

void NpcDialog::closeAll()
{
    FOR_EACH (DialogList::const_iterator, it, instances)
    {
        if (*it)
            (*it)->close();
    }
}

void NpcDialog::placeNormalControls()
{
    if (mShowAvatar)
    {
        place(0, 0, mPlayerBox);
        place(1, 0, mScrollArea, 5, 3);
        place(4, 3, mClearButton);
        place(5, 3, mButton);
    }
    else
    {
        place(0, 0, mScrollArea, 5, 3);
        place(3, 3, mClearButton);
        place(4, 3, mButton);
    }
}

void NpcDialog::placeMenuControls()
{
    if (mShowAvatar)
    {
        place(0, 0, mPlayerBox);
        place(1, 0, mScrollArea, 6, 3);
        place(0, 3, mListScrollArea, 7, 3);
        place(1, 6, mButton2, 2);
        place(3, 6, mClearButton, 2);
        place(5, 6, mButton, 2);
    }
    else
    {
        place(0, 0, mScrollArea, 6, 3);
        place(0, 3, mListScrollArea, 6, 3);
        place(0, 6, mButton2, 2);
        place(2, 6, mClearButton, 2);
        place(4, 6, mButton, 2);
    }
}

void NpcDialog::placeTextInputControls()
{
    if (mShowAvatar)
    {
        place(0, 0, mPlayerBox);
        place(1, 0, mScrollArea, 6, 3);
        place(0, 3, mTextField, 6);
        place(0, 4, mResetButton, 2);
        place(4, 4, mClearButton, 2);
        place(5, 4, mButton, 2);
    }
    else
    {
        place(0, 0, mScrollArea, 6, 3);
        place(0, 3, mTextField, 6);
        place(0, 4, mResetButton, 2);
        place(2, 4, mClearButton, 2);
        place(4, 4, mButton, 2);
    }
}

void NpcDialog::placeIntInputControls()
{
    if (mShowAvatar)
    {
        place(0, 0, mPlayerBox);
        place(1, 0, mScrollArea, 6, 3);
        place(1, 3, mMinusButton, 1);
        place(2, 3, mIntField, 4);
        place(6, 3, mPlusButton, 1);
        place(0, 4, mResetButton, 2);
        place(3, 4, mClearButton, 2);
        place(5, 4, mButton, 2);
    }
    else
    {
        place(0, 0, mScrollArea, 6, 3);
        place(0, 3, mMinusButton, 1);
        place(1, 3, mIntField, 4);
        place(5, 3, mPlusButton, 1);
        place(0, 4, mResetButton, 2);
        place(2, 4, mClearButton, 2);
        place(4, 4, mButton, 2);
    }
}

void NpcDialog::buildLayout()
{
    clearLayout();

    if (mActionState != NPC_ACTION_INPUT)
    {
        if (mActionState == NPC_ACTION_WAIT)
            mButton->setCaption(CAPTION_WAITING);
        else if (mActionState == NPC_ACTION_NEXT)
            mButton->setCaption(CAPTION_NEXT);
        else if (mActionState == NPC_ACTION_CLOSE)
            mButton->setCaption(CAPTION_CLOSE);
        placeNormalControls();
    }
    else if (mInputState != NPC_INPUT_NONE)
    {
        mButton->setCaption(CAPTION_SUBMIT);
        if (mInputState == NPC_INPUT_LIST)
        {
            placeMenuControls();
            mItemList->setSelected(-1);
        }
        else if (mInputState == NPC_INPUT_STRING)
        {
            placeTextInputControls();
        }
        else if (mInputState == NPC_INPUT_INTEGER)
        {
            placeIntInputControls();
        }
    }

    Layout &layout = getLayout();
    layout.setRowHeight(1, Layout::AUTO_SET);

    redraw();

    mScrollArea->setVerticalScrollAmount(mScrollArea->getVerticalMaxScroll());
}

void NpcDialog::saveCamera()
{
    if (!viewport || mCameraMode >= 0)
        return;

    mCameraMode = viewport->getCameraMode();
    mCameraX = viewport->getCameraRelativeX();
    mCameraY = viewport->getCameraRelativeY();
}

void NpcDialog::restoreCamera()
{
    if (!viewport || mCameraMode == -1)
        return;

    if (!mCameraMode)
    {
        if (viewport->getCameraMode() != mCameraMode)
            viewport->toggleCameraMode();
    }
    else
    {
        if (viewport->getCameraMode() != mCameraMode)
            viewport->toggleCameraMode();
        viewport->setCameraRelativeX(mCameraX);
        viewport->setCameraRelativeY(mCameraY);
    }
    mCameraMode = -1;
}

void NpcDialog::showAvatar(const uint16_t avatarId)
{
    const bool needShow = (avatarId != 0);
    if (needShow)
    {
        delete mAvatarBeing;
        mAvatarBeing = new Being(0, ActorSprite::AVATAR, avatarId, nullptr);
        mPlayerBox->setPlayer(mAvatarBeing);
        if (!mAvatarBeing->empty())
        {
            mAvatarBeing->logic();
            const Sprite *const sprite = mAvatarBeing->getSprite(0);
            if (sprite)
            {
                mPlayerBox->setWidth(sprite->getWidth() + 2 * getPadding());
                mPlayerBox->setHeight(sprite->getHeight() + 2 * getPadding());
            }
        }
    }
    else
    {
        delete mAvatarBeing;
        mAvatarBeing = nullptr;
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
    if (being)
        being->setDirection(direction);
}

void NpcDialog::setAvatarAction(const int actionId)
{
    Being *const being = mPlayerBox->getBeing();
    if (being)
        being->setAction(static_cast<Being::Action>(actionId));
}

void NpcDialog::logic()
{
    BLOCK_START("NpcDialog::logic")
    Window::logic();
    if (mShowAvatar && mAvatarBeing)
    {
        mAvatarBeing->logic();
        if (mPlayerBox->getWidth() < static_cast<signed>(3 * getPadding()))
        {
            const Sprite *const sprite = mAvatarBeing->getSprite(0);
            if (sprite)
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
