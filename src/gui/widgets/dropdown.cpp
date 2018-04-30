/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
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

#include "gui/widgets/dropdown.h"

#include "settings.h"

#include "gui/gui.h"
#include "gui/skin.h"

#include "gui/fonts/font.h"

#include "gui/models/extendedlistmodel.h"

#include "gui/widgets/createwidget.h"
#include "gui/widgets/popuplist.h"

#include "render/graphics.h"

#include "resources/imagerect.h"

#include "debug.h"

int DropDown::instances = 0;
Image *DropDown::buttons[2][2];
ImageRect DropDown::skinRect;
float DropDown::mAlpha = 1.0;
Skin *DropDown::mSkin = nullptr;

static std::string const dropdownFiles[2] =
{
    "dropdown.xml",
    "dropdown_pressed.xml"
};

DropDown::DropDown(const Widget2 *const widget,
                   ListModel *const listModel,
                   const bool isExtended,
                   const Modal modal,
                   ActionListener *const listener,
                   const std::string &eventId) :
    ActionListener(),
    BasicContainer(widget),
    KeyListener(),
    MouseListener(),
    FocusListener(),
    SelectionListener(),
    mPopup(CREATEWIDGETR(PopupList, this, listModel, isExtended, modal)),
    mShadowColor(getThemeColor(ThemeColorId::DROPDOWN_SHADOW, 255U)),
    mHighlightColor(getThemeColor(ThemeColorId::HIGHLIGHT, 255U)),
    mPadding(1),
    mImagePadding(2),
    mSpacing(0),
    mFoldedUpHeight(0),
    mSelectionListeners(),
    mExtended(isExtended),
    mDroppedDown(false),
    mPushed(false),
    mIsDragged(false)
{
    mAllowLogic = false;
    mFrameSize = 2;
    mForegroundColor2 = getThemeColor(ThemeColorId::DROPDOWN_OUTLINE, 255U);

    mPopup->setHeight(100);

    // Initialize graphics
    if (instances == 0 && (theme != nullptr))
    {
        // Load the background skin
        for (int i = 0; i < 2; i ++)
        {
            Skin *const skin = theme->load(dropdownFiles[i],
                "dropdown.xml",
                true,
                Theme::getThemePath());
            if (skin != nullptr)
            {
                if (i == 0)
                    mSkin = skin;
                const ImageRect &rect = skin->getBorder();
                for (int f = 0; f < 2; f ++)
                {
                    if (rect.grid[f] != nullptr)
                    {
                        rect.grid[f]->incRef();
                        buttons[f][i] = rect.grid[f];
                        buttons[f][i]->setAlpha(mAlpha);
                    }
                    else
                    {
                        buttons[f][i] = nullptr;
                    }
                }
                if (i != 0)
                    theme->unload(skin);
            }
            else
            {
                for (int f = 0; f < 2; f ++)
                    buttons[f][i] = nullptr;
            }
        }

        // get the border skin
        theme->loadRect(skinRect, "dropdown_background.xml", "", 0, 8);
    }

    instances++;

    setWidth(100);
    setFocusable(true);
    setListModel(listModel);

    if (mPopup->getSelected() < 0)
        mPopup->setSelected(0);

    addMouseListener(this);
    addKeyListener(this);
    addFocusListener(this);

    adjustHeight();
//    mPopup->setForegroundColorAll(getThemeColor(ThemeColorId::DROPDOWN,
//        255U),
//        getThemeColor(ThemeColorId::DROPDOWN_OUTLINE, 255U));
    mForegroundColor = getThemeColor(ThemeColorId::DROPDOWN, 255U);
    mForegroundColor2 = getThemeColor(ThemeColorId::DROPDOWN_OUTLINE, 255U);

    if (!eventId.empty())
        setActionEventId(eventId);

    if (listener != nullptr)
        addActionListener(listener);

    mPopup->adjustSize();

    if (mSkin != nullptr)
    {
        mSpacing = mSkin->getOption("spacing");
        mFrameSize = CAST_U32(mSkin->getOption("frameSize"));
        mPadding = mSkin->getPadding();
        mImagePadding = mSkin->getOption("imagePadding");
    }
    adjustHeight();
}

DropDown::~DropDown()
{
    if (gui != nullptr)
        gui->removeDragged(this);

    instances--;
    if (instances == 0)
    {
        for (int f = 0; f < 2; f ++)
        {
            for (int i = 0; i < 2; i ++)
            {
                if (buttons[f][i] != nullptr)
                    buttons[f][i]->decRef();
            }
        }
        if (theme != nullptr)
        {
            theme->unload(mSkin);
            Theme::unloadRect(skinRect, 0, 8);
        }
    }
}

void DropDown::updateAlpha()
{
    const float alpha = std::max(settings.guiAlpha,
        theme->getMinimumOpacity());

    if (mAlpha != alpha)
    {
        mAlpha = alpha;

        if (buttons[0][0] != nullptr)
            buttons[0][0]->setAlpha(mAlpha);
        if (buttons[0][1] != nullptr)
            buttons[0][1]->setAlpha(mAlpha);
        if (buttons[1][0] != nullptr)
            buttons[1][0]->setAlpha(mAlpha);
        if (buttons[1][1] != nullptr)
            buttons[1][1]->setAlpha(mAlpha);

        for (int a = 0; a < 9; a++)
        {
            if (skinRect.grid[a] != nullptr)
                skinRect.grid[a]->setAlpha(mAlpha);
        }
    }
}

void DropDown::draw(Graphics *const graphics)
{
    BLOCK_START("DropDown::draw")
    int h;

    if (mDroppedDown)
        h = mFoldedUpHeight;
    else
        h = mDimension.height;

    updateAlpha();

    const unsigned int alpha = CAST_U32(mAlpha * 255.0F);
    const int pad = 2 * mPadding;
    mHighlightColor.a = alpha;
    mShadowColor.a = alpha;

    ListModel *const model = mPopup->getListModel();
    if ((model != nullptr) && mPopup->getSelected() >= 0)
    {
        Font *const font = getFont();
        if (mExtended)
        {
            const int sel = mPopup->getSelected();
            ExtendedListModel *const model2
                = static_cast<ExtendedListModel *>(model);
            const Image *const image = model2->getImageAt(sel);
            if (image == nullptr)
            {
                font->drawString(graphics,
                    mForegroundColor,
                    mForegroundColor2,
                    model->getElementAt(sel),
                    mPadding, mPadding);
            }
            else
            {
                graphics->drawImage(image,
                    mImagePadding,
                    (mDimension.height - image->getHeight()) / 2 + mPadding);
                font->drawString(graphics,
                    mForegroundColor,
                    mForegroundColor2,
                    model->getElementAt(sel),
                    image->getWidth() + mImagePadding + mSpacing, mPadding);
            }
        }
        else
        {
            font->drawString(graphics,
                mForegroundColor,
                mForegroundColor2,
                model->getElementAt(mPopup->getSelected()),
                mPadding, mPadding);
        }
    }

    if (isFocused())
    {
        graphics->setColor(mHighlightColor);
        graphics->drawRectangle(Rect(mPadding, mPadding,
            mDimension.width - h - pad, h - pad));
    }

    drawButton(graphics);

    if (mDroppedDown)
    {
        // Draw two lines separating the ListBox with selected
        // element view.
        const int w = mDimension.width;
        graphics->setColor(mHighlightColor);
        graphics->drawLine(0, h, w, h);
        graphics->setColor(mShadowColor);
        graphics->drawLine(0, h + 1, w, h + 1);
    }
    BLOCK_END("DropDown::draw")
}

void DropDown::safeDraw(Graphics *const graphics)
{
    DropDown::draw(graphics);
}

void DropDown::drawFrame(Graphics *const graphics)
{
    BLOCK_START("DropDown::drawFrame")
    const int bs2 = CAST_S32(getFrameSize());
    const Rect &rect = mDimension;
    graphics->drawImageRect(0, 0,
        rect.width + bs2, rect.height + bs2,
        skinRect);
    BLOCK_END("DropDown::drawFrame")
}

void DropDown::safeDrawFrame(Graphics *const graphics)
{
    BLOCK_START("DropDown::drawFrame")
    const int bs2 = CAST_S32(getFrameSize());
    const Rect &rect = mDimension;
    graphics->drawImageRect(0, 0,
        rect.width + bs2, rect.height + bs2,
        skinRect);
    BLOCK_END("DropDown::drawFrame")
}

void DropDown::drawButton(Graphics *const graphics)
{
    const int height = mDroppedDown ? mFoldedUpHeight : mDimension.height;

    Image *image = buttons[mDroppedDown][mPushed];
    if (image != nullptr)
    {
        graphics->drawImage(image,
            mDimension.width - image->getWidth() - mImagePadding,
            (height - image->getHeight()) / 2);
    }
}

void DropDown::keyPressed(KeyEvent& event)
{
    if (event.isConsumed())
        return;

    const InputActionT actionId = event.getActionId();
    PRAGMA45(GCC diagnostic push)
    PRAGMA45(GCC diagnostic ignored "-Wswitch-enum")
    switch (actionId)
    {
        case InputAction::GUI_SELECT:
        case InputAction::GUI_SELECT2:
            dropDown();
            break;

        case InputAction::GUI_UP:
            setSelected(getSelected() - 1);
            break;

        case InputAction::GUI_DOWN:
            setSelected(getSelected() + 1);
            break;

        case InputAction::GUI_HOME:
            setSelected(0);
            break;

        case InputAction::GUI_END:
            if (mPopup->getListModel() != nullptr)
            {
                setSelected(mPopup->getListModel()->
                    getNumberOfElements() - 1);
            }
            break;

        default:
            return;
    }
    PRAGMA45(GCC diagnostic pop)

    event.consume();
}

void DropDown::hideDrop(bool event)
{
    if (event)
        distributeActionEvent();
    mPopup->setVisible(Visible_false);
}

void DropDown::mousePressed(MouseEvent& event)
{
    event.consume();
    // If we have a mouse press on the widget.
    if (event.getButton() == MouseButton::LEFT
        && !mDroppedDown && event.getSource() == this)
    {
        mPushed = true;
        dropDown();
    }
    else
    {
        mPushed = false;
        foldUp();
        hideDrop(true);
    }
}

void DropDown::mouseReleased(MouseEvent &event)
{
    if (mIsDragged)
        mPushed = false;

    const MouseButtonT button = event.getButton();
    const int x = event.getX();
    const int y = event.getY();
    // Released outside of widget. Can happen when we have modal
    // input focus.
    if ((0 > y || y >= mDimension.height || x < 0 || x >= mDimension.width)
        && button == MouseButton::LEFT)
    {
        if (mIsDragged)
            foldUp();
    }
    else if (button == MouseButton::LEFT)
    {
        mPushed = false;
    }

    mIsDragged = false;
}

void DropDown::mouseDragged(MouseEvent &event)
{
    mIsDragged = true;
    event.consume();
}

void DropDown::mouseWheelMovedUp(MouseEvent& event)
{
    setSelected(getSelected() - 1);
    event.consume();
}

void DropDown::mouseWheelMovedDown(MouseEvent& event)
{
    setSelected(getSelected() + 1);
    event.consume();
}

void DropDown::setSelectedString(const std::string &str)
{
    ListModel *const listModel = mPopup->getListModel();
    if (listModel == nullptr)
        return;

    for (int f = 0; f < listModel->getNumberOfElements(); f ++)
    {
        if (listModel->getElementAt(f) == str)
        {
            setSelected(f);
            break;
        }
    }
}

std::string DropDown::getSelectedString() const
{
    ListModel *const listModel = mPopup->getListModel();
    if (listModel == nullptr)
        return "";

    return listModel->getElementAt(getSelected());
}

void DropDown::adjustHeight()
{
    setHeight(getFont()->getHeight() + 2 * mPadding);
}

void DropDown::dropDown()
{
    if (!mDroppedDown)
    {
        if (mParent == nullptr)
            return;
        mDroppedDown = true;
        mFoldedUpHeight = getHeight();
        adjustHeight();

        int x = 0;
        int y = 0;
        getAbsolutePosition(x, y);
        const int frame = CAST_S32(mParent->getFrameSize());
        const int pad = mPopup->getPadding();
        const int pad2 = pad * 2;

        // here width should be adjusted on some other parameters
        mPopup->setWidth(mDimension.width - pad2 + 8);
        mPopup->show(x - mPadding - frame - 1, y + mDimension.height);
        mPopup->requestMoveToTop();
        mPopup->requestFocus();
    }
}

void DropDown::foldUp()
{
    if (mDroppedDown)
    {
        mDroppedDown = false;
        adjustHeight();
    }
}

int DropDown::getSelected() const
{
    return mPopup->getSelected();
}

void DropDown::setSelected(int selected)
{
    if (selected >= 0)
        mPopup->setSelected(selected);
}

void DropDown::setListModel(ListModel *const listModel)
{
    mPopup->setListModel(listModel);

    if (mPopup->getSelected() < 0)
        mPopup->setSelected(0);

    adjustHeight();
}

ListModel *DropDown::getListModel()
{
    return mPopup->getListModel();
}

void DropDown::action(const ActionEvent &actionEvent A_UNUSED)
{
    foldUp();
    distributeActionEvent();
}

Rect DropDown::getChildrenArea()
{
    if (mDroppedDown)
    {
        // Calculate the children area (with the one pixel border in mind)
        return Rect(1, mFoldedUpHeight + 1,
            mDimension.width - 2, mDimension.height - mFoldedUpHeight - 2);
    }

    return Rect();
}

void DropDown::valueChanged(const SelectionEvent& event A_UNUSED)
{
}

void DropDown::updateSelection()
{
    mDroppedDown = false;
    mPushed = false;
    distributeActionEvent();
    distributeValueChangedEvent();
}

void DropDown::addSelectionListener(SelectionListener* selectionListener)
{
    mSelectionListeners.push_back(selectionListener);
}

void DropDown::removeSelectionListener(SelectionListener* listener)
{
    mSelectionListeners.remove(listener);
}

void DropDown::distributeValueChangedEvent()
{
    for (SelectionListenerIterator iter = mSelectionListeners.begin();
          iter != mSelectionListeners.end();
          ++iter)
    {
        SelectionEvent event(this);
        (*iter)->valueChanged(event);
    }
}
