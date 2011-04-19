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
19/04/2011 - tomld v0.31 - complete rewrite of tomld from python to c language
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
#include <time.h>

#define max_char	1024
#define max_num		32
#define max_array	1024
#define max_file	1*1024*1024


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
int maxent = 10000;

/* tomoyo kernel parameter */
char *tkern = "security=tomoyo";

/* tomoyo vars and files */
char *tdomf	= "";
char *texcf	= "";
char *tprof	= ""
"PROFILE_VERSION=20090903\n"
"PREFERENCE::enforcing={ verbose=yes }\n"
"PREFERENCE::learning={ verbose=no max_entry=2048 }\n"
"PREFERENCE::permissive={ verbose=yes }\n"
"0-COMMENT=-----Disabled Mode-----\n"
"0-CONFIG={ mode=disabled }\n"
"1-COMMENT=-----Learning Mode-----\n"
"1-CONFIG={ mode=learning }\n"
"2-COMMENT=-----Permissive Mode-----\n"
"2-CONFIG={ mode=permissive }\n"
"3-COMMENT=-----Enforcing Mode-----\n"
"3-CONFIG={ mode=enforcing }\n";
char *tmanf	= "";
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

/* default profile.conf */



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


/* print debug info about a string */
void debug(char *text)
{
	unsigned long i;
	unsigned long c = 0;
	unsigned long l = strlen(text);
	/* count lines of text */
	for (i = 0; i < l; i++){ if (text[i] == '\n') c++; }
	/* print text and info */
	printf("--\n");
	printf(text);
	/* print newline if missing from the end of string */
	if (text[l-1] != '\n') printf("\n");
	printf("--\n");
	printf("len = %ld, ", strlen(text));
	printf("lines = %ld\n", c);
}


/* convert integer to ascii */
/* custom implementation because itoa function is missing from standard libs */
void itoa(int num, char *buf)
{
	sprintf(buf, "%d", num);
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


/* open pipe and read content */
void pipe_read(char *comm, char *buff, int len)
{
	FILE *p = popen(comm, "r");
	if (!p){
		color_("error: cannot read pipe from ", red);
		color_(comm, red);
		color_("\n", red);
		exit(1);
	}
	fread(buff, len, 1, p);
	pclose(p);
}


/* open file and read content */
void file_read(char *name, char *buff, int len)
{
	FILE *f = fopen(name, "r");
	if (!f){
		color_("error: cannot read file ", red);
		color_(name, red);
		color_("\n", red);
		exit(1);
	}
	fread(buff, len, 1, f);
	fclose(f);
}


/* open file and write content */
void file_write(char *name, char *buff)
{
	FILE *f = fopen(name, "w");
	if (!f){
		color_("error: cannot write file ", red);
		color_(name, red);
		color_("\n", red);
		exit(1);
	}
	fprintf(f, buff);
	fclose(f);
}


/* check kernel version, is it 2.6.33 or above? */
int kernel_version()
{
	char v[] = "/proc/sys/kernel/osrelease";
	char buff[max_char] = "";
	int c, c2;
	int ver = 0;
	char n;
	/* read in kernel version from /proc in a format as 2.6.32-5-amd64 */
	file_read(v, buff, sizeof buff);
	
	c = 0;
	/* read in version numbers in a format where 2.6.33 will be 263300 */
	c2 = 100000;
	while (c2){
		n = buff[c++];
		if (n != '.'){
			char conv[2] = "";
			/* exit if no numbers or end of string */
			if (!(n >= '0' && n <= '9') || !n) return ver;
			/* convert char to string */
			conv[0] = n; conv[1] = 0;
			/* convert string to integer and add it to result */
			ver += c2 * atoi(conv);
			c2 /= 10;
		}
	}

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


/* check if process with the given name is running */
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
		strcpy(mypid, mydir_entry->d_name);
		/* does it contain numbers only meaning they are pids? */
		if (strspn(mypid, "0123456789") == strlen(mypid)) {
			int res;
			char buff[max_char] = "";
			/* create dirname like /proc/pid/exe */
			strcpy(mydir_name, "/proc/");
			strcat(mydir_name, mypid);
			strcat(mydir_name, "/exe");
			/* resolv the link pointing from the exe name */
			res = readlink(mydir_name, buff, (sizeof buff) - 1);
			if (res > 0){
				/* put a zero end mark to the end of the string after string length */
				buff[res] = 0;
				/* compare the link to the process name */
				/* gives back zero on full match */
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


/* check if only one instance of the program is running */
int check_instance(){
	char mypid[max_num] = "";
	/* get my pid number and convert it */
	itoa(getpid(), mypid);
	/* check if my pid file exists */
	if (file_exist(pidf)){
		char pid2[max_num] = "";
		/* read pid number from pid file */
		file_read(pidf, pid2, sizeof pid2);
		/* is it me? */
		if (strcmp(mypid, pid2) == 0) return 0;
		else{
			/* is the process with the foreign pid still running? */
			char path[max_char] = "/proc/";
			strcat(path, pid2);
			/* if running, then return false */
			if (dir_exist(path)) return 1;
			/* if not running, then overwrite pid number in pid file */
			else{
				file_write(pidf, mypid);
			}
		}
	}
	/* create pid file if it doesn't exist */
	else{
		file_write(pidf, mypid);
	}

	return 1;
}


/* get absolute pathname of a file from a name */
/* first the current directory is checked, then the path list on the path env variable */
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
	strcpy(full, "./");
	strcat(full, name);
	if (file_exist(full)){ strcpy(name, full); return 1; }

	/* does the file exist in the paths from path env? */
	path = getenv("PATH");
	while((res = strtok(path, ":"))){
		strcpy(full, res);
		strcat(full, "/");
		strcat(full, name);
		if (file_exist(full)){ strcpy(name, full); return 1; }
		path = 0;
	}

	return 0;
}


void check_prof()
{
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
		char *myarg;
		/* cycle through the arguments */
		while (argc2--){
			int flag_ok = 0;
			myarg = argv[c];
			
			if (!strcmp(myarg, "-v") || !strcmp(myarg, "--verson"))	{ opt_version = 1;	flag_ok = 1; }
			if (!strcmp(myarg, "-h") || !strcmp(myarg, "--help"))	{ opt_help    = 1;	flag_ok = 1; }
			
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
						strcpy(opt_info2, myarg);
					}
					/* it belongs to the extra executables, so store it */
					else flag_progs = 1;
				}
				/* if last option arg was --remove, then this belongs to it */
				if (flag_type == 3){
					/* if former arg was an option, then it belongs to it */
					if (flag_last){
						/* store as --remove parameter */
						strcpy(opt_remove2, myarg);
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
					if (strcmp(myarg, "/") == 0){ color_("error: root directory is not allowed", red); exit(1); }
					/* check if dir name exist */
					if (!dir_exist(myarg)){
						color_("error: no such directory: ", red); color_(myarg, red); color_("\n", red); exit(1); }
					/* expand recursive dir names with "/" char if missing */
					strcpy(path, myarg);
					l = strlen(myarg);
					if (path[l-1] != '/'){ path[l] = '/'; path[l+1] = 0; }
					/* if so, store it in recursive dir array */
					strcpy(opt_recursive2[opt_recursive2_counter++], path);
				}
				/* if argument doesn't belong to any option, then it goes to the extra executables */
				if (!flag_type || flag_progs){
					char path[max_char] = "";
					strcpy(path, myarg);
					/* search for name in path env and check if file exist */
					if(!which(path)){
						color_("error: no such file: ", red); color_(path, red); color_("\n", red); exit(1); }
					/* if so, store it in extra executables */
					strcpy(progs[progs_counter++], myarg);
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
			exit(0);
		}

		/* fail if no arguments for --remove option */
		if (opt_remove && opt_remove2[0] == 0){ color_("error: bad argument\n", red); exit(1); }
		/* tail if no arguments for --recursive option */
		if (opt_recursive && !opt_recursive2_counter){ color_("error: bad argument\n", red); exit(1); }

	}
}


void check_options_colored(int argc, char **argv)
{
	/* is output colored? set color option before anything else */
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
	char comm[max_char] = "";
	char buff[max_char] = "";
	/* check tomoyo tool files */
	if (!file_exist(tload)){ color_("error: ", red); color_(tload, red); color_(" executable binary missing\n", red); exit(1); }
	if (!file_exist(tsave)){ color_("error: ", red); color_(tsave, red); color_(" executable binary missing\n", red); exit(1); }
	/* check status of tomoyo */
	strcpy(comm, tsave); strcat(comm, " u - 2>/dev/null");
	/* get the output of command "tomoyo-savepolicy u -" giving back memory stat */
	pipe_read(comm, buff, sizeof buff);
	/* if no "total" part, then tomoyo is not activated */
	if (!strstr(buff, "Total:")){ color_("error: tomoyo kernel mode is not activated\n", red); exit(1); }
	else{ color_("tomoyo kernel mode is active\n", clr); }
	/* create tomoyo dir if it doesn't exist yet */
	if (!dir_exist(tdir)){ mkdir(tdir, S_IRWXU); }
}


/* ----------------------------------- */
/* ------------ MAIN PART ------------ */
/* ----------------------------------- */

int main(int argc, char **argv){

	/* vars */
	char *user;

	/* check if colored output is needed */
	check_options_colored(argc, argv);

	/* check if i am root */
	user = getenv("USER");
	if (!!strcmp(user, "root")) { color_("error: root privileges needed\n", red); exit(1); }

	/* check already running instance of the program */
	if (!check_instance()) { color_("error: tomld is running already\n", red); exit(1); }

	/* check command line options */
	check_options(argc, argv);


	/* ---------------- */
	/* ----- INIT ----- */
	/* ---------------- */

	/* print version info */
	printf("tomld (tomoyo learning daemon) "); printf(ver); printf("\n");
	
	/* get kernel version in a format where 2.6.32 is 263200 */
	flag_kernel_version = kernel_version();

	/* check tomoyo status */
	check_tomoyo();

	/* check profile.conf and manager.conf files */
	check_prof();


	return 0;
}

