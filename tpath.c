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

#include "tpath.h"


/* check if link exists and return string with resolved link path */
/* return null on fail */
/* returned value must be freed by caller */
char *path_link_read(const char *name)
{
	int flag = 0;
	char *temp = 0;
	char *buff = memget2(max_char);
	
	/* resolve links until no more link */
	strcpy2(&temp, name);
	while (1){
		/* read link path */
		int i = readlink(temp, buff, max_char);
		/* copy link if any */
		if (i > 0){
			flag = 1;
			buff[i] = 0;
			strlenset2(&buff);
			strcpy2(&temp, buff);
		}
		else{
			/* if no link could be resolved at all,
			 * then return original file name */
			if (!flag) strcpy2(&buff, name);
			free2(temp);
			return buff;
		}
	}
}


/* return the filename part of a path string */
/* returned value must be freed by caller */
char *path_get_filename(const char *path)
{
	char *res;
	char c;
	int i, i2, l;
	
	/* alloc mem for result */
	res = memget2(max_char);

	/* search for filename part */
	l = strlen(path);
	if (l > 0){
		i = l;
		/* search for "/" char backwords and stop at it */
		while (i--){
			c = path[i];
			if (c == '/') { i++; break; }
		}
		/* rightmost string is not null? */
		if (i < l){
			/* copy string after "/" char */
			i2 = 0;
			while(i <= l){
				res[i2++] = path[i++];
			}
			strlenset2(&res);
			return res;
		}
	}
	
	return 0;
}


/* check if path string ends with "/" char meaning it's a dir */
/* returns true if so */
int path_is_dir(const char *path)
{
	long l = strlen(path);
	if (!l) return 0;
	if (path[l - 1] == '/') return 1;
	return 0;
}


/* count subdirs in path name */
/* returns the number of subdir names between "/" chars in a path name */
/* path must start with "/" char */
/* only one "/" counts 1 */
int path_count_subdirs_name(const char *path)
{
	char c, d;
	int i, i2;
	
	i = 0;
	i2 = 0;
	c = 0;
	while(1){
		d = c;
		c = path[i];
		if (!c) break;
		if (c == '/' && d != '/') i2++;
		i++;
	}
	return i2;
}


/* get n part of subdir names from path name */
/* value 1 of num gets only a single "/" char if any */
/* path must start with "/" char */
/* returned value must be freed by caller */
char *path_get_subdirs_name(const char *path, int num)
{
	char *path_new;
	char c, d;
	int n, i, i2;

	/* alloc mem for new path name */
	path_new = memget2(max_char);
	
	/* return empty string if num is null */
	if (!num) return path_new;

	/* copy path to new path */
	strcpy2(&path_new, path);
	
	n = 0;
	i = 0;
	i2 = 0;
	c = 0;
	while(1){
		d = c;
		c = path[i];
		if (!c) break;
		/* move i2 to every end of subdir names following a "/" char */
		if (c == '/' && d != '/'){ i2 = i + 1; n++; }
		i++;
		if (n >= num){
			break;
		}
	}
	/* give back only subdir names */
	path_new[i2] = 0;
	strlenset3(&path_new, i2);
	
	return path_new;
}


/* join 2 paths together to make it a full path */
/* returned value must be freed by caller */
char *path_join(char *path1, char *path2)
{
	long l1 = strlen(path1);
	char *res = 0;
	
	/* create new joined path */
	strcpy2(&res, path1);
	/* does path1 end with "/" char? if not, then add "/" char too */
	if (path1[l1 - 1] != '/') strcat2(&res, "/");
	strcat2(&res, path2);
	
	return res;
}


/* return the parent dir of path */
/* returned value must be freed by caller */
char *path_get_parent_dir(char *path)
{
	char c;
	int i;
	long l;
	
	/* alloc mem for new path */
	char *path_new = 0;
	/* copy old path */
	strcpy2(&path_new, path);
	
	l = strlen2(&path_new) - 1;

	/* if path is a dir, then remove "/" chars from the end */
	if (path_is_dir(path_new)){
		while(1){
			if (l < 0) break;
			c = path_new[l];
			if (c != '/') break;
			l--;
		}
	}
	
	/* get the dir part */
	i = 0;
	while(1){
		if (l < 0) break;
		c = path_new[l];
		if (c == '/'){ i = l + 1; break; }
		l--;
	}
	path_new[i] = 0;
	strlenset3(&path_new, i);

	return path_new;
}


/* return the dir part of the path (result is the same if path is a dir) */
/* returned value must be freed by caller */
char *path_get_dir(char *path)
{
	char c;
	int i;
	long l;
	
	/* alloc mem for new path */
	char *path_new = 0;
	/* copy old path */
	strcpy2(&path_new, path);
	
	/* if path is a dir, then return it */
	if (path_is_dir(path_new)) return path_new;
	
	/* if it's a file, then get dir part */
	l = strlen2(&path_new) - 1;
	i = 0;
	while(1){
		if (l < 0) break;
		c = path_new[l];
		if (c == '/'){ i = l + 1; break; }
		l--;
	}
	path_new[i] = 0;
	strlenset3(&path_new, i);

	return path_new;
}


/* get dirlist depth for recursive call */
int path_count_dir_depth_r(char *path)
{
	DIR *mydir;
	struct dirent *md;
	static int depth = 0;
	
	if (!depth) depth = path_count_subdirs_name(path);

	/* open path dir */
	mydir = opendir(path);
	/* return null if dir cannot be opened */
	if (!mydir) return 0;
	/* cycle through dir recursively */
	while((md = readdir(mydir))) {
		/* skip dir . and .. */
		if(!strcmp(md->d_name, ".") || !strcmp(md->d_name, "..")) continue;
		/* is it a dir? */
		if (md->d_type == 4){
			int d;
			/* join paths */
			char *res = path_join(path, md->d_name);
			/* add "/" char to the end of path */
			char *res2 = 0;
			strcpy2(&res2, res);
			strcat2(&res2, "/");
			free2(res);
			/* get dir depth for current dir */
			path_count_dir_depth_r(res2);
			/* count dir depth by checking subdir names in path and store the deepest */
			d = path_count_subdirs_name(res2);
			if (depth < d) depth = d;
			free2(res2);
		}
	}
	closedir(mydir);
	
	/* return dir depth by calculating depth minus number of original subdir names */
	return (depth - path_count_subdirs_name(path) + 1);
}


/* count directory depth of a directory */
int path_count_dir_depth(char *path)
{
	char *res;
	int c;
	
	/* get parent dir */
	res = path_get_dir(path);

	/* get dir depth */	
	c = path_count_dir_depth_r(res);
	
	free2(res);
	
	return c;
}

