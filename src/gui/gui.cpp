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

#include "gui/gui.h"

#include "gui/focushandler.h"
#include "gui/palette.h"
#include "gui/sdlinput.h"
#include "gui/theme.h"
#include "gui/sdlfont.h"

#include "gui/widgets/mouseevent.h"
#include "gui/widgets/window.h"
#include "gui/widgets/windowcontainer.h"

#include "configlistener.h"
#include "configuration.h"
#include "graphics.h"
#include "logger.h"

#include "resources/image.h"
#include "resources/imageset.h"
#include "resources/imageloader.h"
#include "resources/resourcemanager.h"

#include <guichan/exception.hpp>
#include <guichan/image.hpp>

#include "debug.h"

// Guichan stuff
Gui *gui = nullptr;
SDLInput *guiInput = nullptr;

// Bolded font
gcn::Font *boldFont = nullptr;

class GuiConfigListener : public ConfigListener
{
    public:
        GuiConfigListener(Gui *g):
            mGui(g)
        {}

        void optionChanged(const std::string &name)
        {
            if (name == "customcursor" && mGui)
            {
                bool bCustomCursor = config.getBoolValue("customcursor");
                mGui->setUseCustomCursor(bCustomCursor);
            }
        }
    private:
        Gui *mGui;
};

Gui::Gui(Graphics *graphics):
    mCustomCursor(false),
    mMouseCursors(nullptr),
    mMouseCursorAlpha(1.0f),
    mMouseInactivityTimer(0),
    mCursorType(CURSOR_POINTER)
{
    logger->log1("Initializing GUI...");
    // Set graphics
    setGraphics(graphics);

    // Set image loader
    static ImageLoader imageLoader;
    gcn::Image::setImageLoader(&imageLoader);

    // Set input
    guiInput = new SDLInput;
    setInput(guiInput);

    // Set focus handler
    delete mFocusHandler;
    mFocusHandler = new FocusHandler;

    // Initialize top GUI widget
    WindowContainer *guiTop = new WindowContainer;
    guiTop->setFocusable(true);
    guiTop->setSize(graphics->mWidth, graphics->mHeight);
    guiTop->setOpaque(false);
    Window::setWindowContainer(guiTop);
    setTop(guiTop);

    const std::vector<std::string> langs = getLang();
    const bool isJapan = (!langs.empty() && langs[0].size() > 3
        && langs[0].substr(0, 3) == "ja_");

    // Set global font
    const int fontSize = config.getIntValue("fontSize");
    std::string fontFile = config.getValue("font", "");
    if (isJapan)
    {
        fontFile = config.getValue("japanFont", "");
        if (fontFile.empty())
            fontFile = branding.getStringValue("japanFont");
    }

    if (fontFile.empty())
        fontFile = branding.getStringValue("font");

    try
    {
        mGuiFont = new SDLFont(fontFile, fontSize);
    }
    catch (const gcn::Exception &e)
    {
        logger->error(std::string("Unable to load '") + fontFile +
                      std::string("': ") + e.getMessage());
    }

    // Set particle font
    fontFile = config.getValue("particleFont", "");

    if (isJapan)
    {
        fontFile = config.getValue("japanFont", "");
        if (fontFile.empty())
            fontFile = branding.getStringValue("japanFont");
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
        logger->error(std::string("Unable to load '") + fontFile +
                      std::string("': ") + e.getMessage());
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
        logger->error(std::string("Unable to load '") + fontFile +
                      std::string("': ") + e.getMessage());
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
        logger->error(std::string("Unable to load '") + fontFile +
                      std::string("': ") + e.getMessage());
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
        logger->error(std::string("Unable to load '") + fontFile +
                      std::string("': ") + e.getMessage());
    }

    gcn::Widget::setGlobalFont(mGuiFont);

    // Initialize mouse cursor and listen for changes to the option
    setUseCustomCursor(config.getBoolValue("customcursor"));
    mConfigListener = new GuiConfigListener(this);
    config.addListener("customcursor", mConfigListener);
}

Gui::~Gui()
{
    config.removeListener("customcursor", mConfigListener);
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
    delete getTop();

    delete guiInput;
    guiInput = nullptr;

    Theme::deleteInstance();
}

void Gui::logic()
{
    ResourceManager *resman = ResourceManager::getInstance();
    resman->clearScheduled();

    // Fade out mouse cursor after extended inactivity
    if (mMouseInactivityTimer < 100 * 15)
    {
        ++mMouseInactivityTimer;
        mMouseCursorAlpha = std::min(1.0f, mMouseCursorAlpha + 0.05f);
    }
    else
    {
        mMouseCursorAlpha = std::max(0.0f, mMouseCursorAlpha - 0.005f);
    }

    Palette::advanceGradients();

    gcn::Gui::logic();
}

void Gui::draw()
{
    mGraphics->pushClipArea(getTop()->getDimension());
    getTop()->draw(mGraphics);

    int mouseX, mouseY;
    Uint8 button = SDL_GetMouseState(&mouseX, &mouseY);

    if ((SDL_GetAppState() & SDL_APPMOUSEFOCUS || button & SDL_BUTTON(1))
        && mMouseCursors && mCustomCursor && mMouseCursorAlpha > 0.0f)
    {
        Image *mouseCursor = mMouseCursors->get(mCursorType);
        if (mouseCursor)
        {
            mouseCursor->setAlpha(mMouseCursorAlpha);

            static_cast<Graphics*>(mGraphics)->drawImage(
                    mouseCursor,
                    mouseX - 15,
                    mouseY - 17);
        }
    }

    mGraphics->popClipArea();
}

void Gui::videoResized()
{
    WindowContainer *top = static_cast<WindowContainer*>(getTop());

    int oldWidth = top->getWidth();
    int oldHeight = top->getHeight();

    top->setSize(mainGraphics->mWidth, mainGraphics->mHeight);
    top->adjustAfterResize(oldWidth, oldHeight);
}

void Gui::setUseCustomCursor(bool customCursor)
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

void Gui::updateFonts()
{
    const int fontSize = config.getIntValue("fontSize");
    std::string fontFile = config.getValue("font", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("font");

    static_cast<SDLFont*>(mGuiFont)->loadFont(fontFile, fontSize);

    fontFile = config.getValue("particleFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("particleFont");

    static_cast<SDLFont*>(mInfoParticleFont)->loadFont(
        fontFile, fontSize, TTF_STYLE_BOLD);

    fontFile = config.getValue("boldFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("boldFont");

    static_cast<SDLFont*>(boldFont)->loadFont(fontFile, fontSize);

    fontFile = config.getValue("secureFont", "");
    if (fontFile.empty())
        fontFile = branding.getStringValue("secureFont");

    static_cast<SDLFont*>(mSecureFont)->loadFont(fontFile, fontSize);
}

void Gui::distributeMouseEvent(gcn::Widget* source, int type, int button,
                               int x, int y, bool force,
                               bool toSourceOnly)
{
    if (!source || !mFocusHandler)
        return;

    gcn::Widget* parent = source;
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

    while (parent)
    {
        // If the widget has been removed due to input
        // cancel the distribution.
        if (!gcn::Widget::widgetExists(widget))
            break;

        parent = static_cast<gcn::Widget*>(widget->getParent());

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
//                        throw GCN_EXCEPTION("Unknown mouse event type.");
                }
            }

            if (toSourceOnly)
                break;
        }

        gcn::Widget* swap = widget;
        widget = parent;
        parent = static_cast<gcn::Widget*>(swap->getParent());

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
