/*
 *  The ManaPlus Client
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

#include "gui/widgets/setupitem.h"

#include "configuration.h"
#include "soundmanager.h"

#include "gui/gui.h"

#include "gui/fonts/font.h"

#include "gui/models/listmodel.h"

#include "gui/windows/editdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/horizontcontainer.h"
#include "gui/widgets/inttextfield.h"
#include "gui/widgets/label.h"
#include "gui/widgets/slider.h"
#include "gui/widgets/sliderlist.h"
#include "gui/widgets/vertcontainer.h"

#include "gui/widgets/tabs/setuptabscroll.h"

#include "utils/base64.h"
#include "utils/gettext.h"
#include "utils/stdmove.h"
#include "utils/mathutils.h"

#include "debug.h"

SetupItem::SetupItem(const std::string &restrict text,
                     const std::string &restrict description,
                     const std::string &restrict keyName,
                     SetupTabScroll *restrict const parent,
                     const std::string &restrict eventName,
                     const MainConfig mainConfig) :
    ActionListener(),
    Widget2(parent),
    mText(text),
    mDescription(description),
    mKeyName(keyName),
    mParent(parent),
    mEventName(eventName),
    mValue(),
    mDefault(),
    mWidget(nullptr),
    mTempWidgets(),
    mValueType(VBOOL),
    mMainConfig(mainConfig),
    mUseDefault(false)
{
}

SetupItem::SetupItem(const std::string &restrict text,
                     const std::string &restrict description,
                     const std::string &restrict keyName,
                     SetupTabScroll *restrict const parent,
                     const std::string &restrict eventName,
                     const std::string &restrict def,
                     const MainConfig mainConfig) :
    ActionListener(),
    Widget2(parent),
    mText(text),
    mDescription(description),
    mKeyName(keyName),
    mParent(parent),
    mEventName(eventName),
    mValue(),
    mDefault(def),
    mWidget(nullptr),
    mTempWidgets(),
    mValueType(VBOOL),
    mMainConfig(mainConfig),
    mUseDefault(true)
{
}

SetupItem::~SetupItem()
{
}

Configuration *SetupItem::getConfig() const
{
    if (mMainConfig == MainConfig_true)
        return &config;
    return &serverConfig;
}

void SetupItem::load()
{
    if (mKeyName.empty())
        return;

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

void SetupItem::save()
{
    if (mKeyName.empty())
        return;

    Configuration *const cfg = getConfig();
    cfg->setValue(mKeyName, mValue);
}

std::string SetupItem::getActionEventId() const
{
    if (mWidget == nullptr)
        return std::string();

    return mWidget->getActionEventId();
}

void SetupItem::action(const ActionEvent &event)
{
    if (mWidget == nullptr)
        return;

    if (event.getId() == mWidget->getActionEventId())
        doAction();
}

void SetupItem::doAction()
{
    fromWidget();
}

void SetupItem::apply(const std::string &eventName A_UNUSED)
{
    save();
}

void SetupItem::cancel(const std::string &eventName A_UNUSED)
{
    load();
    toWidget();
}

void SetupItem::externalUpdated(const std::string &eventName A_UNUSED)
{
    load();
    toWidget();
}

void SetupItem::externalUnloaded(const std::string &eventName A_UNUSED)
{
}

void SetupItem::fixFirstItemSize(Widget *const widget)
{
    if (widget == nullptr)
        return;
    const int maxSize = mParent->getPreferredFirstItemSize();
    if (widget->getWidth() < maxSize)
        widget->setWidth(maxSize);
}

void SetupItem::rereadValue()
{
    load();
    toWidget();
}

SetupItemCheckBox::SetupItemCheckBox(const std::string &restrict text,
                                     const std::string &restrict description,
                                     const std::string &restrict keyName,
                                     SetupTabScroll *restrict const parent,
                                     const std::string &restrict eventName,
                                     const MainConfig mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, mainConfig),
    mCheckBox(nullptr)
{
    createControls();
}

SetupItemCheckBox::SetupItemCheckBox(const std::string &restrict text,
                                     const std::string &restrict description,
                                     const std::string &restrict keyName,
                                     SetupTabScroll *restrict const parent,
                                     const std::string &restrict eventName,
                                     const std::string &restrict def,
                                     const MainConfig mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, def, mainConfig),
    mCheckBox(nullptr)
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
    mCheckBox->setToolTip(mDescription);
    mWidget = mCheckBox;
    mParent->getContainer()->add1(mWidget);
    mParent->addControl(this);
    mParent->addActionListener(this);
    mWidget->addActionListener(this);
}

void SetupItemCheckBox::fromWidget()
{
    if (mCheckBox == nullptr)
        return;

    if (mCheckBox->isSelected())
        mValue = "1";
    else
        mValue = "0";
}

void SetupItemCheckBox::toWidget()
{
    if (mCheckBox == nullptr)
        return;

    mCheckBox->setSelected(mValue != "0");
}


SetupItemTextField::SetupItemTextField(const std::string &restrict text,
                                       const std::string &restrict description,
                                       const std::string &restrict keyName,
                                       SetupTabScroll *restrict const parent,
                                       const std::string &restrict eventName,
                                       const MainConfig mainConfig,
                                       const UseBase64 useBase64) :
    SetupItem(text, description, keyName, parent, eventName, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mTextField(nullptr),
    mButton(nullptr),
    mEditDialog(nullptr),
    mUseBase64(useBase64)
{
    mValueType = VSTR;
    createControls();
}

SetupItemTextField::SetupItemTextField(const std::string &restrict text,
                                       const std::string &restrict description,
                                       const std::string &restrict keyName,
                                       SetupTabScroll *restrict const parent,
                                       const std::string &restrict eventName,
                                       const std::string &restrict def,
                                       const MainConfig mainConfig,
                                       const UseBase64 useBase64) :
    SetupItem(text, description, keyName, parent, eventName, def, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mTextField(nullptr),
    mButton(nullptr),
    mEditDialog(nullptr),
    mUseBase64(useBase64)
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

void SetupItemTextField::save()
{
    if (mUseBase64 == UseBase64_true)
    {
        std::string normalValue = mValue;
        mValue = encodeBase64String(mValue);
        SetupItem::save();
        mValue = STD_MOVE(normalValue);
    }
    else
    {
        SetupItem::save();
    }
}

void SetupItemTextField::cancel(const std::string &eventName A_UNUSED)
{
    load();
    if (mUseBase64 == UseBase64_true)
        mValue = decodeBase64String(mValue);
    toWidget();
}

void SetupItemTextField::externalUpdated(const std::string &eventName A_UNUSED)
{
    load();
    if (mUseBase64 == UseBase64_true)
        mValue = decodeBase64String(mValue);
    toWidget();
}

void SetupItemTextField::rereadValue()
{
    load();
    if (mUseBase64 == UseBase64_true)
        mValue = decodeBase64String(mValue);
    toWidget();
}

void SetupItemTextField::createControls()
{
    load();
    if (mUseBase64 == UseBase64_true)
        mValue = decodeBase64String(mValue);
    mHorizont = new HorizontContainer(this, 32, 2);

    mLabel = new Label(this, mText);
    mLabel->setToolTip(mDescription);
    mTextField = new TextField(this,
        mValue,
        LoseFocusOnTab_true,
        mParent,
        mEventName);
    // TRANSLATORS: setup item button
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
    if (mTextField == nullptr)
        return;

    mValue = mTextField->getText();
}

void SetupItemTextField::toWidget()
{
    if (mTextField == nullptr)
        return;

    mTextField->setText(mValue);
}

void SetupItemTextField::action(const ActionEvent &event)
{
    if (mTextField == nullptr)
        return;

    const std::string &eventId = event.getId();
    if ((mWidget != nullptr) && eventId == mWidget->getActionEventId())
    {
        fromWidget();
    }
    else if (eventId == mEventName + "_EDIT")
    {
        mEditDialog = CREATEWIDGETR(EditDialog,
            mText,
            mTextField->getText(),
            mEventName + "_EDIT_OK");
        mEditDialog->addActionListener(this);
    }
    else if (eventId == mEventName + "_EDIT_OK")
    {
        mTextField->setText(mEditDialog->getMsg());
        mEditDialog = nullptr;
    }
}

void SetupItemTextField::apply(const std::string &eventName)
{
    if (eventName != mEventName)
        return;

    fromWidget();
    save();
}

SetupItemIntTextField::SetupItemIntTextField(const std::string &restrict text,
                                             const std::string &restrict
                                             description,
                                             const std::string &restrict
                                             keyName,
                                             SetupTabScroll *restrict
                                             const parent,
                                             const std::string &restrict
                                             eventName,
                                             const int min, const int max,
                                             const MainConfig mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mTextField(nullptr),
    mButton(nullptr),
    mEditDialog(nullptr),
    mMin(min),
    mMax(max)
{
    mValueType = VSTR;
    createControls();
}

SetupItemIntTextField::SetupItemIntTextField(const std::string &restrict text,
                                             const std::string &restrict
                                             description,
                                             const std::string &restrict
                                             keyName,
                                             SetupTabScroll *restrict
                                             const parent,
                                             const std::string &restrict
                                             eventName,
                                             const int min, const int max,
                                             const std::string &restrict def,
                                             const MainConfig mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, def, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mTextField(nullptr),
    mButton(nullptr),
    mEditDialog(nullptr),
    mMin(min),
    mMax(max)
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

    mLabel = new Label(this, mText);
    mLabel->setToolTip(mDescription);
    mTextField = new IntTextField(this, atoi(mValue.c_str()),
        mMin, mMax, Enable_true, 30);
    mTextField->setActionEventId(mEventName);
    mTextField->addActionListener(mParent);

    // TRANSLATORS: setup item button
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
    if (mTextField == nullptr)
        return;

    mValue = mTextField->getText();
}

void SetupItemIntTextField::toWidget()
{
    if (mTextField == nullptr)
        return;

    mTextField->setText(mValue);
}

void SetupItemIntTextField::action(const ActionEvent &event)
{
    if (mTextField == nullptr)
        return;

    const std::string &eventId = event.getId();
    if ((mWidget != nullptr) && eventId == mWidget->getActionEventId())
    {
        fromWidget();
    }
    else if (eventId == mEventName + "_EDIT")
    {
        mEditDialog = CREATEWIDGETR(EditDialog,
            mText,
            mTextField->getText(),
            mEventName + "_EDIT_OK");
        mEditDialog->addActionListener(this);
    }
    else if (eventId == mEventName + "_EDIT_OK")
    {
        mTextField->setValue(atoi(mEditDialog->getMsg().c_str()));
        mEditDialog = nullptr;
    }
}

void SetupItemIntTextField::apply(const std::string &eventName)
{
    if (eventName != mEventName)
        return;

    fromWidget();
    save();
}


SetupItemLabel::SetupItemLabel(const std::string &restrict text,
                               const std::string &restrict description,
                               SetupTabScroll *restrict const parent,
                               const Separator separator) :
    SetupItem(text, description, "", parent, "", "", MainConfig_true),
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
    if (mIsSeparator == Separator_true)
    {
        const std::string str(" \342\200\225\342\200\225\342\200\225"
            "\342\200\225\342\200\225 ");
        mLabel = new Label(this, std::string(str).append(mText).append(str));
    }
    else
    {
        mLabel = new Label(this, mText);
    }
    mLabel->setToolTip(mDescription);

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

void SetupItemLabel::action(const ActionEvent &event A_UNUSED)
{
}

void SetupItemLabel::apply(const std::string &eventName A_UNUSED)
{
}


SetupItemDropDown::SetupItemDropDown(const std::string &restrict text,
                                     const std::string &restrict description,
                                     const std::string &restrict keyName,
                                     SetupTabScroll *restrict const parent,
                                     const std::string &restrict eventName,
                                     ListModel *restrict const model,
                                     const int width,
                                     const MainConfig mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mModel(model),
    mDropDown(nullptr),
    mWidth(width)
{
    mValueType = VSTR;
    createControls();
}

SetupItemDropDown::SetupItemDropDown(const std::string &restrict text,
                                     const std::string &restrict description,
                                     const std::string &restrict keyName,
                                     SetupTabScroll *restrict const parent,
                                     const std::string &restrict eventName,
                                     ListModel *restrict const model,
                                     const int width,
                                     const std::string &restrict def,
                                     const MainConfig mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, def, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mModel(model),
    mDropDown(nullptr),
    mWidth(width)
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

    mLabel = new Label(this, mText);
    mLabel->setToolTip(mDescription);
    mDropDown = new DropDown(this, mModel);
    mDropDown->setActionEventId(mEventName);
    mDropDown->addActionListener(mParent);
    mDropDown->setWidth(mWidth);
    mDropDown->setSelected(selectionByValue());

    mWidget = mDropDown;
    if (!mText.empty())
        fixFirstItemSize(mLabel);
    mHorizont->add(mLabel);
    mHorizont->add(mDropDown);

    mParent->getContainer()->add2(mHorizont, true, 4);
    mParent->addControl(this);
    mParent->addActionListener(this);
    mWidget->addActionListener(this);
}

int SetupItemDropDown::selectionByValue()
{
    return atoi(mValue.c_str());
}

void SetupItemDropDown::fromWidget()
{
    if (mDropDown == nullptr)
        return;

    mValue = toString(mDropDown->getSelected());
}

void SetupItemDropDown::toWidget()
{
    if (mDropDown == nullptr)
        return;

    mDropDown->setSelected(selectionByValue());
}


SetupItemDropDownStr::SetupItemDropDownStr(const std::string &restrict text,
                                           const std::string &restrict
                                           description,
                                           const std::string &restrict keyName,
                                           SetupTabScroll *restrict const
                                           parent,
                                           const std::string &restrict
                                           eventName,
                                           ListModel *restrict const model,
                                           const int width,
                                           const MainConfig mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mModel(model),
    mDropDown(nullptr),
    mWidth(width)
{
    mValueType = VSTR;
    createControls();
}

SetupItemDropDownStr::SetupItemDropDownStr(const std::string &restrict text,
                                           const std::string &restrict
                                           description,
                                           const std::string &restrict keyName,
                                           SetupTabScroll *restrict const
                                           parent,
                                           const std::string &restrict
                                           eventName,
                                           ListModel *restrict const model,
                                           const int width,
                                           const std::string &restrict def,
                                           const MainConfig mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, def, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mModel(model),
    mDropDown(nullptr),
    mWidth(width)
{
    mValueType = VSTR;
    createControls();
}

SetupItemDropDownStr::~SetupItemDropDownStr()
{
    mHorizont = nullptr;
    mWidget = nullptr;
    mModel = nullptr;
    mDropDown = nullptr;
    mLabel = nullptr;
}

void SetupItemDropDownStr::createControls()
{
    load();
    mHorizont = new HorizontContainer(this, 32, 2);

    mLabel = new Label(this, mText);
    mLabel->setToolTip(mDescription);
    mDropDown = new DropDown(this, mModel);
    mDropDown->setActionEventId(mEventName);
    mDropDown->addActionListener(mParent);
    mDropDown->setWidth(mWidth);
    mDropDown->setSelected(selectionByValue());

    mWidget = mDropDown;
    if (!mText.empty())
        fixFirstItemSize(mLabel);
    mHorizont->add(mLabel);
    mHorizont->add(mDropDown);

    mParent->getContainer()->add2(mHorizont, true, 4);
    mParent->addControl(this);
    mParent->addActionListener(this);
    mWidget->addActionListener(this);
}

int SetupItemDropDownStr::selectionByValue()
{
    const int sz = mModel->getNumberOfElements();
    for (int f = 0; f < sz; f ++)
    {
        if (mModel->getElementAt(f) == mValue)
        {
            return f;
        }
    }
    return 0;
}

void SetupItemDropDownStr::fromWidget()
{
    if (mDropDown == nullptr)
        return;

    const int sel = mDropDown->getSelected();
    // use first element in model as empty string
    if (sel == 0 || sel >= mModel->getNumberOfElements())
        mValue = "";
    else
        mValue = mModel->getElementAt(sel);
}

void SetupItemDropDownStr::toWidget()
{
    if (mDropDown == nullptr)
        return;

    mDropDown->setSelected(selectionByValue());
}


SetupItemSlider::SetupItemSlider(const std::string &restrict text,
                                 const std::string &restrict description,
                                 const std::string &restrict keyName,
                                 SetupTabScroll *restrict const parent,
                                 const std::string &restrict eventName,
                                 const double min,
                                 const double max,
                                 const double step,
                                 const int width,
                                 const OnTheFly onTheFly,
                                 const MainConfig mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mSlider(nullptr),
    mMin(min),
    mMax(max),
    mStep(step),
    mWidth(width),
    mOnTheFly(onTheFly)
{
    mValueType = VSTR;
    createControls();
}

SetupItemSlider::SetupItemSlider(const std::string &restrict text,
                                 const std::string &restrict description,
                                 const std::string &restrict keyName,
                                 SetupTabScroll *restrict const parent,
                                 const std::string &restrict eventName,
                                 const double min,
                                 const double max,
                                 const double step,
                                 const std::string &restrict def,
                                 const int width,
                                 const OnTheFly onTheFly,
                                 const MainConfig mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, def, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mSlider(nullptr),
    mMin(min),
    mMax(max),
    mStep(step),
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

    mLabel = new Label(this, mText);
    mLabel->setToolTip(mDescription);
    mSlider = new Slider(this, mMin, mMax, mStep);
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
    if (mSlider == nullptr)
        return;

    mValue = toString(mSlider->getValue());
}

void SetupItemSlider::toWidget()
{
    if (mSlider == nullptr)
        return;

    mSlider->setValue(atof(mValue.c_str()));
}

void SetupItemSlider::action(const ActionEvent &event A_UNUSED)
{
    fromWidget();
    if (mOnTheFly == OnTheFly_true)
        save();
}

void SetupItemSlider::apply(const std::string &eventName)
{
    if (eventName != mEventName)
        return;

    fromWidget();
    save();
}


SetupItemSlider2::SetupItemSlider2(const std::string &restrict text,
                                   const std::string &restrict description,
                                   const std::string &restrict keyName,
                                   SetupTabScroll *restrict const parent,
                                   const std::string &restrict eventName,
                                   const int min,
                                   const int max,
                                   const int step,
                                   SetupItemNames *restrict const values,
                                   const OnTheFly onTheFly,
                                   const MainConfig mainConfig,
                                   const DoNotAlign doNotAlign) :
    SetupItem(text, description, keyName, parent, eventName, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mLabel2(nullptr),
    mSlider(nullptr),
    mValues(values),
    mMin(min),
    mMax(max),
    mStep(step),
    mInvertValue(0),
    mInvert(false),
    mOnTheFly(onTheFly),
    mDoNotAlign(doNotAlign)
{
    mValueType = VSTR;
    createControls();
}

SetupItemSlider2::SetupItemSlider2(const std::string &restrict text,
                                   const std::string &restrict description,
                                   const std::string &restrict keyName,
                                   SetupTabScroll *restrict const parent,
                                   const std::string &restrict eventName,
                                   const int min,
                                   const int max,
                                   const int step,
                                   SetupItemNames *restrict const values,
                                   const std::string &restrict def,
                                   const OnTheFly onTheFly,
                                   const MainConfig mainConfig,
                                   const DoNotAlign doNotAlign) :
    SetupItem(text, description, keyName, parent, eventName, def, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mLabel2(nullptr),
    mSlider(nullptr),
    mValues(values),
    mMin(min),
    mMax(max),
    mStep(step),
    mInvertValue(0),
    mInvert(false),
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

    mLabel = new Label(this, mText);
    mLabel->setToolTip(mDescription);
    mLabel2 = new Label(this, "");
    mLabel2->setWidth(width);
    mSlider = new Slider(this, mMin, mMax, mStep);
    mSlider->setActionEventId(mEventName);
    mSlider->addActionListener(mParent);
    mSlider->setValue(atof(mValue.c_str()));
    mSlider->setHeight(30);

    mWidget = mSlider;
    mSlider->setWidth(150);
    mSlider->setHeight(40);
    if (mDoNotAlign == DoNotAlign_false)
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
    if ((mValues == nullptr) || (gui == nullptr))
        return 1;

    int maxWidth = 0;
    SetupItemNamesConstIter it = mValues->begin();
    const SetupItemNamesConstIter it_end = mValues->end();
    const Font *const font = gui->getFont();

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
    if (mSlider == nullptr)
        return;

    int val = roundDouble(mSlider->getValue());
    if (mInvert)
        val = mInvertValue - val;
    mValue = toString(val);
}

void SetupItemSlider2::toWidget()
{
    if (mSlider == nullptr)
        return;

    int val = roundDouble(atof(mValue.c_str()));
    if (mInvert)
        val = mInvertValue - val;
    mSlider->setValue(val);
    updateLabel();
}

void SetupItemSlider2::action(const ActionEvent &event A_UNUSED)
{
    fromWidget();
    updateLabel();
    if (mOnTheFly == OnTheFly_true)
        save();
}

void SetupItemSlider2::updateLabel()
{
    int val = CAST_S32(mSlider->getValue()) - mMin;
    if (val < 0)
    {
        val = 0;
    }
    else
    {
        const int sz = CAST_S32(mValues->size());
        if (val >= sz)
            val = sz - 1;
    }
    std::string str = mValues->at(val);
    mLabel2->setCaption(str);
}

void SetupItemSlider2::apply(const std::string &eventName)
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


SetupItemSliderList::SetupItemSliderList(const std::string &restrict text,
                                         const std::string &restrict
                                         description,
                                         const std::string &restrict keyName,
                                         SetupTabScroll *restrict const parent,
                                         const std::string &restrict eventName,
                                         ListModel *restrict const model,
                                         const int width,
                                         const OnTheFly onTheFly,
                                         const MainConfig mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mSlider(nullptr),
    mModel(model),
    mWidth(width),
    mOnTheFly(onTheFly)
{
    mValueType = VSTR;
}

SetupItemSliderList::SetupItemSliderList(const std::string &restrict text,
                                         const std::string &restrict
                                         description,
                                         const std::string &restrict keyName,
                                         SetupTabScroll *restrict const parent,
                                         const std::string &restrict eventName,
                                         ListModel *restrict const model,
                                         const std::string &restrict def,
                                         const int width,
                                         const OnTheFly onTheFly,
                                         const MainConfig mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, def, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mSlider(nullptr),
    mModel(model),
    mWidth(width),
    mOnTheFly(onTheFly)
{
    mValueType = VSTR;
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

    mLabel = new Label(this, mText);
    mLabel->setToolTip(mDescription);
    mSlider = new SliderList(this, mModel);
    mSlider->postInit2(mParent, mEventName);
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
    if (mSlider == nullptr)
        return;

    mValue = mSlider->getSelectedString();
}

void SetupItemSliderList::toWidget()
{
    if (mSlider == nullptr)
        return;

    mSlider->setSelectedString(mValue);
}

void SetupItemSliderList::action(const ActionEvent &event A_UNUSED)
{
    fromWidget();
    if (mOnTheFly == OnTheFly_true)
        save();
}

void SetupItemSliderList::apply(const std::string &eventName)
{
    if (eventName != mEventName)
        return;

    fromWidget();
    save();
}

SetupItemSound::SetupItemSound(const std::string &restrict text,
                               const std::string &restrict description,
                               const std::string &restrict keyName,
                               SetupTabScroll *restrict const parent,
                               const std::string &restrict eventName,
                               ListModel *restrict const model,
                               const int width,
                               const OnTheFly onTheFly,
                               const MainConfig mainConfig) :
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
    if (mHorizont != nullptr)
        mHorizont->add(mButton);
}

void SetupItemSound::action(const ActionEvent &event)
{
    if (event.getId() == mEventName + "_PLAY")
    {
        if ((mSlider != nullptr) && (mSlider->getSelected() != 0))
        {
            soundManager.playGuiSfx(pathJoin(branding.getStringValue(
                "systemsounds"),
                mSlider->getSelectedString()).append(".ogg"));
        }
    }
    else
    {
        SetupItemSliderList::action(event);
    }
}

SetupItemSliderInt::SetupItemSliderInt(const std::string &restrict text,
                                       const std::string &restrict description,
                                       const std::string &restrict keyName,
                                       SetupTabScroll *restrict const parent,
                                       const std::string &restrict eventName,
                                       ListModel *restrict const model,
                                       const int min,
                                       const int width,
                                       const OnTheFly onTheFly,
                                       const MainConfig mainConfig) :
    SetupItemSliderList(text, description, keyName, parent, eventName,
                        model, width, onTheFly, mainConfig),
    mMin(min)
{
    createControls();
}

void SetupItemSliderInt::addMoreControls()
{
    toWidget();
}

void SetupItemSliderInt::fromWidget()
{
    if (mSlider == nullptr)
        return;

    mValue = toString(mSlider->getSelected() + mMin);
}

void SetupItemSliderInt::toWidget()
{
    if (mSlider == nullptr)
        return;

    mSlider->setSelected(atoi(mValue.c_str()) - mMin);
}
