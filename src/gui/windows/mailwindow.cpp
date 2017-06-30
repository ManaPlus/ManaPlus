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

#include "gui/windows/mailwindow.h"

#include "enums/gui/layouttype.h"

#include "net/mailhandler.h"

#include "gui/models/extendednamesmodel.h"

#include "gui/mailmessage.h"

#include "gui/windows/maileditwindow.h"
#include "gui/windows/mailviewwindow.h"
#include "gui/windows/setupwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/extendedlistbox.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/scrollarea.h"

#include "utils/delete2.h"
#include "utils/dtor.h"
#include "utils/foreach.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

MailWindow *mailWindow = nullptr;

MailWindow::MailWindow() :
    // TRANSLATORS: mail window name
    Window(_("Mail"), Modal_false, nullptr, "mail.xml"),
    ActionListener(),
    mMessages(),
    mMessagesMap(),
    mMailModel(new ExtendedNamesModel),
    mListBox(CREATEWIDGETR(ExtendedListBox,
        this, mMailModel, "extendedlistbox.xml")),
    mListScrollArea(new ScrollArea(this, mListBox,
        fromBool(getOptionBool("showlistbackground"), Opaque),
        "mail_listbackground.xml")),
    // TRANSLATORS: mail window button
    mRefreshButton(new Button(this, _("Refresh"), "refresh", this)),
    // TRANSLATORS: mail window button
    mNewButton(new Button(this, _("New"), "new", this)),
    // TRANSLATORS: mail window button
    mDeleteButton(new Button(this, _("Delete"), "delete", this)),
    // TRANSLATORS: mail window button
    mReturnButton(new Button(this, _("Return"), "return", this)),
    // TRANSLATORS: mail window button
    mOpenButton(new Button(this, _("Open"), "open", this))
{
    setWindowName("Mail");
    setCloseButton(true);
    setResizable(true);
    setCloseButton(true);
    setSaveVisible(true);
    setStickyButtonLock(true);

    if (setupWindow != nullptr)
        setupWindow->registerWindowForReset(this);

    setDefaultSize(310, 180, ImagePosition::CENTER);
    setMinWidth(310);
    setMinHeight(250);
    center();

    mListScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    placer(0, 0, mListScrollArea, 4, 5).setPadding(3);
    placer(4, 0, mRefreshButton);
    placer(4, 1, mOpenButton);
    placer(4, 2, mNewButton);
    placer(4, 3, mDeleteButton);
    placer(4, 4, mReturnButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, LayoutType::SET);

    loadWindowState();
    enableVisibleSound(true);
}

MailWindow::~MailWindow()
{
    delete_all(mMessages);
    delete2(mMailModel);
    delete2(mailViewWindow);
    delete2(mailEditWindow);
}

void MailWindow::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "refresh")
    {
        mailHandler->refresh();
    }
    else if (eventId == "new")
    {
        if (mailEditWindow == nullptr)
        {
            CREATEWIDGETV0(mailEditWindow, MailEditWindow);
        }
    }
    else if (eventId == "open")
    {
        const int sel = mListBox->getSelected();
        if (sel < 0)
            return;
        const MailMessage *const mail = mMessages[sel];
        mailHandler->readMessage(mail->id);
    }
    else if (eventId == "delete")
    {
        const int sel = mListBox->getSelected();
        if (sel < 0)
            return;
        const MailMessage *const mail = mMessages[sel];
        mailHandler->deleteMessage(mail->id);
    }
    else if (eventId == "return")
    {
        const int sel = mListBox->getSelected();
        if (sel < 0)
            return;
        const MailMessage *const mail = mMessages[sel];
        mailHandler->returnMessage(mail->id);
    }
}

void MailWindow::clear()
{
    delete_all(mMessages);
    mMessages.clear();
    mMessagesMap.clear();
    mMailModel->clear();
    mListBox->setSelected(-1);
}

void MailWindow::addMail(MailMessage *const message)
{
    if (message == nullptr)
        return;
    mMessages.push_back(message);
    mMailModel->add(strprintf("%s %s",
        message->unread ? " " : "U",
        message->title.c_str()));
    mMessagesMap[message->id] = message;
}

void MailWindow::removeMail(const int id)
{
    std::map<int, MailMessage*>::iterator it1 = mMessagesMap.find(id);
    if (it1 != mMessagesMap.end())
        mMessagesMap.erase(it1);

    mMailModel->clear();

    FOR_EACH (std::vector<MailMessage*>::iterator, it, mMessages)
    {
        MailMessage *message = *it;
        if ((message != nullptr) && message->id == id)
        {
            mMessages.erase(it);
            delete message;
            break;
        }
    }

    FOR_EACH (std::vector<MailMessage*>::iterator, it, mMessages)
    {
        MailMessage *message = *it;
        if (message != nullptr)
        {
            mMailModel->add(strprintf("%s %s",
                message->unread ? " " : "U",
                message->title.c_str()));
        }
    }
}

void MailWindow::showMessage(MailMessage *const mail)
{
    if (mail == nullptr)
        return;
    const std::map<int, MailMessage*>::const_iterator
        it = mMessagesMap.find(mail->id);
    if (it != mMessagesMap.end())
    {
        const MailMessage *const mail2 = (*it).second;
        mail->time = mail2->time;
        mail->strTime = mail2->strTime;
    }
    delete mailViewWindow;
    CREATEWIDGETV(mailViewWindow, MailViewWindow, mail);
}

void MailWindow::viewNext(const int id)
{
    FOR_EACH (std::vector<MailMessage*>::iterator, it, mMessages)
    {
        MailMessage *message = *it;
        if ((message != nullptr) && message->id == id)
        {
            ++ it;
            if (it == mMessages.end())
            {
                it = mMessages.begin();
                mListBox->setSelected(0);
            }
            else
            {
                mListBox->setSelected(mListBox->getSelected() + 1);
            }
            message = *it;
            mailHandler->readMessage(message->id);
            return;
        }
    }
}

void MailWindow::viewPrev(const int id)
{
    FOR_EACH (std::vector<MailMessage*>::iterator, it, mMessages)
    {
        MailMessage *message = *it;
        if ((message != nullptr) && message->id == id)
        {
            if (it == mMessages.begin())
            {
                it = mMessages.end();
                mListBox->setSelected(CAST_S32(mMessages.size()) - 1);
            }
            else
            {
                mListBox->setSelected(mListBox->getSelected() - 1);
            }
            -- it;
            message = *it;
            mailHandler->readMessage(message->id);
            return;
        }
    }
}

void MailWindow::mouseClicked(MouseEvent &event)
{
    if (event.getButton() == MouseButton::LEFT)
    {
        event.consume();
        if (event.getClickCount() == 2 &&
            event.getSource() == mListBox)
        {
            action(ActionEvent(mListBox, "open"));
        }
    }
}

void MailWindow::postConnection()
{
    mailHandler->refresh();
}

void MailWindow::createMail(const std::string &to)
{
    if (mailEditWindow != nullptr)
        return;

    CREATEWIDGETV0(mailEditWindow, MailEditWindow);
    mailEditWindow->setTo(to);
}
