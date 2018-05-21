/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "gui/widgets/button.h"

#include "settings.h"

#include "resources/imageset.h"
#include "resources/imagerect.h"

#include "resources/image/image.h"

#include "gui/gui.h"
#include "gui/skin.h"

#include "gui/fonts/font.h"

#include "render/renderers.h"

#include "render/vertexes/imagecollection.h"

#include "utils/delete2.h"

#include "debug.h"

float Button::mAlpha = 1.0;

static std::string const data[Button::BUTTON_COUNT] =
{
    ".xml",
    "_highlighted.xml",
    "_pressed.xml",
    "_disabled.xml"
};

Button::Button(const Widget2 *const widget,
               const std::string &restrict skinName) :
    Widget(widget),
    MouseListener(),
    KeyListener(),
    FocusListener(),
    WidgetListener(),
    mSkin(),
    mCaption(),
    mDescription(),
    mSkinName(skinName),
    mTextChunk(),
    mVertexes2(new ImageCollection),
    mEnabledColor(getThemeColor(ThemeColorId::BUTTON, 255U)),
    mEnabledColor2(getThemeColor(ThemeColorId::BUTTON_OUTLINE, 255U)),
    mDisabledColor(getThemeColor(ThemeColorId::BUTTON_DISABLED, 255U)),
    mDisabledColor2(getThemeColor(ThemeColorId::BUTTON_DISABLED_OUTLINE,
        255U)),
    mHighlightedColor(getThemeColor(ThemeColorId::BUTTON_HIGHLIGHTED, 255U)),
    mHighlightedColor2(getThemeColor(
        ThemeColorId::BUTTON_HIGHLIGHTED_OUTLINE, 255U)),
    mPressedColor(getThemeColor(ThemeColorId::BUTTON_PRESSED, 255U)),
    mPressedColor2(getThemeColor(ThemeColorId::BUTTON_PRESSED_OUTLINE, 255U)),
    mImages(nullptr),
    mImageSet(nullptr),
    mAlignment(Graphics::CENTER),
    mClickCount(0),
    mSpacing(),
    mTag(0),
    mMode(0),
    mXOffset(0),
    mYOffset(0),
    mImageWidth(0),
    mImageHeight(0),
    mHasMouse(false),
    mKeyPressed(false),
    mMousePressed(false),
    mStick(false),
    mPressed(false),
    mTextChanged(true)
{
    init();
    adjustSize();
}

Button::Button(const Widget2 *const widget,
               const std::string &restrict caption,
               const std::string &restrict actionEventId,
               const std::string &restrict skinName,
               ActionListener *const listener) :
    Widget(widget),
    MouseListener(),
    KeyListener(),
    FocusListener(),
    WidgetListener(),
    mSkin(),
    mCaption(caption),
    mDescription(),
    mSkinName(skinName),
    mTextChunk(),
    mVertexes2(new ImageCollection),
    mEnabledColor(getThemeColor(ThemeColorId::BUTTON, 255U)),
    mEnabledColor2(getThemeColor(ThemeColorId::BUTTON_OUTLINE, 255U)),
    mDisabledColor(getThemeColor(ThemeColorId::BUTTON_DISABLED, 255U)),
    mDisabledColor2(getThemeColor(ThemeColorId::BUTTON_DISABLED_OUTLINE,
        255U)),
    mHighlightedColor(getThemeColor(ThemeColorId::BUTTON_HIGHLIGHTED, 255U)),
    mHighlightedColor2(getThemeColor(
        ThemeColorId::BUTTON_HIGHLIGHTED_OUTLINE, 255U)),
    mPressedColor(getThemeColor(ThemeColorId::BUTTON_PRESSED, 255U)),
    mPressedColor2(getThemeColor(ThemeColorId::BUTTON_PRESSED_OUTLINE, 255U)),
    mImages(nullptr),
    mImageSet(nullptr),
    mAlignment(Graphics::CENTER),
    mClickCount(0),
    mSpacing(),
    mTag(0),
    mMode(0),
    mXOffset(0),
    mYOffset(0),
    mImageWidth(0),
    mImageHeight(0),
    mHasMouse(false),
    mKeyPressed(false),
    mMousePressed(false),
    mStick(false),
    mPressed(false),
    mTextChanged(true)
{
    init();
    adjustSize();
    setActionEventId(actionEventId);

    if (listener != nullptr)
        addActionListener(listener);
}

Button::Button(const Widget2 *const widget,
               const std::string &restrict caption,
               const std::string &restrict imageName,
               const int imageWidth,
               const int imageHeight,
               const std::string &restrict actionEventId,
               const std::string &restrict skinName,
               ActionListener *const listener) :
    Widget(widget),
    MouseListener(),
    KeyListener(),
    FocusListener(),
    WidgetListener(),
    mSkin(),
    mCaption(caption),
    mDescription(),
    mSkinName(skinName),
    mTextChunk(),
    mVertexes2(new ImageCollection),
    mEnabledColor(getThemeColor(ThemeColorId::BUTTON, 255U)),
    mEnabledColor2(getThemeColor(ThemeColorId::BUTTON_OUTLINE, 255U)),
    mDisabledColor(getThemeColor(ThemeColorId::BUTTON_DISABLED, 255U)),
    mDisabledColor2(getThemeColor(ThemeColorId::BUTTON_DISABLED_OUTLINE,
        255U)),
    mHighlightedColor(getThemeColor(ThemeColorId::BUTTON_HIGHLIGHTED, 255U)),
    mHighlightedColor2(getThemeColor(
        ThemeColorId::BUTTON_HIGHLIGHTED_OUTLINE, 255U)),
    mPressedColor(getThemeColor(ThemeColorId::BUTTON_PRESSED, 255U)),
    mPressedColor2(getThemeColor(ThemeColorId::BUTTON_PRESSED_OUTLINE, 255U)),
    mImages(nullptr),
    mImageSet(nullptr),
    mAlignment(Graphics::CENTER),
    mClickCount(0),
    mSpacing(),
    mTag(0),
    mMode(0),
    mXOffset(0),
    mYOffset(0),
    mImageWidth(imageWidth),
    mImageHeight(imageHeight),
    mHasMouse(false),
    mKeyPressed(false),
    mMousePressed(false),
    mStick(false),
    mPressed(false),
    mTextChanged(true)
{
    init();
    loadImageSet(imageName);
    adjustSize();
    setActionEventId(actionEventId);

    if (listener != nullptr)
        addActionListener(listener);
}

Button::Button(const Widget2 *const widget,
               const std::string &restrict imageName,
               const int imageWidth,
               const int imageHeight,
               const std::string &restrict actionEventId,
               const std::string &restrict skinName,
               ActionListener *const listener) :
    Widget(widget),
    MouseListener(),
    KeyListener(),
    FocusListener(),
    WidgetListener(),
    mSkin(),
    mCaption(),
    mDescription(),
    mSkinName(skinName),
    mTextChunk(),
    mVertexes2(new ImageCollection),
    mEnabledColor(getThemeColor(ThemeColorId::BUTTON, 255U)),
    mEnabledColor2(getThemeColor(ThemeColorId::BUTTON_OUTLINE, 255U)),
    mDisabledColor(getThemeColor(ThemeColorId::BUTTON_DISABLED, 255U)),
    mDisabledColor2(getThemeColor(ThemeColorId::BUTTON_DISABLED_OUTLINE,
        255U)),
    mHighlightedColor(getThemeColor(ThemeColorId::BUTTON_HIGHLIGHTED, 255U)),
    mHighlightedColor2(getThemeColor(
        ThemeColorId::BUTTON_HIGHLIGHTED_OUTLINE, 255U)),
    mPressedColor(getThemeColor(ThemeColorId::BUTTON_PRESSED, 255U)),
    mPressedColor2(getThemeColor(ThemeColorId::BUTTON_PRESSED_OUTLINE, 255U)),
    mImages(nullptr),
    mImageSet(nullptr),
    mAlignment(Graphics::CENTER),
    mClickCount(0),
    mSpacing(),
    mTag(0),
    mMode(0),
    mXOffset(0),
    mYOffset(0),
    mImageWidth(imageWidth),
    mImageHeight(imageHeight),
    mHasMouse(false),
    mKeyPressed(false),
    mMousePressed(false),
    mStick(false),
    mPressed(false),
    mTextChanged(true)
{
    init();
    loadImageSet(imageName);
    adjustSize();
    setActionEventId(actionEventId);

    if (listener != nullptr)
        addActionListener(listener);
}

void Button::init()
{
    mAllowLogic = false;
    addMouseListener(this);
    addKeyListener(this);
    addFocusListener(this);
    addWidgetListener(this);

    setFocusable(true);
    setFrameSize(0);

    if (theme != nullptr)
    {
        for (int mode = 0; mode < BUTTON_COUNT; mode ++)
        {
            Skin *const skin = theme->load(mSkinName + data[mode],
                "button.xml",
                true,
                Theme::getThemePath());
            if (skin != nullptr)
            {
                mSkin[mode] = skin;
                mSpacing[mode] = skin->getOption("spacing");
            }
        }
    }

    updateAlpha();
}

Button::~Button()
{
    if (mWindow != nullptr)
        mWindow->removeWidgetListener(this);

    if (gui != nullptr)
        gui->removeDragged(this);

    if (theme != nullptr)
    {
        for (int mode = 0; mode < BUTTON_COUNT; mode ++)
            theme->unload(mSkin[mode]);
    }
    delete2(mVertexes2);
    if (mImageSet != nullptr)
    {
        mImageSet->decRef();
        mImageSet = nullptr;
    }
    if (mImages != nullptr)
    {
        for (int f = 0; f < BUTTON_COUNT; f ++)
            mImages[f] = nullptr;
        delete [] mImages;
        mImages = nullptr;
    }
    mTextChunk.deleteImage();
}

void Button::loadImage(const std::string &imageName)
{
    if (mImageSet != nullptr)
    {
        mImageSet->decRef();
        mImageSet = nullptr;
    }
    Image *const image = Theme::getImageFromThemeXml(imageName, "");
    mImages = new Image*[BUTTON_COUNT];
    for (int f = 0; f < BUTTON_COUNT; f ++)
        mImages[f] = image;
}

void Button::setImage(Image *const image)
{
    if (image == nullptr)
        return;
    if (mImageSet != nullptr)
    {
        mImageSet->decRef();
        mImageSet = nullptr;
    }
    mImageWidth = image->getWidth();
    mImageHeight = image->getHeight();
    mImages = new Image*[BUTTON_COUNT];
    for (int f = 0; f < BUTTON_COUNT; f ++)
        mImages[f] = image;
}

void Button::loadImageSet(const std::string &imageName)
{
    if (mImageSet != nullptr)
    {
        mImageSet->decRef();
        mImageSet = nullptr;
    }
    mImageSet = Theme::getImageSetFromTheme(imageName,
        mImageWidth, mImageHeight);
    if (mImageSet == nullptr)
        return;
    mImages = new Image*[BUTTON_COUNT];
    mImages[0] = nullptr;
    for (size_t f = 0; f < BUTTON_COUNT; f ++)
    {
        Image *const img = mImageSet->get(f);
        if (img != nullptr)
            mImages[f] = img;
        else
            mImages[f] = mImages[0];
    }
}

void Button::updateAlpha()
{
    const float alpha = std::max(settings.guiAlpha,
        theme->getMinimumOpacity());

    if (mAlpha != alpha)
    {
        mAlpha = alpha;
        for (int mode = 0; mode < BUTTON_COUNT; mode ++)
        {
            for (int a = 0; a < 9; a ++)
            {
                Skin *const skin = mSkin[mode];
                if (skin != nullptr)
                {
                    const ImageRect &rect = skin->getBorder();
                    Image *const image = rect.grid[a];
                    if (image != nullptr)
                        image->setAlpha(mAlpha);
                }
            }
        }
    }
}

void Button::draw(Graphics *const graphics)
{
    BLOCK_START("Button::draw")
    int mode;

    if (!isEnabled())
        mode = BUTTON_DISABLED;
    else if (isPressed2())
        mode = BUTTON_PRESSED;
    else if (mHasMouse)
        mode = BUTTON_HIGHLIGHTED;
    else
        mode = BUTTON_STANDARD;

    const Skin *const skin = mSkin[mode];
    if (skin == nullptr)
    {
        BLOCK_END("Button::draw")
        return;
    }

    updateAlpha();

    bool recalc = false;
    if (mRedraw)
    {
        recalc = true;
    }
    else
    {
        // because we don't know where parent windows was moved,
        // need recalc vertexes
        ClipRect &rect = graphics->getTopClip();
        if (rect.xOffset != mXOffset || rect.yOffset != mYOffset)
        {
            recalc = true;
            mXOffset = rect.xOffset;
            mYOffset = rect.yOffset;
        }
        else if (mMode != mode)
        {
            recalc = true;
            mMode = mode;
        }
        else if (graphics->getRedraw())
        {
            recalc = true;
        }
    }

    const int padding = skin->getPadding();
    const int spacing = mSpacing[mode];

    if (recalc)
        mTextChanged = true;

    int imageX = 0;
    int imageY = 0;
    int textX = 0;
    const Rect &rect = mDimension;
    const int width = rect.width;
    const int height = rect.height;
    Font *const font = getFont();
    int textY = height / 2 - font->getHeight() / 2;
    if (mImages != nullptr)
        imageY = height / 2 - mImageHeight / 2;

// need move calculation from draw!!!

    switch (mAlignment)
    {
        default:
        case Graphics::LEFT:
        {
            if (mImages != nullptr)
            {
                imageX = padding;
                textX = padding + mImageWidth + spacing;
            }
            else
            {
                textX = padding;
            }
            break;
        }
        case Graphics::CENTER:
        {
            const int width1 = font->getWidth(mCaption);
            if (mImages != nullptr)
            {
                const int w = width1 + mImageWidth + spacing;
                imageX = (width - w) / 2;
                textX = imageX + mImageWidth + spacing - width1 / 2;
            }
            else
            {
                textX = (width - width1) / 2;
            }
            break;
        }
        case Graphics::RIGHT:
        {
            const int width1 = font->getWidth(mCaption);
            textX = width - width1 - padding;
            imageX = textX - width1 - spacing;
            break;
        }
    }

    if (recalc)
    {
        mRedraw = false;
        mMode = mode;
        mVertexes2->clear();
        graphics->calcWindow(mVertexes2,
            0, 0,
            width, height,
            skin->getBorder());

        if (mImages != nullptr)
        {
            if (isPressed())
            {
                graphics->calcTileCollection(mVertexes2,
                    mImages[mode],
                    imageX + 1, imageY + 1);
            }
            else
            {
                graphics->calcTileCollection(mVertexes2,
                    mImages[mode],
                    imageX, imageY);
            }
        }
        graphics->finalize(mVertexes2);
    }
    graphics->drawTileCollection(mVertexes2);

    if (isPressed())
    {
        textX ++;
        textY ++;
    }

    if (mTextChanged)
    {
        mTextChunk.textFont = font;
        mTextChunk.deleteImage();
        mTextChunk.text = mCaption;
        switch (mode)
        {
            case BUTTON_DISABLED:
                mTextChunk.color = mDisabledColor;
                mTextChunk.color2 = mDisabledColor2;
                break;
            case BUTTON_PRESSED:
                mTextChunk.color = mPressedColor;
                mTextChunk.color2 = mPressedColor2;
                break;
            case BUTTON_HIGHLIGHTED:
                mTextChunk.color = mHighlightedColor;
                mTextChunk.color2 = mHighlightedColor2;
                break;
            default:
                mTextChunk.color = mEnabledColor;
                mTextChunk.color2 = mEnabledColor2;
                break;
        }
        font->generate(mTextChunk);
        mTextChanged = false;
    }

    const Image *const image = mTextChunk.img;
    if (image != nullptr)
        graphics->drawImage(image, textX, textY);

    BLOCK_END("Button::draw")
}

void Button::safeDraw(Graphics *const graphics)
{
    BLOCK_START("Button::safeDraw")
    int mode;

    if (!isEnabled())
        mode = BUTTON_DISABLED;
    else if (isPressed2())
        mode = BUTTON_PRESSED;
    else if (mHasMouse || isFocused())
        mode = BUTTON_HIGHLIGHTED;
    else
        mode = BUTTON_STANDARD;

    const Skin *const skin = mSkin[mode];
    if (skin == nullptr)
    {
        BLOCK_END("Button::safeDraw")
        return;
    }

    updateAlpha();

    if (mMode != mode)
    {
        mTextChanged = true;
        mMode = mode;
    }

    const int padding = skin->getPadding();
    const int spacing = mSpacing[mode];

    int imageX = 0;
    int imageY = 0;
    int textX = 0;
    const Rect &rect = mDimension;
    const int width = rect.width;
    const int height = rect.height;
    Font *const font = getFont();
    int textY = height / 2 - font->getHeight() / 2;
    if (mImages != nullptr)
        imageY = height / 2 - mImageHeight / 2;

// need move calculation from draw!!!

    switch (mAlignment)
    {
        default:
        case Graphics::LEFT:
        {
            if (mImages != nullptr)
            {
                imageX = padding;
                textX = padding + mImageWidth + spacing;
            }
            else
            {
                textX = padding;
            }
            break;
        }
        case Graphics::CENTER:
        {
            const int width1 = font->getWidth(mCaption);
            if (mImages != nullptr)
            {
                const int w = width1 + mImageWidth + spacing;
                imageX = (width - w) / 2;
                textX = imageX + mImageWidth + spacing - width1 / 2;
            }
            else
            {
                textX = (width - width1) / 2;
            }
            break;
        }
        case Graphics::RIGHT:
        {
            const int width1 = font->getWidth(mCaption);
            textX = width - width1 - padding;
            imageX = textX - width1 - spacing;
            break;
        }
    }

    graphics->drawImageRect(0, 0, width, height, skin->getBorder());

    if (mImages != nullptr)
    {
        if (isPressed())
            graphics->drawImage(mImages[mode], imageX + 1, imageY + 1);
        else
            graphics->drawImage(mImages[mode], imageX, imageY);
    }

    if (isPressed())
    {
        textX ++;
        textY ++;
    }

    if (mTextChanged)
    {
        mTextChunk.textFont = font;
        mTextChunk.deleteImage();
        mTextChunk.text = mCaption;
        switch (mode)
        {
            case BUTTON_DISABLED:
                mTextChunk.color = mDisabledColor;
                mTextChunk.color2 = mDisabledColor2;
                break;
            case BUTTON_PRESSED:
                mTextChunk.color = mPressedColor;
                mTextChunk.color2 = mPressedColor2;
                break;
            case BUTTON_HIGHLIGHTED:
                mTextChunk.color = mHighlightedColor;
                mTextChunk.color2 = mHighlightedColor2;
                break;
            default:
                mTextChunk.color = mEnabledColor;
                mTextChunk.color2 = mEnabledColor2;
                break;
        }
        font->generate(mTextChunk);
        mTextChanged = false;
    }

    const Image *const image = mTextChunk.img;
    if (image != nullptr)
        graphics->drawImage(image, textX, textY);

    BLOCK_END("Button::safeDraw")
}

void Button::mouseReleased(MouseEvent& event)
{
    if (event.getButton() == MouseButton::LEFT)
    {
        if (mStick)
            mPressed = !mPressed;

        if (mMousePressed && mHasMouse)
        {
            mMousePressed = false;
            mClickCount = event.getClickCount();
            distributeActionEvent();
        }
        else
        {
            mMousePressed = false;
            mClickCount = 0;
        }
        event.consume();
    }
}

void Button::widgetResized(const Event &event A_UNUSED)
{
    mRedraw = true;
}

void Button::widgetMoved(const Event &event A_UNUSED)
{
    mRedraw = true;
}

void Button::widgetHidden(const Event &event A_UNUSED)
{
    if (isBatchDrawRenders(openGLMode))
        mVertexes2->clear();
    mTextChunk.deleteImage();
    mRedraw = true;
    mTextChanged = true;
}

void Button::adjustSize()
{
    const Font *const font = getFont();
    const Skin *const skin = mSkin[BUTTON_STANDARD];
    if (skin == nullptr)
        return;
    const int padding = skin->getPadding();

    if (mImages != nullptr)
    {
        const int spacing = mSpacing[BUTTON_STANDARD];
        const int width = font->getWidth(mCaption);
        if (width != 0)
            setWidth(width + mImageWidth + spacing + 2 * padding);
        else
            setWidth(mImageWidth + 2 * padding);
        int height = font->getHeight();
        if (height < mImageHeight)
            height = mImageHeight;
        setHeight(height + 2 * padding);
    }
    else
    {
        setWidth(font->getWidth(mCaption) + 2 * padding);
        setHeight(font->getHeight() + 2 * padding);
    }
}

void Button::keyPressed(KeyEvent& event)
{
    const InputActionT action = event.getActionId();

    if (action == InputAction::GUI_SELECT)
    {
        mKeyPressed = true;
        event.consume();
    }
}

void Button::keyReleased(KeyEvent& event)
{
    const InputActionT action = event.getActionId();

    if (action == InputAction::GUI_SELECT && mKeyPressed)
    {
        mKeyPressed = false;
        if (mStick)
            mPressed = !mPressed;
        distributeActionEvent();
        event.consume();
    }
}

bool Button::isPressed2() const
{
    return mPressed || isPressed();
}

bool Button::isPressed() const
{
    if (mMousePressed)
        return mHasMouse;
    return mKeyPressed;
}

void Button::focusLost(const Event& event A_UNUSED)
{
    mMousePressed = false;
    mKeyPressed = false;
}

void Button::mousePressed(MouseEvent& event)
{
    if (event.getButton() == MouseButton::LEFT)
    {
        mMousePressed = true;
        event.consume();
    }
}

void Button::mouseEntered(MouseEvent& event A_UNUSED)
{
    mHasMouse = true;
}

void Button::mouseExited(MouseEvent& event A_UNUSED)
{
    mHasMouse = false;
}

void Button::mouseDragged(MouseEvent& event)
{
    event.consume();
}

void Button::setParent(Widget *widget)
{
    if (mWindow != nullptr)
        mWindow->addWidgetListener(this);
    Widget::setParent(widget);
}

void Button::setWindow(Widget *const widget)
{
    if ((widget == nullptr) && (mWindow != nullptr))
    {
        mWindow->removeWidgetListener(this);
        mWindow = nullptr;
    }
    else
    {
        Widget2::setWindow(widget);
    }
}
