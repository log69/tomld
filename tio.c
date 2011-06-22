/*
----------------------------------------------------------------------
----------- tio.c ----------------------------------------------------
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

#include "tio.h"


/* var for terminal input / output */
struct termio terminal_backup;


/* return the time elapsed since program start */
float mytime()
{
	static int flag_time = 0;
	static struct timeval start, end;
	static long seconds, useconds;
	static float t = 0;

	/* first run? */
	if (!flag_time){
		gettimeofday(&start, 0);
		flag_time = 1;
	}
	else{
		gettimeofday(&end, 0);
		seconds  = end.tv_sec  - start.tv_sec;
		useconds = end.tv_usec - start.tv_usec;

		t = (((seconds) * 1000 + useconds/1000.0) + 0.5) / 1000;
	}
	
	return t;
}


/* print elapsed time since last time */
void mytime_print()
{
	static float t = 0;
	float t2 = mytime();
	t = t2 - t;
	
	printf("-- time %.2f sec\n", t);
}


/* set terminal input mode for keyboard read */
void key_set_mode()
{
	int fd = fileno(stdin);
	struct termio term;
	ioctl(fd, TCGETA, &terminal_backup);
	term = terminal_backup;
	term.c_cc[VMIN] = 0;
	term.c_cc[VTIME] = 0;
	term.c_lflag = 0;
	ioctl(fd, TCSETA, &term);
}


/* restore original terminal mode */
void key_clear_mode()
{
	int fd = fileno(stdin);
	ioctl(fd, TCSETA, &terminal_backup);
}


/* read keyboard without waiting */
char key_get()
{
	int fd, key;
	char c = '\0';
	key_set_mode();
	fd = fileno(stdin);
	key = read(fd, &c, 1);
	if (!key) c = '\0';
	key_clear_mode();
	return c;
}


/* open pipe and read content with given length */
/* returned value must be freed by caller */
char *pipe_read(const char *comm, long length)
{
	char *buff;
	
	/* open pipe for reading */
	FILE *p = popen(comm, "r");
	if (!p){
		error("error: cannot open pipe for command: ");
		error(comm); newl_();
		exit(1);
	}
	
	/* alloc mem for it */
	buff = memget2(length);
	memset(buff, 0, length);
	/* read pipe */
	fread(buff, length, 1, p);
	pclose(p);
	/* set dynamic string length */
	strlenset2(&buff);
	
	return buff;
}


/* open pipe and write content as input to command */
/* returned value must be freed by caller */
void pipe_write(const char *comm, const char *buff)
{
	/* open pipe for writing */
	FILE *p = popen(comm, "w");
	if (!p){
		error("error: cannot open pipe for command: ");
		error(comm); newl_();
		exit(1);
	}
	
	/* write pipe */
	fprintf(p, "%s\n", buff);
	pclose(p);
}


/* open file and read content with given length, or if length is null, then check length from file descriptor */
/* returned value must be freed by caller */
char *file_read(const char *name, long length)
{
	char *buff;
	unsigned long len = 0;
	
	/* open file for reading */
	FILE *f = fopen(name, "r");
	if (!f){
		error("error: cannot read file ");
		error(name); newl_();
		exit(1);
	}
	/* check file length */
	if (!length){
		/* get file length from file descriptor */
		fseek(f, 0, SEEK_END);
		len = ftell(f);
		fseek(f, 0, SEEK_SET);
	}
	/* length will be unknown (like with /proc files)
	 * so check it by buffered reading because i need the exact length.
	 * unfortunately at tomoyo proc files,
	 * neither ftell nor fread can tell me the number of file position
	 * or the number of read bytes,
	 * so i have to trick with filling the buffer with zero
	 * and check the string length at the end */
	else{
		unsigned long c = 0;
		len = max_char;
		buff = memget2(len);
		while(1){
			memset(buff, 0, len);
			if (!fread(buff, len, 1, f)) break;
			c++;
		}
		len = strlen(buff) + c * len;
		free2(buff);
		fclose(f);
		f = fopen(name, "r");
	}
	
	/* alloc mem */
	buff = memget2(len);
	/* read file */
	if (len > 0){
		fread(buff, len, 1, f);
	}
	fclose(f);
	/* write null to the end of file */
	buff[len] = 0;
	/* set dynamic string length */
	strlenset3(&buff, len);

	return buff;
}


/* open file and write content */
void file_write(const char *name, const char *buff)
{
	FILE *f = fopen(name, "w");
	if (!f){
		error("error: cannot write file ");
		error(name); newl_();
		exit(1);
	}
	/* write contents if not null */
	if (buff) fprintf(f, buff);
	fclose(f);
}


/* check if dir exists */
int dir_exist(const char *name)
{
	DIR *d;
	d = opendir(name);
	if (d) { closedir(d); return 1; }
	return 0;
}


/* check if file exists */
int file_exist(const char *name)
{
	FILE *f;
	int res = 0;

	f = fopen(name, "r+");
	if (f){
		res = 1;
		fclose(f);
	}

	f = fopen(name, "r");
	if (f){
		res = 1;
		fclose(f);
	}

	return res;
}


/* search file name in current dir first, then in bin locations and give back full path on success */
/* returned value must be freed by caller */
char *which(const char *name)
{
	char *path_bin[] = {"/usr/local/sbin", "/usr/local/bin", "/usr/sbin", "/usr/bin", "/sbin", "/bin", 0};

	char *res, *full;
	int i;
	
	/* name starts with "/" char? */
	if (name[0] == '/'){
		/* file exists? if not, then fail */
		if (file_exist(name)){
			/* return resolved link path */
			return path_link_read(name);
		}
		return 0;
	}
	
	full = memget2(max_char);

	/* fle exists in the current dir? */
	getcwd(full, max_char);
	strcat2(&full, "/");
	strcat2(&full, name);
	if (file_exist(full)){
		/* return resolved link path */
		res = path_link_read(full);
		free2(full);
		return res;
	}

	/* file exists in the search paths? */
	i = 0;
	while(1){
		res = path_bin[i++];
		if (!res) break;
		strcpy2(&full, res);
		strcat2(&full, "/");
		strcat2(&full, name);
		if (file_exist(full)){
			/* return resolved link path */
			res = path_link_read(full);
			free2(full);
			return res;
		}
		strnull2(&full);
	}

	free2(full);
	return 0;
}

