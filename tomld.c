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
31/07/2011 - tomld v0.39 - bugfix: name of domain was missing when printing domains without rules
                         - bugfix: don't print "restart needed" message to domains whose process is not running
                         - bugfix in domain_get()
                         - bugfix in processing log files (affects Tomoyo version 2.2)
                         - bugfix in load() checking whether domain is an exception
                         - bugfix in path_link_read() and path_link_read()
                         - bugfix in which()
                         - bugfix in check_instance() to not let more than 1 instances of tomld running together
                         - bugifx in domain_get_log() to reread domain data after i change its profile
                         - bugfix: switch domain with all its subdomains too to learning mode when managing deny logs
                         - bugfix in domain_reshape_rules_create_double(), space was inserted after parameter2 and not before
                         - bugfix in save(): save log mark only if it's not empty
                         - more tiny fixes
                         - simplify messages and code in domain creation
                         - speed up domain_get_profile()
                         - documentation fully revised in english, thanks to Andy Booth
                         - printing info about a domain with --info option can now be used simultaneously
                           while there is another runnin tomld daemon
                         - add [nodomain] tag to tdomf.conf to specify extra executables _not_ to create domain for
                         - rename [exe] tag to [extra] in tdomf.conf
                         - run check() within 2 secs if tomld is in temporary learning mode and the mod time of syslog changes
                         - change creation time to last change time in message when switching a domain to enforcing mode
                         - add possibility to use domain names with --learn option switch, so it switches back
                           only those domains to learning mode which we ask for, and the given patterns match the domain name
                           (thanks to muczy for the idea)
                         - add feature to --info option to print enforcing mode domains in different colors or with a star sing
                         - add tab as a separator beside space character in string functions
                         - don't let several instance of tomld processes with root privileges run together
                           normal privileged processes can run simultanously
                         - add --notify switch for better GUI integration and the ability to notify users through desktop
26/07/2011 - tomld v0.38 - add --log switch to redirect stderr and stdout to a log file
                         - some minor fixes
                         - change default 0.5 sec cycle to 2 sec and 10 sec check() to 30 sec to decrease load
                         - bugfix: stored empty logmark on clear()
                         - speed up load() function
                         - avoid the possibility of any race condition while terminating the program
                           that could be caused by more signals at a time
                         - prohibit to create any rule that matches any of tomld's working directory
                         - print time passed since creation date of domain when switching it to enforcing mode
                         - remove [mta] tag and use [mail] only
                         - add "/" char to the end of dirs only, and not to mail recipients
                         - replace uid check from load() to check_tomoyo()
                         - bugfix: create manager and profile config files on startup if missing
                         - add fflush to some printf functions
                         - run check() at once after requesting temporary learning mode, and not 30 sec later
                         - bugfix: don't write learn file every cycle in manual mode
                         - add [exe] tag to tdomf.conf to specify extra executables to create domain for
                         - bugfix: print info about mail recipients only if option is not --info, --remove etc.
19/07/2011 - tomld v0.37 - handle rules with "allow_execute /proc/$PID/exe" forms present in chromium browser
                         - allow temporary learning mode only for those domains that had access deny logs just now
                         - fix some warnings during compile time (thanks to Andy Booth for reporting it)
                         - update documentation with better english (thanks to Andy Booth for clarifying it)
                         - change --learn option switch to --learn-all and make --learn function for temporary learning mode
                         - add --mail option to send user a mail with the recent deny logs
                         - run whole check() once more on exit, so rules gathered since last check() won't be lost
                         - fix mem leaks
                         - store more chars in logmark from syslog to avoid accidental match because of similar entries
                         - when specifying recursive dirs, the new rules will now be based on the old rule,
                           and not the specified dirs
                         - bugfix in domain_get_rules_with_recursive_dirs()
                         - load tomld config from /etc/tomld/tomld.conf if it exists for customization
                         - add ability to wildcard functions to replace dirs with manually wildcarded ones
                         - bugfix in option handling
                         - add -- option to mark the end of option list
                         - change domain complexity from liner to exponential, so more complex domians will need more time
                         - change working directory for logmark
                         - print statistics about used cpu time, peak of virtual memory and peak of resident memory
                         - bugfix in domain_reshape_rules_create_double()
                         - check on every restart of tomld whether any of the domains still need restart and print info about it
                         - reducing load by saving config files only if their content have changed
                         - reducing load by checking system log only if its modification time has changed
                         - print more info when switching a domain back to learning mode
13/07/2011 - tomld v0.36 - fully automatic enforcing mode is ready, needs a lot of testing though
                         - add ability to accept user request for temporary learning mode for domains with deny logs
                         - empty pid file on exit
                         - fix some mem leaks
                         - runtime working directory is /var/run/tomld/ from now
                         - add a rule with a unique id and with time in seconds to every domain to mark the creation of domain
                           this is to determine from the uptime of process belonging to the domain
                           if it is restarted at least once and so entered its new domain
                           and also this is to determine if the config was created by tomld
                         - add --manual switch to effect the enforcing mode
                         - reorder option switch handling so the not so effective switches run first
                         - add process_get_cpu_time_all()
                         - add domain_check_enforcing()
                         - add domain_update_change_time()
                         - check if tomoyo support is compiled into kernel above version 2.6.36
                         - auto yes for adding denied rules in non-manual mode is disabled
                         - offer terminating already running automatic mode tomld process when running it in manual mode
                         - bugfix in file_read()
                         - some more bugfixes
                         - print start time and end time of tomld
                         - print statistics about min/avg/max times of check() cycles on exit
29/06/2011 - tomld v0.35 - add SIGQUIT to interrupt signals
                         - use second parameter for allow_create and similar only from kernel 2.6.36 and above
                         - wildcard pipe values too
                         - expand special directory list
25/06/2011 - tomld v0.34 - create allow_create rules for allow_write too
                         - wildcard random part of file name in special dirs
                         - delete domain from kernel memory too on --remove
                         - bugfix: fix a segfault in domain_info() on --info
                         - bugfix: cleanup domains at the end of every load()
                         - bugfix: fix log file parsing in domain_get_log()
                         - bugfix: fix special recursive directory wildcard usage
                           (it means at least 1 or more directories, not zero or more)
                         - bugfix: domain wasn't switched back to learning mode after adding access deny messages
                         - bugfix: fix several memory leaks
                         - bugfix in path_is_dir_recursive_wildcard() and speed up the function too
                         - bugfix in compare_paths() to take care of the second parameter too if it is not a dir
                           (since kernel 2.6.33 and above)
                         - bugfix in domain_sort_uniq_rules() to search for wildcard in parameter dir instead of whole rule
                         - speed up kernel_version()
                         - make reshape compatible with kernel 2.6.36 and above
                         - replace strspn() with my function
                         - improve path_wildcard_proc() by replacing all subdirs with numeric wildcard that consist of only numbers
12/06/2011 - tomld v0.33 - handle SIGINT and SIGTERM interrupt signals
                         - fix to view options without root privilege
                         - apply rules on the active domains of the running processes too
                         - merge collected rules from similar domains into my main domain on load
                         - fix a segfault in compare_names()
                         - bugfix in domain_sort_uniq_rules(): store only the rule with more wildcards in it,
                           or if that's equal, then the one with the shortest length from the matching ones
                         - rewrite reload() to update policy by applying a diff only to avoid a race condition
                           for security reasons
                         - bugfix: disable all other domains and delete their rules from memory too on --clear
                         - bugfix: don't make a delete line for use_profile when updating policy with diff in reload()
07/06/2011 - tomld v0.32 - first working c version of tomld
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
                         - bugfix: do not switch newly created domains to enforcing mode
                         - add compatibility to tomoyo version 2.3
05/04/2011 - tomld v0.27 - rewrite domain cleanup function
                         - speed up the new domain cleanup function by skipping rules reading libs
                         - add feature: check rules only if they changed and avoid unnecessary work
                         - improve info() function: show main domains and subdomains with different colors
                         - major bugfixes
03/04/2011 - tomld v0.26 - improve domain cleanup function
                         - improve info function
                         - bugfixes
                         - add --learn switch to switch all domains on mistake back to learning mode
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
23/03/2011 - tomld v0.17 - add --keep switch to run tomld without switching learning mode domains to enforcing mode
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
16/03/2011 - tomld v0.09 - only old domains with profile 1-2 will be switched to enforcing mode on exit
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
	3)	sleep 1 sec, 10 cycles reached?
	4)	check policy (cycle through all processes)
	5)		load policy
	6)		domain exists?
				if no, then create one
				if disabled mode, then switch to learning mode
				if learning mode, then check if enforcing mode is necessary
					(see domain_check_enforcing() procedure)
					if so, then switch to enforcing mode
				if permissive mode, do nothing
				if enforcing mode, do nothing
	7)	get recent deny logs
	8)	convert logs to rules
	9)	add rules to policy if ok
	10)	policy changed since last time?
			if not, then goto 2
	11)	reshape rules
			reshape by recursive dirs
			reshape by special collected dirs
			reshape by doubling create rules
			domain cleanup
			reload new policy to kernel
	12)	hitting q key or SIGINT / SIGTERM / SIGQUIT?
			if no, then check if 5 min passed yet?
				if so, then save policy to disk
				goto 2
			if yes, then save new policy and exit

*/

/* tab = 4 chars */


#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <time.h>
#include <sys/mount.h>
#include <signal.h>

#include "tio.h"


#define max_char	4096
#define max_num		68
#define max_proc	1000



/* ------------------------------------------ */
/* ------------ GLOBAL VARIABLES ------------ */
/* ------------------------------------------ */

/* program version */
char *ver = "0.39";

/* my unique id for version compatibility */
/* this is a remark in the policy for me to know if it's my config
 * and when the domain was created */
/* echo $(echo "tomld" | openssl md5) "$ver" | openssl md5 */
char *myuid = "allow_read /tomld/8c2537b34c2b0549b7342f5aea1db92d";
char *myuid_create = 0;
char *myuid_change = 0;
char *myuid_cputime = 0;

/* home dir */
char *home = "/home";


/* interval of policy check to run in seconds */
float const_time_check = 30;
/* interval of saving configs to disk in seconds */
float const_time_save = 300;
/* interval of maximum time in seconds to wait in temporary learning mode for domains with deny logs
 * (1 hour) */
float const_time_max_learn = 60 * 60;
/* interval of maximum time in seconds that needs to pass since last domain change for tomld
 * to automatically switch domain to enforcing mode, otherwise it calculates it
 * to make a decision (2 weeks) */
int const_time_max_change = 60 * 60 * 24 * 14;
/* constant for minimum cputime needed with timeout to switch domain to enforcing mode */
int const_min_cputime = 100;
/* constant for measuring the complexity of a domain
 * i raise the number of rules of the domain to the power of 2 and
 * multiply it by this factor and compare it to its processes' cpu time */
int const_domain_complexity_factor = 2;


/* path to my executable */
char *my_exe_path = 0;

/* number of max entries in profile config */
#define max_mem "10000"

/* tomoyo kernel parameter */
char *tkern = "security=tomoyo";

/* pattern string for the recursive wildcard "\{\*\}" for compares */
/* this means at least one or more dirs plus file at the end */
char *wildcard_recursive_plain = "\\{\\*\\}";
char *wildcard_recursive_dir   = "\\{\\*\\}/";
char *wildcard_recursive_dir2  = "/\\{\\*\\}/";
int   wildcard_recursive_dir2_len = 8;
char *wildcard_recursive_file  = "\\{\\*\\}/\\*";
char *wildcard_recursive_file2 = "/\\{\\*\\}/\\*";
int   wildcard_recursive_file2_len = 10;

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
char *tdomf_bak  = 0;
char *tdomf_bak2 = 0;
char *tdomf_bak3 = 0;
char *texcf_bak3 = 0;
char *tmarkf_bak3 = 0;

/* system log */
char *tlog	= "/var/log/syslog";
char *tlogf = 0;
int tlogf_mod_time = -1;
int tlearnf_mod_time = -1;

/* tomld paths */
char *tomld_path = 0;
/* kernel time of the last line of system log to identify the end of tomoyo logs
 * and make sure not to read it twice */
char *tmark	= "/var/local/tomld/tomld.logmark";
char *tmarkf = 0;
/* tomld pid file */
char *tpid	= "/var/run/tomld/tomld.pid";
int flag_pid = 0;
/* file to signal user request for learning mode for already running tomld daemon */
char *tlearn = "/var/run/tomld/tomld.learn";
/* file to give a list to already running tomld daemon about required domains
 * to switch them to learning mode, but only these ones */
char *tlearn_list = "/var/run/tomld/tomld.learn.list";
/* contains the last info messages and it always gets overwritten with the next message */
char *tlog2      = "/var/run/tomld/tomld.message";
/* lock file for the former one */
char *tlog2_lock = "/var/run/tomld/tomld.message.lock";
int tlog2_mod_time = -1;
/* flags to signal temporary learning mode
 * flag_learn is to signal if there was a deny log already since user requested temporary learning mode
 * flag_learn2 is to signal if the 1 hour interval of temporary learning mode is still on or ended already
 * flag_learn3 is to run check() at once, but only once */
int flag_learn  = 0;
int flag_learn2 = 0;
int flag_learn3 = 0;
/* buffer to hold the names of domains temporarily switch to learning mode */
char *tprogs_learn_auto = 0;


/* options */
int opt_dashdash	= 0;
int opt_version		= 0;
int opt_help		= 0;
int opt_color		= 0;
int opt_learn		= 0;
int opt_learn_all	= 0;
int opt_manual		= 0;
int opt_clear		= 0;
int opt_reset		= 0;
int opt_info		= 0;
int opt_remove		= 0;
int opt_yes			= 0;
int opt_keep		= 0;
int opt_recursive	= 0;
int opt_once		= 0;
int opt_mail		= 0;
int opt_log			= 0;
int opt_nodomain	= 0;
int opt_notify		= 0;

char *opt_notify2	= 0;
char *opt_learn2	= 0;
char *opt_nodomain2	= 0;
char *opt_log2		= 0;
char *opt_info2		= 0;
char *opt_remove2	= 0;
char *dirs_recursive = 0;
int  *dirs_recursive_depth = 0;
int  *dirs_recursive_sub = 0;

int flag_reset		= 0;
int flag_check		= 0;
int flag_check2		= 0;
int flag_firstrun	= 1;
int flag_safe		= 0;
int flag_new_proc	= 0;

/* time variables for statistics of check cycle time */
float time_max_cycle = 0;
float time_min_cycle = 60 * 60 * 24 * 365;
float time_avg_cycle = 0;
int   time_avg_cycle_counter = 0;


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

/* run time */
float t_start;

/* contains domain names with their cpu times and pids (pid process_binary time) for the case
 * when a domain enters learning mode from enforcing mode for user request
 * and i need to calculate process cpu time compared to the moment when tomld started,
 * so i everytime substract all running processes' cputimes in this list from the summary later */
char *domain_cputime_list_minus = 0;
/* contains domain names with their cpu times and pids (pid process_binary time)
 * so i can count the formerly terminated processes' cpu times too,
 * this is a runtime list and i save it to domain policy only from time to time */
char *domain_cputime_list_plus  = 0;
/* contains domain names with their cpu times,
 * this is a runtime list that i use to print domain status, but only if it has changed */
char *domain_cputime_list_current = 0;

/* mail */
char *mail_mta  = "/usr/sbin/sendmail";
char *mail_users = 0;

/* arrays */
char *tprogs = 0;
char *tprogs_exc = 0;
char *tprogs_exc_manual[] = {"/sbin/init", "/usr/sbin/sshd", 0};
char *tprogs_learn = 0;

char *netf[] = {"/proc/net/tcp", "/proc/net/tcp6", "/proc/net/udp", "/proc/net/udp6", 0};

char *tshell = "/etc/shells";
char *tshellf[] = {"/bin/bash", "/bin/csh", "/bin/dash", "/bin/ksh", "/bin/rbash", "/bin/sh",
"/bin/tcsh", "/usr/bin/es", "/usr/bin/esh", "/usr/bin/fish", "/usr/bin/ksh", "/usr/bin/rc",
"/usr/bin/screen", 0};
char *tshellf2 = 0;

char *tspec = "/etc/tomld/tomld.conf";
char *tspecf = 0;
char *spec_exception[] = {"/", "/dev/", "/etc/", "/home/\\*/", "/root/", 0};
char *spec_wildcard[] = {"/home/\\{\\*\\}/", "/usr/share/\\{\\*\\}/", "/etc/fonts/\\{\\*\\}/",
                "/var/cache/\\{\\*\\}/", "/dev/pts/", 0};
char *spec_replace[] = {"/var/run/gdm/auth-for-\\*/", 0};
char *spec_exception2 = 0;
char *spec_wildcard2 = 0;
char *spec_replace2 = 0;


/* ----------------------------------- */
/* ------------ FUNCTIONS ------------ */
/* ----------------------------------- */


void clear();

void domain_cleanup();

void domain_update_cpu_time_all();


/* print version info */
void version() {
	printf ("tomld (tomoyo learning daemon) %s\n", ver);
	printf ("Copyright (C) 2011 Andras Horvath\n");
	printf ("E-mail: mail@log69.com - suggestions & feedback are welcome\n");
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
	printf ("Tomld (tomoyo learning daemon) is an extension to the Tomoyo security framework. ");
	printf ("Tomoyo increases security by confining applications and services into domains using rules. ");
	printf ("Tomld automates this process helping users harden their systems more easily. ");
	printf ("To do this tomld starts in learning mode, creates Tomoyo domains, collects rules, ");
	printf ("changes them and once the rules appear to be complete tomld enforces the policy.\n");
	printf ("\n");
	printf ("The Tomoyo framework already has a learning mode which automatically sets up rules while ");
	printf ("processes are running. The problem for the user is that these processes use continuously ");
	printf ("changing files and directory names which have to be replaced by wildcarded rules. ");
	printf ("This requires an advanced knowledge of the system.\n");
	printf ("\n");
	printf ("Tomld fully automates the MAC set up, allowing the average user to have ");
	printf ("a much more secure system with minimal effort. ");
	printf ("Currently this solution targets Linux systems only.\n");
	printf ("\n");
	printf ("Tomoyo security module is part of the mainline Linux kernel since version 2.6.30.\n");
	printf ("\n");
	printf ("*TOMOYO(r) is a registered trademark of NTT DATA Corporation\n");
	printf ("\n");
	printf ("LINKS:\n");
	printf ("http://log69.com\n");
	printf ("http://tomoyo.sourceforge.jp\n");
	printf ("http://en.wikipedia.org/wiki/Mandatory_access_control\n");
	printf ("\n");
	printf ("USAGE: tomld [options] [executables]\n");
	printf ("\n");
	printf ("The following options are supported:\n");
	printf ("    -h   --help              **print this help\n");
	printf ("    -v   --version           **print version information\n");
	printf ("    -c   --color             colorize output\n");
	printf ("    -n   --notify  [command] run the command with appended info messages\n");
	printf ("                             (should be run on a desktop as normal user)\n");
	printf ("         --log        [file] redirect stderr and stdout to this file\n");
	printf ("         --no-domain [files] don't create domains for these executables\n");
	printf ("         --clear             clear domain configurations\n");
	printf ("                             (all previously learnt rules will be backed up)\n");
	printf ("         --reset             reinitialize domain configurations\n");
	printf ("                             (all previously learnt rules will be backed up)\n");
	printf ("    -l   --learn  [patterns] **request temporary learning mode for all domains,\n");
	printf ("                             or for those domains that match the patterns\n");
	printf ("                             (this is the recommended way if still necessary)\n");
	printf ("    -i   --info    [pattern] **print domains' rules by pattern\n");
	printf ("                             (without pattern, print a list of main domains)\n");
	printf ("    -r   --remove  [pattern] remove domains by pattern\n");
	printf ("    -R   --recursive  [dirs] replace subdirs of dirs with wildcards in rules\n");
	printf ("    -m   --manual            exiting from tomld for the second time switches\n");
	printf ("                             all old learning mode domains to enforcing mode\n");
	printf ("    -k   --keep              don't change domain's mode for this session\n");
	printf ("                             (learning mode domains will stay so on exit)\n");
	printf ("         --learn-all         switch all domains back to learning mode\n");
	printf ("                             (this is not advised, only for correction purposes)\n");
	printf ("         --mail      [users] send mail to users with recent deny logs\n");
	printf ("    -1   --once              quit after first cycle\n");
	printf ("         --yes               auto confirm everything with yes\n");
	printf ("\n");
	printf ("*executables are additional programs to create domains for\n");
	printf ("\n");
	printf ("**these options can be used simultaneously with a running tomld daemon\n");
	printf ("\n");
	printf ("REQUIREMENTS:\n");
	printf ("Tomoyo enabled kernel (v2.6.30 and above) and tomoyo-tools (v2.2 and above)\n");
	printf ("\n");
	printf ("For full documentation and FAQ, see the website: http://log69.com\n");
	printf ("\n");
}


/* free my pointers */
void myfree()
{
	free2(my_exe_path);
	free2(tdomf);
	free2(texcf);
	free2(tshellf2);
	free2(tprogs);
	free2(tprogs_exc);
	free2(tprogs_learn);
	free2(dirs_recursive);
	if (dirs_recursive_depth)	free(dirs_recursive_depth);
	if (dirs_recursive_sub)		free(dirs_recursive_sub);
	free2(opt_notify2);
	free2(opt_learn2);
	free2(opt_nodomain2);
	free2(opt_log2);
	free2(opt_info2);
	free2(opt_remove2);
	free2(tmarkf);
	free2(tlogf);
	free2(tdomf_bak);
	free2(tdomf_bak2);
	free2(tdomf_bak3);
	free2(texcf_bak3);
	free2(tmarkf_bak3);
	free2(myuid_create);
	free2(myuid_change);
	free2(myuid_cputime);
	free2(domain_cputime_list_minus);
	free2(domain_cputime_list_plus);
	free2(domain_cputime_list_current);
	free2(spec_exception2);
	free2(spec_wildcard2);
	free2(spec_replace2);
	free2(mail_users);
	free2(tomld_path);
}


/* my exit point */
void myexit(int num)
{
	/* empty pid file, but only after options check */
	if (flag_pid && file_exist(tpid)) file_write(tpid, 0);
	/* free my pointers */
	myfree();
	exit(num);
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


/* check for notification messages stored in tlog2 file
 * and send them to a notification daemon if any */
void check_notify()
{
	/* run client side only if i am non-root */
	if (getuid()){
		if (opt_notify){
			static int flag_firstrun_notify = 0;
			int tlog2_mod_time2;
			char *res, *res2;

			if (!opt_notify2){
				error("error: commands for notification missing\n"); myexit(1); }

			color("*listening to messages from tomld daemom...\n", yellow);

			/* infinite cycle */
			while(1){
				/* print my welcome message on first run */
				if (!flag_firstrun_notify){
					char *comm, *temp;
					flag_firstrun_notify = 1;
					
					/* store command */
					temp = opt_notify2;
					comm = string_get_next_line(&temp);
					/* add message */
					strcat2(&comm, " \"Tomld notification service started\"");
					/* run command */
					system(comm);
					free2(comm);

					/* store modification date to not read the current value, but only from now on */
					tlog2_mod_time = file_get_mod_time(tlog2);
				}
				/* check modification time of log2 file and read its content only if modified */
				if (file_exist(tlog2)){
					tlog2_mod_time2 = file_get_mod_time(tlog2);
					if (tlog2_mod_time != tlog2_mod_time2){
						/* store new time */
						tlog2_mod_time = tlog2_mod_time2;
						
						/* is log2 file locked? if yes, then reread it 2 sec later */
						while(1){
							res = 0;
							if (file_exist(tlog2_lock)){
								res = file_read(tlog2_lock, 1); }
							if (!strlen2(&res)){
									
								/* read message */
								res2 = file_read(tlog2, 0);
								if (strlen2(&res2)){
									/* store command */
									char *temp = opt_notify2;
									char *comm = string_get_next_line(&temp);
									color(res2, clr);
									/* add message */
									strcat2(&comm, " \"");
									strcat2(&comm, res2);
									strcat2(&comm, "\"");
									free2(res2);

									/* run command */
									system(comm);
									free2(comm);
									
									free2(res);
									break;
								}
							}
							free2(res);

							/* sleep 2 sec */
							usleep(2000000);
						}
					}
				}
				/* sleep 2 sec */
				usleep(2000000);
			}
		}
	}
}


/* store messages for notify */
void notify(char *text)
{
	if (opt_notify){
		/* lock file */
		file_write(tlog2_lock, "1");

		/* write message */
		file_write(tlog2, text);

		/* unlock file */
		file_write(tlog2_lock, 0);
	}
}


/* give a choice */
int choice(const char *text)
{
	char c = 0, c2;
	
	/* is auto yes on? */
	if (opt_yes){
		printf("%s", text);
		printf(" (y)\n");
		fflush(stdout);
	}
	else{
		/* ask question and wait for user input */
		printf("%s", text);
		printf(" [y/N]");
		fflush(stdout);
		c = getchar();
		if (c == '\n') return 0;
		while((c2 = getchar()) != '\n');
		if (c == 'y' || c == 'Y') return 1;
		return 0;
	}
	return 1;
}


/* print sand clock */
void sand_clock(int dot)
{
	static int flag_clock = 0;

	/* i print these only in manual mode, so it won't fill the log in automatic mode */
	if (opt_manual){
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
}


/* get kernel version in a format where 2.6.33 will be 263300 */
int kernel_version()
{
	char *v = "/proc/sys/kernel/osrelease";
	char *buff;
	int c, c2;
	char n;
	static int ver = 0;

	if (ver) return ver;

	/* read kernel version from /proc in a format as 2.6.32-5-amd64 */
	buff = file_read(v, -1);
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
	buff = file_read(tverk, -1);

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


/* return true if path ends with recursive wildcard (dir or file) */
int path_is_dir_recursive_wildcard(const char *path)
{
	long l1;
	
	if (!path) return 0;

	/* get path length */
	l1 = strlen(path);

	/* compare wildcard dir to the end of paths */
	/* wildcard file's length is longer than dir's, so i compare file first */
	if (wildcard_recursive_file2_len > l1) return 0;
	if (!strcmp(wildcard_recursive_file2, path + l1 - wildcard_recursive_file2_len)) return 1;

	/* compare wildcard file to the end of paths */
	if (wildcard_recursive_dir2_len > l1) return 0;
	if (!strcmp(wildcard_recursive_dir2, path + l1 - wildcard_recursive_dir2_len)) return 1;

	return 0;
}


/* wildcard the leaf dir or file of a path and return the result */
/* returned value must be freed by caller */
char *path_wildcard_dir(char *path)
{
	char *new = 0;

	/* if dir conatins recursive wildcard at the end, then return it as a result */	
	if (path_is_dir_recursive_wildcard(path)){
		strcpy2(&new, path);
		return new;
	}

	/* create wildcarded path */
	new = path_get_parent_dir(path);
	if (path_is_dir(path)) strcat2(&new, "\\*/");
	else                   strcat2(&new, "\\*");
	
	return new;
}


/* wildcard the leaf dir or file of a path and its parent dir too and return the result */
/* returned value must be freed by caller */
char *path_wildcard_dir_plus_parent(char *path)
{
	char *new, *new2;
	
	int flag_dir = path_is_dir(path);
	int flag_rec = path_is_dir_recursive_wildcard(path);

	/* create wildcarded path */	
	new  = path_get_parent_dir(path);
	new2 = path_get_parent_dir(new);
	free2(new); new = new2;

	/* check if result is null, if so, then add "/" char */
	if (!strlen2(&new)){
		strcat2(&new, "/");

		if (flag_rec){
			if (flag_dir) strcat2(&new, wildcard_recursive_dir);
			else strcat2(&new, wildcard_recursive_file);
			return new;
		}
		else{
			if (flag_dir){
				strcat2(&new, "\\*/");
			}
			else{
				strcat2(&new, "\\*");
			}
		}
	}
	else{
		if (flag_rec){
			if (flag_dir) strcat2(&new, wildcard_recursive_dir);
			/* when path ends with a recursive wildcard dir and with a wildcarded file "\\*",
			 * then path stays the same after this,
			 * because here the file and its parent is wildcarded already too */
			else strcat2(&new, wildcard_recursive_file);
			return new;
		}
		else{
			if (flag_dir){
				strcat2(&new, "\\*/\\*/");
			}
			else{
				strcat2(&new, "\\*/\\*");
			}
		}
	}
	
	return new;
}


/* wildcard library files' version numbers */
/* returned value must be freed by caller */
char *path_wildcard_lib(char *path)
{
	char *name, *new = 0;
	char c, c2;
	int i;
	
	name = path_get_filename(path);
	/* does the filename start with "lib"? */
	if (string_search_keyword_first(name, "lib")){
		/* search for ".so" extension */
		i = string_search_keyword(name, ".so");
		/* found anything? */
		if (i > -1){
			/* is EOF or "." after ".so"? */
			c = name[i + 3];
			if (c == 0 || c == '.'){
				char *end = 0;
				int flag = 0;
				/* is there a dot after ".so"? if so, then store the end part */
				if (c) strcpy2(&end, name + i + 3);
				while(i > 0){
					c2 = name[--i];
					if ((c2 > '9' || c2 < '0') && (c2 != '.')) break;
					if (c2 != '.') flag = 1;
				}
				i++;
				/* are there any numbers too between lib name and ".so"? */
				if (flag){
					/* add wildcard and ".so" too */
					name[i] = 0;
					strlenset2(&name);
					strcat2(&name, "\\*.so");
				}
				else{
					name[i + 3] = 0;
					strlenset2(&name);
				}
				/* were there a dot after ".so"? */
				if (end){
					/* search for numbers in the end part */
					flag = 0;
					i = 0;
					while(1){
						c2 = end[i++];
						if (!c2) break;
						if (c2 >= '0' && c2 <= '9'){ flag = 1; break; }
					}
					/* does the end part contain any numbers? */
					/* if so, then add wildcard to the end part too */
					if (flag) strcat2(&name, ".\\*");
					/* if not, then add original end part */
					else      strcat2(&name, end);
					free2(end);
				}
				/* add filename to base dir for result */
				new = path_get_parent_dir(path);
				strcat2(&new, name);
			}
		}
	}

	/* give back original path if no wildcard is added */
	if (!new) strcpy2(&new, path);

	free2(name);
	return new;
}


/* change /proc/$PID/exe to /proc/PID/exe (for aggregator), and
 * the rest of /proc/$PID/ to /proc/\$/ wildcard
 * and replace subdir that consist of only numbers with numeric wildcard */
/* returned value must be freed by caller */
char *path_wildcard_proc(char *path)
{
	char *new = 0, *temp;
	char c;
	int i, i2, flag;
	long l;
	
	if (!path) return 0;
	
	/* does the path start with "/proc/"? */
	if (string_search_keyword_first(path, "/proc/")){
		
		l = strlen(path);
		temp = memget2(l * 2);
		new  = memget2(l * 2);
		strcpy2(&new, "/proc/");
		
		/* replace all subdirs that consist of only numbers with numeric wildcard */
		flag = 1;
		i = 6;
		i2 = 0;
		while(1){
			c = path[i];
			if (!c || c == '/'){
				if (!flag){
					if (i2) strcat2(&new, temp);
				}
				else{
					if (i2) strcat2(&new, "\\$");
				}
				if (i2 && c) strcat2(&new, "/");
				i2 = 0;
				temp[0] = 0;
				flag = 1;
			}
			else{
				temp[i2++] = c; temp[i2] = 0;
				if (c < '0' || c > '9') flag = 0;
			}
			if (!c) break;
			i++;
		}
		
		free2(temp);
	}
	
	/* give back original path if no wildcard is added */
	if (!new) strcpy2(&new, path);

	return new;
}


/* wildcard /home/user/ names */
/* returned value must be freed by caller */
char *path_wildcard_home(char *path)
{
	char *h = 0, *new = 0;
	char c;
	int i;
	
	/* copy home dir */
	strcpy2(&h, home);
	strcat2(&h, "/");
	
	/* does the path start with "/home/"? */
	if (string_search_keyword_first(path, h)){
		i = strlen2(&h);
		/* is there a second subdir? (if next char isn't null) */
		if (path[i]){
			/* jump to end of second subdir */
			while(1){
				c = path[i];
				if (!c || c == '/') break;
				i++;
			}

			/* replace second subdir with numeric wildcard */
			strcpy2(&new, h);
			strcat2(&new, "\\*");
			strcat2(&new, path + i);
		}
	}
	
	/* give back original path if no wildcard is added */
	if (!new) strcpy2(&new, path);

	free2(h);

	return new;
}


/* wildcard random part of file name */
/* i consider random part in a file name if it contains only lower and upper case, and numbers */
/* if this condition doesn't meet, then i return the original name,
 * cause probably new random name will come later anyway */
/* returned value must be freed by caller */
char *path_wildcard_dir_temp_name(char *name)
{
	char *new = 0;
	char *temp = 0;
	char c;
	long l;
	int i, i2;
	int flag_now, flag_lcase, flag_ucase, flag_num, flag_count;
	
	/* return null on null input */
	if (!name) return 0;

	/* check length */
	l = strlen(name);
	if (!l) return 0;
	
	/* alloc mem for temporary name */
	temp = memget2(l * 2);
	
	/* search for random part in name */
	flag_lcase = 0;
	flag_ucase = 0;
	flag_num   = 0;
	flag_count = 0;
	i = 0;
	i2 = 0;
	while(1){
		c = name[i++];
		if (!c) break;
		/* is char alfanumeric char? */
		flag_now = 0;
		if (c >= '0' && c <= '9'){ flag_now = 1; flag_num   = 1; }
		if (c >= 'a' && c <= 'z'){ flag_now = 1; flag_lcase = 1; }
		if (c >= 'A' && c <= 'Z'){ flag_now = 1;
			/* upper case must be exist from the 2nd char */
			if (flag_count >= 1) flag_ucase = 1;
		}
		if (flag_now){
			if (!flag_count){
				strcat2(&new, temp);
				/* reset count */
				i2 = 0;
			}
			/* increase random char count, must be at least 6 or more to success */
			flag_count++;
			/* add char to result */
			temp[i2++] = c; temp[i2] = 0;
		}
		else{
			if (flag_count){
				/* make a wildcard if random part was more or equal than 6 chars */
				if (flag_count >= 6 && flag_lcase && flag_ucase && flag_num){
					strcat2(&new, "\\*");
				}
				/* store original sample if less than 6 chars */
				else{
					strcat2(&new, temp);
				}
				/* reset counts */
				flag_lcase = 0;
				flag_ucase = 0;
				flag_num   = 0;
				flag_count = 0;
				i2 = 0;
			}
			/* add char to result */
			temp[i2++] = c; temp[i2] = 0;
		}
	}

	/* run check once more after quitting cycle */
	
	/* make a wildcard if random part was more or equal than 6 chars */
	if (flag_count >= 6 && flag_lcase && flag_ucase && flag_num){
		strcat2(&new, "\\*");
	}
	/* store original sample if less than 6 chars */
	else{
		strcat2(&new, temp);
	}
	
	free2(temp);
	
	return new;
}


/* wildcard temporary (random type) file name in path */
/* returned value must be freed by caller */
char *path_wildcard_dir_temp(char *path)
{
	char *new = 0;
	
	/* is path a file name? */
	if (!path_is_dir(path)){
		char *mydir   = path_get_parent_dir(path);
		char *myname  = path_get_filename(path);
		char *myname2 = path_wildcard_dir_temp_name(myname);
		
		strcpy2(&new, mydir);
		strcat2(&new, myname2);

		free2(mydir);
		free2(myname);
		free2(myname2);
	}
	else strcpy2(&new, path);
	
	return new;
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
	char c;

	if (!(*text)) return 0;
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

	/* remove extra empty lines from the end */
	while(1){
		if (!end) break;
		end--;
		c = text2[end];
		if (c && c != '\n'){
			end += 2;
			break;
		}
	}
	
	/* domain block length */
	l = keyl + end;

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
char *domain_get_name(char *domain)
{
	char *res, *res2, *temp, *temp2;
	
	/* get first line containing the name of the domain */
	temp = domain;
	res = string_get_next_line(&temp);
	if (!res) return 0;
	/* get second word for domain name */
	temp2 = res;
	res2 = string_get_next_wordn(&temp2, 1);
	free2(res);
	return res2;
}


/* return a new string containing the full name of the domain from the domain policy */
/* returned value must be freed by caller */
char *domain_get_name_full(char *domain)
{
	char *res, *res2, *temp, *temp2;
	
	/* get first line containing the name of the domain */
	temp = domain;
	res = string_get_next_line(&temp);
	if (!res) return 0;
	/* get everything except first word for domain name */
	temp2 = res;
	res2 = string_get_next_word(&temp2); free2(res2);
	res2 = string_get_next_line(&temp2);
	free2(res);
	return res2;
}


/* return a new string containing the name of the subdomain from the domain policy */
/* returned value must be freed by caller */
char *domain_get_name_sub(char *domain)
{
	char *res, *res2, *temp, *temp2;
	
	/* get first line containing the name of the domain */
	temp = domain;
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


/* return a new string containing the main domain of that name
 * or return null if no main domain exists like that */
char *domain_get(char *prog)
{
	char *res, *res2, *temp;

	temp = tdomf;
	while(1){
		/* get first line containing the name of the domain */
		res = domain_get_next(&temp);
		if (!res) break;
		/* get main domain name */
		res2 = domain_get_name_sub(res);
		if (res2){
			/* if prog names matches main domain name, then success */
			if (!strcmp(prog, res2)){
				free2(res2);
				return res;
			}
			free2(res2);
		}
		free2(res);
	}
	return 0;
}


/* return creation time of domain in seconds (how long ago it was created),
 * or -1 if it doesn't exist */
int domain_get_creation_time(char *prog)
{
	char *temp;
	int d_create = -1;
	
	char *domain = domain_get(prog);
	if (domain){
		int i = string_search_keyword(domain, myuid_create);
		if (i > -1){
			char *res, *res2;
			temp = domain + i;
			/* get my unique id with the creation time in it */
			res = string_get_next_line(&temp);
			/* get epoch time from my uid */
			res2 = string_get_number_last(res);
			/* convert epoch string to integer */
			d_create = time(0) - atoi(res2);
			free2(res); free2(res2);
		}
		free2(domain);
	}
	
	return d_create;
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

	/* sort and unique list because there many same entries because of subdomains */
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
	
	/* search for the keyword */
	i = string_search_keyword(text, key);
	
	/* if match */
	if (i > -1){
		/* former char should be a newline */
		if (i < 1) return -1;
		if (text[i - 1] != '\n') return -1;
		/* get numer */
		i += keyl;
		p = string_ctoi(text[i]);
		return p;
	}
	
	return -1;
}


/* set profile number (0-3) of a domain at text position from the beginning of domain policy */
void domain_set_profile(char **text, int profile)
{
	char p;
	char *key = "use_profile ";
	int keyl = 12;
	char *res, *orig, *temp;
	
	temp = *text;
	while(1){
		orig = temp;

		/* get next line */
		res = string_get_next_line(&temp);
		if (!res) break;

		/* search for the keyword */
		if (string_search_keyword_first(res, key)){
			/* set profile */
			p = string_itoc(profile);
			orig[keyl] = p;
			free2(res);
			break;
		}
		free2(res);
	}
}


/* set profile number (0-3) of domain and all its subdomains in domain policy */
void domain_set_profile_for_prog(const char *prog, int profile)
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
		if (res2){

			/* if so, then set its profile mode */
			if (!strcmp(prog, res2)) domain_set_profile(&orig, profile);
		}

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


/* return an integer containing the pid of the path of executable that is running */
/* return null if no process like that is running */
int process_get_pid(const char *name)
{
	DIR *mydir;
	struct dirent *mydir_entry;
	char *mydir_name = 0, *mypid = 0;
	int p = 0;
	
	/* open /proc dir */
	mydir = opendir("/proc/");
	if (!mydir){ error("error: cannot open /proc/ directory\n"); myexit(1); }

	/* cycle through dirs in /proc */
	while((mydir_entry = readdir(mydir))) {
		/* get the dir names inside /proc dir */
		strcpy2(&mypid, mydir_entry->d_name);

		/* does it contain only numbers meaning they are pids? */
		if (string_is_number(mypid)) {
			char *res;
			/* create dirname like /proc/pid/exe */
			strcpy2(&mydir_name, "/proc/");
			strcat2(&mydir_name, mypid);
			strcat2(&mydir_name, "/exe");

			/* resolv the link pointing from the exe name */
			res = path_link_read(mydir_name);
			if (res){
				/* compare the link to the process name */
				if (!strcmp(res, name)) {
					closedir(mydir);
					free2(res);
					free2(mydir_name);

					/* convert pid string to integer */
					p = atoi(mypid);
					free2(mypid);
					return p;
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


/* return an integer list with an ending zero containing all pids of the same executables
 * that are running currently */
/* return null if no process like that is running */
/* returned value must be freed by caller */
int *process_get_pid_list(const char *name)
{
	long list_length = max_proc;
	int list_counter = 0;
	int *list;
	DIR *mydir;
	struct dirent *mydir_entry;
	char *mydir_name = 0, *mypid = 0;
	int p = 0;
	
	/* open /proc dir */
	mydir = opendir("/proc/");
	if (!mydir){ error("error: cannot open /proc/ directory\n"); myexit(1); }

	/* alloc mem for list */
	list = memget_int(list_length);
	
	/* cycle through dirs in /proc */
	while((mydir_entry = readdir(mydir))) {
		/* get the dir names inside /proc dir */
		strcpy2(&mypid, mydir_entry->d_name);

		/* does it contain only numbers meaning they are pids? */
		if (string_is_number(mypid)) {
			char *res;
			/* create dirname like /proc/pid/exe */
			strcpy2(&mydir_name, "/proc/");
			strcat2(&mydir_name, mypid);
			strcat2(&mydir_name, "/exe");

			/* resolv the link pointing from the exe name */
			res = path_link_read(mydir_name);
			if (res){
				/* compare the link to the process name */
				if (!strcmp(res, name)) {
					/* convert pid string to integer */
					p = atoi(mypid);
					/* store pid in my list */
					list[list_counter++] = p;
					
					/* check array boundaries */
					if (list_counter >= list_length){
						error("error: list array too small in process_get_pid_list()\n"); myexit(1); }
				}
				free2(res);
			}
		}
	}
	closedir(mydir);
	free2(mydir_name);
	free2(mypid);

	/* if no process was found, then free list and fail */
	if (!list_counter){ free(list); return 0; }
	
	/* put an ending zero to the end of list */
	list[list_counter] = 0;

	return list;
}


/* return an integer containing the least uptime of the same running processes */
/* return null if no process is running like that */
int process_get_least_uptime(const char *name)
{
	DIR *mydir;
	struct dirent *mydir_entry;
	char *mydir_name = 0, *mypid = 0;
	/* my uptime value to compare to and find the least one */
	int myuptime = 0;
	int myuptime_flag_first = 0;
	/* get system uptime */
	int s_uptime = sys_get_uptime();
	/* converting jiffies to second, this is from manpage of proc */
	int jiffies_per_second=sysconf(_SC_CLK_TCK);
	
	/* open /proc dir */
	mydir = opendir("/proc/");
	if (!mydir){ error("error: cannot open /proc/ directory\n"); myexit(1); }

	/* cycle through dirs in /proc */
	while((mydir_entry = readdir(mydir))) {
		/* get the dir names inside /proc dir */
		strcpy2(&mypid, mydir_entry->d_name);

		/* does it contain only numbers meaning they are pids? */
		if (string_is_number(mypid)) {
			char *res;
			/* create dirname like /proc/pid/exe */
			strcpy2(&mydir_name, "/proc/");
			strcat2(&mydir_name, mypid);
			strcat2(&mydir_name, "/exe");

			/* resolv the link pointing from the exe name */
			res = path_link_read(mydir_name);
			if (res){
				/* compare the link to the process name */
				if (!strcmp(res, name)) {
					char *pstat, *temp, *putime;
					int t;
					
					/* create dirname like /proc/pid/stat */
					strcpy2(&mydir_name, "/proc/");
					strcat2(&mydir_name, mypid);
					strcat2(&mydir_name, "/stat");
					/* read process stat value */
					pstat = file_read(mydir_name, -1);
					temp = pstat;
					/* get starttime of process in jiffies */
					putime = string_get_next_wordn(&temp, 21);
					/* calculate process uptime in sec */
					t = s_uptime - atoi(putime) / jiffies_per_second;
					/* store the least value */
					if (!myuptime_flag_first){
						myuptime_flag_first = 1;
						myuptime = t;
					}
					else{
						if (myuptime > t) myuptime = t;
					}
					free2(putime);
					free2(pstat);
				}
				free2(res);
			}
		}
	}
	closedir(mydir);

	free2(mydir_name);
	free2(mypid);

	return myuptime;
}


/* return an integer containing the sum of cpu time of all of the same running processes in clock ticks
 * it doesn't add the cpu times of the running processes at the start time of tomld
 * the flag_clear option decides, whether i reset all processes' cpu time values of the same name,
 * or just read it out */
/* return null if no process is running like that */
int process_get_cpu_time_all(const char *name, int flag_clear)
{
	DIR *mydir;
	struct dirent *mydir_entry;
	char *mydir_name = 0, *mypid = 0, *list_entry = 0;
	char *mypid2 = 0, *name2 = 0;
	char *pstat, *ptime, *res2, *res3, *temp, *temp2;
	int t, t2;
	int i, i2;
	/* my uptime value to compare to and find the least one */
	int mycputime = 0;


	if (!name) return 0;

	/* open /proc dir */
	mydir = opendir("/proc/");
	if (!mydir){ error("error: cannot open /proc/ directory\n"); myexit(1); }

	/* cycle through dirs in /proc */
	while((mydir_entry = readdir(mydir))) {
		/* get the dir names inside /proc dir */
		strcpy2(&mypid, mydir_entry->d_name);

		/* does it contain only numbers meaning they are pids? */
		if (string_is_number(mypid)) {
			char *res;
			/* create dirname like /proc/pid/exe */
			strcpy2(&mydir_name, "/proc/");
			strcat2(&mydir_name, mypid);
			strcat2(&mydir_name, "/exe");

			/* resolv the link pointing from the exe name */
			res = path_link_read(mydir_name);
			if (res){
				/* compare the link to the process name */
				if (!strcmp(res, name)) {
					
					/* create dirname like /proc/pid/stat */
					strcpy2(&mydir_name, "/proc/");
					strcat2(&mydir_name, mypid);
					strcat2(&mydir_name, "/stat");
					/* read process stat value */
					pstat = file_read(mydir_name, -1);
					/* get all utime, stime, cutime and cstime of process in ticks */
					t = 0;
					temp = pstat;
					ptime = string_get_next_wordn(&temp, 13);
					t += atoi(ptime);
					free2(ptime);
					temp = pstat;
					ptime = string_get_next_wordn(&temp, 14);
					t += atoi(ptime);
					free2(ptime);
					temp = pstat;
					ptime = string_get_next_wordn(&temp, 15);
					t += atoi(ptime);
					free2(ptime);
					temp = pstat;
					ptime = string_get_next_wordn(&temp, 16);
					t += atoi(ptime);
					free2(ptime);
					

					/* *************************************************
					 * manage domain cpu time list minus / plus entries
					 * this list grows with time as new processes appear with new pids,
					 * but this should stay at a low level
					 * *************************************************/
					 
					/* convert cputime integer to string */
					ptime = string_itos(t);
					/* create list entries for compare */
					strcpy2(&list_entry, mypid);
					strcat2(&list_entry, " ");
					strcat2(&list_entry, name);
					strcat2(&list_entry, " ");
					strcat2(&list_entry, ptime);
					strcpy2(&mypid2, mypid);
					strcat2(&mypid2, " ");
					strcpy2(&name2, mypid2);
					strcat2(&name2, name);
					strcat2(&name2, " ");

					/* domain cpu time list minus entry exists? */
					i = string_search_keyword_first_all(domain_cputime_list_minus, name2);
					if (i > -1){
						if (flag_clear){
							/* update it by removing old entry and adding current one */
							res2 = string_remove_line_from_pos(domain_cputime_list_minus, i);
							free2(domain_cputime_list_minus);
							domain_cputime_list_minus = res2;
							strcat2(&domain_cputime_list_minus, list_entry);
							strcat2(&domain_cputime_list_minus, "\n");
						}
						else{
							/* read out cpu time value */
							temp = domain_cputime_list_minus + i;
							res2 = string_get_next_wordn(&temp, 2);
							/* convert value */
							t2 = atoi(res2);
							free2(res2);
							/* substract value from original t */
							t -= t2;
						}
					}
					else{
						/* if whole entry doesn't exist, then check if same pid exists */
						i = string_search_keyword_first_all(domain_cputime_list_minus, mypid2);
						if (i > -1){
							/* if wrong pid exists, then remove it */
							res2 = string_remove_line_from_pos(domain_cputime_list_minus, i);
							free2(domain_cputime_list_minus);
							domain_cputime_list_minus = res2;
						}
						/* if clear flag is set, then add entry */
						if (flag_clear){
							strcat2(&domain_cputime_list_minus, list_entry);
							strcat2(&domain_cputime_list_minus, "\n");
						}
					}

					free2(ptime);


					/* convert cputime integer to string */
					ptime = string_itos(t);
					/* create list entries for compare */
					strcpy2(&list_entry, mypid);
					strcat2(&list_entry, " ");
					strcat2(&list_entry, name);
					strcat2(&list_entry, " ");
					strcat2(&list_entry, ptime);
					strcpy2(&mypid2, mypid);
					strcat2(&mypid2, " ");
					strcpy2(&name2, mypid2);
					strcat2(&name2, name);
					strcat2(&name2, " ");

					/* domain cpu time list plus entry exists? */
					i = string_search_keyword_first_all(domain_cputime_list_plus, name2);
					if (i > -1){
						/* update it by removing old entry and adding new one */
						res2 = string_remove_line_from_pos(domain_cputime_list_plus, i);
						free2(domain_cputime_list_plus);
						domain_cputime_list_plus = res2;
						if (!flag_clear){
							/* odd only if clear flag not set */
							strcat2(&domain_cputime_list_plus, list_entry);
							strcat2(&domain_cputime_list_plus, "\n");
						}
					}
					else{
						if (!flag_clear){
							/* add entry */
							strcat2(&domain_cputime_list_plus, list_entry);
							strcat2(&domain_cputime_list_plus, "\n");
						}
					}
					
					free2(ptime);

					free2(pstat);
				}
				free2(res);
			}
		}
	}

	
	/* reset cpu time in uid of domain too if clear flag set */
	if (flag_clear){
		char *dname = 0;

		/* create full domain name to search for */
		strcpy2(&dname, "<kernel> ");
		strcat2(&dname, name);
		i = string_search_line(tdomf, dname);
		free2(dname);
		if (i > -1){
			/* if match, then search for my uid of cpu time */
			temp = tdomf + i;
			i2 = string_search_keyword_first_all(temp, myuid_cputime);
			if (i2 > -1){
				/* if match, then jump to end of line, and rewrite number with null and spaces */
				temp = tdomf + i + i2;
				if (string_jump_next_line(&temp)){
					/* step back 2 chars to last char of former line containing my cpu time uid */
					temp -= 2;
					/* write spaces to number places */
					while(1){
						char c = *temp;
						if (c < '0' || c > '9'){
							temp++;
							*temp = '0';
							break;
						}
						*temp = ' ';
						temp--;
					}
				}
			}
		}

	}

	/* collect cpu times for summary of domain */
	else{

		t = 0;
		temp = domain_cputime_list_plus;
		while(1){
			/* get next entry */
			res2 = string_get_next_line(&temp);
			if (!res2) break;
			temp2 = res2;
			/* get second column (name) */
			res3 = string_get_next_wordn(&temp2, 1);
			if (res3){
				/* is entry for the domain (second column matches name)? */
				if (!strcmp(res3, name)){
					/* get third column (cpu time) */
					ptime = string_get_next_word(&temp2);
					/* convert it and add it */
					t += atoi(ptime);
					free2(ptime);
				}
				free2(res3);
			}
			free2(res2);
		}
		
		/* sum of times */
		mycputime = t;
	}


	closedir(mydir);
	free2(mydir_name);
	free2(mypid);
	free2(mypid2);
	free2(name2);
	free2(list_entry);

	return mycputime;
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
	res = path_link_read(path);
	free2(path);
	return res;
}


/* return a list with the command line options of running process of that pid */
/* return null if no process like that is running */
/* returned value must be freed by caller */
char *process_get_cmdline(int pid)
{
	char *pid2;
	char *path = 0;
	char *cmd;
	int l;

	/* convert pid integer to string */
	pid2 = string_itos(pid);
	/* create /proc/pid/cmdline path from pid */
	strcpy2(&path, "/proc/");
	strcat2(&path, pid2);
	strcat2(&path, "/cmdline");

	/* file exist? */
	if (!file_exist(path)){
		free2(path); free2(pid2); return 0;
	}
	/* load tomld parameters */
	cmd = file_read(path, -1);
	/* convert null chars to new lines except ending null,
	 * because /proc/pid/cmdline gives back options separated by null char */
	l = strlen2(&cmd);
	if (l > 0){
		while(1){
			l--;
			if (!l) break;
			/* if char is null, then change it to new line */
			if (!cmd[l]) cmd[l] = '\n';
		}
	}
	/* add a new line to it */
	strcat2(&cmd, "\n");
	
	free2(path);
	free2(pid2);
	return cmd;
}


/* is process with the pid running */
int process_running(int pid)
{
	int res;
	char *path = 0;
	/* convert pid integer to string */
	char *pid2 = string_itos(pid);
	/* create process path */
	strcpy2(&path, "/proc/");
	strcat2(&path, pid2);

	/* does process dir exist? */
	res = dir_exist(path);

	free2(pid2);
	free2(path);
	return res;
}


/* load config files from kernel memory to user memory */
void load()
{
	/* vars */
	char *tdomf_new, *res, *res2, *res3, *temp, *temp2, *temp3, *temp4;
	char *name1, *name2;

	/* load domain config */
	free2(tdomf);
	tdomf = file_read(tdomk, -1);

	/* load exception config */
	free2(texcf);
	texcf = file_read(texck, -1);


	/* remove disabled mode entries so runtime will be faster */
	tdomf_new = memget2(max_char);
	temp = tdomf;
	while(1){
		/* get next domain */
		res = domain_get_next(&temp);
		/* exit on end */
		if (!res) break;

		/* add domain if not empty */
		if (domain_get_profile(res)){
			strcat2(&tdomf_new, res);
		}
		free2(res);
	}
	free2(tdomf);
	tdomf = tdomf_new;
		

	/* remove quota_exceeded entries */
	tdomf_new = memget2(max_char);
	temp = tdomf;
	while(1){
		/* get next line */
		res = string_get_next_line(&temp);
		if (!res) break;
		
		/* don't add line with quota_exceeded */
		if (!string_search_keyword_first(res, "quota_exceeded")){
			strcat2(&tdomf_new, res);
			strcat2(&tdomf_new, "\n");
		}
		free2(res);
	}
	free2(tdomf);
	tdomf = tdomf_new;


	/* merge similar domains' rules to my domain
	 * these are active domains with running processes
	 * that will transit into my domain after process restart */
	tdomf_new = memget2(max_char);
	temp = tdomf;
	while(1){
		/* get next domain */
		res = domain_get_next(&temp);
		/* exit on end */
		if (!res) break;

		/* check domains only with profile 1-3 */		
		if (domain_get_profile(res)){

			/* get full domain name */
			temp3 = res;
			name1 = string_get_next_line(&temp3);
			if (name1){

				/* check if it's not an exception domain */
				int flag = 0;
				res2 = domain_get_name(res);
				if (string_search_line(tprogs_exc, res2) > -1) flag = 1;
				free2(res2);
				
				/* skip cycle on a main exception domain */
				if (!flag){

					/* add domain and its rules to new policy */
					strcat2(&tdomf_new, res);
					strcat2(&tdomf_new, "\n");
					
					temp2 = tdomf;
					while(1){
						/* get next domain */
						res2 = domain_get_next(&temp2);
						/* exit on end */
						if (!res2) break;

						/* check domains only with profile 1-3 */		
						if (domain_get_profile(res)){

							/* get full domain name */
							temp4 = res2;
							name2 = string_get_next_line(&temp4);
							if (name2){
							
								/* skip the same domain as my domain name */
								if (strcmp(name1, name2)){
									long l1 = strlen2(&name1);
									long l2 = strlen2(&name2);
									if (l1 > 8 && l2 > 8 && l2 >= l1){
										/* compare the domain names without the starting "<kernel>" part */
										/* if they match, then marge this domain's rules to my main domain too */
										if (!strcmp(name1 + 8, name2 + 8 + l2 - l1)){
											/* skip "use_profile" line, this is the next line */
											res3 = string_get_next_line(&temp4);
											free2(res3);
											/* add rules */
											strcat2(&tdomf_new, temp4);
											strcat2(&tdomf_new, "\n");
										}
									}
								}
								free2(name2);
							}
						}
						free2(res2);
					}

					strcat2(&tdomf_new, "\n");
				}
				free2(name1);
			}
		}
		free2(res);
	}
	free2(tdomf);
	tdomf = tdomf_new;


	domain_cleanup();
}


/* reload config files from variables to kernel memory */
/* this is done using "select" and "delete" keywords during file write */
/* to update rules in an existing domain, i feed the diff data:
  select $domainname
  $newrule1
  $newrule2
  delete $rule1
  delete $rule2
  delete $rule3
  * or if domain doesn't exist, then i create the domain without select keyword,
  * and load the rules without deleting anything:
  $domainname
  $newrule1
  $newrule2
*/
void reload()
{
	char *myappend;
	char *res, *res2, *res3, *res4, *temp, *temp2, *temp3, *temp4, *temp5;
	char *tdomf_old, *tdomf_old2, *texcf_old, *texcf_old2;
	char *name1, *name2, *rules = 0, *rules_old = 0, *profile = 0;
	char *domain_names_active = 0;
	int i;

	/* alloc mem for transitions */
	myappend = memget2(max_char);

	/* load old policy from kernel */
	texcf_old = file_read(texck, -1);
	
	/* load exception policy to kernel too */
	while(1){
		/* zero my list */
		strnull2(&myappend);

		/* get the diff of old and new exception policy */
		res = string_get_diff(texcf, texcf_old);
		if (res){
			strcat2(&myappend, res);
			free2(res);
		}

		/* safety code: load old policy again to check if it hasn't changed
		 * since i started creating the new one */
		texcf_old2 = file_read(texck, -1);
		if (!strcmp(texcf_old, texcf_old2)){
			/* write changes to kernel */
			if (strlen2(&myappend)) file_write(texck, myappend);
			free2(texcf_old2);
			break;
		}
		else{
			/* reload changed policy and run whole check again */
			free2(texcf_old);
			texcf_old = texcf_old2;
		}
	}

	free2(texcf_old);
	
	/* load old policy from kernel */
	tdomf_old = file_read(tdomk, -1);
	
	/* load domain policy to the kernel */
	while(1){	

		/* get the list of all current active full domain names */
		strnull2(&domain_names_active);
		temp = tdomf_old;
		while(1){
			/* get next line */
			res = string_get_next_line(&temp);
			if (!res) break;
			
			/* choose only those lines starting with "<kernel>" */
			if (string_search_keyword_first(res, "<kernel>")){
				/* add full domain name to my list */
				strcat2(&domain_names_active, res);
				strcat2(&domain_names_active, "\n");
			}
			free2(res);
		}

		/* zero my list */
		strnull2(&myappend);

		/* add new policy to append list */
		temp = tdomf;
		while(1){
			/* get next domain policy */
			res = domain_get_next(&temp);
			if (!res) break;

			/* get full domain name with "<kernel>" tag */
			temp2 = res;
			name1 = string_get_next_line(&temp2);
			if (name1){
				/* does my new domain exist in the old policy? */
				if (string_search_line(domain_names_active, name1) == -1){
					/* if not, then i add it as a full new domain without making any diff */
					/* add domain name */
					strcat2(&myappend, name1);
					strcat2(&myappend, "\n");
					/* get profile */
					res4 = string_get_next_line(&temp2);
					strcpy2(&profile, res4);
					free2(res4);
					/* get the new rules */
					strcpy2(&rules, temp2);
					/* add rules */
					strcat2(&myappend, profile);
					strcat2(&myappend, "\n");
					strcat2(&myappend, rules);
					strcat2(&myappend, "\n");
				}
				else{
					/* get profile */
					res4 = string_get_next_line(&temp2);
					strcpy2(&profile, res4);
					free2(res4);
					/* get the new rules */
					strcpy2(&rules, temp2);
					/* get the old domain */
					i = string_search_line(tdomf_old, name1);
					temp5 = tdomf_old + i;
					res3 = domain_get_next(&temp5);
					/* jump to domain's rules */
					temp4 = res3;
					string_jump_next_line(&temp4);
					string_jump_next_line(&temp4);
					/* get the old rules */
					strcpy2(&rules_old, temp4);
					res2 = string_get_diff(rules, rules_old);
					/* add domain name */
					strcat2(&myappend, "select ");
					strcat2(&myappend, name1);
					strcat2(&myappend, "\n");
					/* add profile */
					strcat2(&myappend, profile);
					strcat2(&myappend, "\n");
					/* add the diff of my new rules to the list */
					strcat2(&myappend, res2);
					free2(res2);
					free2(res3);
				}

				/* append list with the new rules for the domains of the current running process too */
				/* i do this by searching through the end of all the active domain names,
				 * and if my full domain names matches the end of any active domain names,
				 * then i apply my rules to that one too,
				 * so this way the current running process will have its rules applied on it on-the-fly,
				 * while it will also enter to my prepared domain after its restart */
				if (strlen2(&domain_names_active)){

					/* cycle through the active domain names */
					temp3 = domain_names_active;
					while(1){
						/* get next active domain name */
						name2 = string_get_next_line(&temp3);
						if (!name2) break;

						/* skip the same as my domain name */
						if (strcmp(name1, name2)){
							/* compare its end to my domain name */
							long l1 = strlen2(&name1);
							long l2 = strlen2(&name2);
							if (l1 > 8 && l2 > 8 && l2 >= l1){
								/* compare the domain names without the starting "<kernel>" part */
								/* if they match, then load my new rules to this active domain too */
								if (!strcmp(name1 + 8, name2 + 8 + l2 - l1)){
									/* get the old domain */
									i = string_search_line(tdomf_old, name2);
									temp5 = tdomf_old + i;
									res3 = domain_get_next(&temp5);
									/* jump to domain's rules */
									temp4 = res3;
									string_jump_next_line(&temp4);
									string_jump_next_line(&temp4);
									/* get the old rules */
									strcpy2(&rules_old, temp4);
									res2 = string_get_diff(rules, rules_old);
									/* add domain name */
									strcat2(&myappend, "select ");
									strcat2(&myappend, name2);
									strcat2(&myappend, "\n");
									/* add profile */
									strcat2(&myappend, profile);
									strcat2(&myappend, "\n");
									/* add the diff of my new rules to the list */
									strcat2(&myappend, res2);
									free2(res2);
									free2(res3);
								}
							}
						}
						free2(name2);
					}
			 	}
				free2(name1);
			}
			free2(res);
		}

		/* safety code: load old policy again to check if it hasn't changed
		 * since i started creating the new one */
		tdomf_old2 = file_read(tdomk, -1);
		if (!strcmp(tdomf_old, tdomf_old2)){
			/* write changes to kernel */
			if (strlen2(&myappend)) file_write(tdomk, myappend);
			free2(tdomf_old2);
			break;
		}
		else{
			/* reload changed policy and run whole check again */
			free2(tdomf_old);
			tdomf_old = tdomf_old2;
		}
	}
	
	free2(domain_names_active);
	free2(rules_old);
	free2(profile);
	free2(rules);
	free2(tdomf_old);

	free2(myappend);
}


/* save config files from variables to disk */
void save()
{
	/* store domain policy backup and save config too if backup is missig yet */
	if (!tdomf_bak3){
		/* store backup */
		strcpy2(&tdomf_bak3, tdomf);
		/* save config to disk */
		file_write(tdom, tdomf);
	}
	else{
		if (tdomf){
			/* compare config to backup and save only if changed */
			if (strcmp(tdomf, tdomf_bak3)){
				/* store backup */
				strcpy2(&tdomf_bak3, tdomf);
				/* save config to disk */
				file_write(tdom, tdomf);
			}
		}
	}

	/* store exception policy backup if missig yet */
	if (!texcf_bak3){
		/* store backup */
		strcpy2(&texcf_bak3, texcf);
		/* save config to disk */
		file_write(texc, texcf);
	}
	else{
		if (texcf){
			/* compare config to backup and save only if changed */
			if (strcmp(texcf, texcf_bak3)){
				/* store backup */
				strcpy2(&texcf_bak3, texcf);
				/* save config to disk */
				file_write(texc, texcf);
			}
		}
	}

	/* store log mark backup if missig yet */
	if (!tmarkf_bak3){
		/* store backup */
		strcpy2(&tmarkf_bak3, tmarkf);
		/* save config to disk */
		if (tmarkf) file_write(tmark, tmarkf);
	}
	else{
		if (tmarkf){
			/* compare config to backup and save only if changed */
			if (strcmp(tmarkf, tmarkf_bak3)){
				/* store backup */
				strcpy2(&tmarkf_bak3, tmarkf);
				/* save config to disk */
				file_write(tmark, tmarkf);
			}
		}
	}
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

	/* create new unique names to config files */
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


/* check if only one instance of the program is running and store pid in pid file */
/* fail if tomld is running already, and they have the same mode (both automatic or both manual) */
/* if the running one ia automatic and the second is manual,
 * then offer terminating the automatic before continue to run manual mode tomld */
void check_instance(){
	char *mypid;
	char *cmd;
	char *pid2;
	int  i, i2, p;
	int  *pid_list;
	int  len;
	int  flag_pid_file;
	int  flag_manual;
	
	/* signal to write null to pid file on exit */
	flag_pid = 1;

	/* store path to my executable to a global variable for later */
	my_exe_path = process_get_path(getpid());

	/* get pid list of the same running processes */
	pid_list = process_get_pid_list(my_exe_path);
	if (!pid_list){
		error("error: unknown problem, zero instance of tomld seem to be running\n");
		myexit(1);
	}
	
	/* remove all pids from list that don't belong to a tomld process running with root priv */
	i = 0;
	while(1){
		int i2;
		char *path = 0, *res;
		int p = pid_list[i];
		if (!p) break;

		/* create path */
		res = string_itos(p);
		strcpy2(&path, "/proc/");
		strcat2(&path, res);
		strcat2(&path, "/status");
		free2(res);
		
		/* read in /proc/PID/status file */
		res = file_read(path, -1);
		free2(path);
		if (res){
			/* search for UID: line */
			i2 = string_search_keyword_first_all(res, "Uid:");
			if (i2 > -1){
				/* get UID: line */
				char *temp = res + i2;
				char *res2 = string_get_next_wordn(&temp, 1);
				char *num = string_get_number(res2);
				/* convert uid to integer */
				int n = atoi(num);
				free2(num);
				free2(res2);
				
				/* remove PID from list if PID is non-zero */
				if (n) pid_list[i] = -1;
			}
			free2(res);
		}
		i++;
	}
	/* reorder pid_list removing values of -1 */
	i = 0;
	i2 = 0;
	while(1){
		int p = pid_list[i2];
		if (!p) break;
		if (p != -1){
			int n = pid_list[i2];
			pid_list[i] = n;
			i++; i2++;
		}
		else i2++;
	}
	pid_list[i] = 0;


	/* get my pid number and convert it to string */
	mypid = string_itos(getpid());

	/* get pid from pid file */
	p = 0;
	/* check if my pid file exists */
	if (file_exist(tpid)){
		/* read pid number from pid file */
		pid2 = file_read(tpid, 0);
		/* is it zero? */
		if (strlen2(&pid2)){
			/* is it me? */
			if (strcmp(mypid, pid2)){
				/* is the process with the foreign pid still running?
				 * if so, then store pid number as an extra tomld process */
				p = atoi(pid2);
				if (!process_running(p)) p = 0;
			}
		}
		free2(pid2);
	}


	/* get list length */
	len = 0;
	flag_pid_file = 0;
	while(1){
		int p2 = pid_list[len];
		if (!p2) break;
		/* compare ites from list to pid from pid file */
		if (p && p == p2) flag_pid_file = 1;
		len++;
	}
	/* if pid from pid file didn't match the list, then add it to the list */
	if (p && !flag_pid_file){
		if (len < max_proc){
			pid_list[len++] = p;
			pid_list[len] = 0;
		}
	}

	/* fail if length is zero or greater than 2,
	 * this error is never supposed to happen */
	if (!len || len > 2){
		error("error: abnormal number of tomld instances seem to be running\n");
		flag_pid = 0;
		free2(mypid); free(pid_list);
		myexit(1);
	}
	/* if only 1 is running, then write pid file and success */
	if (len == 1){
		/* store pid file */
		file_write(tpid, mypid);
		free2(mypid); free(pid_list);
		return;
	}

	/* len is supposed to be 2 here meaning 2 tomld processes are running */

	/* if second tomld is in automatic mode, then always fail */
	if (!opt_manual){

		/* there are 2 tomld processes running here and
		 * on --learn, set temporary learning mode form this tomld process
		 * for the other running automatic mode tomld */
		if (opt_learn){
			/* is there any pattern for learn? */
			if (!opt_learn2){
				/* set temporary learning mode flag */
				file_write(tlearn, "1");
			}
			else{
				/* set temporary learning mode flag with list */
				file_write(tlearn, "2");
				file_write(tlearn_list, opt_learn2);
			}
			color("* sent user request to running daemon for temporary learning mode\n", green);
			flag_pid = 0;
			myexit(0);
		}
		else{
			error("error: tomld is already running\n");
			free2(mypid); free(pid_list);
			flag_pid = 0;
			myexit(1);
		}
	}
	
	/* check if first tomld process has been started in manual mode or not */
	/* get the other tomld process' pid */
	p = getpid();
	if (p != pid_list[0]) p = pid_list[0];
	else p = pid_list[1];
	/* get command line options of pid */
	cmd = process_get_cmdline(p);
	/* search for --manual switch */
	flag_manual = 0;
	if (string_search_line(cmd, "--manual") > -1 || string_search_line(cmd, "-m") > -1) flag_manual = 1;
	free2(cmd);

	/* if first tomld is in manual mode, then always fail too */
	if (flag_manual){
		error("error: tomld is already running\n");
		free2(mypid); free(pid_list);
		flag_pid = 0;
		myexit(1);
	}
	/* if first tomld is in automatic mode and second is in manual,
	 * then i ask if second tomld should terminate the first one */
	else{
		color("tomld is already running in automatic mode\n", red);
		if (choice("should i terminate it and run this one?")){
			int c;
			if (kill(p, SIGTERM)){
				error("error: could not terminate other tomld process\n");
				free2(mypid); free(pid_list);
				flag_pid = 0;
				myexit(1);
			}
			color("waiting for other tomld process to terminate...\n", clr);
			
			/* wait for process to terminate for maximum 10 seconds */
			c = 101;
			while(--c){
				/* still running? if not, then success and exit */
				if (!process_running(p)) break;
				/* wait 0.1 sec */
				usleep(100000);
			}
			/* hasn't process terminated succesfully? */
			if (!c){
				free2(mypid); free(pid_list);
				flag_pid = 0;
				myexit(1);
			}
		}
		else{
			free2(mypid); free(pid_list);
			flag_pid = 0;
			myexit(1);
		}
	}
	
	/* store my pid file */
	file_write(tpid, mypid);

	free2(mypid); free(pid_list);
	return;
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
	tmanf_old = file_read(tmank, -1);
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
	if (strcmp(tmanf2, tmanf_old) || !file_exist(tman)){
		int ret = 0;
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
		ret = system(comm);
		/* command failed? */
		if (ret == -1 || ret == 127){
			error("error: could not load manager config to kernel\n"); myexit(1); }
		free2(comm);
	}
	
	/* load profile.conf from kernel and check if it's the same what i have */
	/* if identical, then no reload is needed */
	tprof_old = file_read(tprok, -1);

	/* sort lines before compare */
	res = string_sort_uniq_lines(tprof_old);
	free2(tprof_old); tprof_old = res;
	tprof2 = string_sort_uniq_lines(tprof);
	
	/* compare kernel profile config and mine */
	/* reload it to kernel if they are not identical */
	if (strcmp(tprof2, tprof_old) || !file_exist(tpro)){
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
		int c = 1;
		char *myarg = 0;
		int flag_type = 0;
		
		/* cycle through the arguments */
		while (argc2--){
			int flag_ok = 0;
			strcpy2(&myarg, argv[c]);
			
			if (!strcmp(myarg, "--"))	{ opt_dashdash = 1;	flag_ok = 1; }

			if (!strcmp(myarg, "-v") || !strcmp(myarg, "--verson"))	{ opt_version = 1;	flag_ok = 1; }
			if (!strcmp(myarg, "-h") || !strcmp(myarg, "--help"  ))	{ opt_help    = 1;	flag_ok = 1; }
			
			if (!strcmp(myarg, "-1") || !strcmp(myarg, "--once" ))	{ opt_once    = 1; flag_ok = 1; }
			if (!strcmp(myarg, "-c") || !strcmp(myarg, "--color"))	{ opt_color   = 1; flag_ok = 1; }
			if (!strcmp(myarg, "-k") || !strcmp(myarg, "--keep" ))	{ opt_keep    = 1; flag_ok = 1; }
			if (!strcmp(myarg, "-m") || !strcmp(myarg, "--manual"))	{ opt_manual  = 1; flag_ok = 1; }

			if (!strcmp(myarg, "-i") || !strcmp(myarg, "--info"     ))	{ opt_info       = 1; flag_ok = 2; }
			if (!strcmp(myarg, "-r") || !strcmp(myarg, "--remove"   ))	{ opt_remove     = 1; flag_ok = 3; }
			if (!strcmp(myarg, "-R") || !strcmp(myarg, "--recursive"))	{ opt_recursive  = 1; flag_ok = 4; }
			if (!strcmp(myarg, "-l") || !strcmp(myarg, "--learn"))	    { opt_learn      = 1; flag_ok = 8; }
			if (!strcmp(myarg, "-n") || !strcmp(myarg, "--notify"))	    { opt_notify     = 1; flag_ok = 9; }

			if (!strcmp(myarg, "--mail" ))	   { opt_mail     = 1; flag_ok = 5; }
			if (!strcmp(myarg, "--log"  ))	   { opt_log      = 1; flag_ok = 6; }
			if (!strcmp(myarg, "--no-domain")) { opt_nodomain = 1; flag_ok = 7; }

			if (!strcmp(myarg, "--yes"  ))	   { opt_yes       = 1; flag_ok = 1; }
			if (!strcmp(myarg, "--clear"))	   { opt_clear     = 1; flag_ok = 1; }
			if (!strcmp(myarg, "--reset"))	   { opt_reset     = 1; flag_reset = 1;  flag_ok = 1; }
			if (!strcmp(myarg, "--learn-all")) { opt_learn_all = 1; flag_ok = 1; }

			/* after -- option everything belongs to exebutables */
			if (flag_type == 99) flag_ok = 0;
			else{
				/* else store option type */
				if (flag_ok) flag_type = flag_ok;
				/* after -- option nothing counts as an option but as a file */
				if (opt_dashdash){ flag_type = 99; }
			}

			/* if this is an argument, and not an option */
			if (!flag_ok){
				/* if last option arg was --info, then this belongs to it */
				if (flag_type == 2) strcpy2(&opt_info2, myarg);
				/* if last option arg was --remove, then this belongs to it */
				if (flag_type == 3) strcpy2(&opt_remove2, myarg);
				/* if last option arg was --recursive, then this belongs to it */
				if (flag_type == 4){
					int l;
					/* root "/" dir not allowed */
					if (!strcmp(myarg, "/")){ error("error: root directory is not allowed"); free2(myarg); myexit(1); }
					/* check if dir name exist */
					if (!dir_exist(myarg)){
						error("error: no such directory: "); error(myarg); newl_(); free2(myarg); myexit(1); }
					/* expand recursive dir names with "/" char if missing */
					l = strlen2(&myarg);
					if (myarg[l-1] != '/'){ myarg[l] = '/'; myarg[l+1] = 0; }
					/* if so, store it in recursive dir array */
					strcat2(&dirs_recursive, myarg);
					strcat2(&dirs_recursive, "\n");
				}
				/* if last option arg was --mail, then this belongs to it */
				if (flag_type == 5){
					strcat2(&mail_users, myarg);
					strcat2(&mail_users, " ");
				}
				/* if last option arg was --log, then this belongs to it */
				if (flag_type == 6){
					strcpy2(&opt_log2, myarg);
				}
				/* if last option arg was --no-domain, then this belongs to it */
				if (flag_type == 7){
					int flag_ok_path = 0;
					char *res4 = which(myarg);
					if (res4){
						strcat2(&opt_nodomain2, res4);
						strcat2(&opt_nodomain2, "\n");
						flag_ok_path = 1;
						free2(res4);
					}
					if (!flag_ok_path){
						error("error: wrong argument: "); error(myarg); newl_();
						free2(myarg); myexit(1); }
				}
				/* if last option arg was --learn, then this belongs to it */
				if (flag_type == 8){
					strcat2(&opt_learn2, myarg);
					strcat2(&opt_learn2, "\n");
				}
				/* if last option arg was --notify, then this belongs to it */
				if (flag_type == 9){
					/* store only the first command */
					if (!opt_notify2){
						/* get only exe */
						char *temp3 = myarg;
						char *res3 = string_get_next_word(&temp3);
						if (res3){
							char *res4 = which(res3);
							if (res4){
								/* get rest of parameters */
								char *res6 = string_get_next_line(&temp3);
								/* store it */
								strcpy2(&opt_notify2, res4);
								strcat2(&opt_notify2, " ");
								strcat2(&opt_notify2, res6);
								free2(res6);
								free2(res4);
							}
							free2(res3);
						}
					}
				}
				/* if argument doesn't belong to any option, then it goes to the extra executables */
				if (flag_type == 1 || flag_type == 99){
					char *res;
					if (myarg[0] == '-' && flag_type != 99){
						error("error: wrong argument: "); error(myarg); newl_();
						free2(myarg); myexit(1);
					}
					/* search for name in paths and check if file exists */
					res = which(myarg);
					if (!res){
						error("error: no such file: "); error(myarg); newl_();
						free2(myarg); myexit(1);
					}
					/* if file exists, store it as extra executables */
					strcat2(&tprogs, res);
					strcat2(&tprogs, "\n");
					free2(res);
				}
			}
			c++;
		}
		
		free2(myarg);

		/* exit after --version or --help option */
		if (flag_type != 99 && (opt_version || opt_help)){
			if (opt_version) version();
			if (opt_help)    help();
			myexit(0);
		}

		/* fail if no arguments for --remove option */
		if (opt_remove && !opt_remove2){ error("error: argument missing for --remove option\n"); myexit(1); }
		/* fail if no arguments for --mail option */
		if (opt_mail && !mail_users){ error("error: argument missing for --mail option\n"); myexit(1); }
		/* fail if no arguments for --log option */
		if (opt_log && !opt_log2){ error("error: argument missing for --log option\n"); myexit(1); }
		/* fail if no arguments for --no-domain option */
		if (opt_nodomain && !opt_nodomain2){ error("error: argument missing for --no-domain option\n"); myexit(1); }
		/* fail if no arguments for --recursive option */
		if (opt_recursive && !dirs_recursive){ error("error: argument missing for --recursive option\n"); myexit(1); }

	}
}


/* check status of tomoyo (kernel mode and tools) */
void check_tomoyo()
{
	char *cmd;
	int tver;

	/* check availability of tomoyo system compiled into the kernel above 2.6.36 */
	if (kernel_version() >= 263600){
		cmd = file_read("/proc/kallsyms", -1);
		if (string_search_keyword(cmd, "tomoyo_supervisor") == -1){
			free2(cmd);
			error("error: current kernel is not compiled with tomoyo support\n");
			myexit(1);
		}
		free2(cmd);
	}

	/* check kernel command line */
	cmd = file_read("/proc/cmdline", -1);
	if (string_search_keyword(cmd, " security=tomoyo") == -1){
		free2(cmd);
		error("error: tomoyo kernel mode is not activated\n");
		myexit(1);
	}
	free2(cmd);

	/* check mount state of securityfs */
	cmd = file_read("/proc/mounts", -1);
	if (string_search_keyword(cmd, "none /sys/kernel/security securityfs") == -1){
		int flag_mount = 0;
		/* mount tomoyo securityfs if not mounted */
		/* shell command: mount -t securityfs none /sys/kernel/security */
		flag_mount = mount("none", "/sys/kernel/security", "securityfs", MS_NOATIME, 0);
		if (flag_mount == -1){
			free2(cmd);
			error("error: tomoyo securityfs cannot be mounted\n");
			myexit(1);
		}
	}
	free2(cmd);
	
	/* check tomoyo version */
	tver = tomoyo_version();
	if (tver < 2200 || tver > 2399){
		error("error: tomoyo version is not compatible\n");
		myexit(1);
	}

	/* check tomoyo tool files */
	if (!file_exist(tinit)){ error("error: "); error(tinit); error(" executable binary missing\n"); myexit(1); }
	if (!file_exist(tload)){ error("error: "); error(tload); error(" executable binary missing\n"); myexit(1); }
/*	if (!file_exist(tsave)){ error("error: "); error(tsave); error(" executable binary missing\n"); myexit(1); }*/


	/* load domain config file and search for my unique id in domain policy
	 * and create new one if incompatible */
	if (file_exist(tdom)){
		tdomf = file_read(tdom, 0);
		/* my unique id matches in domain policy? */
		if (string_search_keyword(tdomf, myuid) == -1){
			backup();
			clear();
			color("* warning: incompatible config files found, created new ones after backup\n", red);
		}
		free2(tdomf); tdomf = 0;
	}
}


/* check and load config data from /etc/tomld/tomld.config file */
void check_config()
{
	char *mydir;
	char mode[] = "0755";

	/* load tomld config of special dirs */
	if (file_exist(tspec)){
		char *res, *res2, *temp, *temp2;
		int flag_spec = 0;
		
		/* read file */
		tspecf = file_read(tspec, 0);
		/* cycle through config and sort it out */
		temp = tspecf;
		while(1){
			/* read 1 line */
			res = string_get_next_line(&temp);
			if (!res) break;
			/* skip lines starting with "#" char or empty ones */
			temp2 = res;
			res2 = string_get_next_word(&temp2);
			if (res2){
				if (res2[0] != '#'){
					int flag_ok = 1;

					/* found tag? */
					if (string_search_keyword_first(res2, "[exception]")){ flag_spec = 1; flag_ok = 0; }
					if (string_search_keyword_first(res2, "[wildcard]")){  flag_spec = 2; flag_ok = 0; }
					if (string_search_keyword_first(res2, "[replace]")){   flag_spec = 3; flag_ok = 0; }
					if (string_search_keyword_first(res2, "[recursive]")){ flag_spec = 4; flag_ok = 0; }
					if (string_search_keyword_first(res2, "[mail]")){      flag_spec = 5; flag_ok = 0; }
					if (string_search_keyword_first(res2, "[extra]")){     flag_spec = 6; flag_ok = 0; }
					if (string_search_keyword_first(res2, "[nodomain]")){  flag_spec = 7; flag_ok = 0; }
					if (string_search_keyword_first(res2, "[notify]")){    flag_spec = 8; flag_ok = 0; }
					
					/* place line containing dirs to appropriate array */
					if (flag_ok){
						/* add "/" char to the end of dirs if missing
						 * if last tag was not [mail] */
						long l = strlen2(&res);
						if ((flag_spec < 5 || flag_spec > 8) && l > 0){
							if (res[l - 1] != '/') strcat2(&res, "/"); }
						/* store line */
						if (flag_spec == 1){ strcat2(&spec_exception2, res); strcat2(&spec_exception2, "\n"); }
						if (flag_spec == 2){ strcat2(&spec_wildcard2,  res); strcat2(&spec_wildcard2,  "\n"); }
						if (flag_spec == 3){ strcat2(&spec_replace2,   res); strcat2(&spec_replace2,   "\n"); }
						if (flag_spec == 4){ strcat2(&dirs_recursive,  res); strcat2(&dirs_recursive,  "\n"); }
						if (flag_spec == 5){ strcat2(&mail_users,      res); strcat2(&mail_users,       " "); }
						if (flag_spec == 6){
							/* executable exists? */
							char *res3 = which(res);
							if (res3){
								/* store executable */
								strcat2(&tprogs, res3);
								strcat2(&tprogs, "\n");
								free2(res3);
							}
						}
						if (flag_spec == 7){
							/* executable exists? */
							char *res3 = which(res);
							if (res3){
								/* store executable as an exception */
								strcat2(&tprogs_exc, res3);
								strcat2(&tprogs_exc, "\n");
								free2(res3);
							}
						}
						if (flag_spec == 8){
							/* store only the first command */
							if (!opt_notify2){
								/* get only exe */
								char *temp3 = res;
								char *res3 = string_get_next_word(&temp3);
								if (res3){
									char *res4 = which(res3);
									if (res4){
										/* get rest of parameters */
										char *res6 = string_get_next_line(&temp3);
										/* store it */
										strcpy2(&opt_notify2, res4);
										strcat2(&opt_notify2, " ");
										strcat2(&opt_notify2, res6);
										free2(res6);
										free2(res4);
									}
									free2(res3);
								}
							}
						}
					}
				}
				free2(res2);
			}
			free2(res);
		}
		free2(tspecf);
	}

	/* initialize special dir lists with default value that have no tag in config file */
	if (!spec_exception2) spec_exception2 = array_copy_to_string_list(spec_exception);
	if (!spec_wildcard2)  spec_wildcard2  = array_copy_to_string_list(spec_wildcard);
	if (!spec_replace2)   spec_replace2   = array_copy_to_string_list(spec_replace);
	if (dirs_recursive)   opt_recursive   = 1;
	if (mail_users)       opt_mail        = 1;
	if (opt_notify2)      opt_notify      = 1;

	/* create tomoyo dir if it doesn't exist yet */
	if (!dir_exist(tdir)){ mkdir(tdir, S_IRWXU); }

	/* create tomld dirs if they don't exist yet */
	mydir = path_get_parent_dir(tpid);
	if (!dir_exist(mydir)){ mkdir(mydir, S_IRWXU); }
	/* chmod 0755 dir */
	if (chmod (mydir, strtol(mode, 0, 8)) < 0)
	free2(mydir);
	mydir = path_get_parent_dir(tmark);
	if (!dir_exist(mydir)){ mkdir(mydir, S_IRWXU); }
	/* chmod 0755 dir */
	if (chmod (mydir, strtol(mode, 0, 8)) < 0)
	free2(mydir);
}

/* disable all domains and delete all rules from kernel memory */
void domain_delete_all()
{
	char *res, *res2, *res3, *temp, *temp2;
	char *tdomf_old, *tdomf_old2;
	char *myappend = 0;

	while(1){
		/* load old policy from kernel memory */
		tdomf_old = file_read(tdomk, -1);
		
		/* zero my config */
		strnull2(&myappend);

		/* create my list for load */
		temp = tdomf_old;
		while(1){
			/* get next domain policy */
			res = domain_get_next(&temp);
			if (!res) break;
			/* get domain name */
			temp2 = res;
			res2 = string_get_next_line(&temp2);
			if (res2){
				/* select domain name */
				strcat2(&myappend, "select ");
				strcat2(&myappend, res2);
				strcat2(&myappend, "\nuse_profile 0\n");
				/* skip "use_profile" line */
				res3 = string_get_next_line(&temp2);
				if (res3){
					free2(res3);
					/* delete rules */
					while(1){
						/* get next rule */
						res3 = string_get_next_line(&temp2);
						if (!res3) break;
						if (strlen2(&res3)){
							/* set delete for the rule */
							strcat2(&myappend, "delete ");
							strcat2(&myappend, res3);
							strcat2(&myappend, "\n");
						}
						free2(res3);
					}
				}
				free2(res2);
			}
			free2(res);
		}

		/* safety code: load old policy again to check if it hasn't changed
		 * since i started changing it */
		tdomf_old2 = file_read(tdomk, -1);
		if (!strcmp(tdomf_old, tdomf_old2)){
			free2(tdomf_old2);
			/* write changes to kernel */
			file_write(tdomk, myappend);
			break;
		}
		else{
			/* reload changed policy and run again */
			free2(tdomf_old);
			tdomf_old = tdomf_old2;
		}
	}
	
	free2(tdomf_old);
	free2(myappend);
}


/* disable domain name and delete all rules of it from kernel memory */
void domain_delete(const char *name)
{
	char *res, *res2, *res3, *temp, *temp2;
	char *tdomf_old, *tdomf_old2;
	char *myappend = 0;

	if (!name) return;

	while(1){
		/* load old policy from kernel memory */
		tdomf_old = file_read(tdomk, -1);
		
		/* zero my config */
		strnull2(&myappend);

		/* create my list for load */
		temp = tdomf_old;
		while(1){
			/* get next domain policy */
			res = domain_get_next(&temp);
			if (!res) break;
			/* get domain name */
			temp2 = res;
			res2 = string_get_next_line(&temp2);
			if (res2){
				/* does it match with the domain name to be deleted? */
				if (!strcmp(res2, name)){
					/* select domain name */
					strcat2(&myappend, "select ");
					strcat2(&myappend, res2);
					strcat2(&myappend, "\nuse_profile 0\n");
					/* skip "use_profile" line */
					res3 = string_get_next_line(&temp2);
					if (res3){
						free2(res3);
						/* delete rules */
						while(1){
							/* get next rule */
							res3 = string_get_next_line(&temp2);
							if (!res3) break;
							if (strlen2(&res3)){
								/* set delete for the rule */
								strcat2(&myappend, "delete ");
								strcat2(&myappend, res3);
								strcat2(&myappend, "\n");
							}
							free2(res3);
						}
					}
				}
				free2(res2);
			}
			free2(res);
		}

		/* safety code: load old policy again to check if it hasn't changed
		 * since i started changing it */
		tdomf_old2 = file_read(tdomk, -1);
		if (!strcmp(tdomf_old, tdomf_old2)){
			free2(tdomf_old2);
			/* write changes to kernel */
			file_write(tdomk, myappend);
			break;
		}
		else{
			/* reload changed policy and run again */
			free2(tdomf_old);
			tdomf_old = tdomf_old2;
		}
	}
}


/* clear config files */
void clear()
{
	/* create new configs */
	strnull2(&tdomf);
	strnull2(&texcf);

	strcpy2(&texcf, "initialize_domain /sbin/init\n");
	strcat2(&texcf, "aggregator /proc/\\$/exe /proc/PID/exe\n");

	strcpy2(&tdomf, "<kernel>\nuse_profile 0\n");
	/* add a rule with my unique id too */
	strcat2(&tdomf, myuid);
	strcat2(&tdomf, "\n");

	/* write config files */
	file_write(texc, texcf);
	file_write(tdom, tdomf);
	
	/* delete all other domains from memory too */
	domain_delete_all();

	/* load config files from disk to memory */
	reload();
}


/* info about domains by a pattern */
void domain_info(const char *pattern)
{
	int flag_pattern = 0;
	
	/* load config files from kernel memory */
	load();

	/* is there any pattern? */
	if (pattern){
		if (pattern[0]) flag_pattern = 1;
	}
	
	if (flag_pattern){
		char *tdomf2, *res, *res2, *temp;
		int i, prof;
		int count = 0;
		int flag_first = 0;
		
		tdomf2 = tdomf;
		while(1){
			/* get next domain */
			res = domain_get_next(&tdomf2);
			/* exit on end */
			if (!res) break;

			/* get profile */
			prof = domain_get_profile(res);
			
			/* get first line */
			/* here res2 should be something, so i don't check data availability */
			temp = res;
			res2 = string_get_next_line(&temp);
			
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
				/* print header part in purple if domain is in enforcing mode or else in blue */
				if (prof == 3){ color(res2, purple); newl(); }
				else{ color(res2, blue); newl(); }
				free2(res2);
				/* print use_profile here too */
				res2 = string_get_next_line(&temp);
				color(res2, green); newl();
				free2(res2);
				
				/* sort the rest of the policy text */
				text_new = string_sort_uniq_lines(temp);
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
		else color("error: no domains found\n", clr);
	}

	/* list domain names only */	
	else{
		char *tdomf2, *res, *res2, *temp, *temp2;
		char *c1, *c2;
		char *texcf_new = 0;
		
		tdomf2 = tdomf;
		while(1){
			/* get next domain */
			res = domain_get_next(&tdomf2);
			/* exit on end */
			if (!res) break;

			/* get main domain name */
			res2 = domain_get_name(res);
			if (res2){
				if (domain_get_profile(res) == 3){
					strcat2(&texcf_new, "1 ");
				}
				else{
					strcat2(&texcf_new, "2 ");
				}
				strcat2(&texcf_new, res2);
				strcat2(&texcf_new, "\n");
				free2(res2);
			}
			free2(res);
		}

		/* sort list */		
		res = string_sort_uniq_lines(texcf_new);
		free2(texcf_new); texcf_new = res;

		newl();

		/* print list
		 * it has 2 columns, first is a number; 1 is for enforcing mode domains
		 * and 2 is for the rest */
		temp = texcf_new;
		while(1){
			/* get next line */
			res = string_get_next_line(&temp);
			if (!res) break;
			
			/* first column */
			temp2 = res;
			c1 = string_get_next_word(&temp2);
			if (c1){
				c2 = string_get_next_word(&temp2);
				if (c2){
					/* color option is on? if not, then print '*' char before enforcing mode domains */
					if (opt_color){
						/* enforcing mode domain? */
						if (!strcmp(c1, "1")){ color(c2, purple); newl(); }
						else{ color(c2, blue); newl(); }
					}
					else{
						/* enforcing mode domain? */
						if (!strcmp(c1, "1")){ color("*", clr); color(c2, purple); newl(); }
						else{ color(c2, blue); newl(); }
					}
					free2(c2);
				}
				free2(c1);
			}
			
			free2(res);
		}

		free2(texcf_new);
	}
}


/* remove domains by a pattern */
void domain_remove(const char *pattern)
{
	int flag_pattern = 0;
	
	/* load config files from kernel memory */
	load();

	/* is there any pattern? */
	if (pattern){
		if (pattern[0]) flag_pattern = 1;
	}
	
	if (flag_pattern){
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
							/* remove domain from active kernel domains */
							domain_delete(res2);
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
		else color("error: no domains found\n", clr);
	}
}


/* switch all domains with profile 2-3 back to learning mode */
void domain_set_learn_all()
{
	char *res, *name, *name_sub, *temp, *orig;
	
	/* load config files */
	load();

	/* cycle through domains */
	temp = tdomf;
	while(1){
		/* get next domain */
		orig = temp;
		res = domain_get_next(&temp);
		if (!res) break;
		/* switch domain to learning mode */
		domain_set_profile(&orig, 1);
		/* reset cpu time counter for prog because of learning mode */
		/* get domain names */
		name     = domain_get_name(res);
		name_sub = domain_get_name_full(res);
		/* is it a main domain? */
		if (name && name_sub){
			if (!strcmp(name, name_sub)){
				process_get_cpu_time_all(name, 1);
			}
			free2(name);
			free2(name_sub);
		}

		free2(res);
	}
	
	/* update cpu times in domain policy too (not only in memory) */
	domain_update_cpu_time_all();

	/* save config files and load them to kernel */
	save();
	reload();
}


/* switch old domains only with profile 1-2 to enforcing mode */
void domain_set_enforce_old()
{
	char *res, *res2, *prog, *prog_sub, *temp, *orig;
	int m;
	int flag_old = 0;
	int flag_turned = 0;

	/* is --keep option on? */
	if (!opt_keep){
		
		if (opt_manual){

			/* load config files */
			load();

			/* check if there are old domains with enforcing mode */
			temp = tdomf;
			while(1){
				/* get next domain */
				orig = temp;
				res = domain_get_next(&temp);
				if (!res) break;
				free2(res);
				/* get domain profile */
				m = domain_get_profile(orig);
				/* there are old enforcing mode domains, i have the answer, so quit */
				if (m == 3){ flag_old = 1; break; }
			}

			/* switch old domains to enforcing mode */
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
								color("* switch old domains back to enforcing mode\n", red);
								
								/* notification */
								notify("switch old domains back to enforcing mode");
							}
							/* switch domain and all its subdomains to enforcing mode */
							color(prog, blue); newl();
							domain_set_profile_for_prog(prog, 3);
						}
					}
					free2(prog);
					free2(prog_sub);
				}
				free2(res);
			}

			/* reload them to kernel */		
			reload();
			
			/* did i switch any old domain to enforcing mode? */
			if (!flag_turned){
				if (flag_old) color("* all old domains in enforcing mode already\n", green);
				else color("* no domains in enforcing mode currently\n", green);
			}
		}
		/* auto mode */
		else{
			/* switch temporary learning mode domains back to enforcing mode */
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
					prog_sub = domain_get_name_full(res);
					if (prog && prog_sub){

						/* is it a main domain? */
						if (!strcmp(prog, prog_sub)){
							/* is domain in temporary list? */
							if (string_search_line(tprogs_learn_auto, prog) > -1){
								/* check if not an exception domain */
								if (string_search_line(tprogs_exc, prog_sub) == -1){
									char *nn = 0;

									/* print info once */
									if (!flag_turned){
										flag_turned = 1;
										color("* switch temporary domains back to enforcing mode\n", red);
									}
									/* switch domain and all its subdomains to enforcing mode */
									color(prog, blue); color(", ", clr);
									color("switch to enforcing mode\n", purple);
									domain_set_profile_for_prog(prog, 3);
									
									/* notification */
									strcpy2(&nn, prog); strcat2(&nn, ", switch to enforcing mode");
									notify(nn); free2(nn);
									
									/* remove domain from temporary list */
									res2 = string_remove_line(tprogs_learn_auto, prog);
									free2(tprogs_learn_auto);
									tprogs_learn_auto = res2;
								}
							}
						}
					}
					free2(prog);
					free2(prog_sub);
				}
				free2(res);
			}

			/* reload them to kernel */		
			reload();
		}
	}
	else{
		color("keep domains unchanged for now on demand\n", green);
	}
}


/* check if time for enforcing mode has come for any domain
 * and if so, then switch it to enforcing mode */
void domain_check_enforcing(char *domain)
{
	char *name, *res, *res2, *temp, *ptime, *ptime2;
	int s_uptime, cputime_all_percent, t2, i;
	int d_create, d_change, d_rules, d_cputime, p_uptime, p_cputime;
	int flag_enforcing, flag_cputime_change;
	
	/* get main domain name */
	name = domain_get_name(domain);
	
	/* domain's process is running currently? */
	if (process_running(process_get_pid(name))){

		/* get process uptime of domain */
		p_uptime = process_get_least_uptime(name);
		/* get creation time of domain */
		i = string_search_keyword(domain, myuid_create);
		if (i > -1){
			temp = domain + i;
			/* get my unique id with the creation time in it */
			res = string_get_next_line(&temp);
			/* get epoch time from my uid */
			res2 = string_get_number_last(res);
			/* convert epoch string to integer */
			d_create = time(0) - atoi(res2);
			
			free2(res); free2(res2);

			/* are all processes' uptime greater than the time passed since domain creation time?
			 * if so, then this means that none of the domain's processes has been restarted since then
			 * so this one blocks switching it to enforcing mode */
			if (d_create > p_uptime + 1){

				/* get the sum of cpu times of the domain's all processes */
				p_cputime = process_get_cpu_time_all(name, 0);
				/* get the formerly stored cpu time of domain */
				d_cputime = 0;
				i = string_search_keyword(domain, myuid_cputime);
				if (i > -1){
					temp = domain + i;
					/* get my unique id with the cpu time in it */
					res = string_get_next_line(&temp);
					/* get time string from my uid */
					res2 = string_get_number_last(res);
					/* convert time string to integer */
					d_cputime = atoi(res2);
					free2(res2); free2(res);
				}

				/* get last change time of domain */
				i = string_search_keyword(domain, myuid_change);
				if (i > -1){
					temp = domain + i;
					/* get my unique id with the last change time in it */
					res = string_get_next_line(&temp);
					/* get epoch time from my uid */
					res2 = string_get_number_last(res);
					/* convert epoch string to integer */
					d_change = time(0) - atoi(res2);
					/* change time must not be greater than system uptime,
					 * bacuse if the user turns off the computer immediately and then later back on,
					 * the change time would be huge without any sense */
					s_uptime = sys_get_uptime();
					if (d_change > s_uptime) d_change = s_uptime;
					
					free2(res); free2(res2);

					/* have the processes' cpu time reached a value compared to the complexity
					 * of the domain? (i measure it by the number of its rules)
					 * or is the domain's last change time greater than const_time_max_change?
					 * if so, then i switch the domain to enforcing mode */
					flag_enforcing = 0;
					if (d_change > const_time_max_change && d_cputime + p_cputime > const_min_cputime) flag_enforcing = 1;
					if (!flag_enforcing){
						/* a minimum time has to pass since last domain change to let the
						 * completness of domain grow, or else i reset cpu time of processes
						 * but if it is after reboot, then take uptime into account too
						 * i calculate min time from time of check constant because this is the
						 * intervall to check if domain has changed */
						if (s_uptime > const_time_check * 2 && d_change < const_time_check * 2){
							/* reset cpu times */
							process_get_cpu_time_all(name, 1);
							p_cputime = 0;
							d_cputime = 0;
						}
						/* count complexity of domain by counting rules, then
						 * raising it to the power of 2 and multiplying it by another factor,
						 * and it has to have a minimum limit too */
						d_rules = string_count_lines(domain) + 10;
						d_rules = d_rules * d_rules * const_domain_complexity_factor;
						/* if a particular cpu time has grown compared to the domain complexity
						 * since the last time of domain change, then i switch domain to enforcing mode */
						if (d_rules < d_cputime + p_cputime) flag_enforcing = 1;


						/* ***************************************************************
						 * print domain status on change
						 * ******************************/
						flag_cputime_change = 0;
						cputime_all_percent = (d_cputime + p_cputime) * 100 / d_rules;
						/* convert cputime integer to string */
						ptime = string_itos(cputime_all_percent);
						/* domain cpu time list current entry exists? */
						i = string_search_keyword_first_all(domain_cputime_list_current, name);
						if (i > -1){
							/* read old value */
							temp = domain_cputime_list_current + i;
							ptime2 = string_get_next_wordn(&temp, 1);
							t2 = atoi(ptime2);
							free2(ptime2);

							/* old value match new one? */
							if (t2 != cputime_all_percent){
								/* if no, then i will print status info */
								flag_cputime_change = 1;
								/* update entry by removing old entry and adding new one */
								res2 = string_remove_line_from_pos(domain_cputime_list_current, i);
								free2(domain_cputime_list_current);
								domain_cputime_list_current = res2;
								/* add entry */
								strcat2(&domain_cputime_list_current, name);
								strcat2(&domain_cputime_list_current, " ");
								strcat2(&domain_cputime_list_current, ptime);
								strcat2(&domain_cputime_list_current, "\n");
							}
						}
						else{
							/* add entry */
							strcat2(&domain_cputime_list_current, name);
							strcat2(&domain_cputime_list_current, " ");
							strcat2(&domain_cputime_list_current, ptime);
							strcat2(&domain_cputime_list_current, "\n");
						}
						free2(ptime);

						/* if there was a change in cputime, then print status */
						if (flag_cputime_change){
							color(name, blue);
							/* get human readable seconds */
							res = mytime_get_sec_human(d_change);
							if (opt_color) printf(", changed %s ago, %s%d%%%s complete\n", res, red, cputime_all_percent, clr);
							else           printf(", changed %s ago, %d%% complete\n", res, cputime_all_percent);
							fflush(stdout);
							free2(res);
						}

						/* **************************************************************** */
					}
					if (flag_enforcing){
						char *nn = 0;

						/* switch domain to enforcing mode */
						color(name, blue);
						color(", ", clr);
						color("switch to enforcing mode", purple);
						res = mytime_get_sec_human(d_change);
						color(" after ", clr);
						color(res, clr);
						newl();
						free2(res);
						domain_set_profile_for_prog(name, 3);

						/* notification */
						strcpy2(&nn, name); strcat2(&nn, ", switch to enforcing mode");
						notify(nn); free2(nn);
					}
				}
			}
		}
	}
	
	free2(name);
}


/* get recent access deny logs */
void domain_get_log()
{
	/* vars */
	char *res, *res2, *temp, *temp2, *temp3, *orig, *dlist;
	char *start, *tlogf2 = 0, *tlogf3 = 0;
	char *rules = 0, *prog_rules = 0;
	char *key = 0;
	char key2[] = "denied for ";
	int i, i2, l;
	int key2_len = 11;
	int tlogf_mod_time2;


	/* ------------------- */
	/* ----- get log ----- */
	/* ------------------- */

	/* check modification time of log file and read its content only if modified */
	tlogf_mod_time2 = file_get_mod_time(tlog);
	if (tlogf_mod_time != tlogf_mod_time2){
		/* store new time */
		tlogf_mod_time = tlogf_mod_time2;

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
			/* get 1 line only */
			temp3 = temp2;
			res = string_get_next_line(&temp3);
			if (res){
				/* search for kernel entry */
				i = string_search_keyword(res, "kernel: [");
				i2 = string_search_keyword(res + i, "]");
				if (i > -1 && i2 > -1){
					/* set tmarkf to the last kernel messages' time stamp */
					/* copy line */
					strcpy2(&tmarkf, res);
					i = i + i2 + 1;
					tmarkf[i] = 0;
					strlenset3(&tmarkf, i);
				}
				free2(res);
			}
		}
		/* clear tmarkf if no kernel messages */
		else{
			if (tmarkf) strnull2(&tmarkf);
		}


		/* --------------------------------- */
		/* ----- convert logs to rules ----- */
		/* --------------------------------- */

		/* get a list of all subdomains, so later i add those rules only whose domains exist */
		dlist = domain_get_list();

		if (tomoyo_version() <= 2299) l = 23;
		else l = 15;

		temp = tlogf2;
		while(1){
			/* next rule */
			temp2 = temp;
			res = string_get_next_line(&temp);
			if (!res) break;
			/* get allow_ type */
			temp2 += l;
			res2 = string_get_next_word(&temp2);
			if (!res2){
				error("error: unexpected error, log message format is not correct\n");
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
			/* search the position of "denied for " text,
			 * so i know that all the text before is the parameters of the rule */
			i = string_search_keyword(temp2, key2);
			if (i == -1){
				error("error: unexpected error, log message format is not correct\n");
				free2(tlogf2);
				free2(tlogf3);
				free2(prog_rules);
				free2(rules);
				myexit(1);
			}
			/* get the rule only from log */
			res2 = 0;
			strcpy2(&res2, temp2);
			/* remove spaces from the end of parameter text */
			while(1){
				char c;
				if (i <= 0) break;
				c = res[--i];
				if (c != ' ') break;
			}
			res2[i] = 0;
			strlenset3(&res2, i);
			if (!res2){
				error("error: unexpected error, log message format is not correct\n");
				free2(res);
				free2(tlogf2);
				free2(tlogf3);
				free2(prog_rules);
				free2(rules);
				myexit(1);
			}
			i = strlen2(&res2);
			/* remove single quote from the end of param text */
			if (res2[i - 1] == '\'') res2[i - 1] = 0;
			/* add all together, and rule is complete */
			strcat2(&rules, res2);
			free2(res2);

			
			/* get program name the rule belongs to */
			/* jump behind "denied for " part */
			i = string_search_keyword(temp2, key2);
			if (i == -1){
				error("error: unexpected error, log message format is not correct\n");
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
				error("error: unexpected error, log message format is not correct\n");
				free2(res);
				free2(tlogf2);
				free2(tlogf3);
				free2(prog_rules);
				free2(rules);
				myexit(1);
			}
			/* add rule to list only if binary name belongs to an existing domain */
			if (string_search_line(dlist, res2) > -1){
				/* create text for sorting in a format like "binary allow_ rule" */
				strcat2(&prog_rules, res2);
				strcat2(&prog_rules, " ");
				strcat2(&prog_rules, rules);
				strcat2(&prog_rules, "\n");
				free2(res2);
			}
			
			free2(res);
		}

		free2(rules);
		free2(dlist);


		/* ------------------------------- */
		/* ----- add rules to policy ----- */
		/* ------------------------------- */

		if (prog_rules){
			char *tdomf_new, *prog, *rule, *rules_new = 0, *prog_rules_new = 0;
			char *log_recent = 0;

			/* sort and unique rules */
			res = string_sort_uniq_lines(prog_rules);
			free2(prog_rules);
			prog_rules = res;

			if (strlen2(&prog_rules)){
				color("* access deny log messages ", yellow);
				mytime_print_date(); newl();
			}

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
				/* is manual mode on? */
				if (opt_manual){
					/* choose if to allow denied rule */
					if (choice("  add rules?")){
						/* add rules to new rules */
						strcat2(&prog_rules_new, res);
						strcat2(&prog_rules_new, "\n");
					}
				}
				else{
					/* check if there was a user requested for temporary learning mode,
					 * and if so, then allow rules from deny logs */
					if (flag_learn){

						/* if there a list with domain pattern? */
						if (!opt_learn2){
							/* add rules to new rules */
							strcat2(&prog_rules_new, res);
							strcat2(&prog_rules_new, "\n");
							color("  add rules? (y)\n", clr);
						}
						else{
							/* yes, there is a list */
							int flag = 0;
							char *res6, *temp6;
							temp6 = opt_learn2;
							while(1){
								/* get next pattern */
								res6 = string_get_next_line(&temp6);
								if (!res6) break;
								/* does any pattern match the prog name? */
								if (string_search_keyword(prog, res6) > -1){ flag = 1; break; }
								free2(res6);
							}
							/* was there a match? if so, then add rules */
							if (flag){
								/* add rules to new rules */
								strcat2(&prog_rules_new, res);
								strcat2(&prog_rules_new, "\n");
								color("  add rules? (y)\n", clr);
							}
							/* else don't add rules */
							else{
								color("  add rules? (n)\n", clr);
								/* store recent deny logs */
								strcat2(&log_recent, res);
								strcat2(&log_recent, "\n");
							}
						}
					}
					else{
						color("  add rules? (n)\n", clr);
						/* store recent deny logs */
						strcat2(&log_recent, res);
						strcat2(&log_recent, "\n");
					}
				}

				free2(prog);
				free2(rule);
				free2(res);
			}
			free2(prog_rules);
			prog_rules = prog_rules_new;

			
			if (log_recent){

				/* notification */
				notify(log_recent);

				/* on --mail, send mail about recent deny logs to user too */
				if (opt_mail){
					char *comm = 0;
					char *text = 0;

					/* create message */
					strcat2(&text, "Subject: deny logs from tomld\n");
					strcat2(&text, log_recent);

					/* create command */
					strcat2(&comm, mail_mta);
					strcat2(&comm, " ");
					strcat2(&comm, mail_users);
					
					/* mail binary exists? */
					pipe_write(comm, text);
					free2(comm);
					free2(text);
				}
			}
			free2(log_recent);
			
			

			/* clear learn flag, because i want to allow temporary learning mode only for those domains,
			 * that had access deny logs just now,
			 * the rest of the enforcing mode domains should stay as the are for security reasons */
			flag_learn = 0;

			/* are there any new rules after confirmation? */
			if (strlen2(&prog_rules)){

				/* alloc mem for new domain policy */
				tdomf_new = memget2(max_char);

				/* cycle through domains to add new rules */
				temp = tdomf;
				while(1){
					/* get next domain policy */
					orig = temp;
					res = domain_get_next(&temp);
					if (!res) break;
					/* get subdomain name */
					res2 = domain_get_name_sub(res);
					if (res2){
						int flag_once = 0;
						temp2 = prog_rules;
						strnull2(&rules_new);

						while(1){
							/* get prog name and its rules */
							prog = string_get_next_word(&temp2);
							if (!prog) break;
							rule = string_get_next_line(&temp2);
							if (!rule){ free2(prog); break; }

							/* is it a main domain? (compare prog name to subdomain name) */
							if (!strcmp(prog, res2)){
								/* if match, add rule to domain policy */
								strcat2(&rules_new, rule);
								strcat2(&rules_new, "\n");
								if (!flag_once){
									/* switch domain to learning mode */
									domain_set_profile_for_prog(prog, 1);
									/* reread domain because it changed when i changed its profile just now
									 * in 'tdomf', and the old one was in 'res' */
									free2(res);
									temp = orig;
									res = domain_get_next(&temp);
									/* reset cpu time counter for prog because of switching it to learning mode,
									 * or else it would switch back to enforcing mode immediately */
									process_get_cpu_time_all(prog, 1);
									/* add domain to temporary list */
									if (string_search_line(tprogs_learn_auto, prog) == -1){
										char *nn;
										
										strcat2(&tprogs_learn_auto, prog);
										strcat2(&tprogs_learn_auto, "\n");

										/* print info */
										color(prog, blue); color(", ", clr);
										color("switch to learning mode\n", clr);

										/* notification */
										strcpy2(&nn, prog);	strcat2(&nn, ", switch to learning mode");
										notify(nn); free2(nn);
									}
									/* do it only once per domain for speed */
									flag_once = 1;
								}
							}

							free2(rule);
							free2(prog);
						}
						free2(res2);
					}
					/* add domain policy to new policy */
					strcat2(&tdomf_new, res);
					strcat2(&tdomf_new, "\n");
					strcat2(&tdomf_new, rules_new);
					strcat2(&tdomf_new, "\n");
					free2(res);
				}
				free2(rules_new);
				
				color("* switch domains with new rules to learning mode\n", red);

				/* replace old policy with new one */
				free2(tdomf);
				tdomf = tdomf_new;
			}
		}

		free2(prog_rules);
		free2(tlogf2);
		free2(tlogf3);
	}
}


/* print programs already in domain but not in progs list */
void domain_print_list_not_progs()
{
	/* vars */
	char *res, *res2, *list, *list2 = 0, *list3, *temp;

	/* get list of all main domain names */
	list = domain_get_list();

	/* remove entries from the list that are in tprogs too */
	temp = list;
	while(1){
		res = string_get_next_line(&temp);
		if (!res) break;
		/* does tprogs contain any of the domain names? */
		if (string_search_line(tprogs, res) == -1){
			/* add domain to tprogs if not in exceptions */
			if (string_search_line(tprogs_exc, res) == -1){
				strcat2(&tprogs, res);
				strcat2(&tprogs, "\n");
			}
		}
		else{
			/* these are already existing domains */
			strcat2(&list2, res);
			strcat2(&list2, "\n");
		}
		free2(res);
	}
	/* sort list */
	res = string_sort_uniq_lines(tprogs);
	free2(tprogs); tprogs = res;

	
	free2(list);
	list = list2;

	/* is any element in the list? */
	if (flag_firstrun && strlen2(&list)){

		/* sort list */
		list2 = string_sort_uniq_lines(list);
		free2(list); list = list2;

		color("* already existing main domains\n", green);

		/* get list of filenames only */
		list2 = 0;
		temp = list;
		while(1){
			res = string_get_next_line(&temp);
			if (!res) break;
			res2 = path_get_filename(res);
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
		if (!prog) break;
		

		/* does the domain exception exist for the program?
		 * if not, then add program entry to exception policy */
		strcpy2(&s, "initialize_domain ");
		strcat2(&s, prog);
		if (string_search_line(texcf, s) == -1){
			strcat2(&texcf, s);
			strcat2(&texcf, "\n");
		}
		free2(s); s = 0;
		

		/* does the domain exist for the program? */
		pos = domain_exist(prog);
		if (pos == -1){
			char *t;
			
			color(prog, blue);
			color(", no domain, ", clr);
			color("create domain with learning mode", green);

			/* if the program is running already, then restart is needed for the rules to take effect */
			if (process_get_pid(prog)) color(" (restart needed)", red);

			if (!flag_firstrun) newl();
			/* create a rule for it */
			strcat2(&tdomf, "<kernel> ");
			strcat2(&tdomf, prog);
			strcat2(&tdomf, "\nuse_profile 1\n");
			/* add a rule with my unique id and the time in seconds
			 * to know when i created this domain and when it changed last time
			 * so i will know from the uptime of the process
			 * if it was restarted at least once since domain creation */
			t = string_ltos(time(0));
			strcat2(&tdomf, myuid_create);
			strcat2(&tdomf, t);
			strcat2(&tdomf, "\n");
			strcat2(&tdomf, myuid_change);
			strcat2(&tdomf, t);
			strcat2(&tdomf, "\n");
			strcat2(&tdomf, myuid_cputime);
			strcat2(&tdomf, "0\n");
			free2(t);

			/* store prog name to know not to switch these ones to enforcing mode on exit */
			if (string_search_line(tprogs_learn, prog) == -1){
				strcat2(&tprogs_learn, prog);
				strcat2(&tprogs_learn, "\n");
			}
		}
		else{
			int profile;
			if (flag_firstrun){
				int p_uptime, d_create;

				color(prog, blue);
				color(", domain exists", clr);

				/* **************************************
				 * check if restart is still needed,
				 * this happens when the uptime of process is greater than
				 * the time passed since domain creation */

				/* get process uptime of domain */
				p_uptime = process_get_least_uptime(prog);
				/* get creation time of domain */
				d_create = domain_get_creation_time(prog);
				/* are all processes' uptime greater than the time passed since domain creation time?
				 * if so, then this means that none of the domain's processes has been restarted since then
				 * so this one blocks switching it to enforcing mode */
				if (p_uptime && (d_create == -1 || d_create < p_uptime + 1)) color(" (restart needed)", red);
				/* *************************************** */

			}

			/* get profile mode for domain */
			profile = domain_get_profile(tdomf + pos);
			if (profile == -1){ error("error: domain policy is corrupt\n"); free2(prog); myexit(1); }
			/* check which mode is on */

			/* disabled mode */
			if (!profile){
				if (flag_firstrun){
					color(prog, blue);
					color(", disabled mode, ", clr);
					color("switch to learning mode", green);
				}
				/* switch the domain and all its subdomains to learning mode */
				domain_set_profile_for_prog(prog, 1);
				/* reset cpu time counter for prog because of switching it to learning mode */
				process_get_cpu_time_all(prog, 1);
			}

			/* learning mode */
			if (profile == 1){
				if (flag_firstrun) color(", learning mode on", clr);

				/* check if time for enforcing mode has come for any domain
				 * and if so, then switch domain to enforcing mode */
				else{
					if (!opt_manual){
						/* set position in domain policy to beginning of domain */
						char *temp = tdomf + pos;
						/* get domain policy */
						char *res = domain_get_next(&temp);
						/* check domain if it has to be switched to enforcing mode */
						domain_check_enforcing(res);
						free2(res);
					}
				}
			}

			/* permissive mode */
			if (profile == 2){
				if (flag_firstrun) color(", permissive mode on", clr);
			}

			/* enforcing mode */
			if (profile == 3){
				if (flag_firstrun){ color(", ", clr); color("enforcing mode on", purple); }
			}

		}
		
		if (flag_firstrun) newl();
		
		free2(prog);
	}
}


/* compare names conatining wildcards of "\\*" char */
/* i check only the left most and right most parts of the "*" chars at the edge,
 * no matter how many "*" chars are in the name,
 * cause more than 1 means the middle tags may be combined any way */
int compare_names(char *name1, char *name2)
{
	char *s1, *s2;
	char *left1 = 0, *left2 = 0, *right1 = 0, *right2 = 0;
	char c, c_old;
	int i1, i2;
	int flag1 = 0;
	int flag2 = 0;
	long l1 = 0, l2 = 0;
	long left1l = 0, left2l = 0, right1l = 0, right2l = 0;

	/* get name lengths */
	if (name1) l1 = strlen(name1);
	if (name2) l2 = strlen(name2);
	s1 = memget2(l1);
	s2 = memget2(l2);
	
	/* *********************************************** */
	/* simplify names by replacing "\\*" with "*" char */
	/* *********************************************** */
	c = 0;
	i1 = 0;
	i2 = 0;
	while(1){
		c_old = c;
		c = name1[i1];
		if (!c) break;
		if (c == '*'){
			if (c_old == '\\') i2--;
			flag1 = 1;
		}
		s1[i2] = c;
		i1++; i2++;
	}
	s1[i2] = 0;
	strlenset3(&s1, i2);
	l1 = i2;
	
	c = 0;
	i1 = 0;
	i2 = 0;
	while(1){
		c_old = c;
		c = name2[i1];
		if (!c) break;
		if (c == '*'){
			if (c_old == '\\') i2--;
			flag2 = 1;
		}
		s2[i2] = c;
		i1++; i2++;
	}
	s2[i2] = 0;
	strlenset3(&s2, i2);
	l2 = i2;
	
	/* ************************************************************* */
	/* get left most and right most parts of strings beside "*" char */
	/* ************************************************************* */
	if (flag1){
		
		/* get left 1 */
		left1 = memget2(l1);
		i1 = 0;
		i2 = 0;
		while(1){
			c = s1[i1];
			if (c == '*') break;
			left1[i2] = c;
			i1++; i2++;
		}
		left1[i2] = 0;
		strlenset3(&left1, i2);
		left1l = i2;

		/* get right 1 */
		right1 = memget2(l1);
		i1 = l1;
		while(1){
			if (s1[i1] == '*') break;
			i1--;
		}
		i1++;
		i2 = 0;
		while(1){
			c = s1[i1];
			if (!c) break;
			right1[i2] = c;
			i1++; i2++;
		}
		right1[i2] = 0;
		strlenset3(&right1, i2);
		right1l = i2;
	}

	if (flag2){
		
		/* get left 1 */
		left2 = memget2(l2);
		i1 = 0;
		i2 = 0;
		while(1){
			c = s2[i1];
			if (c == '*') break;
			left2[i2] = c;
			i1++; i2++;
		}
		left2[i2] = 0;
		strlenset3(&left2, i2);
		left2l = i2;

		/* get right 1 */
		right2 = memget2(l2);
		i1 = l2;
		while(1){
			if (s2[i1] == '*') break;
			i1--;
		}
		i1++;
		i2 = 0;
		while(1){
			c = s2[i1];
			if (!c) break;
			right2[i2] = c;
			i1++; i2++;
		}
		right2[i2] = 0;
		strlenset3(&right2, i2);
		right2l = i2;
	}
	
	/* **************************************************************** */
	/* check the four cases by comparing left most and right most parts */
	/* **************************************************************** */
	if (!flag1 && !flag2){
		if (l1 == l2){
			if (!strcmp(s1, s2)){
				free2(s1);
				free2(s2);
				return 1;
			}
		}
	}
	else if (!flag1 && flag2){
		if (l1 >= left2l + right2l){
			if (string_search_keyword_first(s1, left2)
			 && string_search_keyword_last(s1, right2)){
				free2(left2);
				free2(right2);
				free2(s1);
				free2(s2);
				return 1;
			}
		}
		free2(left2);
		free2(right2);
	}
	else if (flag1 && !flag2){
		if (l2 >= left1l + right1l){
			if (string_search_keyword_first(s2, left1)
			 && string_search_keyword_last(s2, right1)){
				free2(left1);
				free2(right1);
				free2(s1);
				free2(s2);
				return 1;
			}
		}
		free2(left1);
		free2(right1);
	}
	else if (flag1 && flag2){
		if (left1l > left2l) i1 = string_search_keyword_first(left1, left2);
		else                 i1 = string_search_keyword_first(left2, left1);
		if (right1l > right2l) i2 = string_search_keyword_last(right1, right2);
		else                   i2 = string_search_keyword_last(right2, right1);
		if (i1 && i2){
			free2(left1);
			free2(left2);
			free2(right1);
			free2(right2);
			free2(s1);
			free2(s2);
			return 1;
		}
		free2(left1);
		free2(left2);
		free2(right1);
		free2(right2);
	}
	
	free2(s1);
	free2(s2);
	return 0;
}


/* compare paths with wildcards */
/* paths' types (both dirs or both files) must match */
/* wildcard can be "\\*" or recursive wildcard */
/* only standalone recursive wildcard at the end is supported */
/* return true if they match, even if both are null */
int compare_paths(char *path1, char *path2)
{
	/* vars */
	char c1, c2;
	int flag;
	int i, i1, i2;
	int w1, w2;
	int is_dir;
	char *s1, *s2;
	
	if (!path1 || !path2) return 0;
	
	c1 = path1[0];
	c2 = path2[0];
	
	/* success if both are null */
	if (!c1 && !c2) return 1;
	if (!c1 || !c2) return 0;
	
	/* fail if they don't start with "/" char and they don't match either */
	/* this is needed for kernel above 2.6.33 where an allow_create and allow_mkdir
	 * rule can have a second parameter that is not a dir (like 0644) */
	if (c1 != '/' || c2 != '/'){
		if (strcmp(path1, path2)) return 0;
	}
	
	/* fail if one of it is a dir and the other is a file */
	is_dir = path_is_dir(path1);
	if (is_dir != path_is_dir(path2)) return 0;
	
	/* alloc mem for subdir names */
	s1 = memget2(max_char);
	s2 = memget2(max_char);
	
	/* compare paths only */
	i1 = 1;
	i2 = 1;
	while(1){
		/* get first subdir name */
		strnull2(&s1);
		i = 0;
		flag = 0;
		while(1){
			c1 = path1[i1++];
			if (!c1) break;
			/* skip multiple "/" chars next to each other */
			if (!flag){
				if (c1 != '/'){
					flag = 1;
					s1[i++] = c1;
				}
			}
			else{
				if (c1 == '/') break;
				s1[i++] = c1;
			}
		}
		s1[i] = 0;
		strlenset3(&s1, i);

		/* get second subdir name */
		strnull2(&s2);
		i = 0;
		flag = 0;
		while(1){
			c2 = path2[i2++];
			if (!c2) break;
			/* skip multiple "/" chars next to each other */
			if (!flag){
				if (c2 != '/'){
					flag = 1;
					s2[i++] = c2;
				}
			}
			else{
				if (c2 == '/') break;
				s2[i++] = c2;
			}
		}
		s2[i] = 0;
		strlenset3(&s2, i);

		/* if any of the subdir name contains recursive wildcard, then success */
		/* i check full name of recursive wildcards only at the end */
		/* recursive wildcard somewhere in the name is not supported */
		w1 = strcmp(s1, wildcard_recursive_plain);
		w2 = strcmp(s2, wildcard_recursive_plain);
		if (!w1 || !w2){
			/* if paths are not dirs, then there must be another subdir in the other path
			 * that is not special recursive wildcard, because the wildcard means
			 * at least 1 or more dirs */
			if (!is_dir){
				/* if s1 is recursive wildcard and s2 is the last subdir (file), then fail */
				if (!w1 && !c2) break;
				/* if s2 is recursive wildcard and s1 is the last subdir (file), then fail */
				if (!w2 && !c1) break;
			}
			free2(s1);
			free2(s2);
			return 1;
		}

		/* compare names simple way */
		if (strcmp(s1, s2)){
			/* compare wildcard names if no simple match and fail if no match either this way */
			if (!compare_names(s1, s2)) break;
		}

		/* return success if match and both reached end */
		if (!c1 && !c2){
			free2(s1);
			free2(s2);
			return 1;
		}
		
		/* fail and exit if only one of the subdir name reached end */
		if (!c1 || !c2) break;
	}
	
	free2(s1);
	free2(s2);
	
	return 0;
}


/* compare rules */
int compare_rules(char *r1, char *r2)
{
	/* vars */
	char *type1 = 0, *type2 = 0;
	char *rule1a = 0, *rule1b = 0, *rule2a = 0, *rule2b = 0;
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
		free2(type1); free2(type2);
		free2(rule1b);
		return 0;
	}
	
	/* get second params if any */
	rule2a = string_get_next_word(&temp2);
	rule2b = string_get_next_word(&temp2);
	/* fail if no first path in param 2 */
	if (!rule2a){
		free2(type1); free2(type2);
		free2(rule1a); free2(rule1b);
		free2(rule2b);
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


/* add dir to a list of dirs if it doesn't contain it yet and return new list */
/* returned value must be freed by caller */
char *compare_path_add_dir_to_list_uniq(char *list, char *dir)
{
	char *res, *temp, *new = 0;
	int flag = 0;

	/* copy old list */
	strcpy2(&new, list);

	/* search for the dir in the list by wildcarded compare */
	temp = new;
	while(1){
		res = string_get_next_line(&temp);
		if (!res) break;
		if (compare_paths(res, dir)){ flag = 1; free2(res); break; }
		free2(res);
	}
	/* add dir to list if it doesn't contain it yet*/
	if (!flag){
		strcat2(&new, dir);
		strcat2(&new, "\n");
	}
	
	return new;
}


/* search for dir in a list of dirs and return true on match */
int compare_path_search_dir_in_list(char *list, char *dir)
{
	char *res, *temp;

	/* search for the dir in the list by wildcarded compare */
	temp = list;
	while(1){
		res = string_get_next_line(&temp);
		if (!res) return 0;
		if (compare_paths(res, dir)){ free2(res); return 1; }
		free2(res);
	}
	
	return 0;
}


/* search for dir in a list of dirs comparing only the beginnings of paths
 * and return path with all wildcards taken from both paths on match */
/* returned value must be freed by caller */
char *compare_path_search_dir_in_list_first_subdirs(char *list, char *dir)
{
	char *res, *res2, *temp;
	int c, c1, c2, i1, i2, in1, in2;
	int flag1, flag2;
	long l1, l2;

	/* search for the dir in the list by wildcarded compare */
	temp = list;
	while(1){
		res = string_get_next_line(&temp);
		if (!res) return 0;
		c = path_count_subdirs_name(res);
		res2 = path_get_subdirs_name(dir, c);
		if (compare_paths(res, res2)){

			/* merge together the 2 paths subdir by subdir
			 * prefering the one from the list
			 * or the one with wildcard in it */
			char *path1 = res;
			char *path2 = dir;
			char *new, *new1, *new2;
			l1 = strlen2(&path1);
			l2 = strlen2(&path2);
			new  = memget2((l1 + l2) * 2);
			/* paths not null? */
			if (l1 && l2){
				new1 = memget2(l1 * 2);
				new2 = memget2(l2 * 2);
				i1 = 0; i2 = 0;
				c1 = 0; c2 = 0;

				/* merge paths */
				while(1){
					/* get subdir from path1 */
					flag1 = 0;
					in1 = 0;
					while(1){
						c1 = path1[i1];
						if (c1 == '*') flag1 = 1;
						new1[in1] = c1;
						if (!c1 || c1 == '/'){
							if (c1){ in1++; i1++; }
							break;
						}
						in1++;
						i1++;
					}
					new1[in1] = 0;
					strlenset3(&new1, in1);
					
					/* get subdir from path2 */
					flag2 = 0;
					in2 = 0;
					while(1){
						c2 = path2[i2];
						if (c2 == '*') flag2 = 1;
						new2[in2] = c2;
						if (!c2 || c2 == '/'){
							if (c2){ in2++; i2++; }
							break;
						}
						in2++;
						i2++;
					}
					new2[in2] = 0;
					strlenset3(&new2, in2);
					
					/* compare subnames */
					/* if both contain wildcard, then i prefer the one from list (path1)
					 * or the one that is not null */
					if (flag1)               strcat2(&new, new1);
					else if (flag2)          strcat2(&new, new2);
					else if (strlen2(&new1)) strcat2(&new, new1);
					else                     strcat2(&new, new2);
					
					/* exit if both paths reached end */
					if (!c1 && !c2) break;
				}
				free2(new1);
				free2(new2);
			}
			free2(res);
			free2(res2);
			return new;
		}
		free2(res);
		free2(res2);
	}
	
	return 0;
}


/* return true if path is within any of tomld's path */
int path_is_tomld_dir(char *path)
{
	char *path2, *res, *res2, *temp;
	int c1, c2;

	if (!path) return 0;

	/* get dirs only */
	path2 = path_get_dir(path);
	c1 = path_count_subdirs_name(path2);

	temp = tomld_path;
	while(1){
		/* get next path of tomld */
		res = string_get_next_line(&temp);
		if (!res) break;

		/* path contains more or equal number of subdirs than tomld's? */
		c2 = path_count_subdirs_name(res);
		if (c1 >= c2){
			res2 = path_get_subdirs_name(path2, c2);

			/* paths' beginnings match? */
			if (compare_paths(res2, res)){
				/* if so, then return true and exit */
				free2(path2);
				free2(res);
				free2(res2);
				return 1;
			}
			free2(res2);
		}
		free2(res);
	}
	free2(path2);

	return 0;
}


/* return new list of rules based on the input rule with wildcards of matching recursive dirs */
/* returned value must be freed by caller */
char *domain_get_rules_with_recursive_dirs(char *rule)
{
	char *type, *path1, *path2;
	char *res, *res2, *temp, *rules_new;
	char *path_new1, *path_new2;
	int i, count1, count2;
	int c = 0;

	/* return null if input rule is null or "quota_exceeded" */
	if (!rule) return 0;
	if (!strcmp(rule, "quota_exceeded")) return 0;

	/* are there any recursive dirs specified? */
	if (!dirs_recursive) return 0;

	/* get rule type */
	temp = rule;
	type = string_get_next_word(&temp);
	/* return null on empty line */
	if (!type) return 0;
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
			strcpy2(&path_new1, res2);

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
				strcpy2(&path_new2, res2);

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
				if (path_is_dir(path1)){
					strcat2(&rules_new, wildcard_recursive_dir);
				}
				else{
					/* for recursive access of a dir, 2 lines needed, for example:
					 * "allow_read /tmp/test/\{\*\}/\*" and
					 * "allow_read /tmp/test/\*" to allow read from the dir itself too,
					 * cause recursive wildcard means at least 1 dir or more */
					strcat2(&rules_new, wildcard_recursive_file);
					strcat2(&rules_new, "\n");
					strcat2(&rules_new, type);
					strcat2(&rules_new, " ");
					strcat2(&rules_new, path_new1);
					strcat2(&rules_new, "\\*");
				}
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
				if (path_is_dir(path1)){
					strcat2(&rules_new, wildcard_recursive_dir);
				}
				else{
					strcat2(&rules_new, wildcard_recursive_file);
					strcat2(&rules_new, " ");
					strcat2(&rules_new, path2);
					strcat2(&rules_new, "\n");
					strcat2(&rules_new, type);
					strcat2(&rules_new, " ");
					strcat2(&rules_new, path_new1);
					strcat2(&rules_new, "\\*");
				}
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
				if (path_is_dir(path1)){
					strcat2(&rules_new, wildcard_recursive_dir);
				}
				else{
					strcat2(&rules_new, wildcard_recursive_file);
					strcat2(&rules_new, "\n");
					strcat2(&rules_new, type);
					strcat2(&rules_new, " ");
					strcat2(&rules_new, path1);
					strcat2(&rules_new, " ");
					strcat2(&rules_new, path_new2);
					strcat2(&rules_new, "\\*");
				}
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


/* sort and unique rules of the same types in a specific way */
/* returned value must be freed by caller */
char *domain_sort_uniq_rules(char *rules)
{
	char *res, *res2, *temp, *temp2, *temp3, *temp4, *old, *new, *rules_new, *rules2, *param1, *param2;
	int i1, i2, l1, l2, lo, ln;
	int c, c1, c2, co, cn;

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
		/* get params of rule */
		temp3 = res;
		param1 = string_get_next_wordn(&temp3, 1);
		param2 = string_get_next_word(&temp3);
		/* get length of rule */
		l1 = strlen2(&res);
		/* count wildcards in rule */
		/* search for recursive wildcard */
		/* if there is a recursive wildcard in the path of the rule,
		 * then i mark it as if it had many wildcard to choose rather this
		 * instead of the one without recursive wildcard */
		if (path_is_dir_recursive_wildcard(param1) || path_is_dir_recursive_wildcard(param2)) c1 = 9999;
		else{
			c = l1; c1 = 0;
			while(c--) if (res[c] == '*') c1++;
		}
		free2(param1); free2(param2);
		/* compare rule to all rules */
		i2 = 0;
		temp2 = rules;
		strnull2(&new);
		while(1){
			res2 = string_get_next_line(&temp2);
			if (!res2) break;
			/* skip comparing the same entry in the list */
			if (i1 != i2){
				
				/* compare rules containing wildcard */
				if (compare_rules(res, res2)){
					/* get params of rule */
					temp4 = res2;
					param1 = string_get_next_wordn(&temp4, 1);
					param2 = string_get_next_word(&temp4);
					/* get length of rule */
					l2 = strlen2(&res2);
					/* count wildcards in rule */
					/* search for recursive wildcard */
					if (path_is_dir_recursive_wildcard(param1) || path_is_dir_recursive_wildcard(param2)) c2 = 9999;
					else{
						c = l2; c2 = 0;
						while(c--) if (res2[c] == '*') c2++;
					}
					free2(param1); free2(param2);
					/* if match, store the one with the recursive wildcard, or else
					 * store the one with more wildcards in it, or if that's equal,
					 * then the one with the shortest length from the matching ones */
					if (c1 != c2){
						if (c1 > c2){ strcpy2(&old, res);  co = c1; }
						else        { strcpy2(&old, res2); co = c2; }
						/* get length of rule */
						ln = strlen2(&new);
						/* count wildcards in rule */
						if (path_is_dir_recursive_wildcard(new)) cn = 9999;
						else{
							c = ln; cn = 0;
							while(c--) if (new[c] == '*') cn++;
						}
						/* choose the rule with more wildcards */
						if ((!ln) || (cn < co)) strcpy2(&new, old);
					}
					else{
						if (l1 < l2){ strcpy2(&old, res);  lo = l1; }
						else        { strcpy2(&old, res2); lo = l2; }
						ln = strlen2(&new);
						/* choose the shorter rule */
						if ((!ln) || (ln > lo)) strcpy2(&new, old);
					}
				}
			}
			free2(res2);
			i2++;
		}

		/* store the chosen rule if there was any match */
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
	char *tdomf_new, *rules, *rules2, *rules_temp, *rule_type;
	int c;
	
	/* alloc mem for new policy */
	tdomf_new = memget2(max_char);
	/* alloc mem for sorted rules */
	rules = memget2(max_char);
	
	/* cycle through domains and sort and make unique the rules of each */
	temp = tdomf;
	while(1){
		/* get next domain policy */
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

	/* cycle through domains and sort and make unique the rules within the same type */
	temp = tdomf;
	while(1){
		/* get next domain policy */
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
				
				/* get only same type of rules and sort and unique them in a specific way */
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


/* merge together domains of the same names */
void domain_merge_same()
{
	char *d1, *d2, *temp1, *temp2, *temp3;
	char *name1, *name2, *tdomf_new, *list = 0;
	
	/* get mem for new policy */
	tdomf_new = memget2(max_char);
	
	/* cycle through domains */
	temp1 = tdomf;
	while(1){
		/* get next doman */
		d1 = domain_get_next(&temp1);
		if (!d1) break;
		/* get domain's full name */
		name1 = domain_get_name_full(d1);
		
		/* is this domain in the list yet? if so, then skip it
		 * this list is to filter out same domain names and use domain only once */
		if (string_search_line(list, name1) == -1){
			
			/* add domain to new policy */
			strcat2(&tdomf_new, d1);

			/* cycle through domains and compare each to themselves */
			/* search domains starting only from behind the main one */
			temp2 = temp1;
			while(1){
				/* get next doman */
				d2 = domain_get_next(&temp2);
				if (!d2) break;
				/* get domain's full name */
				name2 = domain_get_name_full(d2);
				
				/* domains match? */
				if (name1 && name2){
					if (!strcmp(name1, name2)){
						/* add this domain's rules too to the domain in main cycle */
						temp3 = d2;
						if (string_jump_next_line(&temp3) && string_jump_next_line(&temp3)){
							strcat2(&tdomf_new, temp3);
						}
					}
				}

				free2(name2);
				free2(d2);
			}

			/* new line to new policy */
			strcat2(&tdomf_new, "\n");

			/* add domain to the list */
			strcat2(&list, name1);
			strcat2(&list, "\n");
		}

		free2(name1);
		free2(d1);
	}
	
	/* store new policy */
	free2(tdomf);
	tdomf = tdomf_new;
}


/* add rules with wildcarded recursive dirs to all domain policy rules if they contain any */
void domain_reshape_rules_recursive_dirs()
{
	/* vars */
	char *res, *res2, *res3, *temp, *temp2;
	char *tdomf_new, *rules, *rules2;


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
					res3 = domain_get_rules_with_recursive_dirs(res2);
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


/* the spec predefined dirs and those dirs that have files
 * newly created in them will be wildcarded */
void domain_reshape_rules_wildcard_spec()
{
	/* vars */
	char *res, *res2, *res3, *temp, *temp2;
	char *rule_type, *param1, *param2;
	char *tdomf_new;

	/* there are 2 runs: first is to collect all the dir names that have create entries
	 * second run is to make all entries containing the former ones wildcarded
	 * and do this overall because it cannot be known if one non-unique filname will be reused
	 * by other processes */
	char *spec2 = 0;
	char *spec3_mkdir = 0;

	/* these are the special create entries, where the place of the file will be wildcarded
	 * because it cannot be determined fully yet if the file being created has a unique filename
	 * or a constantly changing one (temporary name) */
	/* allow_mkdir is necessary in this create list too to wildcard dirs of those files too */
	char *cre[] = {"allow_create", "allow_mksock", "allow_rename", "allow_unlink",
	               "allow_mkdir", "allow_link", 0};

	/* allow_mkdir will also have a special handling, cause usually files are created in the
	 * new dir too, and it cannot be surely told if the dir itself has a unique name too,
	 * so this exact dir should be wildcarded too with the files in it */
	char *cre2[] = {"allow_mkdir", 0};

	/* these are rule types to which i always add a second parameter 0-0xFFFFFFFF */
	char *cre3[] = {"allow_ioctl", "allow_chown", "allow_chgrp", 0};

	/* these are rule types to which i always add a second parameter FFFF
	 * because in kernel 2.6.36 and above Tomoyo checks for DAC's permission
	 * more info on kernel differences: http://tomoyo.sourceforge.jp/comparison.html */
	char *cre4[] = {"allow_create", "allow_mkdir", "allow_mkfifo", "allow_mksock", "allow_chmod", 0};
	

	/* cycle through rules of all domains and collect more special dirs (that will be wildcarded) */
	temp = tdomf;
	while(1){
		/* get next rule */
		res = string_get_next_line(&temp);
		if (!res) break;
		
		/* is it a rule starting with "allow_" tag? */
		if (string_search_keyword_first(res, "allow_") && !string_search_keyword_first(res, "allow_execute ")){

			/* get rule type */
			temp2 = res;
			rule_type = string_get_next_word(&temp2);
			if (rule_type){

				char *pdir1 = 0, *pdir2 = 0;

				/* get params */
				param1 = string_get_next_word(&temp2);
				param2 = string_get_next_word(&temp2);
				/* get their parent dirs only (cannot be root dir /) */
				if (param1) pdir1 = path_get_parent_dir(param1);
				if (param2) pdir2 = path_get_parent_dir(param2);

				/* ************************************** */
				/* check if rule is a special create rule */
				/* ************************************** */
				if (array_search_keyword(cre, rule_type)){

					/* are there any parameters (more words)? */
					if (param1){
						/* add dir to the list if not there yet */
						res2 = compare_path_add_dir_to_list_uniq(spec2, pdir1);
						free2(spec2); spec2 = res2;
					}

					/* are there any parameters (more words)? */
					if (param2){
						if (param2[0] == '/'){
							/* add dir to the list if not there yet */
							res2 = compare_path_add_dir_to_list_uniq(spec2, pdir2);
							free2(spec2); spec2 = res2;
						}
					}
				}
				
				/* ************************************* */
				/* check if rule is a special mkdir rule */
				/* ************************************* */
				if (array_search_keyword(cre2, rule_type)){

					/* are there any parameters (more words)? */
					if (param1){
						/* add dir to the list if not there yet */
						res3 = path_wildcard_dir(param1);
						res2 = compare_path_add_dir_to_list_uniq(spec3_mkdir, res3);
						free2(spec3_mkdir); spec3_mkdir = res2;
						free2(res3);
					}

					/* are there any parameters (more words)? */
					if (param2){
						if (param2[0] == '/'){
							/* add dir to the list if not there yet */
							res3 = path_wildcard_dir(param2);
							res2 = compare_path_add_dir_to_list_uniq(spec3_mkdir, res3);
							free2(spec3_mkdir); spec3_mkdir = res2;
							free2(res3);
						}
					}
				}
				
				free2(pdir1);
				free2(pdir2);
				free2(param1);
				free2(param2);
				free2(rule_type);
			}
		}
		free2(res);
	}

	/* alloc mem for new policy */
	tdomf_new = memget2(max_char);

	/* cycle through rules of all domains and reshape them */
	temp = tdomf;
	while(1){
		/* get next rule */
		res = string_get_next_line(&temp);
		if (!res) break;
		
		/* is it a rule starting with "allow_" tag? */
		if (string_search_keyword_first(res, "allow_") && !string_search_keyword_first(res, "allow_execute ")){

			/* get rule type */
			temp2 = res;
			rule_type = string_get_next_word(&temp2);
			if (rule_type){

				char *pdir1 = 0, *pdir2 = 0;
				int c;

				/* get params and their parent dirs */
				param1 = string_get_next_word(&temp2);
				param2 = string_get_next_word(&temp2);
				if (param1) pdir1 = path_get_parent_dir(param1);
				if (param2) pdir2 = path_get_parent_dir(param2);

				/* handle allow_ioctl whose first parameter may not be, and second parameter is not a dir */
				/* **************************************************** */
				/* check if rule is an "allow_ioctl" rule */
				/* **************************************************** */
				if (array_search_keyword(cre3, rule_type)){

					/* param1 */
					if (param1){

						/* check for socket param */
						if (string_search_keyword_first(param1, "socket:[")){
							/* wildcard socket param */
							strcpy2(&param1, "socket:[\\*]");
						}
						
						/* check for pipe param */
						if (string_search_keyword_first(param1, "pipe:[")){
							/* wildcard socket param */
							strcpy2(&param1, "pipe:[\\*]");
						}
						
						/* wildcard param2 if it exists, or always add it above kernel 2.6.36 */
						if (param2 || (kernel_version() >= 263600)){
							strcpy2(&param2, "0-0xFFFFFFFF");
						}
					}
				}

				/* handle allow_mksock and the same whose second parameters are not dirs */
				/* ********************************************** */
				/* check if rule is an "mksock, mkfifo etc." rule */
				/* ********************************************** */
				if (array_search_keyword(cre4, rule_type)){

					/* param2 */
					if (param2){

						/* wildcard param2 if it exists, or always add it above kernel 2.6.36 */
						if (kernel_version() >= 263600){
							strcpy2(&param2, "0-0xFFFF");
						}
					}
				}

				/* ****************************************************** */
				/* handle the rest of the rules with params that are dirs */
				/* ****************************************************** */

				/* add rule to new policy */
				strcat2(&tdomf_new, rule_type);

				/* cycle through the 2 params */
				c = 0;
				while(++c <= 2){
					int flag    = 0;
					int flag3   = 0;
					int flag_ex = 0;
					char *param = 0, *pdir = 0;

					/* set param to 1 and after 2 */
					if (c == 1){ strcpy2(&param, param1); strcpy2(&pdir, pdir1); }
					if (c == 2){ strcpy2(&param, param2); strcpy2(&pdir, pdir2); }
					if (param){
						
						/* check param only if it's a dir starting with "/" char */
						if (param[0] == '/'){

							/* check dir in exception */
							if (compare_path_search_dir_in_list(spec_exception2, pdir)) flag_ex = 1;
							
							/* is it in spec_wildcard? */
							if (!flag_ex){
								if (compare_path_search_dir_in_list(spec_wildcard2, pdir)) flag = 1;
							}

							/* is it in spec2? */
							if (!flag_ex){
								if (!flag){
									if (compare_path_search_dir_in_list(spec2, pdir)) flag = 1;
								}
							}

							/* is it in spec3_mkdir? */
							if (!flag_ex){
								/* isn't parent dir in special excluded dir? */
								res2 = path_get_parent_dir(pdir);
								if (!compare_path_search_dir_in_list(spec_exception2, res2)){
									/* is dir in mkdir list? */
									if (compare_path_search_dir_in_list(spec3_mkdir, pdir)) flag3 = 1;
								}
								free2(res2);
							}

							/* replace dir with the one in spec_replace if any */
							res2 = compare_path_search_dir_in_list_first_subdirs(spec_replace2, param);
							if (res2){
								free2(param);
								param = res2;
							}
							
							/* path is in spec_ex */
							if (flag_ex){
								res2 = path_wildcard_dir_temp(param);
								free2(param); param = res2;
							}
							
							/* path is in spec or spec2 */
							if (flag){
								res2 = path_wildcard_dir(param);
								free2(param); param = res2;
							}
							
							/* path is in spec3_mkdir */
							if (flag3){
								res2 = path_wildcard_dir_plus_parent(param);
								free2(param); param = res2;
							}

							/* wildcard library files version numbers */
							res2 = path_wildcard_lib(param);
							free2(param); param = res2;
							
							/* wildcard /proc/$PID/ if it's in "/proc/[0-9]+/" form */
							res2 = path_wildcard_proc(param);
							free2(param); param = res2;

							/* wildcard /home/$USER/ dir to support multiple users */
							res2 = path_wildcard_home(param);
							free2(param); param = res2;
						}
						
						/* add param to rule */
						strcat2(&tdomf_new, " ");
						strcat2(&tdomf_new, param);
					}
					free2(pdir);
					free2(param);
				}

				/* new lin in new policy */
				strcat2(&tdomf_new, "\n");
				
				
				free2(pdir1);
				free2(pdir2);
				free2(param1);
				free2(param2);
				free2(rule_type);
			}
		}
		
		/* add line to new policy */
		else{
			strcat2(&tdomf_new, res);
			strcat2(&tdomf_new, "\n");
		}
		
		free2(res);
	}

	free2(spec2);
	free2(spec3_mkdir);
	free2(tdomf);
	tdomf = tdomf_new;
}


/* all entries with allow_create will be recreated with allow_unlink, allow_truncate
 * and allow_read/write entries too because there are deny logs frequently
 * coming back for the created files trying to be written, unlinked or truncated.
 * what is created should be allowed to be written or unlinked or truncated too */
void domain_reshape_rules_create_double()
{
	/* vars */
	char *res, *temp, *temp2;
	char *rule_type, *param1, *param2;
	char *tdomf_new;
	
	char *cre[] = {"allow_create", "allow_read/write", "allow_write", "allow_unlink",
		"allow_truncate", 0};
		
	/* alloc mem for new policy */
	tdomf_new = memget2(max_char);
	
	/* cycle through rules of all domains and on create rules, add create, read/write,
	 * unlink and truncate too */
	temp = tdomf;
	while(1){
		/* get next rule */
		res = string_get_next_line(&temp);
		if (!res) break;
		
		/* get rule type and params */
		temp2 = res;
		rule_type = string_get_next_word(&temp2);
		param1 = string_get_next_word(&temp2);
		param2 = string_get_next_word(&temp2);

		/* is the rule any of the create rule type? */
		if (array_search_keyword(cre, rule_type)){

			/* allow_create can have second parameter from kernel 2.6.36 and above */
			strcat2(&tdomf_new, "allow_create ");
			strcat2(&tdomf_new, param1);
			if (param2){
				strcat2(&tdomf_new, " ");
				strcat2(&tdomf_new, param2);
			}
			else{
				if (kernel_version() >= 263600){
					strcat2(&tdomf_new, " ");
					strcat2(&tdomf_new, "0-0xFFFF");
				}
			}
			strcat2(&tdomf_new, "\n");

			strcat2(&tdomf_new, "allow_read/write ");
			strcat2(&tdomf_new, param1);
			strcat2(&tdomf_new, "\n");

			strcat2(&tdomf_new, "allow_unlink ");
			strcat2(&tdomf_new, param1);
			strcat2(&tdomf_new, "\n");

			strcat2(&tdomf_new, "allow_truncate ");
			strcat2(&tdomf_new, param1);
			strcat2(&tdomf_new, "\n");
			
		}
		else{
			strcat2(&tdomf_new, res);
			strcat2(&tdomf_new, "\n");
		}
		
		free2(param1);
		free2(param2);
		free2(rule_type);
		free2(res);
	}
	
	/* replace old policy with new one */
	free2(tdomf);
	tdomf = tdomf_new;
}


/* remove all entries that contains a path that matches any of tomld's working directory */
void domain_remove_tomld_dir()
{
	/* vars */
	char *res, *temp, *temp2;
	char *rule_type, *param1, *param2;
	char *tdomf_new;
	
	/* alloc mem for new policy */
	tdomf_new = memget2(max_char);
	
	/* cycle through rules of all domains */
	temp = tdomf;
	while(1){
		/* get next rule */
		res = string_get_next_line(&temp);
		if (!res) break;
		
		/* is it a rule? */
		if (string_search_keyword_first(res, "allow_")){
			
			/* get rule type and params */
			temp2 = res;
			rule_type = string_get_next_word(&temp2);
			param1 = string_get_next_word(&temp2);
			param2 = string_get_next_word(&temp2);

			if (!path_is_tomld_dir(param1) && !path_is_tomld_dir(param2))
			{
				strcat2(&tdomf_new, res);
				strcat2(&tdomf_new, "\n");
			}
			
			free2(param1);
			free2(param2);
			free2(rule_type);
		}

		/* if not, then no check, add it anyway */
		else{
			strcat2(&tdomf_new, res);
			strcat2(&tdomf_new, "\n");
		}

		free2(res);
	}
	
	/* replace old policy with new one */
	free2(tdomf);
	tdomf = tdomf_new;
}


/* rehsape rules */
void domain_reshape_rules()
{
	sand_clock(0);
	domain_cleanup();
	
	domain_reshape_rules_recursive_dirs();

	sand_clock(0);
	domain_cleanup();
	
	domain_reshape_rules_wildcard_spec();
	
	sand_clock(0);
	domain_cleanup();

	domain_reshape_rules_create_double();

	sand_clock(0);
	domain_cleanup();

	domain_remove_tomld_dir();
}


/* update the change time entries in the domains whose rules or profile changed since last time
 * except my uid entries
 * i reset last changed time even if it is because of another domain's change,
 * because after switching the domain to enforcing mode,
 * the rules cannot be changed anymore (except by user request for temporary learning mode) */
void domain_update_change_time()
{
	char *d1, *d2, *name1, *name2, *res, *temp, *temp2, *temp3;
	char *tdomf_new;
	int flag_match, flag_change;
/*	char *res1, *res2, *ŧemp1;
	int i1, i2;*/
	
	/* backup policy exists yet? if not, then copy policy */
	if (!tdomf_bak2){
		/* copy policy to backup */
		strcpy2(&tdomf_bak2, tdomf);
		return;
	}
	else{
		/* policy files match? if not, then store it */
		if (!strcmp(tdomf_bak2, tdomf)) return;
	}

	
	/* if backup policy exists but it doesn't match to current policy,
	 * then update change times in current one and copy it to backup */
	tdomf_new = memget2(max_char);
	temp = tdomf;
	while(1){
		/* get next domain from backup */
		d1 = domain_get_next(&temp);
		if (!d1) break;

		/* this is a flag to see if there was any match in backup domain and current one */
		flag_match  = 0;
		flag_change = 0;

		/* get domain name */
		name1 = domain_get_name_full(d1);
		if (name1){
			
			temp2 = tdomf_bak2;
			while(1){
				/* get next current domain */
				d2 = domain_get_next(&temp2);
				if (!d2) break;
				/* get domain name */
				name2 = domain_get_name_full(d2);
				if (name2){
					
					/* compare backup domain to current one */
					if (!strcmp(name1, name2)){
						/* there was a match */
						flag_match = 1;

						/* compare backup domain's policy to current domain's policy */
						if (strcmp(d1, d2)){
							/* there is a difference */
							flag_change = 1;
						}
						
						/* jump out because there was a matching domain name */
						free2(name2);
						free2(d2);
						break;
					}
					free2(name2);
				}
				free2(d2);
			}
			
			/* if there was no match, then this is a new domain,
			 * so i add it to new policy */
			if (!flag_match){
				strcat2(&tdomf_new, d1);
				strcat2(&tdomf_new, "\n");
			}
			else{
				/* if there was a match and they are the same,
				 * then i also add the domain to new policy */
				if (!flag_change){
					strcat2(&tdomf_new, d1);
					strcat2(&tdomf_new, "\n");
				}
				else{
					/* if no match, then update current domain's policy by
					 * copying every rule except my uuid that i change before */
					temp3 = d1;
					/* copy headers, like domain name and use_profile */
					res = string_get_next_line(&temp3);
					strcat2(&tdomf_new, res);
					strcat2(&tdomf_new, "\n");
					free2(res);
					res = string_get_next_line(&temp3);
					strcat2(&tdomf_new, res);
					strcat2(&tdomf_new, "\n");
					free2(res);

					/* copy the rest of the rules */
					while(1){
						/* get next rule */
						res = string_get_next_line(&temp3);
						if (!res) break;

						/* check if rule is my unique id */
						if (string_search_keyword_first(res, myuid_change)){
							/* copy my unique id of change time with the new time */
							char *t = string_ltos(time(0));
							strcat2(&tdomf_new, myuid_change);
							strcat2(&tdomf_new, t);
							strcat2(&tdomf_new, "\n");
							free2(t);
						}
						else{
							/* copy rule to new policy */
							strcat2(&tdomf_new, res);
							strcat2(&tdomf_new, "\n");
						}
						
						free2(res);
					}
					strcat2(&tdomf_new, "\n");
				}
			}
			free2(name1);
		}
		free2(d1);
	}

	/* copy new policy to current one */
	free2(tdomf);
	tdomf = tdomf_new;

	/* copy new policy to backup */
	strcpy2(&tdomf_bak2, tdomf);
}


/* update the cpu time entries of all domains
 * this must be one of the last operation to run (it's in finish()),
 * because here i add together the summary of all cpu time values */
void domain_update_cpu_time_all()
{
	char *name, *ptime, *tdomf_new;
	char *res, *res2, *res3;
	char *temp, *temp2, *temp3;
	int flag_list_match, flag_rule_match, t;
	
	tdomf_new = memget2(max_char);
	
	/* update cpu times in all domains */
	temp = tdomf;
	while(1){
		/* get next domain */
		res = domain_get_next(&temp);
		if (!res) break;
		
		/* get domain name */
		name = domain_get_name_full(res);
		if (name){
			/* collect entry for domain from domain cpu time list plus */
			flag_list_match = 0;
			t = 0;
			temp2 = domain_cputime_list_plus;
			while(1){
				/* get next entry */
				res2 = string_get_next_line(&temp2);
				if (!res2) break;
				temp3 = res2;
				/* get second column (name) */
				res3 = string_get_next_wordn(&temp3, 1);
				if (res3){
					/* does entry match the domain (second column matches name)? */
					if (!strcmp(res3, name)){
						flag_list_match = 1;
						/* get third column (cpu time) */
						ptime = string_get_next_word(&temp3);
						/* convert it and add it */
						t += atoi(ptime);
						free2(ptime);
					}
					free2(res3);
				}
				free2(res2);
			}
			
			/* if match, then update my uid cpu time rule in domain 1 by 1 */
			if (flag_list_match){

				/* cycle through rules */
				flag_rule_match = 0;
				temp2 = res;
				while(1){
					/* get next rule */
					res2 = string_get_next_line(&temp2);
					if (!res2) break;
					
					/* is rule my uid cpu time entry? */
					if (string_search_keyword_first(res2, myuid_cputime)){
						flag_rule_match = 1;
						/* if so, then update it by adding together old and new value */
						/* read old cpu time value */
						ptime = string_get_number_last(res2);
						/* add them */
						t += atoi(ptime);
						free2(ptime);
						/* convert new cpu time value */
						ptime = string_itos(t);
						/* add my new uid rule to new domain policy */
						strcat2(&tdomf_new, myuid_cputime);
						strcat2(&tdomf_new, ptime);
						strcat2(&tdomf_new, "\n");
						free2(ptime);
					}
					else{
						/* add rule to new domain policy */
						strcat2(&tdomf_new, res2);
						strcat2(&tdomf_new, "\n");
					}
					
					free2(res2);
				}
				
				/* if there was no match, then create my uid of cpu time anyway */
				if (!flag_rule_match){
					/* convert new cpu time value */
					ptime = string_itos(t);
					/* add my new uid rule to new domain policy */
					strcat2(&tdomf_new, myuid_cputime);
					strcat2(&tdomf_new, ptime);
					strcat2(&tdomf_new, "\n");
					free2(ptime);
				}
			}

			/* if there was no match, then add all domain in one step */
			else{
				strcat2(&tdomf_new, res);
			}
			
			free2(name);
			strcat2(&tdomf_new, "\n");
		}
		free2(res);
	}
	
	free2(tdomf);
	tdomf = tdomf_new;
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


/* check signal of user request for temporary learning mode for domains with deny logs
 * this is to take max 1 hour or until only the next deny logs, whichever happens first */
void check_learn()
{
	static int t = 0;

	/* only in automatic mode */
	if (!opt_manual){

		if (!flag_learn2){
			/* check global var for temporary learning mode request */
			if (file_exist(tlearn)){
				
				int tlearnf_mod_time2;
				/* check modification time of learn file and read its content only if modified */
				tlearnf_mod_time2 = file_get_mod_time(tlearn);
				if (tlearnf_mod_time != tlearnf_mod_time2){
					char *f;
					/* store new time */
					tlearnf_mod_time = tlearnf_mod_time2;

					/* read 1 char from signal file */
					f = file_read(tlearn, 1);
					/* check if length is not null */
					if (strlen2(&f)){
						/* clear list */
						free2(opt_learn2); opt_learn2 = 0;
						/* is there a list for learn with domain pattern? */
						if (!strcmp(f, "2")){
							/* load list with domain pattern */
							opt_learn2 = file_read(tlearn_list, 0);
						}
						/* set starting time */
						t = mytime();
						color("* user request for temporary learning mode (max 1 hour) ", yellow);
						mytime_print_date(); newl();
						
						/* notification */
						notify("temporary learning mode (max 1 hour)");
						
						if (opt_learn2){
							char *temp6;
							color("  for domains matching patterns:", yellow);
							temp6 = opt_learn2;
							while(1){
								char *res6 = string_get_next_line(&temp6);
								if (!res6) break;
								color(" ", red);
								color(res6, red);
								free2(res6);
							}
							newl();
						}
						/* clear temporary learning mode flag file */
						file_write(tlearn, 0);
						/* set flag for temporary learning mode */
						flag_learn  = 1;
						flag_learn2 = 1;
						flag_learn3 = 1;
					}
					free2(f);
				}
			}
		}
		else{
			/* check if 1 hour passed yet */
			if ((mytime() - t) >= const_time_max_learn){
				/* clear flag */
				flag_learn  = 0;
				flag_learn2 = 0;
				color("* time ended for temporary learning mode ", yellow);
				mytime_print_date(); newl();
				
				/* notification */
				notify("time ended for temporary learning mode");
				/* switch back to enforcing mode */
				domain_set_enforce_old();
			}
		}

	}
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
	if (check_policy_change()){

		/* reshape rules */
		domain_reshape_rules();
		
		/* domain update change times */
		domain_update_change_time();
		
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
		tshellf2 = file_read(tshell, 0);
	}
	/* if not, get shell names from defined list */
	else{
		int i = 0;
		while(1){
			char *res = tshellf[i++];
			if (!res) break;
			strcpy2(&tshellf2, res);
			strcat2(&tshellf2, "\n");
		}
	}
	
	/* add shells to exceptions */
	if (strlen2(&tshellf2)){
		temp = tshellf2;
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
	
	/* add command line exceptions to the list */
	strcat2(&tprogs_exc, opt_nodomain2);

	/* sort exception list */
	if (tprogs_exc){
		res = string_sort_uniq_lines(tprogs_exc);
		free2(tprogs_exc);
		tprogs_exc = res;
	}

	/* sort program list */
	if (strlen2(&tprogs)){
		res = string_sort_uniq_lines(tprogs);
		free2(tprogs);
		tprogs = res;
	}

	/* initialize recursive dirs' depth and sub values */
	if (dirs_recursive){
		i = string_count_lines(dirs_recursive);
		if (i > 0){
			if (!dirs_recursive_depth) dirs_recursive_depth = memget_int(i+1);
			if (!dirs_recursive_sub)   dirs_recursive_sub   = memget_int(i+1);
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
			res = file_read(netf[i++], -1);
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
				if (res3){
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
			error("error: cannot open /proc/ directory\n");
			free2(netf3);
			myexit(1);
		}
		/* cycle through dirs in /proc */
		while((mydir_entry = readdir(mydir))) {
			char *myprog, *mypid;
			/* get my pid number from dir name in /proc */
			mypid = memget2(max_num);
			strcpy2(&mypid, mydir_entry->d_name);
			/* does it contain only numbers meaning they are pids? */
			if (string_is_number(mypid)) {
				char *mydir_name;
				/* create dirname like /proc/pid/exe */
				mydir_name = memget2(max_char);
				strcpy2(&mydir_name, "/proc/");
				strcat2(&mydir_name, mypid);
				strcat2(&mydir_name, "/exe");
				/* resolv the link pointing from the exe name */
				myprog = path_link_read(mydir_name);
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
							mysock = path_link_read(mydir_name2);
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
	char *res, *temp, *temp2, *sd, *sr;
	int d = 0, r = 0;
	char *mypid, *mydir_name = 0, *pstat, *ptime;
	int t, t2;
	float tt;
	/* converting jiffies to second, this is from manpage of proc */
	int jiffies_per_second=sysconf(_SC_CLK_TCK);
	
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
	
	/* print stat about the min, max and average time of check cycle */
	printf("cycle times min/avg/max %.2f/%.2f/%.2f sec\n", time_min_cycle, time_avg_cycle / (float)(time_avg_cycle_counter), time_max_cycle);
	fflush(stdout);

	/* create dirname like /proc/pid/stat */
	mypid = string_itos(getpid());
	strcpy2(&mydir_name, "/proc/");
	strcat2(&mydir_name, mypid);
	strcat2(&mydir_name, "/stat");
	/* read process stat value */
	pstat = file_read(mydir_name, -1);

	/* get all utime, stime, cutime and cstime of process in seconds */
	t = 0;
	temp = pstat;
	ptime = string_get_next_wordn(&temp, 13);
	t += atoi(ptime);
	free2(ptime);
	temp = pstat;
	ptime = string_get_next_wordn(&temp, 14);
	t += atoi(ptime);
	free2(ptime);
	temp = pstat;
	ptime = string_get_next_wordn(&temp, 15);
	t += atoi(ptime);
	free2(ptime);
	temp = pstat;
	ptime = string_get_next_wordn(&temp, 16);
	t += atoi(ptime);
	free2(ptime);
	/* convert clock ticks to seconds */
	t = t / jiffies_per_second;
	
	/* get uptime of process */
	temp = pstat;
	/* get starttime of process in jiffies */
	ptime = string_get_next_wordn(&temp, 21);
	/* calculate process uptime in sec */
	t2 = sys_get_uptime() - atoi(ptime) / jiffies_per_second;
	free2(ptime);
	free2(pstat);
	
	/* calculate cpu usage of process in percentage
	 * process_cpu_time / process_uptime * 100 */
	tt = 0;
	if (t2) tt = (float)(t) * 100 / (float)(t2);
	printf("used cpu %.2f %%, ", tt);
	fflush(stdout);
	

	/* create dirname like /proc/pid/status */
	strcpy2(&mydir_name, "/proc/");
	strcat2(&mydir_name, mypid);
	strcat2(&mydir_name, "/status");
	/* read process stat value */
	pstat = file_read(mydir_name, -1);
	
	/* get virtual and resident peak of memory usage */
	/* get VM peak */
	temp = pstat;
	string_jump_next_linen(&temp, 10);
	temp2 = temp;
	res = string_get_next_wordn(&temp2, 1);
	t = atoi(res);
	free2(res);
	/* get RSS peak */
	temp = pstat;
	string_jump_next_linen(&temp, 13);
	temp2 = temp;
	res = string_get_next_wordn(&temp2, 1);
	t2 = atoi(res);
	free2(res);
	
	/* calculate values to MB and print them */
	printf("vm peak %.1f MB, rss peak %.1f MB\n", (float)(t) / 1024, (float)(t2) / 1024);
	fflush(stdout);

	free2(mydir_name);
	free2(mypid);
	free2(pstat);
}


/* save configs and finish program */
void finish()
{
	/* signals are pointed to this finish() function
	 * so this code is to avoid race condition if tomld gets more
	 * external signals to terminate and so to run finsih() */
	static int flag_safe = 0;
	if (!flag_safe){
		flag_safe = 1;

		newl();

		if (flag_safe){
			/* run check for the last time */
			check();

			/* switch all old domains (manual mode) or temporary learning mode domains (auto mode)
			 * to enforcing mode before exiting */
			domain_set_enforce_old();

			/* update cpu times of all domains in domain policy */
			/* this must be one of the last operation to run, because here
			 * i add together the summary of all cpu time values */
			domain_update_cpu_time_all();

			/* save configs to disk */
			save();
			reload();
		}
		else if (flag_firstrun) color("* haven't finished to run at least once\n", red);

		/* clear temporary learning mode flag file on exit for security */
		file_write(tlearn, 0);
		
		/* print statistics */
		statistics();
		
		/* print end time */
		color("ended ", clr); mytime_print_date(); newl(); newl();

		/* exit and free all my pointers */
		myexit(0);
	}
}


/* some initializations */
void myinit()
{
	char *res;

	/* store start time */
	t_start = mytime();
	
	/* create my uids */
	strcpy2(&myuid_create, myuid);
	strcpy2(&myuid_change, myuid);
	strcpy2(&myuid_cputime, myuid);
	strcat2(&myuid_create, "/create_time/");
	strcat2(&myuid_change, "/change_time/");
	strcat2(&myuid_cputime, "/cpu_time/");
	
	/* create tomld paths from its vars */
	res = path_get_dir(tpid);
	string_add_line_uniq(&tomld_path, res); free2(res);
	res = path_get_dir(tmark);
	string_add_line_uniq(&tomld_path, res); free2(res);
	res = path_get_dir(tlearn);
	string_add_line_uniq(&tomld_path, res); free2(res);
	res = path_get_dir(tlearn_list);
	string_add_line_uniq(&tomld_path, res); free2(res);
	res = path_get_dir(tspec);
	string_add_line_uniq(&tomld_path, res); free2(res);
}


/* ----------------------------------- */
/* ------------ MAIN PART ------------ */
/* ----------------------------------- */

int main(int argc, char **argv)
{
	float t, t2, t3;

	/* some initializations */
	myinit();
	
	/* check command line options */
	check_options(argc, argv);

	/* check and load config */
	check_config();

	/* if --notify option is on and user is non-root, then
	 * check for notification messages stored in tlog2 file
	 * and send them to a notification daemon if any
	 * this is an infinite cycle */
	check_notify();
	/* clear log */
	file_write(tlog2, 0);


	/* check if i am root */
	if (getuid()) { error("error: root privilege needed\n"); myexit(1); }

	/* create exception list from program names and available shells */
	check_exceptions();

	/* check tomoyo status */
	check_tomoyo();


	/* on --info, print domain information */
	if (opt_info){
		domain_info(opt_info2);
		myexit(0);
	}

	/* check already running instance of the program */
	check_instance();


	/* ---------------- */
	/* ----- INIT ----- */
	/* ---------------- */

	/* on --log, redirect stderr and stdout to file */
	if (opt_log) file_std_redir(opt_log2);

	/* print start time */
	if (!opt_info && !opt_remove && !opt_help && !opt_version && !opt_learn_all){
		color("started ", clr); mytime_print_date(); newl();
	}

	/* print version info */
	color("tomld (tomoyo learning daemon) ", clr); color(ver, clr); newl();
	
	/* create profile.conf and manager.conf files */
	create_prof();

	/* check if mta binary exists */
	if (opt_mail){
		if (file_exist(mail_mta)){
			if (!opt_info && !opt_remove && !opt_help && !opt_version && !opt_learn_all){
				color("* mail requested to following recipients: ", yellow);
				color(mail_users, yellow);
				newl();
			}
		}
		else{
			color("* mail requested but binary ", red);
			color(mail_mta, red);
			color(" missing\n", red);
			/* clear mail option because of missing mta binary */
			opt_mail = 0;
		}
	}
	
	/* this is a single tomld process here and
	 * on --learn, request temporary learning mode for this process */
	if (opt_learn){
		/* is there any pattern for learn? */
		if (!opt_learn2){
			/* set temporary learning mode flag */
			file_write(tlearn, "1");
		}
		else{
			/* set temporary learning mode flag with list */
			file_write(tlearn, "2");
			file_write(tlearn_list, opt_learn2);
		}
	}
	else{
		/* clear temporary learning mode flag file on start */
		file_write(tlearn, 0);
	}

	/* on --learn-all, switch all domains to learning mode */
	if (opt_learn_all){
		color("* switching all domains to learning mode on demand\n", red);
		if (!choice("are you sure?")){
			color("no change\n", green);
			myexit(0);
		}
		backup();
		color("policy file backups created\n", green);
		domain_set_learn_all();
		color("all domains switched back to learning mode\n", red);
		myexit(0);
	}

	/* on --remove, remove domain */
	if (opt_remove){
		domain_remove(opt_remove2);
		myexit(0);
	}

	/* on --clear, empty configuration files */
	if (!opt_reset && opt_clear){
		color("* clearing domain configurations on demand\n", red);
		if (!choice("are you sure?")){
			color("no change\n", green);
			myexit(0);
		}
		backup();
		color("policy file backups created\n", green);
		clear();
		color("* configuration cleared\n", red);
		myexit(0);
	}

	/* on --reset option, create backup of config files */
	if (opt_reset){
		color("* resetting domain configurations on demand\n", red);
		if (!choice("are you sure?")){
			color("no change\n", green);
			myexit(0);
		}
		backup();
		color("policy file backups created\n", green);
	}

	/* create new empty policy files if missing or if --reset switch is on */
	if (!file_exist(tdom) || !file_exist(texc) || opt_reset) clear();

	/* store negatív reference time for check() function to make check() run at least once */	
	t  = -const_time_check;
	t2 = 0;

	while(1){
		int tlogf_mod_time2;

		/* check running processes */
		check_processes();
		/* check signal of user request for learning mode */
		check_learn();
		/* check modification time of log file and read its content only if modified */
		tlogf_mod_time2 = file_get_mod_time(tlog);

		/* run policy check from time to time
		 * or at once if temporary learning mode is requested,
		 * or also at once if mod time of syslog changed and tomld is having temporary learning mode
		 * (only with temp learn mode or else a process could DoS the tomld process),
		 * and not 30 sec later */
		if ((mytime() - t) >= const_time_check || flag_learn3 ||
			(flag_learn && tlogf_mod_time != tlogf_mod_time2)){

			flag_safe = 0;
			flag_learn3 = 0;

			/* store time for speed comparision */			
			t3 = mytime();

			/* manage policy and rules */
			check();
			
			/* store longest time of cycle */
			t3 = mytime() - t3;
			if (time_max_cycle < t3) time_max_cycle = t3;
			if (time_min_cycle > t3) time_min_cycle = t3;
			time_avg_cycle += t3;
			time_avg_cycle_counter++;
			
			/* run only once */
			if (flag_firstrun){
				flag_firstrun = 0;
				color("* first whole running cycle took ", green);
				printf("%.2fs\n", t3); fflush(stdout);
				if (!opt_once){
					color("(press q to quit)\n", red);
				}
				/* setup signal handler here and exit on interrupt signals */
				signal (SIGINT,  finish);
				signal (SIGTERM, finish);
				/* it gets SIGQUIT on machine reboot or shutdown */
				signal (SIGQUIT, finish);
			}

			/* now it's safe to enforce mode and save config on interrupt, cause check() finished running */
			flag_safe = 1;
			
			/* exit if --once option is on */
			if (opt_once) break;
			
			/* reset time */
			t = mytime();
		}
		
		/* sleep some */
		usleep(2000000);
		/* exit if 'q' key is pressed */
		if (key_get() == 'q') break;

		/* save configs to disk from time to time */
		if ((mytime() - t2) >= const_time_save){
			/* save configs */
			save();

			/* reset time */
			t2 = mytime();
		}
	}

	finish();
	
	return 0;
}

