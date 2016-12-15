/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_SKILLRECTANGLELISTBOX_H
#define GUI_WIDGETS_SKILLRECTANGLELISTBOX_H

#include "const/resources/skill.h"

#include "dragdrop.h"
#include "settings.h"

#include "gui/skin.h"
#include "gui/viewport.h"

#include "gui/widgets/widget.h"

#include "gui/fonts/font.h"

#include "gui/models/skillmodel.h"

#include "gui/popups/popupmenu.h"
#include "gui/popups/skillpopup.h"

#include "utils/delete2.h"
#include "utils/stringutils.h"

#include "render/graphics.h"

#include "localconsts.h"

class SkillModel;

class SkillRectangleListBox final : public Widget,
                                    public MouseListener
{
    public:
        SkillRectangleListBox(const Widget2 *const widget,
                              SkillModel *const model) :
            Widget(widget),
            MouseListener(),
            mHighlightColor(getThemeColor(ThemeColorId::HIGHLIGHT)),
            mTextColor(getThemeColor(ThemeColorId::TEXT)),
            mTextColor2(getThemeColor(ThemeColorId::TEXT_OUTLINE)),
            mCooldownColor(getThemeColor(ThemeColorId::SKILL_COOLDOWN)),
            mForegroundSelectedColor(getThemeColor(
                ThemeColorId::LISTBOX_SELECTED)),
            mForegroundSelectedColor2(getThemeColor(
                ThemeColorId::LISTBOX_SELECTED_OUTLINE)),
            mModel(model),
            mSkin(nullptr),
            mSelected(-1),
            mPadding(2),
            mBoxWidth(80),
            mBoxHeight(70),
            mIconXOffset(24),
            mIconYOffset(10),
            mTextXOffset(0),
            mTextYOffset(44),
            mSkillClicked(false)
        {
            if (theme)
            {
                mSkin = theme->load("skillrectanglelistbox.xml",
                    "listbox.xml");
            }

            if (mSkin)
            {
                mPadding = mSkin->getPadding();
                mBoxWidth = mSkin->getOption("boxWidth", 80);
                mBoxHeight = mSkin->getOption("boxHeight", 70);
                mIconXOffset = mSkin->getOption("iconXOffset", 24);
                mIconYOffset = mSkin->getOption("iconYOffset", 10);
                mTextXOffset = mSkin->getOption("textXOffset", 0);
                mTextYOffset = mSkin->getOption("textYOffset", 44);
            }
            Font *const font = getFont();
            int minWidth = font->getWidth("Lvl: 10/10") + mTextXOffset + 2;
            int minHeight = font->getHeight() + mTextYOffset + 2;
            if (mBoxWidth < minWidth)
                mBoxWidth = minWidth;
            if (mBoxHeight < minHeight)
                mBoxHeight = minHeight;
            int maxX = 0;
            int maxY = 0;
            for (int i = 0;
                 i < model->getNumberOfElements();
                 ++i)
            {
                SkillInfo *const e = model->getSkillAt(i);
                if (e)
                {
                    if (e->x > maxX)
                        maxX = e->x;
                    if (e->y > maxY)
                        maxY = e->y;
                }
            }
            maxX ++;
            maxY ++;
            setWidth(maxX * mBoxWidth);
            setHeight(maxY * mBoxHeight);
            addMouseListener(this);
        }

        A_DELETE_COPY(SkillRectangleListBox)

        ~SkillRectangleListBox()
        {
            delete2(mModel)
        }

        SkillInfo *getSelectedInfo() const
        {
            if (!mModel)
                return nullptr;
            const int selected = mSelected;
            if (selected < 0 ||
                selected > mModel->getNumberOfElements())
            {
                return nullptr;
            }

            return mModel->getSkillAt(selected);
        }

        void draw(Graphics *const graphics) override final A_NONNULL(2)
        {
            if (!mModel)
                return;

            SkillModel *const model = mModel;
            updateAlpha();

            int maxX = 0;
            int maxY = 0;
            mHighlightColor.a = CAST_S32(mAlpha * 255.0F);
            graphics->setColor(mHighlightColor);
            Font *const font = getFont();
            if (mSelected >= 0)
            {
                SkillInfo *const e = model->getSkillAt(mSelected);
                if (e)
                {
                    //const SkillData *const data = e->data;
                    const int x = e->x * mBoxWidth + mPadding;
                    const int y = e->y * mBoxHeight + mPadding;

                    graphics->fillRectangle(Rect(x, y,
                        mBoxWidth, mBoxHeight));

                    const int xOffset = (mBoxWidth -
                        font->getWidth(e->skillLevel)) / 2;
                    font->drawString(graphics,
                        mForegroundSelectedColor,
                        mForegroundSelectedColor,
                        e->skillLevel,
                        x + mTextXOffset + xOffset,
                        y + mTextYOffset);
                }
            }

            // +++ need split drawing icons and text
            for (int i = 0;
                 i < model->getNumberOfElements();
                 ++i)
            {
                SkillInfo *const e = model->getSkillAt(i);
                if (e)
                {
                    if (e->x > maxX)
                        maxX = e->x;
                    if (e->y > maxY)
                        maxY = e->y;
                    const SkillData *const data = e->data;
                    const int x = e->x * mBoxWidth + mPadding;
                    const int y = e->y * mBoxHeight + mPadding;

                    graphics->drawImage(data->icon,
                        x + mIconXOffset,
                        y + mIconYOffset);

                    if (i != mSelected)
                    {
                        const int width1 = font->getWidth(e->skillLevel);
                        const int xOffset = (mBoxWidth -
                            width1) / 2;
                        font->drawString(graphics,
                            mTextColor,
                            mTextColor2,
                            e->skillLevel,
                            x + mTextXOffset + xOffset,
                            y + mTextYOffset);
                        if (e->skillLevelWidth < 0)
                        {
                            // Add one for padding
                            e->skillLevelWidth = width1 + 1;
                        }
                    }
                    else
                    {
                        if (e->skillLevelWidth < 0)
                        {
                            // Add one for padding
                            e->skillLevelWidth = font->getWidth(e->skillLevel) + 1;
                        }
                    }
                }
            }
            maxX ++;
            maxY ++;
            setWidth(maxX * mBoxWidth);
            setHeight(maxY * mBoxHeight);
        }

        void safeDraw(Graphics *const graphics) override final A_NONNULL(2)
        {
            SkillRectangleListBox::draw(graphics);
        }

        const SkillInfo *getSkillByEvent(const MouseEvent &event) const
        {
            if (!mModel)
                return nullptr;
            const int posX = (event.getX() - mPadding) / mBoxWidth;
            const int posY = (event.getY() - mPadding) / mBoxHeight;
            for (int i = 0;
                 i < mModel->getNumberOfElements();
                 ++i)
            {
                SkillInfo *const e = mModel->getSkillAt(i);
                if (e)
                {
                    if (posX == e->x && posY == e->y)
                        return e;
                }
            }
            return nullptr;
        }

        int getSelectionByEvent(const MouseEvent &event) const
        {
            if (!mModel)
                return -1;
            const int posX = (event.getX() - mPadding) / mBoxWidth;
            const int posY = (event.getY() - mPadding) / mBoxHeight;
            for (int i = 0;
                 i < mModel->getNumberOfElements();
                 ++i)
            {
                SkillInfo *const e = mModel->getSkillAt(i);
                if (e)
                {
                    if (posX == e->x && posY == e->y)
                        return i;
                }
            }
            return -1;
        }

        void mouseMoved(MouseEvent &event) override final
        {
            if (!viewport || !dragDrop.isEmpty())
                return;

            const SkillInfo *const skill = getSkillByEvent(event);
            if (!skill)
                return;
            skillPopup->show(skill,
                skill->customSelectedLevel,
                skill->customCastType,
                skill->customOffsetX,
                skill->customOffsetY);
            skillPopup->position(viewport->mMouseX,
                viewport->mMouseY);
        }

        void mouseDragged(MouseEvent &event) override final
        {
            if (event.getButton() != MouseButton::LEFT)
                return;
            setSelected(std::max(0, getSelectionByEvent(event)));

            if (dragDrop.isEmpty())
            {
                if (mSkillClicked)
                {
                    mSkillClicked = false;
                    const SkillInfo *const skill = getSkillByEvent(event);
                    if (!skill)
                        return;
                    dragDrop.dragSkill(skill, DragDropSource::Skills);
                    dragDrop.setItem(skill->id + SKILL_MIN_ID);
                    dragDrop.setItemData(strprintf("%d %d %d",
                        CAST_S32(skill->customCastType),
                        skill->customOffsetX,
                        skill->customOffsetY));
                }
            }
        }

        void mousePressed(MouseEvent &event) override final
        {
            const MouseButtonT button = event.getButton();
            if (button == MouseButton::LEFT ||
                button == MouseButton::RIGHT)
            {
                const SkillInfo *const skill = getSkillByEvent(event);
                if (!skill)
                    return;
                event.consume();
                mSkillClicked = true;
                SkillModel *const model = mModel;
                if (model &&
                    mSelected >= 0 &&
                    model->getSkillAt(mSelected) == skill)
                {
                    skillPopup->hide();
                    if (button == MouseButton::LEFT &&
                        event.getX() >
                        getWidth() - mPadding - skill->skillLevelWidth)
                    {
                        popupMenu->showSkillLevelPopup(skill);
                    }
                    else if (button == MouseButton::RIGHT)
                    {
                        popupMenu->showSkillPopup(skill);
                    }
                }
            }
        }

        void mouseReleased(MouseEvent &event) override final
        {
            if (event.getButton() == MouseButton::LEFT)
            {
                setSelected(std::max(0, getSelectionByEvent(event)));
                distributeActionEvent();
            }
        }

        void mouseExited(MouseEvent &event A_UNUSED) override final
        {
            skillPopup->hide();
        }

        void updateAlpha()
        {
            const float alpha = std::max(settings.guiAlpha,
                theme->getMinimumOpacity());

            if (mAlpha != alpha)
                mAlpha = alpha;
        }

        void setSelected(const int selected)
        {
            if (!mModel)
            {
                mSelected = -1;
            }
            else
            {
                if (selected < 0)
                    mSelected = -1;
                else if (selected >= mModel->getNumberOfElements())
                    mSelected = mModel->getNumberOfElements() - 1;
                else
                    mSelected = selected;
            }
        }

    private:
        Color mHighlightColor;
        Color mTextColor;
        Color mTextColor2;
        Color mCooldownColor;
        Color mForegroundSelectedColor;
        Color mForegroundSelectedColor2;
        SkillModel *mModel;
        Skin *mSkin;
        int mSelected;
        int mPadding;
        int mBoxWidth;
        int mBoxHeight;
        int mIconXOffset;
        int mIconYOffset;
        int mTextXOffset;
        int mTextYOffset;
        bool mSkillClicked;
        static float mAlpha;
};

float SkillRectangleListBox::mAlpha = 1.0;

#endif  // GUI_WIDGETS_SKILLRECTANGLELISTBOX_H
