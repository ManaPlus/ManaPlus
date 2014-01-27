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

#include "mouseinput.h"

#include "gui/gui.h"

#include "gui/focushandler.h"
#include "gui/palette.h"
#include "gui/sdlfont.h"
#include "gui/sdlinput.h"
#include "gui/theme.h"
#include "gui/viewport.h"

#include "gui/widgets/mouseevent.h"
#include "gui/widgets/window.h"

#include "client.h"
#include "configuration.h"
#include "dragdrop.h"
#include "touchmanager.h"

#include "input/keydata.h"
#include "input/keyevent.h"
#include "input/keyinput.h"

#include "resources/cursor.h"
#include "resources/image.h"
#include "resources/imageset.h"
#include "resources/resourcemanager.h"

#include "utils/langs.h"
#include "utils/timer.h"

#include <guichan/exception.hpp>

#include "debug.h"

// Guichan stuff
Gui *gui = nullptr;
SDLInput *guiInput = nullptr;

// Bolded font
SDLFont *boldFont = nullptr;

class GuiConfigListener final : public ConfigListener
{
    public:
        explicit GuiConfigListener(Gui *const g):
            mGui(g)
        {}

        A_DELETE_COPY(GuiConfigListener)

        virtual ~GuiConfigListener()
        {
            CHECKLISTENERS
        }

        void optionChanged(const std::string &name)
        {
            if (!mGui)
                return;
            if (name == "customcursor")
                mGui->setUseCustomCursor(config.getBoolValue("customcursor"));
            else if (name == "doubleClick")
                mGui->setDoubleClick(config.getBoolValue("doubleClick"));
        }
    private:
        Gui *mGui;
};

Gui::Gui() :
    gcn::Gui(),
    mConfigListener(new GuiConfigListener(this)),
    mGuiFont(),
    mInfoParticleFont(),
    mHelpFont(),
    mSecureFont(),
    mNpcFont(),
    mMouseCursors(nullptr),
    mMouseCursorAlpha(1.0F),
    mMouseInactivityTimer(0),
    mCursorType(Cursor::CURSOR_POINTER),
#ifdef ANDROID
    mLastMouseRealX(0),
    mLastMouseRealY(0),
#endif
    mFocusListeners(),
    mForegroundColor(Theme::getThemeColor(Theme::TEXT)),
    mForegroundColor2(Theme::getThemeColor(Theme::TEXT_OUTLINE)),
    mTime(0),
    mCustomCursor(false),
    mDoubleClick(true)
{
}

void Gui::postInit(Graphics *const graphics)
{
    logger->log1("Initializing GUI...");
    // Set graphics
    setGraphics(graphics);

    // Set input
    guiInput = new SDLInput;
    setInput(guiInput);

    // Set focus handler
    delete mFocusHandler;
    mFocusHandler = new FocusHandler;

    // Initialize top GUI widget
    WindowContainer *const guiTop = new WindowContainer(nullptr);
    guiTop->setFocusable(true);
    guiTop->setSize(graphics->mWidth, graphics->mHeight);
    guiTop->setOpaque(false);
    Window::setWindowContainer(guiTop);
    setTop(guiTop);

    const StringVect langs = getLang();
    const bool isJapan = (!langs.empty() && langs[0].size() > 3
        && langs[0].substr(0, 3) == "ja_");
    const bool isChinese = (!langs.empty() && langs[0].size() > 3
        && langs[0].substr(0, 3) == "zh_");

    // Set global font
    const int fontSize = config.getIntValue("fontSize");
    std::string fontFile = config.getValue("font", "");
    if (isJapan)
    {
        fontFile = config.getValue("japanFont", "");
        if (fontFile.empty())
            fontFile = branding.getStringValue("japanFont");
    }
    else if (isChinese)
    {
        fontFile = config.getValue("chinaFont", "");
        if (fontFile.empty())
            fontFile = branding.getStringValue("chinaFont");
    }
    if (fontFile.empty())
        fontFile = branding.getStringValue("font");

    try
    {
        mGuiFont = new SDLFont(fontFile, fontSize);
    }
    catch (const gcn::Exception &e)
    {
        logger->error(std::string("Unable to load '").append(fontFile)
                      .append("': ").append(e.getMessage()));
    }


    // Set particle font
    fontFile = config.getValue("particleFont", "");
    if (isJapan)
    {
        fontFile = config.getValue("japanFont", "");
        if (fontFile.empty())
            fontFile = branding.getStringValue("japanFont");
    }
    else if (isChinese)
    {
        fontFile = config.getValue("chinaFont", "");
        if (fontFile.empty())
            fontFile = branding.getStringValue("chinaFont");
    }
    if (fontFile.empty())
        fontFile = branding.getStringValue("particleFont");

    try
    {
        mInfoParticleFont = new SDLFont(
            fontFile, fontSize, TTF_STYLE_BOLD);
    }
    catch (const gcn::Exception &e)
    {
        logger->error(std::string("Unable to load '").append(fontFile)
                      .append("': ").append(e.getMessage()));
    }


    // Set bold font
    fontFile = config.getValue("boldFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("boldFont");

    try
    {
        boldFont = new SDLFont(fontFile, fontSize);
    }
    catch (const gcn::Exception &e)
    {
        logger->error(std::string("Unable to load '").append(fontFile)
                      .append("': ").append(e.getMessage()));
    }


    // Set help font
    fontFile = config.getValue("helpFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("helpFont");

    try
    {
        mHelpFont = new SDLFont(fontFile, fontSize);
    }
    catch (const gcn::Exception &e)
    {
        logger->error(std::string("Unable to load '").append(fontFile)
                      .append("': ").append(e.getMessage()));
    }


    // Set secure font
    fontFile = config.getValue("secureFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("secureFont");

    try
    {
        mSecureFont = new SDLFont(fontFile, fontSize);
    }
    catch (const gcn::Exception &e)
    {
        logger->error(std::string("Unable to load '").append(fontFile)
                      .append("': ").append(e.getMessage()));
    }


    // Set npc font
    const int npcFontSize = config.getIntValue("npcfontSize");
    fontFile = config.getValue("npcFont", "");
    if (isJapan)
    {
        fontFile = config.getValue("japanFont", "");
        if (fontFile.empty())
            fontFile = branding.getStringValue("japanFont");
    }
    else if (isChinese)
    {
        fontFile = config.getValue("chinaFont", "");
        if (fontFile.empty())
            fontFile = branding.getStringValue("chinaFont");
    }
    if (fontFile.empty())
        fontFile = branding.getStringValue("npcFont");

    try
    {
        mNpcFont = new SDLFont(fontFile, npcFontSize);
    }
    catch (const gcn::Exception &e)
    {
        logger->error(std::string("Unable to load '").append(fontFile)
                      .append("': ").append(e.getMessage()));
    }

    gcn::Widget::setGlobalFont(mGuiFont);

    // Initialize mouse cursor and listen for changes to the option
    setUseCustomCursor(config.getBoolValue("customcursor"));
    setDoubleClick(config.getBoolValue("doubleClick"));
    config.addListener("customcursor", mConfigListener);
    config.addListener("doubleClick", mConfigListener);
}

Gui::~Gui()
{
    config.removeListeners(mConfigListener);
    delete mConfigListener;
    mConfigListener = nullptr;

    if (mMouseCursors)
    {
        mMouseCursors->decRef();
        mMouseCursors = nullptr;
    }

    delete mGuiFont;
    mGuiFont = nullptr;
    delete boldFont;
    boldFont = nullptr;
    delete mHelpFont;
    mHelpFont = nullptr;
    delete mSecureFont;
    mSecureFont = nullptr;
    delete mInfoParticleFont;
    mInfoParticleFont = nullptr;
    delete mNpcFont;
    mNpcFont = nullptr;
    delete getTop();

    delete guiInput;
    guiInput = nullptr;

    Theme::deleteInstance();
}

void Gui::logic()
{
    BLOCK_START("Gui::logic")
    ResourceManager *const resman = ResourceManager::getInstance();
    resman->clearScheduled();

    if (!mTop)
    {
        BLOCK_END("Gui::logic")
        return;
    }

    handleModalFocus();
    handleModalMouseInputFocus();

    if (guiInput)
        handleMouseInput();

    mTop->logic();
    BLOCK_END("Gui::logic")
}

void Gui::slowLogic()
{
    BLOCK_START("Gui::slowLogic")
    Palette::advanceGradients();

    // Fade out mouse cursor after extended inactivity
    if (mMouseInactivityTimer < 100 * 15)
    {
        ++mMouseInactivityTimer;
        mMouseCursorAlpha = std::min(1.0F, mMouseCursorAlpha + 0.05F);
    }
    else
    {
        mMouseCursorAlpha = std::max(0.0F, mMouseCursorAlpha - 0.005F);
    }
    if (mGuiFont)
        mGuiFont->slowLogic(0);
    if (mInfoParticleFont)
        mInfoParticleFont->slowLogic(1);
    if (mHelpFont)
        mHelpFont->slowLogic(2);
    if (mSecureFont)
        mSecureFont->slowLogic(3);
    if (boldFont)
        boldFont->slowLogic(4);
    if (mNpcFont)
        mNpcFont->slowLogic(5);
    if (windowContainer)
        windowContainer->slowLogic();

    const int time = cur_time;
    if (mTime != time)
    {
        logger->flush();
        mTime = time;
    }

    BLOCK_END("Gui::slowLogic")
}

void Gui::clearFonts()
{
    if (mGuiFont)
        mGuiFont->clear();
    if (mInfoParticleFont)
        mInfoParticleFont->clear();
    if (mHelpFont)
        mHelpFont->clear();
    if (mSecureFont)
        mSecureFont->clear();
    if (boldFont)
        boldFont->clear();
    if (mNpcFont)
        mNpcFont->clear();
}

bool Gui::handleInput()
{
    if (mInput)
        return handleKeyInput2();
    else
        return false;
}

bool Gui::handleKeyInput2()
{
    if (!guiInput)
        return false;

    BLOCK_START("Gui::handleKeyInput2")
    bool consumed(false);

    while (!mInput->isKeyQueueEmpty())
    {
        const KeyInput keyInput = guiInput->dequeueKeyInput2();

        // Save modifiers state
        mShiftPressed = keyInput.isShiftPressed();
        mMetaPressed = keyInput.isMetaPressed();
        mControlPressed = keyInput.isControlPressed();
        mAltPressed = keyInput.isAltPressed();

        KeyEvent keyEventToGlobalKeyListeners(nullptr,
            mShiftPressed, mControlPressed, mAltPressed, mMetaPressed,
            keyInput.getType(), keyInput.isNumericPad(),
            keyInput.getActionId(), keyInput.getKey());

#ifdef USE_SDL2
        if (!keyInput.getText().empty())
            keyEventToGlobalKeyListeners.setText(keyInput.getText());
#endif

        distributeKeyEventToGlobalKeyListeners(
            keyEventToGlobalKeyListeners);

        // If a global key listener consumes the event it will not be
        // sent further to the source of the event.
        if (keyEventToGlobalKeyListeners.isConsumed())
        {
            consumed = true;
            continue;
        }

        if (mFocusHandler)
        {
            bool keyEventConsumed = false;

            // Send key inputs to the focused widgets
            if (mFocusHandler->getFocused())
            {
                KeyEvent keyEvent(getKeyEventSource(),
                    mShiftPressed, mControlPressed, mAltPressed, mMetaPressed,
                    keyInput.getType(), keyInput.isNumericPad(),
                    keyInput.getActionId(), keyInput.getKey());
#ifdef USE_SDL2
                if (!keyInput.getText().empty())
                    keyEvent.setText(keyInput.getText());
#endif

                if (!mFocusHandler->getFocused()->isFocusable())
                    mFocusHandler->focusNone();
                else
                    distributeKeyEvent(keyEvent);

                keyEventConsumed = keyEvent.isConsumed();
                if (keyEventConsumed)
                    consumed = true;
            }

            // If the key event hasn't been consumed and
            // tabbing is enable check for tab press and
            // change focus.
            if (!keyEventConsumed && mTabbing && keyInput.getActionId()
                == static_cast<int>(Input::KEY_GUI_TAB)
                && keyInput.getType() == gcn::KeyInput::PRESSED)
            {
                if (keyInput.isShiftPressed())
                    mFocusHandler->tabPrevious();
                else
                    mFocusHandler->tabNext();
            }
        }
    }  // end while
    BLOCK_END("Gui::handleKeyInput2")
    return consumed;
}

void Gui::draw()
{
    BLOCK_START("Gui::draw 1")
    mGraphics->pushClipArea(getTop()->getDimension());
    getTop()->draw(mGraphics);
    touchManager.draw();

    int mouseX, mouseY;
    const uint8_t button = getMouseState(&mouseX, &mouseY);

    if ((client->getMouseFocused() || button & SDL_BUTTON(1))
        && mMouseCursors && mCustomCursor && mMouseCursorAlpha > 0.0F)
    {
        Graphics *g2 = static_cast<Graphics*>(mGraphics);
        const Image *const image = dragDrop.getItemImage();
        if (image)
        {
            const int posX = mouseX - (image->mBounds.w / 2);
            const int posY = mouseY - (image->mBounds.h / 2);
            g2->drawImage2(image, posX, posY);
        }
        if (mGuiFont)
        {
            const std::string &str = dragDrop.getText();
            if (!str.empty())
            {
                const int posX = mouseX - mGuiFont->getWidth(str) / 2;
                const int posY = mouseY + (image ? image->mBounds.h / 2 : 0);
                g2->setColorAll(mForegroundColor, mForegroundColor2);
                mGuiFont->drawString(g2, str, posX, posY);
            }
        }

        Image *const mouseCursor = mMouseCursors->get(mCursorType);
        if (mouseCursor)
        {
            mouseCursor->setAlpha(mMouseCursorAlpha);
            g2->drawImage2(mouseCursor, mouseX - 15, mouseY - 17);
        }
    }

    mGraphics->popClipArea();
    BLOCK_END("Gui::draw 1")
}

void Gui::videoResized() const
{
    WindowContainer *const top = static_cast<WindowContainer* const>(getTop());

    if (top)
    {
        const int oldWidth = top->getWidth();
        const int oldHeight = top->getHeight();

        top->setSize(mainGraphics->mWidth, mainGraphics->mHeight);
        top->adjustAfterResize(oldWidth, oldHeight);
    }
}

void Gui::setUseCustomCursor(const bool customCursor)
{
    if (customCursor != mCustomCursor)
    {
        mCustomCursor = customCursor;

        if (mCustomCursor)
        {
            // Hide the SDL mouse cursor
            SDL_ShowCursor(SDL_DISABLE);

            // Load the mouse cursor
            if (mMouseCursors)
                mMouseCursors->decRef();
            mMouseCursors = Theme::getImageSetFromTheme("mouse.png", 40, 40);

            if (!mMouseCursors)
                logger->log("Error: Unable to load mouse cursors.");
        }
        else
        {
            // Show the SDL mouse cursor
            SDL_ShowCursor(SDL_ENABLE);

            // Unload the mouse cursor
            if (mMouseCursors)
            {
                mMouseCursors->decRef();
                mMouseCursors = nullptr;
            }
        }
    }
}

void Gui::handleMouseMoved(const gcn::MouseInput &mouseInput)
{
    gcn::Gui::handleMouseMoved(mouseInput);
    mMouseInactivityTimer = 0;
}

void Gui::handleMousePressed(const gcn::MouseInput &mouseInput)
{
    const int x = mouseInput.getX();
    const int y = mouseInput.getY();
    const unsigned int button = mouseInput.getButton();
    const int timeStamp = mouseInput.getTimeStamp();

    gcn::Widget *sourceWidget = getMouseEventSource(x, y);

    if (mFocusHandler->getDraggedWidget())
        sourceWidget = mFocusHandler->getDraggedWidget();

    int sourceWidgetX, sourceWidgetY;
    sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);

    if ((mFocusHandler->getModalFocused()
        && sourceWidget->isModalFocused())
        || !mFocusHandler->getModalFocused())
    {
        sourceWidget->requestFocus();
    }

    if (mDoubleClick && timeStamp - mLastMousePressTimeStamp < 250
        && mLastMousePressButton == button)
    {
        mClickCount ++;
    }
    else
    {
        mClickCount = 1;
    }

    distributeMouseEvent(sourceWidget, MouseEvent::PRESSED, button, x, y);
    mFocusHandler->setLastWidgetPressed(sourceWidget);
    mFocusHandler->setDraggedWidget(sourceWidget);
    mLastMouseDragButton = button;
    mLastMousePressButton = button;
    mLastMousePressTimeStamp = timeStamp;
}

void Gui::updateFonts()
{
    const int fontSize = config.getIntValue("fontSize");
    std::string fontFile = config.getValue("font", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("font");

    mGuiFont->loadFont(fontFile, fontSize);

    fontFile = config.getValue("particleFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("particleFont");

    mInfoParticleFont->loadFont(fontFile, fontSize, TTF_STYLE_BOLD);

    fontFile = config.getValue("boldFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("boldFont");

    boldFont->loadFont(fontFile, fontSize);

    const int npcFontSize = config.getIntValue("npcfontSize");

    fontFile = config.getValue("npcFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("npcFont");

    mNpcFont->loadFont(fontFile, npcFontSize);
}

void Gui::distributeMouseEvent(gcn::Widget* source, int type, int button,
                               int x, int y, bool force,
                               bool toSourceOnly)
{
    if (!source || !mFocusHandler)
        return;

    gcn::Widget* widget = source;

    if (!force && mFocusHandler->getModalFocused() != nullptr
        && !widget->isModalFocused())
    {
        return;
    }

    if (!force && mFocusHandler->getModalMouseInputFocused() != nullptr
        && !widget->isModalMouseInputFocused())
    {
        return;
    }

    MouseEvent mouseEvent(source, mShiftPressed, mControlPressed,
        mAltPressed, mMetaPressed, type, button,
        x, y, mClickCount);

    gcn::Widget* parent = source;
    while (parent)
    {
        // If the widget has been removed due to input
        // cancel the distribution.
        if (!gcn::Widget::widgetExists(widget))
            break;

        parent = widget->getParent();

        if (widget->isEnabled() || force)
        {
            int widgetX, widgetY;
            widget->getAbsolutePosition(widgetX, widgetY);

            mouseEvent.setX(x - widgetX);
            mouseEvent.setY(y - widgetY);

            std::list<gcn::MouseListener*> mouseListeners
                = widget->_getMouseListeners();

            // Send the event to all mouse listeners of the widget.
            for (std::list<gcn::MouseListener*>::const_iterator
                 it = mouseListeners.begin();
                 it != mouseListeners.end(); ++ it)
            {
                switch (mouseEvent.getType())
                {
                    case gcn::MouseEvent::ENTERED:
                        (*it)->mouseEntered(mouseEvent);
                        break;
                    case gcn::MouseEvent::EXITED:
                        (*it)->mouseExited(mouseEvent);
                        break;
                    case gcn::MouseEvent::MOVED:
                        (*it)->mouseMoved(mouseEvent);
                        break;
                    case gcn::MouseEvent::PRESSED:
                        (*it)->mousePressed(mouseEvent);
                        break;
                    case gcn::MouseEvent::RELEASED:
                    case 100:  // manual hack for release on target after drag
                        (*it)->mouseReleased(mouseEvent);
                        break;
                    case gcn::MouseEvent::WHEEL_MOVED_UP:
                        (*it)->mouseWheelMovedUp(mouseEvent);
                        break;
                    case gcn::MouseEvent::WHEEL_MOVED_DOWN:
                        (*it)->mouseWheelMovedDown(mouseEvent);
                        break;
                    case gcn::MouseEvent::DRAGGED:
                        (*it)->mouseDragged(mouseEvent);
                        break;
                    case gcn::MouseEvent::CLICKED:
                        (*it)->mouseClicked(mouseEvent);
                        break;
                    default:
                        break;
                }
            }

            if (toSourceOnly)
                break;
        }

        const gcn::Widget *const swap = widget;
        widget = parent;
        parent = swap->getParent();

        if (type == gcn::MouseEvent::RELEASED)
            dragDrop.clear();

        // If a non modal focused widget has been reach
        // and we have modal focus cancel the distribution.
        if (mFocusHandler->getModalFocused()
            && !widget->isModalFocused())
        {
            break;
        }

        // If a non modal mouse input focused widget has been reach
        // and we have modal mouse input focus cancel the distribution.
        if (mFocusHandler->getModalMouseInputFocused()
            && !widget->isModalMouseInputFocused())
        {
            break;
        }
    }
}

void Gui::resetClickCount()
{
    mClickCount = 1;
    mLastMousePressTimeStamp = 0;
}

MouseEvent *Gui::createMouseEvent(Window *const widget)
{
    if (!viewport || !widget)
        return nullptr;

    int x = 0;
    int y = 0;
    int mouseX = 0;
    int mouseY = 0;

    getAbsolutePosition(widget, x, y);
    getMouseState(&mouseX, &mouseY);

    return new MouseEvent(widget, mShiftPressed,
        mControlPressed, mAltPressed, mMetaPressed, 0, 0,
        mouseX - x, mouseY - y, mClickCount);
}

void Gui::getAbsolutePosition(gcn::Widget *restrict widget,
                              int &restrict x, int &restrict y)
{
    x = 0;
    y = 0;
    while (widget->getParent())
    {
        x += widget->getX();
        y += widget->getY();
        widget = widget->getParent();
    }
}

void Gui::handleMouseInput()
{
    BLOCK_START("Gui::handleMouseInput")
    while (!mInput->isMouseQueueEmpty())
    {
        const MouseInput mouseInput = guiInput->dequeueMouseInput2();

        if (touchManager.processEvent(mouseInput))
        {
#ifdef ANDROID
#ifndef USE_SDL2
            SDL_WarpMouse(mLastMouseX, mLastMouseY,
                mLastMouseRealX, mLastMouseRealY);
#endif
#endif
            mMouseInactivityTimer = 0;
            continue;
        }

        // Save the current mouse state. It will be needed if modal focus
        // changes or modal mouse input focus changes.
        mLastMouseX = mouseInput.getX();
        mLastMouseY = mouseInput.getY();
#ifdef ANDROID
        mLastMouseRealX = mouseInput.getRealX();
        mLastMouseRealY = mouseInput.getRealY();
#endif
        switch (mouseInput.getType())
        {
            case gcn::MouseInput::PRESSED:
                handleMousePressed(mouseInput);
                break;
            case gcn::MouseInput::RELEASED:
                handleMouseReleased(mouseInput);
                break;
            case gcn::MouseInput::MOVED:
                handleMouseMoved(mouseInput);
                break;
            case gcn::MouseInput::WHEEL_MOVED_DOWN:
                handleMouseWheelMovedDown(mouseInput);
                break;
            case gcn::MouseInput::WHEEL_MOVED_UP:
                handleMouseWheelMovedUp(mouseInput);
                break;
            default:
                throw GCN_EXCEPTION("Unknown mouse input type.");
                break;
        }
    }
    BLOCK_END("Gui::handleMouseInput")
}

void Gui::handleMouseReleased(const gcn::MouseInput &mouseInput)
{
    gcn::Widget *sourceWidget = getMouseEventSource(
        mouseInput.getX(), mouseInput.getY());

    int sourceWidgetX, sourceWidgetY;
    if (mFocusHandler->getDraggedWidget())
    {
        if (sourceWidget != mFocusHandler->getLastWidgetPressed())
            mFocusHandler->setLastWidgetPressed(nullptr);

        gcn::Widget *oldWidget = sourceWidget;
        sourceWidget = mFocusHandler->getDraggedWidget();
        if (oldWidget != sourceWidget)
        {
            oldWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);
            distributeMouseEvent(oldWidget,
                100,
                mouseInput.getButton(),
                mouseInput.getX(),
                mouseInput.getY());
        }
    }

    sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);
    distributeMouseEvent(sourceWidget,
                         MouseEvent::RELEASED,
                         mouseInput.getButton(),
                         mouseInput.getX(),
                         mouseInput.getY());

    if (mouseInput.getButton() == mLastMousePressButton
        && mFocusHandler->getLastWidgetPressed() == sourceWidget)
    {
        distributeMouseEvent(sourceWidget,
                             MouseEvent::CLICKED,
                             mouseInput.getButton(),
                             mouseInput.getX(),
                             mouseInput.getY());

        mFocusHandler->setLastWidgetPressed(nullptr);
    }
    else
    {
        mLastMousePressButton = 0;
        mClickCount = 0;
    }

    if (mFocusHandler->getDraggedWidget())
        mFocusHandler->setDraggedWidget(nullptr);
}

void Gui::addGlobalFocusListener(gcn::FocusListener* focusListener)
{
    mFocusListeners.push_back(focusListener);
}

void Gui::removeGlobalFocusListener(gcn::FocusListener* focusListener)
{
    mFocusListeners.remove(focusListener);
}

void Gui::distributeGlobalFocusGainedEvent(const gcn::Event &focusEvent)
{
    for (FocusListenerIterator iter = mFocusListeners.begin();
         iter != mFocusListeners.end();
         ++ iter)
    {
        (*iter)->focusGained(focusEvent);
    }
}

void Gui::removeDragged(gcn::Widget *widget)
{
    if (!mFocusHandler)
        return;

    if (mFocusHandler->getDraggedWidget() == widget)
        mFocusHandler->setDraggedWidget(nullptr);
}

uint32_t Gui::getMouseState(int *const x, int *const y) const
{
    const uint32_t res = SDL_GetMouseState(x, y);
    const int scale = mainGraphics->getScale();
    (*x) /= scale;
    (*y) /= scale;
    return res;
}
