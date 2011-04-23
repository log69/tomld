/*
----------------------------------------------------------------------
----------- tomld (tomoyo learning daemon) ---------------------------
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

changelog:
-----------
21/04/2011 - tomld v0.31 - complete rewrite of tomld from python to c language
                         - drop platform check
16/04/2011 - tomld v0.30 - bugfix in recursive dir handling
                         - use special recursive wildcard in dir handling that is available since tomoyo version 2.3
14/04/2011 - tomld v0.29 - print error messages and extra info to stderr instead of stdout
                           so to print only rules into a file is easy now: tomld -i pattern 1>output
                         - bugfix: don't count additional programs more than once if the same is specified more times
                         - bugfix: check running instance at the very beginning of the program
                         - bugfix: adding extra check for existence and content of manager.conf
                         - mark all shells in /etc/shells as domain exceptions if their binary exist
                         - bugfix in removing deleted entries
                         - change in recursive dir handling
                         - add custom default recursive directories that may have random part in their names
                           a dir like this is /var/run/gdm/ of gdm3
                         - support in settings for debian testing and ubuntu beta is removed temporary
                           because of bugs in those versions until they get fixed
                         - improve checking profile config
                         - reenable ubuntu beta support
07/04/2011 - tomld v0.28 - change quit method from ctrl+c to q key
                         - bugfix: do not turn on enforcing mode for newly created domains
                         - add compatibility to tomoyo version 2.3
05/04/2011 - tomld v0.27 - rewrite domain cleanup function
                         - speed up the new domain cleanup function by skipping rules reading libs
                         - add feature: check rules only if they changed and avoid unnecessary work
                         - improve info() function: show main domains and subdomains with different colors
                         - major bugfixes
03/04/2011 - tomld v0.26 - improve domain cleanup function
                         - improve info function
                         - bugfixes
                         - add --learn switch to turn learning mode back for all domains on mistake
02/04/2011 - tomld v0.25 - more major bugfixes
                         - add sand clock to see when check rutin is working, so we can stop it while sleeping
                         - some code cleanup
                         - speed up compare rutins a bit
31/03/2011 - tomld v0.24 - major bugfixes
                         - improve domain cleanup function by making the rules more unique
29/03/2011 - tomld v0.23 - add feature to try to detect temporary names and wildcard them
                         - major bugfixes
28/03/2011 - tomld v0.22 - speed up info and remove functions
                         - add full recursive directory subtitution with wildcards
                         - several bugfix
27/03/2011 - tomld v0.21 - more bugfixes and code cleanup
                         - change in structure: from now allow_mkdir will cause the file's parent dir to be wildcarded too
                         - change in structure: i created an exception list for the dirs, so the policy gets a bit tighter with this
                           exception list contains now = "/etc", "/home/any/"
26/03/2011 - tomld v0.20 - minor bugfixes
                         - some code cleanup
25/03/2011 - tomld v0.19 - create policy file backups only with --reset or --clear switches
                         - print info when backups are created
                         - expand documentation
                         - handle error message type "domain not defined" in log
24/03/2011 - tomld v0.18 - create profile.conf file on startup if missing
                         - set maximum accept entry value in profile.conf to a predefined one
                         - add --once switch to quit after first cycle immediately (might be useful for scripts)
                         - don't print confirmation text at the end of log messages with --yes on
                         - don't print sleeping period with --once on
23/03/2011 - tomld v0.17 - add --keep switch to run tomld without turning learning mode domains into enforcing mode
                         - add --recursive switch to mark subdirs of these dirs fully wildcarded (usable for samba shares for example)
                         - on rules with allow_create entries, create the same with allow_unlink and allow_read/write too
                         - ask for confirmation on adding system logs to rules (--yes switch is usable here to bypass this)
22/03/2011 - tomld v0.16 - add --remove switch to remove domains by a pattern on demand
                         - add --yes switch for auto confirmation
                         - function for --info switch rewritten to speed up search
                         - remove rules and domains marked as (deleted) on startup
                           (thanks to Ritesh Raj Sarraf)
                         - debian kernel has Tomoyo enabled already, fix package check (only tomoyo-tools is needed)
                           (thanks to Ritesh Raj Sarraf)
                         - bugfix: variables of config files were not initialized
21/03/2011 - tomld v0.15 - remove disabled mode domains automatically to speed up the things
                         - change directory parameters' handling in rule reshape code to speed up more
                         - show statistics about active domains and rules on exit
20/03/2011 - tomld v0.14 - bugfix: only first log message was converted to rule
18/03/2011 - tomld v0.13 - additional major bugfixes
18/03/2011 - tomld v0.12 - add --info switch to print the specified domain's rules and bugfixes
17/03/2011 - tomld v0.11 - print domain info only once and print changes continously
17/03/2011 - tomld v0.10 - put allow_rename on the wildcard list too (same as allow_create) and other major changes
16/03/2011 - tomld v0.09 - only old domains with profile 1-2 will be turned into enforcing mode on exit
15/03/2011 - tomld v0.08 - tested version with minor bugfixes
15/03/2011 - tomld v0.07 - create wildcard for newly created files and libfile-0.1.2.so.3.4 library version numbers
14/03/2011 - tomld v0.06 - major bugfixes
12/03/2011 - tomld v0.05 - first fully working version with policy reshape
09/03/2011 - tomld v0.04 - get access denied logs and add them to policy
04/03/2011 - tomld v0.03 - manage and check policy and rules
03/03/2011 - tomld v0.02 - discover which running processes send or recieve network packets
28/02/2011 - tomld v0.01 - basic program structure
27/02/2011 - project start


flow chart:
------------

	1)	init
	2)	search for new processes using network
	3)	sleep short
	3)	while counter-- goto 2 (means long sleep for check)
	4)	load policy
	5)	check prolicy for domain and all its subdomains
		if disabled mode
			turn on learning mode
		if learning mode
			no change
		if permissive mode
			no change
		if enforcing mode
			no change
	6)	get recent log
	7)	convert logs to rules and add them to policy (confirm)
	8)	switch back domains with access deny from enforcing to learning mode until exit
	9)	reshape policy rules:
			domain config cleanup (sort and unique)
			wildcard subdirs of recursive dirs (if any)
			collect dir names of create rules (that are not in exception list)
			collect dir names to mkdir (that are not in exception list)
			wildcard formerly collected dir names (only leaf file or dir name)
			wildcard collected mkdir names' leaf and one parent dir too
			wildcard library files version numbers
			wildcard pid number in /proc/number/ dirs
			wildcard user name in /home/user/ dirs
			domain config cleanup (sort and unique)
			wildcard files and dirs with changing names that are not wildcarded yet
				(this means dirs in the exception list)
			domain config cleanup (sort and unique)
			readd every create rule as unlink, read/write etc. too
			domain config cleanup (sort and unique)
	10)	save policy
	11) goto 2
	12) on exit, turn all old domains with profile 1-2 into enforcing mode

*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>


#define max_char	1024
#define max_num		32
#define max_array	1024
#define max_file	10*1024*1024


/* ------------------------------------------ */
/* ------------ GLOBAL VARIABLES ------------ */
/* ------------------------------------------ */

/* program version */
char *ver = "0.31";

/* home dir */
char *home = "/home";

/* policy check interval in seconds */
int count = 10;

/* max entries variable in profile config */
#define maxmem "10000"

/* tomoyo kernel parameter */
char *tkern = "security=tomoyo";

/* tomoyo vars and files */
char *tdomf	= "";
char *texcf = "";

/* default profile.conf */
char *tprof22 = ""
	"0-COMMENT=-----Disabled Mode-----\n"
	"0-MAC_FOR_FILE=disabled\n"
	"0-MAX_ACCEPT_ENTRY="maxmem"\n"
	"0-TOMOYO_VERBOSE=disabled\n"
	"1-COMMENT=-----Learning Mode-----\n"
	"1-MAC_FOR_FILE=learning\n"
	"1-MAX_ACCEPT_ENTRY="maxmem"\n"
	"1-TOMOYO_VERBOSE=disabled\n"
	"2-COMMENT=-----Permissive Mode-----\n"
	"2-MAC_FOR_FILE=permissive\n"
	"2-MAX_ACCEPT_ENTRY="maxmem"\n"
	"2-TOMOYO_VERBOSE=enabled\n"
	"3-COMMENT=-----Enforcing Mode-----\n"
	"3-MAC_FOR_FILE=enforcing\n"
	"3-MAX_ACCEPT_ENTRY="maxmem"\n"
	"3-TOMOYO_VERBOSE=enabled\n";

char *tprof23 = ""
	"PROFILE_VERSION=20090903\n"
	"PREFERENCE::enforcing={ verbose=yes }\n"
	"PREFERENCE::learning={ verbose=no max_entry="maxmem" }\n"
	"PREFERENCE::permissive={ verbose=yes }\n"
	"0-COMMENT=-----Disabled Mode-----\n"
	"0-CONFIG={ mode=disabled }\n"
	"1-COMMENT=-----Learning Mode-----\n"
	"1-CONFIG={ mode=learning }\n"
	"2-COMMENT=-----Permissive Mode-----\n"
	"2-CONFIG={ mode=permissive }\n"
	"3-COMMENT=-----Enforcing Mode-----\n"
	"3-CONFIG={ mode=enforcing }\n";

/* default manager.conf */
char *tmanf22 = ""
	"/usr/sbin/tomoyo-loadpolicy\n"
	"/usr/sbin/tomoyo-editpolicy\n"
	"/usr/sbin/tomoyo-setlevel\n"
	"/usr/sbin/tomoyo-setprofile\n"
	"/usr/sbin/tomoyo-ld-watch\n";

char *tmanf23 = ""
	"/usr/sbin/tomoyo-loadpolicy\n"	
	"/usr/sbin/tomoyo-editpolicy\n"
	"/usr/sbin/tomoyo-setlevel\n"
	"/usr/sbin/tomoyo-setprofile\n"
	"/usr/sbin/tomoyo-ld-watch\n"
	"/usr/sbin/tomoyo-queryd\n";

char *tdir	= "/etc/tomoyo";
char *tdom	= "/etc/tomoyo/domain_policy.conf";
char *texc	= "/etc/tomoyo/exception_policy.conf";
char *tpro	= "/etc/tomoyo/profile.conf";
char *tman	= "/etc/tomoyo/manager.conf";
char *tinit	= "/usr/lib/tomoyo/tomoyo_init_policy";
char *tload	= "/usr/sbin/tomoyo-loadpolicy";
char *tsave	= "/usr/sbin/tomoyo-savepolicy";

/* system log */
char *tlog	= "/var/log/syslog";

/* backup conf for checking change of rules */
char *tdomf_bak = "";

/* this stores the kernel time of last line of the system log */
/* to identify it and make sure not to read it twice */
char *tmark	= "/var/local/tomld.logmark";
/* tomld pid file */
char *pidf	= "/var/run/tomld.pid";

/* options */
int opt_version		= 0;
int opt_help		= 0;
int opt_color		= 0;
int opt_learn		= 0;
int opt_clear		= 0;
int opt_reset		= 0;
int opt_info		= 0;
int opt_remove		= 0;
int opt_yes			= 0;
int opt_keep		= 0;
int opt_recursive	= 0;
int opt_once		= 0;

char opt_info2     [max_char]  = "";
char opt_remove2   [max_char]  = "";
char opt_recursive2[max_array][max_char];
int  opt_recursive2_counter = 0;

int flag_reset		= 0;
int flag_check		= 0;
int flag_check2		= 0;
int flag_check3		= 0;
int flag_firstrun	= 1;
int flag_clock		= 0;
int flag_safe		= 0;
int flag_kernel_version = 0;

/* colors */
char *gray		= "\033[37;48m";
char *cyan		= "\033[36;48m";
char *purple	= "\033[35;48m";
char *blue		= "\033[34;48m";
char *green		= "\033[32;48m";
char *red		= "\033[31;48m";
char *yellow	= "\033[33;48m";
char *bold		= "\033[1m";
char *clr		= "\033[0m";

/* arrays */
char progs[max_array][max_char];
int  progs_counter = 0;

char netf[][max_char] = {"/proc/net/tcp", "/proc/net/tcp6", "/proc/net/udp", "/proc/net/udp6"};
int  netf_counter = 4;

char shell[][max_char] = {"/bin/bash", "/bin/csh", "/bin/dash", "/bin/ksh", "/bin/rbash",
"/bin/sh", "/bin/tcsh", "/usr/bin/es", "/usr/bin/esh", "/usr/bin/fish",
"/usr/bin/ksh", "/usr/bin/rc", "/usr/bin/screen"};
int  shell_counter = 13;

/* other vars */
struct termio terminal_backup;



/* ----------------------------------- */
/* ------------ FUNCTIONS ------------ */
/* ----------------------------------- */

/* print version info */
void version() {
	printf ("tomld (tomoyo learning daemon) "); printf(ver); printf("\n");
	printf ("Copyright (C) 2011 Andras Horvath\n");
	printf ("E-mail: mail@log69.com - suggestions & feedbacks are welcome\n");
	printf ("URL: http://log69.com - the official site\n");
	printf ("\n");
	printf ("LICENSE:\n");
	printf ("This program is free software; you can redistribute it and/or modify it ");
	printf ("under the terms of the GNU General Public License as published by the ");
	printf ("Free Software Foundation; either version 3 of the License, ");
	printf ("or (at your option) any later version.\n");
	printf ("\n");
	printf ("This program is distributed in the hope that it will be useful, ");
	printf ("but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY ");
	printf ("or FITNESS FOR A PARTICULAR PURPOSE.  ");
	printf ("See the GNU General Public License for more details.\n");
	printf ("\n");
	printf ("You should have received a copy of the GNU General Public License along with this program.  ");
	printf ("If not, see <http://www.gnu.org/licenses/>.\n");
	printf ("\n");
}


/* print help info */
void help() {
	printf ("DESCRIPTION:\n");
	printf ("tomld (tomoyo learning daemon) is an extension to the Tomoyo security module ");
	printf ("to ease and automate the setup process of a MAC (Mandatory Access Control) aware system ");
	printf ("to increase security of the system by closing applications and services into a domain with their rules. ");
	printf ("It helps the user harden his system easily. ");
	printf ("Starts domains in learning mode, collects rules, then later changes these rules and enforces the policy.\n");
	printf ("\n");
	printf ("Tomoyo module contains already the learning ability to automatically setup rules ");
	printf ("while the processes are running in learning mode. Only, there are continously changing ");
	printf ("files and directory names that the processes use, so those have to be replaced with wildcarded rules. ");
	printf ("This needs a rather advanced knowledge of the system.\n");
	printf ("\n");
	printf ("This tool tries to fully automate the MAC setup, hoping the average users ");
	printf ("would be able to use a much more secure system on their own with minimal efforts.\n");
	printf ("\n");
	printf ("Tomoyo security module is part of the mainline Linux kernel since version 2.6.30. ");
	printf ("Currently this solution targets Linux systems only.\n");
	printf ("\n");
	printf ("*TOMOYO(r) is a registered trademark of NTT DATA Corporation\n");
	printf ("\n");
	printf ("LINKS:\n");
	printf ("http://tomoyo.sourceforge.jp\n");
	printf ("http://tomoyo.sourceforge.jp/documentation.html.en\n");
	printf ("http://tomoyo.sourceforge.jp/2.2/index.html.en\n");
	printf ("http://en.wikipedia.org/wiki/Mandatory_access_control\n");
	printf ("\n");
	printf ("USAGE: tomld.py [options] [executables]\n");
	printf ("\n");
	printf ("The following options are supported:\n");
	printf ("    -h   --help             print this help\n");
	printf ("    -v   --version          print version information\n");
	printf ("    -c   --color            colorized output\n");
	printf ("         --clear            clear domain configurations\n");
	printf ("         --reset            reinitialize domain configurations\n");
	printf ("                            (all previously learnt rules will be backed up)\n");
	printf ("    -i   --info   [pattern] print domains' rules by pattern\n");
	printf ("                            (without pattern, print a list of main domains)\n");
	printf ("    -l   --learn            turn learning mode back for all domains\n");
	printf ("                            (this is not advised, only for correction purposes)\n");
	printf ("    -r   --remove [pattern] remove domains by pattern\n");
	printf ("         --yes              auto confirm with yes\n");
	printf ("    -k   --keep             don't change domain's mode for this run\n");
	printf ("                            (learning mode domains will stay so on exit)\n");
	printf ("    -R   --recursive [dirs] replace subdirs of dirs in rules with wildcards\n");
	printf ("    -1   --once             quit after first cycle\n");
	printf ("                            (might be useful for scripts)\n");
	printf ("\n");
	printf ("*executables are additonal programs to create domains for\n");
	printf ("\n");
	printf ("requirements: Tomoyo enabled kernel (v2.6.30 and above) and tomoyo-tools (v2.2 and above)\n");
	printf ("\n");
	printf ("REMARKS:\n");
	printf ("- it is written in python, tested with version 2.6\n");
	printf ("- the program assumes to be run in a fully trusted environment\n");
	printf ("- it is advised to start with new rules instead of keeping any manually created ones\n");
	printf ("- processes in the domains should run in learning mode for quite some time to collect ");
	printf ("all the necessary rules to avoid malfunctioning by access deny later\n");
	printf ("- before reboot, the rules should be stored manually by running \"tomoyo-savepolicy\" ");
	printf ("or \"tomld -k -1\" as root if there are still domains in learning mode, ");
	printf ("because Tomoyo forgets the learned rules on system restart\n");
	printf ("- enforcing mode should never be switched back off for security reason\n");
	printf ("- exception domains means programs with no rules at all\n");
	printf ("- programs should be used in a maximum possible way during the learning phase ");
	printf ("to force a rule for all kinds of behave\n");
	printf ("- on --reset switch the config files will be backed up and the former log entries ");
	printf ("still won't be considered, so it really means a new start\n");
	printf ("- the program can be stopped any time by pressing q whereafter all old domains (learning and permissive) ");
	printf ("are turned into enforcing mode\n");
	printf ("- tomld files: "); printf (tmark); printf (" (this contains a mark to identify the end of the recently read message logs), ");
	printf (pidf); printf(" (pid file to avoid multpiple instances of the program to be running at the same time)\n");
	printf ("- in case a software or its settings change with a new version, the rules can be regenerated easily\n");
	printf ("- the running processes need to be restarted on newly created domains\n");
	printf ("\n");
	printf ("HOW TO USE:\n");
	printf ("- download, move it to a directory that root owns only, change its owner to root, ");
	printf ("change writeable only by root, make it executable, and run it as root\n");
	printf ("- run it the 1st time, now it will offer to install all missing packages and setup grub with kernel parameter\n");
	printf ("- system reboot is necessary now\n");
	printf ("- run it the 2nd time, now it will create domains for the processes automatically with learning mode\n");
	printf ("- stop it now, cause the domains will stay in learning mode and the tomoyo module will collect all rules\n");
	printf ("- reboot the system or at least all the services and applications that have domains now\n");
	printf ("- hours or days later let's run it the 3rd time, now the access denied logs will be converted to rules, ");
	printf ("and on exit all remaining domains will be turned into enforcing mode\n");
	printf ("\n");
}


/* my exit point and free mem */
void myexit(int num)
{
	free(tdomf);
	free(texcf);
	exit(num);
}


/* print a new line */
void newl()
{
	printf("\n");
}


/* print a new line to stderr */
void newl_()
{
	fprintf(stderr, "%s", "\n");
	fflush(stderr);
}


/* print debug info about a string */
void debug(char *text)
{
	long i;
	long c = 0;
	long l = strlen(text);
	/* count lines of text */
	for (i = 0; i < l; i++){ if (text[i] == '\n') c++; }
	/* print text and info */
	printf("--\n");
	printf(text);
	/* print newline if missing from the end of string */
	if (text[l-1] != '\n') newl();
	printf("-- debug bytes %ld and ", strlen(text));
	printf("lines %ld\n", c);
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


/* convert integer to ascii */
/* custom implementation because "itoa" function is missing from standard libs */
void itoa(char *buf, int num)
{
	sprintf(buf, "%d", num);
}


/* convert char to integer */
int ctoi(char c)
{
	char s[2];
	s[0] = c; s[1] = 0;
	return (atoi(s));
}


/* print colored output to stdout */
void color(char *text, char *col)
{
	if (opt_color){
		fprintf(stdout, "%s", col);
		fprintf(stdout, "%s", text);
		fprintf(stdout, "%s", clr);
		fflush(stdout);
	}
	else{
		fprintf(stdout, "%s", text);
		fflush(stdout);
	}
}


/* print colored output to stderr */
void color_(char *text, char *col)
{
	if (opt_color){
		fprintf(stderr, "%s", col);
		fprintf(stderr, "%s", text);
		fprintf(stderr, "%s", clr);
		fflush(stderr);
	}
	else{
		fprintf(stderr, "%s", text);
		fflush(stderr);
	}
}


/* allocate memory and return pointer */
char *memory_get(long num)
{
	char* p = malloc((sizeof(char)) * (num + 1));
	if (!p){ color_("error: out of memory\n", red); myexit(1); }
	/* clear first byte */
	p[0] = 0;
	return p;
}


/* return a new string containing the next line of a string and move the pointer to the beginning of the next line */
/* returned value must be freed by caller */
char *string_get_next_line(char **text)
{
	char *res, c;
	int i = 0;

	if (!(*text)[0]) return 0;

	do{
		c = (*text)[i++];
	/* exit on end or on a new line */
	} while(c && c != '\n');

	/* allocate mem for the new string */
	res = memory_get(i);
	/* copy it */
	strncpy(res, (*text), i-1);
	res[i-1] = 0;
	/* move pointer to the next line */
	(*text) += i;

	return res;
}


/* return the length of the next line in a string */
int string_next_line_len(char *text)
{
	int i = 0;
	while(1){
		char c = text[i];
		/* exit on end or on a new line */
		if (!c || c == '\n') return i;
		i++;
	}
}


/* move pointer to the beginning of next line in string skipping empty lines */
/* return null on fail */
int string_jump_next_line(char **text)
{
	char c;
	int i = 0;

	/* jump to next line */
	while(1){
		c = (*text)[i++];
		/* exit on end */
		if (!c) return 0;
		/* exit on new line */
		if (c == '\n') break;
	}

	/* skip new lines */
	while(1){
		c = (*text)[i];
		/* exit on end */
		if (!c) return 0;
		/* exit on new line */
		if (c != '\n') break;
		i++;
	}

	/* success, move pointer */
	(*text) += i;
	return 1;
}


/* return a new string containing the next word in a string and move the pointer to the beginning of the next word */
/* check only the current line, not the whole string */
/* returned value must be freed by caller */
char *string_get_next_word(char **text)
{
	char *res, c;
	int i = 0;
	int start, l;

	/* skip white spaces or exit on end of line */
	while (1){
		c = (*text)[i];
		if (!c || c == '\n') return 0;
		if (c != ' ') break;
		i++;
	}

	start = i;
	while (1){
		/* get next char */
		c = (*text)[i];
		/* exit on new line */
		if (!c || c == '\n' || c == ' ') break;
		i++;
	}
	
	l = i - start;
	/* fail on zero length result */
	if (!l) return 0;

	/* allocate mem for the new string */
	res = memory_get(l);
	/* copy word */
	i = l;
	while(i--){
		res[i] = (*text)[start + i];
	}
	res[l] = 0;
	/* skip more white spaces and move pointer to the next line */
	i = start + l;
	while (1){
		c = (*text)[i];
		if (c != ' ') break;
		i++;
	}
	(*text) += i;

	return res;
}


/* return n. word from a line of string */
/* returned value must be freed by caller */
char *string_get_next_wordn(char **text, int num)
{
	char *res;
	
	while(1){
		res = string_get_next_word(text);
		if (!res) return 0;
		if (num <= 0) return res;
		free(res);
		num--;
	}
}


/* search for a keyword in a string and return the position where it starts */
/* return -1 if no keyword found */
int string_search_keyword(char *text, char *key)
{
	char c1, c2;
	int i, i2, start;

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
	
	return 0;
}


/* qsort string comparison function */ 
int string_cmp(const void *a, const void *b) 
{ 
	const char **ia = (const char **)a;
	const char **ib = (const char **)b;
	return strcmp(*ia, *ib);
}


/* sort lines of a string and make it uniq */
/* returned value must be freed by caller */
char *string_sort_uniq_lines(char *text)
{
	char c, *res, *text_orig, **ptr;
	char *text_new, *text_final, *text_sort;
	char *res2 = "";
	int flag_first = 0;
	int flag_newl = 0;
	int flag_diff = 0;
	int i, i2, l1, l2, count;
	int maxl = strlen(text);
	
	/* create a copy of text for sorting */
	text_sort = memory_get(maxl);
	strncpy(text_sort, text, maxl);

	/* count lines */
	i = 0;
	count = 0;
	while(1){
		c = text[i++];
		/* exit on end */
		if (!c) break;
		if (c == '\n') count++;
	}

	/* create array of char pointers pointing to string lines */
	ptr = malloc((sizeof(char**)) * (count + 1));
	if (!ptr){ color_("error: out of memory\n", red); myexit(1); }
	/* clear first byte */
	ptr[0] = 0;
	
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
	text_new = memory_get(maxl);

	/* collect lines back from pointer list */
	i = 0;
	i2 = count;
	while(i2--){
		char *s = ptr[i++];
		strncat(text_new, s, strlen(s));
		strncat(text_new, "\n", 1);
	}
	free(text_sort);

	/* create another string holder */
	text_final = memory_get(maxl);

	/* make it uniq */
	text_orig = text_new;
	while(1){
		/* get next line */
		res = string_get_next_line(&text_orig);
		/* exit on end */
		if (!res){
			if (flag_first) free(res2);
			break;
		}

		/* first run */		
		if (!flag_first){
			l1 = strlen(res);
			strncat(text_final, res, l1);
			strncat(text_final, "\n", 1);
		}
		else{
			/* compare 2 lines */
			l1 = strlen(res);
			l2 = strlen(res2);
			flag_diff = 0;
			if (l1 != l2) flag_diff = 1;
			else if (strncmp(res, res2, l1)) flag_diff = 1;
			/* store if different */
			if (flag_diff){
				strncat(text_final, res, l1);
				strncat(text_final, "\n", 1);
			}
			free(res2);
		}
		
		res2 = res;
		flag_first = 1;
	}
	free(text_new);

	return text_final;
}


/* return a new string containing the next domain entry and move the pointer to the beginning of the next domain */
/* returned value must be freed by caller */
char *domain_get_next(char **text)
{
	char *key = "<kernel>";
	long keyl = strlen(key);
	long l;
	char *text2, *res;
	int i, start, end;

	if (!(*text)[0]) return 0;

	/* search for first keyword */
	text2 = (*text);
	start = string_search_keyword(text2, key);
	
	/* fail if no match */
	if (start == -1) return 0;

	/* move position to the end of keyword and search for second keyword */
	text2 += start + keyl;
	end = string_search_keyword(text2, key);
	/* if reached the end of text, then use that as the remaining part of domain block */
	if (end == -1){
		end = 0;
		while(text2[end++]);
	}
	
	/* domain block length */
	l = keyl + end - 1;

	/* allocate mem for the new string */
	res = memory_get(l);
	/* copy mem block */
	i = l;
	while(i--){
		res[i] = (*text)[start + i];
	}
	res[l] = 0;
	
	/* move pointer to the next line */
	(*text) += start + l;

	return res;
}


/* return profile number of domain */
int domain_get_profile(char *text)
{
	int i, p;
	char *key = "use_profile ";
	char *res, *orig;
	
	while(1){
		orig = text;

		/* get next line */
		res = string_get_next_line(&text);
		if (!res) break;

		/* search for the keyword */
		i = string_search_keyword(res, key);
		free(res);

		/* if match */
		if (i > -1){
			i += strlen(key);
			p = ctoi(orig[i]);
			return p;
		}
	}
	
	return 0;
}


/* ------------------------------------------------------------------------------------- */


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
	if (key == 0) c = '\0';
	key_clear_mode();
	return c;
}


/* give a choice */
int choice(char *text)
{
	char c = 0;
	printf(text);
	printf(" [y/N]");
	scanf("%c", &c);
	if (c == 'y') return 1;
	return 0;
}


/* open pipe and read content with given length */
char *pipe_read(char *comm, long length)
{
	char *buff;
	
	/* open file for reading */
	FILE *p = popen(comm, "r");
	if (!p){
		color_("error: cannot read pipe from command: ", red);
		color_(comm, red); newl();
		myexit(1);
	}
	
	/* alloc mem for it */
	buff = memory_get(length);
	memset(buff, 0, length);
	/* read pipe */
	fread(buff, length, 1, p);
	pclose(p);
	
	return buff;
}


/* open file and read content with given length, or if length is null, then give back file length to caller */
char *file_read(char *name, long *length)
{
	char *buff;
	long len = 0;

	/* open file for reading */
	FILE *f = fopen(name, "r");
	if (!f){
		color_("error: cannot read file ", red);
		color_(name, red); newl();
		myexit(1);
	}
	/* check file length */
	if (!(*length)){
		/* get file length from file descriptor */
		fseek(f, 0, SEEK_END);
		len = ftell(f) + 1;
		fseek(f, 0, SEEK_SET);
		/* store file length for caller */
		*length = len;
	}
	/* length is manually set */
	else{
		len = *length;
	}

	/* alloc mem */
	buff = memory_get(len);
	memset(buff, 0, len);
	/* read file */
	fread(buff, len, 1, f);
	fclose(f);
	/* write null to the end of file */
	buff[len] = 0;

	return buff;
}


/* open file and write content */
void file_write(char *name, char *buff)
{
	FILE *f = fopen(name, "w");
	if (!f){
		color_("error: cannot write file ", red);
		color_(name, red); newl();
		myexit(1);
	}
	fprintf(f, buff);
	fclose(f);
}


/* get kernel version in a format where 2.6.33 will be 263300 */
int kernel_version()
{
	char v[] = "/proc/sys/kernel/osrelease";
	char *buff;
	int c, c2;
	int ver = 0;
	char n;
	/* define length for file manually because it's a kernel file from /proc and doesn't have real length */
	long len = max_char;
	/* read in kernel version from /proc in a format as 2.6.32-5-amd64 */
	buff = file_read(v, &len);
	c = 0;
	/* create in version numbers */
	c2 = 100000;
	while (c2){
		n = buff[c++];
		if (n != '.'){
			/* exit if no numbers or end of string */
			if (!(n >= '0' && n <= '9') || !n) return ver;
			/* convert string to integer and add it to result */
			ver += c2 * ctoi(n);
			c2 /= 10;
		}
	}
	
	free(buff);

	return ver;
}


/* print sand clock */
void sand_clock(int dot)
{
	if (dot == 1){
		if (!flag_firstrun){
			printf(".");
			flag_clock = 0;
		}
	}
	else if (dot){
		if (!flag_firstrun){
			printf("+");
			flag_clock = 0;
		}
	}
	else{
		int c = flag_clock % 4;
		if (c == 0) printf("-\b");
		if (c == 1) printf("\\\b");
		if (c == 2) printf("|\b");
		if (c == 3) printf("/\b");
		flag_clock += 1;
	}
	fflush(stdout);
}


/* check if process with path is running */
int running(char *name){
	DIR *mydir;
	struct dirent *mydir_entry;
	char mydir_name[max_char] = "";
	char mypid[max_num] = "";
	
	/* open /proc dir */
	mydir = opendir("/proc/");
	if (!mydir){ color_("error: cannot open /proc/ directory\n", red); return 0; }
	/* cycle through dirs in /proc */
	while((mydir_entry = readdir(mydir))) {
		/* get the dir names inside /proc dir */
		strncpy(mypid, mydir_entry->d_name, max_char);
		/* does it contain numbers only meaning they are pids? */
		if (strspn(mypid, "0123456789") == strlen(mypid)) {
			int res;
			char buff[max_char] = "";
			/* create dirname like /proc/pid/exe */
			strncpy(mydir_name, "/proc/", max_char);
			strncat(mydir_name, mypid, max_char);
			strncat(mydir_name, "/exe", max_char);
			/* resolv the link pointing from the exe name */
			res = readlink(mydir_name, buff, max_char - 1);
			if (res > 0){
				/* put a null end mark to the end of the string after string length */
				buff[res] = 0;
				/* compare the link to the process name */
				/* gives back null on full match */
				if (strncmp(buff, name, max_char) == 0) {
					closedir(mydir);
					return 1;
				}
			}
		}
	}
	closedir(mydir);

	return 0;
	
}


/* check if dir exists */
int dir_exist(char *name){
	DIR *d;
	d = opendir(name);
	if (d) { closedir(d); return 1; }
	return 0;
}


/* check if file exists */
int file_exist(char *name){
	FILE *f = fopen(name, "r+");
	if (!f) return 0;
	fclose(f);
	return 1;
}


/* load config files from kernel memory to user memory */
void load()
{
	char comm[max_char] = "";
	char *tdomf2, *tdomf_new, *res;

	/* string for command */
	strncpy(comm, tsave, max_char); strncat(comm, " d -", max_char);
	/* load domain config */
	tdomf = pipe_read(comm, max_file);

	/* string for command */
	strncpy(comm, tsave, max_char); strncat(comm, " e -", max_char);
	/* load exception config */
	texcf = pipe_read(comm, max_file);
	
	/* alloc memory for new policy */
	tdomf_new = memory_get(max_file);

	/* remove disabled mode entries so runtime will be faster */
	tdomf2 = tdomf;
	
	while(1){
/*		int flag_root_domain = 0;
		char *res2, *res_orig;*/
		
		/* get next domain */
		res = domain_get_next(&tdomf2);
		/* exit if reaching end */
		if (!res) break;
		
		/* root domain <kernel> with profile 0 should stay */
/*		res_orig = res;
		res2 = string_get_next_line(&res_orig);
		if (res2){
			flag_root_domain = strcmp(res2, "<kernel>");
		}
		if (domain_get_profile(res) || !flag_root_domain){*/


		/* check domain profile and add only profile with non-zero */
		if (domain_get_profile(res)){
			/* if it's not null, then add it to the new policy */
			strncat(tdomf_new, res, max_file);
			strncat(tdomf_new, "\n", 1);
		}
		free(res);
		
	}

	/* remove deleted entries */

	/* remove quota_exceeded entries too (replace text with spaces) */

	free(tdomf);
	tdomf = tdomf_new;
}


/* check if only one instance of the program is running */
int check_instance(){
	char mypid[max_num] = "";
	/* get my pid number and convert it */
	itoa(mypid, getpid());
	/* check if my pid file exists */
	if (file_exist(pidf)){
		char *pid2;
		long len = 0;
		/* read pid number from pid file */
		pid2 = file_read(pidf, &len);
		/* is it me? */
		if (strncmp(mypid, pid2, max_char) == 0) return 0;
		else{
			/* is the process with the foreign pid still running? */
			char path[max_char] = "/proc/";
			strncat(path, pid2, max_char);
			/* if running, then return false */
			if (dir_exist(path)) return 1;
			/* if not running, then overwrite pid number in pid file */
			else{
				file_write(pidf, mypid);
			}
		}
		free(pid2);
	}
	/* create pid file if it doesn't exist */
	else{
		file_write(pidf, mypid);
	}

	return 1;
}


/* get absolute pathname of a file from a name */
/* first the current directory is checked, then the path list on the path env variable */
/* return true if success and write back new path to old name */
int which(char *name){
	char *res;
	char *path;
	char full[max_char] = "";

	/* does the name contain any "/" char? */
	/* if so, then it's whether a relative or a full path, so i don't check it any further */
	if (strpbrk(name, "/")){
		/* is the file exist? if not, then fail */
		if (file_exist(name)) return 1;
		return 0;
	}

	/* does the file exist in the current dir? */
	strncpy(full, "./", max_char);
	strncat(full, name, max_char);
	if (file_exist(full)){ strncpy(name, full, max_char); return 1; }

	/* does the file exist in the paths from path env? */
	path = getenv("PATH");
	while((res = strtok(path, ":"))){
		strncpy(full, res, max_char);
		strncat(full, "/", max_char);
		strncat(full, name, max_char);
		if (file_exist(full)){ strncpy(name, full, max_char); return 1; }
		path = 0;
	}

	return 0;
}


/* create profile.conf and manager.conf files */
void create_prof()
{
	char comm[max_char] = "";
	/* check kernel version, if it's under 2.6.33, then use profile for tomoyo version 2.2.x */
	if (flag_kernel_version < 263300){
		/* write profiles to disk */
		file_write(tpro, tprof22);
		file_write(tman, tmanf22);
	}
	/* use profile for tomoyo version 2.3.x */
	else{
		/* write profiles to disk */
		file_write(tpro, tprof23);
		file_write(tman, tmanf23);
	}
	/* load profile.conf from disk to memory */
	strncpy(comm, tload, max_char); strncat(comm, " p", max_char);
	system(comm);
	/* load manager.conf from disk to memory */
	strncpy(comm, tload, max_char); strncat(comm, " m", max_char);
	system(comm);
}


/* -------------------------------------- */
/* ----- CHECK COMMAND LINE OPTIONS ----- */
/* -------------------------------------- */

void check_options(int argc, char **argv){
	/* more than 1 argument? */
	/* don't count the first argument that is the executable name itself */
	int argc2 = argc - 1;
	if (argc2 > 0) {
		int flag_type = 0;
		int flag_last = 0;
		int c = 1;
		char myarg[max_char] = "";
		/* cycle through the arguments */
		while (argc2--){
			int flag_ok = 0;
			/* clear argument buffer for security */
			int i = max_char;
			while(--i) myarg[i] = 0;
			strncpy(myarg, argv[c], max_char);
			
			if (!strncmp(myarg, "-v", max_char) || !strncmp(myarg, "--verson", max_char))	{ opt_version = 1;	flag_ok = 1; }
			if (!strncmp(myarg, "-h", max_char) || !strncmp(myarg, "--help"  , max_char))	{ opt_help    = 1;	flag_ok = 1; }
			
			if (!strncmp(myarg, "-1", max_char) || !strncmp(myarg, "--once" , max_char))	{ opt_once   = 1; flag_ok = 1; }
			if (!strncmp(myarg, "-c", max_char) || !strncmp(myarg, "--color", max_char))	{ opt_color  = 1; flag_ok = 1; }
			if (!strncmp(myarg, "-k", max_char) || !strncmp(myarg, "--keep" , max_char))	{ opt_keep   = 1; flag_ok = 1; }
			if (!strncmp(myarg, "-l", max_char) || !strncmp(myarg, "--learn", max_char))	{ opt_learn  = 1; flag_ok = 1; }

			if (!strncmp(myarg, "-i", max_char) || !strncmp(myarg, "--info"     , max_char))	{ opt_info       = 1; flag_ok = 2; }
			if (!strncmp(myarg, "-r", max_char) || !strncmp(myarg, "--remove"   , max_char))	{ opt_remove     = 1; flag_ok = 3; }
			if (!strncmp(myarg, "-R", max_char) || !strncmp(myarg, "--recursive", max_char))	{ opt_recursive  = 1; flag_ok = 4; }

			if (!strncmp(myarg, "--yes"  , max_char))	{ opt_yes   = 1; flag_ok = 1; }
			if (!strncmp(myarg, "--clear", max_char))	{ opt_clear = 1; flag_ok = 1; }
			if (!strncmp(myarg, "--reset", max_char))	{ opt_reset = 1; flag_reset = 1;  flag_ok = 1; }

			/* store option type if it was any of --info, --remove or --recursive */
			/* so if the next argument is not an option, then i know which former one it belongs to */
			if (flag_ok > 1) flag_type = flag_ok;
			/* if argument doesn't start with "-" char */
			if (myarg[0] != '-'){
				int flag_progs = 0;
				/* if last option arg was --info, then this belongs to it */
				if (flag_type == 2){
					/* if former arg was an option, then it belongs to it */
					if (flag_last){
						/* store as --info parameter */
						strncpy(opt_info2, myarg, max_char);
					}
					/* it belongs to the extra executables, so store it */
					else flag_progs = 1;
				}
				/* if last option arg was --remove, then this belongs to it */
				if (flag_type == 3){
					/* if former arg was an option, then it belongs to it */
					if (flag_last){
						/* store as --remove parameter */
						strncpy(opt_remove2, myarg, max_char);
					}
					/* it belongs to the extra executables, so store it */
					else flag_progs = 1;
				}
				/* if last option arg was --recursive, then this belongs to it */
				if (flag_type == 4){
					char path[max_char] = "";
					int l;
					flag_progs = 0;
					/* root "/" dir not allowed */
					if (strncmp(myarg, "/", max_char) == 0){ color_("error: root directory is not allowed", red); myexit(1); }
					/* check if dir name exist */
					if (!dir_exist(myarg)){
						color_("error: no such directory: ", red); color_(myarg, red); newl(); myexit(1); }
					/* expand recursive dir names with "/" char if missing */
					strncpy(path, myarg, max_char);
					l = strlen(myarg);
					if (path[l-1] != '/'){ path[l] = '/'; path[l+1] = 0; }
					/* if so, store it in recursive dir array */
					strncpy(opt_recursive2[opt_recursive2_counter++], path, max_char);
				}
				/* if argument doesn't belong to any option, then it goes to the extra executables */
				if (!flag_type || flag_progs){
					char path[max_char] = "";
					strncpy(path, myarg, max_char);
					/* search for name in path env and check if file exist */
					if(!which(path)){
						color_("error: no such file: ", red); color_(path, red); newl(); myexit(1); }
					/* if so, store it in extra executables */
					strncpy(progs[progs_counter++], myarg, max_char);
				}
			}

			/* store the last option type */
			flag_last = flag_ok;

			c++;
		}

		/* exit after --version or --help option */
		if (opt_version || opt_help){
			if (opt_version) version();
			if (opt_help)    help();
			myexit(0);
		}

		/* fail if no arguments for --remove option */
		if (opt_remove && opt_remove2[0] == 0){ color_("error: bad argument\n", red); myexit(1); }
		/* tail if no arguments for --recursive option */
		if (opt_recursive && !opt_recursive2_counter){ color_("error: bad argument\n", red); myexit(1); }

	}
}


/* is output colored? */
void check_options_colored(int argc, char **argv)
{
	int argc2 = argc - 1;
	int c = 1;
	if (argc2 > 0) {
		char *myarg;
		while (argc2--){
			myarg = argv[c];
			if (!strncmp(myarg, "-c", max_char) || !strncmp(myarg, "--color", max_char))	{ opt_color  = 1; }
		}
	}
}


/* check status of tomoyo (kernel mode and tools) */
void check_tomoyo()
{
	char *buff;
	char comm[max_char] = "";
	/* check tomoyo tool files */
	if (!file_exist(tload)){ color_("error: ", red); color_(tload, red); color_(" executable binary missing\n", red); myexit(1); }
	if (!file_exist(tsave)){ color_("error: ", red); color_(tsave, red); color_(" executable binary missing\n", red); myexit(1); }
	/* check status of tomoyo */
	strncpy(comm, tsave, max_char); strncat(comm, " u - 2>/dev/null", max_char);
	/* get the output of command "tomoyo-savepolicy u -" giving back memory stat */
	buff = pipe_read(comm, max_char);
	/* if no "total" part, then tomoyo is not activated */
	if (!strstr(buff, "Total:")){ color_("error: tomoyo kernel mode is not activated\n", red); myexit(1); }
	else{ color_("tomoyo kernel mode is active\n", clr); }
	/* create tomoyo dir if it doesn't exist yet */
	if (!dir_exist(tdir)){ mkdir(tdir, S_IRWXU); }
}


/* clear config files */
void clear()
{
	char comm[max_char] = "";
	/* write config files */
	file_write(texc, "");
	file_write(tdom, "<kernel>\nuse_profile 0\n\n");
	/* load config files from disk to memory */
	strncpy(comm, tload, max_char); strncat(comm, " fa", max_char);
	system(comm);
}


/* info about domains by a pattern */
void domain_info(char *pattern)
{
	/* load config files from kernel memory */
	load();

	/* is there any pattern? */
	if (pattern[0]){
		char *tdomf2, *res, *res2, *res_orig;
		int i;
		char counts[max_num] = "";
		int count = 0;
		int flag_first = 0;
		
		tdomf2 = tdomf;
		while(1){
			/* get next domain */
			res = domain_get_next(&tdomf2);
			/* exit on end */
			if (!res) break;

			res_orig = res;
			/* get first line */
			/* here res2 should be something, so i don't check data availability */
			res2 = string_get_next_line(&res);
			
			/* search for a keyword */
			i = string_search_keyword(res2, pattern);
			
			/* print domain if match */
			if (i > -1){
				/* increase counter for summary */
				count++;

				/* print new line only once for header */
				if (!flag_first){ newl_(); flag_first = 1; }
				/* print header part in blue */
				color(res2, blue); newl(); newl();
				free(res2);

				/* print the rest of the domain part */
				while(1){
					res2 = string_get_next_line(&res);
					/* exit if reaching end of domain block */
					if (!res2) break;
					/* print non empty lines */
					if (res2[0]){
						char h[max_char] = "";
						strncpy(h, home, strlen(home));
						strncat(h, "/", 1);
					
						/* print rules with reading libs in yellow */
						if ((string_search_keyword(res2, "allow_read /lib/") > -1) ||
						    (string_search_keyword(res2, "allow_read /usr/lib/") > -1)){
							color(res2, yellow); newl();
						}
						/* print rules with touching /home in light blue */
						else if (string_search_keyword(res2, h) > -1){
							color(res2, cyan); newl();
						}
						/* print the rest in red */
						else{
							color(res2, red); newl();
						}
					}
					free(res2);
				}
			}
			else free(res2);

			free(res_orig);
		}
		
		/* print summary */
		if (count){
			newl();
			itoa(counts, count);
			color_("(found ", clr); color_(counts, clr);
			if (count == 1) color_(" domain)\n", clr);
			else            color_(" domains)\n", clr);
		}
		else color_("error: no domains found\n", red);
	}

	/* list domain names only */	
	else{
		char *tdomf2, *res, *res2, *res_orig;
		char *texcf_new;
		long maxl;
		
		/* get policy size to use it for allocating memory as a maximum value */
		maxl = strlen(tdomf);
		texcf_new = memory_get(maxl);
		
		tdomf2 = tdomf;
		while(1){
			/* get next domain */
			res = domain_get_next(&tdomf2);
			/* exit on end */
			if (!res) break;

			res_orig = res;
			/* get first line */
			/* here res2 should be something, so i don't check data availability */

			res2 = string_get_next_wordn(&res, 1);
			if (res2){
				strncat(texcf_new, res2, strlen(res2));
				strncat(texcf_new, "\n", 1);
				free(res2);
			}
		}
		
		res = string_sort_uniq_lines(texcf_new);
		if (res){
			free(texcf_new);
			texcf_new = res;
		}
		
		newl_();
		color(texcf_new, blue); newl();
		free(texcf_new);
	}
}


/* remove domains by a pattern */
void domain_remove(char *text)
{
	debug(text);
}


/* turn back learning mode for all domains with profile 2-3 */
void domain_learn_all()
{
}


/* ----------------------------------- */
/* ------------ MAIN PART ------------ */
/* ----------------------------------- */

int main(int argc, char **argv){

	/* vars */
	char *user;

	/* is output colored? set only color option here before anything else */
	check_options_colored(argc, argv);

	/* check if i am root */
	user = getenv("USER");
	if (strncmp(user, "root", 5)) { color_("error: root privileges needed\n", red); myexit(1); }

	/* check already running instance of the program */
	if (!check_instance()) { color_("error: tomld is running already\n", red); myexit(1); }

	/* check command line options */
	check_options(argc, argv);


	/* ---------------- */
	/* ----- INIT ----- */
	/* ---------------- */

	/* print version info */
	color_("tomld (tomoyo learning daemon) ", clr); color_(ver, clr); newl_();
	
	/* store kernel version */
	flag_kernel_version = kernel_version();

	/* check tomoyo status */
	check_tomoyo();

	/* create profile.conf and manager.conf files */
	create_prof();

	/* on --reset option, create backup of config files with tomoyo-savepolicy */
	/* (tomoyo-savepolicy always creates backup files using timestamp) */
	if (opt_reset){
		color("* resetting domain configurations on demand\n", red);
		if (!choice("are you sure?")) myexit(0);
		system(tsave);
		color("policy file backups created\n", green);
	}

	/* create new empty policy files if missing or if --reset switch is on */
	if (!file_exist(tdom) || !file_exist(texc) || opt_reset) clear();

	/* on --clear, empty configuration files */
	if (!opt_reset && opt_clear){
		color("* clearing domain configurations on demand\n", red);
		if (!choice("are you sure?")) myexit(0);
		system(tsave);
		color("policy file backups created\n", green);
		clear();
		color("* configuration cleared\n", red);
		myexit(0);
	}

	/* on --info, print domain information */
	if (opt_info){
		domain_info(opt_info2);
		myexit(0);
	}

	/* on --remove, remove domain */
	if (opt_remove){
		domain_remove(opt_remove2);
		myexit(0);
	}

	/* on --learn, turn all domains into learning mode */
	if (opt_learn){
		color("* turning all domains into learning mode on demand\n", red);
		if (!choice("are you sure?")) myexit(0);
		system(tsave);
		color("policy file backups created\n", green);
		domain_learn_all();
		color("all domains turned back to learning mode\n", red);
		myexit(0);
	}


	return 0;
}

