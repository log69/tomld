/*
----------------------------------------------------------------------
----------- tio.h ----------------------------------------------------
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

#include <time.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "tpath.h"

#define max_char	4096


float mytime();

void mytime_print();

void mytime_print_date();

char *mytime_get_sec_human();

void mytime_print();

int sys_get_uptime();

void key_set_mode();

void key_clear_mode();

char key_get();

char *pipe_read(const char *comm, long length);

void pipe_write(const char *comm, const char *buff);

char *file_read(const char *name, long length);

void file_write(const char *name, const char *buff);

void file_writea(const char *name, const char *buff);

int dir_exist(const char *name);

int file_exist(const char *name);

char *which(const char *name);

