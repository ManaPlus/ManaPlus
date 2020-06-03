#!/usr/bin/env bash

# Fix compilation warning
sed -i 's|#define GL_GLEXT_VERSION 29|#ifndef GL_GLEXT_VERSION\n#define GL_GLEXT_VERSION 29\n#endif|' /usr/include/SDL/SDL_opengl.h
