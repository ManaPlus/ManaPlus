/*
 *  The ManaPlus Client
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

#include "gui/widgets/setupitem.h"

#include "configuration.h"
#include "main.h"
#include "logger.h"
#include "sound.h"

#include "gui/editdialog.h"
#include "gui/gui.h"
#include "gui/sdlfont.h"

#include "gui/widgets/checkbox.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/horizontcontainer.h"
#include "gui/widgets/inttextfield.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/slider.h"
#include "gui/widgets/sliderlist.h"
#include "gui/widgets/vertcontainer.h"

#include "utils/dtor.h"
#include "utils/gettext.h"
#include "utils/mathutils.h"

#include <guichan/font.hpp>

#include "debug.h"

SetupItem::SetupItem(std::string text, std::string description,
                     std::string keyName, SetupTabScroll *const parent,
                     std::string eventName, const bool mainConfig) :
    gcn::ActionListener(),
    Widget2(),
    mText(text),
    mDescription(description),
    mKeyName(keyName),
    mParent(parent),
    mEventName(eventName),
    mMainConfig(mainConfig),
    mUseDefault(false),
    mValue(""),
    mDefault(""),
    mWidget(nullptr),
    mValueType(VBOOL)
{
}

SetupItem::SetupItem(std::string text, std::string description,
                     std::string keyName, SetupTabScroll *const parent,
                     std::string eventName, std::string def,
                     const bool mainConfig) :
    gcn::ActionListener(),
    Widget2(),
    mText(text),
    mDescription(description),
    mKeyName(keyName),
    mParent(parent),
    mEventName(eventName),
    mMainConfig(mainConfig),
    mUseDefault(true),
    mValue(""),
    mDefault(def),
    mWidget(nullptr),
    mValueType(VBOOL)
{
}

SetupItem::~SetupItem()
{

}

Configuration *SetupItem::getConfig() const
{
    if (mMainConfig)
        return &config;
    else
        return &serverConfig;
}

void SetupItem::load()
{
    const Configuration *const cfg = getConfig();
    if (mUseDefault)
    {
        mValue = cfg->getValue(mKeyName, mDefault);
    }
    else
    {
        switch (mValueType)
        {
            case VBOOL:
                if (cfg->getBoolValue(mKeyName))
                    mValue = "1";
                else
                    mValue = "0";
                break;
            case VSTR:
            default:
                mValue = cfg->getStringValue(mKeyName);
                break;
            case VINT:
                mValue = toString(cfg->getIntValue(mKeyName));
                break;
            case VNONE:
                break;
        }
    }
}

void SetupItem::save() const
{
    Configuration *const cfg = getConfig();
    cfg->setValue(mKeyName, mValue);
}

std::string SetupItem::getActionEventId()
{
    if (!mWidget)
        return "";

    return mWidget->getActionEventId();
}

void SetupItem::action(const gcn::ActionEvent &event)
{
    if (!mWidget)
        return;

    if (event.getId() == mWidget->getActionEventId())
        action();
}

void SetupItem::action()
{
    fromWidget();
}

void SetupItem::apply(std::string eventName A_UNUSED)
{
    save();
}

void SetupItem::cancel(std::string eventName A_UNUSED)
{
    load();
    toWidget();
}

void SetupItem::externalUpdated(std::string eventName A_UNUSED)
{
    load();
    toWidget();
}

void SetupItem::fixFirstItemSize(gcn::Widget *const widget)
{
    const int maxSize = mParent->getPreferredFirstItemSize();
    if (widget->getWidth() < maxSize)
        widget->setWidth(maxSize);
}

SetupItemCheckBox::SetupItemCheckBox(std::string text, std::string description,
                                     std::string keyName,
                                     SetupTabScroll *const parent,
                                     std::string eventName,
                                     const bool mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, mainConfig)
{
    createControls();
}

SetupItemCheckBox::SetupItemCheckBox(std::string text, std::string description,
                                     std::string keyName,
                                     SetupTabScroll *const parent,
                                     std::string eventName, std::string def,
                                     const bool mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, def, mainConfig)
{
    createControls();
}

SetupItemCheckBox::~SetupItemCheckBox()
{
    mWidget = nullptr;
}

void SetupItemCheckBox::createControls()
{
    load();
    mCheckBox = new CheckBox(this, mText, mValue != "0", mParent, mEventName);
    mWidget = mCheckBox;
    mParent->getContainer()->add1(mWidget);
    mParent->addControl(this);
    mParent->addActionListener(this);
    mWidget->addActionListener(this);
}

void SetupItemCheckBox::fromWidget()
{
    if (!mCheckBox)
        return;

    if (mCheckBox->isSelected())
        mValue = "1";
    else
        mValue = "0";
}

void SetupItemCheckBox::toWidget()
{
    if (!mCheckBox)
        return;

    mCheckBox->setSelected(mValue != "0");
}


SetupItemTextField::SetupItemTextField(std::string text,
                                       std::string description,
                                       std::string keyName,
                                       SetupTabScroll *const parent,
                                       std::string eventName,
                                       const bool mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mTextField(nullptr),
    mButton(nullptr),
    mEditDialog(nullptr)
{
    mValueType = VSTR;
    createControls();
}

SetupItemTextField::SetupItemTextField(std::string text,
                                       std::string description,
                                       std::string keyName,
                                       SetupTabScroll *const parent,
                                       std::string eventName, std::string def,
                                       const bool mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, def, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mTextField(nullptr),
    mButton(nullptr),
    mEditDialog(nullptr)
{
    mValueType = VSTR;
    createControls();
}

SetupItemTextField::~SetupItemTextField()
{
    mHorizont = nullptr;
    mWidget = nullptr;
    mTextField = nullptr;
    mLabel = nullptr;
    mButton = nullptr;
}

void SetupItemTextField::createControls()
{
    load();
    mHorizont = new HorizontContainer(this, 32, 2);

    mLabel = new Label(mText);
    mTextField = new TextField(this, mValue, true, mParent, mEventName);
    mButton = new Button(this, _("Edit"), mEventName + "_EDIT", mParent);
    mWidget = mTextField;
    mTextField->setWidth(200);
    fixFirstItemSize(mLabel);
    mHorizont->add(mLabel);
    mHorizont->add(mTextField);
    mHorizont->add(mButton);

    mParent->getContainer()->add2(mHorizont, true, 4);
    mParent->addControl(this);
    mParent->addControl(this, mEventName + "_EDIT");
    mParent->addControl(this, mEventName + "_EDIT_OK");
    mParent->addActionListener(this);
    mWidget->addActionListener(this);
    mButton->addActionListener(this);
}

void SetupItemTextField::fromWidget()
{
    if (!mTextField)
        return;

    mValue = mTextField->getText();
}

void SetupItemTextField::toWidget()
{
    if (!mTextField)
        return;

    mTextField->setText(mValue);
}

void SetupItemTextField::action(const gcn::ActionEvent &event)
{
    if (!mTextField)
        return;

    if (event.getId() == mWidget->getActionEventId())
    {
        fromWidget();
    }
    else if (event.getId() == mEventName + "_EDIT")
    {
        mEditDialog =  new EditDialog(mText, mTextField->getText(),
            mEventName + "_EDIT_OK");
        mEditDialog->addActionListener(this);
    }
    else if (event.getId() == mEventName + "_EDIT_OK")
    {
        mTextField->setText(mEditDialog->getMsg());
        mEditDialog = nullptr;
    }
}

void SetupItemTextField::apply(std::string eventName)
{
    if (eventName != mEventName)
        return;

    fromWidget();
    save();
}

SetupItemIntTextField::SetupItemIntTextField(std::string text,
                                             std::string description,
                                             std::string keyName,
                                             SetupTabScroll *const parent,
                                             std::string eventName,
                                             const int min, const int max,
                                             const bool mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mTextField(nullptr),
    mButton(nullptr),
    mMin(min),
    mMax(max),
    mEditDialog(nullptr)
{
    mValueType = VSTR;
    createControls();
}

SetupItemIntTextField::SetupItemIntTextField(std::string text,
                                             std::string description,
                                             std::string keyName,
                                             SetupTabScroll *const parent,
                                             std::string eventName,
                                             const int min, const int max,
                                             std::string def,
                                             const bool mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, def, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mTextField(nullptr),
    mButton(nullptr),
    mMin(min),
    mMax(max),
    mEditDialog(nullptr)
{
    mValueType = VSTR;
    createControls();
}

SetupItemIntTextField::~SetupItemIntTextField()
{
    mHorizont = nullptr;
    mWidget = nullptr;
    mTextField = nullptr;
    mLabel = nullptr;
    mButton = nullptr;
}

void SetupItemIntTextField::createControls()
{
    load();
    mHorizont = new HorizontContainer(this, 32, 2);

    mLabel = new Label(mText);
    mTextField = new IntTextField(this, atoi(mValue.c_str()),
        mMin, mMax, true, 30);
    mTextField->setActionEventId(mEventName);
    mTextField->addActionListener(mParent);

    mButton = new Button(this, _("Edit"), mEventName + "_EDIT", mParent);
    mWidget = mTextField;
    mTextField->setWidth(50);
    fixFirstItemSize(mLabel);
    mHorizont->add(mLabel);
    mHorizont->add(mTextField);
    mHorizont->add(mButton);

    mParent->getContainer()->add2(mHorizont, true, 4);
    mParent->addControl(this);
    mParent->addControl(this, mEventName + "_EDIT");
    mParent->addControl(this, mEventName + "_EDIT_OK");
    mParent->addActionListener(this);
    mWidget->addActionListener(this);
    mButton->addActionListener(this);
}

void SetupItemIntTextField::fromWidget()
{
    if (!mTextField)
        return;

    mValue = mTextField->getText();
}

void SetupItemIntTextField::toWidget()
{
    if (!mTextField)
        return;

    mTextField->setText(mValue);
}

void SetupItemIntTextField::action(const gcn::ActionEvent &event)
{
    if (!mTextField)
        return;

    if (event.getId() == mWidget->getActionEventId())
    {
        fromWidget();
    }
    else if (event.getId() == mEventName + "_EDIT")
    {
        mEditDialog =  new EditDialog(mText, mTextField->getText(),
            mEventName + "_EDIT_OK");
        mEditDialog->addActionListener(this);
    }
    else if (event.getId() == mEventName + "_EDIT_OK")
    {
        mTextField->setValue(atoi(mEditDialog->getMsg().c_str()));
        mEditDialog = nullptr;
    }
}

void SetupItemIntTextField::apply(std::string eventName)
{
    if (eventName != mEventName)
        return;

    fromWidget();
    save();
}


SetupItemLabel::SetupItemLabel(std::string text, std::string description,
                               SetupTabScroll *const parent,
                               const bool separator) :
    SetupItem(text, description, "", parent, "", "", true),
    mLabel(nullptr),
    mIsSeparator(separator)
{
    mValueType = VNONE;
    createControls();
}

SetupItemLabel::~SetupItemLabel()
{
    mWidget = nullptr;
    mLabel = nullptr;
}

void SetupItemLabel::createControls()
{
    if (mIsSeparator)
    {
        const std::string str = " \342\200\225\342\200\225\342\200\225"
            "\342\200\225\342\200\225 ";
        mLabel = new Label(str + mText + str);
    }
    else
    {
        mLabel = new Label(mText);
    }

    mWidget = mLabel;
    mParent->getContainer()->add1(mWidget);
    mParent->addControl(this);
    mParent->addActionListener(this);
    mWidget->addActionListener(this);
}

void SetupItemLabel::fromWidget()
{
}

void SetupItemLabel::toWidget()
{
}

void SetupItemLabel::action(const gcn::ActionEvent &event A_UNUSED)
{
}

void SetupItemLabel::apply(std::string eventName A_UNUSED)
{
}


SetupItemDropDown::SetupItemDropDown(std::string text,
                                     std::string description,
                                     std::string keyName,
                                     SetupTabScroll *const parent,
                                     std::string eventName,
                                     gcn::ListModel *const model,
                                     const bool mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mModel(model),
    mDropDown(nullptr)
{
    mValueType = VSTR;
    createControls();
}

SetupItemDropDown::SetupItemDropDown(std::string text,
                                     std::string description,
                                     std::string keyName,
                                     SetupTabScroll *const parent,
                                     std::string eventName,
                                     gcn::ListModel *const model,
                                     std::string def,
                                     const bool mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, def, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mModel(model),
    mDropDown(nullptr)
{
    mValueType = VSTR;
    createControls();
}

SetupItemDropDown::~SetupItemDropDown()
{
    mHorizont = nullptr;
    mWidget = nullptr;
    mModel = nullptr;
    mDropDown = nullptr;
    mLabel = nullptr;
}

void SetupItemDropDown::createControls()
{
    load();
    mHorizont = new HorizontContainer(this, 32, 2);

    mLabel = new Label(mText);
    mDropDown = new DropDown(this, mModel);
    mDropDown->setActionEventId(mEventName);
    mDropDown->addActionListener(mParent);

    mWidget = mDropDown;
//    mTextField->setWidth(50);
    fixFirstItemSize(mLabel);
    mHorizont->add(mLabel);
    mHorizont->add(mDropDown);

    mParent->getContainer()->add2(mHorizont, true, 4);
    mParent->addControl(this);
    mParent->addActionListener(this);
    mWidget->addActionListener(this);
}

void SetupItemDropDown::fromWidget()
{
    if (!mDropDown)
        return;

    mValue = mDropDown->getSelectedString();
}

void SetupItemDropDown::toWidget()
{
    if (!mDropDown)
        return;

    mDropDown->setSelectedString(mValue);
}


SetupItemSlider::SetupItemSlider(std::string text, std::string description,
                                 std::string keyName,
                                 SetupTabScroll *const parent,
                                 std::string eventName,
                                 const double min, const double max,
                                 const int width, const bool onTheFly,
                                 const bool mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mSlider(nullptr),
    mMin(min),
    mMax(max),
    mWidth(width),
    mOnTheFly(onTheFly)
{
    mValueType = VSTR;
    createControls();
}

SetupItemSlider::SetupItemSlider(std::string text, std::string description,
                                 std::string keyName,
                                 SetupTabScroll *const parent,
                                 std::string eventName,
                                 const double min, const double max,
                                 std::string def, const int width,
                                 const bool onTheFly,
                                 const bool mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, def, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mSlider(nullptr),
    mMin(min),
    mMax(max),
    mWidth(width),
    mOnTheFly(onTheFly)
{
    mValueType = VSTR;
    createControls();
}

SetupItemSlider::~SetupItemSlider()
{
    mHorizont = nullptr;
    mWidget = nullptr;
    mSlider = nullptr;
    mLabel = nullptr;
}

void SetupItemSlider::createControls()
{
    load();
    mHorizont = new HorizontContainer(this, 32, 2);

    mLabel = new Label(mText);
    mSlider = new Slider(mMin, mMax);
    mSlider->setActionEventId(mEventName);
    mSlider->addActionListener(mParent);
    mSlider->setValue(atof(mValue.c_str()));
    mSlider->setHeight(30);

    mWidget = mSlider;
    mSlider->setWidth(mWidth);
    mSlider->setHeight(40);
    fixFirstItemSize(mLabel);
    mHorizont->add(mLabel);
    mHorizont->add(mSlider, -10);

    mParent->getContainer()->add2(mHorizont, true, 4);
    mParent->addControl(this);
    mParent->addActionListener(this);
    mWidget->addActionListener(this);
}

void SetupItemSlider::fromWidget()
{
    if (!mSlider)
        return;

    mValue = toString(mSlider->getValue());
}

void SetupItemSlider::toWidget()
{
    if (!mSlider)
        return;

    mSlider->setValue(atof(mValue.c_str()));
}

void SetupItemSlider::action(const gcn::ActionEvent &event A_UNUSED)
{
    fromWidget();
    if (mOnTheFly)
        save();
}

void SetupItemSlider::apply(std::string eventName)
{
    if (eventName != mEventName)
        return;

    fromWidget();
    save();
}


SetupItemSlider2::SetupItemSlider2(std::string text, std::string description,
                                   std::string keyName,
                                   SetupTabScroll *const parent,
                                   std::string eventName,
                                   const int min, const int max,
                                   SetupItemNames *const values,
                                   const bool onTheFly,
                                   const bool mainConfig,
                                   const bool doNotAlign) :
    SetupItem(text, description, keyName, parent, eventName, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mLabel2(nullptr),
    mSlider(nullptr),
    mValues(values),
    mMin(min),
    mMax(max),
    mInvert(false),
    mInvertValue(0),
    mOnTheFly(onTheFly),
    mDoNotAlign(doNotAlign)
{
    mValueType = VSTR;
    createControls();
}

SetupItemSlider2::SetupItemSlider2(std::string text, std::string description,
                                   std::string keyName,
                                   SetupTabScroll *const parent,
                                   std::string eventName,
                                   const int min, const int max,
                                   SetupItemNames *const values,
                                   std::string def,
                                   const bool onTheFly, const bool mainConfig,
                                   const bool doNotAlign) :
    SetupItem(text, description, keyName, parent, eventName, def, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mLabel2(nullptr),
    mSlider(nullptr),
    mValues(values),
    mMin(min),
    mMax(max),
    mInvert(false),
    mInvertValue(0),
    mOnTheFly(onTheFly),
    mDoNotAlign(doNotAlign)
{
    mValueType = VSTR;
    createControls();
}

SetupItemSlider2::~SetupItemSlider2()
{
    mHorizont = nullptr;
    mWidget = nullptr;
    mSlider = nullptr;
    mLabel = nullptr;
}

void SetupItemSlider2::createControls()
{
    load();
    mHorizont = new HorizontContainer(this, 32, 2);

    const int width = getMaxWidth();

    mLabel = new Label(mText);
    mLabel2 = new Label("");
    mLabel2->setWidth(width);
    mSlider = new Slider(mMin, mMax);
    mSlider->setActionEventId(mEventName);
    mSlider->addActionListener(mParent);
    mSlider->setValue(atof(mValue.c_str()));
    mSlider->setHeight(30);

    mWidget = mSlider;
    mSlider->setWidth(150);
    mSlider->setHeight(40);
    if (!mDoNotAlign)
        fixFirstItemSize(mLabel);
    mHorizont->add(mLabel);
    mHorizont->add(mSlider, -10);
    mHorizont->add(mLabel2);

    mParent->getContainer()->add2(mHorizont, true, 4);
    mParent->addControl(this);
    mParent->addActionListener(this);
    mWidget->addActionListener(this);
    updateLabel();
}

int SetupItemSlider2::getMaxWidth()
{
    if (!mValues || !gui)
        return 1;

    int maxWidth = 0;
    SetupItemNamesConstIter it = mValues->begin();
    const SetupItemNamesConstIter it_end = mValues->end();
    const gcn::Font *const font = gui->getFont();

    while (it != it_end)
    {
        const int w = font->getWidth(*it);
        if (w > maxWidth)
            maxWidth = w;

        ++ it;
    }
    return maxWidth;
}

void SetupItemSlider2::fromWidget()
{
    if (!mSlider)
        return;

    int val = roundDouble(mSlider->getValue());
    if (mInvert)
        val = mInvertValue - val;
    mValue = toString(val);
}

void SetupItemSlider2::toWidget()
{
    if (!mSlider)
        return;

    int val = roundDouble(atof(mValue.c_str()));
    if (mInvert)
        val = mInvertValue - val;
    mSlider->setValue(val);
    updateLabel();
}

void SetupItemSlider2::action(const gcn::ActionEvent &event A_UNUSED)
{
    fromWidget();
    updateLabel();
    if (mOnTheFly)
        save();
}

void SetupItemSlider2::updateLabel()
{
    int val = static_cast<int>(mSlider->getValue()) - mMin;
    if (val < 0)
        val = 0;
    else if (val >= static_cast<signed>(mValues->size()))
        val = static_cast<signed>(mValues->size()) - 1;
    std::string str = mValues->at(val);
    mLabel2->setCaption(str);
}

void SetupItemSlider2::apply(std::string eventName)
{
    if (eventName != mEventName)
        return;

    fromWidget();
    save();
}

void SetupItemSlider2::setInvertValue(const int v)
{
    mInvert = true;
    mInvertValue = v;
    toWidget();
}


SetupItemSliderList::SetupItemSliderList(std::string text,
                                         std::string description,
                                         std::string keyName,
                                         SetupTabScroll *const parent,
                                         std::string eventName,
                                         gcn::ListModel *const model,
                                         const int width, const bool onTheFly,
                                         const bool mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mSlider(nullptr),
    mModel(model),
    mWidth(width),
    mOnTheFly(onTheFly)
{
    mValueType = VSTR;
//    createControls();
}

SetupItemSliderList::SetupItemSliderList(std::string text,
                                         std::string description,
                                         std::string keyName,
                                         SetupTabScroll *const parent,
                                         std::string eventName,
                                         gcn::ListModel *const model,
                                         std::string def, const int width,
                                         const bool onTheFly,
                                         const bool mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, def, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mSlider(nullptr),
    mModel(model),
    mWidth(width),
    mOnTheFly(onTheFly)
{
    mValueType = VSTR;
//    createControls();
}

SetupItemSliderList::~SetupItemSliderList()
{
    mHorizont = nullptr;
    mWidget = nullptr;
    mSlider = nullptr;
    mLabel = nullptr;
}

void SetupItemSliderList::createControls()
{
    load();
    mHorizont = new HorizontContainer(this, 32, 2);

    mLabel = new Label(mText);
    mSlider = new SliderList(this, mModel, mParent, mEventName);
    mSlider->setSelectedString(mValue);
    mSlider->adjustSize();

    mWidget = mSlider;
    fixFirstItemSize(mLabel);
    mHorizont->add(mLabel, 5);
    mHorizont->add(mSlider);

    addMoreControls();

    mParent->getContainer()->add2(mHorizont, true, 4);
    mParent->addControl(this);
    mParent->addActionListener(this);
    mWidget->addActionListener(this);
}

void SetupItemSliderList::fromWidget()
{
    if (!mSlider)
        return;

    mValue = mSlider->getSelectedString();
}

void SetupItemSliderList::toWidget()
{
    if (!mSlider)
        return;

    mSlider->setSelectedString(mValue);
}

void SetupItemSliderList::action(const gcn::ActionEvent &event A_UNUSED)
{
    fromWidget();
    if (mOnTheFly)
        save();
}

void SetupItemSliderList::apply(std::string eventName)
{
    if (eventName != mEventName)
        return;

    fromWidget();
    save();
}

SetupItemSound::SetupItemSound(std::string text, std::string description,
                               std::string keyName,
                               SetupTabScroll *const parent,
                               std::string eventName,
                               gcn::ListModel *const model,
                               const int width, const bool onTheFly,
                               const bool mainConfig) :
    SetupItemSliderList(text, description, keyName, parent, eventName,
                        model, width, onTheFly, mainConfig),
    mButton(nullptr)
{
    createControls();
}

void SetupItemSound::addMoreControls()
{
    mButton = new Button(this, BUTTON_PLAY, 16, 16,
        mEventName + "_PLAY", this);
    mHorizont->add(mButton);
}

void SetupItemSound::action(const gcn::ActionEvent &event)
{
    if (event.getId() == mEventName + "_PLAY")
    {
        if (mSlider->getSelected())
        {
            sound.playGuiSfx(branding.getStringValue("systemsounds")
                + mSlider->getSelectedString() + ".ogg");
        }
    }
    else
    {
        SetupItemSliderList::action(event);
    }
}
