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
25/04/2011 - tomld v0.31 - complete rewrite of tomld from python to c language
                         - drop platform check
                         - improve checking of tomoyo status and availability
                         - some memory leaks fixed
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

/* tab = 4 chars */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/mount.h>


#define max_char	4096
#define max_num		32
#define max_file	10*1024*1024


/* ------------------------------------------ */
/* ------------ GLOBAL VARIABLES ------------ */
/* ------------------------------------------ */

/* program version */
char *ver = "0.31";

/* home dir */
char *home = "/home";

/* interval of policy check in seconds */
int count = 10;

/* number of max entries in profile config */
#define max_mem "10000"

/* tomoyo kernel parameter */
char *tkern = "security=tomoyo";

/* tomoyo vars and files */
char *tdomf	= 0;
char *texcf = 0;

/* default profile.conf */
char *tprof22 = ""
	"0-COMMENT=-----Disabled Mode-----\n"
	"0-MAC_FOR_FILE=disabled\n"
	"0-MAX_ACCEPT_ENTRY="max_mem"\n"
	"0-TOMOYO_VERBOSE=disabled\n"
	"1-COMMENT=-----Learning Mode-----\n"
	"1-MAC_FOR_FILE=learning\n"
	"1-MAX_ACCEPT_ENTRY="max_mem"\n"
	"1-TOMOYO_VERBOSE=disabled\n"
	"2-COMMENT=-----Permissive Mode-----\n"
	"2-MAC_FOR_FILE=permissive\n"
	"2-MAX_ACCEPT_ENTRY="max_mem"\n"
	"2-TOMOYO_VERBOSE=enabled\n"
	"3-COMMENT=-----Enforcing Mode-----\n"
	"3-MAC_FOR_FILE=enforcing\n"
	"3-MAX_ACCEPT_ENTRY="max_mem"\n"
	"3-TOMOYO_VERBOSE=enabled\n";

char *tprof23 = ""
	"PROFILE_VERSION=20090903\n"
	"PREFERENCE::enforcing={ verbose=yes }\n"
	"PREFERENCE::learning={ verbose=no max_entry="max_mem" }\n"
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
char *tverk = "/sys/kernel/security/tomoyo/version";
char *tdom	= "/etc/tomoyo/domain_policy.conf";
char *texc	= "/etc/tomoyo/exception_policy.conf";
char *tdomk = "/sys/kernel/security/tomoyo/domain_policy";
char *texck = "/sys/kernel/security/tomoyo/exception_policy";
char *tpro	= "/etc/tomoyo/profile.conf";
char *tman	= "/etc/tomoyo/manager.conf";
char *tprok	= "/sys/kernel/security/tomoyo/profile";
char *tmank	= "/sys/kernel/security/tomoyo/manager";
char *tinit	= "/sbin/tomoyo-init";
char *tload	= "/usr/sbin/tomoyo-loadpolicy";
char *tsave	= "/usr/sbin/tomoyo-savepolicy";

/* system log */
char *tlog	= "/var/log/syslog";

/* backup conf for checking change of rules */
char *tdomf_bak = "";

/* this stores the kernel time of the last line of system log */
/* to identify the end of tomoyo logs and make sure not to read it twice */
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
char *dirs_recursive = 0;
long *dirs_recursive_depth = 0;

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
char *tprogs = 0;

char *tprogs_exc = 0;
char *tprogs_exc_manual[] = {"/usr/sbin/sshd", 0};

char *netf[] = {"/proc/net/tcp", "/proc/net/tcp6", "/proc/net/udp", "/proc/net/udp6", 0};

char tshell[] = "/etc/shells";
char *tshellf = 0;
char *tshellf2[] = {"/bin/bash", "/bin/csh", "/bin/dash", "/bin/ksh", "/bin/rbash", "/bin/sh",
"/bin/tcsh", "/usr/bin/es", "/usr/bin/esh", "/usr/bin/fish", "/usr/bin/ksh", "/usr/bin/rc",
"/usr/bin/screen", 0};

char *path_bin[] = {"/usr/local/sbin", "/usr/local/bin", "/usr/sbin", "/usr/bin", "/sbin", "/bin", 0};

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


/* free my pointers */
void myfree()
{
	if (tdomf)					free(tdomf);
	if (texcf)					free(texcf);
	if (tshellf)				free(tshellf);
	if (tprogs)					free(tprogs);
	if (tprogs_exc)				free(tprogs_exc);
	if (dirs_recursive)			free(dirs_recursive);
	if (dirs_recursive_depth)	free(dirs_recursive_depth);
}


/* my exit point */
void myexit(int num)
{
	/* free my pointers */
	myfree();
	exit(num);
}


/* count lines of string */
int string_count_lines(char *text)
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
	long c;
	long l = strlen(text);
	/* count lines of text */
	c = string_count_lines(text);
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


/* allocate memory for char and return pointer */
/* returned value must be freed by caller */
char *memory_get(long num)
{
	char* p = malloc((sizeof(char)) * (num + 1));
	if (!p){ color_("error: out of memory\n", red); myexit(1); }
	/* clear first byte */
	p[0] = 0;
	return p;
}


/* allocate memory for char pointer list and return pointer */
/* returned value must be freed by caller */
char **memory_get_ptr(long num)
{
	char** p = malloc((sizeof(char**)) * (num + 1));
	if (!p){ color_("error: out of memory\n", red); myexit(1); }
	/* clear first byte */
	p[0] = 0;
	return p;
}


/* allocate memory for long and return pointer */
/* returned value must be freed by caller */
long *memory_get_long(long num)
{
	long *p = malloc((sizeof(long)) * (num + 1));
	if (!p){ color_("error: out of memory\n", red); myexit(1); }
	/* clear first byte */
	p[0] = 0;
	return p;
}


/* my strncat */
char *strncat2(char *s1, char *s2)
{
/*	return (strncat(s1, s2, sizeof(s1) - strlen(s1) - 1));*/
	return (strncat(s1, s2, strlen(s2)+1));
}


/* my strncpy */
char *strncpy2(char *s1, char *s2)
{
/*	char *res = strncpy(s1, s2, sizeof(s1)/sizeof(s1[0]));
	s1[sizeof(s1)-1] = 0;
*/
	char *res = strncpy(s1, s2, strlen(s2)+1);
	return res;
}


/* convert char to integer */
int string_ctoi(char c)
{
	char s[2];
	s[0] = c; s[1] = 0;
	return (atoi(s));
}


/* convert integer to string */
/* returned value must be freed by caller */
char *string_itos(int num)
{
	char *res = memory_get(max_num);
	sprintf(res, "%d", num);
	return res;
}


/* convert long integer to string */
/* returned value must be freed by caller */
char *string_ltos(unsigned long num)
{
	char *res = memory_get(max_num);
	sprintf(res, "%ld", num);
	return res;
}


/* return the filename part of a path string */
/* returned value must be freed by caller */
char *string_get_filename(char *text)
{
	char *res;
	char c;
	int i, i2, l;
	
	/* alloc mem for result */
	res = memory_get(max_char);

	/* search for filename part */
	l = strlen(text);
	if (l > 0){
		i = l;
		/* search for "/" char backwords and stop at it */
		while (i--){
			c = text[i];
			if (c == '/') { i++; break; }
		}
		/* rightmost string is not null? */
		if (i < l){
			/* copy string after "/" char */
			i2 = 0;
			while(i <= l){
				res[i2++] = text[i++];
			}
			return res;
		}
	}
	
	return 0;
}


/* return the first occurence of string containing only numbers */
/* returned value must be freed by caller */
char *string_get_number(char *text)
{
	char *res;
	char c;
	int i = 0;
	int start = 0;
	int l;
	
	while(1){
		c = text[i];
		if (!c || c == '\n') return 0;
		if (c >= '0' && c <= '9') break;
		i++;
	}

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
	res = memory_get(l);
	/* copy word */
	i = l;
	while(i--){
		res[i] = text[start + i];
	}
	res[l] = 0;

	return res;
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


/* move pointer to the beginning of next line in a string skipping empty lines */
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
	if (l < 1) return 0;

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
	char *res, c;
	int i = 0;
	int start = 0;
	int l;

	/* count num pieces of words */
	if (num < 0) num = 0;
	num++;
	while (num--){

		/* skip white spaces or exit on end of line */
		while (1){
			c = (*text)[i];
			/* exit on end */
			if (!c || c == '\n') return 0;
			if (c != ' ') break;
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
			if (c == ' ') break;
			i++;
		}
	}
	
	l = i - start;
	/* fail on zero length result */
	if (l < 1) return 0;

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

/*	char *res;
	
	while(1){
		res = string_get_next_word(text);
		if (!res) return 0;
		if (num <= 0) return res;
		free(res);
		num--;
	}*/

}


/* search for a keyword in a string and return the position where it starts */
/* return -1 on fail */
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
	
	return -1;
}


/* search for a line in a string and return the position where it starts */
/* return -1 on fail */
int string_search_line(char *text, char *line)
{
	char c1, c2;
	int i, i2, start;

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


/* compare strings for qsort */ 
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
	char c, *res, *text_temp, **ptr;
	char *text_new, *text_final, *text_sort;
	char *res2 = "";
	int flag_first = 0;
	int flag_newl = 0;
	int flag_diff = 0;
	int i, i2, l1, l2, count;
	int maxl = strlen(text);
	
	/* create a copy of text for sorting */
	text_sort = memory_get(maxl);
	strncpy2(text_sort, text);

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
	ptr = memory_get_ptr(count);
	
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
		strncat2(text_new, s);
		strncat2(text_new, "\n");
	}
	free(text_sort);
	free(ptr);

	/* create another string holder */
	text_final = memory_get(maxl);

	/* make it uniq */
	text_temp = text_new;
	while(1){
		/* get next line */
		res = string_get_next_line(&text_temp);
		/* exit on end */
		if (!res){
			if (flag_first) free(res2);
			break;
		}

		/* first run */		
		if (!flag_first){
			l1 = strlen(res);
			strncat2(text_final, res);
			strncat2(text_final, "\n");
		}
		else{
			/* compare 2 lines */
			l1 = strlen(res);
			l2 = strlen(res2);
			flag_diff = 0;
			if (l1 != l2) flag_diff = 1;
			else if (strcmp(res, res2)) flag_diff = 1;
			/* store if different */
			if (flag_diff){
				strncat2(text_final, res);
				strncat2(text_final, "\n");
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


/* return a new string containing the name of the domain from the domain policy */
/* returned value must be freed by caller */
char *domain_get_name(char *text)
{
	char *res, *res2, *temp, *temp2;
	
	/* get first line containing the name of the domain */
	temp = text;
	res = string_get_next_line(&temp);
	if (!res) return 0;
	/* get second word for domain name */
	temp2 = res;
	res2 = string_get_next_wordn(&temp2, 1);
	free(res);
	if (!res2) return 0;
	return res2;
}


/* return a new string containing a list of all domain names */
/* returned value must be freed by caller */
char *domain_get_list()
{
	char *tdomf2, *res, *res2, *list, *list2;
	
	/* alloc mem for new list */
	list = memory_get(max_file);

	/* collect domain names */
	tdomf2 = tdomf;
	while(1){
		/* get next domain */
		res = domain_get_next(&tdomf2);
		/* exit on end */
		if (!res) break;
		/* get domain name */
		res2 = domain_get_name(res);
		if (res2){
			strncat2(list, res2);
			strncat2(list, "\n");
			free(res2);
		}
		free(res);
	}
	
	/* sort and uniq list because there many same entries because of subdomains */
	if (list[0]){
		list2 = string_sort_uniq_lines(list);
		free(list);
		list = list2;
	}

	return list;
}


/* return profile number of domain (0-3) */
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
			p = string_ctoi(orig[i]);
			return p;
		}
	}
	
	return 0;
}


/* ------------------------------------------------------------------------------------- */


/* print time elapsed since program start */
void mytime()
{
	static int flag_time = 0;
	static struct timeval start, end;
	static long seconds, useconds;
	static float t, t_old;

	/* first run? */
	if (!flag_time){
		gettimeofday(&start, 0);
		flag_time = 1;
		t_old = 0;
	}
	else{
		gettimeofday(&end, 0);
		seconds  = end.tv_sec  - start.tv_sec;
		useconds = end.tv_usec - start.tv_usec;

		t = (((seconds) * 1000 + useconds/1000.0) + 0.5) / 1000;

		printf("-- time %.3fs (+%.3f)\n", t, t - t_old);
		t_old = t;
	}
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
/* returned value must be freed by caller */
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


/* open file and read content with given length, or if length is null, then check length from file descriptor */
/* returned value must be freed by caller */
char *file_read(char *name, long length)
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
	if (!length){
		/* get file length from file descriptor */
		fseek(f, 0, SEEK_END);
		len = ftell(f) + 1;
		fseek(f, 0, SEEK_SET);
	}
	/* length is manually set */
	else{
		len = length;
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
	char *v = "/proc/sys/kernel/osrelease";
	char *buff;
	int c, c2;
	int ver = 0;
	char n;
	/* read kernel version from /proc in a format as 2.6.32-5-amd64 */
	buff = file_read(v, max_char);
	c = 0;
	/* create version numbers */
	c2 = 100000;
	while (c2){
		n = buff[c++];
		if (n != '.'){
			/* exit if no numbers or at end of string */
			if (!(n >= '0' && n <= '9') || !n){ free(buff); return ver; }
			/* convert string to integer and add it to result */
			ver += c2 * string_ctoi(n);
			c2 /= 10;
		}
	}
	free(buff);

	return ver;
}


/* get tomoyo version */
int tomoyo_version()
{
	char *buff;
	int c, c2;
	int ver = 0;
	char n;

	/* read version string */
	buff = file_read(tverk, max_char);

	c = 0;
	/* create version numbers */
	c2 = 1000;
	while (c2){
		n = buff[c++];
		if (n != '.'){
			/* exit if no numbers or at end of string */
			if (!(n >= '0' && n <= '9') || !n){ free(buff); return ver; }
			/* convert string to integer and add it to result */
			ver += c2 * string_ctoi(n);
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


/* check if process is running currently */
/* full path name required */
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
		strncpy2(mypid, mydir_entry->d_name);
		/* does it contain numbers only meaning they are pids? */
		if (strspn(mypid, "0123456789") == strlen(mypid)) {
			int res;
			char buff[max_char] = "";
			/* create dirname like /proc/pid/exe */
			strncpy2(mydir_name, "/proc/");
			strncat2(mydir_name, mypid);
			strncat2(mydir_name, "/exe");
			/* resolv the link pointing from the exe name */
			res = readlink(mydir_name, buff, max_char);
			if (res > 0){
				/* compare the link to the process name */
				if (strcmp(buff, name) == 0) {
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
	/* vars */
	char *tdomf2, *tdomf_new, *res, *res2, *temp;
	
	/* load domain config */
	tdomf = file_read(tdomk, max_file);

	/* load exception config */
	texcf = file_read(texck, max_file);
	
	/* alloc memory for new policy */
	tdomf_new = memory_get(max_file);

	/* remove disabled mode entries so runtime will be faster */
	tdomf2 = tdomf;
	
	while(1){
		int flag_deleted = 0;
		
		/* get next domain */
		res = domain_get_next(&tdomf2);
		/* exit on end */
		if (!res) break;
		temp = res;

		/* check if domain is marked as (deleted) */
		res2 = string_get_next_line(&temp);
		if (res2){
			/* don't add domain marked as (deleted) */
			if (string_search_keyword(res2, "(deleted)") > -1) flag_deleted = 1;
			free(res2);
		}
		
		/* root domain <kernel> with profile 0 should stay */
/*		int flag_root_domain = 0;
		char *res2, *temp;
		temp = res;
		res2 = string_get_next_line(&temp);
		if (res2){
			flag_root_domain = strcmp(res2, "<kernel>");
		}
		if (domain_get_profile(res) || !flag_root_domain){*/


		/* check domain profile and add only profile with non-zero but only if not marked as deleted */
		if (domain_get_profile(res) && !flag_deleted){
			/* if it's not null, then add it to the new policy */
			temp = res;
			while(1){
				/* read domain line by line */
				res2 = string_get_next_line(&temp);
				if (!res2) break;
				
				/* remove (deleted) and quota_exceeded entries */
				if((string_search_keyword(res2, "(deleted)") == -1) && (string_search_keyword(res2, "(deleted)") == -1)){
					/* add entry if ok */
					strncat2(tdomf_new, res2);
					strncat2(tdomf_new, "\n");
				}
				free(res2);
			}
		}
		free(res);
	}

	free(tdomf);
	tdomf = tdomf_new;
}


/* reload config files from variables to kernel memory */
void reload()
{
	file_write(tdom, tdomk);
	file_write(texc, texck);
}


/* save config files from variables to disk */
void save()
{
	/* save configs to disk */
	file_write(tdom, tdomf);
	file_write(texc, texcf);
}


/* create backup and save config files from variables to disk with new names */
void backup()
{
	char tdom2[max_char] = "";
	char texc2[max_char] = "";
	char *num;
	struct timeval t;

	/* get elapsed seconds since 1970 */
	gettimeofday(&t, 0);
	/* convert long integer to string */
	num = string_ltos(t.tv_sec);

	/* create new uniqe names to config files */
	strncpy2(tdom2, tdom);
	strncat2(tdom2, ".backup.");
	strncat2(tdom2, num);
	strncpy2(texc2, texc);
	strncat2(texc2, ".backup.");
	strncat2(texc2, num);
	free(num);

	/* save configs to backup files on disk */
	file_write(tdom2, tdomf);
	file_write(texc2, texcf);
}


/* check if only one instance of the program is running */
int check_instance(){
	char *mypid;
	/* get my pid number and convert it to string */
	mypid = string_itos(getpid());
	/* check if my pid file exists */
	if (file_exist(pidf)){
		char *pid2;
		/* read pid number from pid file */
		pid2 = file_read(pidf, 0);
		/* is it me? */
		if (strcmp(mypid, pid2) == 0){ free(mypid); free(pid2); return 0; }
		else{
			/* is the process with the foreign pid still running? */
			char path[max_char] = "/proc/";
			strncat2(path, pid2);
			/* if running, then return false */
			if (dir_exist(path)){ free(mypid); free(pid2); return 1; }
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

	free(mypid);

	return 1;
}


/* search file name in current dir first, then in bin locations and give back full path on success */
/* returned value must be freed by caller */
char *which(char *name){
	char *res;
	char full[max_char] = "";
	char buff[max_char] = "";
	int i;

	/* name starts with "/" char? */
	if (name[0] == '/'){
		/* file exists? if not, then fail */
		if (file_exist(full)){
			/* read link path */
			i = readlink(name, buff, max_char);
			if (i > -1){ strncpy2(full, buff); }
			res = memory_get(strlen(full));
			strncpy2(res, full);
			return res;
		}
		return 0;
	}

	/* fle exists in the current dir? */
	getcwd(full, max_char);
	strncat2(full, "/");
	strncat2(full, name);
	if (file_exist(full)){
		/* read link path */
		i = readlink(full, buff, max_char);
		if (i > -1){ strncpy2(full, buff); }
		res = memory_get(strlen(full));
		strncpy2(res, full);
		return res;
	}

	/* file exists in the search paths? */
	i = 0;
	while(1){
		res = path_bin[i++];
		if (!res) return 0;
		strncpy2(full, res);
		strncat2(full, "/");
		strncat2(full, name);
		if (file_exist(full)){
			/* read link path */
			i = readlink(full, buff, max_char);
			if (i > -1){ strncpy2(full, buff); }
			res = memory_get(strlen(full));
			strncpy2(res, full);
			return res;
		}
		full[0] = 0;
	}
}


/* create profile.conf and manager.conf files */
void create_prof()
{
	/* check tomoyo version */
	if (tomoyo_version() <= 2299){
		/* write profiles to disk */
		file_write(tpro, tprof22);
		file_write(tman, tmanf22);
		/* load profiles to kernel memory */
		file_write(tprok, tprof22);
		file_write(tmank, tmanf22);
	}
	/* use profile for tomoyo version 2.3.x */
	else if (tomoyo_version() >= 2300){
		/* write profiles to disk */
		file_write(tpro, tprof23);
		file_write(tman, tmanf23);
		/* load profiles to kernel memory */
		file_write(tprok, tprof23);
		file_write(tmank, tmanf23);
	}
	else{
		color_("error: tomoyo version is not compatible\n", red);
		myexit(1);
	}
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
			strncpy2(myarg, argv[c]);
			
			if (!strcmp(myarg, "-v") || !strcmp(myarg, "--verson"))	{ opt_version = 1;	flag_ok = 1; }
			if (!strcmp(myarg, "-h") || !strcmp(myarg, "--help"  ))	{ opt_help    = 1;	flag_ok = 1; }
			
			if (!strcmp(myarg, "-1") || !strcmp(myarg, "--once" ))	{ opt_once   = 1; flag_ok = 1; }
			if (!strcmp(myarg, "-c") || !strcmp(myarg, "--color"))	{ opt_color  = 1; flag_ok = 1; }
			if (!strcmp(myarg, "-k") || !strcmp(myarg, "--keep" ))	{ opt_keep   = 1; flag_ok = 1; }
			if (!strcmp(myarg, "-l") || !strcmp(myarg, "--learn"))	{ opt_learn  = 1; flag_ok = 1; }

			if (!strcmp(myarg, "-i") || !strcmp(myarg, "--info"     ))	{ opt_info       = 1; flag_ok = 2; }
			if (!strcmp(myarg, "-r") || !strcmp(myarg, "--remove"   ))	{ opt_remove     = 1; flag_ok = 3; }
			if (!strcmp(myarg, "-R") || !strcmp(myarg, "--recursive"))	{ opt_recursive  = 1; flag_ok = 4; }

			if (!strcmp(myarg, "--yes"  ))	{ opt_yes   = 1; flag_ok = 1; }
			if (!strcmp(myarg, "--clear"))	{ opt_clear = 1; flag_ok = 1; }
			if (!strcmp(myarg, "--reset"))	{ opt_reset = 1; flag_reset = 1;  flag_ok = 1; }

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
						strncpy2(opt_info2, myarg);
					}
					/* it belongs to the extra executables, so store it */
					else flag_progs = 1;
				}
				/* if last option arg was --remove, then this belongs to it */
				if (flag_type == 3){
					/* if former arg was an option, then it belongs to it */
					if (flag_last){
						/* store as --remove parameter */
						strncpy2(opt_remove2, myarg);
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
					if (strcmp(myarg, "/") == 0){ color_("error: root directory is not allowed", red); myexit(1); }
					/* check if dir name exist */
					if (!dir_exist(myarg)){
						color_("error: no such directory: ", red); color_(myarg, red); newl(); myexit(1); }
					/* expand recursive dir names with "/" char if missing */
					strncpy2(path, myarg);
					l = strlen(myarg);
					if (path[l-1] != '/'){ path[l] = '/'; path[l+1] = 0; }
					/* alloc mem for dirs_recursive */
					if (!dirs_recursive) dirs_recursive = memory_get(max_file);
					/* if so, store it in recursive dir array */
					strncpy2(dirs_recursive, path);
					strncpy2(dirs_recursive, "\n");
				}
				/* if argument doesn't belong to any option, then it goes to the extra executables */
				if (!flag_type || flag_progs){
					/* search for name in paths and check if file exists */
					char *res = which(myarg);
					if(!res){
						color_("error: no such file: ", red); color_(myarg, red); newl(); myexit(1); }
					/* alloc mem for tprogs */
					if (!tprogs) tprogs = memory_get(max_file);
					/* if so, store it as extra executables */
					strncat2(tprogs, res);
					strncat2(tprogs, "\n");
					free(res);
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
		/* fail if no arguments for --recursive option */
		if (opt_recursive && !dirs_recursive){ color_("error: bad argument\n", red); myexit(1); }

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
			if (!strcmp(myarg, "-c") || !strcmp(myarg, "--color"))	{ opt_color  = 1; }
		}
	}
}


/* check status of tomoyo (kernel mode and tools) */
void check_tomoyo()
{
	char *cmd;
	int tver;

	/* check kernel command line */
	cmd = file_read("/proc/cmdline", max_char);
	if (string_search_keyword(cmd, " security=tomoyo") == -1){
		free(cmd);
		color_("error: tomoyo kernel mode is not activated\n", red);
		myexit(1);
	}
	free(cmd);

	/* check mount state of securityfs */
	cmd = file_read("/proc/mounts", max_file);
	if (string_search_keyword(cmd, "none /sys/kernel/security securityfs") == -1){
		int flag_mount = 0;
		/* mount tomoyo securityfs if not mounted */
		/* shell command: mount -t securityfs none /sys/kernel/security */
		flag_mount = mount("none", "/sys/kernel/security", "securityfs", MS_NOATIME, 0);
		if (flag_mount == -1){
			free(cmd);
			color_("error: tomoyo securityfs cannot be mounted\n", red);
			myexit(1);
		}
	}
	free(cmd);

	color_("tomoyo kernel mode is active\n", clr);
	
	/* check tomoyo version */
	tver = tomoyo_version();
	if (tver < 2200 || tver > 2399){
		color_("error: tomoyo version is not compatible\n", red);
		myexit(1);
	}

	/* check tomoyo tool files */
	if (!file_exist(tinit)){ color_("error: ", red); color_(tinit, red); color_(" executable binary missing\n", red); myexit(1); }
/*	if (!file_exist(tload)){ color_("error: ", red); color_(tload, red); color_(" executable binary missing\n", red); myexit(1); }*/
/*	if (!file_exist(tsave)){ color_("error: ", red); color_(tsave, red); color_(" executable binary missing\n", red); myexit(1); }*/

	/* create tomoyo dir if it doesn't exist yet */
	if (!dir_exist(tdir)){ mkdir(tdir, S_IRWXU); }
}


/* clear config files */
void clear()
{
	/* create backup of fromber config files */
	backup();
	/* free up memory of configs */
	if (tdomf) free(tdomf);
	if (texcf) free(texcf);
	/* create new configs */
	tdomf = memory_get(max_file);
	texcf = memory_get(max_file);
	strncpy2(tdomf, "<kernel>\nuse_profile 0\n\n");
	texcf[0] = 0;
	/* write config files */
	file_write(texc, texcf);
	file_write(tdom, tdomf);
	/* load config files from disk to memory */
	reload();
}


/* info about domains by a pattern */
void domain_info(char *pattern)
{
	/* load config files from kernel memory */
	load();

	/* is there any pattern? */
	if (pattern[0]){
		char *tdomf2, *res, *res2, *res_temp;
		int i;
		int count = 0;
		int flag_first = 0;
		
		tdomf2 = tdomf;
		while(1){
			/* get next domain */
			res = domain_get_next(&tdomf2);
			/* exit on end */
			if (!res) break;
			res_temp = res;

			/* get first line */
			/* here res2 should be something, so i don't check data availability */
			res2 = string_get_next_line(&res_temp);
			
			/* search for a keyword */
			i = string_search_keyword(res2, pattern);
			
			/* print domain if match */
			if (i == -1) free(res2);
			else{
				char *text_new, *text_temp;
			
				/* increase counter for summary */
				count++;

				/* print new line to stderr only once for header */
				if (!flag_first){ newl_(); flag_first = 1; }
				else newl();
				/* print header part in blue */
				color(res2, blue); newl();
				free(res2);
				/* print use_profile here too */
				res2 = string_get_next_line(&res_temp);
				color(res2, green); newl();
				free(res2);
				
				/* sort the rest of the policy text */
				text_new = string_sort_uniq_lines(res_temp);
				text_temp = text_new;

				/* print the rest of the domain part */
				while(1){
					res2 = string_get_next_line(&text_temp);
					/* exit on end of domain block */
					if (!res2) break;
					/* print non empty lines */
					if (res2[0]){
						char h[max_char] = "";
						strncpy2(h, home);
						strncat2(h, "/");
					
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
				free(text_new);
			}
			free(res);
		}
		
		/* print summary */
		if (count){
			char *res = string_itos(count);
			newl();
			color_("(found ", clr); color_(res, clr);
			if (count == 1) color_(" domain)\n", clr);
			else            color_(" domains)\n", clr);
			free(res);
		}
		else color_("error: no domains found\n", red);
	}

	/* list domain names only */	
	else{
		char *tdomf2, *res, *res2, *res_temp;
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

			res_temp = res;
			/* get first line */
			/* here res2 should be something, so i don't check data availability */

			res2 = string_get_next_wordn(&res_temp, 1);
			if (res2){
				strncat2(texcf_new, res2);
				strncat2(texcf_new, "\n");
				free(res2);
			}
			free(res);
		}
		
		res = string_sort_uniq_lines(texcf_new);
		if (res){
			newl_(); color(res, blue); newl();
			free(res);
		}
		else{
			newl_(); color(texcf_new, blue); newl();
		}		
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


/* print programs already in domain but not in progs list */
void domain_print_list_not_progs()
{
	/* vars */
	char *res, *res2, *list, *list2, *list3, *temp;
	
	/* get list of all domain names */
	list = domain_get_list();
	
	/* alloc mem for new list */
	list2 = memory_get(max_file);

	/* remove entries from the list that are in tprogs too */
	temp = list;
	while(1){
		res = string_get_next_line(&temp);
		if (!res) break;
		/* does tprogs contain any of the domain names? */
		if (string_search_line(tprogs, res) == -1){
			strncat2(list2, res);
			strncat2(list2, "\n");
			/* add domain to tprogs if no in exceptions */
			if (string_search_line(tprogs_exc, res) == -1){
				strncat2(tprogs, res);
				strncat2(tprogs, "\n");
			}
		}
		free(res);
	}
	
	free(list);
	list = list2;
	
	/* is any element in the list? */
	if (list[0]){
		/* sort list */
		list2 = string_sort_uniq_lines(list);
		free(list);
		list = list2;
		
		color("* already existing main domains", green); newl();

		/* alloc mem for new sorted list with filenames only */
		list2 = memory_get(max_file);

		/* get list of filenames only */
		temp = list;
		while(1){
			res = string_get_next_line(&temp);
			if (!res) break;
			res2 = string_get_filename(res);
			if (res2){
				strncat2(list2, res2);
				strncat2(list2, "\n");
				free(res2);
			}
			free(res);
		}
		
		/* sort filename list */
		list3 = string_sort_uniq_lines(list2);
		free(list2);
		list2 = list3;
		
		/* print list */
		temp = list2;
		while(1){
			res = string_get_next_line(&temp);
			if (!res) break;
			color(res, blue);
			color(" ", blue);
			free(res);
		}
		newl();
		
		free(list2);
	}
	
	free(list);
}


/* check if domain exist and wich mode it's in, and print info about it */
void domain_print_mode()
{
	/*vars */
	char *prog, *temp;
	
	if (flag_firstrun) color("* checking policy and rules\n", yellow);
	
	/* cycle thorugh progs */
	temp = tprogs;
	while(1){
		char s[max_char] = "";
		
		prog = string_get_next_line(&temp);
		if(!prog) break;
		
		/* does the domain exist for the program? */
		strncpy2(s, "initialize_domain ");
		strncat2(s, prog);
		if (string_search_line(texcf, s) > -1){
			if (flag_firstrun){
				color(prog, blue);
				color(", domain exists", clr);
				flag_check3 = 1;
			}
		}
		/* if not, then add program entry to exception policy */
		else{
			color(prog, blue);
			color(", no domain, ", clr);
			flag_check3 = 1;
			/* create a domain for the program */
			color("create domain", green);
			/* if the program is running already, then restart is needed for the rules to take effect */
			if (running(prog)) color(" (restart needed)", red);
			strncat2(texcf, "initialize_domain ");
			strncat2(texcf, prog);
			strncat2(texcf, "\n");
		}
		
		/* does the rule exist for it? */
		
		
		free(prog);
	}
}


/* manage policy and rules */
void check()
{
	/* vars */
/*	char *res;*/
	
	/* load config files */
	load();
	
	mytime();
	
	/* print programs already in domain but not in progs list */
	domain_print_list_not_progs();
	
	/* check if domain exist and wich mode it's in */
	domain_print_mode();

}


/* get shell and program exceptions */
void check_exceptions()
{
	char *res, *temp;
	int i;

	/* load /etc/shells if exists */
	if (file_exist(tshell)){
		tshellf = file_read(tshell, 0);
	}
	/* if not, get shell names from defined list */
	else{
		int i = 0;
		tshellf = memory_get(max_file);
		while(1){
			char *res = tshellf2[i++];
			if (!res) break;
			strncpy2(tshellf, res);
			strncpy2(tshellf, "\n");
		}
	}
	
	/* alloc mem for program exceptions */
	if (!tprogs_exc) tprogs_exc = memory_get(max_file);
	
	/* add shells to exceptions */
	temp = tshellf;
	while(1){
		res = string_get_next_line(&temp);
		if (!res) break;
		/* if line doesn't start with "#" char meaning a remark */
		if (res[0] != '#'){
			/* add it only if it exists */
			if (file_exist(res)){
				strncat2(tprogs_exc, res);
				strncat2(tprogs_exc, "\n");
			}
		}
		free(res);
	}

	/* add manual programs to exceptions */
	i = 0;
	while(1){
		res = tprogs_exc_manual[i++];
		if (!res) break;
		strncat2(tprogs_exc, res);
		strncat2(tprogs_exc, "\n");
	}
	
	/* sort exception list */
	if (tprogs_exc){
		res = string_sort_uniq_lines(tprogs_exc);
		if (res){
			free(tprogs_exc);
			tprogs_exc = res;
		}
	}

	/* sort program list */
	if (tprogs){
		res = string_sort_uniq_lines(tprogs);
		if (res){
			free(tprogs);
			/* realloc more mem above the sorted list to expand it later */
			tprogs = memory_get(max_file);
			strncpy2(tprogs, res);
			free(res);
		}
	}

	/* initialize recursive dirs' depth values */
	if (dirs_recursive){
		i = string_count_lines(dirs_recursive);
		if (i > 0){
			if (!dirs_recursive_depth) dirs_recursive_depth = memory_get_long(i+1);
			while(i--){
				dirs_recursive_depth[i] = -1;
			}
		}
	}
}


/* ------------------------------------ */
/* ------- PROCESS SEARCH LOOP -------- */
/* ------------------------------------ */
/* print process names using tcp or udp packets */
void check_processes()
{
	/* vars */
	int mypid;
	int count2;

	/* get my pid */
	mypid = getpid();

	/* print exceptions */
	if (tprogs_exc){
		char *temp = tprogs_exc;
		color("* exception domains\n", green);
		while(1){
			char *res = string_get_next_line(&temp);
			if (!res) break;
			color(res, purple);
			color(" ", purple);
			free(res);
		}
		newl();
	}

	/* print on demand programs */
	if (tprogs){
		color("* additional programs on demand\n", green);
		color(tprogs, blue);
	}
	
	/* store start of my process time for later to check speed */
	mytime();

	/* check processes using network */
	color("* processes using network", green); newl();	

	count2 = 0;
	while(1){
		char *netf2, *netf3;
		int i;

		DIR *mydir;
		struct dirent *mydir_entry;
		
		/* read up all net stat files and create a list of inode numbers (column 10)
		   of all processes using network */
		netf2 = memory_get(max_file);
		i = 0;
		while (1){
			char *res, *res2, *res3, *temp, *temp2;
			if (!netf[i]) break;
			/* read file */
			res = file_read(netf[i++], max_char);
			temp = res;
			/* skip one line */
			res2 = string_get_next_line(&temp);
			free(res2);
			/* get all column 10 (uid) */
			while(1){
				/* get next line */
				res2 = string_get_next_line(&temp);
				if (!res2) break;
				temp2 = res2;
				res3 = string_get_next_wordn(&temp2, 9);
				if(res3){
					strncat2(netf2, "socket:[");
					strncat2(netf2, res3);
					strncat2(netf2, "]\n");
					free(res3);
				}
				free(res2);
			}
			free(res);
		}

		/* sort pid list */
		netf3 = string_sort_uniq_lines(netf2);
		free(netf2);
		

		/* find all processes with the matching inode numbers in netf3's list */	
		/* open /proc dir */
		mydir = opendir("/proc/");
		if (!mydir){
			color_("error: cannot open /proc/ directory\n", red);
			free(netf3);
			myexit(1);
		}
		/* cycle through dirs in /proc */
		while((mydir_entry = readdir(mydir))) {
			char mypid[max_num] = "";
			/* get my pid number from dir name in /proc */
			strncpy2(mypid, mydir_entry->d_name);
			/* does it contain numbers only meaning they are pids? */
			if (strspn(mypid, "0123456789") == strlen(mypid)) {
				int res;
				char mydir_name[max_char] = "";
				char myprog[max_char] = "";
				/* create dirname like /proc/pid/exe */
				strncpy2(mydir_name, "/proc/");
				strncat2(mydir_name, mypid);
				strncat2(mydir_name, "/exe");
				/* resolv the link pointing from the exe name */
				res = readlink(mydir_name, myprog, max_char);
				if (res > 0){
					DIR *mydir2;
					struct dirent *mydir_entry2;
					char myfd[max_char] = "";
					/* create dir name like /proc/pid/fd/ */
					strncpy2(myfd, "/proc/");
					strncat2(myfd, mypid);
					strncat2(myfd, "/fd/");
					mydir2 = opendir(myfd);
					
					if (mydir2){
						/* cycle through files in /proc/pid/fd/ */
						while((mydir_entry2 = readdir(mydir2))) {
							char mydir_name2[max_char] = "";
							char mysock[max_char] = "";
							int resfd;
							/* create dirname like /proc/pid/fd/4 */
							strncpy2(mydir_name2, myfd);
							strncat2(mydir_name2, mydir_entry2->d_name);
							/* resolv the links from the /proc/pid/fd/ dir */
							resfd = readlink(mydir_name2, mysock, max_char);
							if (resfd > 0){
								/* does it contain a name like "socket:"? */
								if (string_search_keyword(mysock, "socket:") > -1){
									/* is the inode number in netf3's list? */
									if (string_search_line(netf3, mysock) > -1){
										/* alloc mem for list if not created yet */
										if (!tprogs) tprogs = memory_get(max_file);
										/* is it in my progs list yet? */
										if (string_search_line(tprogs, myprog) == -1){
											strncat2(tprogs, myprog);
											strncat2(tprogs, "\n");
											color(myprog, blue); newl();
										}
									}
								}
							}
						}
						closedir(mydir2);
					}
				}
			}
		}
		closedir(mydir);



		free(netf3);
		break;
	}

	/* print running time */
	mytime();
	
	/* manage policy and rules */
	check();

	/* print running time */
	mytime();
	
}


/* ----------------------------------- */
/* ------------ MAIN PART ------------ */
/* ----------------------------------- */

int main(int argc, char **argv){

	/* store start time */
	mytime();

	/* is output colored? set only color option here before anything else */
	check_options_colored(argc, argv);

	/* check if i am root */
	if (getuid()) { color_("error: root privileges needed\n", red); myexit(1); }

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
		backup();
		color("policy file backups created\n", green);
	}

	/* create new empty policy files if missing or if --reset switch is on */
	if (!file_exist(tdom) || !file_exist(texc) || opt_reset) clear();

	/* on --clear, empty configuration files */
	if (!opt_reset && opt_clear){
		color("* clearing domain configurations on demand\n", red);
		if (!choice("are you sure?")) myexit(0);
		backup();
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
		backup();
		color("policy file backups created\n", green);
		domain_learn_all();
		color("all domains turned back to learning mode\n", red);
		myexit(0);
	}

	/* create excaption list from program names and available shells */
	check_exceptions();

	/* check running processes */
	check_processes();


	/* free all my pointers */
	myfree();

	return 0;
}

