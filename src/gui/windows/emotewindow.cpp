/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "gui/windows/emotewindow.h"

#include "gui/models/colormodel.h"
#include "gui/models/namesmodel.h"

#include "gui/windows/setupwindow.h"

#include "gui/widgets/colorpage.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/emotepage.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/tabbedarea.h"

#include "utils/delete2.h"
#include "utils/foreach.h"
#include "utils/gettext.h"

#include "utils/translation/podict.h"

#include "resources/imageset.h"

#include "resources/db/textdb.h"

#include "resources/image/image.h"

#include "debug.h"

EmoteWindow *emoteWindow = nullptr;
static const int fontSizeListSize = 2;

static const char *const fontSizeList[] =
{
    // TRANSLATORS: font size
    N_("Normal font"),
    // TRANSLATORS: font size
    N_("Bold font"),
};

EmoteWindow::EmoteWindow() :
    // TRANSLATORS: emotes window name
    Window(_("Emotes"), Modal_false, nullptr, "emotes.xml"),
    mTabs(CREATEWIDGETR(TabbedArea, this)),
    mEmotePage(new EmotePage(this)),
    mColorModel(ColorModel::createDefault(this)),
    mColorPage(CREATEWIDGETR(ColorPage, this, mColorModel, "colorpage.xml")),
    mScrollColorPage(new ScrollArea(this, mColorPage, Opaque_false,
        "emotepage.xml")),
    mFontModel(new NamesModel),
    mFontPage(CREATEWIDGETR(ListBox, this, mFontModel, "")),
    mScrollFontPage(new ScrollArea(this, mFontPage, Opaque_false,
        "fontpage.xml")),
    mTextModel(new NamesModel),
    mTextPage(CREATEWIDGETR(ListBox, this, mTextModel, "")),
    mScrollTextPage(new ScrollArea(this, mTextPage, Opaque_false,
        "textpage.xml")),
    mImageSet(Theme::getImageSetFromThemeXml("emotetabs.xml", "", 17, 16))
{
    setShowTitle(false);
    setResizable(true);

    if (setupWindow != nullptr)
        setupWindow->registerWindowForReset(this);

    addMouseListener(this);
}

void EmoteWindow::postInit()
{
    Window::postInit();
    const int pad2 = mPadding * 2;
    const int width = 200;
    const int height = 150;
    setWidth(width + pad2);
    setHeight(height + pad2);
    add(mTabs);
    mTabs->setPosition(mPadding, mPadding);
    mTabs->setWidth(width);
    mTabs->setHeight(height);
    center();

    setTitleBarHeight(getPadding() + getTitlePadding());
    mScrollColorPage->setVerticalScrollPolicy(ScrollArea::SHOW_ALWAYS);
    mScrollColorPage->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mScrollFontPage->setVerticalScrollPolicy(ScrollArea::SHOW_NEVER);
    mScrollFontPage->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mScrollTextPage->setVerticalScrollPolicy(ScrollArea::SHOW_NEVER);
    mScrollTextPage->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);

    mFontModel->fillFromArray(&fontSizeList[0], fontSizeListSize);

    mFontPage->setCenter(true);
    mTextPage->setCenter(true);

    if ((mImageSet != nullptr) && mImageSet->size() >= 3)
    {
        for (int f = 0; f < 3; f ++)
        {
            Image *const image = mImageSet->get(f);
            if (image != nullptr)
                image->incRef();
        }

        mTabs->addTab(mImageSet->get(0), mEmotePage);
        mTabs->addTab(mImageSet->get(2), mScrollColorPage);
        mTabs->addTab(mImageSet->get(1), mScrollFontPage);
    }
    else
    {
        // TRANSLATORS: emotes tab name
        mTabs->addTab(_("Emotes"), mEmotePage);
        // TRANSLATORS: emotes tab name
        mTabs->addTab(_("Colors"), mScrollColorPage);
        // TRANSLATORS: emotes tab name
        mTabs->addTab(_("Fonts"), mScrollFontPage);
    }
    // TRANSLATORS: emotes tab name
    mTabs->addTab(_("T"), mScrollTextPage);

    mEmotePage->setActionEventId("emote");
    mColorPage->setActionEventId("color");
    mFontPage->setActionEventId("font");
    mTextPage->setActionEventId("text");
}

EmoteWindow::~EmoteWindow()
{
    mTabs->removeAll(false);
    mTabs->removeTab(mTabs->getTabByIndex(0));
    delete2(mEmotePage)
    delete2(mColorPage)
    delete2(mColorModel)
    delete2(mScrollColorPage)
    delete2(mFontPage)
    delete2(mFontModel)
    delete2(mScrollFontPage)
    delete2(mTextPage)
    delete2(mTextModel)
    delete2(mScrollTextPage)
    if (mImageSet != nullptr)
    {
        mImageSet->decRef();
        mImageSet = nullptr;
    }
}

void EmoteWindow::show()
{
    setVisible(Visible_true);
    if (dictionary != nullptr)
    {
        mTextModel->clear();
        const STD_VECTOR<std::string> &texts = TextDb::getTexts();
        FOR_EACH (StringVectCIter, it, texts)
        {
            mTextModel->add(dictionary->getStr(*it));
        }
    }
}

void EmoteWindow::hide()
{
    setVisible(Visible_false);
}

std::string EmoteWindow::getSelectedEmote() const
{
    const int index = mEmotePage->getSelectedIndex();
    if (index < 0)
        return std::string();

    char chr[2];
    chr[0] = CAST_8('0' + index);
    chr[1] = 0;
    return std::string("%%").append(&chr[0]);
}

void EmoteWindow::clearEmote()
{
    const int index = mEmotePage->getSelectedIndex();
    mEmotePage->resetAction();
    if (index >= 0)
        setVisible(Visible_false);
}

std::string EmoteWindow::getSelectedColor() const
{
    const int index = mColorPage->getSelected();
    if (index < 0)
        return std::string();

    char chr[2];
    chr[0] = CAST_8('0' + index);
    chr[1] = 0;
    return std::string("##").append(&chr[0]);
}

int EmoteWindow::getSelectedTextIndex() const
{
    return mTextPage->getSelected();
}

void EmoteWindow::clearColor()
{
    mColorPage->resetAction();
    setVisible(Visible_false);
}

std::string EmoteWindow::getSelectedFont() const
{
    const int index = mFontPage->getSelected();
    if (index < 0)
        return std::string();

    if (index == 0)
        return "##b";
    return "##B";
}

void EmoteWindow::clearFont()
{
    mFontPage->setSelected(-1);
    setVisible(Visible_false);
}

void EmoteWindow::clearText()
{
    mTextPage->setSelected(-1);
    setVisible(Visible_false);
}

void EmoteWindow::addListeners(ActionListener *const listener)
{
    mEmotePage->addActionListener(listener);
    mColorPage->addActionListener(listener);
    mFontPage->addActionListener(listener);
    mTextPage->addActionListener(listener);
}

void EmoteWindow::widgetResized(const Event &event)
{
    Window::widgetResized(event);
    const int pad2 = mPadding * 2;
    const int width = mDimension.width;
    const int height = mDimension.height;

    mTabs->setSize(width - pad2, height - pad2);
    mTabs->adjustWidget(mEmotePage);
    mTabs->adjustWidget(mScrollColorPage);
    mColorPage->setSize(mScrollColorPage->getWidth(),
        mScrollColorPage->getHeight());
    mEmotePage->widgetResized(event);
}

void EmoteWindow::widgetMoved(const Event &event)
{
    Window::widgetMoved(event);
    mEmotePage->widgetResized(event);
}
