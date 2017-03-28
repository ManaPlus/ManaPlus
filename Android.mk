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
    $(ROOT_PATH)/curl/include \
    $(ROOT_PATH)/SDL2_net-2.0.0

LOCAL_CPPFLAGS += -DUSE_SDL2 -DTMWA_SUPPORT -DUSE_OPENGL -DENABLE_NLS -DENABLE_LIBXML
LOCAL_CPPFLAGS += -frtti -fexceptions -Wno-error=format-security -fvisibility=hidden
LOCAL_CPPFLAGS += -ffast-math -funswitch-loops -pedantic -Wall -Wc++11-compat -std=c++0x -funsafe-loop-optimizations -flto -fwhole-program
LOCAL_CPPFLAGS += -fpic -fpie

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_mixer sdl_net SDL2_ttf SDL2_image libcurl intl png sdl2_gfx xml2

LOCAL_LDLIBS := -lGLESv1_CM -lEGL -llog -lz

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
    $(subst $(LOCAL_PATH)/,, \
    $(wildcard $(LOCAL_PATH)/src/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/being/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/render/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/render/vertexes/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/render/openglx/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/render/shaders/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/render/opengl/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/render/nacl/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/utils/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/utils/translation/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/utils/xml/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/listeners/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/const/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/const/render/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/const/utils/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/const/resources/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/const/resources/map/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/const/resources/item/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/const/gui/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/const/net/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/resources/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/resources/image/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/resources/animation/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/resources/resourcemanager/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/resources/atlas/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/resources/db/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/resources/sprite/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/resources/inventory/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/resources/loaders/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/resources/dye/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/resources/map/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/resources/rect/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/resources/item/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/resources/skill/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/gui/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/gui/windows/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/gui/shortcut/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/gui/widgets/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/gui/widgets/attrs/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/gui/widgets/tabs/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/gui/widgets/tabs/chat/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/gui/fonts/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/gui/popups/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/gui/models/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/sdl2gfx/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/actions/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/input/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/input/touch/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/input/pages/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/net/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/net/eathena/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/net/tmwa/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/net/ea/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/test/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/debug/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/fs/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/fs/virtfs/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/enums/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/enums/being/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/enums/render/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/enums/resources/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/enums/resources/map/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/enums/resources/item/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/enums/resources/skill/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/enums/gui/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/enums/input/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/enums/net/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/enums/fs/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/enums/simpletypes/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/enums/particle/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/enums/events/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/particle/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/events/*.cpp) \
    )

include $(BUILD_SHARED_LIBRARY)
