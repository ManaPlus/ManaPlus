/*
 *  Gui Skinning
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2009  The Mana World Development Team
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

#include "gui/theme.h"

#include "client.h"
#include "configuration.h"
#include "graphicsmanager.h"

#include "resources/dye.h"
#include "resources/image.h"
#include "resources/resourcemanager.h"

#include "utils/dtor.h"
#include "utils/physfstools.h"

#include <algorithm>

#include "debug.h"

static std::string defaultThemePath;

std::string Theme::mThemePath;
std::string Theme::mThemeName;
std::string Theme::mScreenDensity;

Theme *Theme::mInstance = nullptr;

// Set the theme path...
static void initDefaultThemePath()
{
    const ResourceManager *const resman = ResourceManager::getInstance();
    defaultThemePath = branding.getStringValue("guiThemePath");

    logger->log("defaultThemePath: " + defaultThemePath);
    if (!defaultThemePath.empty() && resman->isDirectory(defaultThemePath))
        return;
    else
        defaultThemePath = "themes/";
}

Skin::Skin(ImageRect *restrict skin, const ImageRect *restrict images,
           const std::string &filePath, const std::string &name,
           const int padding, const int titlePadding,
           std::map<std::string, int> *restrict const options):
    instances(1),
    mFilePath(filePath),
    mName(name),
    mBorder(skin),
    mCloseImage(images->grid[0]),
    mCloseImageHighlighted(images->grid[1]),
    mStickyImageUp(images->grid[2]),
    mStickyImageDown(images->grid[3]),
    mPadding(padding),
    mTitlePadding(titlePadding),
    mOptions(options)
{
    if (!mCloseImageHighlighted)
    {
        mCloseImageHighlighted = mCloseImage;
        if (mCloseImageHighlighted)
            mCloseImageHighlighted->incRef();
    }
}

Skin::~Skin()
{
    for (int i = 0; i < 9; i++)
    {
        if (mBorder && mBorder->grid[i])
        {
            mBorder->grid[i]->decRef();
            mBorder->grid[i] = nullptr;
        }
    }

    if (mCloseImage)
    {
        mCloseImage->decRef();
        mCloseImage = nullptr;
    }

    if (mCloseImageHighlighted)
    {
        mCloseImageHighlighted->decRef();
        mCloseImageHighlighted = nullptr;
    }

    if (mStickyImageUp)
    {
        mStickyImageUp->decRef();
        mStickyImageUp = nullptr;
    }

    if (mStickyImageDown)
    {
        mStickyImageDown->decRef();
        mStickyImageDown = nullptr;
    }

    delete mOptions;
    mOptions = nullptr;
    delete mBorder;
    mBorder = nullptr;
}

void Skin::updateAlpha(const float minimumOpacityAllowed)
{
    const float alpha = static_cast<float>(
        std::max(static_cast<double>(minimumOpacityAllowed),
        static_cast<double>(client->getGuiAlpha())));

    if (mBorder)
    {
        for (int i = 0; i < 9; i++)
        {
            if (mBorder->grid[i])
                mBorder->grid[i]->setAlpha(alpha);
        }
    }

    if (mCloseImage)
        mCloseImage->setAlpha(alpha);
    if (mCloseImageHighlighted)
        mCloseImageHighlighted->setAlpha(alpha);
    if (mStickyImageUp)
        mStickyImageUp->setAlpha(alpha);
    if (mStickyImageDown)
        mStickyImageDown->setAlpha(alpha);
}

int Skin::getMinWidth() const
{
    if (!mBorder || !mBorder->grid[ImageRect::UPPER_LEFT]
        || !mBorder->grid[ImageRect::UPPER_RIGHT])
    {
        return 1;
    }

    return mBorder->grid[ImageRect::UPPER_LEFT]->getWidth() +
           mBorder->grid[ImageRect::UPPER_RIGHT]->getWidth();
}

int Skin::getMinHeight() const
{
    if (!mBorder || !mBorder->grid[ImageRect::UPPER_LEFT]
        || !mBorder->grid[ImageRect::LOWER_LEFT])
    {
        return 1;
    }

    return mBorder->grid[ImageRect::UPPER_LEFT]->getHeight() +
           mBorder->grid[ImageRect::LOWER_LEFT]->getHeight();
}

Theme::Theme() :
    Palette(THEME_COLORS_END * THEME_PALETTES),
    mSkins(),
    mMinimumOpacity(-1.0F),
    mProgressColors(ProgressColors(THEME_PROG_END))
{
    initDefaultThemePath();

    config.addListener("guialpha", this);

    mColors[HIGHLIGHT].ch = 'H';
    mColors[CHAT].ch = 'C';
    mColors[GM].ch = 'G';
    mColors[GLOBAL].ch = 'g';
    mColors[PLAYER].ch = 'Y';
    mColors[WHISPER_TAB].ch = 'W';
    mColors[WHISPER_TAB_OFFLINE].ch = 'w';
    mColors[IS].ch = 'I';
    mColors[PARTY_CHAT_TAB].ch = 'P';
    mColors[GUILD_CHAT_TAB].ch = 'U';
    mColors[SERVER].ch = 'S';
    mColors[LOGGER].ch = 'L';
    mColors[HYPERLINK].ch = '<';
    mColors[SELFNICK].ch = 's';
    mColors[OLDCHAT].ch = 'o';
    mColors[AWAYCHAT].ch = 'a';
    mCharColors['H'] = HIGHLIGHT;
    mCharColors['C'] = CHAT;
    mCharColors['G'] = GM;
    mCharColors['g'] = GLOBAL;
    mCharColors['Y'] = PLAYER;
    mCharColors['W'] = WHISPER_TAB;
    mCharColors['w'] = WHISPER_TAB_OFFLINE;
    mCharColors['I'] = IS;
    mCharColors['P'] = PARTY_CHAT_TAB;
    mCharColors['U'] = GUILD_CHAT_TAB;
    mCharColors['S'] = SERVER;
    mCharColors['L'] = LOGGER;
    mCharColors['<'] = HYPERLINK;
    mCharColors['s'] = SELFNICK;
    mCharColors['o'] = OLDCHAT;
    mCharColors['a'] = AWAYCHAT;

    // here need use outlined colors
    mCharColors['H' | 0x80] = HIGHLIGHT_OUTLINE;
    mCharColors['C' | 0x80] = CHAT_OUTLINE;
    mCharColors['G' | 0x80] = GM_OUTLINE;
    mCharColors['g' | 0x80] = GLOBAL_OUTLINE;
    mCharColors['Y' | 0x80] = PLAYER_OUTLINE;
    mCharColors['W' | 0x80] = WHISPER_TAB_OUTLINE;
    mCharColors['w' | 0x80] = WHISPER_TAB_OFFLINE_OUTLINE;
    mCharColors['I' | 0x80] = IS_OUTLINE;
    mCharColors['P' | 0x80] = PARTY_CHAT_TAB_OUTLINE;
    mCharColors['U' | 0x80] = GUILD_CHAT_TAB_OUTLINE;
    mCharColors['S' | 0x80] = SERVER_OUTLINE;
    mCharColors['L' | 0x80] = LOGGER_OUTLINE;
    mCharColors['<' | 0x80] = HYPERLINK_OUTLINE;
    mCharColors['s' | 0x80] = SELFNICK_OUTLINE;
    mCharColors['o' | 0x80] = OLDCHAT_OUTLINE;
    mCharColors['a' | 0x80] = AWAYCHAT_OUTLINE;
}

Theme::~Theme()
{
    delete_all(mSkins);
    config.removeListener("guialpha", this);
    CHECKLISTENERS
    delete_all(mProgressColors);
}

Theme *Theme::instance()
{
    if (!mInstance)
        mInstance = new Theme;

    return mInstance;
}

void Theme::deleteInstance()
{
    delete mInstance;
    mInstance = nullptr;
}

gcn::Color Theme::getProgressColor(const int type, const float progress)
{
    int color[3] = {0, 0, 0};

    if (mInstance)
    {
        const DyePalette *const dye = mInstance->mProgressColors[type];

        if (dye)
            dye->getColor(progress, color);
        else
            logger->log("color not found: " + toString(type));
    }

    return gcn::Color(color[0], color[1], color[2]);
}

Skin *Theme::load(const std::string &filename, const std::string &filename2,
                  const bool full, const std::string &restrict defaultPath)
{
    // Check if this skin was already loaded

    const SkinIterator skinIterator = mSkins.find(filename);
    if (mSkins.end() != skinIterator)
    {
        if (skinIterator->second)
            skinIterator->second->instances++;
        return skinIterator->second;
    }

    Skin *skin = nullptr;
    if (mScreenDensity.empty())
    {   // if no density detected
        skin = readSkin(filename, full);
        if (!skin && !filename2.empty() && filename2 != filename)
            skin = readSkin(filename2, full);
        if (!skin && filename2 != "window.xml")
            skin = readSkin("window.xml", full);
    }
    else
    {   // first use correct density images
        const std::string endStr("_" + mScreenDensity + ".xml");
        std::string name = filename;
        if (findCutLast(name, ".xml"))
            skin = readSkin(name + endStr, full);
        if (!skin)
            skin = readSkin(filename, full);
        if (!skin && !filename2.empty() && filename2 != filename)
        {
            name = filename2;
            if (findCutLast(name, ".xml"))
                skin = readSkin(name + endStr, full);
            if (!skin)
                skin = readSkin(filename2, full);
        }
        if (!skin && filename2 != "window.xml")
        {
            skin = readSkin("window" + endStr, full);
            if (!skin)
                skin = readSkin("window.xml", full);
        }
    }

    if (!skin)
    {
        // Try falling back on the defaultPath if this makes sense
        if (filename != defaultPath)
        {
            logger->log("Error loading skin '%s', falling back on default.",
                        filename.c_str());

            skin = readSkin(defaultPath, full);
        }

        if (!skin)
        {
            logger->log(strprintf("Error: Loading default skin '%s' failed. "
                                  "Make sure the skin file is valid.",
                                  defaultPath.c_str()));
        }
    }

    mSkins[filename] = skin;
    return skin;
}

void Theme::unload(Skin *const skin)
{
    if (!skin)
        return;
    skin->instances --;
    if (!skin->instances)
    {
        SkinIterator it = mSkins.begin();
        const SkinIterator it_end = mSkins.end();
        while (it != it_end)
        {
            if (it->second == skin)
            {
                mSkins.erase(it);
                break;
            }
            ++ it;
        }
        delete skin;
    }
}

void Theme::setMinimumOpacity(const float minimumOpacity)
{
    if (minimumOpacity > 1.0F)
        return;

    mMinimumOpacity = minimumOpacity;
    updateAlpha();
}

void Theme::updateAlpha()
{
    FOR_EACH (SkinIterator, iter, mSkins)
    {
        if (iter->second)
            iter->second->updateAlpha(mMinimumOpacity);
    }
}

void Theme::optionChanged(const std::string &)
{
    updateAlpha();
}

struct SkinParameter final
{
    int index;
    std::string name;
};

static const SkinParameter skinParam[] =
{
    {0, "top-left-corner"},
    {0, "standart"},
    {0, "up"},
    {0, "hstart"},
    {0, "in"},
    {0, "normal"},
    {1, "top-edge"},
    {1, "highlighted"},
    {1, "down"},
    {1, "hmiddle"},
    {1, "in-highlighted"},
    {1, "checked"},
    {2, "top-right-corner"},
    {2, "pressed"},
    {2, "left"},
    {2, "hend"},
    {2, "out"},
    {2, "disabled"},
    {3, "left-edge"},
    {3, "disabled"},
    {3, "right"},
    {3, "hgrip"},
    {3, "out-highlighted"},
    {3, "disabled-checked"},
    {4, "bg-quad"},
    {4, "vstart"},
    {4, "normal-highlighted"},
    {5, "right-edge"},
    {5, "vmiddle"},
    {5, "checked-highlighted"},
    {6, "bottom-left-corner"},
    {6, "vend"},
    {7, "bottom-edge"},
    {7, "vgrip"},
    {8, "bottom-right-corner"},
};

static const SkinParameter imageParam[] =
{
    {0, "closeImage"},
    {1, "closeImageHighlighted"},
    {2, "stickyImageUp"},
    {3, "stickyImageDown"},
};

struct SkinHelper final
{
    SkinHelper() :
        partType(),
        xPos(),
        yPos(),
        width(),
        height(),
        rect(),
        node(),
        image(),
        resman()
    {
    }

    A_DELETE_COPY(SkinHelper)

    std::string partType;
    int xPos;
    int yPos;
    int width;
    int height;
    ImageRect *rect;
    XmlNodePtr *node;
    Image *image;
    ResourceManager *resman;

    bool loadList(const SkinParameter *const params, const size_t size)
    {
        for (unsigned f = 0; f < size; f ++)
        {
            if (partType == params[f].name)
            {
                rect->grid[params[f].index] = resman->getSubImage(
                    image, xPos, yPos, width, height);
                return true;
            }
        }
        return false;
    }
};

Skin *Theme::readSkin(const std::string &filename, const bool full)
{
    if (filename.empty())
        return nullptr;

//    logger->log("Loading skin '%s'.", filename.c_str());

    XML::Document doc(resolveThemePath(filename));
    const XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "skinset"))
        return nullptr;

    const std::string skinSetImage = XML::getProperty(rootNode, "image", "");

    if (skinSetImage.empty())
    {
        logger->log1("Theme::readSkin(): Skinset does not define an image!");
        return nullptr;
    }

    Image *const dBorders = Theme::getImageFromTheme(skinSetImage);
    ImageRect *const border = new ImageRect;
    ImageRect *const images = new ImageRect;
    memset(border, 0, sizeof(ImageRect));
    memset(images, 0, sizeof(ImageRect));
    int padding = 3;
    int titlePadding = 4;
    int titlebarHeight = 0;
    int titlebarHeightRelative = 0;
    int closePadding = 3;
    int stickySpacing = 3;
    int stickyPadding = 3;
    int resizePadding = 2;
    std::map<std::string, int> *const mOptions
        = new std::map<std::string, int>();

    // iterate <widget>'s
    for_each_xml_child_node(widgetNode, rootNode)
    {
        if (!xmlNameEqual(widgetNode, "widget"))
            continue;

        const std::string widgetType =
                XML::getProperty(widgetNode, "type", "unknown");
        if (widgetType == "Window")
        {
            SkinHelper helper;
            helper.resman = ResourceManager::getInstance();
            const int globalXPos = XML::getProperty(widgetNode, "xpos", 0);
            const int globalYPos = XML::getProperty(widgetNode, "ypos", 0);
            for_each_xml_child_node(partNode, widgetNode)
            {
                if (xmlNameEqual(partNode, "part"))
                {
                    helper.partType = XML::getProperty(
                        partNode, "type", "unknown");
                    helper.xPos = XML::getProperty(
                        partNode, "xpos", 0) + globalXPos;
                    helper.yPos = XML::getProperty(
                        partNode, "ypos", 0) + globalYPos;
                    helper.width = XML::getProperty(partNode, "width", 1);
                    helper.height = XML::getProperty(partNode, "height", 1);
                    helper.image = dBorders;

                    helper.rect = border;
                    if (!helper.loadList(skinParam,
                        sizeof(skinParam) / sizeof(SkinParameter)))
                    {
                        helper.rect = images;
                        helper.loadList(imageParam,
                            sizeof(imageParam) / sizeof(SkinParameter));
                    }
                }
                else if (full && xmlNameEqual(partNode, "option"))
                {
                    const std::string name = XML::getProperty(
                        partNode, "name", "");
                    if (name == "padding")
                    {
                        padding = XML::getProperty(partNode, "value", 3);
                    }
                    else if (name == "titlePadding")
                    {
                        titlePadding = XML::getProperty(partNode, "value", 4);
                    }
                    else if (name == "closePadding")
                    {
                        closePadding = XML::getProperty(partNode, "value", 3);
                    }
                    else if (name == "stickySpacing")
                    {
                        stickySpacing = XML::getProperty(partNode, "value", 3);
                    }
                    else if (name == "stickyPadding")
                    {
                        stickyPadding = XML::getProperty(partNode, "value", 3);
                    }
                    else if (name == "titlebarHeight")
                    {
                        titlebarHeight = XML::getProperty(
                            partNode, "value", 0);
                    }
                    else if (name == "titlebarHeightRelative")
                    {
                        titlebarHeightRelative = XML::getProperty(
                            partNode, "value", 0);
                    }
                    else if (name == "resizePadding")
                    {
                        resizePadding = XML::getProperty(
                            partNode, "value", 2);
                    }
                    else
                    {
                        (*mOptions)[name] = XML::getProperty(
                            partNode, "value", 0);
                    }
                }
            }
        }
        else
        {
            logger->log("Theme::readSkin(): Unknown widget type '%s'",
                        widgetType.c_str());
        }
    }

    if (dBorders)
        dBorders->decRef();

    (*mOptions)["closePadding"] = closePadding;
    (*mOptions)["stickyPadding"] = stickyPadding;
    (*mOptions)["stickySpacing"] = stickySpacing;
    (*mOptions)["titlebarHeight"] = titlebarHeight;
    (*mOptions)["titlebarHeightRelative"] = titlebarHeightRelative;
    (*mOptions)["resizePadding"] = resizePadding;

    Skin *const skin = new Skin(border, images, filename, "", padding,
        titlePadding, mOptions);
    delete images;
    skin->updateAlpha(mMinimumOpacity);
    return skin;
}

bool Theme::tryThemePath(const std::string &themeName)
{
    if (!themeName.empty())
    {
        const std::string path = defaultThemePath + themeName;
        if (PhysFs::exists(path.c_str()))
        {
            mThemePath = path;
            mThemeName = themeName;
            if (instance())
                instance()->loadColors("");
            return true;
        }
    }

    return false;
}

void Theme::fillSkinsList(StringVect &list)
{
    char **skins = PhysFs::enumerateFiles(
        branding.getStringValue("guiThemePath").c_str());

    for (char **i = skins; *i; i++)
    {
        if (PhysFs::isDirectory((
            branding.getStringValue("guiThemePath") + *i).c_str()))
        {
            list.push_back(*i);
        }
    }

    PhysFs::freeList(skins);
}

void Theme::fillFontsList(StringVect &list)
{
    PHYSFS_permitSymbolicLinks(1);
    char **fonts = PhysFs::enumerateFiles(
        branding.getStringValue("fontsPath").c_str());

    for (char **i = fonts; *i; i++)
    {
        if (!PhysFs::isDirectory((
            branding.getStringValue("fontsPath") + *i).c_str()))
        {
            list.push_back(*i);
        }
    }

    PhysFs::freeList(fonts);
    PHYSFS_permitSymbolicLinks(0);
}

void Theme::fillSoundsList(StringVect &list)
{
    char **skins = PhysFs::enumerateFiles(
        branding.getStringValue("systemsounds").c_str());

    for (char **i = skins; *i; i++)
    {
        if (!PhysFs::isDirectory((
            branding.getStringValue("systemsounds") + *i).c_str()))
        {
            std::string str = *i;
            if (findCutLast(str, ".ogg"))
                list.push_back(str);
        }
    }

    PhysFs::freeList(skins);
}

void Theme::selectSkin()
{
    prepareThemePath();
    mScreenDensity = graphicsManager.getDensityString();
}

void Theme::prepareThemePath()
{
    initDefaultThemePath();

    mThemePath.clear();
    mThemeName.clear();

    // Try theme from settings
    if (tryThemePath(config.getStringValue("theme")))
        return;

    // Try theme from branding
    if (tryThemePath(branding.getStringValue("theme")))
        return;

    if (mThemePath.empty())
        mThemePath = "graphics/gui";

    instance()->loadColors(mThemePath);

    logger->log("Selected Theme: " + mThemePath);
}

std::string Theme::resolveThemePath(const std::string &path)
{
    // Need to strip off any dye info for the existence tests
    const int pos = static_cast<int>(path.find('|'));
    std::string file;
    if (pos > 0)
        file = path.substr(0, pos);
    else
        file = path;

    // File with path
    if (file.find('/') != std::string::npos)
    {
        // Might be a valid path already
        if (PhysFs::exists(file.c_str()))
            return path;
    }

    // Try the theme
    file = getThemePath().append("/").append(file);

    if (PhysFs::exists(file.c_str()))
        return getThemePath().append("/").append(path);

    // Backup
    return branding.getStringValue("guiPath").append(path);
}

Image *Theme::getImageFromTheme(const std::string &path)
{
    ResourceManager *const resman = ResourceManager::getInstance();
    return resman->getImage(resolveThemePath(path));
}

ImageSet *Theme::getImageSetFromTheme(const std::string &path,
                                      const int w, const int h)
{
    ResourceManager *const resman = ResourceManager::getInstance();
    return resman->getImageSet(resolveThemePath(path), w, h);
}

static int readColorType(const std::string &type)
{
    static const std::string colors[Theme::THEME_COLORS_END] =
    {
        "BROWSERBOX",
        "BROWSERBOX_OUTLINE",
        "SELFNICK",
        "SELFNICK_OUTLINE",
        "TEXT",
        "TEXT_OUTLINE",
        "CARET",
        "SHADOW",
        "OUTLINE",
        "BORDER",
        "PROGRESS_BAR",
        "PROGRESS_BAR_OUTLINE",
        "BUTTON",
        "BUTTON_OUTLINE",
        "BUTTON_DISABLED",
        "BUTTON_DISABLED_OUTLINE",
        "BUTTON_HIGHLIGHTED",
        "BUTTON_HIGHLIGHTED_OUTLINE",
        "BUTTON_PRESSED",
        "BUTTON_PRESSED_OUTLINE",
        "CHECKBOX",
        "CHECKBOX_OUTLINE",
        "DROPDOWN",
        "DROPDOWN_OUTLINE",
        "LABEL",
        "LABEL_OUTLINE",
        "LISTBOX",
        "LISTBOX_OUTLINE",
        "LISTBOX_SELECTED",
        "LISTBOX_SELECTED_OUTLINE",
        "RADIOBUTTON",
        "RADIOBUTTON_OUTLINE",
        "POPUP",
        "POPUP_OUTLINE",
        "TAB",
        "TAB_OUTLINE",
        "TAB_HIGHLIGHTED",
        "TAB_HIGHLIGHTED_OUTLINE",
        "TAB_SELECTED",
        "TAB_SELECTED_OUTLINE",
        "TEXTBOX",
        "TEXTFIELD",
        "TEXTFIELD_OUTLINE",
        "WINDOW",
        "WINDOW_OUTLINE",
        "PARTY_CHAT_TAB",
        "PARTY_CHAT_TAB_OUTLINE",
        "PARTY_SOCIAL_TAB",
        "PARTY_SOCIAL_TAB_OUTLINE",
        "GUILD_CHAT_TAB",
        "GUILD_CHAT_TAB_OUTLINE",
        "GUILD_SOCIAL_TAB",
        "GUILD_SOCIAL_TAB_OUTLINE",
        "GM_CHAT_TAB",
        "GM_CHAT_TAB_OUTLINE",
        "PARTY_CHAT_TAB_HIGHLIGHTED",
        "PARTY_CHAT_TAB_HIGHLIGHTED_OUTLINE",
        "PARTY_SOCIAL_TAB_HIGHLIGHTED",
        "PARTY_SOCIAL_TAB_HIGHLIGHTED_OUTLINE",
        "GUILD_CHAT_TAB_HIGHLIGHTED",
        "GUILD_CHAT_TAB_HIGHLIGHTED_OUTLINE",
        "GUILD_SOCIAL_TAB_HIGHLIGHTED",
        "GUILD_SOCIAL_TAB_HIGHLIGHTED_OUTLINE",
        "GM_CHAT_TAB_HIGHLIGHTED",
        "GM_CHAT_TAB_HIGHLIGHTED_OUTLINE",
        "PARTY_CHAT_TAB_SELECTED",
        "PARTY_CHAT_TAB_SELECTED_OUTLINE",
        "PARTY_SOCIAL_TAB_SELECTED",
        "PARTY_SOCIAL_TAB_SELECTED_OUTLINE",
        "GUILD_CHAT_TAB_SELECTED",
        "GUILD_CHAT_TAB_SELECTED_OUTLINE",
        "GUILD_SOCIAL_TAB_SELECTED",
        "GUILD_SOCIAL_TAB_SELECTED_OUTLINE",
        "GM_CHAT_TAB_SELECTED",
        "GM_CHAT_TAB_SELECTED_OUTLINE",
        "BACKGROUND",
        "BACKGROUND_GRAY",
        "SCROLLBAR_GRAY",
        "DROPDOWN_SHADOW",
        "HIGHLIGHT",
        "HIGHLIGHT_OUTLINE",
        "TAB_FLASH",
        "TAB_FLASH_OUTLINE",
        "TAB_PLAYER_FLASH",
        "TAB_PLAYER_FLASH_OUTLINE",
        "SHOP_WARNING",
        "ITEM_EQUIPPED",
        "ITEM_EQUIPPED_OUTLINE",
        "ITEM_NOT_EQUIPPED",
        "ITEM_NOT_EQUIPPED_OUTLINE",
        "CHAT",
        "CHAT_OUTLINE",
        "GM",
        "GM_OUTLINE",
        "GLOBAL",
        "GLOBAL_OUTLINE",
        "PLAYER",
        "PLAYER_OUTLINE",
        "WHISPER_TAB",
        "WHISPER_TAB_OUTLINE",
        "WHISPER_TAB_OFFLINE",
        "WHISPER_TAB_OFFLINE_OUTLINE",
        "WHISPER_TAB_HIGHLIGHTED",
        "WHISPER_TAB_HIGHLIGHTED_OUTLINE",
        "WHISPER_TAB_OFFLINE_HIGHLIGHTED",
        "WHISPER_TAB_OFFLINE_HIGHLIGHTED_OUTLINE",
        "WHISPER_TAB_SELECTED",
        "WHISPER_TAB_SELECTED_OUTLINE",
        "WHISPER_TAB_OFFLINE_SELECTED",
        "WHISPER_TAB_OFFLINE_SELECTED_OUTLINE",
        "IS",
        "IS_OUTLINE",
        "SERVER",
        "SERVER_OUTLINE",
        "LOGGER",
        "LOGGER_OUTLINE",
        "HYPERLINK",
        "HYPERLINK_OUTLINE",
        "UNKNOWN_ITEM",
        "UNKNOWN_ITEM_OUTLINE",
        "GENERIC",
        "GENERIC_OUTLINE",
        "HEAD",
        "HEAD_OUTLINE",
        "USABLE",
        "USABLE_OUTLINE",
        "TORSO",
        "TORSO_OUTLINE",
        "ONEHAND",
        "ONEHAND_OUTLINE",
        "LEGS",
        "LEGS_OUTLINE",
        "FEET",
        "FEET_OUTLINE",
        "TWOHAND",
        "TWOHAND_OUTLINE",
        "SHIELD",
        "SHIELD_OUTLINE",
        "RING",
        "RING_OUTLINE",
        "NECKLACE",
        "NECKLACE_OUTLINE",
        "ARMS",
        "ARMS_OUTLINE",
        "AMMO",
        "AMMO_OUTLINE",
        "SERVER_VERSION_NOT_SUPPORTED",
        "SERVER_VERSION_NOT_SUPPORTED_OUTLINE",
        "WARNING",
        "WARNING_OUTLINE",
        "CHARM",
        "CHARM_OUTLINE",
        "PLAYER_ADVANCED",
        "PLAYER_ADVANCED_OUTLINE",
        "BUBBLE_NAME",
        "BUBBLE_NAME_OUTLINE",
        "BUBBLE_TEXT",
        "BUBBLE_TEXT_OUTLINE",
        "BLACK",
        "BLACK_OUTLINE",
        "RED",
        "RED_OUTLINE",
        "GREEN",
        "GREEN_OUTLINE",
        "BLUE",
        "BLUE_OUTLINE",
        "ORANGE",
        "ORANGE_OUTLINE",
        "YELLOW",
        "YELLOW_OUTLINE",
        "PINK",
        "PINK_OUTLINE",
        "PURPLE",
        "PURPLE_OUTLINE",
        "GRAY",
        "GRAY_OUTLINE",
        "BROWN",
        "BROWN_OUTLINE",
        "STATUSBAR_ON",
        "STATUSBAR_OFF",
        "TABLE_BACKGROUND",
        "SLOTS_BAR",
        "SLOTS_BAR_OUTLINE",
        "HP_BAR",
        "HP_BAR_OUTLINE",
        "MP_BAR",
        "MP_BAR_OUTLINE",
        "NO_MP_BAR",
        "NO_MP_BAR_OUTLINE",
        "XP_BAR",
        "XP_BAR_OUTLINE",
        "WEIGHT_BAR",
        "WEIGHT_BAR_OUTLINE",
        "MONEY_BAR",
        "MONEY_BAR_OUTLINE",
        "ARROWS_BAR",
        "ARROWS_BAR_OUTLINE",
        "STATUS_BAR",
        "STATUS_BAR_OUTLINE",
        "JOB_BAR",
        "JOB_BAR_OUTLINE",
        "OLDCHAT",
        "OLDCHAT_OUTLINE",
        "AWAYCHAT",
        "AWAYCHAT_OUTLINE"
    };

    if (type.empty())
        return -1;

    for (int i = 0; i < Theme::THEME_COLORS_END; i++)
    {
        if (compareStrI(type, colors[i]) == 0)
            return i;
    }

    return -1;
}

static gcn::Color readColor(const std::string &description)
{
    const int size = static_cast<const int>(description.length());
    if (size < 7 || description[0] != '#')
    {
        logger->log("Error, invalid theme color palette: %s",
                    description.c_str());
        return Palette::BLACK;
    }

    int v = 0;
    for (int i = 1; i < 7; ++i)
    {
        signed const char c = description[i];
        int n;

        if ('0' <= c && c <= '9')
        {
            n = c - '0';
        }
        else if ('A' <= c && c <= 'F')
        {
            n = c - 'A' + 10;
        }
        else if ('a' <= c && c <= 'f')
        {
            n = c - 'a' + 10;
        }
        else
        {
            logger->log("Error, invalid theme color palette: %s",
                        description.c_str());
            return Palette::BLACK;
        }

        v = (v << 4) | n;
    }

    return gcn::Color(v);
}

static Palette::GradientType readColorGradient(const std::string &grad)
{
    static const std::string grads[] =
    {
        "STATIC",
        "PULSE",
        "SPECTRUM",
        "RAINBOW"
    };

    if (grad.empty())
        return Palette::STATIC;

    for (int i = 0; i < 4; i++)
    {
        if (compareStrI(grad, grads[i]))
            return static_cast<Palette::GradientType>(i);
    }

    return Palette::STATIC;
}

static int readProgressType(const std::string &type)
{
    static const std::string colors[Theme::THEME_PROG_END] =
    {
        "HP",
        "MP",
        "NO_MP",
        "EXP",
        "INVY_SLOTS",
        "WEIGHT",
        "JOB",
        "UPDATE",
        "MONEY",
        "ARROWS",
        "STATUS"
    };

    if (type.empty())
        return -1;

    for (int i = 0; i < Theme::THEME_PROG_END; i++)
    {
        if (compareStrI(type, colors[i]) == 0)
            return i;
    }

    return -1;
}

void Theme::loadColors(std::string file)
{
    if (file == "")
        file = "colors.xml";
    else
        file.append("/colors.xml");

    XML::Document doc(resolveThemePath(file));
    const XmlNodePtr root = doc.rootNode();

    if (!root || !xmlNameEqual(root, "colors"))
    {
        logger->log("Error loading colors file: %s", file.c_str());
        return;
    }

    logger->log("Loading colors file: %s", file.c_str());

    int type;
    std::string temp;
    gcn::Color color;
    GradientType grad;

    for_each_xml_child_node(paletteNode, root)
    {
        if (xmlNameEqual(paletteNode, "progressbar"))
        {
            type = readProgressType(XML::getProperty(paletteNode, "id", ""));
            if (type < 0)
                continue;

            mProgressColors[type] = new DyePalette(XML::getProperty(
                paletteNode, "color", ""), 6);
        }
        else if (!xmlNameEqual(paletteNode, "palette"))
        {
            continue;
        }

        const int paletteId = XML::getProperty(paletteNode, "id", 1);
        if (paletteId < 0 || paletteId >= THEME_PALETTES)
            continue;

        for_each_xml_child_node(node, paletteNode)
        {
            if (xmlNameEqual(node, "color"))
            {
                const std::string id = XML::getProperty(node, "id", "");
                type = readColorType(id);
                if (type < 0)
                    continue;

                temp = XML::getProperty(node, "color", "");
                if (temp.empty())
                    continue;

                color = readColor(temp);
                grad = readColorGradient(XML::getProperty(node, "effect", ""));
                mColors[paletteId * THEME_COLORS_END + type].set(
                    type, color, grad, 10);

                if (!findLast(id, "_OUTLINE"))
                {
                    const int type2 = readColorType(id + "_OUTLINE");
                    if (type2 < 0)
                        continue;
                    const int idx = paletteId * THEME_COLORS_END;
                    mColors[idx + type2] = mColors[idx + type];
                }
            }
        }
    }
}

void Theme::loadRect(ImageRect &image, const std::string &name,
                     const std::string &name2,
                     const int start, const int end)
{
    Skin *const skin = load(name, name2, false);
    if (skin)
    {
        const ImageRect &rect = skin->getBorder();
        for (int f = start; f <= end; f ++)
        {
            if (rect.grid[f])
            {
                image.grid[f] = rect.grid[f];
                image.grid[f]->incRef();
            }
        }
        unload(skin);
    }
}

Skin *Theme::loadSkinRect(ImageRect &image, const std::string &name,
                          const std::string &name2, const int start,
                          const int end)
{
    Skin *const skin = load(name, name2);
    if (skin)
    {
        const ImageRect &rect = skin->getBorder();
        for (int f = start; f <= end; f ++)
        {
            if (rect.grid[f])
            {
                image.grid[f] = rect.grid[f];
                image.grid[f]->incRef();
            }
        }
    }
    return skin;
}

void Theme::unloadRect(const ImageRect &rect, const int start,
                       const int end) const
{
    for (int f = start; f <= end; f ++)
    {
        if (rect.grid[f])
            rect.grid[f]->decRef();
    }
}

Image *Theme::getImageFromThemeXml(const std::string &name,
                                   const std::string &name2)
{
    Theme *const theme = Theme::instance();
    Skin *const skin = theme->load(name, name2, false);
    if (skin)
    {
        const ImageRect &rect = skin->getBorder();
        if (rect.grid[0])
        {
            Image *const image = rect.grid[0];
            image->incRef();
            theme->unload(skin);
            return image;
        }
        theme->unload(skin);
    }
    return nullptr;
}

ImageSet *Theme::getImageSetFromThemeXml(const std::string &name,
                                         const std::string &name2,
                                         const int w, const int h)
{
    Theme *const theme = Theme::instance();
    Skin *const skin = theme->load(name, name2, false);
    if (skin)
    {
        const ImageRect &rect = skin->getBorder();
        if (rect.grid[0])
        {
            Image *const image = rect.grid[0];
            const SDL_Rect &rect2 = image->mBounds;
            if (rect2.w && rect2.h)
            {
                ResourceManager *const resman = ResourceManager::getInstance();
                ImageSet *const imageSet = resman->getSubImageSet(image, w, h);
                theme->unload(skin);
                return imageSet;
            }
        }
        theme->unload(skin);
    }
    return nullptr;
}

#define readValue(name) \
        info->name = reinterpret_cast<const char*>(\
            xmlNodeGetContent(infoNode))

#define readIntValue(name) \
        info->name = atoi(reinterpret_cast<const char*>(\
            xmlNodeGetContent(infoNode)))

#define readFloatValue(name) \
        info->name = atof(reinterpret_cast<const char*>(\
            xmlNodeGetContent(infoNode)))

ThemeInfo *Theme::loadInfo(const std::string &themeName)
{
    std::string path;
    if (themeName.empty())
    {
        path = "graphics/gui/info.xml";
    }
    else
    {
        path = std::string(defaultThemePath).append(
            themeName).append("/info.xml");
    }
    logger->log("loading: " + path);
    XML::Document doc(path);
    const XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "info"))
        return nullptr;

    ThemeInfo *info = new ThemeInfo();

    const std::string fontSize2("fontSize_" + mScreenDensity);
    const std::string npcfontSize2("npcfontSize_" + mScreenDensity);
    for_each_xml_child_node(infoNode, rootNode)
    {
        if (xmlNameEqual(infoNode, "name"))
            readValue(name);
        else if (xmlNameEqual(infoNode, "copyright"))
            readValue(copyright);
        else if (xmlNameEqual(infoNode, "font"))
            readValue(font);
        else if (xmlNameEqual(infoNode, "boldFont"))
            readValue(boldFont);
        else if (xmlNameEqual(infoNode, "particleFont"))
            readValue(particleFont);
        else if (xmlNameEqual(infoNode, "helpFont"))
            readValue(helpFont);
        else if (xmlNameEqual(infoNode, "secureFont"))
            readValue(secureFont);
        else if (xmlNameEqual(infoNode, "japanFont"))
            readValue(japanFont);
        else if (xmlNameEqual(infoNode, "fontSize"))
            readIntValue(fontSize);
        else if (xmlNameEqual(infoNode, "npcfontSize"))
            readIntValue(npcfontSize);
        else if (xmlNameEqual(infoNode, "guialpha"))
            readFloatValue(guiAlpha);
        else if (xmlNameEqual(infoNode, fontSize2.c_str()))
            readIntValue(fontSize);
        else if (xmlNameEqual(infoNode, npcfontSize2.c_str()))
            readIntValue(npcfontSize);
    }
    return info;
}
