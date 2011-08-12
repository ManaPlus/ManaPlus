/* Code taken from: http://mumble.sourceforge.net/Link */

#include "mumblemanager.h"

#include "configuration.h"
#include "logger.h"

#include "utils/mathutils.h"

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <ctype.h>

#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#include <SDL_syswm.h>

#ifndef WIN32
#include <sys/mman.h>
#endif

#include "debug.h"

MumbleManager::MumbleManager() :
    mLinkedMem(0)
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
#else
        munmap(mLinkedMem, sizeof(struct LinkedMem));
#endif
        mLinkedMem = 0;
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
        res = static_cast<uint16_t>(atoi(mapName.c_str()));
    }
    return res;
}

void MumbleManager::setMapBase(uint16_t mapid)
{
    mMapBase[0] = 10000.0f * (mapid & 0x1f);
    mapid >>= 5;
    mMapBase[1] = 1000.0f * (mapid & 0x3f);
    mapid >>= 6;
    mMapBase[2] = 10000.0f * (mapid & 0x1f);
}

void MumbleManager::init()
{
#if defined BSD4_4
    return;
#endif

    if (mLinkedMem || !config.getBoolValue("enableMumble"))
        return;

    logger->log1("MumbleManager::init");
#ifdef WIN32
    HANDLE hMapObject = OpenFileMappingW(FILE_MAP_ALL_ACCESS,
                                         FALSE, L"MumbleLink");
    if (hMapObject == NULL)
    {
        logger->log1("MumbleManager::init can't open MumbleLink");
        return;
    }

    mLinkedMem = (LinkedMem *) MapViewOfFile(hMapObject,
        FILE_MAP_ALL_ACCESS, 0, 0, sizeof(LinkedMem));

    if (mLinkedMem == NULL)
    {
        CloseHandle(hMapObject);
        hMapObject = NULL;
        logger->log1("MumbleManager::init can't map MumbleLink");
        return;
    }
#elif defined BSD4_4
#else
    char memName[256];
    snprintf(memName, 256, "/MumbleLink.%d", getuid());

    int shmfd = shm_open(memName, O_RDWR, S_IRUSR | S_IWUSR);

    if (shmfd < 0)
    {
        logger->log1("MumbleManager::init can't"
            " open shared memory MumbleLink");
        return;
    }

    mLinkedMem = static_cast<LinkedMem *>(mmap(NULL, sizeof(struct LinkedMem),
        PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0));

    if (mLinkedMem == reinterpret_cast<void *>(-1))
    {
        mLinkedMem = NULL;
        logger->log1("MumbleManager::init can't map MumbleLink");
        return;
    }

#endif
    wcsncpy(mLinkedMemCache.name, L"ManaPlus", 256);
    wcsncpy(mLinkedMemCache.description, L"ManaPlus Plugin", 2048);
    mLinkedMemCache.uiVersion = 2;

    // Left handed coordinate system.
    // X positive towards "left".
    // Y positive towards "up".
    // Z positive towards "into screen".
    //
    // 1 unit = 1 meter

    // Unit vector pointing out of the avatars eyes (here Front looks into scene).
    /* no way to look "up", 2d */
    mLinkedMemCache.fAvatarFront[1] = 0.0f;

    // Unit vector pointing out of the top of the avatars head
    // (here Top looks straight up).
    /* no way to change this in tmw */
    mLinkedMemCache.fAvatarTop[0] = 0.0f;
    mLinkedMemCache.fAvatarTop[1] = 1.0f;
    mLinkedMemCache.fAvatarTop[2] = 0.0f;

    mLinkedMemCache.fCameraFront[0] = 0.0f;
    mLinkedMemCache.fCameraFront[1] = 0.0f;
    mLinkedMemCache.fCameraFront[2] = 1.0f;

    mLinkedMemCache.fCameraTop[0] = 0.0f;
    mLinkedMemCache.fCameraTop[1] = 1.0f;
    mLinkedMemCache.fCameraTop[2] = 0.0f;

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
    memcpy(mLinkedMem, &mLinkedMemCache, sizeof(mLinkedMemCache));
}

void MumbleManager::setAction(int action)
{
    if (!mLinkedMem)
        return;

    switch(action)
    {
        case 0: /* STAND */
        case 1: /* WALK */
        case 2: /* ATTACK */
        case 5: /* HURT */
            mLinkedMemCache.fAvatarPosition[1] = 1.5f;
            break;
        case 3: /* SIT */
            mLinkedMemCache.fAvatarPosition[1] = 1.0f;
            break;
            case 4: /* DEAD */
        default:
            mLinkedMemCache.fAvatarPosition[1] = 0.0f;
            break;
    }
    mLinkedMemCache.fAvatarPosition[1] += mMapBase[1];
    mLinkedMemCache.fCameraPosition[1] = mLinkedMemCache.fAvatarPosition[1];

    mLinkedMemCache.uiTick++;
    memcpy(mLinkedMem, &mLinkedMemCache, sizeof(mLinkedMemCache));
}

void MumbleManager::setPos(int tileX, int tileY, int direction)
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
    switch(direction)
    {
        case 4: /* UP */
            mLinkedMemCache.fAvatarFront[0] = 0.0f;
            mLinkedMemCache.fAvatarFront[2] = 1.0f;
            break;
        case 1: /* DOWN */
            mLinkedMemCache.fAvatarFront[0] = 0.0f;
            mLinkedMemCache.fAvatarFront[2] = -1.0f;
            break;
        case 2: /* LEFT */
            mLinkedMemCache.fAvatarFront[0] = 1.0f;
            mLinkedMemCache.fAvatarFront[2] = 0.0f;
            break;
        case 8: /* RIGHT */
            mLinkedMemCache.fAvatarFront[0] = -1.0f;
            mLinkedMemCache.fAvatarFront[2] = 0.0f;
            break;
        default:
            break;
    }

    mLinkedMemCache.uiTick ++;
    memcpy(mLinkedMem, &mLinkedMemCache, sizeof(mLinkedMemCache));
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

    unsigned size = static_cast<unsigned>(serverName.size());
    if (size > sizeof(mLinkedMemCache.context) - 1)
        size = sizeof(mLinkedMemCache.context) - 1;

    memset(mLinkedMemCache.context, 0, sizeof(mLinkedMemCache.context));
    memcpy(mLinkedMemCache.context, serverName.c_str(), size);
    mLinkedMemCache.context[size] = '\0';
    mLinkedMemCache.context_len = size;
    mLinkedMemCache.uiTick ++;
    memcpy(mLinkedMem, &mLinkedMemCache, sizeof(mLinkedMemCache));
}
