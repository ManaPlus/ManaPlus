/*
 *  Gui Skinning
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2009  The Mana World Development Team
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

#include "gui/theme.h"

#include "client.h"
#include "configuration.h"
#include "logger.h"

#include "resources/dye.h"
#include "resources/image.h"
#include "resources/imageset.h"
#include "resources/resourcemanager.h"

#include "utils/dtor.h"
#include "utils/stringutils.h"
#include "utils/xml.h"

#include <physfs.h>

#include <algorithm>
#include <physfs.h>

#include "debug.h"

static std::string defaultThemePath;

std::string Theme::mThemePath;
std::string Theme::mThemeName;
Theme *Theme::mInstance = nullptr;

// Set the theme path...
static void initDefaultThemePath()
{
    ResourceManager *resman = ResourceManager::getInstance();
    defaultThemePath = branding.getStringValue("guiThemePath");

    logger->log("defaultThemePath: " + defaultThemePath);
    if (!defaultThemePath.empty() && resman->isDirectory(defaultThemePath))
        return;
    else
        defaultThemePath = "themes/";
}

Skin::Skin(ImageRect skin, Image *close, Image *stickyUp, Image *stickyDown,
           const std::string &filePath,
           const std::string &name, int padding, int titlePadding):
    instances(1),
    mFilePath(filePath),
    mName(name),
    mBorder(skin),
    mCloseImage(close),
    mStickyImageUp(stickyUp),
    mStickyImageDown(stickyDown),
    mPadding(padding),
    mTitlePadding(titlePadding)
{
}

Skin::~Skin()
{
    // Clean up static resources
    for (int i = 0; i < 9; i++)
    {
        if (mBorder.grid[i])
        {
            mBorder.grid[i]->decRef();
            mBorder.grid[i] = nullptr;
        }
    }

    if (mCloseImage)
    {
        mCloseImage->decRef();
        mCloseImage = nullptr;
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
}

void Skin::updateAlpha(float minimumOpacityAllowed)
{
    const float alpha = static_cast<float>(
        std::max(static_cast<double>(minimumOpacityAllowed),
        static_cast<double>(Client::getGuiAlpha())));

    for (int i = 0; i < 9; i++)
    {
        if (mBorder.grid[i])
            mBorder.grid[i]->setAlpha(alpha);
    }

    if (mCloseImage)
        mCloseImage->setAlpha(alpha);
    if (mStickyImageUp)
        mStickyImageUp->setAlpha(alpha);
    if (mStickyImageDown)
        mStickyImageDown->setAlpha(alpha);
}

int Skin::getMinWidth() const
{
    if (!mBorder.grid[ImageRect::UPPER_LEFT]
        || !mBorder.grid[ImageRect::UPPER_RIGHT])
    {
        return 1;
    }

    return mBorder.grid[ImageRect::UPPER_LEFT]->getWidth() +
           mBorder.grid[ImageRect::UPPER_RIGHT]->getWidth();
}

int Skin::getMinHeight() const
{
    if (!mBorder.grid[ImageRect::UPPER_LEFT]
        || !mBorder.grid[ImageRect::LOWER_LEFT])
    {
        return 1;
    }

    return mBorder.grid[ImageRect::UPPER_LEFT]->getHeight() +
           mBorder.grid[ImageRect::LOWER_LEFT]->getHeight();
}

Theme::Theme():
    Palette(THEME_COLORS_END),
    mMinimumOpacity(-1.0f),
    mProgressColors(ProgressColors(THEME_PROG_END))
{
    initDefaultThemePath();

    config.addListener("guialpha", this);

    mColors[HIGHLIGHT].ch = 'H';
    mColors[CHAT].ch = 'C';
    mColors[GM].ch = 'G';
    mColors[PLAYER].ch = 'Y';
    mColors[WHISPER].ch = 'W';
    mColors[WHISPER_OFFLINE].ch = 'w';
    mColors[IS].ch = 'I';
    mColors[PARTY_CHAT_TAB].ch = 'P';
    mColors[GUILD_CHAT_TAB].ch = 'U';
    mColors[SERVER].ch = 'S';
    mColors[LOGGER].ch = 'L';
    mColors[HYPERLINK].ch = '<';
    mCharColors['H'] = HIGHLIGHT;
    mCharColors['C'] = CHAT;
    mCharColors['G'] = GM;
    mCharColors['Y'] = PLAYER;
    mCharColors['W'] = WHISPER;
    mCharColors['w'] = WHISPER_OFFLINE;
    mCharColors['I'] = IS;
    mCharColors['P'] = PARTY_CHAT_TAB;
    mCharColors['U'] = GUILD_CHAT_TAB;
    mCharColors['S'] = SERVER;
    mCharColors['L'] = LOGGER;
    mCharColors['<'] = HYPERLINK;
}

Theme::~Theme()
{
    delete_all(mSkins);
    config.removeListener("guialpha", this);
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

gcn::Color Theme::getProgressColor(int type, float progress)
{
    int color[3] = {0, 0, 0};

    if (mInstance)
    {
        DyePalette *dye = mInstance->mProgressColors[type];

        if (dye)
            dye->getColor(progress, color);
        else
            logger->log("color not found: " + toString(type));
    }

    return gcn::Color(color[0], color[1], color[2]);
}

Skin *Theme::load(const std::string &filename, const std::string &defaultPath)
{
    // Check if this skin was already loaded

    SkinIterator skinIterator = mSkins.find(filename);
    if (mSkins.end() != skinIterator)
    {
        if (skinIterator->second)
            skinIterator->second->instances++;
        return skinIterator->second;
    }

    Skin *skin = readSkin(filename);

    if (!skin)
        skin = readSkin("window.xml");

    if (!skin)
    {
        // Try falling back on the defaultPath if this makes sense
        if (filename != defaultPath)
        {
            logger->log("Error loading skin '%s', falling back on default.",
                        filename.c_str());

            skin = readSkin(defaultPath);
        }

        if (!skin)
        {
            logger->log(strprintf("Error: Loading default skin '%s' failed. "
                                  "Make sure the skin file is valid.",
                                  defaultPath.c_str()));
        }
    }

    // Add the skin to the loaded skins
    mSkins[filename] = skin;

    return skin;
}

void Theme::unload(Skin *skin)
{
    if (!skin)
        return;
    skin->instances --;
    // unload theme if no instances
    if (!skin->instances)
    {
        SkinIterator it = mSkins.begin();
        SkinIterator it_end = mSkins.end();
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

void Theme::setMinimumOpacity(float minimumOpacity)
{
    if (minimumOpacity > 1.0f)
        return;

    mMinimumOpacity = minimumOpacity;
    updateAlpha();
}

void Theme::updateAlpha()
{
    for (SkinIterator iter = mSkins.begin(), iter_end = mSkins.end();
         iter != iter_end;
         ++ iter)
    {
        if (iter->second)
            iter->second->updateAlpha(mMinimumOpacity);
    }
}

void Theme::optionChanged(const std::string &)
{
    updateAlpha();
}

struct SkinParameter
{
    int index;
    std::string name;
};

static SkinParameter skinParam[] =
{
    {0, "top-left-corner"},
    {0, "standart"},
    {0, "up"},
    {0, "hstart"},
    {1, "top-edge"},
    {1, "highlighted"},
    {1, "down"},
    {1, "hmiddle"},
    {2, "top-right-corner"},
    {2, "pressed"},
    {2, "left"},
    {2, "hend"},
    {3, "left-edge"},
    {3, "disabled"},
    {3, "right"},
    {3, "hgrip"},
    {4, "bg-quad"},
    {4, "vstart"},
    {5, "right-edge"},
    {5, "vmiddle"},
    {6, "bottom-left-corner"},
    {6, "vend"},
    {7, "bottom-edge"},
    {7, "vgrip"},
    {8, "bottom-right-corner"},
};

static SkinParameter imageParam[] =
{
    {0, "closeImage"},
    {1, "stickyImageUp"},
    {2, "stickyImageDown"},
};

struct SkinHelper
{
    std::string partType;
    int xPos;
    int yPos;
    int width;
    int height;
    ImageRect *rect;
    XmlNodePtr *node;
    Image *image;
    ResourceManager *resman;

    bool loadList(SkinParameter *params, size_t size)
    {
        for (unsigned f = 0; f < size; f ++)
        {
            if (partType == params[f].name)
            {
//                rect->grid[params[f].index] = image->getSubImage(
//                    xPos, yPos, width, height);
                rect->grid[params[f].index] = resman->getSubImage(
                    image, xPos, yPos, width, height);
                return true;
            }
        }
        return false;
    }
};

Skin *Theme::readSkin(const std::string &filename)
{
    if (filename.empty())
        return nullptr;

//    logger->log("Loading skin '%s'.", filename.c_str());

    XML::Document doc(resolveThemePath(filename));
    XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "skinset"))
        return nullptr;

    const std::string skinSetImage = XML::getProperty(rootNode, "image", "");

    if (skinSetImage.empty())
    {
        logger->log1("Theme::readSkin(): Skinset does not define an image!");
        return nullptr;
    }

    Image *dBorders = Theme::getImageFromTheme(skinSetImage);
    ImageRect border;
    ImageRect images;
    memset(&border, 0, sizeof(ImageRect));
    memset(&images, 0, sizeof(ImageRect));
    int padding = 3;
    int titlePadding = 4;

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

                    helper.rect = &border;
                    if (!helper.loadList(skinParam,
                        sizeof(skinParam) / sizeof(SkinParameter)))
                    {
                        helper.rect = &images;
                        helper.loadList(imageParam,
                            sizeof(imageParam) / sizeof(SkinParameter));
                    }
                }
                else if (xmlNameEqual(partNode, "option"))
                {
                    const std::string name = XML::getProperty(
                        partNode, "name", "");
                    if (name == "padding")
                        padding = XML::getProperty(partNode, "value", 3);
                    else if (name == "titlePadding")
                        titlePadding = XML::getProperty(partNode, "value", 4);
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

    Skin *skin = new Skin(border, images.grid[0], images.grid[1],
        images.grid[2], filename, "", padding, titlePadding);
    skin->updateAlpha(mMinimumOpacity);
    return skin;
}

bool Theme::tryThemePath(std::string themeName)
{
    if (!themeName.empty())
    {
        std::string path = defaultThemePath + themeName;
        if (PHYSFS_exists(path.c_str()))
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
    char **skins = PHYSFS_enumerateFiles(
        branding.getStringValue("guiThemePath").c_str());

    for (char **i = skins; *i; i++)
    {
        if (PHYSFS_isDirectory((
            branding.getStringValue("guiThemePath") + *i).c_str()))
        {
            list.push_back(*i);
        }
    }

    PHYSFS_freeList(skins);
}

void Theme::fillFontsList(StringVect &list)
{
    PHYSFS_permitSymbolicLinks(1);
    char **fonts = PHYSFS_enumerateFiles(
        branding.getStringValue("fontsPath").c_str());

    for (char **i = fonts; *i; i++)
    {
        if (!PHYSFS_isDirectory((
            branding.getStringValue("fontsPath") + *i).c_str()))
        {
            list.push_back(*i);
        }
    }

    PHYSFS_freeList(fonts);
    PHYSFS_permitSymbolicLinks(0);
}

void Theme::fillSoundsList(StringVect &list)
{
    char **skins = PHYSFS_enumerateFiles(
        branding.getStringValue("systemsounds").c_str());

    for (char **i = skins; *i; i++)
    {
        if (!PHYSFS_isDirectory((
            branding.getStringValue("systemsounds") + *i).c_str()))
        {
            std::string str = *i;
            if (findCutLast(str, ".ogg"))
                list.push_back(str);
        }
    }

    PHYSFS_freeList(skins);
}

void Theme::selectSkin()
{
    prepareThemePath();
}

void Theme::prepareThemePath()
{
    initDefaultThemePath();

    mThemePath = "";
    mThemeName = "";

    // Try theme from settings
    if (tryThemePath(config.getValue("selectedSkin", "")))
        return;

    // Try theme from settings
    if (tryThemePath(config.getValue("theme", "")))
        return;

    // Try theme from branding
    if (tryThemePath(branding.getValue("theme", "")))
        return;

    if (mThemePath.empty())
        mThemePath = "graphics/gui";

    instance()->loadColors(mThemePath);

    logger->log("Selected Theme: " + mThemePath);
}

std::string Theme::resolveThemePath(const std::string &path)
{
    // Need to strip off any dye info for the existence tests
    int pos = static_cast<int>(path.find('|'));
    std::string file;
    if (pos > 0)
        file = path.substr(0, pos);
    else
        file = path;

    // File with path
    if (file.find('/') != std::string::npos)
    {
        // Might be a valid path already
        if (PHYSFS_exists(file.c_str()))
            return path;
    }

    // Try the theme
    file = getThemePath() + "/" + file;

    if (PHYSFS_exists(file.c_str()))
        return getThemePath() + "/" + path;

    // Backup
    return branding.getStringValue("guiPath") + path;
}

Image *Theme::getImageFromTheme(const std::string &path)
{
    ResourceManager *resman = ResourceManager::getInstance();
    return resman->getImage(resolveThemePath(path));
}

ImageSet *Theme::getImageSetFromTheme(const std::string &path,
                                      int w, int h)
{
    ResourceManager *resman = ResourceManager::getInstance();
    return resman->getImageSet(resolveThemePath(path), w, h);
}

static int readColorType(const std::string &type)
{
    static std::string colors[] =
    {
        "TEXT",
        "SHADOW",
        "OUTLINE",
        "BORDER",
        "PROGRESS_BAR",
        "BUTTON",
        "BUTTON_DISABLED",
        "TAB",
        "PARTY_CHAT_TAB",
        "PARTY_SOCIAL_TAB",
        "GUILD_CHAT_TAB",
        "GUILD_SOCIAL_TAB",
        "BACKGROUND",
        "BACKGROUND_GRAY",
        "SCROLLBAR_GRAY",
        "DROPDOWN_SHADOW",
        "HIGHLIGHT",
        "TAB_FLASH",
        "TAB_PLAYER_FLASH",
        "SHOP_WARNING",
        "ITEM_EQUIPPED",
        "ITEM_NOT_EQUIPPED",
        "CHAT",
        "GM",
        "PLAYER",
        "WHISPER",
        "WHISPER_OFFLINE",
        "IS",
        "SERVER",
        "LOGGER",
        "HYPERLINK",
        "UNKNOWN_ITEM",
        "GENERIC",
        "HEAD",
        "USABLE",
        "TORSO",
        "ONEHAND",
        "LEGS",
        "FEET",
        "TWOHAND",
        "SHIELD",
        "RING",
        "NECKLACE",
        "ARMS",
        "AMMO",
        "SERVER_VERSION_NOT_SUPPORTED",
        "WARNING",
        "CHARM",
        "PLAYER_ADVANCED",
        "RED",
        "GREEN",
        "BLUE",
        "ORANGE",
        "YELLOW",
        "PINK",
        "PURPLE",
        "GRAY",
        "BROWN"
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
    int size = static_cast<int>(description.length());
    if (size < 7 || description[0] != '#')
    {
        logger->log("Error, invalid theme color palette: %s",
                    description.c_str());
        return Palette::BLACK;
    }

    int v = 0;
    for (int i = 1; i < 7; ++i)
    {
        char c = description[i];
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
    static std::string grads[] =
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
    static std::string colors[] =
    {
        "DEFAULT",
        "HP",
        "MP",
        "NO_MP",
        "EXP",
        "INVY_SLOTS",
        "WEIGHT",
        "JOB"
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
//    if (file == mThemePath)
//        return; // No need to reload

    if (file == "")
        file = "colors.xml";
    else
        file += "/colors.xml";

    XML::Document doc(resolveThemePath(file));
    XmlNodePtr root = doc.rootNode();

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

    for_each_xml_child_node(node, root)
    {
        if (xmlNameEqual(node, "color"))
        {
            type = readColorType(XML::getProperty(node, "id", ""));
            if (type < 0) // invalid or no type given
                continue;

            temp = XML::getProperty(node, "color", "");
            if (temp.empty()) // no color set, so move on
                continue;

            color = readColor(temp);
            grad = readColorGradient(XML::getProperty(node, "effect", ""));

            mColors[type].set(type, color, grad, 10);
        }
        else if (xmlNameEqual(node, "progressbar"))
        {
            type = readProgressType(XML::getProperty(node, "id", ""));
            if (type < 0) // invalid or no type given
                continue;

            mProgressColors[type] = new DyePalette(XML::getProperty(node,
                                                   "color", ""));
        }
    }
}

void Theme::loadRect(ImageRect &image, std::string name, int start, int end)
{
    Skin *skin = load(name);
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

void Theme::unloadRect(ImageRect &rect, int start, int end)
{
    for (int f = start; f <= end; f ++)
    {
        if (rect.grid[f])
            rect.grid[f]->decRef();
    }
}

Image *Theme::getImageFromThemeXml(const std::string &name)
{
    Theme *theme = Theme::instance();
    Skin *skin = theme->load(name);
    if (skin)
    {
        const ImageRect &rect = skin->getBorder();
        if (rect.grid[0])
        {
            Image *image = rect.grid[0];
            image->incRef();
            theme->unload(skin);
            return image;
        }
        theme->unload(skin);
    }
    return nullptr;
}
