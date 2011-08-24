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


/* return a string containing the full date */
/* returned value must be freed by caller */
void mytime_print_date()
{
	char *res = 0;
	long l;
	/* get epoch time */
	time_t  timer = time(NULL);
	/* convert epoch time to full date string */
	strcpy2(&res, ctime(&timer));
	l = strlen2(&res);
	/* remove ending new line char from date string */
	if (l > 0) l--;
	res[l] = 0;
	strlenset3(&res, l);
	
	/* print it */
	printf("at %s", res);
	free2(res);
}


/* return a string containing a human readable time converted from seconds */
/* returned value must be freed by caller */
char *mytime_get_sec_human(int sec)
{
	char *ptime, *t = 0;
	int s;
	
	int day  = sec / 60 / 60 / 24;
	int hour = sec / 60 / 60;
	int min  = sec / 60;

	if (day)      { s = day;  strcpy2(&t, " day");  }
	else if (hour){ s = hour; strcpy2(&t, " hour"); }
	else if (min) { s = min;  strcpy2(&t, " minute");  }
	else          { s = sec;  strcpy2(&t, " second");  }
	
	/* plural */
	if (s > 1) strcat2(&t, "s");
	
	ptime = string_itos(s);
	strcat2(&ptime, t);
	free2(t);

	return ptime;
}


/* print elapsed time since last time measuring speed */
void mytime_print()
{
	static float t = 0;
	t = mytime() - t;
	
	printf("-- time %.2f sec\n", t);
}


/* get system uptime in seconds */
int sys_get_uptime()
{
	char *f = "/proc/uptime";
	char *res, *res2;
	int i;
	
	/* read uptime from /proc system */
	res = file_read(f, -1);
	/* read first number part only */
	res2 = string_get_number(res);
	/* convert string number to integer */
	i = atoi(res2);

	free2(res2);
	free2(res);

	return i;
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
	int ret;
	
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
	ret = fread(buff, length, 1, p);
	if (length > 0 && !ret){
		error("error: unknown error while reading from pipe with command: ");
		error(comm); newl_();
		exit(1);
	}
		
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


/* open file and read content with given length if length is greater than 0,
 * or if length is null, check length from file descriptor,
 * or if length is -1, check length from read file */
/* returned value must be freed by caller */
char *file_read(const char *name, long length)
{
	char *buff;
	unsigned long len = 0;
	
	/* open file for reading */
	FILE *f = fopen(name, "rb");
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
	else if (length == -1){
		unsigned long c = 0;
		len = max_char;
		buff = memget2(len);
		while(1){
			memset(buff, 0, len);
			if (!fread(buff, len, 1, f)) break;
			c++;
		}
		/* check length of last buffered string by searching for non-zero char backwards */
		len = max_char;
		while(1){
			if (!len) break;
			if (buff[--len]){ len++; break; }
		}
		len += c * max_char;
		free2(buff);
		fclose(f);
		f = fopen(name, "rb");
	}
	else len = length;
	
	/* alloc mem */
	buff = memget2(len);
	/* read file */
	if (len > 0){
		unsigned long len2 = fread(buff, len, 1, f);
		/* store the number of actually read bytes as length, because it can happen
		 * that the length of file is less than the requested number of bytes */
		if (length > 0) len = len2;
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
	FILE *f = fopen(name, "wb");
	if (!f){
		error("error: cannot write file ");
		error(name); newl_();
		exit(1);
	}
	/* write contents if not null */
	if (buff){
		fprintf(f, "%s", buff);
	}
	fclose(f);
}


/* open file and write content in append mode */
void file_writea(const char *name, const char *buff)
{
	FILE *f = fopen(name, "ab");
	if (!f){
		error("error: cannot write file ");
		error(name); newl_();
		exit(1);
	}
	/* write contents if not null */
	if (buff){
		fprintf(f, "%s", buff);
	}
	fclose(f);
}


/* redirect stderr and stdout to the file */
void file_std_redir(const char *name)
{
	FILE *f;
	f = freopen(name, "ab", stdout);
	if (!f){
		error("error: cannot redirect stdout to file ");
		error(name); newl_();
		exit(1);
	}
	f = freopen(name, "ab", stderr);
	if (!f){
		error("error: cannot redirect stderr to file ");
		error(name); newl_();
		exit(1);
	}
}


/* return modification time of file */
int file_get_mod_time(const char *name)
{
	int mod = 0;
	struct stat buff;
	if (stat(name, &buff) == -1){
		error("error: cannot stat file ");
		error(name); newl_();
		exit(1);
	}
	
	mod = buff.st_mtime;
	
	return mod;
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

	f = fopen(name, "rb+");
	if (f){
		res = 1;
		fclose(f);
	}

	f = fopen(name, "rb");
	if (f){
		res = 1;
		fclose(f);
	}

	return res;
}


/* mkdir recursively (input can be file too, not only dir) */
void mkdir_recursive(char *dir)
{
	const int max_iter = 32;
	int i, l;
	
	if (!dir) return;

	/* get number of subdirs */	
	l = path_count_subdirs_name(dir);
	if (l > 1){
		i = 2;
		/* mkdir recursively */
		while(1){
			/* get first subdirs of path */
			char *d = path_get_subdirs_name(dir, i);
			/* create dir */
			mkdir(d, S_IRWXU);
			free2(d);
			i++;
			if (i > l || i > max_iter) break;
		}
	}
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
	if (!getcwd(full, max_char)){
		error("error: could not get current working directory\n"); exit(1); }
	strlenset2(&full);
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

