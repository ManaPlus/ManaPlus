/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2009  The Mana World Development Team
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

#include "gui/theme.h"

#include "configuration.h"
#include "graphicsmanager.h"

#include "const/gui/theme.h"

#include "fs/virtfs/virtfs.h"
#include "fs/virtfs/virtfstools.h"
#include "fs/virtfs/virtlist.h"

#include "gui/skin.h"
#include "gui/themeinfo.h"

#include "resources/imagerect.h"

#include "resources/dye/dyepalette.h"

#include "resources/image/image.h"

#include "resources/loaders/imageloader.h"
#include "resources/loaders/imagesetloader.h"
#include "resources/loaders/subimageloader.h"
#include "resources/loaders/subimagesetloader.h"
#include "resources/loaders/xmlloader.h"

#include "utils/dtor.h"

#include "debug.h"

static std::string defaultThemePath;

std::string Theme::mThemePath;
std::string Theme::mThemeName;
std::string Theme::mScreenDensity;

Theme *theme = nullptr;

// Set the theme path...
static void initDefaultThemePath()
{
    defaultThemePath = branding.getStringValue("guiThemePath");

    logger->log("defaultThemePath: " + defaultThemePath);
    if (!defaultThemePath.empty() &&
        VirtFs::isDirectory(defaultThemePath))
    {
        return;
    }
    else
    {
        defaultThemePath = "themes/";
    }
}

Theme::Theme() :
    Palette(CAST_S32(ThemeColorId::THEME_COLORS_END) * THEME_PALETTES),
    mSkins(),
    mMinimumOpacity(-1.0F),
    mProgressColors(ProgressColors(CAST_SIZE(
                    ProgressColorId::THEME_PROG_END)))
{
    initDefaultThemePath();

    config.addListener("guialpha", this);

    mColors[CAST_SIZE(ThemeColorId::HIGHLIGHT)].ch = 'H';
    mColors[CAST_SIZE(ThemeColorId::CHAT)].ch = 'C';
    mColors[CAST_SIZE(ThemeColorId::GM)].ch = 'G';
    mColors[CAST_SIZE(ThemeColorId::GLOBAL)].ch = 'g';
    mColors[CAST_SIZE(ThemeColorId::PLAYER)].ch = 'Y';
    mColors[CAST_SIZE(ThemeColorId::WHISPER_TAB)].ch = 'W';
    mColors[CAST_SIZE(ThemeColorId::WHISPER_TAB_OFFLINE)].ch = 'w';
    mColors[CAST_SIZE(ThemeColorId::IS)].ch = 'I';
    mColors[CAST_SIZE(ThemeColorId::PARTY_CHAT_TAB)].ch = 'P';
    mColors[CAST_SIZE(ThemeColorId::GUILD_CHAT_TAB)].ch = 'U';
    mColors[CAST_SIZE(ThemeColorId::SERVER)].ch = 'S';
    mColors[CAST_SIZE(ThemeColorId::LOGGER)].ch = 'L';
    mColors[CAST_SIZE(ThemeColorId::HYPERLINK)].ch = '<';
    mColors[CAST_SIZE(ThemeColorId::SELFNICK)].ch = 's';
    mColors[CAST_SIZE(ThemeColorId::OLDCHAT)].ch = 'o';
    mColors[CAST_SIZE(ThemeColorId::AWAYCHAT)].ch = 'a';
    mCharColors['H'] = CAST_S32(ThemeColorId::HIGHLIGHT);
    mCharColors['C'] = CAST_S32(ThemeColorId::CHAT);
    mCharColors['G'] = CAST_S32(ThemeColorId::GM);
    mCharColors['g'] = CAST_S32(ThemeColorId::GLOBAL);
    mCharColors['Y'] = CAST_S32(ThemeColorId::PLAYER);
    mCharColors['W'] = CAST_S32(ThemeColorId::WHISPER_TAB);
    mCharColors['w'] = CAST_S32(ThemeColorId::WHISPER_TAB_OFFLINE);
    mCharColors['I'] = CAST_S32(ThemeColorId::IS);
    mCharColors['P'] = CAST_S32(ThemeColorId::PARTY_CHAT_TAB);
    mCharColors['U'] = CAST_S32(ThemeColorId::GUILD_CHAT_TAB);
    mCharColors['S'] = CAST_S32(ThemeColorId::SERVER);
    mCharColors['L'] = CAST_S32(ThemeColorId::LOGGER);
    mCharColors['<'] = CAST_S32(ThemeColorId::HYPERLINK);
    mCharColors['s'] = CAST_S32(ThemeColorId::SELFNICK);
    mCharColors['o'] = CAST_S32(ThemeColorId::OLDCHAT);
    mCharColors['a'] = CAST_S32(ThemeColorId::AWAYCHAT);

    // here need use outlined colors
    mCharColors['H' | 0x80]
        = CAST_S32(ThemeColorId::HIGHLIGHT_OUTLINE);
    mCharColors['C' | 0x80] = CAST_S32(ThemeColorId::CHAT_OUTLINE);
    mCharColors['G' | 0x80] = CAST_S32(ThemeColorId::GM_OUTLINE);
    mCharColors['g' | 0x80] = CAST_S32(ThemeColorId::GLOBAL_OUTLINE);
    mCharColors['Y' | 0x80] = CAST_S32(ThemeColorId::PLAYER_OUTLINE);
    mCharColors['W' | 0x80]
        = CAST_S32(ThemeColorId::WHISPER_TAB_OUTLINE);
    mCharColors['w' | 0x80]
        = CAST_S32(ThemeColorId::WHISPER_TAB_OFFLINE_OUTLINE);
    mCharColors['I' | 0x80] = CAST_S32(ThemeColorId::IS_OUTLINE);
    mCharColors['P' | 0x80]
        = CAST_S32(ThemeColorId::PARTY_CHAT_TAB_OUTLINE);
    mCharColors['U' | 0x80]
        = CAST_S32(ThemeColorId::GUILD_CHAT_TAB_OUTLINE);
    mCharColors['S' | 0x80] = CAST_S32(ThemeColorId::SERVER_OUTLINE);
    mCharColors['L' | 0x80] = CAST_S32(ThemeColorId::LOGGER_OUTLINE);
    mCharColors['<' | 0x80]
        = CAST_S32(ThemeColorId::HYPERLINK_OUTLINE);
    mCharColors['s' | 0x80] = CAST_S32(ThemeColorId::SELFNICK_OUTLINE);
    mCharColors['o' | 0x80] = CAST_S32(ThemeColorId::OLDCHAT_OUTLINE);
    mCharColors['a' | 0x80] = CAST_S32(ThemeColorId::AWAYCHAT_OUTLINE);
}

Theme::~Theme()
{
    delete_all(mSkins);
    config.removeListener("guialpha", this);
    CHECKLISTENERS
    delete_all(mProgressColors);
}

Color Theme::getProgressColor(const ProgressColorIdT type,
                              const float progress)
{
    int color[3] = {0, 0, 0};

    if (theme)
    {
        const DyePalette *const dye
            = theme->mProgressColors[CAST_SIZE(type)];

        if (dye)
        {
            dye->getColor(progress, color);
        }
        else
        {
            logger->log("color not found: "
                + toString(CAST_S32(type)));
        }
    }

    return Color(color[0], color[1], color[2]);
}

Skin *Theme::load(const std::string &filename,
                  const std::string &filename2,
                  const bool full,
                  const std::string &restrict defaultPath)
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
        Skin *const skin = iter->second;
        if (skin)
            skin->updateAlpha(mMinimumOpacity);
    }
}

void Theme::optionChanged(const std::string &)
{
    updateAlpha();
}

struct SkinParameter final
{
    A_DEFAULT_COPY(SkinParameter)
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
        image()
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

    bool loadList(const SkinParameter *const params,
                  const size_t size) A_NONNULL(2)
    {
        for (size_t f = 0; f < size; f ++)
        {
            const SkinParameter &param = params[f];
            if (partType == param.name)
            {
                rect->grid[param.index] = Loader::getSubImage(
                    image,
                    xPos, yPos,
                    width, height);
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

    const std::string path = resolveThemePath(filename);
    if (!VirtFs::exists(path))
        return nullptr;
    XML::Document *const doc = Loader::getXml(path,
        UseVirtFs_true,
        SkipError_true);
    if (!doc)
        return nullptr;
    XmlNodeConstPtr rootNode = doc->rootNode();
    if (!rootNode || !xmlNameEqual(rootNode, "skinset"))
    {
        doc->decRef();
        return nullptr;
    }

    const std::string skinSetImage = XML::getProperty(rootNode, "image", "");

    if (skinSetImage.empty())
    {
        logger->log1("Theme::readSkin(): Skinset does not define an image!");
        doc->decRef();
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
    StringIntMap *const mOptions = new StringIntMap;

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
                    helper.width = XML::getProperty(partNode, "width", 0);
                    helper.height = XML::getProperty(partNode, "height", 0);
                    if (!helper.width || !helper.height)
                        continue;
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
    doc->decRef();
    return skin;
}

bool Theme::tryThemePath(const std::string &themeName)
{
    if (!themeName.empty())
    {
        const std::string path = defaultThemePath + themeName;
        if (VirtFs::exists(path))
        {
            mThemePath = path;
            mThemeName = themeName;
            if (theme)
                theme->loadColors("");
            return true;
        }
    }

    return false;
}

void Theme::fillSkinsList(StringVect &list)
{
    VirtFs::getDirs(branding.getStringValue("guiThemePath"), list);
}

void Theme::fillFontsList(StringVect &list)
{
    VirtFs::permitLinks(true);
    VirtFs::getFiles(branding.getStringValue("fontsPath"), list);
    VirtFs::permitLinks(false);
}

void Theme::fillSoundsList(StringVect &list)
{
    VirtList *const skins = VirtFs::enumerateFiles(
        branding.getStringValue("systemsounds"));

    FOR_EACH (StringVectCIter, i, skins->names)
    {
        if (!VirtFs::isDirectory((branding.getStringValue(
            "systemsounds") + *i)))
        {
            std::string str = *i;
            if (findCutLast(str, ".ogg"))
                list.push_back(str);
        }
    }

    VirtFs::freeList(skins);
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

    theme->loadColors(mThemePath);

    logger->log("Selected Theme: " + mThemePath);
}

std::string Theme::resolveThemePath(const std::string &path)
{
    // Need to strip off any dye info for the existence tests
    const int pos = CAST_S32(path.find('|'));
    std::string file;
    if (pos > 0)
        file = path.substr(0, pos);
    else
        file = path;

    // File with path
    if (file.find('/') != std::string::npos)
    {
        // Might be a valid path already
        if (VirtFs::exists(file))
            return path;
    }

    // Try the theme
    file = pathJoin(getThemePath(), file);

    if (VirtFs::exists(file))
        return pathJoin(getThemePath(), path);

    // Backup
    return pathJoin(branding.getStringValue("guiPath"), path);
}

Image *Theme::getImageFromTheme(const std::string &path)
{
    return Loader::getImage(resolveThemePath(path));
}

ImageSet *Theme::getImageSetFromTheme(const std::string &path,
                                      const int w, const int h)
{
    return Loader::getImageSet(resolveThemePath(path), w, h);
}

static int readColorType(const std::string &type)
{
    static const std::string colors[CAST_SIZE(
        ThemeColorId::THEME_COLORS_END)] =
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
        "BATTLE_CHAT_TAB",
        "BATTLE_CHAT_TAB_OUTLINE",
        "CHANNEL_CHAT_TAB",
        "CHANNEL_CHAT_TAB_OUTLINE",
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
        "BATTLE_CHAT_TAB_HIGHLIGHTED",
        "BATTLE_CHAT_TAB_HIGHLIGHTED_OUTLINE",
        "CHANNEL_CHAT_TAB_HIGHLIGHTED",
        "CHANNEL_CHAT_TAB_HIGHLIGHTED_OUTLINE",
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
        "BATTLE_CHAT_TAB_SELECTED",
        "BATTLE_CHAT_TAB_SELECTED_OUTLINE",
        "CHANNEL_CHAT_TAB_SELECTED",
        "CHANNEL_CHAT_TAB_SELECTED_OUTLINE",
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
        "CARD",
        "CARD_OUTLINE",
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
        "AWAYCHAT_OUTLINE",
        "SKILL_COOLDOWN",
        "TEXT_DISABLED",
        "TEXT_DISABLED_OUTLINE"
    };

    if (type.empty())
        return -1;

    for (int i = 0; i < CAST_S32(ThemeColorId::THEME_COLORS_END); i++)
    {
        if (compareStrI(type, colors[i]) == 0)
            return i;
    }

    return -1;
}

static Color readColor(const std::string &description)
{
    const int size = static_cast<const int>(description.length());
    if (size < 7 || description[0] != '#')
    {
        logger->log("Error, invalid theme color palette: %s",
                    description.c_str());
        return Palette::BLACK;
    }

    unsigned int v = 0;
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

    return Color(v);
}

static GradientTypeT readColorGradient(const std::string &grad)
{
    static const std::string grads[] =
    {
        "STATIC",
        "PULSE",
        "SPECTRUM",
        "RAINBOW"
    };

    if (grad.empty())
        return GradientType::STATIC;

    for (int i = 0; i < 4; i++)
    {
        if (compareStrI(grad, grads[i]))
            return static_cast<GradientTypeT>(i);
    }

    return GradientType::STATIC;
}

static int readProgressType(const std::string &type)
{
    static const std::string colors[CAST_SIZE(
        ProgressColorId::THEME_PROG_END)] =
    {
        "HP",
        "HP_POISON",
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

    for (int i = 0; i < CAST_S32(ProgressColorId::THEME_PROG_END); i++)
    {
        if (compareStrI(type, colors[i]) == 0)
            return i;
    }

    return -1;
}

void Theme::loadColors(std::string file)
{
    if (file.empty())
        file = "colors.xml";
    else
        file = pathJoin(file, "colors.xml");

    XML::Document *const doc = Loader::getXml(resolveThemePath(file),
        UseVirtFs_true,
        SkipError_false);
    if (!doc)
        return;
    XmlNodeConstPtrConst root = doc->rootNode();

    if (!root || !xmlNameEqual(root, "colors"))
    {
        logger->log("Error loading colors file: %s", file.c_str());
        doc->decRef();
        return;
    }

    logger->log("Loading colors file: %s", file.c_str());

    for_each_xml_child_node(paletteNode, root)
    {
        if (xmlNameEqual(paletteNode, "progressbar"))
        {
            const int type = readProgressType(XML::getProperty(
                paletteNode, "id", ""));
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
                const int type = readColorType(id);
                if (type < 0)
                    continue;

                const std::string temp = XML::getProperty(node, "color", "");
                if (temp.empty())
                    continue;

                const Color color = readColor(temp);
                const GradientTypeT grad = readColorGradient(
                    XML::getProperty(node, "effect", ""));
                mColors[paletteId * CAST_SIZE(
                    ThemeColorId::THEME_COLORS_END) + type].set(
                    type, color, grad, 10);

                if (!findLast(id, "_OUTLINE"))
                {
                    const int type2 = readColorType(id + "_OUTLINE");
                    if (type2 < 0)
                        continue;
                    const int idx = paletteId
                        * CAST_S32(ThemeColorId::THEME_COLORS_END);
                    mColors[idx + type2] = mColors[idx + type];
                }
            }
        }
    }
    doc->decRef();
}

#define loadGrid() \
    { \
        const ImageRect &rect = skin->getBorder(); \
        for (int f = start; f <= end; f ++) \
        { \
            if (rect.grid[f]) \
            { \
                image.grid[f] = rect.grid[f]; \
                image.grid[f]->incRef(); \
            } \
        } \
    }

void Theme::loadRect(ImageRect &image,
                     const std::string &name,
                     const std::string &name2,
                     const int start,
                     const int end)
{
    Skin *const skin = load(name, name2, false);
    if (skin)
    {
        loadGrid();
        unload(skin);
    }
}

Skin *Theme::loadSkinRect(ImageRect &image,
                          const std::string &name,
                          const std::string &name2,
                          const int start,
                          const int end)
{
    Skin *const skin = load(name, name2);
    if (skin)
        loadGrid();
    return skin;
}

void Theme::unloadRect(const ImageRect &rect,
                       const int start,
                       const int end)
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
    if (!theme)
        return nullptr;

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
    if (!theme)
        return nullptr;

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
                ImageSet *const imageSet = Loader::getSubImageSet(
                    image, w, h);
                theme->unload(skin);
                return imageSet;
            }
        }
        theme->unload(skin);
    }
    return nullptr;
}

#define readValue(name) \
        { \
            tmpData = reinterpret_cast<XmlChar*>( \
                XmlNodeGetContent(infoNode)); \
            info->name = tmpData; \
            XmlFree(tmpData); \
        }

#define readIntValue(name) \
        { \
            tmpData = reinterpret_cast<XmlChar*>( \
                XmlNodeGetContent(infoNode)); \
            info->name = atoi(tmpData); \
            XmlFree(tmpData); \
        }

#define readFloatValue(name) \
        { \
            tmpData = reinterpret_cast<XmlChar*>( \
                XmlNodeGetContent(infoNode)); \
            info->name = static_cast<float>(atof(tmpData)); \
            XmlFree(tmpData); \
        }

ThemeInfo *Theme::loadInfo(const std::string &themeName)
{
    std::string path;
    if (themeName.empty())
    {
        path = "graphics/gui/info.xml";
    }
    else
    {
        path = pathJoin(defaultThemePath,
            themeName,
            "info.xml");
    }
    logger->log("loading: " + path);
    XML::Document *const doc = Loader::getXml(path,
        UseVirtFs_true,
        SkipError_false);
    if (!doc)
        return nullptr;
    XmlNodeConstPtrConst rootNode = doc->rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "info"))
    {
        doc->decRef();
        return nullptr;
    }

    ThemeInfo *const info = new ThemeInfo;

    const std::string fontSize2("fontSize_" + mScreenDensity);
    const std::string npcfontSize2("npcfontSize_" + mScreenDensity);
    XmlChar *tmpData = nullptr;
    for_each_xml_child_node(infoNode, rootNode)
    {
        if (xmlNameEqual(infoNode, "name"))
            readValue(name)
        else if (xmlNameEqual(infoNode, "copyright"))
            readValue(copyright)
        else if (xmlNameEqual(infoNode, "font"))
            readValue(font)
        else if (xmlNameEqual(infoNode, "boldFont"))
            readValue(boldFont)
        else if (xmlNameEqual(infoNode, "particleFont"))
            readValue(particleFont)
        else if (xmlNameEqual(infoNode, "helpFont"))
            readValue(helpFont)
        else if (xmlNameEqual(infoNode, "secureFont"))
            readValue(secureFont)
        else if (xmlNameEqual(infoNode, "npcFont"))
            readValue(npcFont)
        else if (xmlNameEqual(infoNode, "japanFont"))
            readValue(japanFont)
        else if (xmlNameEqual(infoNode, "chinaFont"))
            readValue(chinaFont)
        else if (xmlNameEqual(infoNode, "fontSize"))
            readIntValue(fontSize)
        else if (xmlNameEqual(infoNode, "npcfontSize"))
            readIntValue(npcfontSize)
        else if (xmlNameEqual(infoNode, "guialpha"))
            readFloatValue(guiAlpha)
        else if (xmlNameEqual(infoNode, fontSize2.c_str()))
            readIntValue(fontSize)
        else if (xmlNameEqual(infoNode, npcfontSize2.c_str()))
            readIntValue(npcfontSize)
    }
    doc->decRef();
    return info;
}

ThemeColorIdT Theme::getIdByChar(const signed char c, bool &valid) const
{
    const CharColors::const_iterator it = mCharColors.find(c);
    if (it != mCharColors.end())
    {
        valid = true;
        return static_cast<ThemeColorIdT>((*it).second);
    }

    valid = false;
    return ThemeColorId::BROWSERBOX;
}
