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

#include "settings.h"

#include "enums/gui/layouttype.h"

#include "net/mail2handler.h"
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
#include "utils/stdmove.h"
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
        this, mMailModel, "extendedlistbox.xml", 0)),
    mListScrollArea(new ScrollArea(this, mListBox,
        fromBool(getOptionBool("showlistbackground"), Opaque),
        "mail_listbackground.xml")),
    // TRANSLATORS: mail window button
    mRefreshButton(new Button(this, _("Refresh"), "refresh", this)),
    // TRANSLATORS: mail window button
    mNewButton(new Button(this, _("New"), "new", this)),
    // TRANSLATORS: mail window button
    mDeleteButton(new Button(this, _("Delete"), "delete", this)),
    mReturnButton(new Button(this,
        // TRANSLATORS: mail window button
        settings.enableNewMailSystem ? _("Get old") : _("Return"),
        "return", this)),
    // TRANSLATORS: mail window button
    mOpenButton(new Button(this, _("Open"), "open", this)),
    mOpenType(MailOpenType::Mail),
    mUseMail2(settings.enableNewMailSystem),
    mLastPage(false)
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
        refreshMails();
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
        if (mUseMail2)
            mail2Handler->readMail(mOpenType, mail->id);
        else
            mailHandler->readMessage(CAST_S32(mail->id));
    }
    else if (eventId == "delete")
    {
        const int sel = mListBox->getSelected();
        if (sel < 0)
            return;
        const MailMessage *const mail = mMessages[sel];
        if (mUseMail2)
            mail2Handler->deleteMail(mOpenType, mail->id);
        else
            mailHandler->deleteMessage(CAST_S32(mail->id));
    }
    else if (eventId == "return")
    {
        if (mUseMail2)
        {
            const size_t idx = mMessages.size();
            if (idx == 0)
                mail2Handler->refreshMailList(MailOpenType::Mail, 0);
            else
                mail2Handler->nextPage(mOpenType, mMessages[idx - 1]->id);
        }
        else
        {
            const int sel = mListBox->getSelected();
            if (sel < 0)
                return;
            const MailMessage *const mail = mMessages[sel];
            mailHandler->returnMessage(CAST_S32(mail->id));
        }
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

std::string MailWindow::getMailHeader(const MailMessage *const message) const
{
    if (mUseMail2)
    {
        std::string header;
        if (message->read)
            header.append(" ");
        else
            header.append("U");
        const MailMessageType::Type type = message->type;
        if ((type & MailMessageType::Money) != 0)
            header.append("M");
        else
            header.append(" ");
        if ((type & MailMessageType::Item) != 0)
            header.append("I");
        else
            header.append(" ");
        header.append(" ").append(message->title);
        return STD_MOVE(header);
    }
    return strprintf("%s %s",
        message->read ? " " : "U",
        message->title.c_str());
}

void MailWindow::addMail(MailMessage *const message)
{
    if (message == nullptr)
        return;
    mMessages.push_back(message);
    mMailModel->add(getMailHeader(message));
    mMessagesMap[message->id] = message;
}

void MailWindow::removeMail(const int64_t id)
{
    std::map<int64_t, MailMessage*>::iterator it1 = mMessagesMap.find(id);
    if (it1 != mMessagesMap.end())
        mMessagesMap.erase(it1);

    mMailModel->clear();

    FOR_EACH (STD_VECTOR<MailMessage*>::iterator, it, mMessages)
    {
        MailMessage *message = *it;
        if ((message != nullptr) && message->id == id)
        {
            mMessages.erase(it);
            delete message;
            break;
        }
    }

    FOR_EACH (STD_VECTOR<MailMessage*>::iterator, it, mMessages)
    {
        MailMessage *message = *it;
        if (message != nullptr)
            mMailModel->add(getMailHeader(message));
    }
}

void MailWindow::showMessage(MailMessage *const mail,
                             const int itemsCount)
{
    if (mail == nullptr)
        return;
    const std::map<int64_t, MailMessage*>::const_iterator
        it = mMessagesMap.find(mail->id);
    if (it != mMessagesMap.end())
    {
        const MailMessage *const mail2 = (*it).second;
        mail->time = mail2->time;
        mail->strTime = mail2->strTime;
    }
    delete mailViewWindow;
    CREATEWIDGETV(mailViewWindow, MailViewWindow, mail,
        itemsCount);
}

void MailWindow::viewNext(const int64_t id)
{
    FOR_EACH (STD_VECTOR<MailMessage*>::iterator, it, mMessages)
    {
        MailMessage *message = *it;
        if (message != nullptr &&
            message->id == id)
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
            if (mUseMail2)
                mail2Handler->readMail(mOpenType, message->id);
            else
                mailHandler->readMessage(CAST_S32(message->id));
            return;
        }
    }
}

void MailWindow::viewPrev(const int64_t id)
{
    FOR_EACH (STD_VECTOR<MailMessage*>::iterator, it, mMessages)
    {
        MailMessage *message = *it;
        if (message != nullptr &&
            message->id == id)
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
            if (mUseMail2)
                mail2Handler->readMail(mOpenType, message->id);
            else
                mailHandler->readMessage(CAST_S32(message->id));
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
    refreshMails();
}

void MailWindow::refreshMails()
{
    if (mUseMail2)
    {
        clear();
        mail2Handler->refreshMailList(MailOpenType::Mail, 0);
        mLastPage = false;
        mReturnButton->setEnabled(true);
    }
    else
    {
        mailHandler->refresh();
    }
}

void MailWindow::createMail(const std::string &to)
{
    if (mailEditWindow != nullptr)
        return;

    CREATEWIDGETV0(mailEditWindow, MailEditWindow);
    mailEditWindow->setTo(to);
}

MailMessage *MailWindow::findMail(const int64_t id)
{
    std::map<int64_t, MailMessage*>::iterator it = mMessagesMap.find(id);
    if (it != mMessagesMap.end())
        return (*it).second;
    return nullptr;
}

void MailWindow::setLastPage()
{
    mLastPage = true;
    mReturnButton->setEnabled(false);
}

void MailWindow::refreshMailNames()
{
    mMailModel->clear();
    FOR_EACH (STD_VECTOR<MailMessage*>::iterator, it, mMessages)
    {
        MailMessage *message = *it;
        if (message != nullptr)
            mMailModel->add(getMailHeader(message));
    }
}
