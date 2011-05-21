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


/* ------------------------------------------ */
/* ------------ GLOBAL VARIABLES ------------ */
/* ------------------------------------------ */

/* program version */
char *ver = "0.31";

/* home dir */
char *home = "/home";

/* interval of policy check to run in seconds */
float time_check = 10;

/* path to my executable */
char *my_exe_path = 0;

/* number of max entries in profile config */
#define max_mem "10000"

/* tomoyo kernel parameter */
char *tkern = "security=tomoyo";

/* pattern string for the recursive wildcard "\{\*\}" for compares */
char *wildcard_recursive_dir = "\\{\\*\\}";

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

/* backup conf for checking change of rules */
char *tdomf_bak = 0;

/* system log */
char *tlog	= "/var/log/syslog";
char *tlogf = 0;

/* this stores the kernel time of the last line of system log */
/* to identify the end of tomoyo logs and make sure not to read it twice */
char *tmark	= "/var/local/tomld.logmark";
char *tmarkf = 0;
/* tomld pid file */
char *tpid	= "/var/run/tomld.pid";

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

char *opt_info2		= 0;
char *opt_remove2	= 0;
char *dirs_recursive = 0;
long *dirs_recursive_depth = 0;
long *dirs_recursive_sub = 0;

int flag_reset		= 0;
int flag_check		= 0;
int flag_check2		= 0;
int flag_check3		= 0;
int flag_firstrun	= 1;
int flag_clock		= 0;
int flag_safe		= 0;
int flag_new_proc	= 0;
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
char *tprogs_learn = 0;

char *netf[] = {"/proc/net/tcp", "/proc/net/tcp6", "/proc/net/udp", "/proc/net/udp6", 0};

char *tshell = "/etc/shells";
char *tshellf = 0;
char *tshellf2[] = {"/bin/bash", "/bin/csh", "/bin/dash", "/bin/ksh", "/bin/rbash", "/bin/sh",
"/bin/tcsh", "/usr/bin/es", "/usr/bin/esh", "/usr/bin/fish", "/usr/bin/ksh", "/usr/bin/rc",
"/usr/bin/screen", 0};

char *path_bin[] = {"/usr/local/sbin", "/usr/local/bin", "/usr/sbin", "/usr/bin", "/sbin", "/bin", 0};

/* other vars */
struct termio terminal_backup;

int debug_var = 0;


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
	printf (tpid); printf(" (pid file to avoid multpiple instances of the program to be running at the same time)\n");
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


/* free my allocated dynamic string space */
void free2(char *ptr)
{
	if (ptr) free((unsigned long*)(ptr) - 2);
}


/* free my pointers */
void myfree()
{
	free2(my_exe_path);
	free2(tdomf);
	free2(texcf);
	free2(tshellf);
	free2(tprogs);
	free2(tprogs_exc);
	free2(tprogs_learn);
	free2(dirs_recursive);
	if (dirs_recursive_depth)	free(dirs_recursive_depth);
	if (dirs_recursive_sub)		free(dirs_recursive_sub);
	free2(opt_info2);
	free2(opt_remove2);
	free2(tmarkf);
	free2(tlogf);
	free2(tdomf_bak);
}


/* my exit point */
void myexit(int num)
{
	/* free my pointers */
	myfree();
	exit(num);
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
		printf(text);
		/* print newline if missing from the end of string */
		if (text[l-1] != '\n') newl();
		printf("-- debug bytes %ld and ", strlen(text));
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


/* print colored output to stdout */
void color(const char *text, const char *col)
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
void color_(const char *text, const char *col)
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
	if (!p){ color_("error: out of memory\n", red); myexit(1); }
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
	if (!p){ color_("error: out of memory\n", red); myexit(1); }
	/* clear first byte */
	p[0] = 0;
	return p;
}


/* allocate memory for char pointer list and return pointer */
/* returned value must be freed by caller */
char **memget_ptr(unsigned long num)
{
	char** p = malloc((sizeof(char**)) * (num + 1));
	if (!p){ color_("error: out of memory\n", red); myexit(1); }
	/* clear first byte */
	p[0] = 0;
	return p;
}


/* allocate memory for long and return pointer */
/* returned value must be freed by caller */
long *memget_long(unsigned long num)
{
	long *p = malloc((sizeof(long)) * (num + 1));
	if (!p){ color_("error: out of memory\n", red); myexit(1); }
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
		free2((*s1));
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
		free2((*s1));
		*s1 = s3;
		p2 = (unsigned long*)(*s1);
	}
	/* store length of new result string */
	*(p2 - 1) = l3 - 1;
	/* copy source behind dest string */
	while(l2--) (*s1)[l1 + l2] = s2[l2];
}


/* my strncat */
/*char *strncat2(char *s1, const char *s2, unsigned long size)
{
	return (strncat(s1, s2, size - strlen(s1) - 1));
*/
/*	char *res = s1;
	if (size > 0) {
		while (*res++);
		res--;
		while ((*res++ = *s2++) && (--size));
		*res = 0;
		return s1;
	}
	else return res;*/
/*}*/


/* my strncpy */
/*
char *strncpy2(char *s1, const char *s2, unsigned long size)
{*/
	/* this version of strncpy takes ages to finish running */
	/* because it fills all the the rest of the bytes too */
/*	char *res = strncpy(s1, s2, size);
	s1[size - 1] = 0;
	return res;*/

	/* this version of strncpy is fast */
/*	char *res = s1;
	while((*s1++ = *s2++) && (size--));
	*(--s1) = 0;
	return res;
}*/


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


/* convert long integer to string */
/* returned value must be freed by caller */
char *string_ltos(unsigned long num)
{
	char *res = memget2(max_num);
	sprintf(res, "%ld", num);
	strlenset2(&res);
	return res;
}


/* check if link exists and return string with resolved link path */
/* return null on fail */
/* returned value must be freed by caller */
char *link_read(const char *name)
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
char *string_get_filename(const char *text)
{
	char *res;
	char c;
	int i, i2, l;
	
	/* alloc mem for result */
	res = memget2(max_char);

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
			strlenset2(&res);
			return res;
		}
	}
	
	return 0;
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
	res[i] = 0;
	/* move pointer to the next line, or leave it on null end */
	if (c) i++;
	(*text) += i;
	strlenset3(&res, i);

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
	res[l] = 0;
	strlenset3(&res, l);

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


/* move pointer to the beginning of next line in a string skipping empty lines */
/* return null on fail */
int string_jump_next_line(char **text)
{
	char c;
	int i = 0;

	if (!(*text)) return 0;

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

	if (!(*text)) return 0;

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
		if (c != ' ') break;
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
		if (c != ' ') break;
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
		if (c == ' ' || i < start) break;
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


/* search for a keyword from the beginning of a string */
/* returns true if success */
int string_search_keyword_first(const char *text, const char *key)
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


/* compare strings for qsort */ 
int string_cmp(const void *a, const void *b) 
{ 
	const char **ia = (const char **)a;
	const char **ib = (const char **)b;
	return strcmp(*ia, *ib);
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


/* return the parent dir if path is a file */
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
	
	/* if path is a dir, then return it */
	if (path_is_dir(path)) return path_new;
	
	/* if it's a file, then get dir part */
	l = strlen(path) - 1;
	i = 0;
	while(1){
		if (l < 0) break;
		c = path[l];
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
	res = path_get_parent_dir(path);

	/* get dir depth */	
	c = path_count_dir_depth_r(res);
	
	free2(res);
	
	return c;
}


/* return a new string containing the next domain entry and move the pointer to the beginning of the next domain */
/* returned value must be freed by caller */
char *domain_get_next(char **text)
{
	char *key = "<kernel>";
	long keyl = 8;
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
	res = memget2(l);
	/* copy mem block */
	i = l;
	while(i--){
		res[i] = (*text)[start + i];
	}
	res[l] = 0;
	strlenset3(&res, l);
	
	/* move pointer to the next line */
	(*text) += start + l;

	return res;
}


/* return a new string containing the name of the main domain from the domain policy */
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
	free2(res);
	return res2;
}


/* return a new string containing the name of the subdomain from the domain policy */
/* returned value must be freed by caller */
char *domain_get_name_sub(char *text)
{
	char *res, *res2, *temp, *temp2;
	
	/* get first line containing the name of the domain */
	temp = text;
	res = string_get_next_line(&temp);
	if (!res) return 0;
	/* get last word for main or subdomain name */
	temp2 = res;
	res2 = string_get_last_word(&temp2);
	free2(res);
	return res2;
}


/* return true if prog exists as a main domain */
int domain_main_exist(char *prog)
{
	char *res, *res2, *temp;
	
	temp = tdomf;
	while(1){
		/* get first line containing the name of the domain */
		res = domain_get_next(&temp);
		if (!res) break;
		/* get main domain name */
		res2 = domain_get_name(res);
		if (res2){
			/* if prog names matches main domain name, then prog exists as a main domain and success */
			if (!strcmp(prog, res2)){
				free2(res2);
				free2(res);
				return 1;
			}
			free2(res2);
		}
		free2(res);
	}
	return 0;
}


/* return a new string containing a list of all domain names */
/* returned value must be freed by caller */
char *domain_get_list()
{
	char *tdomf2, *res, *res2, *list, *list2;
	
	/* alloc mem for new list */
	list = memget2(max_char);

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
			strcat2(&list, res2);
			strcat2(&list, "\n");
			free2(res2);
		}
		free2(res);
	}
	
	/* sort and uniq list because there many same entries because of subdomains */
	if (strlen2(&list)){
		list2 = string_sort_uniq_lines(list);
		free2(list);
		list = list2;
	}

	return list;
}


/* return the index position in domain policy where domain starts */
/* return -1 if domain does not exist */
int domain_exist(const char *text)
{
	int i;
	char *temp;
	
	temp = memget2(max_char);
	strcpy2(&temp, "<kernel> ");
	strcat2(&temp, text);
	i = string_search_line(tdomf, temp);
	free2(temp);

	return i;
}


/* return profile number of domain (0-3) from text position */
/* return -1 on fail */
int domain_get_profile(char *text)
{
	int i, p;
	char *key = "use_profile ";
	int keyl = 12;
	char *res, *orig;
	
	while(1){
		orig = text;

		/* get next line */
		res = string_get_next_line(&text);
		if (!res) break;

		/* search for the keyword */
		i = string_search_keyword(res, key);
		free2(res);

		/* if match */
		if (i > -1){
			i += keyl;
			p = string_ctoi(orig[i]);
			return p;
		}
	}
	
	return -1;
}


/* set profile number (0-3) of a domain at text position from the beginning of domain policy */
void domain_set_profile(char *text, int profile)
{
	int i, p;
	char *key = "use_profile ";
	int keyl = 12;
	char *res, *orig;
	
	while(1){
		orig = text;

		/* get next line */
		res = string_get_next_line(&text);
		if (!res) break;

		/* search for the keyword */
		i = string_search_keyword(res, key);
		free2(res);

		/* if match */
		if (i > -1){
			i += keyl;
			p = string_itoc(profile);
			orig[i] = p;
			break;
		}
	}
}


/* set profile number (0-3) of domain and all its subdomains in domain policy */
void domain_set_profile_all(const char *prog, int profile)
{
	char *res, *res2, *orig, *temp;
	
	temp = tdomf;
	while(1){
		orig = temp;

		/* get next domain policy */
		res = domain_get_next(&temp);
		if (!res) break;
		/* check if domain is a main or subdomain of what i'm looking for */
		res2 = domain_get_name(res);

		/* if so, then set its profile mode */
		if (!strcmp(prog, res2)) domain_set_profile(orig, profile);

		free2(res2);
		free2(res);
	}
}


/* add rule to prog domain and subdomain */
void domain_add_rule(char *prog, char *rule)
{
	char *res, *res2, *temp, *temp2, *tdomf_new;

	/* alloc mem for new policy */	
	tdomf_new = memget2(max_char);
	
	/* cycle through the domains */
	temp = tdomf;
	while(1){
		/* get next domain policy */
		res = domain_get_next(&temp);
		if (!res) break;
		/* add domain policy to new policy */
		strcat2(&tdomf_new, res);
		strcat2(&tdomf_new, "\n");
		/* check if this is what i'm looking for */
		temp2 = res;
		res2 = domain_get_name_sub(temp2);
		if (res2){
			/* prog name matches subdomain name? */
			if (!strcmp(prog, res2)){
				/* if so, then add rule to new policy */
				strcat2(&tdomf_new, rule);
				strcat2(&tdomf_new, "\n");
			}
			free2(res2);
		}
		free2(res);
		/* add new line to new policy */
		strcat2(&tdomf_new, "\n");
	}
	
	/* replace old policy with new one */
	free2(tdomf);
	tdomf = tdomf_new;
}


/* ------------------------------------------------------------------------------------- */


/* print time elapsed since program start */
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


/* give a choice */
int choice(const char *text)
{
	char c = 0, c2;
	if (opt_yes){
		printf(text);
		printf(" (y)\n");
	}
	else{
		printf(text);
		printf(" [y/N]");
		c = getchar();
		if (c == '\n') return 0;
		while((c2 = getchar()) != '\n');
		if (c == 'y' || c == 'Y') return 1;
		return 0;
	}
	return 1;
}


/* open pipe and read content with given length */
/* returned value must be freed by caller */
char *pipe_read(const char *comm, long length)
{
	char *buff;
	
	/* open pipe for reading */
	FILE *p = popen(comm, "r");
	if (!p){
		color_("error: cannot open pipe for command: ", red);
		color_(comm, red); newl_();
		myexit(1);
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
		color_("error: cannot open pipe for command: ", red);
		color_(comm, red); newl_();
		myexit(1);
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
		color_("error: cannot read file ", red);
		color_(name, red); newl_();
		myexit(1);
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
	
	if (len < 1){
		color_("error: cannot read file ", red);
		color_(name, red); newl_();
		myexit(1);
	}

	/* alloc mem */
	buff = memget2(len);
	/* read file */
	fread(buff, len, 1, f);
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
		color_("error: cannot write file ", red);
		color_(name, red); newl_();
		myexit(1);
	}
	/* write contents if not null */
	if (buff) fprintf(f, buff);
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
	buff = file_read(v, 1);
	c = 0;
	/* create version numbers */
	c2 = 100000;
	while (c2){
		n = buff[c++];
		if (n != '.'){
			/* exit if no numbers or at end of string */
			if (!(n >= '0' && n <= '9') || !n){ free2(buff); return ver; }
			/* convert string to integer and add it to result */
			ver += c2 * string_ctoi(n);
			c2 /= 10;
		}
	}
	free2(buff);

	return ver;
}


/* get tomoyo version */
int tomoyo_version()
{
	char *buff;
	char n;
	int c, c2;
	static int ver = 0;
	
	if (ver) return ver;

	/* read version string */
	buff = file_read(tverk, 1);

	c = 0;
	/* create version numbers */
	c2 = 1000;
	while (c2){
		n = buff[c++];
		if (n != '.'){
			/* exit if no numbers or at end of string */
			if (!(n >= '0' && n <= '9') || !n){ free2(buff); return ver; }
			/* convert string to integer and add it to result */
			ver += c2 * string_ctoi(n);
			c2 /= 10;
		}
	}
	free2(buff);
	
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
int process_running(const char *name){
	DIR *mydir;
	struct dirent *mydir_entry;
	char *mydir_name, *mypid;
	
	/* open /proc dir */
	mydir = opendir("/proc/");
	if (!mydir){ color_("error: cannot open /proc/ directory\n", red); return 0; }

	mydir_name = memget2(max_char);
	mypid = memget2(max_num);

	/* cycle through dirs in /proc */
	while((mydir_entry = readdir(mydir))) {
		/* get the dir names inside /proc dir */
		strcpy2(&mypid, mydir_entry->d_name);

		/* does it contain numbers only meaning they are pids? */
		if (strspn(mypid, "0123456789") == strlen2(&mypid)) {
			char *res;
			/* create dirname like /proc/pid/exe */
			strcpy2(&mydir_name, "/proc/");
			strcat2(&mydir_name, mypid);
			strcat2(&mydir_name, "/exe");

			/* resolv the link pointing from the exe name */
			res = link_read(mydir_name);
			if (res){
				/* compare the link to the process name */
				if (!strcmp(res, name)) {
					closedir(mydir);
					free2(res);
					free2(mydir_name);
					free2(mypid);
					return 1;
				}
				free2(res);
			}
		}
	}
	closedir(mydir);

	free2(mydir_name);
	free2(mypid);

	return 0;
	
}


/* return a string containing the full path of the executable that belongs to the pid */
/* returned value must be freed by caller */
char *process_get_path(int pid)
{
	char *path, *str, *res;

	path = memget2(max_char);
	strcpy2(&path, "/proc/");

	/* create path */
	str = string_itos(pid);
	strcat2(&path, str);
	strcat2(&path, "/exe");
	free2(str);

	/* resolv the link pointing from the exe name */
	res = link_read(path);
	free2(path);
	return res;
}


/* check if dir exists */
int dir_exist(const char *name){
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


/* load config files from kernel memory to user memory */
void load()
{
	/* vars */
	char *tdomf_new, *res, *res2, *temp, *temp2;
	
	/* load domain config */
	free2(tdomf);
	tdomf = file_read(tdomk, 1);

	/* load exception config */
	free2(texcf);
	texcf = file_read(texck, 1);
	
	/* alloc memory for new policy */
	tdomf_new = memget2(max_char);

	/* remove disabled mode entries so runtime will be faster */
	temp2 = tdomf;
	
	while(1){
		int flag_deleted = 0;
		
		/* get next domain */
		res = domain_get_next(&temp2);
		/* exit on end */
		if (!res) break;
		temp = res;

		/* check if domain is marked as (deleted) */
		res2 = string_get_next_line(&temp);
		if (res2){
			/* don't add domain marked as (deleted) */
			if (string_search_keyword(res2, "(deleted)") > -1) flag_deleted = 1;
			free2(res2);
		}
		
		/* root domain <kernel> with profile 0 should stay */
/*		int flag_root_domain = 0;
		char *res2, *temp;
		temp = res;
		res2 = string_get_next_line(&temp);
		if (res2){
			flag_root_domain = strcmp(res2, "<kernel>");
		}
		if (domain_get_profile(res) != -1 || !flag_root_domain){*/


		/* check domain profile and add only profile with non-zero but only if not marked as deleted */
		if (domain_get_profile(res) && !flag_deleted){
			/* if it's not null, then add it to the new policy */
			temp = res;
			while(1){
				/* read domain line by line */
				res2 = string_get_next_line(&temp);
				if (!res2) break;

				/* don't add empty lines */
				if (strlen2(&res2)){
					/* remove (deleted) and quota_exceeded entries */
					if((string_search_keyword(res2, "(deleted)") == -1) && (string_search_keyword(res2, "(deleted)") == -1)){
						/* add entry if ok */
						strcat2(&tdomf_new, res2);
						strcat2(&tdomf_new, "\n");
					}
				}
				free2(res2);
			}
			strcat2(&tdomf_new, "\n");
		}
		free2(res);
	}

	free2(tdomf);
	tdomf = tdomf_new;
}


/* reload config files from variables to kernel memory */
/* this is done using "select" and "delete" keywords during file write */
/* for example:
 * select <kernel> /usr/sbin/httpd
 * allow_read /var/www/html/\*.html
 * delete allow_read /var/www/html/index.html
 * delete allow_read /var/www/html/welcome.html
*/
void reload()
{
	char *myappend;
	char *res, *res2, *rule, *temp, *temp2;
	char *tdomf_old, *tdomf_old2, *texcf_old, *texcf_old2;
	
	/* alloc mem for transitions */
	myappend = memget2(max_char);
	
	/* load old policy from kernel */
	tdomf_old = file_read(tdomk, 1);

	/* load domain policy to the kernel */
	while(1){	
		/* create append list with deleting old policy from kernel
		 * using "select" and "delete" keywords */
		temp = tdomf_old;
		while(1){
			/* get next domain policy */
			res = domain_get_next(&temp);
			if (!res) break;

			/* get full domain name with "<kernel>" tag */
			temp2 = res;
			res2 = string_get_next_line(&temp2);
			if (res2){
				/* append list with select <kernel> /bin/... */
				strcat2(&myappend, "select ");
				strcat2(&myappend, res2);
				strcat2(&myappend, "\n");

				/* append list with delete rules */
				while(1){
					/* get next rule */
					rule = string_get_next_line(&temp2);
					if (!rule) break;
					/* add only if not an empty line */
					if (strlen2(&rule)){
						/* add delete keyword and rule to append list */
						strcat2(&myappend, "delete ");
						strcat2(&myappend, rule);
						strcat2(&myappend, "\n");
					}
					free2(rule);
				}
				free2(res2);

				/* add "use_profile 0" tag to set domain's profile back to zero too */
				strcat2(&myappend, "use_profile 0\n");
			}
			free2(res);
		}

		/* add new policy to append list */
		temp = tdomf;
		while(1){
			/* get next domain policy */
			res = domain_get_next(&temp);
			if (!res) break;

			/* get full domain name with "<kernel>" tag */
			temp2 = res;
			res2 = string_get_next_line(&temp2);
			if (res2){
				/* append list with select <kernel> /bin/... */
				strcat2(&myappend, "select ");
				strcat2(&myappend, res2);
				strcat2(&myappend, "\n");

				/* append list with delete rules */
				while(1){
					/* get next rule */
					rule = string_get_next_line(&temp2);
					if (!rule) break;
					/* add only if not an empty line */
					if (strlen2(&rule)){
						/* add rule to append list */
						strcat2(&myappend, rule);
						strcat2(&myappend, "\n");
					}
					free2(rule);
				}
				free2(res2);
			}
			free2(res);
		}
		
		/* safety code: load old policy again to check if it hasn't changed
		 * since i started creating the new one */
		tdomf_old2 = file_read(tdomk, 1);
		/* sometimes the read tomoyo policy differs in the number
		 * of new lines at the end of text, therefore i remove
		 * empty lines from the end before compare */
		if (!strcmp(tdomf_old, tdomf_old2)){
			free2(tdomf_old2);
			/* write changes to kernel */
			file_write(tdomk, myappend);
			break;
		}
		else{
			free2(tdomf_old);
			tdomf_old = tdomf_old2;
		}
	}
	
	free2(tdomf_old);

	/* load old policy from kernel */
	texcf_old = file_read(texck, 1);
	
	/* load exception policy to kernel too */
	while(1){
	
		/* create another append list with deleting exception policy lines */
		strnull2(&myappend);
		temp = texcf_old;
		while(1){
			/* get next line of exception policy */
			res = string_get_next_line(&temp);
			if (!res) break;
			/* append lines with delete keyword */
			strcat2(&myappend, "delete ");
			strcat2(&myappend, res);
			strcat2(&myappend, "\n");
			free2(res);
		}

		/* cadd new rules to append list */
		temp = texcf;
		while(1){
			/* get next line of exception policy */
			res = string_get_next_line(&temp);
			if (!res) break;
			/* append lines with delete keyword */
			strcat2(&myappend, res);
			strcat2(&myappend, "\n");
			free2(res);
		}
		
		/* safety code: load old policy again to check if it hasn't changed
		 * since i started creating the new one */
		texcf_old2 = file_read(texck, 1);
		if (!strcmp(texcf_old, texcf_old2)){
			free2(texcf_old2);
			/* write changes to kernel */
			file_write(texck, myappend);
			break;
		}
		else{
			free2(texcf_old);
			texcf_old = texcf_old2;
		}
	}

	free2(texcf_old);
	free2(myappend);
}


/* save config files from variables to disk */
void save()
{
	/* save configs to disk */
	file_write(tdom, tdomf);
	file_write(texc, texcf);
	/* save log mark to disk */
	file_write(tmark, tmarkf);
}


/* create backup and save config files from variables to disk with new names */
void backup()
{
	char *tdom2 = 0, *texc2 = 0, *num;
	struct timeval t;
	
	/* get elapsed seconds since 1970 */
	gettimeofday(&t, 0);
	/* convert long integer to string */
	num = string_ltos(t.tv_sec);

	/* create new uniqe names to config files */
	strcpy2(&tdom2, tdom);
	strcat2(&tdom2, ".backup.");
	strcat2(&tdom2, num);
	strcpy2(&texc2, texc);
	strcat2(&texc2, ".backup.");
	strcat2(&texc2, num);
	free2(num);

	/* save configs to backup files on disk */
	file_write(tdom2, tdomf);
	file_write(texc2, texcf);
	
	free2(tdom2);
	free2(texc2);
}


/* check if only one instance of the program is running */
int check_instance(){
	char *mypid;

	/* store path to my executable */
	/* to a global variable for later */
	my_exe_path = process_get_path(getpid());

	/* get my pid number and convert it to string */
	mypid = string_itos(getpid());
	/* check if my pid file exists */
	if (file_exist(tpid)){
		char *pid2;
		/* read pid number from pid file */
		pid2 = file_read(tpid, 0);
		/* is it me? */
		if (!strcmp(mypid, pid2)){ free2(mypid); free2(pid2); return 0; }
		else{
			/* is the process with the foreign pid still running? */
			char *path;
			path = memget2(max_num);
			strcpy2(&path, "/proc/");
			strcat2(&path, pid2);
			/* if running, then return false */
			if (dir_exist(path)){ free2(mypid); free2(pid2); free2(path); return 0; }
			/* if not running, then overwrite pid number in pid file */
			else{
				file_write(tpid, mypid);
			}
			free2(path);
		}
		free2(pid2);
	}
	/* create pid file if it doesn't exist */
	else{
		file_write(tpid, mypid);
	}

	free2(mypid);
	
	return 1;
}


/* search file name in current dir first, then in bin locations and give back full path on success */
/* returned value must be freed by caller */
char *which(const char *name){
	char *res, *full;
	int i;
	
	/* name starts with "/" char? */
	if (name[0] == '/'){
		/* file exists? if not, then fail */
		if (file_exist(name)){
			/* return resolved link path */
			return link_read(name);
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
		res = link_read(full);
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
			res = link_read(full);
			free2(full);
			return res;
		}
		strnull2(&full);
	}

	free2(full);
	return 0;
}


/* create profile.conf and manager.conf files and load them to kernel if they are different */
void create_prof()
{
	char *tmanf_old, *tmanf, *tmanf2 = 0, *tprof_old, *tprof, *tprof2, *res;
	
	/* check tomoyo version */
	if (tomoyo_version() <= 2299){ tmanf = tmanf22; tprof = tprof22; }
	else                         { tmanf = tmanf23; tprof = tprof23; }

	/* load manager.conf from kernel and check if it's the same what i have */
	/* if identical, then no reload is needed */
	tmanf_old = file_read(tmank, 1);
	strcpy2(&tmanf2, tmanf);

	/* add my executable too to the binary list in manager.conf */
	strcat2(&tmanf2, my_exe_path);
	strcat2(&tmanf2, "\n");

	/* sort lines before compare */
	res = string_sort_uniq_lines(tmanf_old);
	free2(tmanf_old); tmanf_old = res;
	res = string_sort_uniq_lines(tmanf2);
	free2(tmanf2); tmanf2 = res;
	
	/* compare kernel manager config and mine */
	/* reload it to kernel if they are not identical */
	if (strcmp(tmanf2, tmanf_old)){
		char *comm = 0;
		/* write config to disk */
		file_write(tman, tmanf2);
		/* load config from disk to kernel */
		strcpy2(&comm, tload);
		strcat2(&comm, " m");
		/* this system() call here is the only one and this is unavoidable
		 * because only the external tomoyo-loadpolicy had the right to
		 * upload my new manager.conf config to the kernel,
		 * but from now on i have the right too to change policy through /sys */
		system(comm);
		free2(comm);
	}
	
	/* load profile.conf from kernel and check if it's the same what i have */
	/* if identical, then no reload is needed */
	tprof_old = file_read(tprok, 1);

	/* sort lines before compare */
	res = string_sort_uniq_lines(tprof_old);
	free2(tprof_old); tprof_old = res;
	tprof2 = string_sort_uniq_lines(tprof);
	
	/* compare kernel profile config and mine */
	/* reload it to kernel if they are not identical */
	if (strcmp(tprof2, tprof_old)){
		/* write profiles to disk */
		file_write(tpro, tprof);
		/* write profiles to kernel */
		file_write(tprok, tprof);
	}

	free2(tmanf_old);
	free2(tprof_old);
	free2(tmanf2);
	free2(tprof2);
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
		char *myarg = 0;
		
		/* cycle through the arguments */
		while (argc2--){
			int flag_ok = 0;
			strcpy2(&myarg, argv[c]);
			
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
						strcpy2(&opt_info2, myarg);
					}
					/* it belongs to the extra executables, so store it */
					else flag_progs = 1;
				}
				/* if last option arg was --remove, then this belongs to it */
				if (flag_type == 3){
					/* if former arg was an option, then it belongs to it */
					if (flag_last){
						/* store as --remove parameter */
						strcpy2(&opt_remove2, myarg);
					}
					/* it belongs to the extra executables, so store it */
					else flag_progs = 1;
				}
				/* if last option arg was --recursive, then this belongs to it */
				if (flag_type == 4){
					int l;
					flag_progs = 0;
					/* root "/" dir not allowed */
					if (!strcmp(myarg, "/")){ color_("error: root directory is not allowed", red); free2(myarg); myexit(1); }
					/* check if dir name exist */
					if (!dir_exist(myarg)){
						color_("error: no such directory: ", red); color_(myarg, red); newl(); free2(myarg); myexit(1); }
					/* expand recursive dir names with "/" char if missing */
					l = strlen2(&myarg);
					if (myarg[l-1] != '/'){ myarg[l] = '/'; myarg[l+1] = 0; }
					/* if so, store it in recursive dir array */
					strcat2(&dirs_recursive, myarg);
					strcat2(&dirs_recursive, "\n");
				}
				/* if argument doesn't belong to any option, then it goes to the extra executables */
				if (!flag_type || flag_progs){
					/* search for name in paths and check if file exists */
					char *res = which(myarg);
					if(!res){
						color_("error: no such file: ", red); color_(myarg, red); newl(); free2(myarg); myexit(1); }
					/* if file exists, store it as extra executables */
					strcat2(&tprogs, res);
					strcat2(&tprogs, "\n");
					free2(res);
				}
			}

			/* store the last option type */
			flag_last = flag_ok;

			c++;
		}
		
		free2(myarg);

		/* exit after --version or --help option */
		if (opt_version || opt_help){
			if (opt_version) version();
			if (opt_help)    help();
			myexit(0);
		}

		/* fail if no arguments for --remove option */
		if (opt_remove && !strlen2(&opt_remove2)){ color_("error: bad argument\n", red); myexit(1); }
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
	cmd = file_read("/proc/cmdline", 1);
	if (string_search_keyword(cmd, " security=tomoyo") == -1){
		free2(cmd);
		color_("error: tomoyo kernel mode is not activated\n", red);
		myexit(1);
	}
	free2(cmd);

	/* check mount state of securityfs */
	cmd = file_read("/proc/mounts", 1);
	if (string_search_keyword(cmd, "none /sys/kernel/security securityfs") == -1){
		int flag_mount = 0;
		/* mount tomoyo securityfs if not mounted */
		/* shell command: mount -t securityfs none /sys/kernel/security */
		flag_mount = mount("none", "/sys/kernel/security", "securityfs", MS_NOATIME, 0);
		if (flag_mount == -1){
			free2(cmd);
			color_("error: tomoyo securityfs cannot be mounted\n", red);
			myexit(1);
		}
	}
	free2(cmd);

	color("tomoyo kernel mode is active\n", clr);
	
	/* check tomoyo version */
	tver = tomoyo_version();
	if (tver < 2200 || tver > 2399){
		color_("error: tomoyo version is not compatible\n", red);
		myexit(1);
	}

	/* check tomoyo tool files */
	if (!file_exist(tinit)){ color_("error: ", red); color_(tinit, red); color_(" executable binary missing\n", red); myexit(1); }
	if (!file_exist(tload)){ color_("error: ", red); color_(tload, red); color_(" executable binary missing\n", red); myexit(1); }
/*	if (!file_exist(tsave)){ color_("error: ", red); color_(tsave, red); color_(" executable binary missing\n", red); myexit(1); }*/

	/* create tomoyo dir if it doesn't exist yet */
	if (!dir_exist(tdir)){ mkdir(tdir, S_IRWXU); }
}


/* clear config files */
void clear()
{
	/* create new configs */
	strnull2(&tdomf);
	strnull2(&texcf);
	strcpy2(&tdomf, "<kernel>\nuse_profile 0\n\n");
	strcpy2(&texcf, "initialize_domain /sbin/init\n");
	/* write config files */
	file_write(texc, texcf);
	file_write(tdom, tdomf);
	/* load config files from disk to memory */
	reload();
}


/* info about domains by a pattern */
void domain_info(const char *pattern)
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
			if (i == -1) free2(res2);
			else{
				char *text_new, *text_temp;
			
				/* increase counter for summary */
				count++;

				/* print new line to stderr only once for header */
				if (!flag_first){ newl_(); flag_first = 1; }
				else newl();
				/* print header part in blue */
				color(res2, blue); newl();
				free2(res2);
				/* print use_profile here too */
				res2 = string_get_next_line(&res_temp);
				color(res2, green); newl();
				free2(res2);
				
				/* sort the rest of the policy text */
				text_new = string_sort_uniq_lines(res_temp);
				text_temp = text_new;

				/* print the rest of the domain part */
				while(1){
					res2 = string_get_next_line(&text_temp);
					/* exit on end of domain block */
					if (!res2) break;
					/* print non empty lines */
					if (strlen2(&res2)){
						char *h = 0;
						strcpy2(&h, home);
						strcat2(&h, "/");
					
						/* print rules with reading libs in yellow */
						if (string_search_keyword_first(res2, "allow_read /lib/") ||
						    string_search_keyword_first(res2, "allow_read /usr/lib/")){
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
						free2(h);
					}
					free2(res2);
				}
				free2(text_new);
			}
			free2(res);
		}
		
		/* print summary */
		if (count){
			char *res = string_itos(count);
			newl();
			color("(found ", clr); color(res, clr);
			if (count == 1) color(" domain)\n", clr);
			else            color(" domains)\n", clr);
			free2(res);
		}
		else color_("error: no domains found\n", red);
	}

	/* list domain names only */	
	else{
		char *tdomf2, *res, *res2, *res_temp;
		char *texcf_new = 0;
		
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
				strcat2(&texcf_new, res2);
				strcat2(&texcf_new, "\n");
				free2(res2);
			}
			free2(res);
		}
		
		res = string_sort_uniq_lines(texcf_new);
		newl(); color(res, blue); newl();
		free2(res);
		free2(texcf_new);
	}
}


/* remove domains by a pattern */
void domain_remove(const char *pattern)
{
	/* load config files from kernel memory */
	load();

	/* is there any pattern? */
	if (pattern[0]){
		char *res, *res2, *res3, *temp, *temp2, *temp3;
		char *tdomf_new, *texcf_new;
		int i;
		int count = 0;
		int count2 = 0;

		/* count matching domains */
		temp = tdomf;
		while(1){
			/* get next domain */
			res = domain_get_next(&temp);
			/* exit on end */
			if (!res) break;

			/* get full domain name */
			temp2 = res;
			res2 = string_get_next_line(&temp2);
			if (res2){
				/* search for a keyword */
				i = string_search_keyword(res2, pattern);
				/* count domain if match */
				if (i > -1) count++;
				free2(res2);
			}
			free2(res);
		}
		
		/* print summary */
		if (count){
			char *res = string_itos(count);
			newl();
			color("(found ", clr); color(res, clr);
			if (count == 1) color(" domain)\n", clr);
			else            color(" domains)\n", clr);
			free2(res);
			
			/* alloc mem for new domain policy */
			tdomf_new = memget2(max_char);
			
			temp = tdomf;
			while(1){
				/* get next domain */
				res = domain_get_next(&temp);
				/* exit on end */
				if (!res) break;

				/* get full domain name */
				temp2 = res;
				res2 = string_get_next_line(&temp2);
				if (res2){
					/* search for a keyword */
					i = string_search_keyword(res2, pattern);
					/* remove domain if match */
					if (i > -1){
						/* print info */
						color(res2, blue);
						color("  ", clr);
						if (!choice("remove domain?")){
							/* add domain if answer is no */
							strcat2(&tdomf_new, res);
							strcat2(&tdomf_new, "\n");
						}
						else{
							char *s = 0;
							/* count for remove */
							count2++;
							/* remove domain from exception policy too */
							/* get main domain name */
							temp3 = res2;
							res3 = string_get_next_wordn(&temp3, 1);
							/* create domain entry */
							strcpy2(&s, "initialize_domain ");
							strcat2(&s, res3);
							/* remove domain entry from exception policy */
							texcf_new = string_remove_line(texcf, s);
							free2(texcf); texcf = texcf_new;
							free2(res3);
							free2(s);
						}
					}
					/* add domain to new policy if no match */
					else{
						strcat2(&tdomf_new, res);
						strcat2(&tdomf_new, "\n");
					}
					free2(res2);
				}
				free2(res);
			}
			
			/* replace old policy with new one */
			free2(tdomf);
			tdomf = tdomf_new;

			/* print info */			
			if (count2 > 0){
				if (count2 == 1) color("1 domain removed\n\n", red);
				else{
					char *s = string_itos(count2);
					color(s, red);
					color(" domains removed\n\n", red);
					free2(s);
				}

				/* reload and save new policy */
				reload();
				save();
			}
			else color("no domain was removed\n\n", green);
		}
		else color_("error: no domains found\n", red);
	}
}


/* turn back learning mode for all domains with profile 2-3 */
void domain_set_learn_all()
{
	char *res, *temp, *orig;
	
	/* load config files */
	load();

	/* cycle through domains */
	temp = tdomf;
	while(1){
		/* get next domain */
		orig = temp;
		res = domain_get_next(&temp);
		if (!res) break;
		/* turn domain into learning mode */
		domain_set_profile(orig, 1);
		free2(res);
	}
	
	/* save config files and load them to kernel */
	reload();
}


/* turn on enforcing mode for old domains only with profile 1-2 */
void domain_set_enforce_old()
{
	char *res, *prog, *prog_sub, *temp, *orig;
	int m;
	int flag_old = 0;
	int flag_turned = 0;

	/* is --keep option on? */
	if (!opt_keep){

		/* load config files */
		load();

		/* check if there are old domains with enforcing mode */
		temp = tdomf;
		while(1){
			/* get next domain */
			orig = temp;
			res = domain_get_next(&temp);
			if (!res) break;
			/* get domain profile */
			m = domain_get_profile(orig);
			/* there are old enforcing mode domains, i have the answer, so quit */
			if (m == 3){ flag_old = 1; break; }
			free2(res);
		}

		/* turn old domains into enforcing mode */
		temp = tdomf;
		while(1){
			/* get next domain */
			orig = temp;
			res = domain_get_next(&temp);
			if (!res) break;
			/* get domain profile */
			m = domain_get_profile(orig);
			/* is domain in mode 1 or 2? */
			if (m == 1 || m == 2){
				/* get domain name */
				prog = domain_get_name(res);
				prog_sub = domain_get_name_sub(res);

				/* check if it's not a newly created domain */
				if (string_search_line(tprogs_learn, prog_sub) == -1){
					int i = string_search_line(tprogs_exc, prog_sub);
					/* check if not an exception domain */
					/* or if an exception domain, check if it's not a main domain */
					if ((i == -1) || (i != -1 && !domain_main_exist(prog))){

						/* print info once */
						if (!flag_turned){
							flag_turned = 1;
							color("* turn on enforcing mode for old domains\n", green);
						}
						/* turn enforcing mode on for domain and all its subdomains */
						color(prog, blue); newl();
						domain_set_profile_all(prog, 3);
					}
				}
				free2(prog);
				free2(prog_sub);
			}
			free2(res);
		}

		/* save config files to disk and load them to kernel */		
		reload();
		
		/* did i turn any old domains into enforcing mode? */
		if (!flag_turned){
			if (flag_old) color("* all old domains in enforcing mode already\n", green);
			else color("* no domains in enforcing mode currently\n", green);
		}
	}
	else{
		color("keep domains unchanged for now on demand\n", red);
	}
}

/* -------------------------------------------------------------------------------------- */


/* get recent access deny logs */
void domain_get_log()
{
	/* vars */
	char *res, *res2, *temp, *temp2, *orig;
	char *start, *tlogf2 = 0, *tlogf3 = 0;
	char *rules = 0, *prog_rules = 0;
	char *key = 0;
	char key2[] = "denied for ";
	int i, i2, l;
	int key2_len = 11;


	/* ------------------- */
	/* ----- get log ----- */
	/* ------------------- */

	/* load log mark from file if not loaded yet */
	if (!tmarkf){
		if (file_exist(tmark)){
			tmarkf = file_read(tmark, 0);
		}
	}
	/* delete former log */
	free2(tlogf);
	/* read in new log */
	tlogf = file_read(tlog, 0);
	
	/* get messages only from mark, so jump to mark if it exists */
	start = tlogf;
	if (tmarkf){
		i = string_search_keyword(start, tmarkf);
		if (i > -1){
			start += i;
			/* jump after the next line where i found the log mark */
			string_jump_next_line(&start);
		}
		/* clear tmarkf if no previous log mark found */
		/* it might mean the log has been rotated already */
		else{
			if (tmarkf) strnull2(&tmarkf);
			start = tlogf;
		}
	}
	
	/* search for tomoyo error messages from mark */
	/* collect access deny messages */
	if (tomoyo_version() <= 2299) strcpy2(&key, " TOMOYO-ERROR: Access ");
	else strcpy2(&key, " ERROR: Access ");
	temp = start;
	while(1){
		/* jump to the beginning of the next tomoyo error message */
		i = string_search_keyword(temp, key);
		/* exit on fail meaning no more messages */
		if (i == -1) break;
		temp += i;
		/* get the whole line of the message */
		res = string_get_next_line(&temp);
		if (res){
			/* store it */
			strcat2(&tlogf2, res);
			strcat2(&tlogf2, "\n");
			free2(res);
		}
	}
	/* collect domain deny messages */
	if (tomoyo_version() <= 2299) strcpy2(&key, " TOMOYO-ERROR: Domain ");
	else strcpy2(&key, " ERROR: Domain ");
	temp = start;
	while(1){
		/* jump to the beginning of the next tomoyo error message */
		i = string_search_keyword(temp, key);
		/* exit on fail meaning no more messages */
		if (i == -1) break;
		temp += i;
		/* get the whole line of the message */
		res = string_get_next_line(&temp);
		if (res){
			/* store it */
			strcat2(&tlogf3, res);
			strcat2(&tlogf3, "\n");
			free2(res);
		}
	}
	
	free2(key);
	
	/* debug part to print domain deny messages if any */
	if (strlen2(&tlogf3)){
		debug(tlogf3);
	}

	/* set mark to the last log line */
	temp = tlogf;
	temp2 = 0;
	i = -1;
	/* search for the last kernel message in the log */
	while(1){
		orig = temp;
		res = string_get_next_line(&temp);
		if (!res) break;
		i = string_search_keyword(res, "kernel: [");
		if (i > -1) temp2 = orig;
		free2(res);
	}

	/* temp2 shows the beginning of the line of the last kernel message */
	if (temp2){
		i = string_search_keyword(temp2, "kernel: [");
		i2 = string_search_keyword(temp2 + i, "]");
		if (i > 1 && i2 > -1){
			/* set tmarkf to the last kernel messages time stamp */
			*(temp2 + i + i2 + 1) = 0;
			strcpy2(&tmarkf, temp2 + i);
			strlenset3(&tmarkf, i2 + 1);
		}
	}
	/* clear tmarkf if no kernel messages */
	else{
		if (tmarkf) strnull2(&tmarkf);
	}


	/* --------------------------------- */
	/* ----- convert logs to rules ----- */
	/* --------------------------------- */

	l = strlen("TOMOYO-ERROR: Access '");
	temp = tlogf2;
	while(1){
		/* next rule */
		temp2 = temp;
		res = string_get_next_line(&temp);
		if (!res) break;
		/* get allow_ type */
		temp2 += l + 1,
		res2 = string_get_next_word(&temp2);
		if (!res2){
			color_("error: unexpected error, log message format is not correct\n", red);
			free2(res);
			free2(tlogf2);
			free2(tlogf3);
			free2(prog_rules);
			free2(rules);
			myexit(1);
		}
		/* does it contain a "(" char? */
		i = string_search_keyword(res2, "(");
		/* if so, them remove it */
		if (i > -1){
			res2[i] = 0;
		}
		/* add together allow_ type */
		strcpy2(&rules, "allow_");
		strcat2(&rules, res2);
		strcat2(&rules, " ");
		free2(res2);
		
		/* get parameters of rule */
		res2 = string_get_next_word(&temp2);
		if (!res2){
			color_("error: unexpected error, log message format is not correct\n", red);
			free2(res);
			free2(tlogf2);
			free2(tlogf3);
			free2(prog_rules);
			free2(rules);
			myexit(1);
		}
		i = strlen2(&res2);
		if (i > 0) res2[i - 1] = 0;
		/* add all together, and rule is complete */
		strcat2(&rules, res2);
		free2(res2);

		
		/* get program name the rule belongs to */
		/* jump behind "denied for " part */
		i = string_search_keyword(temp2, key2);
		if (i == -1){
			color_("error: unexpected error, log message format is not correct\n", red);
			free2(res);
			free2(tlogf2);
			free2(tlogf3);
			free2(prog_rules);
			free2(rules);
			myexit(1);
		}
		/* get binary name */
		temp2 += i + key2_len;
		res2 = string_get_next_word(&temp2);
		if (!res2){
			color_("error: unexpected error, log message format is not correct\n", red);
			free2(res);
			free2(tlogf2);
			free2(tlogf3);
			free2(prog_rules);
			free2(rules);
			myexit(1);
		}
		/* create text for sorting in a format like "binary allow_ rule" */
		strcat2(&prog_rules, res2);
		strcat2(&prog_rules, " ");
		strcat2(&prog_rules, rules);
		strcat2(&prog_rules, "\n");
		free2(res2);

		
		free2(res);
	}


	/* ------------------------------- */
	/* ----- add rules to policy ----- */
	/* ------------------------------- */

	if (prog_rules){
		char *tdomf_new, *prog, *rule, *orig, *prog_rules_new = 0;

		/* sort and uniq rules */
		res = string_sort_uniq_lines(prog_rules);
		free2(prog_rules);
		prog_rules = res;

		if (strlen2(&prog_rules)) color("* adding log messages to policy\n", yellow);

		/* cycle through new rules and ask for confirmation to add it to domain policy */
		temp = prog_rules;
		while(1){
			/* get next rule */
			res = string_get_next_line(&temp);
			if (!res) break;
			/* get prog name and its rules */
			temp2 = res;
			prog = string_get_next_word(&temp2);
			rule = string_get_next_line(&temp2);
			/* print and confirm */
			color(prog, blue);
			color("  ", clr);
			color(rule, purple);
			if (choice("  add rules?")){
				/* add rules to new rules */
				strcat2(&prog_rules_new, res);
				strcat2(&prog_rules_new, "\n");
			}
			free2(prog);
			free2(rule);
			free2(res);
		}
		free2(prog_rules);
		prog_rules = prog_rules_new;
		
		/* are there any new rules after confirmation? */
		if (strlen2(&prog_rules)){

			/* alloc mem for new domain policy */
			tdomf_new = memget2(max_char);

			/* cycle through domains to add new rules */
			temp = tdomf;
			while(1){
				orig = temp;
				/* get next domain policy */
				res = domain_get_next(&temp);
				if (!res) break;
				/* add domain policy to new policy */
				strcat2(&tdomf_new, res);
				/* get subdomian name */
				res2 = domain_get_name_sub(res);
				if (res2){
					int flag_prof = 0;
					temp2 = prog_rules;
					while(1){
						/* get prog name and its rules */
						prog = string_get_next_word(&temp2);
						if (!prog) break;
						rule = string_get_next_line(&temp2);
						if (!rule){ free2(prog); break; }

						/* compare prog name to subdomain name */
						if (!strcmp(prog, res2)){
							/* if match, add rule to domain policy */
							strcat2(&tdomf_new, rule);
							strcat2(&tdomf_new, "\n");
							/* switch domain to learning mode */
							if (!flag_prof){
								domain_set_profile(orig, 1);
								/* do it only once per domain for speed */
								flag_prof = 1;
							}
						}

						free2(rule);
						free2(prog);
					}
					free2(res2);
				}
				strcat2(&tdomf_new, "\n");
				free2(res);
			}
			
			color("learning mode turned on for domains with new rules\n", red);

			/* replace old policy with new one */
			free2(tdomf);
			tdomf = tdomf_new;
		}
	}
	
	free2(prog_rules);
	free2(tlogf2);
	free2(tlogf3);
}


/* print programs already in domain but not in progs list */
void domain_print_list_not_progs()
{
	/* vars */
	char *res, *res2, *list, *list2 = 0, *list3, *temp;
	
	/* get list of all domain names */
	list = domain_get_list();

	/* remove entries from the list that are in tprogs too */
	temp = list;
	while(1){
		res = string_get_next_line(&temp);
		if (!res) break;
		/* does tprogs contain any of the domain names? */
		if (string_search_line(tprogs, res) == -1){
			strcat2(&list2, res);
			strcat2(&list2, "\n");
			/* add domain to tprogs if not in exceptions */
			if (string_search_line(tprogs_exc, res) == -1){
				strcat2(&tprogs, res);
				strcat2(&tprogs, "\n");
			}
		}
		free2(res);
	}
	
	free2(list);
	list = list2;
	
	/* is any element in the list? */
	if (strlen2(&list)){
		/* sort list */
		list2 = string_sort_uniq_lines(list);
		free2(list);
		list = list2;
		strnull2(&list2);
		
		color("* already existing main domains\n", green);


		/* get list of filenames only */
		temp = list;
		while(1){
			res = string_get_next_line(&temp);
			if (!res) break;
			res2 = string_get_filename(res);
			if (res2){
				strcat2(&list2, res2);
				strcat2(&list2, "\n");
				free2(res2);
			}
			free2(res);
		}
		
		/* sort filename list */
		list3 = string_sort_uniq_lines(list2);
		free2(list2);
		list2 = list3;
		
		/* print list */
		temp = list2;
		while(1){
			res = string_get_next_line(&temp);
			if (!res) break;
			color(res, blue);
			color(" ", blue);
			free2(res);
		}
		newl();
		
		free2(list2);
	}
	
	free2(list);
}


/* check if domain exist and wich mode it's in, and print info about it */
void domain_print_mode()
{
	/*vars */
	char *prog, *temp;
	int pos;
	
	if (flag_firstrun) color("* checking policy and rules\n", yellow);

	/* cycle thorugh progs */
	temp = tprogs;
	while(1){
		char *s = 0;
		
		prog = string_get_next_line(&temp);
		if(!prog) break;
		
		/* does the domain exist for the program? */
		strcpy2(&s, "initialize_domain ");
		strcat2(&s, prog);
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
			if (process_running(prog)) color(" (restart needed)", red);
			strcat2(&texcf, s);
			strcat2(&texcf, "\n");
		}
		free2(s);
		
		/* does the rule exist for it? */
		pos = domain_exist(prog);
		if (pos == -1){
			color(", no rule, ", clr);
			color("create rule with learning mode on", green);
			if (!flag_firstrun) newl();
			/* create a rule for it */
			strcat2(&tdomf, "<kernel> ");
			strcat2(&tdomf, prog);
			strcat2(&tdomf, "\nuse_profile 1\n");
			/* store prog name to know not to turn on enforcing mode for these ones on exit */
			if(string_search_line(tprogs_learn, prog) == -1){
				strcat2(&tprogs_learn, prog);
				strcat2(&tprogs_learn, "\n");
			}
		}
		else{
			int profile;
			if (flag_firstrun) color(", rule exists", clr);
			/* get profile mode for domain */
			profile = domain_get_profile(tdomf + pos);
			if (profile == -1){ color_("error: domain policy is corrupt\n", red); free2(prog); myexit(1); }
			/* check which mode is on */
			
			/* disabled mode */
			if (!profile){
				if (!flag_check3){
					color(prog, blue);
					flag_check3 = 1;
				}
				color(", disabled mode, ", clr);
				color("turn on learning mode", green);
				if (!flag_firstrun) newl();
				/* turn on learning mode for the domain and all its subdomains */
				domain_set_profile_all(prog, 1);
			}

			/* learning mode */
			if (profile == 1){
				if (flag_firstrun) color(", learning mode on", clr);
			}

			/* permissive mode */
			if (profile == 2){
				if (flag_firstrun) color(", permissive mode on", clr);
			}

			/* enforcing mode */
			if (profile == 3){
				if (flag_firstrun) color(", enforcing mode on", purple);
			}

		}
		
		if (flag_firstrun) newl();
		
		free2(prog);
	}
}


/* check change of policy, and return true if changed */
int check_policy_change(){
	/* backup policy exists yet? if not, then copy policy */
	if (!tdomf_bak){
		/* copy policy to backup */
		strcpy2(&tdomf_bak, tdomf);
		return 1;
	}
	else{
		/* policy files match? if not, then store it */
		if (!strcmp(tdomf_bak, tdomf)) return 0;
		else{
			/* copy policy to backup */
			strcpy2(&tdomf_bak, tdomf);
			return 1;
		}
	}
}


/* compare paths with wildcards */
/* a "/" or a null char must follow every wildcard in the path name */
/* with other words, only one '*' wildcard can be used per path tag */
/* return true if they match, even if both are null */
int compare_paths(char *path1, char *path2)
{
	/* vars */
	char c1, c2, c1_old, c2_old, c1_new, c2_new;
	int i1, i2, wlen;
	
	c1 = path1[0];
	c2 = path2[0];
	
	/* success if both are null */
	if (!c1 && !c2) return 1;
	if (!c1 || !c2) return 0;
	
	/* fail if they don't start with "/" char */
	if (c1 != '/' || c2 != '/') return 0;
	
	/* compare paths only */
	wlen = strlen(wildcard_recursive_dir);
	i1 = 0; i2 = 0;
	c1 = 0; c2 = 0;
	while(1){
		/* get next chars */
		c1_old = c1; c2_old = c2;
		c1 = path1[i1];
		c2 = path2[i2];
		
		/* store next coming chars too for later compare if not out of bound yet */
		if (c1) c1_new = path1[i1 + 1]; else c1_new = 0;
		if (c2) c2_new = path2[i2 + 1]; else c2_new = 0;
		
		/* if both chars are null then success */
		if (!c1 && !c2) return 1;
		
		/* if both chars match then continue, even if chars are "/" or "\\" */
		if (c1 == c2){
			i1++;
			i2++;
		}
		else{

			/* ---------------------------------------- */
			/* ----- check for recursive wildcard ----- */
			/* ---------------------------------------- */
			if (string_search_keyword_first(path1 + i1, wildcard_recursive_dir)){
				/* move this after wildcard */
				i1 += wlen;
				c1 = path1[i1];
				/* null or "/" should come after wildcard
				 * cause i don't insert recursive wildcard if it's not the last one */
				if (c1 && c1 != '/'){
					color("error: unexpected wildcard usage in domain rules\n", red);
					myexit(1);
				}
				/* move the other path poiter too to its last char (before null)
				 * so their last char should be "/" or nothing (null following) */
				while(1){
					c2 = path2[++i2];
					/* exit on end, and move char back one */
					if (!c2){
						c2 = path2[--i2];
						break; }
				}
				/* if both last chars are "/", then success */
				if (c1 == '/' && c2 == '/') return 1;
				else if (c1 == '/' && c2 != '/') return 0;
				else if (c1 != '/' && c2 == '/') return 0;
				/* or if not, then if both of them are null, it's success too
				 * (c2 would be null anyway, cause i moved it to the last char before */
				else if (!c1) return 1;
				return 0;
			}
			else if (string_search_keyword_first(path2 + i2, wildcard_recursive_dir)){
				/* move this after wildcard */
				i2 += wlen;
				c2 = path2[i2];
				/* null or "/" should come after wildcard
				 * cause i don't insert recursive wildcard if it's not the last one */
				if (c2 && c2 != '/'){
					color("error: unexpected wildcard usage in domain rules\n", red);
					myexit(1);
				}
				/* move the other path poiter too to its last char (before null)
				 * so their last char should be "/" or nothing (null following) */
				while(1){
					c1 = path1[++i1];
					/* exit on end, and move char back one */
					if (!c1){
						c1 = path1[--i1];
						break; }
				}
				/* if both last chars are "/", then success */
				if (c2 == '/' && c1 == '/') return 1;
				else if (c2 == '/' && c1 != '/') return 0;
				else if (c2 != '/' && c1 == '/') return 0;
				/* or if not, then if both of them are null, it's success too
				 * (c1 would be null anyway, cause i moved it to the last char before */
				else if (!c2) return 1;
				return 0;
			}

			/* ------------------------------------- */
			/* ----- check for normal wildcard ----- */
			/* ------------------------------------- */
			else{

				/* step 1 char forward if wildcard is coming */
				if (c1 == '\\' && c1_new == '*'){
					c1_old = c1; c1 = c1_new; ++i1; }
				if (c2 == '\\' && c2_new == '*'){
					c2_old = c2; c2 = c2_new; ++i2; }
				
				/* if any of the char is a wildcard "*", then check if before char is "\"
				 * and make the other jump to the next "/"
				 * this one is needed because it can happen that the "\" char matches
				 * but not "*" comes after one of them */
				if (c1 == '*' && c1_old == '\\'){
					/* null or "/" should come after wildcard
					 * cause i'm not using wildcard withing words, only by itself */
					c1 = path1[++i1];
					if (c1 && c1 != '/'){
						color("error: unexpected wildcard usage in domain rules\n", red);
						myexit(1);
					}
					/* if already standing on a "/" char, then ok and continue */
					if (c2 != '/'){
						/* else jump to next "/" */
						while(1){
							c2 = path2[++i2];
							if (c2 == '/' || !c2) break;
						}
					}
				}
				else if (c2 == '*' && c2_old == '\\'){
					/* null or "/" should come after wildcard */
					c2 = path2[++i2];
					if (c2 && c2 != '/'){
						color("error: unexpected wildcard usage in domain rules\n", red);
						myexit(1);
					}
					/* if already standing on a "/" char, then ok and continue */
					if (c1 != '/'){
						/* else jump to next "/" */
						while(1){
							c1 = path1[++i1];
							if (c1 == '/' || !c1) break;
						}
					}
				}
				/* no match, fail */
				else return 0;
			}
		}
	}
}


/* compare rules */
int compare_rules(char *r1, char *r2)
{
	/* vars */
	char *type1 = 0, *type2 = 0, *rule1a = 0, *rule1b = 0, *rule2a = 0, *rule2b = 0;
	char *temp1, *temp2;
	int flag_double = 1;
	
	/* fail if either of the rules are null */
	if (!r1[0] || !r2[0]) return 0;
	
	/* get rule types */
	temp1 = r1;
	type1 = string_get_next_word(&temp1);
	temp2 = r2;
	type2 = string_get_next_word(&temp2);
	/* fail if types are null */
	if (!type1 || !type2){
		free2(type1); free2(type2); return 0; }
	/* compare types, fail if no match */
	if (strcmp(type1, type2)){
		free2(type1); free2(type2); return 0; }
	
	/* get rule paths */
	rule1a = string_get_next_word(&temp1);
	rule1b = string_get_next_word(&temp1);
	/* fail if no first path in param 1 */
	if (!rule1a){
		free2(type1); free2(type2); return 0; }
	
	/* get second params if any */
	rule2a = string_get_next_word(&temp2);
	rule2b = string_get_next_word(&temp2);
	/* fail if no first path in param 2 */
	if (!rule2a){
		free2(type1); free2(type2); free2(rule1a); free2(rule1b);
		return 0;
	}
	/* second params exist too */
	if (rule2b) flag_double++;
	
	/* compare rules' paths */

	/* first params */
	if (!compare_paths(rule1a, rule2a)){
		free2(type1); free2(type2); free2(rule1a); free2(rule2a);
		free2(rule1b); free2(rule2b);
		return 0;
	}
	if (flag_double > 1){
		/* second params */
		if (!compare_paths(rule1b, rule2b)){
			free2(type1);  free2(type2);  free2(rule1a);
			free2(rule2a); free2(rule1b); free2(rule2b);
			return 0;
		}
	}

	/* free mem */
	free2(type1);
	free2(type2);
	free2(rule1a);
	free2(rule2a);
	free2(rule1b);
	free2(rule2b);

	return 1;
}


/* return new rules based on the input rule with wildcards of matching recursive dirs */
/* returned value must be freed by caller */
char *get_rules_with_recursive_dirs(char *rule)
{
	char *type, *path1, *path2;
	char *res, *res2, *temp, *rules_new;
	char *path_new1, *path_new2;
	int i, count1, count2;
	long c = 0;

	/* return null if input rule is null */
	if (!rule) return 0;

	/* are there any recursive dirs specified? */
	if (!dirs_recursive) return 0;

	/* get rule type */
	temp = rule;
	type = string_get_next_word(&temp);
	/* return null on empty line */
	if(!type) return 0;
	/* get path param 1 */
	path1 = string_get_next_word(&temp);
	/* get path param 2 */
	path2 = string_get_next_word(&temp);

	path_new1 = memget2(max_char);
	path_new2 = memget2(max_char);
	count1 = 0;
	count2 = 0;

	/* ----------------------- */
	/* ----- first param ----- */
	/* ----------------------- */
	/* cycle through recursive dirs */
	temp = dirs_recursive;
	i = 0;
	while(1){
		/* get next recursive dir name */
		res = string_get_next_line(&temp);
		if (!res) break;

		/* check if subdir name in the recursive dir is calculated yet */
		c = dirs_recursive_sub[i];
		if (c == -1){
			c = path_count_subdirs_name(res);
			dirs_recursive_sub[i] = c;
		}
		/* get beginning of path to compare it to recursive dir */
		res2 = path_get_subdirs_name(path1, c);
		
		/* compare them */
		if (compare_paths(res, res2)){
			/* success, store it on match and exit */
			strcpy2(&path_new1, res);

			/* if tomoyo version is under 2.3.x, then i have to manually add many "\*" wildcards
			 * to recursive dirs, so i have to calculate dir depth */
			if (tomoyo_version() <= 2299){
				/* check if depth is already caclulated yet */
				c = dirs_recursive_depth[i];
				if (c == -1){
					/* calculate depth */
					c = path_count_dir_depth(res);
					dirs_recursive_depth[i] = c;
				}
				count1 = c;
			}
			else count1 = 1;
			
			free2(res2);
			free2(res);
			break;
		}
		free2(res2);
		free2(res);
		i++;
	}

	/* ------------------------ */
	/* ----- second param ----- */
	/* ------------------------ */
	if (path2){
		/* cycle through recursive dirs */
		temp = dirs_recursive;
		i = 0;
		while(1){
			/* get next recursive dir name */
			res = string_get_next_line(&temp);
			if (!res) break;

			/* check if subdir name in the recursive dir is calculated yet */
			c = dirs_recursive_sub[i];
			if (c == -1){
				c = path_count_subdirs_name(res);
				dirs_recursive_sub[i] = c;
			}
			/* get beginning of path to compare it to recursive dir */
			res2 = path_get_subdirs_name(path2, c);
			
			/* compare them */
			if (compare_paths(res, res2)){
				/* success, store it on match and exit */
				strcpy2(&path_new2, res);

				/* if tomoyo version is under 2.3.x, then i have to manually add many "\*" wildcards
				 * to recursive dirs, so i have to calculate dir depth */
				if (tomoyo_version() <= 2299){
					/* check if depth is already caclulated yet */
					c = dirs_recursive_depth[i];
					if (c == -1){
						/* calculate depth */
						c = path_count_dir_depth(res);
						dirs_recursive_depth[i] = c;
					}
					count2 = c;
				}
				else count2 = 1;
				
				free2(res2);
				free2(res);
				break;
			}
			free2(res2);
			free2(res);
			i++;
		}
	}

	/* return null if no match */	
	if (!count1 && !count2){
		free2(type);
		free2(path1);
		free2(path2);
		free2(path_new1);
		free2(path_new2);
		return 0;
	}
	
	/* alloc mem for temp dir name */
	rules_new = memget2(max_char);

	/* add new rules with recursive wildcard if any match */
	/* if tomoyo version is under 2.3.x, then i have to manually add many "\*" wildcards
	 * to recursive dirs */
	if (tomoyo_version() <= 2299){
		/* only 1 param? */
		if (!path2){
			if (count1){
				while(count1--){
					int flag = 0;
					/* add rule type */
					strcat2(&rules_new, type);
					strcat2(&rules_new, " ");
					/* add new first param */
					strcat2(&rules_new, path_new1);
					/* add wildcards */
					c = count1 + 1;
					while(c--){
						if (!flag){ strcat2(&rules_new, "\\*"); flag = 1; }
						else        strcat2(&rules_new, "/\\*");
					}
					/* add "/" char if path is a dir */
					if (path_is_dir(path1)) strcat2(&rules_new, "/");
					strcat2(&rules_new, "\n");
				}
			}
		}
		/* 2 params */
		else{
			if (count1 && !count2){
				while(count1--){
					int flag = 0;
					/* add rule type */
					strcat2(&rules_new, type);
					strcat2(&rules_new, " ");
					/* add new first param */
					strcat2(&rules_new, path_new1);
					/* add wildcards */
					c = count1 + 1;
					while(c--){
						if (!flag){ strcat2(&rules_new, "\\*"); flag = 1; }
						else        strcat2(&rules_new, "/\\*");
					}
					/* add "/" char if path is a dir */
					if (path_is_dir(path1)) strcat2(&rules_new, "/");
					strcat2(&rules_new, " ");
					/* add old second param */
					strcat2(&rules_new, path2);
					strcat2(&rules_new, "\n");
				}
			}
			if (!count1 && count2){
				while(count2--){
					int flag = 0;
					/* add rule type */
					strcat2(&rules_new, type);
					strcat2(&rules_new, " ");
					/* add old first param */
					strcat2(&rules_new, path1);
					strcat2(&rules_new, " ");
					/* add new second param */
					strcat2(&rules_new, path_new2);
					/* add wildcards */
					c = count2 + 1;
					while(c--){
						if (!flag){ strcat2(&rules_new, "\\*"); flag = 1; }
						else        strcat2(&rules_new, "/\\*");
					}
					/* add "/" char if path is a dir */
					if (path_is_dir(path2)) strcat2(&rules_new, "/");
					strcat2(&rules_new, "\n");
				}
			}
		}
	}
	/* newer tomoyo version, so use special wildcard */
	else{
		/* only 1 param? */
		if (!path2){
			if (count1){
				/* add rule type */
				strcat2(&rules_new, type);
				strcat2(&rules_new, " ");
				/* add new first param */
				strcat2(&rules_new, path_new1);
				strcat2(&rules_new, wildcard_recursive_dir);
				/* add "/" char if path is a dir */
				if (path_is_dir(path1)) strcat2(&rules_new, "/");
				strcat2(&rules_new, "\n");
			}
		}
		/* 2 params */
		else{
			if (count1 && !count2){
				/* add rule type */
				strcat2(&rules_new, type);
				strcat2(&rules_new, " ");
				/* add new first param */
				strcat2(&rules_new, path_new1);
				strcat2(&rules_new, wildcard_recursive_dir);
				/* add "/" char if path is a dir */
				if (path_is_dir(path1)) strcat2(&rules_new, "/");
				/* add old second param */
				strcat2(&rules_new, " ");
				strcat2(&rules_new, path2);
				strcat2(&rules_new, "\n");
			}
			if (!count1 && count2){
				/* add rule type */
				strcat2(&rules_new, type);
				strcat2(&rules_new, " ");
				/* add old first param */
				strcat2(&rules_new, path1);
				/* add new second param */
				strcat2(&rules_new, " ");
				strcat2(&rules_new, path_new2);
				strcat2(&rules_new, wildcard_recursive_dir);
				/* add "/" char if path is a dir */
				if (path_is_dir(path2)) strcat2(&rules_new, "/");
				strcat2(&rules_new, "\n");
			}
		}
	}
	
	free2(type);
	free2(path1);
	free2(path2);
	free2(path_new1);
	free2(path_new2);

	return rules_new;
}


/* sort and uniq rules of the same types in a specific way */
/* returned value must be freed by caller */
char *domain_sort_uniq_rules(char *rules)
{
	char *res, *res2, *temp, *temp2, *old, *new, *rules_new, *rules2;
	int i1, i2, l1, l2, lo, ln;

	/* alloc mem for new rules */
	rules_new = memget2(max_char);
	old = memget2(max_char);
	new = memget2(max_char);
	
	/* cycle through rules and compare each to themselves */
	i1 = 0;
	temp = rules;
	while(1){
		res = string_get_next_line(&temp);
		if (!res) break;
		l1 = strlen2(&res);
		
		/* compare rule to all rules */
		i2 = 0;
		temp2 = rules;
		strnull2(&new);
		while(1){
			/* skip comparing the same entry in the list */
			if (i1 != i2){
				res2 = string_get_next_line(&temp2);
				if (!res2) break;
				
				/* compare rules containing wildcard */
				if (compare_rules(res, res2)){
					l2 = strlen2(&res2);
					if (l1 < l2){ strcpy2(&old, res); lo = l1; }
					else        { strcpy2(&old, res2); lo = l2; }
					/* if match, store only the shortest one from the matching ones */
					ln = strlen2(&new);
					if ((!ln) || (ln > lo)) strcpy2(&new, old);
				}

				free2(res2);
			}
			i2++;
		}

		/* store the shorter matched rule if there was any match */
		if (strlen2(&new)){
			strcat2(&rules_new, new);
			strcat2(&rules_new, "\n");
		}
		/* store the original rule if there was no match */
		else{
			strcat2(&rules_new, res);
			strcat2(&rules_new, "\n");
		}
		
		free2(res);
		i1++;
	}

	/* sort new rule list */
	rules2 = string_sort_uniq_lines(rules_new);
	free2(rules_new);
	rules_new = rules2;
	
	free2(old);
	free2(new);
	return rules_new;
}


/* sort and unique all rules in all domains */
void domain_cleanup()
{
	/* vars */
	char *res, *res2, *res3, *temp, *temp2, *temp3;
	char *orig, *tdomf_new, *rules, *rules2, *rules_temp, *rule_type;
	int c;
	
	/* alloc mem for new policy */
	tdomf_new = memget2(max_char);
	/* alloc mem for sorted rules */
	rules = memget2(max_char);
	
	/* cycle through domains and sort and make uniq the rules of each */
	temp = tdomf;
	while(1){
		/* get next domain policy */
		orig = temp;
		res = domain_get_next(&temp);
		if (!res) break;
		
		/* copy header part of domain (first 2 lines: <kernel> and use_profile) */
		temp2 = res;
		res2 = string_get_next_line(&temp2);
		if (res2){
			strcat2(&tdomf_new, res2);
			strcat2(&tdomf_new, "\n");
			free2(res2);
			
			res2 = string_get_next_line(&temp2);
			if (res2){
				strcat2(&tdomf_new, res2);
				strcat2(&tdomf_new, "\n");
				free2(res2);
				
				/* get only rules */
				strnull2(&rules);
				while(1){
					/* get next rule */
					res2 = string_get_next_line(&temp2);
					if (!res2) break;
					/* add none-empty rule line */
					if (strlen2(&res2)){
						strcat2(&rules, res2);
						strcat2(&rules, "\n");
					}
					free2(res2);
				}
				/* sort rules */
				res2 = string_sort_uniq_lines(rules);
				/* add sorted rules to new policy */
				strcat2(&tdomf_new, res2);
				strcat2(&tdomf_new, "\n");
				free2(res2);
			}
		}
		free2(res);
	}
	free2(rules);

	/* replace old policy with new one */
	free2(tdomf);
	tdomf = tdomf_new;


	/* alloc mem for new policy */
	tdomf_new = memget2(max_char);
	/* alloc mem for sorted rules */
	rules = memget2(max_char);
	rules_temp = memget2(max_char);
	rule_type = memget2(max_char);

	/* cycle through domains and sort and make uniq the rules within the same type */
	temp = tdomf;
	while(1){
		/* get next domain policy */
		orig = temp;
		res = domain_get_next(&temp);
		if (!res) break;

		/* copy header part of domain (first 2 lines: <kernel> and use_profile) */
		temp2 = res;
		res2 = string_get_next_line(&temp2);
		if (res2){
			strcat2(&tdomf_new, res2);
			strcat2(&tdomf_new, "\n");
			free2(res2);
			
			res2 = string_get_next_line(&temp2);
			if (res2){
				strcat2(&tdomf_new, res2);
				strcat2(&tdomf_new, "\n");
				free2(res2);
				
				/* get only same type of rules and sort and uniq them in a specific way */
				c = 0;
				strnull2(&rules_temp);
				while(1){
					/* get next rule */
					res2 = string_get_next_line(&temp2);
					if (!res2){
						/* on end of rules, add them to new policy if any */
						if (c > 0){
							/* sort rules if more than 1 is in the list */
							if (c > 1){
								rules2 = domain_sort_uniq_rules(rules);
								free2(rules);
								rules = rules2;
							}
							/* add rules to new policy */
							strcat2(&rules_temp, rules);
						}
						break;
					}

					/* speed up sorting by skipping lib reading rules */
					if (string_search_keyword_first(res2, "allow_read /lib/") ||
						string_search_keyword_first(res2, "allow_read /usr/lib/")){

						/* add rule to new policy */
						strcat2(&rules_temp, res2);
						strcat2(&rules_temp, "\n");
					}
					else{

						/* get the rule type (first param of rule) */
						temp3 = res2;
						res3 = string_get_next_word(&temp3);
						if (res3){

							/* check if there is any rule type stored yet */
							if (!c){
								/* if not, then store first type and store the rule too */
								strcpy2(&rule_type, res3);
								strcpy2(&rules, res2);
								strcat2(&rules, "\n");
								/* counter for number of rules stored in list */
								c = 1;
							}
							/* rule type already stored, so compare the former one with this one */
							/* and collect the same types for special sorting */
							else{
								if (!strcmp(rule_type, res3)){
									strcat2(&rules, res2);
									strcat2(&rules, "\n");
									/* increase number of rules stored already */
									c++;
								}
								/* types don't match, so sort them and start collecting the next type */
								else{
									/* sort rules if more than 1 is in the list */
									if (c > 1){
										rules2 = domain_sort_uniq_rules(rules);
										free2(rules);
										rules = rules2;
									}
									/* add rules to new temp policy */
									strcat2(&rules_temp, rules);
									/* store new rule type and next rule */
									strcpy2(&rule_type, res3);
									strcpy2(&rules, res2);
									strcat2(&rules, "\n");
									/* reset rule number */
									c = 1;
								}
							}
							free2(res3);
						}
					}
					free2(res2);
				}
				/* sort temp rules */
				rules2 = string_sort_uniq_lines(rules_temp);
				/* add sorted rules to new policy */
				strcat2(&tdomf_new, rules2);
				strcat2(&tdomf_new, "\n");
				free2(rules2);
			}
		}
		free2(res);
	}
	free2(rule_type);
	free2(rules);
	free2(rules_temp);
	
	/* replace old policy with new one */
	free2(tdomf);
	tdomf = tdomf_new;
}


/* add rules with wildcarded recursive dirs to all domain policy rules if they contain any */
void domain_reshape_rules_recursive_dirs()
{
	/* vars */
	char *res, *res2, *res3, *temp, *temp2;
	char *orig, *tdomf_new, *rules, *rules2;

	/* do the whole check if there are any recursive dirs at all, or else exit */	
	if (!dirs_recursive) return;
	
	/* alloc mem for new policy */
	tdomf_new = memget2(max_char);
	/* alloc mem for new rule */
	rules  = memget2(max_char);
	rules2 = memget2(max_char);
	
	/* cycle through domains and change all subdir names of all recursive dirs to fully wildcarded */
	temp = tdomf;
	while(1){
		/* get next domain policy */
		orig = temp;
		res = domain_get_next(&temp);
		if (!res) break;

		/* copy header part of domain (first 2 lines: <kernel> and use_profile) */
		temp2 = res;
		res2 = string_get_next_line(&temp2);
		if (res2){
			strcat2(&tdomf_new, res2);
			strcat2(&tdomf_new, "\n");
			free2(res2);
			
			res2 = string_get_next_line(&temp2);
			if (res2){
				strcat2(&tdomf_new, res2);
				strcat2(&tdomf_new, "\n");
				free2(res2);
				
				/* cycle through the rules */
				strnull2(&rules);
				while(1){
					/* get next rule */
					res2 = string_get_next_line(&temp2);
					if (!res2) break;
					
					/* store old rules */
					strcat2(&rules, res2);
					strcat2(&rules, "\n");
					
					/* get a modified rule by recursive dirs if any */
					res3 = get_rules_with_recursive_dirs(res2);
					if (res3){
						strcat2(&rules, res3);
						free2(res3);
					}

					free2(res2);
				}
				/* insert rule only if the former rule was not the same */
				/* this is to avoid massive multiplication of the rules because of the recursive check */
				if (strcmp(rules, rules2)){
					/* add if no match */
					strcat2(&tdomf_new, rules);
					strcat2(&tdomf_new, "\n");
					/* store new rule as old for next check */
					strcpy2(&rules2, rules);
				}
			}
		}
		free2(res);
	}
	free2(rules);
	free2(rules2);
	
	/* replace old policy with new one */
	free2(tdomf);
	tdomf = tdomf_new;
}


/* rehsape rules */
void domain_reshape_rules()
{
	domain_cleanup();
	
	domain_reshape_rules_recursive_dirs();

	domain_cleanup();
}


/* manage policy and rules */
void check()
{
	/* load config files */
	sand_clock(0);
	load();
	
	/* print programs already in domain but not in progs list */
	sand_clock(0);
	domain_print_list_not_progs();
	
	/* check if domain exist and which mode it's in */
	sand_clock(0);
	domain_print_mode();

	/* get recent access deny logs */
	sand_clock(0);
	domain_get_log();
	
	/* check change of policy and run if there is any change only, don't do unnecessary work */
	if(check_policy_change()){

		/* reshape rules */
		sand_clock(0);
		domain_reshape_rules();
		
		/* reload config files into memory */
		sand_clock(0);
		reload();

		sand_clock(2);
	}
	else{
		sand_clock(1);
	}
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
		while(1){
			char *res = tshellf2[i++];
			if (!res) break;
			strcpy2(&tshellf, res);
			strcat2(&tshellf, "\n");
		}
	}
	
	/* add shells to exceptions */
	if (strlen2(&tshellf)){
		temp = tshellf;
		while(1){
			res = string_get_next_line(&temp);
			if (!res) break;
			/* if line doesn't start with "#" char meaning a remark */
			if (res[0] != '#'){
				/* add it only if it exists */
				if (file_exist(res)){
					strcat2(&tprogs_exc, res);
					strcat2(&tprogs_exc, "\n");
				}
			}
			free2(res);
		}
	}

	/* add manual programs to exceptions */
	i = 0;
	while(1){
		res = tprogs_exc_manual[i++];
		if (!res) break;
		strcat2(&tprogs_exc, res);
		strcat2(&tprogs_exc, "\n");
	}

	/* add my executable too to the list */
	if (my_exe_path){
		strcat2(&tprogs_exc, my_exe_path);
		strcat2(&tprogs_exc, "\n");
	}

	/* sort exception list */
	if (tprogs_exc){
		res = string_sort_uniq_lines(tprogs_exc);
		free2(tprogs_exc);
		tprogs_exc = res;
	}

	/* sort program list */
	if (strlen2(&tprogs)){
		res = string_sort_uniq_lines(tprogs);
		/* realloc more mem above the sorted list to expand it later */
		free2(tprogs);
		tprogs = res;
	}

	/* initialize recursive dirs' depth and sub values */
	if (dirs_recursive){
		i = string_count_lines(dirs_recursive);
		if (i > 0){
			if (!dirs_recursive_depth) dirs_recursive_depth = memget_long(i+1);
			if (!dirs_recursive_sub)   dirs_recursive_sub   = memget_long(i+1);
			while(i--){
				dirs_recursive_depth[i] = -1;
				dirs_recursive_sub[i]   = -1;
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

	/* run these only once */
	if (!flag_new_proc){
		flag_new_proc = 1;

		/* print exceptions */
		if (tprogs_exc){
			char *temp = tprogs_exc;
			color("* exception domains\n", green);
			while(1){
				char *res = string_get_next_line(&temp);
				if (!res) break;
				color(res, purple);
				color(" ", purple);
				free2(res);
			}
			newl();
		}

		/* print on demand programs */
		if (tprogs){
			color("* additional programs on demand\n", green);
			color(tprogs, blue);
		}
		
		/* check processes using network */
		color("* new processes using network\n", green);
	}

	count2 = 0;
	while(1){
		char *netf2, *netf3;
		int i;

		DIR *mydir;
		struct dirent *mydir_entry;
		
		/* read up all net stat files and create a list of inode numbers (column 10)
		   of all processes using network */
		netf2 = memget2(max_char);
		i = 0;
		while (1){
			char *res, *res2, *res3, *temp, *temp2;
			if (!netf[i]) break;
			/* read file */
			res = file_read(netf[i++], 1);
			temp = res;
			/* skip one line */
			res2 = string_get_next_line(&temp);
			free2(res2);
			/* get all column 10 (uid) */
			while(1){
				/* get next line */
				res2 = string_get_next_line(&temp);
				if (!res2) break;
				temp2 = res2;
				res3 = string_get_next_wordn(&temp2, 9);
				if(res3){
					strcat2(&netf2, "socket:[");
					strcat2(&netf2, res3);
					strcat2(&netf2, "]\n");
					free2(res3);
				}
				free2(res2);
			}
			free2(res);
		}

		/* sort pid list */
		netf3 = string_sort_uniq_lines(netf2);
		free2(netf2);


		/* find all processes with the matching inode numbers in netf3's list */	
		/* open /proc dir */
		mydir = opendir("/proc/");
		if (!mydir){
			color_("error: cannot open /proc/ directory\n", red);
			free2(netf3);
			myexit(1);
		}
		/* cycle through dirs in /proc */
		while((mydir_entry = readdir(mydir))) {
			char *myprog, *mypid;
			/* get my pid number from dir name in /proc */
			mypid = memget2(max_num);
			strcpy2(&mypid, mydir_entry->d_name);
			/* does it contain numbers only meaning they are pids? */
			if (strspn(mypid, "0123456789") == strlen2(&mypid)) {
				char *mydir_name;
				/* create dirname like /proc/pid/exe */
				mydir_name = memget2(max_char);
				strcpy2(&mydir_name, "/proc/");
				strcat2(&mydir_name, mypid);
				strcat2(&mydir_name, "/exe");
				/* resolv the link pointing from the exe name */
				myprog = link_read(mydir_name);
				if (myprog){
					DIR *mydir2;
					struct dirent *mydir_entry2;
					char *myfd;
					/* create dir name like /proc/pid/fd/ */
					myfd = memget2(max_char);
					strcpy2(&myfd, "/proc/");
					strcat2(&myfd, mypid);
					strcat2(&myfd, "/fd/");
					mydir2 = opendir(myfd);
					
					if (mydir2){
						/* cycle through files in /proc/pid/fd/ */
						while((mydir_entry2 = readdir(mydir2))) {
							char *mysock;
							char *mydir_name2;
							/* create dirname like /proc/pid/fd/4 */
							mydir_name2 = memget2(max_char);
							strcpy2(&mydir_name2, myfd);
							strcat2(&mydir_name2, mydir_entry2->d_name);
							/* resolv the links from the /proc/pid/fd/ dir */
							mysock = link_read(mydir_name2);
							if (mysock){
								/* does it contain a name like "socket:"? */
								if (string_search_keyword(mysock, "socket:") > -1){
									/* is the inode number in netf3's list? */
									if (string_search_line(netf3, mysock) > -1){
										/* is it in my progs list yet? */
										if (string_search_line(tprogs, myprog) == -1 && string_search_line(tprogs_exc, myprog) == -1){
											strcat2(&tprogs, myprog);
											strcat2(&tprogs, "\n");
											if (flag_firstrun){ color(myprog, blue); newl(); }
										}
									}
								}
								free2(mysock);
							}
							free2(mydir_name2);
						}
						closedir(mydir2);
					}
					free2(myfd);
					free2(myprog);
				}
				free2(mydir_name);
			}
			free2(mypid);
		}
		closedir(mydir);



		free2(netf3);
		break;
	}
}


/* print statistics about domains */
void statistics()
{
	char *res, *temp, *sd, *sr;
	int d = 0;
	int r = 0;
	
	/* count domains */
	temp = tdomf;
	while(1){
		res = domain_get_next(&temp);
		if (!res) break;
		d++;
		free2(res);
	}

	/* count rules */
	temp = tdomf;
	while(1){
		res = string_get_next_line(&temp);
		if (!res) break;
		if (string_search_keyword_first(res, "allow_")) r++;
		free2(res);
	}
	
	/* print stat */
	sd = string_itos(d);
	sr = string_itos(r);
	color(sd, clr);
	color(" active domains, ", clr);
	color(sr, clr);
	color(" rules\n", clr);
	free2(sd);
	free2(sr);
}


/* ----------------------------------- */
/* ------------ MAIN PART ------------ */
/* ----------------------------------- */

int main(int argc, char **argv){

	float t, t_start;
	
	/* store start time */
	t_start = mytime();

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
	color("tomld (tomoyo learning daemon) ", clr); color(ver, clr); newl();
	
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
		domain_set_learn_all();
		color("all domains turned back to learning mode\n", red);
		myexit(0);
	}

	/* create excaption list from program names and available shells */
	check_exceptions();

	/* store negatív reference time for check() function to make check() run at least once */	
	t = -time_check;

	while(1){

		/* check running processes */
		check_processes();

		/* run policy check from time to time */
		if ((mytime() - t) >= time_check){
			flag_safe = 0;

			/* manage policy and rules */
			check();
			
			/* print only once */
			if (flag_firstrun){
				flag_firstrun = 0;
				color("* whole running cycle took ", green);
				printf("%.2fs\n", mytime());
				if (!opt_once){
					color("(press q to quit)\n", red);
				}
			}

			/* now it's safe to enforce mode and save config on interrupt, cause check() finished running */
			flag_safe = 1;
			
			/* exit if --once option is on */
			if (opt_once) break;
			
			/* reset time */
			t = mytime();
		}
		
		/* sleep some */
		usleep(500000);
		if (key_get() == 'q'){ newl(); break; }
	}

	if (flag_safe){
		/* turn on enforcing mode for all old domains before exiting */
		domain_set_enforce_old();
	}
	else if (flag_firstrun) color("* haven't finished to run at least once\n", red);
	
	/* print statistics */
	statistics();

	/* save configs to disk */
	save();

	/* free all my pointers */
	myfree();
	
	return 0;
}

