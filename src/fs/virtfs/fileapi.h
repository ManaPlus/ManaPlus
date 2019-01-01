/*
 *  The ManaPlus Client
 *  Copyright (C) 2017-2019  The ManaPlus Developers
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

#ifndef UTILS_VIRTFS_FILEAPI_H
#define UTILS_VIRTFS_FILEAPI_H

#include "localconsts.h"

#ifdef USE_FILE_FOPEN
#define FILEHTYPE FILE *const
#define FILEMTYPE char *
#define FILEHDEFAULT nullptr
#define FILEOPEN(path, mode) fopen(path, mode)
#define FILECLOSE fclose
#define FILESEEK fseek
#define FILEOPEN_FLAG_READ "rb"
#define FILEOPEN_FLAG_WRITE "wb"
#define FILEOPEN_FLAG_APPEND "ab"
#else   // USE_FILE_FOPEN
#define FILEHTYPE const int
#define FILEMTYPE int
#define FILEHDEFAULT -1
#define FILEOPEN(path, mode) open(path, mode, S_IRUSR | S_IWUSR)
#define FILECLOSE close
#define FILESEEK lseek
#define FILEOPEN_FLAG_READ O_RDONLY
#define FILEOPEN_FLAG_WRITE O_WRONLY | O_CREAT | O_TRUNC
#define FILEOPEN_FLAG_APPEND O_WRONLY | O_CREAT | O_APPEND
#endif  // USE_FILE_FOPEN

#endif  // UTILS_VIRTFS_FILEAPI_H
