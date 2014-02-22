LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := manaplus

ROOT_PATH := $(LOCAL_PATH)/..
SDL_PATH := ../SDL

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
    $(LOCAL_PATH)/src \
    $(ROOT_PATH)/xml2/include \
    $(ROOT_PATH)/gettext-0.18/gettext-runtime/intl \
    $(ROOT_PATH)/sdl_gfx \
    $(ROOT_PATH)/physfs/include \
    $(ROOT_PATH)/curl/include \
    $(ROOT_PATH)/SDL2_net-2.0.0

LOCAL_CPPFLAGS += -DUSE_SDL2 -DEATHENA_SUPPORT -DUSE_OPENGL -DENABLE_NLS
LOCAL_CPPFLAGS += -frtti -fexceptions -Wno-error=format-security -fvisibility=hidden
LOCAL_CPPFLAGS += -ffast-math -funswitch-loops -pedantic -Wall -Wc++11-compat -std=c++0x -funsafe-loop-optimizations -flto -fwhole-program


LOCAL_SHARED_LIBRARIES := SDL2 SDL2_mixer sdl_net SDL2_ttf SDL2_image libcurl intl png physfs sdl2_gfx xml2

LOCAL_LDLIBS := -lGLESv1_CM -lEGL -llog -lz

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
    $(subst $(LOCAL_PATH)/,, \
    $(wildcard $(LOCAL_PATH)/src/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/being/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/events/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/gui/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/gui/models/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/gui/popups/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/gui/widgets/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/gui/widgets/tabs/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/gui/windows/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/gui/basic/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/gui/basic/widgets/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/input/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/net/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/net/ea/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/net/ea/gui/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/net/eathena/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/net/eathena/gui/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/net/tmwa/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/net/tmwa/gui/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/listeners/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/particle/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/render/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/resources/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/resources/db/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/test/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/utils/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/utils/translation/*.cpp) \
    )

include $(BUILD_SHARED_LIBRARY)
