/*
 *  The ManaPlus Client
 *  Copyright (C) 2015-2019  The ManaPlus Developers
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

#ifdef __native_client__

#include "utils/naclmessages.h"

#include <ppapi_simple/ps.h>
#include <ppapi_simple/ps_event.h>
#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/var.h>

#include <mutex>
#include <condition_variable>

#include "debug.h"

struct NaclMessageHandle final
{
    NaclMessageHandle() :
        handled(false),
        type(),
        message(),
        condv()
    { }

    A_DELETE_COPY(NaclMessageHandle)

    bool handled;
    std::string type;
    std::string message;
    std::condition_variable condv;
};

void naclPostMessage(const std::string &type,
                     const std::string &message)
{
    pp::Var msgVar = pp::Var(std::string(type).append(":").append(message));
    pp::Instance(PSGetInstanceId()).PostMessage(msgVar);
}

static void naclMessageHandlerFunc(struct PP_Var key,
                                   struct PP_Var value,
                                   void* user_data)
{
    NaclMessageHandle *handle = reinterpret_cast<NaclMessageHandle *>(
        user_data);

    if (key.type != PP_VARTYPE_STRING || value.type != PP_VARTYPE_STRING)
        return;
    if (pp::Var(key).AsString() != handle->type)
        return;

    handle->handled = true;
    handle->message = pp::Var(value).AsString();

    handle->condv.notify_one();
}

NaclMessageHandle *naclRegisterMessageHandler(const std::string &type)
{
    NaclMessageHandle *handle = new NaclMessageHandle;
    handle->handled = false;
    handle->type = type;

    PSEventRegisterMessageHandler(type.c_str(),
        naclMessageHandlerFunc,
        reinterpret_cast<void *>(handle));
    return handle;
}

void naclUnregisterMessageHandler(NaclMessageHandle *handle)
{
    PSEventRegisterMessageHandler(handle->type.c_str(),
        nullptr,
        nullptr);
    delete handle;
}

std::string naclWaitForMessage(NaclMessageHandle *handle)
{
    std::mutex mtx;
    std::unique_lock <std::mutex> lck(mtx);

    while (!handle->handled)
        handle->condv.wait(lck);

    handle->handled = false;
    return handle->message;
}

#endif  // __native_client__
