/*
 *  Retrieve string pasted depending on OS mechanisms.
 *  Copyright (C) 2001-2010  Wormux Team
 *  Copyright (C) 2011  ManaPlus Developers
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

/*
 *  IMPORTANT!
 *
 *  This code was taken from Wormux svn trunk at Feb 25 2010. Please don't
 *  make any unnecessary modifications, and try to sync up modifications
 *  when possible.
 */

#ifdef _MSC_VER
#  include "msvc/config.h"
#elif defined(HAVE_CONFIG_H)
#  include "config.h"
#endif

#include <SDL_syswm.h>
#include "copynpaste.h"

#include "debug.h"

#ifdef WIN32
bool retrieveBuffer(std::string& text, std::string::size_type& pos)
{
    bool  ret = false;

    if (!OpenClipboard(NULL))
        return false;

    HANDLE  h = GetClipboardData(CF_UNICODETEXT);
    if (h)
    {
        LPCWSTR data = (LPCWSTR)GlobalLock(h);

        if (data)
        {
            int len = WideCharToMultiByte(CP_UTF8, 0, data, -1,
                NULL, 0, NULL, NULL);
            if (len > 0)
            {
                // Convert from UTF-16 to UTF-8
                void *temp = calloc(len, 1);
                if (WideCharToMultiByte(CP_UTF8, 0, data, -1,
                    (LPSTR)temp, len, NULL, NULL))
                {
                    text.insert(pos, (char*)temp);
                    pos += len-1;
                }
                free(temp);
                ret = true;
            }
        }
        GlobalUnlock(h);
    }
    else
    {
        h = GetClipboardData(CF_TEXT);

        if (h)
        {
            const char *data = (char*)GlobalLock(h);
            if (data)
            {
                text.insert(pos, data);
                pos += strlen(data);
                ret = true;
            }
            GlobalUnlock(h);
        }
    }

    CloseClipboard();
    return ret;
}

bool sendBuffer(std::string& text)
{
    int wCharsLen = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, NULL, 0);
    if (!wCharsLen)
        return false;

    HANDLE h;
    WCHAR *out;
    h = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, wCharsLen * sizeof(WCHAR));
    out = (WCHAR*)GlobalLock(h);

    MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, out, wCharsLen);

    if (!OpenClipboard(0))
    {
        GlobalUnlock(h);
        GlobalFree(h);
        return false;
    }
    GlobalUnlock(h);
    EmptyClipboard();
    if (!SetClipboardData(CF_UNICODETEXT, out))
    {
        GlobalFree(h);
        CloseClipboard();
        return false;
    }
    GlobalFree(h);
    CloseClipboard();

    return true;
}

#elif defined(__APPLE__)

#ifdef Status
#undef Status
#endif

#include <Carbon/Carbon.h>

// Sorry for the very long code, all nicer OS X APIs are coded in
// Objective C and not C!
// Also it does very thorough error handling
bool getDataFromPasteboard(PasteboardRef inPasteboard,
                           char* flavorText /* out */,
                           const int bufSize)
{
    OSStatus            err = noErr;
    PasteboardSyncFlags syncFlags;
    ItemCount           itemCount;

    syncFlags = PasteboardSynchronize(inPasteboard);

    //require_action( syncFlags & kPasteboardModified, PasteboardOutOfSync,
    //               err = badPasteboardSyncErr );

    err = PasteboardGetItemCount(inPasteboard, &itemCount);
    require_noerr(err, CantGetPasteboardItemCount);

    for (UInt32 itemIndex = 1; itemIndex <= itemCount; itemIndex ++)
    {
        PasteboardItemID    itemID;
        CFArrayRef          flavorTypeArray;
        CFIndex             flavorCount;

        err = PasteboardGetItemIdentifier(inPasteboard, itemIndex, &itemID);
        require_noerr(err, CantGetPasteboardItemIdentifier);

        err = PasteboardCopyItemFlavors(inPasteboard,
            itemID, &flavorTypeArray);
        require_noerr(err, CantCopyPasteboardItemFlavors);

        flavorCount = CFArrayGetCount(flavorTypeArray);

        for (CFIndex flavorIndex = 0; flavorIndex < flavorCount;
             flavorIndex ++)
        {
            CFStringRef             flavorType;
            CFDataRef               flavorData;
            CFIndex                 flavorDataSize;
            flavorType = (CFStringRef)CFArrayGetValueAtIndex(
                flavorTypeArray, flavorIndex);

            // we're only interested by text...
            if (UTTypeConformsTo(flavorType, CFSTR("public.utf8-plain-text")))
            {
                err = PasteboardCopyItemFlavorData(inPasteboard, itemID,
                                                   flavorType, &flavorData);
                require_noerr(err, CantCopyFlavorData);
                flavorDataSize = CFDataGetLength(flavorData);
                flavorDataSize = (flavorDataSize<254) ? flavorDataSize : 254;

                if (flavorDataSize + 2 > bufSize)
                {
                    fprintf(stderr,
                        "Cannot copy clipboard, contents is too big!\n");
                    return false;
                }

                for (short dataIndex = 0; dataIndex <= flavorDataSize;
                     dataIndex ++)
                {
                    char byte = *(CFDataGetBytePtr(flavorData) + dataIndex);
                    flavorText[dataIndex] = byte;
                }

                flavorText[flavorDataSize] = '\0';
                flavorText[flavorDataSize + 1] = '\n';

                CFRelease (flavorData);
                return true;
            }

            continue;
CantCopyFlavorData:
            fprintf(stderr, "Cannot copy clipboard, CantCopyFlavorData!\n");
        }

        CFRelease (flavorTypeArray);
        continue;

CantCopyPasteboardItemFlavors:
        fprintf(stderr,
            "Cannot copy clipboard, CantCopyPasteboardItemFlavors!\n");
        continue;
CantGetPasteboardItemIdentifier:
        fprintf(stderr,
            "Cannot copy clipboard, CantGetPasteboardItemIdentifier!\n");
        continue;
    }
    fprintf(stderr,
        "Cannot copy clipboard, found no acceptable flavour!\n");
    return false;

CantGetPasteboardItemCount:
    fprintf(stderr, "Cannot copy clipboard, CantGetPasteboardItemCount!\n");
    return false;
//PasteboardOutOfSync:
//    fprintf(stderr, "Cannot copy clipboard, PasteboardOutOfSync!\n");
//      return false;
}

bool getClipBoard(char* text /* out */, const int bufSize)
{
    OSStatus err = noErr;

    PasteboardRef theClipboard;
    err = PasteboardCreate(kPasteboardClipboard, &theClipboard);
    require_noerr(err, PasteboardCreateFailed);

    if (!getDataFromPasteboard(theClipboard, text, bufSize))
    {
        fprintf(stderr,
            "Cannot copy clipboard, getDataFromPasteboardFailed!\n");
        return false;
    }

    CFRelease(theClipboard);

    return true;

    // ---- error handling
PasteboardCreateFailed:
    fprintf(stderr, "Cannot copy clipboard, PasteboardCreateFailed!\n");
    CFRelease(theClipboard);
    return false;
}

bool retrieveBuffer(std::string& text, std::string::size_type& pos)
{
    const int bufSize = 512;
    char buffer[bufSize + 1];

    if (getClipBoard(buffer, bufSize))
    {
        text = buffer;
        pos += strlen(buffer);
        return true;
    }
    else
    {
        return false;
    }
}

bool sendBuffer(std::string& text)
{
    return false;
}

#elif USE_X11

#include <unistd.h>

static char* getSelection2(Display *dpy, Window us, Atom selection,
                           Atom request_target)
{
    int max_events = 50;
    Window owner = XGetSelectionOwner(dpy, selection);
    int ret;

    //printf("XConvertSelection on %s\n", XGetAtomName(dpy, selection));
    if (owner == None)
    {
        //printf("No owner\n");
        return NULL;
    }
    XConvertSelection(dpy, selection, request_target,
        XA_PRIMARY, us, CurrentTime);
    XFlush(dpy);

    while (max_events --)
    {
        XEvent e;

        XNextEvent(dpy, &e);
        if (e.type == SelectionNotify)
        {
//            printf("Received %s, %s, %s\n", XGetAtomName(dpy, selection),
//                XGetAtomName(dpy, e.xselection.selection),
//                XGetAtomName(dpy, request_target));
            if (e.xselection.property == None)
            {
                //printf("Couldn't convert\n");
                return NULL;
            }

            long unsigned len, left, dummy;
            int format;
            Atom type;
            unsigned char *data = NULL;

            ret = XGetWindowProperty(dpy, us, e.xselection.property, 0, 0,
                False, AnyPropertyType, &type, &format, &len, &left, &data);
            if (left < 1)
            {
                if (ret == Success)
                XFree(data);
                return NULL;
            }

            ret = XGetWindowProperty(dpy, us, e.xselection.property, 0,
                left, False, AnyPropertyType, &type, &format, &len,
                &dummy, &data);

            if (ret != Success)
            {
                //printf("Failed to get property: %p on %lu\n", data, len);
                return NULL;
            }

            //printf(">>>  Got %s: len=%lu left=%lu (event %i)\n", data,
            //    len, left, 50-max_events);
            return reinterpret_cast<char*>(data);
        }
    }
    return NULL;
}

static Atom requestAtom;

static char* getSelection(Display *dpy, Window us, Atom selection)
{
    char *data = NULL;
    if (requestAtom != None)
        data = getSelection2(dpy, us, selection, requestAtom);
    if (!data)
        data = getSelection2(dpy, us, selection, XA_STRING);
    return data;
}

bool retrieveBuffer(std::string& text, std::string::size_type& pos)
{
    SDL_SysWMinfo info;

    //printf("Retrieving buffer...\n");
    SDL_VERSION(&info.version);
    if (SDL_GetWMInfo(&info))
    {
        Display *dpy = info.info.x11.display;
        Window us = info.info.x11.window;
        char *data = NULL;

        requestAtom = XInternAtom (dpy, "UTF8_STRING", true);

        if (!data)
            data = getSelection(dpy, us, XA_PRIMARY);

        if (!data)
            data = getSelection(dpy, us, XA_SECONDARY);
        if (!data)
        {
            Atom XA_CLIPBOARD = XInternAtom(dpy, "CLIPBOARD", 0);
            if (XA_CLIPBOARD != None)
                data = getSelection(dpy, us, XA_CLIPBOARD);
        }

        if (data)
        {
            // check cursor position
            if (pos > text.size())
                pos = text.size();

            text.insert(pos, data);
            pos += strlen(data);
            XFree(data);

            return true;
        }
    }
    return false;
}

bool sendBuffer(std::string& text)
{
    pid_t pid;
    int fd[2];

    if (pipe(fd))
        return false;

    if ((pid = fork()) == -1)
    {   // fork error
        return false;
    }
    else if (!pid)
    {   // child
        close(fd[1]);

        if (fd[0] != STDIN_FILENO)
        {
            if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
            {
                close(fd[0]);
                exit(1);
            }
            close(fd[0]);
        }
        execl("/usr/bin/xsel", "xsel", "-i", (char *)0);
        exit(1);
    }

    // parent
    close(fd[0]);
    const int len = strlen(text.c_str());
    if (write(fd[1], text.c_str(), len) != len)
    {
        close(fd[1]);
        return false;
    }
    close(fd[1]);
    return true;
}

#else
bool retrieveBuffer(std::string&, std::string::size_type&)
{
    return false;
}

bool sendBuffer(std::string& text)
{
    return false;
}
#endif
