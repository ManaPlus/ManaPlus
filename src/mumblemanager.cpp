/*
 *  Code taken from: http://mumble.sourceforge.net/Link
 *
 *  All code listed below is in the public domain and can be used, shared or
 *  modified freely
 *
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
 */

#ifdef USE_MUMBLE

#include "mumblemanager.h"

#include "configuration.h"
#include "logger.h"

#include "utils/mathutils.h"

#if defined(__NetBSD__) || defined(__OpenBSD__)
#include <sys/param.h>
#endif  // defined(__NetBSD__) || defined(__OpenBSD__)
#include <wchar.h>
#include <cctype>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_syswm.h>
PRAGMA48(GCC diagnostic pop)

#ifndef WIN32
#include <sys/mman.h>
#endif  // WIN32

#include "debug.h"

#ifdef USE_MUMBLE
MumbleManager *mumbleManager = nullptr;
#endif  // USE_MUMBLE

MumbleManager::MumbleManager() :
    mLinkedMem(nullptr),
    mLinkedMemCache()
{
    mMapBase[0] = mMapBase[1] = mMapBase[2] = 0.;
    init();
}

MumbleManager::~MumbleManager()
{
    if (mLinkedMem)
    {
#ifdef WIN32
        UnmapViewOfFile(mLinkedMem);
#elif defined BSD4_4
#else  // WIN32

        munmap(mLinkedMem, sizeof(LinkedMem));
#endif  // WIN32

        mLinkedMem = nullptr;
    }
}

uint16_t MumbleManager::getMapId(std::string mapName)
{
    uint16_t res = 0;
    if (mapName.size() != 5 || mapName[3] != '-')
    {
        res = getCrc16(mapName);
    }
    else
    {
        mapName = mapName.substr(0, 3) + mapName[4];
        res = CAST_U16(atoi(mapName.c_str()));
    }
    return res;
}

void MumbleManager::setMapBase(uint16_t mapid)
{
    mMapBase[0] = 10000.0F * (mapid & 0x1F);
    mapid >>= 5;
    mMapBase[1] = 1000.0F * (mapid & 0x3F);
    mapid >>= 6;
    mMapBase[2] = 10000.0F * (mapid & 0x1F);
}

void MumbleManager::init()
{
#if defined BSD4_4
    return;
#endif  // defined BSD4_4

    if (mLinkedMem || !config.getBoolValue("enableMumble"))
        return;

    logger->log1("MumbleManager::init");
#ifdef WIN32
    HANDLE hMapObject = OpenFileMappingW(FILE_MAP_ALL_ACCESS,
                                         FALSE, L"MumbleLink");
    if (!hMapObject)
    {
        logger->log1("MumbleManager::init can't open MumbleLink");
        return;
    }

    mLinkedMem = reinterpret_cast<LinkedMem *>(MapViewOfFile(hMapObject,
        FILE_MAP_ALL_ACCESS, 0, 0, sizeof(LinkedMem)));

    if (!mLinkedMem)
    {
        CloseHandle(hMapObject);
        hMapObject = nullptr;
        logger->log1("MumbleManager::init can't map MumbleLink");
        return;
    }
#elif defined BSD4_4
#else  // WIN32

    char memName[256];
    snprintf(memName, sizeof(memName), "/MumbleLink.%u", getuid());

    const int shmfd = shm_open(memName, O_RDWR, S_IRUSR | S_IWUSR);

    if (shmfd < 0)
    {
        logger->log1("MumbleManager::init can't"
            " open shared memory MumbleLink");
        return;
    }

    mLinkedMem = static_cast<LinkedMem *>(mmap(nullptr,
        sizeof(LinkedMem), PROT_READ | PROT_WRITE,
        MAP_SHARED, shmfd, 0));

    if (mLinkedMem == reinterpret_cast<void *>(-1))
    {
        mLinkedMem = nullptr;
        logger->log1("MumbleManager::init can't map MumbleLink");
        return;
    }

#endif  // WIN32

    wcsncpy(mLinkedMemCache.name, L"ManaPlus", 256);
    wcsncpy(mLinkedMemCache.description, L"ManaPlus Plugin", 2048);
    mLinkedMemCache.uiVersion = 2;

    // Left handed coordinate system.
    // X positive towards "left".
    // Y positive towards "up".
    // Z positive towards "into screen".
    //
    // 1 unit = 1 meter

    // Unit vector pointing out of the avatars eyes
    // (here Front looks into scene).
    /* no way to look "up", 2d */
    mLinkedMemCache.fAvatarFront[1] = 0.0F;

    // Unit vector pointing out of the top of the avatars head
    // (here Top looks straight up).
    /* no way to change this in tmw */
    mLinkedMemCache.fAvatarTop[0] = 0.0F;
    mLinkedMemCache.fAvatarTop[1] = 1.0F;
    mLinkedMemCache.fAvatarTop[2] = 0.0F;

    mLinkedMemCache.fCameraFront[0] = 0.0F;
    mLinkedMemCache.fCameraFront[1] = 0.0F;
    mLinkedMemCache.fCameraFront[2] = 1.0F;

    mLinkedMemCache.fCameraTop[0] = 0.0F;
    mLinkedMemCache.fCameraTop[1] = 1.0F;
    mLinkedMemCache.fCameraTop[2] = 0.0F;

    mLinkedMemCache.uiTick++;
}

void MumbleManager::setPlayer(const std::string &userName)
{
    if (!mLinkedMem)
        return;

    // Identifier which uniquely identifies a certain player in a context
    // (e.g. the ingame Name).
    mbstowcs(mLinkedMemCache.identity, userName.c_str(), 256);
    mLinkedMemCache.uiTick ++;
PRAGMA8(GCC diagnostic push)
PRAGMA8(GCC diagnostic ignored "-Wclass-memaccess")
    memcpy(mLinkedMem, &mLinkedMemCache, sizeof(mLinkedMemCache));
PRAGMA8(GCC diagnostic pop)
}

void MumbleManager::setAction(const int action)
{
    if (!mLinkedMem)
        return;

    switch (action)
    {
        case 0: /* STAND */
        case 1: /* WALK */
        case 2: /* ATTACK */
        case 5: /* HURT */
            mLinkedMemCache.fAvatarPosition[1] = 1.5F;
            break;
        case 3: /* SIT */
            mLinkedMemCache.fAvatarPosition[1] = 1.0F;
            break;
        case 4: /* DEAD */
        default:
            mLinkedMemCache.fAvatarPosition[1] = 0.0F;
            break;
    }
    mLinkedMemCache.fAvatarPosition[1] += mMapBase[1];
    mLinkedMemCache.fCameraPosition[1] = mLinkedMemCache.fAvatarPosition[1];

    mLinkedMemCache.uiTick++;
PRAGMA8(GCC diagnostic push)
PRAGMA8(GCC diagnostic ignored "-Wclass-memaccess")
    memcpy(mLinkedMem, &mLinkedMemCache, sizeof(mLinkedMemCache));
PRAGMA8(GCC diagnostic pop)
}

void MumbleManager::setPos(const int tileX, const int tileY,
                           const int direction)
{
    if (!mLinkedMem)
        return;

    // Position of the avatar (here standing slightly off the origin)
    // lm->fAvatarPosition

    /* tmw coordinates work exactly the other way round */
    mLinkedMemCache.fAvatarPosition[0] = static_cast<float>(tileX)
        + mMapBase[0];
    mLinkedMemCache.fAvatarPosition[2] = static_cast<float>(tileY)
        + mMapBase[2];

    // Same as avatar but for the camera.
    // lm->fCameraPosition, fCameraFront, fCameraTop

    // Same as avatar but for the camera.
    mLinkedMemCache.fCameraPosition[0] = mLinkedMemCache.fAvatarPosition[0];
    mLinkedMemCache.fCameraPosition[2] = mLinkedMemCache.fAvatarPosition[2];

    // Unit vector pointing out of the avatars eyes
    // (here Front looks into scene).
    switch (direction)
    {
        case 4: /* UP */
            mLinkedMemCache.fAvatarFront[0] = 0.0F;
            mLinkedMemCache.fAvatarFront[2] = 1.0F;
            break;
        case 1: /* DOWN */
            mLinkedMemCache.fAvatarFront[0] = 0.0F;
            mLinkedMemCache.fAvatarFront[2] = -1.0F;
            break;
        case 2: /* LEFT */
            mLinkedMemCache.fAvatarFront[0] = 1.0F;
            mLinkedMemCache.fAvatarFront[2] = 0.0F;
            break;
        case 8: /* RIGHT */
            mLinkedMemCache.fAvatarFront[0] = -1.0F;
            mLinkedMemCache.fAvatarFront[2] = 0.0F;
            break;
        default:
            break;
    }

    mLinkedMemCache.uiTick ++;
PRAGMA8(GCC diagnostic push)
PRAGMA8(GCC diagnostic ignored "-Wclass-memaccess")
    memcpy(mLinkedMem, &mLinkedMemCache, sizeof(mLinkedMemCache));
PRAGMA8(GCC diagnostic pop)
}

void MumbleManager::setMap(const std::string &mapName)
{
    if (!mLinkedMem)
        return;

    // Context should be equal for players which should be able to hear each
    // other positional and differ for those who shouldn't
    // (e.g. it could contain the server+port and team)

    setMapBase(getMapId(mapName));
    setAction(0); /* update y coordinate */
}

void MumbleManager::setServer(const std::string &serverName)
{
    if (!mLinkedMem)
        return;

    unsigned size = CAST_U32(serverName.size());
    if (size > sizeof(mLinkedMemCache.context) - 1)
        size = sizeof(mLinkedMemCache.context) - 1;

    memset(mLinkedMemCache.context, 0, sizeof(mLinkedMemCache.context));
PRAGMA8(GCC diagnostic push)
PRAGMA8(GCC diagnostic ignored "-Wclass-memaccess")
    memcpy(mLinkedMemCache.context, serverName.c_str(), size);
PRAGMA8(GCC diagnostic pop)
    mLinkedMemCache.context[size] = '\0';
    mLinkedMemCache.context_len = size;
    mLinkedMemCache.uiTick ++;
PRAGMA8(GCC diagnostic push)
PRAGMA8(GCC diagnostic ignored "-Wclass-memaccess")
    memcpy(mLinkedMem, &mLinkedMemCache, sizeof(mLinkedMemCache));
PRAGMA8(GCC diagnostic pop)
}

#endif  // USE_MUMBLE
