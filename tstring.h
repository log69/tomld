/*
----------------------------------------------------------------------
----------- tstring.h ------------------------------------------------
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_num		68


void free2(char *ptr);

int string_count_lines(const char *text);

void newl();

void newl_();

void debug(const char *text);

void debugi(int num);

void debugl(long num);

void debugf(float num);

void debugp(char *p);

void error(const char *text);

char *memget2(unsigned long num);

char *memget(unsigned long num);

char **memget_ptr(unsigned long num);

long *memget_long(unsigned long num);

unsigned long strlen2(char **s1);

void strlenset2(char **s1);

void strlenset3(char **s1, unsigned long l);

void strnull2(char **s1);

void strcpy2(char **s1, const char *s2);

void strcat2(char **s1, const char *s2);

int string_ctoi(char c);

char string_itoc(int i);

char *string_itos(int num);

char *string_ltos(unsigned long num);

char *string_get_number(const char *text);

char *string_get_number_last(const char *text);

int string_is_number(const char *text);

char *string_get_next_line(char **text);

char *string_get_last_line(char **text);

int string_next_line_len(const char *text);

int string_jump_next_line(char **text);

char *string_get_next_word(char **text);

char *string_get_next_wordn(char **text, int num);

char *string_get_last_word(char **text);

char *string_get_diff(char *new, char *old);

int string_search_keyword_first(const char *text, const char *key);

int string_search_keyword_last(const char *text, const char *key);

int string_search_keyword(const char *text, const char *key);

int string_search_line(const char *text, const char *line);

char *string_remove_line(char *text, const char *line);

int string_cmp(const void *a, const void *b);

char *string_sort_uniq_lines(const char *text);

char *array_copy_to_string_list(char **array);

char *array_search_keyword(char **array, const char *key);

