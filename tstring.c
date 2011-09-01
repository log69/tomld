/*
----------------------------------------------------------------------
----------- tstring.c ------------------------------------------------
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

#include "tstring.h"


/* free my allocated dynamic string space */
void free2(char *ptr)
{
	if (ptr) free((unsigned long*)(ptr) - 2);
}


/* count lines of string */
int string_count_lines(const char *text)
{
	int i = 0;
	int c = 0;
	while(1){
		if (!text[i]) return c;
		if (text[i] == '\n') c++;
		i++;
	}
}


/* print a new line */
void newl()
{
	printf("\n");
	fflush(stdout);
}


/* print a new line to stderr */
void newl_()
{
	fprintf(stderr, "%s", "\n");
	fflush(stderr);
}


/* print debug info about a string */
void debug(const char *text)
{
	long c;
	long l;
	/* isn't text a null pointer? */
	if (text){
		l = strlen(text);
		/* count lines of text */
		c = string_count_lines(text);
		/* print text and info */
		printf("--\n");
		printf("%s", text);
		/* print newline if missing from the end of string */
		if (text[l-1] != '\n') newl();
		printf("-- debug bytes %ld and ", (long) strlen(text));
		printf("lines %ld\n", c);
	}
	else printf("-- null pointer\n");
}


/* print debug info about an integer */
void debugi(int num)
{
	printf("-- debug integer is %d\n", num);
}


/* print debug info about a long integer */
void debugl(long num)
{
	printf("-- debug long integer is %ld\n", num);
}


/* print debug info about a float */
void debugf(float num)
{
	printf("-- debug float is %f\n", num);
}


/* print debug info about a pointer */
void debugp(char *p)
{
	printf("-- debug pointer is %p\n", p);
}


/* print error message to stderr */
void error(const char *text)
{
	fprintf(stderr, "%s", text);
	fflush(stderr);
}


/* allocate dynamic string memory for char and return pointer */
/* my dynamic string looks like this: */
/* [maxlen] [len] [s t r i n g c h a r s \0 \0 \0] */
/* i make the pointer point to the data part after maxlen and len data */
/* returned value must be freed by caller */
char *memget2(unsigned long num)
{
	/* get long pointer for extra datas */
	unsigned long *p2;
	/* alloc mem */
	char *p = malloc(sizeof(char) * (num + 1) + sizeof(unsigned long) + sizeof(unsigned long));
	if (!p){ error("error: out of memory\n"); exit(1); }
	/* convert pointer */
	p2 = (unsigned long*)(p);
	/* store memory size as the first data in pointer */
	*p2 = num + 1; p2++;
	/* store string length as the second data in pointer */
	*p2 = 0; p2++;
	/* set pointer to string data */
	p = (char*)(p2);
	/* clear string */
	p[0] = 0;
	return p;
}


/* allocate memory for char and return pointer */
/* returned value must be freed by caller */
char *memget(unsigned long num)
{
	char *p = malloc((sizeof(char)) * (num + 1));
	if (!p){ error("error: out of memory\n"); exit(1); }
	/* clear first byte */
	p[0] = 0;
	return p;
}


/* allocate memory for char pointer list and return pointer */
/* returned value must be freed by caller */
char **memget_ptr(unsigned long num)
{
	char** p = malloc((sizeof(char**)) * (num + 1));
	if (!p){ error("error: out of memory\n"); exit(1); }
	/* clear first byte */
	p[0] = 0;
	return p;
}


/* allocate memory for long and return pointer */
/* returned value must be freed by caller */
int *memget_int(unsigned long num)
{
	int *p = malloc((sizeof(int)) * (num + 1));
	if (!p){ error("error: out of memory\n"); exit(1); }
	/* clear first byte */
	p[0] = 0;
	return p;
}


/* my strlen for dynamic strings */
unsigned long strlen2(char **s1)
{
	if ((*s1)) return *((unsigned long*)(*s1) - 1);
	else return 0;
}


/* my setting length for dynamic strings */
void strlenset2(char **s1)
{
	*((unsigned long*)(*s1) - 1) = strlen((*s1));
}


/* my setting length for dynamic strings */
void strlenset3(char **s1, unsigned long l)
{
	*((unsigned long*)(*s1) - 1) = l;
}


/* zero my dynamic string and its length too */
void strnull2(char **s1)
{
	if (*s1){
		*((unsigned long*)(*s1) - 1) = 0;
		*(*s1) = 0;
	}
}


/* my strcpy for dynamic strings */
void strcpy2(char **s1, const char *s2)
{
	unsigned long *p2, l2, size;
	/* safety check for null pointers */
	if (!s2) return;
	/* length of source string */
	l2 = strlen(s2) + 1;
	/* alloc mem and initialize it if destination string is a null pointer */
	if (!(*s1)) *s1 = memget2(l2 * 2);
	/* get long pointer for extra datas */
	p2 = (unsigned long*)(*s1);
	/* maximum size of string memory */
	size = *(p2 - 2);
	/* allocate new bigger one if space is smaller than needed */
	if (size < l2){
		/* new mem */
		char *s3 = memget2(l2 * 2);
		free2(*s1);
		*s1 = s3;
		p2 = (unsigned long*)(*s1);
	}
	/* store length of new result string */
	*(p2 - 1) = l2 - 1;
	/* copy source to dest string */
	while(l2--) (*s1)[l2] = s2[l2];
}


/* my strcat for dynamic strings */
void strcat2(char **s1, const char *s2)
{
	unsigned long *p2, l1, l2, l3, size;
	/* safety check for null pointers */
	if (!s2) return;
	/* length of source string */
	l2 = strlen(s2) + 1;
	/* alloc mem and initialize it if destination string is a null pointer */
	if (!(*s1)) *s1 = memget2(l2 * 2);
	/* get long pointer for extra datas */
	p2 = (unsigned long*)(*s1);
	/* length of destination string */
	l1 = *(p2 - 1);
	/* total length needed to store result */
	l3 = l1 + l2;
	/* maximum size of string memory */
	size = *(p2 - 2);
	/* allocate new bigger one if space is smaller than needed */
	if (size < l3){
		/* new mem */
		char *s3 = memget2(l3 * 2);
		unsigned long l4 = l1 + 1;
		/* copy old string */
		while(l4--) s3[l4] = (*s1)[l4];
		free2(*s1);
		*s1 = s3;
		p2 = (unsigned long*)(*s1);
	}
	/* store length of new result string */
	*(p2 - 1) = l3 - 1;
	/* copy source behind dest string */
	while(l2--) (*s1)[l1 + l2] = s2[l2];
}


/* convert char to integer */
int string_ctoi(char c)
{
	char s[2];
	s[0] = c; s[1] = 0;
	return (atoi(s));
}


/* convert integer to char */
/* return modulus of dividing by 10 */
char string_itoc(int i)
{
	return ('0' + (i % 10));
}


/* convert integer to string */
/* returned value must be freed by caller */
char *string_itos(int num)
{
	char *res = memget2(max_num);
	sprintf(res, "%d", num);
	strlenset2(&res);
	return res;
}


/* convert integer to string and expand it with leading zeros */
/* returned value must be freed by caller */
char *string_itos_zeros(int num, int zeros)
{
	int l;
	char *res = memget2(max_num);
	sprintf(res, "%d", num);
	strlenset2(&res);

	/* get length of string */
	l = strlen2(&res);

	/* is string length less than the number of the desired length with leading zeros? */
	if (l < zeros){
		/* add leading zeroes */
		int c = zeros - l;
		if (c > 0){
			char *res2 = memget2(zeros);
			while(c--) strcat2(&res2, "0");
			strcat2(&res2, res);
			free2(res); res = res2;
		}
	}
	
	return res;
}


/* convert long integer to string */
/* returned value must be freed by caller */
char *string_ltos(unsigned long num)
{
	char *res = memget2(max_num);
	sprintf(res, "%ld", num);
	strlenset2(&res);
	return res;
}


/* return the first occurence of string containing only numbers */
/* returned value must be freed by caller */
char *string_get_number(const char *text)
{
	char *res;
	char c;
	int i = 0;
	int start = 0;
	int l;
	
	if (!text) return 0;
	
	/* go to the first number char */
	while(1){
		c = text[i];
		if (!c || c == '\n') return 0;
		if (c >= '0' && c <= '9') break;
		i++;
	}

	/* read only number chars and stop */
	start = i;
	while(1){
		c = text[i];
		if (c < '0' || c > '9') break;
		i++;
	}
	
	l = i - start;
	/* fail on zero length result */
	if (l < 1) return 0;

	/* allocate mem for the new string */
	res = memget2(l);
	/* copy word */
	i = l;
	while(i--){
		res[i] = text[start + i];
	}
	res[l] = 0;
	strlenset2(&res);

	return res;
}


/* return the last occurence of string containing only numbers */
/* returned value must be freed by caller */
char *string_get_number_last(const char *text)
{
	char *res;
	char c;
	int i = 0;
	int start, end;
	int l;
	
	if (!text) return 0;

	i = strlen(text);
	
	/* go to the first number char from the end */
	while(1){
		i--;
		if (i < 0) return 0;
		c = text[i];
		if (c >= '0' && c <= '9') break;
	}

	/* read only number chars and stop */
	end = i;
	while(1){
		c = text[i];
		if (c < '0' || c > '9') break;
		i--;
	}
	
	start = i + 1;
	l = end - start + 1;
	/* fail on zero length result */
	if (l < 1) return 0;

	/* allocate mem for the new string */
	res = memget2(l);
	/* copy word */
	i = l;
	while(i--){
		res[i] = text[start + i];
	}
	res[l] = 0;
	strlenset2(&res);

	return res;
}


/* return true if string consists of only numbers */
int string_is_number(const char *text)
{
	int i = 0;
	char c;
	int res = 1;

	if (!text) return 0;

	/* search for only num chars */
	while(1){
		c = text[i++];
		/* exit on null */
		if (!c) break;
		/* fail if not only numbers are in text */
		if (c < '0' || c > '9'){ res = 0; break; }
	}
	
	return res;
}


/* return a new string containing the next line of a string and move the pointer to the beginning of the this line */
/* returned value must be freed by caller */
char *string_get_next_line(char **text)
{
	char *res = 0, c;
	int i = 0;
	
	if (!(*text)) return 0;
	if (!(*text)[0]) return 0;

	while(1){
		c = (*text)[i];
		/* exit on end or on a new line */
		if (!c || c == '\n') break;
		i++;
	}

	/* copy only "i" chars */
	c = (*text)[i];
	(*text)[i] = 0;
	strcpy2(&res, (*text));
	(*text)[i] = c;
	/* move pointer to the next line, or leave it on null end */
	if (c) i++;
	(*text) += i;

	return res;
}


/* return a new string containing the last line of a string and move the pointer to the beginning of the this line */
/* returned value must be freed by caller */
char *string_get_last_line(char **text)
{
	char *res = 0, c;
	int i, l;

	/* get string length */	
	l = strlen((*text));
	/* return null if input string is null too */
	if (l < 1){
		res = memget2(1);
		return res;
	}
	
	/* go backwards till i find a new line */
	i = l - 1;
	while(1){
		/* last char may be new line, that still counts for the last line */
		i--;
		if (i < 0) break;
		c = (*text)[i];
		if (c == '\n'){ i++; break; }
	}
	if (i < 0) i = 0;
	l = l - i + 1;

	/* move pointer to the next line */
	(*text) += i;
	l--;
	/* copy onyl "l" chars */
	c = (*text)[l];
	(*text)[l] = 0;
	strcpy2(&res, (*text));
	(*text)[l] = c;

	return res;
}


/* return the length of the next line in a string */
int string_next_line_len(const char *text)
{
	int i = 0;
	while(1){
		char c = text[i];
		/* exit on end or on a new line */
		if (!c || c == '\n') return i;
		i++;
	}
}


/* move pointer to the beginning of next line in a string */
/* return null on fail */
int string_jump_next_line(char **text)
{
	char c;
	int i = 0;

	if (!(*text)) return 0;

	/* jump to next line */
	while(1){
		c = (*text)[i];
		/* exit on end and fail */
		if (!c){ (*text) += i; return 0; }
		/* exit on new line and success */
		if (c == '\n'){ (*text) += i + 1; return 1; }
		i++;
	}
}


/* move pointer to the beginning of next line in a string and jump count of lines more */
/* return null on fail */
int string_jump_next_linen(char **text, int count)
{
	int res = 0;
	int c = count;
	while((--c) >= 0) res = string_jump_next_line(text);
	
	return res;
}


/* return a new string containing the next word in a string and move the pointer to the beginning of the next word */
/* check only the current line, not the whole string */
/* returned value must be freed by caller */
char *string_get_next_word(char **text)
{
	char *res, c;
	int i = 0;
	int start, l;

	if (!(*text)) return 0;

	/* skip white spaces or exit on end of line */
	while (1){
		c = (*text)[i];
		if (!c || c == '\n') return 0;
		if (c != ' ' && c != '\t') break;
		i++;
	}

	start = i;
	while (1){
		/* get next char */
		c = (*text)[i];
		/* exit on new line */
		if (!c || c == '\n' || c == ' ' || c == '\t') break;
		i++;
	}
	
	l = i - start;
	/* fail on zero length result */
	if (l < 1) return 0;

	/* allocate mem for the new string */
	res = memget2(l);
	/* copy word */
	i = l;
	while(i--){
		res[i] = (*text)[start + i];
	}
	res[l] = 0;
	strlenset3(&res, l);
	/* skip more white spaces and move pointer to the next line */
	i = start + l;
	while (1){
		c = (*text)[i];
		if (c != ' ' && c != '\t') break;
		i++;
	}
	(*text) += i;

	return res;
}


/* return n. word from a line of string */
/* returned value must be freed by caller */
char *string_get_next_wordn(char **text, int num)
{
	char *res, c;
	int i = 0;
	int start = 0;
	int l;

	if (!(*text)) return 0;

	/* count num pieces of words */
	if (num < 0) num = 0;
	num++;
	while (num--){

		/* skip white spaces or exit on end of line */
		while (1){
			c = (*text)[i];
			/* exit on end */
			if (!c || c == '\n') return 0;
			if (c != ' ' && c != '\t') break;
			i++;
		}

		start = i;
		while (1){
			/* get next char */
			c = (*text)[i];
			/* exit on end if not the last word yet */
			if (!c || c == '\n'){
				if (num > 0) return 0;
				else break;
			}
			if (c == ' ' || c == '\t') break;
			i++;
		}
	}
	
	l = i - start;
	/* fail on zero length result */
	if (l < 1) return 0;

	/* allocate mem for the new string */
	res = memget2(l);
	/* copy word */
	i = l;
	while(i--){
		res[i] = (*text)[start + i];
	}
	res[l] = 0;
	strlenset3(&res, l);
	/* skip more white spaces and move pointer to the next line */
	i = start + l;
	while (1){
		c = (*text)[i];
		if (c != ' ' && c != '\t') break;
		i++;
	}
	(*text) += i;

	return res;
}


/* return last word from a line of string */
/* returned value must be freed by caller */
char *string_get_last_word(char **text)
{
	char *res, c;
	int i = 0;
	int start, end;
	int l;

	if (!(*text)) return 0;

	/* skip white spaces or exit on end of line */
	while (1){
		c = (*text)[i];
		/* exit on end */
		if (!c || c == '\n') return 0;
		if (c != ' ' && c != '\t') break;
		i++;
	}

	/* get length from here to the end of the line */
	start = i;
	while(1){
		c = (*text)[i];
		if (!c || c == '\n') break;
		i++;
	}
	
	/* get last word going backwards */
	i--;
	end = i;
	while(1){
		c = (*text)[i];
		if (c == ' ' || c == '\t' || i < start) break;
		i--;
	}
	i++;
	
	/* allocate mem for the new string */
	l = end - i + 1;
	res = memget2(l + 1);
	/* copy word */
	start = i;
	i = l;
	while(i--){
		res[i] = (*text)[start + i];
	}
	res[l] = 0;
	strlenset3(&res, l);
	/* set pointer to the beginning of the last word */
	(*text) += start;

	return res;
}


/* get the diff of two strings by adding "delete" keyword and return result */
/* returned value must be freed by caller */
char *string_get_diff(char *new, char *old)
{
	char *res, *temp;
	char *list = 0;
	
	temp = new;
	while(1){
		/* get next line */
		res = string_get_next_line(&temp);
		if (!res) break;
		/* skip empty lines */
		if (strlen2(&res)){
			/* does the new exist in the old one? */
			if (string_search_line(old, res) == -1){
				/* if not, then i add it */
				strcat2(&list, res);
				strcat2(&list, "\n");
			}
		}
		free2(res);
	}

	temp = old;
	while(1){
		/* get next line */
		res = string_get_next_line(&temp);
		if (!res) break;
		/* skip empty lines */
		if (strlen2(&res)){
			/* does the old exist in the new one? */
			if (string_search_line(new, res) == -1){
				/* if not, then i delete it */
				strcat2(&list, "delete ");
				strcat2(&list, res);
				strcat2(&list, "\n");
			}
		}
		free2(res);
	}

	return list;
}


/* search for a keyword from the beginning of a string */
/* returns true if success */
int string_search_keyword_first(char *text, char *key)
{
	char c1, c2;
	int i = 0;
	
	if (!text) return 0;

	while(1){
		c1 = text[i];
		c2 = key[i];
		if (!c2) return 1;
		if (!c1 || c1 != c2) return 0;
		i++;
	}
}


/* search for a keyword from the beginning of the lines in a string */
/* return char position if success or -1 on fail */
int string_search_keyword_first_all(char *text, char *key)
{
	char *res, *temp, *orig;
	
	if (!text) return -1;

	temp = text;
	while(1){
		orig = temp;
		/* get next line */
		res = string_get_next_line(&temp);
		if (!res) break;
		if (string_search_keyword_first(res, key)){
			free2(res);
			return (orig - text);
		}
		free2(res);
	}

	return -1;
}


/* search for a keyword from the end of a string */
/* returns true if success */
int string_search_keyword_last(const char *text, const char *key)
{
	char c1, c2;
	int i1, i2;
	long l1, l2;
	
	if (!text) return 0;

	l1 = strlen(text);
	l2 = strlen(key);
	if (l2 > l1) return 0;

	i1 = l1 - l2;
	i2 = 0;
	while(1){
		c1 = text[i1];
		c2 = key[i2];
		if (!c1) return 1;
		if (c1 != c2) return 0;
		i1++; i2++;
	}
}


/* search for a keyword in a string and return the position where it starts */
/* return -1 on fail */
int string_search_keyword(const char *text, const char *key)
{
	char c1, c2;
	int i, i2, start;

	if (!text) return -1;

	/* search for the keyword */
	i = 0;
	i2 = 0;
	start = 0;
	while(1){
		c1 = text[i];
		c2 = key[i2];
		/* stop on reaching end of keyword and success */
		if (!c2) return start;
		/* stop on end of text and fail */
		if (!c1) return -1;
		/* stop if no match or if end of text */
		if (c1 != c2){
			i2 = 0;
			start = i + 1;
		}
		else i2++;
		i++;
	}
	
	return -1;
}


/* search for a line in a string and return the position where it starts */
/* return -1 on fail */
int string_search_line(const char *text, const char *line)
{
	char c1, c2;
	int i, i2, start;
	
	/* fail on null pointer */
	if (!text) return -1;

	/* search for the line */
	i = 0;
	i2 = 0;
	start = 0;
	while(1){
		c1 = text[i];
		c2 = line[i2];
		/* stop on reaching end of line and end of string together and success */
		if (!c2 && (c1 == '\n' || !c1)) return start;
		/* stop on end of text and fail */
		if (!c1) return -1;
		/* jump to next line if no match */
		if (c1 != c2){
			while(1){
				c1 = text[i];
				/* fail if no more lines */
				if (!c1) return -1;
				if (c1 == '\n') break;
				i++;
			}
			i2 = 0;
			start = i + 1;
		}
		else i2++;
		i++;
	}
	
	return -1;
}


/* add a line to string if the line doesn't exist yet */
void string_add_line_uniq(char **text, const char *line)
{
	if (!line) return;

	/* list contains line? */
	if (string_search_line(*text, line) == -1){
		/* if not then add it */
		strcat2(text, line);
		strcat2(text, "\n");
	}
}


/* remove a whole line from a string and return the result */
/* returned value must be freed by caller */
char *string_remove_line(char *text, const char *line)
{
	char *res, *temp;
	char *text_new = memget2(strlen(text));
	
	temp = text;
	while(1){
		/* get next line */
		res = string_get_next_line(&temp);
		if (!res) break;
		/* compare it */
		if (strcmp(res, line)){
			/* add line to new string if no match */
			strcat2(&text_new, res);
			strcat2(&text_new, "\n");
		}
	}

	/* return result */
	return text_new;
}


/* remove a whole line from a string from a position and return the result */
/* returned value must be freed by caller */
char *string_remove_line_from_pos(char *text, unsigned int pos)
{
	char *new = 0, *temp;

	if (!text) return 0;

	/* copy original text to new */	
	strcpy2(&new, text);
	/* pos is within text? if not, then return whole text */
	if (pos > strlen(new)) return new;

	/* mark an end at pos */
	new[pos] = 0;
	strlenset3(&new, pos);
	
	/* move end to pos */
	temp = text + pos;
	if (string_jump_next_line(&temp)) strcat2(&new, temp);

	return new;
}


/* compare strings for qsort */ 
int string_cmp(const void *a, const void *b) 
{ 
	const char **ia = (const char **)a;
	const char **ib = (const char **)b;
	return strcmp(*ia, *ib);
}


/* sort lines of a string */
/* returned value must be freed by caller */
char *string_sort_lines(const char *text)
{
	char c, **ptr;
	char *text_new, *text_sort;
	int flag_newl = 0;
	int i, i2, count;
	int maxl;

	if (!text) return 0;

	/* return null on zero input */
	if (!text[0]) return (memget2(1));

	maxl = strlen(text) + 1;
	text_sort = memget2(maxl);
	/* create a copy of text for sorting */
	strcpy2(&text_sort, text);

	/* count lines */
	i = 0;
	count = 0;
	while(1){
		c = text[i++];
		/* exit on end */
		if (!c) break;
		if (c == '\n') count++;
	}

	/* alloc mem for char pointers pointing to string lines */
	ptr = memget_ptr(count);
	
	/* change new line chars to nulls and create pointer list to string lines */
	i = 0;
	i2 = 0;
	flag_newl = 0;
	while(1){
		c = text_sort[i];
		/* exit on end */
		if (!c) break;
		/* add pointer at first line or after every new line */
		if (!flag_newl){
			ptr[i2++] = text_sort + i;
			flag_newl = 1;
		}
		/* make a mark at new line and change new line to zero */
		if (c == '\n'){
			text_sort[i] = 0;
			flag_newl = 0;
		}
		i++;
	}
	
	/* sort it */
	qsort(ptr, count, sizeof(char *), string_cmp);

	/* create another string holder */
	text_new = memget2(maxl);

	/* collect lines back from pointer list */
	i = 0;
	i2 = count;
	while(i2--){
		char *s = ptr[i++];
		strcat2(&text_new, s);
		strcat2(&text_new, "\n");
	}
	free2(text_sort);
	free(ptr);

	return text_new;
}


/* sort lines of a string and make it uniq */
/* returned value must be freed by caller */
char *string_sort_uniq_lines(const char *text)
{
	char c, *res, *text_temp, **ptr;
	char *text_new, *text_final, *text_sort;
	char *res2 = 0;
	int flag_first = 0;
	int flag_newl = 0;
	int flag_diff = 0;
	int i, i2, l1, l2, count;
	int maxl;

	if (!text) return 0;

	/* return null on zero input */
	if (!text[0]) return (memget2(1));

	maxl = strlen(text) + 1;
	text_sort = memget2(maxl);
	/* create a copy of text for sorting */
	strcpy2(&text_sort, text);

	/* count lines */
	i = 0;
	count = 0;
	while(1){
		c = text[i++];
		/* exit on end */
		if (!c) break;
		if (c == '\n') count++;
	}

	/* alloc mem for char pointers pointing to string lines */
	ptr = memget_ptr(count);
	
	/* change new line chars to nulls and create pointer list to string lines */
	i = 0;
	i2 = 0;
	flag_newl = 0;
	while(1){
		c = text_sort[i];
		/* exit on end */
		if (!c) break;
		/* add pointer at first line or after every new line */
		if (!flag_newl){
			ptr[i2++] = text_sort + i;
			flag_newl = 1;
		}
		/* make a mark at new line and change new line to zero */
		if (c == '\n'){
			text_sort[i] = 0;
			flag_newl = 0;
		}
		i++;
	}
	
	/* sort it */
	qsort(ptr, count, sizeof(char *), string_cmp);

	/* create another string holder */
	text_new = memget2(maxl);

	/* collect lines back from pointer list */
	i = 0;
	i2 = count;
	while(i2--){
		char *s = ptr[i++];
		strcat2(&text_new, s);
		strcat2(&text_new, "\n");
	}
	free2(text_sort);
	free(ptr);

	/* create another string holder */
	text_final = memget2(maxl);

	/* make it uniq */
	text_temp = text_new;
	while(1){
		/* get next line */
		res = string_get_next_line(&text_temp);
		/* exit on end */
		if (!res){
			free2(res2);
			break;
		}

		/* first run */		
		if (!flag_first){
			l1 = strlen2(&res);
			strcat2(&text_final, res);
			strcat2(&text_final, "\n");
		}
		else{
			/* compare 2 lines */
			l1 = strlen2(&res);
			l2 = strlen2(&res2);
			flag_diff = 0;
			if (l1 != l2) flag_diff = 1;
			else if (strcmp(res, res2)) flag_diff = 1;
			/* store if different */
			if (flag_diff){
				strcat2(&text_final, res);
				strcat2(&text_final, "\n");
			}
			free2(res2);
		}
		
		res2 = res;
		flag_first = 1;
	}
	free2(text_new);

	return text_final;
}


/* copy the elements of the array to a string list and return the string */
/* returned value must be freed by caller */
char *array_copy_to_string_list(char **array)
{
	char *new = 0;
	char *ptr;
	
	while(1){
		ptr = *(array++);
		if (!ptr) break;
		strcat2(&new, ptr);
		strcat2(&new, "\n");
	}
	
	return new;
}


/* search for a keyword in an array and return pointer to it */
/* return 0 on fail */
char *array_search_keyword(char **array, const char *key)
{
	char *ptr;
	
	if (!key) return 0;
	
	while(1){
		ptr = *(array++);
		if (!ptr) return 0;
		if (!strcmp(ptr, key)) return ptr;
	}
}

