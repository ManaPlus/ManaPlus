# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Windows)

# toolchain prefix, can be overridden by -DTOOLCHAIN=...
# IF (NOT TOOLCHAIN)
SET(TOOLCHAIN "i686-w64-mingw32-")
# ENDIF()

# which compilers to use for C and C++
SET(CMAKE_C_COMPILER ${TOOLCHAIN}gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN}g++)
SET(CMAKE_RC_COMPILER ${TOOLCHAIN}windres)

SET(CMAKE_INSTALL_PREFIX ./win)
SET(CMAKE_BUILD_TYPE None)
SET(CMAKE_C_FLAGS -O3 -Wall)
SET(CMAKE_CXX_FLAGS -O3 -Wall)

SET(SDLIMAGE_INCLUDE_DIR ../libs/include/SDL)
SET(SDLMIXER_INCLUDE_DIR ../libs/include/SDL)
SET(SDLNET_INCLUDE_DIR ../libs/include/SDL)
SET(SDLTTF_INCLUDE_DIR ../libs/include/SDL)
SET(SDL_INCLUDE_DIR ../libs/include/SDL)
SET(ENABLE_MANASERV OFF)

# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH  /home/mingw/cross/i686-w64-mingw32 /../libs/lib )

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

