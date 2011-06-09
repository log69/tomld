/*
----------------------------------------------------------------------
----------- tpath.c --------------------------------------------------
----------- This file is part of tomld (tomoyo learning daemon) ------
----------- Copyright (C) 2011 Andras Horvath, Hungary ---------------
----------- http://log69.com -----------------------------------------
----------------------------------------------------------------------

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License as
	published by the Free Software Foundation; either version 3 of the
	License, or	(at your option) any later version.

	This program is distributed in the hope that it will be useful,	but
	WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

----------------------------------------------------------------------
*/

#include <unistd.h>
#include <dirent.h>
#include "tstring.h"

#define max_char	4096


char *path_link_read(const char *name);

char *path_get_filename(const char *text);

int path_is_dir(const char *path);

int path_count_subdirs_name(const char *path);

char *path_get_subdirs_name(const char *path, int num);

char *path_join(char *path1, char *path2);

char *path_get_parent_dir(char *path);

char *path_get_dir(char *path);

int path_count_dir_depth_r(char *path);

int path_count_dir_depth(char *path);

