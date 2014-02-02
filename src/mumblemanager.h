/*
 *  Code taken from: http://mumble.sourceforge.net/Link
 *
 *  All code listed below is in the public domain and can be used, shared or
 *  modified freely
 *
 *  Copyright (C) 2011-2014  The ManaPlus Developers
 */

#ifndef MUMBLEMANAGER_H
#define MUMBLEMANAGER_H

#include "main.h"

#ifdef USE_MUMBLE

#include <string>
#if defined (__FreeBSD__) || (defined __DragonFly__)
#include <sys/stat.h>
#endif

#include "localconsts.h"

struct LinkedMem
{
    uint32_t uiVersion;
    uint32_t uiTick;
    float fAvatarPosition[3];
    float fAvatarFront[3];
    float fAvatarTop[3];
    wchar_t name[256];
    float fCameraPosition[3];
    float fCameraFront[3];
    float fCameraTop[3];
    wchar_t identity[256];
    uint32_t context_len;
    char context[256];
    wchar_t description[2048];
};

class MumbleManager final
{
    public:
        MumbleManager();

        A_DELETE_COPY(MumbleManager)

        ~MumbleManager();

        void init();

        void setPlayer(const std::string &userName);

        void setAction(const int action);

        void setPos(const int tileX, const int tileY, const int direction);

        void setMap(const std::string &mapName);

        void setServer(const std::string &serverName);

    private:
        static uint16_t getMapId(std::string mapName) A_WARN_UNUSED;

        void setMapBase(uint16_t mapid);

        LinkedMem *mLinkedMem;
        LinkedMem mLinkedMemCache;
        float mMapBase[3];
};

extern MumbleManager *mumbleManager;

#endif  // USE_MUMBLE
#endif  // MUMBLEMANAGER_H
