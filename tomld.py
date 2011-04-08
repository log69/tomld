#!/usr/bin/python
# coding=utf8


# ----------------------------------------------------------------------
# ----------- tomld (tomoyo learning daemon) ---------------------------
# ----------- Copyright (C) 2011 Andras Horvath, Hungary ---------------
# ----------- http://log69.com -----------------------------------------
# ----------------------------------------------------------------------
#
#	This program is free software; you can redistribute it and/or
#	modify it under the terms of the GNU General Public License as
#	published by the Free Software Foundation; either version 3 of the
#	License, or	(at your option) any later version.
#
#	This program is distributed in the hope that it will be useful,	but
#	WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#	See the GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with this program. If not, see <http://www.gnu.org/licenses/>.
#
# ----------------------------------------------------------------------
#
# changelog:
# -----------
# 07/04/2011 - tomld v0.28 - change quit method from ctrl+c to q key
#                          - bugfix: do not turn on enforcing mode for newly created domains
#                          - add compatibility to tomoyo version 2.3
# 05/04/2011 - tomld v0.27 - rewrite domain cleanup function
#                          - speed up the new domain cleanup function by skipping rules reading libs
#                          - add feature: check rules only if they changed and avoid unnecessary work
#                          - improve info() function: show main domains and subdomains with different colors
#                          - major bugfixes
# 03/04/2011 - tomld v0.26 - improve domain cleanup function
#                          - improve info function
#                          - bugfixes
#                          - add --learn switch to turn learning mode back for all domains on mistake
# 02/04/2011 - tomld v0.25 - more major bugfixes
#                          - add sand clock to see when check rutin is working, so we can stop it while sleeping
#                          - some code cleanup
#                          - speed up compare rutins a bit
# 31/03/2011 - tomld v0.24 - major bugfixes
#                          - improve domain cleanup function by making the rules more unique
# 29/03/2011 - tomld v0.23 - add feature to try to detect temporary names and wildcard them
#                          - major bugfixes
# 28/03/2011 - tomld v0.22 - speed up info and remove functions
#                          - add full recursive directory subtitution with wildcards
#                          - several bugfix
# 27/03/2011 - tomld v0.21 - more bugfixes and code cleanup
#                          - change in structure: from now allow_mkdir will cause the file's parent dir to be wildcarded too
#                          - change in structure: i created an exception list for the dirs, so the policy gets a bit tighter with this
#                            exception list contains now = "/etc", "/home/\*/"
# 26/03/2011 - tomld v0.20 - minor bugfixes
#                          - some code cleanup
# 25/03/2011 - tomld v0.19 - create policy file backups only with --reset or --clear switches
#                          - print info when backups are created
#                          - expand documentation
#                          - handle error message type "domain not defined" in log
# 24/03/2011 - tomld v0.18 - create profile.conf file on startup if missing
#                          - set maximum accept entry value in profile.conf to a predefined one
#                          - add --once switch to quit after first cycle immediately (might be useful for scripts)
#                          - don't print confirmation text at the end of log messages with --yes on
#                          - don't print sleeping period with --once on
# 23/03/2011 - tomld v0.17 - add --keep switch to run tomld without turning learning mode domains into enforcing mode
#                          - add --recursive switch to mark subdirs of these dirs fully wildcarded (usable for samba shares for example)
#                          - on rules with allow_create entries, create the same with allow_unlink and allow_read/write too
#                          - ask for confirmation on adding system logs to rules (--yes switch is usable here to bypass this)
# 22/03/2011 - tomld v0.16 - add --remove switch to remove domains by a pattern on demand
#                          - add --yes switch for auto confirmation
#                          - function for --info switch rewritten to speed up search
#                          - remove rules and domains marked as (deleted) on startup
#                            (thanks to Ritesh Raj Sarraf)
#                          - debian kernel has Tomoyo enabled already, fix package check (only tomoyo-tools is needed)
#                            (thanks to Ritesh Raj Sarraf)
#                          - bugfix: variables of config files were not initialized
# 21/03/2011 - tomld v0.15 - remove disabled mode domains automatically to speed up the things
#                          - change directory parameters' handling in rule reshape code to speed up more
#                          - show statistics about active domains and rules on exit
# 20/03/2011 - tomld v0.14 - bugfix: only first log message was converted to rule
# 18/03/2011 - tomld v0.13 - additional major bugfixes
# 18/03/2011 - tomld v0.12 - add --info switch to print the specified domain's rules and bugfixes
# 17/03/2011 - tomld v0.11 - print domain info only once and print changes continously
# 17/03/2011 - tomld v0.10 - put allow_rename on the wildcard list too (same as allow_create) and other major changes
# 16/03/2011 - tomld v0.09 - only old domains with profile 1-2 will be turned into enforcing mode on exit
# 15/03/2011 - tomld v0.08 - tested version with minor bugfixes
# 15/03/2011 - tomld v0.07 - create wildcard for newly created files and libfile-0.1.2.so.3.4 library version numbers
# 14/03/2011 - tomld v0.06 - major bugfixes
# 12/03/2011 - tomld v0.05 - first fully working version with policy reshape
# 09/03/2011 - tomld v0.04 - get access denied logs and add them to policy
# 04/03/2011 - tomld v0.03 - manage and check policy and rules
# 03/03/2011 - tomld v0.02 - discover which running processes send or recieve network packets
# 28/02/2011 - tomld v0.01 - basic program structure
# 27/02/2011 - project start


# flow chart:
# ------------
#
#	1)	init
#	2)	search for new processes using network
#	3)	sleep short
#	3)	while counter-- goto 2 (means long sleep for check)
#	4)	load policy
#	5)	check prolicy for domain and all its subdomains
#		if disabled mode
#			turn on learning mode
#		if learning mode
#			no change
#		if permissive mode
#			no change
#		if enforcing mode
#			no change
#	6)	get recent log
#	7)	convert logs to rules and add them to policy (confirm)
#	8)	switch back domains with access deny from enforcing to learning mode until exit
#	9)	reshape policy rules:
#			domain config cleanup (sort and unique)
#			wildcard subdirs of recursive dirs (if any)
#			collect dir names of create rules (that are not in exception list)
#			collect dir names to mkdir (that are not in exception list)
#			wildcard formerly collected dir names (only leaf file or dir name)
#			wildcard collected mkdir names' leaf and one parent dir too
#			wildcard library files version numbers
#			wildcard pid number in /proc/number/ dirs
#			wildcard user name in /home/user/ dirs
#			domain config cleanup (sort and unique)
#			wildcard files and dirs with changing names that are not wildcarded yet
#				(this means dirs in the exception list)
#			domain config cleanup (sort and unique)
#			readd every create rule as unlink, read/write etc. too
#			domain config cleanup (sort and unique)
#	10)	save policy
#	11) goto 2
#	12) on exit, turn all old domains with profile 1-2 into enforcing mode



import os, sys
import time, re
import platform
import termios


# **************************
# ******* VARIABLES ********
# **************************

# program version
ver = "0.27"

# home dir
home = "/home"

# policy check interval in seconds
global count; count = 10

# max entries variable in profile config
global maxe; maxe = 10000

# tomoyo kernel parameter
global tkern; tkern = "security=tomoyo"
# tomoyo package names (debian, ubuntu)
global tpak1d; tpak1d = "linux-patch-tomoyo1.7"
global tpak2d; tpak2d = "tomoyo-tools"

# tomoyo vars and files
global tdomf; tdomf   = ""
global texcf; texcf   = ""
global tprof; tprof   = ""
global tdir;  tdir    = "/etc/tomoyo"
global tdom;  tdom    = "/etc/tomoyo/domain_policy.conf"
global texc;  texc    = "/etc/tomoyo/exception_policy.conf"
global tpro;  tpro    = "/etc/tomoyo/profile.conf"
global tinit
global tload
global tsave
global tinit1; tinit1 = "/usr/lib/tomoyo/tomoyo_init_policy"
global tload1; tload1 = "/usr/sbin/tomoyo-loadpolicy"
global tsave1; tsave1 = "/usr/sbin/tomoyo-savepolicy"

# system log
global tlog;  tlog  = "/var/log/syslog"

# backup conf for checking change of rules
global tdomf_bak; tdomf_bak = ""

# this stores the kernel time of last line of the system log
# to identify it and make sure not to read it twice
global tmarkf
global tmark; tmark = "/var/local/tomld.logmark"
# tomld pid file
global pidf;  pidf  = "/var/run/tomld.pid"

# options
global opt_color;		opt_color		= 0
global opt_learn;		opt_learn		= 0
global opt_clear;		opt_clear		= 0
global opt_reset;		opt_reset		= 0
global opt_info;		opt_info		= 0
global opt_info2;		opt_info2		= ""
global opt_remove;		opt_remove		= 0
global opt_remove2;		opt_remove2		= ""
global opt_yes;			opt_yes			= 0
global opt_keep;		opt_keep		= 0
global opt_recursive;	opt_recursive	= 0
global opt_once;		opt_once		= 0

global flag_reset;  flag_reset  = 0
global flag_check;  flag_check  = 0
global flag_check2; flag_check2 = 0
global flag_check3; flag_check3 = 0
global flag_firstrun; flag_firstrun = 1
global flag_clock;	flag_clock = 0
global flag_safe;   flag_safe   = 0

# colors
global gray;   gray		= "\033[37;48m"
global cyan;   cyan		= "\033[36;48m"
global purple; purple	= "\033[35;48m"
global blue;   blue		= "\033[34;48m"
global green;  green	= "\033[32;48m"
global red;    red		= "\033[31;48m"
global yellow; yellow	= "\033[33;48m"
global bold;   bold		= "\033[1m"
global clr;    clr		= "\033[0m"

# terminal
global term_fd, term_old, term_new
global term_flag; term_flag = 0


# all binaries with domains
global progs; progs = []
# binaries only with newly created learning mode
# these won't have enforcing mode turned on on exit
global progl; progl = []
# variable to store the time to measure speed of run
global speed; speed = 0

# supported platforms
supp = ["debian 6", "debian wheezy/sid", "ubuntu 10.10", "ubuntu 11.04"]

# this will contain the dirs to be fully wildcarded resursively with --recursive switch on
global specr;  specr  = []
# this will show the full dir depths in numbers to all recursive dirs
global specr2_count; specr2_count = []

# special dirs - the leaf dirs or files get wildcarded in any subdirs of them
global spec; spec = [home + "/", "/usr/share/", "/etc/fonts/", "/var/cache/"]

# just like the above, but these are the exception dirs
# exact match needs on these dirs for the leaf dir or file not to get wildcarded
global spec_ex; spec_ex = ["/etc/", home + "/\*/", "/root/"]

# exception programs - these programs will never get a standalone enforcing mode domain
global spec_prog; spec_prog = ["/bin/sh", "/bin/bash", "/bin/dash", "/usr/sbin/sshd"]



# **************************
# ******* FUNCTIONS ********
# **************************

# print version
def version():
	print "tomld (tomoyo learning daemon)", ver
	print "Copyright (C) 2011 Andras Horvath"
	print "E-mail: mail@log69.com - suggestions & feedbacks are welcome"
	print "URL: http://log69.com - the official site"
	print
	print "LICENSE:"
	print "This program is free software; you can redistribute it and/or modify it",
	print "under the terms of the GNU General Public License as published by the",
	print "Free Software Foundation; either version 3 of the License,",
	print "or (at your option) any later version."
	print
	print "This program is distributed in the hope that it will be useful,",
	print "but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY",
	print "or FITNESS FOR A PARTICULAR PURPOSE. ",
	print "See the GNU General Public License for more details."
	print
	print "You should have received a copy of the GNU General Public License along with this program. ",
	print "If not, see <http://www.gnu.org/licenses/>."
	print


# print help
def help():
	print "DESCRIPTION:"
	print "tomld (tomoyo learning daemon) is an extension to the Tomoyo security module",
	print "to ease and automate the setup process of a MAC (Mandatory Access Control) aware system",
	print "to increase security of the system by closing applications and services into a domain with their rules.",
	print "It helps the user harden his system easily.",
	print "Starts domains in learning mode, collects rules, then later changes these rules and enforces the policy."
	print
	print "Tomoyo module contains already the learning ability to automatically setup rules",
	print "while the processes are running in learning mode. Only, there are continously changing",
	print "files and directory names that the processes use, so those have to be replaced with wildcarded rules.",
	print "This needs a rather advanced knowledge of the system."
	print
	print "This tool tries to fully automate the MAC setup, hoping the average users",
	print "would be able to use a much more secure system on their own with minimal efforts."
	print
	print "Tomoyo security module is part of the mainline Linux kernel since version 2.6.30.",
	print "Currently this solution targets Linux systems only."
	print
	print "*TOMOYO(r) is a registered trademark of NTT DATA Corporation"
	print
	print "LINKS:"
	print "http://tomoyo.sourceforge.jp"
	print "http://tomoyo.sourceforge.jp/documentation.html.en"
	print "http://tomoyo.sourceforge.jp/2.2/index.html.en"
	print "http://en.wikipedia.org/wiki/Mandatory_access_control"
	print
	print "USAGE: tomld.py [options] [executables]"
	print
	print "The following options are supported:"
	print "    -h   --help             print this help"
	print "    -v   --version          print version information"
	print "    -c   --color            colorized output"
	print "         --clear            clear domain configurations"
	print "         --reset            reinitialize domain configurations"
	print "                            (all previously learnt rules will be backed up)"
	print "    -i   --info   [pattern] print domains' rules by pattern"
	print "                            (without pattern, print a list of main domains)"
	print "    -l   --learn            turn learning mode back for all domains"
	print "                            (this is not advised, only for correction purposes)"
	print "    -r   --remove [pattern] remove domains by pattern"
	print "         --yes              auto confirm with yes"
	print "    -k   --keep             don't change domain's mode for this run"
	print "                            (learning mode domains will stay so on exit)"
	print "    -R   --recursive [dirs] replace subdirs of dirs in rules with wildcards"
	print "    -1   --once             quit after first cycle"
	print "                            (might be useful for scripts)"
	print
	print "*executables are additonal programs to create domains for"
	print
	print "supported platforms are:"
	for i in supp: print "  ", i
	print
	print
	print "REMARKS:"
	print "- it is written in python, tested with version 2.6"
	print "- the program assumes to be run in a fully trusted environment"
	print "- processes in the domains should run in learning mode for quite some time to collect",
	print "all the necessary rules to avoid malfunctioning by access deny later"
	print "- before reboot, the rules should be stored manually by running \"tomoyo-savepolicy\"",
	print "or \"tomld -k -1\" as root if there are still domains in learning mode,",
	print "because Tomoyo forgets the learned rules on system restart"
	print "- enforcing mode should never be switched back off for security reason"
	print "- exception domains means programs with no rules at all"
	print "- programs should be used in a maximum possible way during the learning phase",
	print "to force a rule for all kinds of behave"
	print "- on --reset switch the config files will be backed up and the former log entries",
	print "still won't be considered, so it really means a new start"
	print "- the program can be stopped any time by pressing q whereafter all old domains (learning and permissive)",
	print "are turned into enforcing mode"
	print "- tomld files: " + tmark + " (this contains a mark to identify the end of the recently read message logs),",
	print pidf + " (pid file to avoid multpiple instances of the program to be running at the same time)"
	print "- in case a software or its settings change with a new version, the rules can be regenerated easily"
	print "- the running processes need to be restarted on newly created domains"
	print
	print "HOW TO USE:"
	print "- download, move it to a directory that root owns only, change its owner to root,",
	print "change writeable only by root, make it executable, and run it as root"
	print "- run it the 1st time, now it will offer to install all missing packages and setup grub with kernel parameter"
	print "- system reboot is necessary now"
	print "- run it the 2nd time, now it will create domains for the processes automatically with learning mode"
	print "- stop it now, cause the domains will stay in learning mode and the tomoyo module will collect all rules"
	print "- reboot the system or at least all the services and applications that have domains now"
	print "- hours or days later let's run it the 3rd time, now the access denied logs will be converted to rules,",
	print "and on exit all remaining domains will be turned into enforcing mode"
	print


# print sand clock
def sand_clock(dot = 0):
	global flag_firstrun
	global flag_clock
	if dot == 1:
		if not flag_firstrun:
			sys.stdout.write(".")
			flag_clock = 0
	elif dot:
		if not flag_firstrun:
			sys.stdout.write("+")
			flag_clock = 0
	else:
		c = flag_clock % 4
		if c == 0: sys.stdout.write("-\b")
		if c == 1: sys.stdout.write("\\\b")
		if c == 2: sys.stdout.write("|\b")
		if c == 3: sys.stdout.write("/\b")
		flag_clock += 1
	sys.stdout.flush()


# sort and unique a list
def uniq(list):
	l = len(list)
	if l > 1:
		list.sort()
		c = 0
		while c < l-1:
			if list[c] == list [c+1]:
				del list[c+1]
				c-=1; l-=1
			c+=1


# compare paths containing \* wildcard
def compare_dirs(d1, d2):
	if (not d1) or (not d2): return 0
	e1 = d1.split("/")
	e2 = d2.split("/")
	l1 = len(e1)
	l2 = len(e2)
	if l1 == l2:
		for i in range(0, l1):
			c1 = e1[i]
			c2 = e2[i]
			if (not c1 == "\*") and (not c2 == "\*") and (not c1 == c2):
				return 0
		return 1
	return 0


# compare paths containing \* wildcard
# but recursively, only the beginning of the dir must match
# d1 must contain the beginning of d2 (d2 is specr)
def compare_recursive(d1, d2):
	if (not d1) or (not d2): return ""
	dd1 = re.search("^.*/", d1, re.M)
	if not dd1: return ""
	dd2 = re.search("^.*/", d2, re.M)
	if not dd2: return ""
	e1 = dd1.group().split("/")
	e2 = dd2.group().split("/")
	l1 = len(e1)-1
	l2 = len(e2)-1
	if l1 < l2: return ""
	# compare the dirs
	flag = 0
	for i in range(0, l2):
		c1 = e1[i]
		c2 = e2[i]
		if (not c1 == "\*") and (not c2 == "\*") and (not c1 == c2):
			return ""

	return d2


# compare names containing only 1 \* wildcard anywhere
# returns 1 also if both of them are null
def compare_names(d1, d2):
	if (not d1) and (not d2): return 1
	if (not d1) or  (not d2): return 0
	r1 = re.findall("\*", d1)
	r2 = re.findall("\*", d2)
	if (not r1) and (not r2) and (not d1 == d2): return 0
	if r1:
		if len(r1) > 1: return 0
		e1 = re.sub("\\\\\*", "*", d1)
		d1 = e1
	if r2:
		if len(r2) > 1: return 0
		e2 = re.sub("\\\\\*", "*", d2)
		d2 = e2
	# the length cannot be zero here because it's checked already
	l1 = len(d1)
	l2 = len(d2)
	
	# first name contains wildcard
	if r1 and (not r2):
		w1 = re.search("^[^\*]*", d1, re.M).group()
		w2 = re.search("[^\*]*$", d1, re.M).group()
		wl1 = len(w1)
		wl2 = len(w2)
		if not (l2 >= wl1 + wl2): return 0
		if not (w1 == d2[:wl1] and w2 == d2[l2-wl2:l2]): return 0
		return 1
	
	# second name contains wildcard
	if (not r1) and r2:
		w1 = re.search("^[^\*]*", d2, re.M).group()
		w2 = re.search("[^\*]*$", d2, re.M).group()
		wl1 = len(w1)
		wl2 = len(w2)
		# first word's char length must be more or equal than the other's without wildcard
		if not (l1 >= wl1 + wl2): return 0
		if not (w1 == d1[:wl1] and w2 == d1[l1-wl2:l1]): return 0
		return 1
	
	# both names contains wildcard
	if r1 and r2:
		w1 = re.search("^[^\*]*", d1, re.M).group()
		w2 = re.search("[^\*]*$", d1, re.M).group()
		w3 = re.search("^[^\*]*", d2, re.M).group()
		w4 = re.search("[^\*]*$", d2, re.M).group()
		wl1 = len(w1)
		wl2 = len(w2)
		wl3 = len(w3)
		wl4 = len(w4)
		wd1 = wl1
		wd2 = wl2
		# compare the less chars beside the wildcard from both sides			
		if wl1 > wl3: wd1 = wl3
		if wl2 > wl4: wd2 = wl4
		if not (d1[:wd1] == d2[:wd1] and d1[l1-wd2:l1] == d2[l2-wd2:l2]): return 0
		return 1
	
	return 1



# compare rules containing \* wildcard anywhere
def compare_rules(d1, d2):
	if (not d1) or (not d2): return 0
	e1 = d1.split()
	e2 = d2.split()
	l1 = len(e1)
	l2 = len(e2)
	# fail if number of parameters differ
	if not l1 == l2: return 0
	# fail if rule type differs
	if not e1[0] == e2[0]: return 0
	# compare the paths only
	for i in range(1, l1):
		f1 = e1[i]
		f2 = e2[i]
		r1 = re.search("\*", f1)
		r2 = re.search("\*", f2)
		if (not r1) and (not r2) and (not f1 == f2):
			return 0
		elif r1 or r2:
			ee1 = f1.split("/")
			ee2 = f2.split("/")
			ll1 = len(ee1)-1
			ll2 = len(ee2)-1
			if not ll1 == ll2: return 0
			# compare the dir and file names between "/" char
			for i2 in range(0, ll1+1):
				if not compare_names(ee1[i2], ee2[i2]): return 0

	return 1


# check if package is installed (debian)
def package_d(name):
	# debian or ubuntu
	pl = platform.linux_distribution()[0].lower()
	if pl == "debian" or pl == "ubuntu":
		f = "/var/lib/dpkg/status"
		if os.path.isfile(f):
			try: f2 = open(f).read()
			except: color("error: cannot open file " + f2, red); myexit(1)
			p = re.search("^Package: " + re.escape(name) + "$\n^Status: install ok installed$", f2, re.M)
			if p: return 1

	return 0
	

# get absolute pathname from a name
# first the current directory is checked, then the path list on the path env variable
def which(file):
	f2 = os.path.realpath(file)
	if os.path.isfile(f2): return f2
	else:
		# get current dir
		p2 = [os.path.realpath(".")]
		# get PATH env dir
		p = os.getenv("PATH").split(":")
		if p:
			# add dirs
			for i in p:
				if i: p2.append(i)
		# search fir file in dirs
		for i in p2:
			if os.path.isdir(i):
				for f in os.listdir(i):
					if file == f:
						f2 = os.path.realpath(os.path.join(i, f))
						if os.path.isfile(f2): return f2

	return ""


# print colored text
def color(text, col = "", newline = 0):
	if opt_color:
		if col == "": t = text
		else: t = col + text + clr
		if newline == 0: print t
		# print without new line
		else:
			sys.stdout.write(t)
			sys.stdout.flush()
	else:
		if newline == 0:
			print text
		else:
			sys.stdout.write(text)
			sys.stdout.flush()


# print error message on corrupt config file
# normally this should never run
def error_conf(num):
	color("error " + num + ": config file is corrupt", red); myexit(1)


# choice
def choice(text):
	if opt_yes:
		print text + " [y/N] y"
		sys.stdout.flush()
		return 1
	else:
		print text + " [y/N]",
		sys.stdout.flush()
		try: c = raw_input().lower()
		except: print; myexit(1)
		yes = ["yes", "ye", "y"]
		if c in yes: return 1
	return 0


# load config files
# kernel memory --> variables
def load():
	global tdomf
	global texcf

	# load config from memory to variables
	try: tdomf = os.popen(tsave + " d -").read()
	except: color("error: cannot load domain policy from memory", red); myexit(1)
	try: texcf = os.popen(tsave + " e -").read()
	except: color("error: cannot load exception policy from memory", red); myexit(1)

	# remove disabled mode entries so runtime will be faster
	s = re.sub(re.compile("^<kernel>.+$\n+use_profile +0 *$\n+", re.M), "", tdomf)
	# remove deleted entries too
	s2 = re.findall("^.*\(deleted\)$", tdomf, re.M)
	if s2:
		for i in s2:
			# domain is marked as "(deleted)"
			if re.search("^<kernel>", i, re.M):
				tdomf2 = tdomf + "\n<kernel>"
				r2 = re.findall(re.escape(i) + "\n.*?^(?=<kernel>)", tdomf2, re.M + re.I + re.DOTALL)
				if r2:
					for i2 in r2:
						# remove domain from config
						r6 = re.sub(re.escape(r5), "", s, re.M)
						s = r6
			# rule is marked with "(deleted)"
			else:
				s3 = re.sub(re.escape(i), "", s)
				s = s3
	tdomf = s
	# remove quota_exceeded entries too
	s = re.sub(re.compile("^quota_exceeded *$\n", re.M), "", tdomf)

	tdomf = s


# save config files
# variables --> disk --> kernel memory
def save():
	global tdomf
	# remove disabled mode entries so runtime will be faster
	s = re.sub(re.compile("^<kernel>.+$\n+use_profile +0 *$\n+", re.M), "", tdomf)
	tdomf = s
	# write back policy files to disk
	try:
		f = open(texc, "w")
		f.write(texcf)
		f.flush()
		f.close
	except:
		color("error: cannot write exception policy file", red)
		myexit(1)
	try:
		f = open(tdom, "w")
		f.write(tdomf)
		f.flush()
		f.close
	except:
		color("error: cannot write domain policy file", red)
		myexit(1)
	# load config from disk to memory
	os.system(tload + " fa")


# clear config files
def clear():
	try:
		f = open(texc, "w")
		f.write("")
		f.flush()
		f.close
	except:
		color("error: cannot create exception policy file", red)
		myexit(1)
	try:
		f = open(tdom, "w")
		f.write("<kernel>\nuse_profile 0\n\n")
		f.flush()
		f.close
	except:
		color("error: cannot create domain policy file", red)
		myexit(1)
	# load config files from disk to memory
	os.system(tload + " fa")


# check profile config
def check_prof():
	global maxe
	global tprof
	# save config from memory to disk
	os.system(tsave + " p")
	# load config file
	try: tprof = open(tpro).read()
	except: color("error: cannot open file " + tpro, red); myexit(1)
	# store default entries
	p = []
	p.append("0-MAC_FOR_FILE=disabled")
	p.append("0-TOMOYO_VERBOSE=disabled")
	p.append("0-MAX_ACCEPT_ENTRY=" + str(maxe))
	p.append("1-MAC_FOR_FILE=learning")
	p.append("1-TOMOYO_VERBOSE=disabled")
	p.append("1-MAX_ACCEPT_ENTRY=" + str(maxe))
	p.append("2-MAC_FOR_FILE=permissive")
	p.append("2-TOMOYO_VERBOSE=enabled")
	p.append("2-MAX_ACCEPT_ENTRY=" + str(maxe))
	p.append("3-MAC_FOR_FILE=enforcing")
	p.append("3-TOMOYO_VERBOSE=enabled")
	p.append("3-MAX_ACCEPT_ENTRY=" + str(maxe))
	# search for max entries
	flag = 0
	for i in p:
		r = re.search("^[^=]*=", i, re.M)
		if r:
			r2 = r.group()
			s = re.search("^" + re.escape(r2), tprof, re.M)
			# add missing entry
			if not s:
				flag = 1
				tprof += i
	# replace all max entry values with predefined one
	s = re.sub("MAX_ACCEPT_ENTRY=.*", "MAX_ACCEPT_ENTRY=" + str(maxe),  tprof)
	tprof = s
	# save config file and load to memory
	try:
		f = open(tpro, "w")
		f.write(tprof)
		f.flush()
		f.close
	except:
		color("error: cannot write profile config file", red)
		myexit(1)
	os.system(tload + " p")
	os.system(tsave + " p")


# turn back learning mode from enforcing mode
def learn(prog3):
	global tdomf
	# look up all domain with profile 3
	x = re.findall("^<kernel>.* +" + re.escape(prog3) + " *$\n+use_profile +[23] *$", tdomf, re.M)
	# turn back learning mode form enforcing mode for binary
	if x:
		color(prog3 + "  ", blue, 1)
		color("switch back domain from enforcing to learning mode", red)
		for i in x:
			sd1 = re.sub("use_profile +3 *", "use_profile 1", i)
			sd2 = re.sub(re.escape(i), sd1, tdomf)
			tdomf = sd2


# turn back learning mode for all domains with profile 2-3
def learn_all():
	global tdomf
	load()
	# change all domains with profile 2 or 3 to 1
	tdomf2 = re.sub(re.compile("^use_profile +[23] *$", re.M), "use_profile 1", tdomf)
	tdomf = tdomf2
	save()


# turn on enforcing mode for old domains only with profile 1-2 before exiting
def enforce():
	if not opt_keep:
		global progl
		global tdomf
		# look up all domain with profile 1 or 2
		x = re.findall("^<kernel> +/[^ ]+.*$\n+use_profile +[1-2] *$", tdomf, re.M)
		x2 = []
		if x:
			# check if it's not a standalone domain and not a domain with newly created learning mode
			for i in x:
				p = re.search("/[^ ]*$", i, re.M).group()
				p2 = re.search("^<kernel> +" + re.escape(p) + " *$", tdomf, re.M)
				# if it's not a newly created domain
				if not p in progl:
					# if not an exception domain or an exceptin domain but not a main domain
					if not p in spec_prog or ((p in spec_prog) and (not p2)):
						x2.append(i)
		# check if there are old domains with enforcing mode
		flag_old = 0
		if re.search("^<kernel> +/[^ ]+.*$\n+use_profile +3 *$", tdomf, re.M): flag_old = 1
		# turn on enforcing mode for domains
		if x2:
			flag_old2 = 0
			for i in x2:
				p = re.search("^<kernel> +/[^ ]+ *$", i, re.M)
				if p:
					if not flag_old2: color("* turn on enforcing mode for old domains", green); flag_old2 = 1
					# get only binary name
					p2 = re.search("/[^ ]+", p.group(), re.M)
					# print it
					color(p2.group(), blue)
				sd1 = re.sub("use_profile +[1-2] *", "use_profile 3", i)
				sd2 = re.sub(re.escape(i), sd1, tdomf)
				tdomf = sd2
		else:
			if flag_old:
				color("* all old domains in enforcing mode already", green)
			else:
				color("* no domains in enforcing mode currently", green)
	else:
		color("keep domains unchanged for now on demand", red)


# check if process is running currently
def running(prog):
	# directory list
	for pid in os.listdir("/proc"):
		f = os.path.join("/proc", pid, "exe")
		if os.path.isfile(f):
			# link to binary
			if prog == os.readlink(f): return 1

	return 0


# check if this is the only instance of the program
def check_instance():
	mypid = str(os.getpid())
	if os.path.exists(pidf):
		pid = "0"
		try:
			f = open(pidf, "r")
			pid = f.read()
			f.close
		except:
			color("error: cannot open pid file", red)
			myexit(1)
		# is it me?
		if pid == mypid: return 0
		# is the process with the foreign pid still running?
		else:
			# running?
			if os.path.isdir("/proc/" + pid):
				return 1
			# not running
			else:
				try:
					f = open(pidf, "w")
					f.write(mypid)
					f.flush()
					f.close
				except:
					color("error: cannot write pid file", red)
					myexit(1)
	# create pid file
	else:
		try:
			f = open(pidf, "w+")
			f.write(mypid)
			f.flush()
			f.close
		except:
			color("error: cannot write pid file", red)
			myexit(1)

	return 0


# info about domains by a pattern
def info(text = ""):
	global tdomf
	global texcf
	load()
	if text:
		# show info about domains and rules
		r1 = re.findall("^<kernel>.*" + re.escape(text) + ".*$", tdomf, re.M + re.I)
		if r1:
			print
			# how many domains are found?
			for i in r1:
				color(i, blue)
				# it's needed to mark the last domain's end too
				tdomf2 = tdomf + "\n<kernel>"
				r2 = re.findall("^" + re.escape(i) + "\n.*?^(?=<kernel>)", tdomf2, re.M + re.DOTALL)
				if r2:
					for i2 in r2:
						i3 = re.search("use_profile.*", i2, re.M + re.DOTALL)
						if i3:
							# remove the last newline
							i4 = i3.group()
							i5 = i4
							if i4[-1] == "\n": i5 = i4[:-1]
							i6 = i5.splitlines()[0]
							i7 = i5.splitlines()[1:]
							color(i6, red)
							if i7:
								# sort the rules
								i7.sort()
								# print allow_read libs with different colors
								for i3 in i7:
									if i3:
										rr1 = re.search("^allow_read +/lib/|^allow_read +/usr/lib/", i3, re.M)
										rr2 = re.search(" +" + home + "/", i3, re.M)
										if rr2:
											color(i3, cyan)
										elif rr1:
											color(i3, yellow)
										else:
											color(i3, red)
								print
			# print stat
			l = len(r1)
			if l > 1:
				color("(found " + str(l) + " domains)")
			else:
				color("(found " + str(l) + " domain)")
		else:
			color("error: no domains found", red)
	else:
		# search for all active domains
		r = re.findall("^<kernel>.+$\n+use_profile +[1-3] *$", tdomf, re.M)
		if r:
			# select main domains and print
			r3 = []
			for i in r:
				r2 = re.search("^<kernel> +[^ ]+", i, re.M)
				if r2:
					r7 = r2.group().splitlines()[0]
					r4 = re.search("[^ ]+$", r7, re.M).group()
					r5 = r4
					if r4[-1] == "\n": r5 = r4[:-1]
					if not r5 in r3:
						r3.append(r5)

			# select the rest of the subdomains and print with other color
			r6 = []
			for i in r:
				r2 = re.search("[^ ]+$", i, re.M)
				if r2:
					r4 = r2.group()
					r5 = r4
					if r4[-1] == "\n": r5 = r4[:-1]
					if (not r5 in r6) and (not r5 in r3):
						r6.append(r5)

			# print result
			print
			r6.sort()
			r3.sort()
			for i in r6:
				color(i, yellow)
			for i in r3:
				color(i, blue)
			print
		else:
			color("error: no domains found", red)


# remove domains by a pattern
def remove(text):
	global tdomf
	global texcf
	load()
	if text:
		# search for the domain to remove
		r1 = re.findall("^<kernel>.*" + re.escape(text) + ".*$", tdomf, re.M + re.I)
		if r1:
			print
			# how many domains are found?
			l = len(r1)
			if l > 1:
				color("(found " + str(l) + " domains)")
			else:
				color("(found " + str(l) + " domain)")
			flag_count = 0
			for i in r1:
				color(i, blue, 1)
				color("  ", "", 1)
				# ask for confirmation
				if choice("remove domain?"):
					tdomf2 = tdomf + "\n\n<kernel>\n"
					r2 = re.findall("^" + re.escape(i) + "\n.*?^(?=<kernel>)", tdomf2, re.M + re.I + re.DOTALL)
					if r2:
						for i2 in r2:
							# remove domain from domain policy
							r6 = re.sub(re.escape(i2), "", tdomf2, re.M)
							flag_count += 1

							tdomf2 = r6

					tdomf = re.sub(re.compile("^<kernel> *$", re.M), "", tdomf2, re.M)
					
					# remove domain from exception policy
					p = re.search("^<kernel> +/[^ ]+", i, re.M)
					if p:
						# get the main binary name
						p2 = re.search("/[^ ]+$", p.group(), re.M)
						p3 = p2.group()
						# remove it from exception policy
						r6 = re.sub(".* +" + re.escape(p3), "", texcf)
						texcf = r6


			if flag_count > 0:
				if flag_count == 1:
					color("1 domain removed", red)
					print
				else:
					color(str(flag_count) + " domains removed", red)
					print
				save()
			else:
				color("no domain was removed", green)
				print
		else:
			color("error: no domains by pattern", red)


# compare 3 files or dirs and give back a wildcarded name if they seem to be the same (but contain random part)
def compare_temp(last1, last2, last3):
	# create type of rule
	cre = ["allow_create", "allow_mksock", "allow_mkdir", "allow_rename", "allow_link"]

	# all of them have the same length?
	if len(last3) == len(last1) and len(last3) == len(last2):
		# they have parameters?
		a1 = last1.split()
		a2 = last2.split()
		a3 = last3.split()
		l1 = len(a1)
		l2 = len(a2)
		l3 = len(a3)
		# they have the same number of parameters with at least 1?
		if l3 == l1 and l3 == l2 and l3 > 1:
			# check with 1 parameter only
			if l3 == 2:
				# rule types are the same?
				if a3[0] == a1[0] and a3[0] == a2[0]:
					# is it create type of rule?
					if a3[0] in cre:
						# parameters are not nothing?
						if a1[1] and a2[1] and a3[1]:
							# parameters are files or dirs?
							flag_dir = 0
							# files
							if (not a1[1][-1] == "/") and (not a2[1][-1] == "/") and (not a3[1][-1] == "/"):
								flag_dir = 1
							# dirs
							elif (a1[1][-1] == "/") and (a2[1][-1] == "/") and (a3[1][-1] == "/"):
								flag_dir = 2
							if flag_dir:
								b1 = ""; b2 = ""; b3 = ""
								if flag_dir == 1:
									b1 = a1[1]; b2 = a2[1]; b3 = a3[1]
								else:
									b1 = a1[1][:-1]; b2 = a2[1][:-1]; b3 = a3[1][:-1]
								# get the dir names
								d1 = re.search("^/.+/", b1, re.M)
								d2 = re.search("^/.+/", b2, re.M)
								d3 = re.search("^/.+/", b3, re.M)
								if d1 and d2 and d3:
									dd1 = d1.group()
									dd2 = d2.group()
									dd3 = d3.group()
									# their dirs match?
									if dd3 == dd1 and dd3 == dd2:
										# get the file names
										f1 = re.search("[^/]+$", b1, re.M)
										f2 = re.search("[^/]+$", b2, re.M)
										f3 = re.search("[^/]+$", b3, re.M)
										if f1 and f2 and f3:
											ff1 = f1.group()
											ff2 = f2.group()
											ff3 = f3.group()
											# their file names doesn't contain any wildcard?
											# this means their dir is among the exception dirs,
											# otherwise they would have been managed before
											# because of having a create rule
											w1 = re.search("\*|\$", ff1)
											w2 = re.search("\*|\$", ff2)
											w3 = re.search("\*|\$", ff3)
											if (not w1) and (not w2) and (not w3):
												# if some part of the file matches in them
												flag = 0
												flag_notall = 0
												pos = 0
												l = len(ff1)
												# check the beginning
												for i2 in range(0, l):
													if ff3[i2] == ff1[i2] and ff3[i2] == ff2[i2]:
														flag = 1
														pos = i2
													else:
														flag_notall = 1
														break
												# if there was any part the same but not all was the same
												if flag and flag_notall:
													new = ff3[0:pos+1] + "\*"
													if flag_dir == 2: new += "/"
													i = a3[0] + " " + dd3 + new + "\n"
													return i

			# check with 2 parameters
			elif l3 == 3:
				# rule types are the same?
				if a3[0] == a1[0] and a3[0] == a2[0]:
					# is it create type of rule?
					if a3[0] in cre:
						# parameters are not nothing?
						if a1[1] and a2[1] and a3[1] and a1[2] and a2[2] and a3[2]:
							# parameters are files or dirs?
							flag_dir = 0
							# files
							if (not a1[1][-1] == "/") and (not a2[1][-1] == "/") and (not a3[1][-1] == "/") and \
							   (not a1[2][-1] == "/") and (not a2[2][-1] == "/") and (not a3[2][-1] == "/"):
								flag_dir = 1
							# dirs
							elif (a1[1][-1] == "/") and (a2[1][-1] == "/") and (a3[1][-1] == "/") and \
							     (a1[2][-1] == "/") and (a2[2][-1] == "/") and (a3[2][-1] == "/"):
								flag_dir = 2
							if flag_dir:
								b1  = ""; b2  = ""; b3  = ""
								b1_ = ""; b2_ = ""; b3_ = ""
								if flag_dir == 1:
									b1  = a1[1]; b2  = a2[1]; b3  = a3[1]
									b1_ = a1[2]; b2_ = a2[2]; b3_ = a3[2]
								else:
									b1  = a1[1][:-1]; b2  = a2[1][:-1]; b3  = a3[1][:-1]
									b1_ = a1[2][:-1]; b2_ = a2[2][:-1]; b3_ = a3[2][:-1]
								# get the dir names
								d1  = re.search("^/.+/", b1,  re.M)
								d2  = re.search("^/.+/", b2,  re.M)
								d3  = re.search("^/.+/", b3,  re.M)
								d1_ = re.search("^/.+/", b1_, re.M)
								d2_ = re.search("^/.+/", b2_, re.M)
								d3_ = re.search("^/.+/", b3_, re.M)
								if d1 and d2 and d3 and d1_ and d2_ and d3_:
									dd1  = d1.group()
									dd2  = d2.group()
									dd3  = d3.group()
									dd1_ = d1_.group()
									dd2_ = d2_.group()
									dd3_ = d3_.group()
									# their dirs match?
									if dd3 == dd1 and dd3 == dd2 and dd3_ == dd1_ and dd3_ == dd2_:
										# get the file names
										f1  = re.search("[^/]+$", b1, re.M)
										f2  = re.search("[^/]+$", b2, re.M)
										f3  = re.search("[^/]+$", b3, re.M)
										f1_ = re.search("[^/]+$", b1_, re.M)
										f2_ = re.search("[^/]+$", b2_, re.M)
										f3_ = re.search("[^/]+$", b3_, re.M)
										if f1 and f2 and f3 and f1_ and f2_ and f3_:
											ff1  = f1.group()
											ff2  = f2.group()
											ff3  = f3.group()
											ff1_ = f1_.group()
											ff2_ = f2_.group()
											ff3_ = f3_.group()
											# their file names doesn't contain any wildcard?
											# this means their dir is among the exception dirs,
											# otherwise they would have been managed before
											# because of having a create rule
											w1 = re.search("\*|\$", ff1)
											w2 = re.search("\*|\$", ff2)
											w3 = re.search("\*|\$", ff3)
											w1_ = re.search("\*|\$", ff1_)
											w2_ = re.search("\*|\$", ff2_)
											w3_ = re.search("\*|\$", ff3_)

											# default new path name is old path name
											new  = ff3
											new_ = ff3_
											if flag_dir == 2: new  += "/"
											if flag_dir == 2: new_ += "/"
											flag = 0
											flag_notall = 0
											flag2 = 0
											flag_notall2 = 0
											if (not w1) and (not w2) and (not w3):
												# if some part of the file matches in them
												pos = 0
												l = len(ff1)
												# check the beginning
												for i2 in range(0, l):
													if ff3[i2] == ff1[i2] and ff3[i2] == ff2[i2]:
														flag = 1
														pos = i2
													else:
														flag_notall = 1
														break
												# if there was any part the same but not all was the same
												if flag and flag_notall:
													new = ff3[0:pos+1] + "\*"
													if flag_dir == 2: new += "/"

											if (not w1_) and (not w2_) and (not w3_):
												# if some part of the file matches in them
												pos = 0
												l = len(ff1_)
												# check the beginning
												for i2 in range(0, l):
													if ff3_[i2] == ff1_[i2] and ff3_[i2] == ff2_[i2]:
														flag2 = 1
														pos = i2
													else:
														flag_notall2 = 1
														break
												# if there was any part the same but not all was the same
												if flag2 and flag_notall2:
													new_ = ff3_[0:pos+1] + "\*"
													if flag_dir == 2: new_ += "/"
												

											# return the result if there was any change
											if (flag and flag_notall) or (flag2 and flag_notall2):
												i = a3[0] + " " + dd3 + new + " " + dd3_ + new_ + "\n"
												return i

	return ""


# sort and unique a list of rules
def uniq_rules_more(list):
	# at leat 2 rules needed to run sort on them
	l = len(list)
	if l < 2: return list
	
	list2 = []
	# compare all rules with all of them
	for i1 in range(0, l):
		# speed up this part here a bit and reduce number of cycles by skipping rules reading libs
		rr1 = re.search("^allow_read +/lib/|^allow_read +/usr/lib/", list[i1], re.M)
		if not rr1:
			new = ""
			# inside cycle
			for i2 in range(0, l):
				# skip the part checking the rule with itself
				if not i1 == i2:
					rule1 = list[i1]
					rule2 = list[i2]
					# check the number of tags in the rules
					p1 = len(rule1.split("/"))
					p2 = len(rule2.split("/"))
					# if tag numberd match:
					if p1 == p2:
						# compare rules
						if compare_rules(rule1, rule2):
							old = rule1
							if len(rule1) > len(rule2): old = rule2
							# store only the shortest one from the matching ones
							if (not new) or (len(new) > len(old)):
								new = old

			# store the original rule if there was no match
			if not new:
				new = list[i1]
			if not new in list2:
				list2.append(new)
		# add it if if it's a rule reading libs
		else:
			new = list[i1]
			if not new in list2:
				list2.append(new)

	# these 2 sortings are needed for other functions in the main rutins
	if list2:
		# first, sort rules by char length
		list2.sort(key=lambda i: len(i))
		# second, sort rules by number of path splitter
		list2.sort(key=lambda i: len(i.split("/")))
	
	return list2


# sort and unique all rules in all domains
def domain_cleanup():
	global tdomf
	tdomf3 = ""
	tdomf2 = tdomf + "\n<kernel>"
	# collect all domains separately
	r = re.findall("^<kernel>.*?^(?=<kernel>)", tdomf2, re.M + re.DOTALL)

	if r:
		# cycle through domains
		for i in r:
			
			r2 = []
			rule2 = ""
			# cycle through lines of domain and make the rules unique and sort it
			for i2 in i.splitlines():
				if i2:
					# is the line a rule?
					if not i2[0:6] == "allow_":
						tdomf3 += i2 + "\n"
					else:
						if not i2 in r2:
							r2.append(i2)

			r2.sort()
			
			# unique and sort rules again in a better way within only the same types
			r3 = []
			l = len(r2)
			c = 0
			type1 = ""
			while (c < l):
				rule2 = r2[c]
				type2 = rule2.split()[0]
				if not type1: type1 = type2
				# rule types match?
				if type1 == type2:
					#if so, then build list from it
					r3.append(rule2)
				if (not type1 == type2):

					if r3:
						# sort and unique list containing only some types of rules
						r4 = uniq_rules_more(r3)
						r4.sort()
						# store the sorted result
						tdomf3 +=  "\n".join(r4) + "\n"

					r3 = []
					r3.append(rule2)
					type1 = type2
	
				c += 1
				
				# this is the last cycle, so i have to run sort manually
				if c >= l:
					r4 = uniq_rules_more(r3)
					r4.sort()
					tdomf3 +=  "\n".join(r4) + "\n"
				
			tdomf3 += "\n"
			
		tdomf = tdomf3


# get 1 key and wait for it for 1 second
# http://docs.python.org/library/termios.html
def getkey():
	global term_fd, term_old, term_new, term_flag
	term_fd = sys.stdin.fileno()
	term_old = termios.tcgetattr(term_fd)
	term_new = termios.tcgetattr(term_fd)
	term_new[3] = term_new[3] & ~termios.ICANON & ~termios.ECHO
	# do not wait (value 1) but poll it (value 0)
	term_new[6][termios.VMIN] = 0
	# wait 1 second (10 times 0.1 second)
	term_new[6][termios.VTIME] = 10
	termios.tcsetattr(term_fd, termios.TCSANOW, term_new)
	c = None
	term_flag = 1
	try:
		c = os.read(term_fd, 1)
	finally:
		termios.tcsetattr(term_fd, termios.TCSAFLUSH, term_old)
	return c


# set back normal terminal attributes
def clearkey():
	global term_flag
	if term_flag:
		termios.tcsetattr(term_fd, termios.TCSAFLUSH, term_old)
		term_flag = 0


# print domain stat
def stat():
	global tdomf
	d = re.findall("^<kernel>.+$\n+use_profile +[1-3] *$", tdomf, re.M)
	r = re.findall("^allow_", tdomf, re.M)
	color(str(len(d)) + " active domains, " + str(len(r)) + " rules")


# my finish
def myfinish():
	global flag_safe
	global flag_firstrun
	# check if everything was run at least once before turning on enforcing mode
	if flag_safe:
		# turn on enforcing mode for all old domains before exiting
		enforce()
		# save config
		save()
	elif flag_firstrun:
		color("* haven't finished to run at least once", red)
	stat()
	myexit()


# my exit
def myexit(num = 0):
	# delete pid file
	try: os.remove(pidf)
	except: None
	# exit
	exit(num)


# ----------------------------------------------------------------------


# **********************
# ******* CHECK ********
# **********************

# manage policy and rules
def check():
# ----------------------------------------------------------------------

	global flag_firstrun

	sand_clock()


#  	speed2 = time.clock()
#	print "time ", str(time.clock() - speed2)

	global progl
	# load config files
	global tdomf
	global texcf
	load()
	

# ----------------------------------------------------------------------
	# print programs already in domain (only profile 1-3 matters), but not in progs list
	global flag_check
	global flag_check2
	global flag_check3
	flag_check3 = 0
	if not flag_check:
		flag_check = 1
		x = re.findall("^<kernel> +/[^ ]+ *$\n+use_profile +[1-3] *$", tdomf, re.M)
		if x:
			for i in x:
				p = re.search("^<kernel> .*$", i, re.M).group()[9:]
				if not p in progs:
					if not flag_check2:
						flag_check2 = 1
						color("* already existing domains", green)
					# store if not an exception
					if not p in spec_prog:
						progs.append(p)
						color(p, blue)

# ----------------------------------------------------------------------
	# print output only once if no change happened
	if flag_firstrun: color("* checking policy and rules", yellow)

	sand_clock()

	for prog in progs:
		# does the domain exist for the program?
		x = re.search("^initialize_domain *" + re.escape(prog) + " *$", texcf, re.M)
		if x:
			# pritn only once
			if flag_firstrun:
				color(prog, blue, 1); flag_check3 = 1
				color(", domain exists", "", 1)
		else:
			color(prog, blue, 1); flag_check3 = 1
			color(", no domain", "", 1)
			# create a domain for the program
			print ", ",; color("create domain", green, 1)
			# if the program is running already, then restart is needed for the rules to take effect
			if running(prog): color(" (restart needed)", red, 1)
			texcf += "\ninitialize_domain " + prog + "\n"

		# does the rule exist for it?
		x = re.findall("^<kernel> +" + re.escape(prog) + " *$\n+use_profile +[0-9]+ *$", tdomf, re.M)
		if not x:
			color(", no rule", "", 1)
			print ", ",; color("create rule with learning mode on", green, 1)
			if not flag_firstrun: print
			# create a rule for it
			tdomf += "\n<kernel> " + prog + "\n\nuse_profile 1\n\n"
			# store prog name to know not to turn on enforcing mode for these ones on exit
			progl.append(prog)
		else:
			# only 1 rule should exist
			if len(x) > 1: error_conf("101")
			# print only once
			if flag_firstrun:
				color(", rule exists", "", 1)
			# search for all subdomains too
			xx = re.findall("^<kernel> +" + re.escape(prog) + ".*$\n+use_profile +[0-9]+ *$", tdomf, re.M)
			if not xx: error_conf("102")

			# cycle through the domain and its subdomains
			for d in xx:
				# get subdomain binary
				prog2 = re.search(re.escape(prog) + ".*$", d, re.M).group()
				# get mode
				x2 = re.findall("use_profile +[0-9]+", d, re.M)
				if len(x2) > 1: error_conf("103")
				x3 = re.findall("[0-9]+", x2[0], re.M)
				profile = int(x3[0])
				if profile < 0 or profile > 3: error_conf("104")

				# check which mode is on
				if profile == 0:
					# print console messages only for the main domain
					if prog2 == prog:
						if not flag_check3: color(prog, blue, 1); flag_check3 = 1
						color(", disabled mode", "", 1)
						print ", ",
						color("turn on learning mode", green, 1)
						if not flag_firstrun: print
					# if in disabled mode, turn on learning mode for the domain or subdomain
					sd1 = re.sub("use_profile +0 *", "use_profile 1", d)
					sd2 = re.sub(re.escape(d), sd1, tdomf)
					tdomf = sd2

				if profile == 1:
					if flag_firstrun:
						if prog2 == prog: color(", learning mode on", "", 1)

				if profile == 2:
					if flag_firstrun:
						if prog2 == prog: color(", permissive mode on", "", 1)

				if profile == 3:
					# enfocing mode should never be switched back off once it's on
					if flag_firstrun:
						if prog2 == prog: print ", ",; color("enforcing mode on", purple, 1)

		# new line
		if flag_firstrun: print


# ----------------------------------------------------------------------

	# ************************
	# ******* GET LOG ********
	# ************************

	sand_clock()

	# get recent tomoyo error messages from syslog
	global tmarkf
	# load log mark from file
	if os.path.isfile(tmark):
		try: tmarkf = open(tmark).read()
		except: color("error: cannot open file " + tmark, red); myexit(1)
	else:
		tmarkf = ""
	# read the log in
	try: log = open(tlog).read()
	except: color("error: cannot open file " + tlog, red); myexit(1)
	log2 = []
	# search for tomoyo error messages
#	s = re.findall(".* TOMOYO-ERROR: +.*$", log, re.M)
	# updating log format from tomoyo version 2.2 to 2.3
	s = re.findall(".*[\- ]ERROR: Access +.*$|.*[\- ]ERROR: Domain +.*$", log, re.M)
	l=len(s)

	# check the mark if there are log messages
	if l > 0:
		# if mark exists
		if tmarkf:
			m = " kernel: \[" + tmarkf + "\] "
			# seek the mark in the log
			flag = 0
			ind = 0
			for i2 in range(0, l):
				if re.search(m, s[i2]):
					flag = 1
					ind = i2
					break
			# if mark matches somewhere in the log, then return the lines of log only from the mark
			if flag == 1: log2 = s[ind+1:l]
			else:         log2 = s
				
			# replace mark with last line's kernel time
			# get the kernel time from the last result of the former search
			s2 = re.search(" kernel: \[[0-9 ]+\.[0-9 ]+\] ", s[l-1])
			if not s2: color("error: unexpected error, log message format is not correct", red); myexit(1)
			m2 = re.search("[0-9 ]+\.[0-9 ]+", s2.group()).group()
			# replace old mark with this new one
			tmarkf = m2

		# if mark doesn't exist yet, create mark and give back full log
		else:
			# get the kernel time from the last result of the former search
			s2 = re.search(" kernel: \[[0-9 ]+\.[0-9 ]+\] ", s[l-1])
			if not s2: color("error: unexpected error, log message format is not correct", red); myexit(1)
			m2 = re.search("[0-9 ]+\.[0-9 ]+", s2.group()).group()
			# add new mark
			tmarkf = m2
			log2 = s

	else:
		# delete mark if no log messages
		tmarkf = ""
	
	# write back log mark to its file
	try:
		f = open(tmark, "w")
		f.write(tmarkf)
		f.flush()
		f.close
	except:
		color("error: cannot create file " + tmark, red)
		myexit(1)
	

	# **********************************************
	# ******* CONVERT LOG AND ADD TO POLICY ********
	# **********************************************

	sand_clock()
	
	# convert log messages into rules and add them to policy
	flag_print = 0
	prog3 = []
	rule3 = []

	# don't take any log with --reset switch in the first round
	global flag_reset
	if flag_reset:
		flag_reset = 0
	else:
		logall = "\n".join(log2)

		# 2 kinds of message types:
		# TOMOYO-ERROR: Access 'read(...) /etc/file' denied for /bin/program
		# TOMOYO-ERROR: Domain '<kernel> /bin/this/program /bin/other/program' not defined.
#		log3 = re.findall(".* TOMOYO-ERROR: Access \'.*$", logall, re.M)
#		log4 = re.findall(".* TOMOYO-ERROR: Domain \'.*$", logall, re.M)

		# updating log format from tomoyo version 2.2 to 2.3
		# 2 kinds of message types:
		# ERROR: Access read(...) /etc/file' denied for /bin/program
		# ERROR: Domain <kernel> /bin/this/program /bin/other/program' not defined.
		log3 = re.findall(".*[\- ]ERROR: Access \'*.*$", logall, re.M)
		log4 = re.findall(".*[\- ]ERROR: Domain \'*.*$", logall, re.M)

		# log type 1
		if log3:
			# collect logs and create rules from it
			for i in log3:
				s1 = re.sub(".*[\- ]ERROR: Access \'*", "", i)
				s2 = re.sub("\'* denied for .*$", "", s1)
				prog2 = re.sub(".*\'* denied for ", "", s1)
				comm2 = "allow_" + re.search("^[^ \(]*", s2).group()
				ff    = re.search("[ ].*$", s2).group()
				ff2   = re.sub("^[ ]*", "", ff)
				# change "\" character to "\\" because of unicode
				file2 = re.sub("\\\\", "\\\\\\\\", ff2)
				rule2 = comm2 + " " + file2
				flag = 0
				# check if rule with binary exist already
				for i in range(0, len(rule3)):
					if (prog2 == prog3[i] and rule2 == rule3[i]):
						flag = 1
						break
				# store if it doesn't exist
				if flag == 0:
					prog3.append(prog2)
					rule3.append(rule2)

			# add rules to poliy file
			if rule3 and prog3:
				# sort prog3 and rule3 lists by prog3's values
				l = len(prog3)
				prog4 = []
				rule4 = []
				for i in range(0, l): prog4.append(prog3[i])
				prog4.sort()
				for i in prog4:
					c = prog3.index(i)
					r = rule3[c]
					rule4.append(r)
					prog3.pop(c)
					rule3.pop(c)
			
				l = len(rule4)
				flag_choice = 0
				for i in range(0, l):
					xx = re.findall("^<kernel>.* +" + re.escape(prog4[i]) + "$\n+use_profile +3 *$", tdomf, re.M)
					# check if log entry contains a domain that doesn't exist
					if xx:
						# print log messages
						if flag_print == 0:
							flag_print = 1
							color("* adding log messages to policy", yellow)
						color(prog4[i] + "  ", blue, 1)
						color(rule4[i], purple, 1)

						# confirmation (only if no --yes switch)
						c = 1
						if not opt_yes:
							c = choice("  add rules?")
						else:
							print
						if c:
							flag_choice = 1
							# always add rule to all domain's binary, not only who have their own domains
							for d in xx:
								d2 = d + "\n" + rule4[i]
								s = re.sub(re.escape(d), d2, tdomf, re.M)
								tdomf = s

						if flag_choice:
							# switch back domain to learning mode
							# and print this message after every different programs
							p = prog4[i]
							if i >= l-1:
								learn(p)
								flag_choice = 0
							elif not p == prog4[i+1]:
								learn(p)
								flag_choice = 0

		# log type 2
		if log4:
			# collect logs and create rules from it
			log5 = []
			for i in log4:
				# get the domain name that was denied
				s1 = re.sub(".*[\- ]ERROR: Domain \'*", "", i)
				s2 = re.sub("\'* not defined\..*$", "", s1)
				if s2 not in log5:
					log5.append(s2)

			if log5:
				log5.sort()
				# cycle through the domains
				prog4 = []
				rule4 = []
				l = len(log5)
				for i in range(0, l):
					# get the main binary of the domain in the log
					s1 = re.sub("^<kernel> +", "", log5[i], re.M)
					s2 = re.search("^[^ ]+", s1, re.M)
					if not s2:
						color("error: not expected format of error log", red)
						myexit(1)
					s3 = s2.group()
					prog4.append(s3)
					rule4.append(log5[i])

				l = len(rule4)
				if l > 0:
					if flag_print == 0:
						flag_print = 1
						color("* adding log messages to policy", yellow)
					flag_choice = 0
					for i in range(0, l):
						color(prog4[i] + "  ", blue, 1)
						color(rule4[i], purple, 1)

						# confirmation (only if no --yes switch)
						c = 1
						if not opt_yes:
							c = choice("  add rules?")
						else:
							print
						if c:
							# add subdomain from log
							tdomf += log5[i] + "\n\nuse_profile 1\n\n"

							# print info after every different domain
							p = prog4[i]
							flag = 0
							if i >= l-1: flag = 1
							elif not p == prog4[i+1]: flag = 1
							if flag:
								color(p + "  ", blue, 1)
								color("new domains added in learning mode", red)


# ----------------------------------------------------------------------

	# *************************************
	# **** CHECK CHANGE OF POLICY TEXT ****
	# *************************************
	# check change of rules
	global tdomf_bak
	# backup conf exists yet?
	if not tdomf_bak:
		tdomf_bak = tdomf
	else:
		# exit function if they match, don't do unnecessary work
		if tdomf_bak == tdomf:
			sand_clock(1)
			return
		# store it if no match
		else:
			tdomf_bak = tdomf


# ----------------------------------------------------------------------

	# ******************************
	# ******* RESHAPE RULES ********
	# ******************************

	sand_clock()

	domain_cleanup()

	# -----------------------------------------------------------------------------------------------------------

	# recursive dir handling: change all subdir names of all dir in specr to fully wildcarded
	tdomf2 = ""
	l = len(specr)
	if l > 0:

		s2 = ""
		for i in tdomf.splitlines(1):
			s = i
	

			# operate only on rules
			if i[0:6] == "allow_":
				r = i.split()
				l2 = len(r)
				dir1 = ""
				dir2 = ""

				# 1 or 2 parameters?
				if l2 == 2:
					ind1 = 0
					# is the dir in specr?
					for i3 in specr:
						dir1 = compare_recursive(r[1], i3)
						if dir1: break
						ind1 += 1
					# if so
					if dir1:
						c = specr2_count[ind1]
						if c > 0:
							s = ""
							for i4 in range(1, c+1):
								dr = ""
								# is it a dir or file originally
								if r[1][-1] == "/":
									dr = dir1
									for i5 in range(c-i4, c):
										dr += "\*/"
								else:
									dr = dir1[:-1]
									for i5 in range(c-i4, c):
										dr += "/\*"
							
								s += r[0] + " " + dr + "\n"

				elif l2 == 3:
					ind1 = 0
					ind2 = 0
					# is the dir in specr?
					for i3 in specr:
						dir1 = compare_recursive(r[1], i3)
						if dir1: break
						ind1 += 1
					# is the dir2 in specr?
					for i3 in specr:
						dir2 = compare_recursive(r[2], i3)
						if dir2: break
						ind2 += 1

					# if any of them yes and refer to the same recursive dir
					if dir1 or dir2:
				
#						if dir1 and (not dir2):
#							c = specr2_count[ind1]
#							if c > 0:
#								s = ""
#								for i4 in range(1, c+1):
#									dr = ""
#									# is it a dir or file originally
#									if r[1][-1] == "/":
#										dr = dir1
#										for i5 in range(c-i4, c):
#											dr += "\*/"
#									else:
#										dr = dir1[:-1]
#										for i5 in range(c-i4, c):
#											dr += "/\*"
#							
#									s += r[0] + " " + dr + " " + r[2] + "\n"

#						if (not dir1) and dir2:
#							c = specr2_count[ind2]
#							if c > 0:
#								s = ""
#								for i4 in range(1, c+1):
#									dr = ""
#									# is it a dir or file originally
#									if r[2][-1] == "/":
#										dr = dir2
#										for i5 in range(c-i4, c):
#											dr += "\*/"
#									else:
#										dr = dir2[:-1]
#										for i5 in range(c-i4, c):
#											dr += "/\*"
#							
#									s += r[0] + " " + r[1] + " " + dr + "\n"

						if (dir1 and dir2) and (ind1 == ind2):
							c = specr2_count[ind1]
							if c > 0:
								s = ""
								for i4 in range(1, c+1):
									dr = ""
									# is it a dir or file originally
									if r[1][-1] == "/":
										dr = dir1
										for i5 in range(c-i4, c):
											dr += "\*/"
									else:
										dr = dir1[:-1]
										for i5 in range(c-i4, c):
											dr += "/\*"
							
									s += r[0] + " " + dr + " " + dr + "\n"
	

#						if (dir1 and dir2) and (not ind1 == ind2):
#							pass
	

				# insert rule only if the former rule was not the same
				# this is to avoid massive multiplication of the rules because of the recursive check
				if not s == s2:
					tdomf2 += s
					s2 = s

			else:
				tdomf2 += s
				s2 = ""

		tdomf = tdomf2



	sand_clock()


	# the spec predefined dirs and those dirs that have files newly created in them
	# will be wildcarded

	# there are 2 runs: first is to collect all the dir names that have create entries
	# second run is to make all entries containing the former ones wildcarded
	# and do this overall because it cannot be known if one non-unique filname will be reused
	# by other processes
	spec2 = []
	spec3 = []

	# these are the special create entries, where the place of the file will be wildcarded
	# because it cannot be determined fully yet if the file being created has a unique filename
	# or a contantly changing one (temporary name)
	cre = ["allow_create", "allow_mksock", "allow_rename", "allow_unlink", "allow_mkdir", "allow_link"]

	# allow_mkdir will also have a special handling, cause usually files are created in the
	# new dir too, and it cannot be surely told if the dir itself has a unique name too,
	# so this exact dir should be wildcarded too with the files in it
	cre2 = ["allow_mkdir"]

	# collect more special dirs (that will be wildcarded)
	temp1 = re.findall("^allow_.*$", tdomf, re.M)
	# filter and unify directories to speed up the things
	for i in temp1:
		i2 = i.split()
		l = len(i2)
		# good to go with at least 2 parameters
		if l >= 2:

			# is it a special allow_ line?
			if i2[0] in cre:
				# check params from number 2 (first is only the allow_)
				for i3 in range(1, l):
					# get the dir name only (cannot be root dir /)
					r = re.search("^/.+/", i2[i3], re.M)
					if r:
						r2 = r.group()
						flag = 0
						for i4 in spec2:
							if compare_dirs(r2, i4):
								flag = 1
								break
						if not flag:
							spec2.append(r2)

			# is it a special allow_mkdir line?
			if i2[0] in cre2:
				# check params from number 2 (first is only the allow_)
				for i3 in range(1, l):
					# get the dir name only (cannot be root dir /)
					r = re.search("^/.+/", i2[i3], re.M)
					if r:
						r2 = r.group()
						flag = 0
						for i4 in spec3:
							if compare_dirs(r2, i4):
								flag = 1
								break
						if not flag:
							spec3.append(r2)


	# iterate through all the rules and reshape them
	tdomf2 = ""
	for i in tdomf.splitlines(1):
		
		# operate only on rules
		if i[0:6] == "allow_":
			i2 = i.split()
			l = len(i2)
			# good to go if number of parameters are more than 1
			if l >= 2:
				s = i2[0]

				# handle some of the special allow_ entries differently whose parameters are not dirs
				if i2[0] == "allow_ioctl":
					for i3 in range(1, l):
						if i3 == 1:
							param = i2[i3]
#							r = re.sub("socket:\[[0-9]+\]", "socket:[\\\\$]", param)
							# updating policy format from tomoyo version 2.2 to 2.3
							r = re.sub("socket:\[.*\].*$", "socket:[\\\\*]", param)
							param = r

						# remove second parameter at allow_ioctl rule (tomoyo version 2.3)
						if i3 >= 2:
							param = "0x0000-0xFFFF"

						# add param if not nothing
						if param:
							s += " " + param

				# handle params that are dirs
				else:
					# check params from number 2 (first is only the allow_)
					for i3 in range(1, l):
					
						# is param dir among spec or spec2 or specr dirs?
						# only one of them is enough to find
						
						param = i2[i3]
						# get parent dir of param (if file or dir)
						paramd = ""
						if not param[-1] == "/":
							r = re.search("^/.+/", param, re.M)
							if r: paramd = r.group()
						else:
							r = re.search("^/.+/", param[:-1], re.M)
							if r: paramd = r.group()
						
						param2 = ""
						flag  = 0
						flag3 = 0


						flag_ex = 0
						# check dir in exception						
						if not flag_ex:
							for i5 in spec_ex:
								if compare_dirs(paramd, i5):
									flag_ex = 1
									break




						# is it in spec?
						if not flag_ex:
							if not flag:
								for d in spec:
									l1 = len(d)
									l2 = len(param)
									if l1 < l2:
										if param[0:l1] == d:
											flag = 1
											break
						
						# is it in spec2?
						if not flag_ex:
							if not flag:
								for i5 in spec2:
									if compare_dirs(paramd, i5):
										flag = 1
										break

						# is it in spec3?
						if not flag_ex:
							if not flag3:
								for i5 in spec3:
									if compare_dirs(paramd, i5):
										flag3 = 1
										break

						# path is in spec or spec2
						if flag:
							# is it a file or dir?
							if param[-1] == "/":
								r = re.sub("/[^/]+/$", "/\\\\*/", param)
							else:
								r = re.sub("/[^/]+$", "/\\\\*", param)
							param = r

						# path is in spec3
						if flag3:
							# is it a file? (dir is managed before with spec and spec2 code)
							if not param[-1] == "/":
								# wildcard file with parent dir too
								r = re.sub("/[^/]+/[^/]+$", "/\\\\*/\\\\*", param)
								param = r


						# wildcard library files version numbers
						if re.search("/lib.+[\.0-9]*\.so[\.0-9]*$", param, re.M):
							r = re.sub("[\.0-9]+\.so\.[\.0-9]+$", "\\\\*.so.\\\\*", param)
							param = r
							r = re.sub("[\.0-9]+\.so$", "\\\\*.so", param)
							param = r
							r = re.sub("\.so\.[\.0-9]+$", ".so.\\\\*", param)
							param = r
						
						# is it in "/proc/[0-9]+/" form? if so, then change the PID number to wildcard
						if re.search("^/proc/[0-9]+/", param):
							r = re.sub("^/proc/[0-9]+/", "/proc/\\\\$/", param)
							param = r

						# change /home/user/ dir to /home/\*/ form to support multiple users
						i4 = re.sub("^" + re.escape(home) + "/[^/]+/", home + "/\\\\*/", param)
						param = i4


						# add param if not nothing
						if param:
							s += " " + param

				i = s + "\n"

		tdomf2 += i

	tdomf = tdomf2


	# ----------------------------------------------------------------------------------------------------------
	# ***********************
	# ***  FINAL RESHAPE  ***
	# ***********************
	# iterate through all the rules again and reshape them

	sand_clock()

	# domain sorting and cleanup is needed here cause the following rutins heavily depend on a sorted dataflow
	domain_cleanup()


	# also, check the last 3 rules and if they match partly (being temp files), then wildcard it
	# but only if it's in an exception dir, cause otherwise it's managed before

	last1 = ""
	last2 = ""
	last3 = ""
	tdomf2 = ""
	for i in tdomf.splitlines(1):
		last3 = i
		
		# check the last 3 rules and determine if the params start with the same name, but end the other way
		# this might mean thease are temporary files containg random chars
		# within the same domain yet?
		if last3[0:6] == "allow_":
			# were there 2 rules yet?
			if last1 and last2:
				d = compare_temp(last1, last2, last3)
				if d:
					i = d
					last1 = ""
					last2 = ""
					last3 = ""


			# roll the rules
			last1 = last2
			last2 = last3
		else:
			last2 = ""
			last3 = ""

		tdomf2 += i

	tdomf = tdomf2
	
	domain_cleanup()


	sand_clock()

	# all entries with allow_create will be recreated with allow_unlink and allow_read/write entries too
	# cause there are deny logs frequently coming back for the created files trying to be written, unlinked or trancated
	# what is created should be allowed to be written or unlinked or truncated

	tdomf2 = ""
	for i in tdomf.splitlines(1):
		
		# on create rule, add read/write, unlink and truncate too if 
		if i[0:13] == "allow_create ":
			# remove second number parameter at allow_create rule for the rest of the rules (tomoyo version 2.3)
			i1 = re.sub(" +[0-9]+$", "", i, re.M)
			
			i2 = re.sub("allow_create ", "allow_read/write ", i1, re.M)
			i3 = re.sub("allow_create ", "allow_unlink ", i1, re.M)
			i4 = re.sub("allow_create ", "allow_truncate ", i1, re.M)
			i = i + i2 + i3 + i4

		tdomf2 += i

	tdomf = tdomf2

	sand_clock()

	domain_cleanup()

	save()

	sand_clock(2)


# ----------------------------------------------------------------------

# *********************
# ******* INIT ********
# *********************

# check command line options
opt_all = ["-v", "--version", "-h", "--help", "--clear", "--reset", "-c", "--color", \
		   "-i", "--info", "-r", "--remove", "--yes", "-k", "--keep", "-R", "--recursive", \
		   "-1", "--once", "-l", "--learn"]
l = len(sys.argv) - 1
if (l > 0):
	op = sys.argv[1:]
	if op.count("-1") or op.count("--once"):
		opt_once = 1
	if op.count("-c") or op.count("--color"):
		opt_color = 1
	if op.count("-k") or op.count("--keep"):
		opt_keep = 1
	# search for bad option switches
	for i in op:
		# i have to check here if option is not null ""
		if not i:
			color("error: bad option parameter", red)
			myexit(1)
		if i[0] == "-":
			if not i in opt_all:
				color("error: bad option parameter", red)
				myexit(1)
	flag_exit = 0
	if op.count("-v") or op.count("--version"):
		version()
		flag_exit = 1
	if op.count("-h") or op.count("--help"):
		help()
		flag_exit = 1
	if flag_exit: myexit()
	if op.count("-l") or op.count("--learn"):
		opt_learn = 1
	if op.count("--clear"):
		opt_clear = 1
	if op.count("--reset"):
		opt_reset = 1
		flag_reset = 1
	if op.count("--yes"):
		opt_yes = 1

	ind_info = -1
	ind1 = op.count("-i")
	ind2 = op.count("--info")
	if ind1 + ind2 > 1:
		color("error: bad option parameter", red)
		myexit(1)
	if ind1:
		ind_info = op.index("-i")
	elif ind2:
		ind_info = op.index("--info")
	if ind1 > 0 or ind2 > 0:
		l2 = len(op)
		if ind_info+1 < l2:
			opt_info = 1
			opt_info2 = op[ind_info+1]
			if opt_info2[0] == "-":
				opt_info2 = ""
		else:
			opt_info = 1
			opt_info2 = ""

	ind_remove = -1
	ind1 = op.count("-r")
	ind2 = op.count("--remove")
	if ind1 + ind2 > 1:
		color("error: bad option parameter", red)
		myexit(1)
	if ind1:
		ind_remove = op.index("-r")
	elif ind2:
		ind_remove = op.index("--remove")
	if ind1 > 0 or ind2 > 0:
		l2 = len(op)
		if ind_remove+1 < l2:
			opt_remove = 1
			opt_remove2 = op[ind_remove+1]
			# error if the second parameter is a switch too
			if opt_remove2[0] == "-":
				opt_remove2 = ""
				color("error: bad option parameter", red)
				myexit(1)
		else:
			color("error: bad option parameter", red)
			myexit(1)

	ind_recursive = -1
	ind1 = op.count("-R")
	ind2 = op.count("--recursive")
	if ind1 + ind2 > 1:
		color("error: bad option parameter", red)
		myexit(1)
	if ind1:
		ind_recursive = op.index("-R")
	elif ind2:
		ind_recursive = op.index("--recursive")
	if ind1 > 0 or ind2 > 0:
		l2 = len(op)
		if ind_recursive+1 < l2:
			for i in range(ind_recursive+1, len(op)):
				if op[i][0] == "-": break
				opt_recursive = 1
				d = op[i]
				# dir / is not allowed
				if d == "/":
					color("error: root directory is not allowed", red)
					myexit(1)
				d2 = os.path.realpath(d)
				if os.path.isdir(d2):
					if not d2 in specr:
						specr.append(d2)
						op[i] = d2
				else:
					color("error: no such directory: " + d2, red)
					myexit(1)
			# error if the second parameter is a switch too
			if not len(specr):
				color("error: bad option parameter", red)
				myexit(1)
		else:
			color("error: bad option parameter", red)
			myexit(1)
	specr.sort()
	

	# the rest of the parameters not starting with "-"
	# are additional programs to check
	# except parameter of --info and --remove
	l3 = len(op)
	for i in range(0, l3):
		if (not op[i][0] == "-") and (not i == ind_info+1) and (not i == ind_remove+1) and (not op[i] in specr):
			i2 = which(op[i])
			# does the file exist on the path?
			if os.path.isfile(i2):
				# store it if not an exception
				if not i2 in spec_prog:
					progs.append(i2)
			else:
				color("error: no such file: " + i2, red)
				myexit(1)

	# expand dir names in specr with "/" char
	for i in range(0, len(specr)):
		if not specr[i][-1] == "/": specr[i] += "/"
	

# print some info
print "tomld (tomoyo learning daemon)", ver

# check if i am root
if not os.getuid() == 0: color("error: root privileges needed", red); myexit(1)

# check already running instance of the program
if check_instance():
	color("error: tomld is running already", red)
	myexit(1)


# check system type and print system info
# is it Linux?
if not platform.system().lower() == "linux": color("error: this platform is unsupported", red); myexit(1)
# store platform name and version
r = platform.linux_distribution()[0].lower() + " " + platform.linux_distribution()[1].lower()
pl = re.sub("  +", " ", r)
r = pl
pl = re.sub(" *$", "", r, re.M)

print "platform is", pl

# check if it's supported platform
flag = 0
for i in supp:
	# add a "." char to the end of the platform name if the last chars are numbers
	# so minor version numbers won't be problems
	if i[-1] in "0123456789":
		i += "."
	print i
	# check which name is shorter, and compare to the shortest only
	# so "debian 6." fits to "debian 6.0.1"
	l1 = len(i)
	l2 = len(pl)
	if l1 < l2:
		if pl.startswith(i): flag = 1
	else:
		if i.startswith(pl): flag = 1
if flag == 0:
	color("error: this platform is unsupported", red)
	print "supported platforms are:"
	for i in supp: print "  ", i
	myexit(1)


# ----------------------------------------------------------------------
# check status of tomoyo
f = "/proc/cmdline"
try: cmdl = open(f).read()
except: color("error: cannot open file " + f, red); myexit(1)


if tkern in cmdl: print "tomoyo kernel mode is active"
else:
	color("error: tomoyo is not activated", red)

# auto install missing packages
pl1 = platform.linux_distribution()[0].lower()
tpak1 = ""
tpak2 = ""
if pl1 == "debian" or pl1 == "ubuntu":
	tpak1 = tpak1d
	tpak2 = tpak2d
	tinit = tinit1
	tload = tload1
	tsave = tsave1

	#if not (package_d(tpak1) and package_d(tpak2)):
	if not package_d(tpak2):
		color("error: tomoyo packages are missing", red)
		color("install packages (" + tpak2 + ") and reboot the system with " \
			+ tkern + " kernel parameter", red)
		if choice("should i do that for you?"):

			# install packs
			color("ok, installing packages", yellow)
			comm = "/usr/bin/apt-get"
			if not os.path.isfile(comm):
				color("error: command not found: " + comm, red); myexit(1)
			os.system(comm + " update")
			os.system(comm + " install " + tpak2)
			color("installation finished", yellow)

			# check grub
			color("checking grub", yellow)
			if not package_d("grub-pc"):
				color("error: no grub found, you must manually set " + tkern + " kernel parameter on boot", red)
			else:
				# add grub parameter
				f = "/etc/default/grub"
				if not os.path.isfile(f):
					color("error: no such file: " + i, red); myexit(1)
				f2 = open(f).read()
				# kernel parameter set yet?
				p0 = re.search("^GRUB_CMDLINE_LINUX=\".*" + re.escape(tkern) + ".*\"$", f2, re.M)
				if not p0:
					p = re.search("^GRUB_CMDLINE_LINUX=\".*\"$", f2, re.M)
					if p:
						p1 = p.group()
						p2 = re.sub("\"$", " " + tkern + "\"", p1)
						p3 = re.sub(re.escape(p1), p2, f2)
						f2 = p3
					else:
						f2 += "\nGRUB_CMDLINE_LINUX=\"" + tkern + "\"\n"
					try:
						f = open(f, "w")
						f.write(f2)
						f.flush()
						f.close
					except:
						color("error: could not write grub config file, you must manually set " + tkern + " kernel parameter on boot", red)
					# update grub
					comm = "/usr/sbin/update-grub"
					if not os.path.isfile(comm):
						color("error: command not found: " + comm, red)
						color("error: could not update grub config, you must manually set " + tkern + " kernel parameter on boot", red)
					color("updating grub config", yellow)
					os.system(comm)

			color("system must be rebooted, and run tomld again", red)
			myexit(1)

		myexit(1)


# ----------------------------------------------------------------------
# check tomoyo tool files
if not os.path.isfile(tload): color("error: " + tload + " executable binary missing", red); myexit(1)
if not os.path.isfile(tsave): color("error: " + tsave + " executable binary missing", red); myexit(1)

# create tomoyo dir if it doesn't exist yet
if not os.path.isdir(tdir):
	os.mkdir(tdir, 0700)

check_prof()

# on --reset, backup config files with tomoyo-savepolicy
# (it always creates backup files using timestamp when saving)
if opt_reset:
	color("* resetting domain configurations on demand", red)
	if not choice("are you sure?"): myexit()
	os.system(tsave)
	color("policy file backups created", green)

# create new empty policy files if missing or if --reset switch is on
if (not os.path.isfile(tdom)) or (not os.path.isfile(texc)) or opt_reset:
	clear()

# on --clear, empty configuration files
if (not opt_reset) and opt_clear:
	color("* clearing domain configurations on demand", red)
	if not choice("are you sure?"): myexit()
	os.system(tsave)
	color("policy file backups created", green)
	clear()
	color("* configuration cleared", red)
	myexit()

if (opt_info):
	info(opt_info2)
	myexit()

if (opt_remove):
	remove(opt_remove2)
	myexit()

if (opt_learn):
	color("* turning all domains into learning mode on demand", red)
	if not choice("are you sure?"): myexit()
	os.system(tsave)
	color("policy file backups created", green)
	learn_all()
	color("all domains turned back to learning mode", red)
	myexit()


# ----------------------------------------------------------------------

# calculate dir depths of all recursive dirs
if specr:
	color("calculating directory depths for recursive dirs...", yellow, 1)
	for i in specr:
		c = 0
		c1 = len(re.findall("/", i))
		for dirpath, dirnames, filenames in os.walk(i):
			c2 = len(re.findall("/", dirpath))
			if c2 > c: c = c2
		c = c - c1 + 1
		# add one plus extra
		c += 1
		specr2_count.append(c)
	color("done", yellow)



# ************************************
# ******* PROCESS SEARCH LOOP ********
# ************************************

# print process names using tcp or udp packets

# list of network stat files
netf = []
netf.append("/proc/net/tcp")
netf.append("/proc/net/tcp6")
netf.append("/proc/net/udp")
netf.append("/proc/net/udp6")


# iterate through /proc/PID/fd and see if the links there are sockets
# if so, then look them up in /proc/net/tcp and udp whether they are listed there
# if again so, then resolve the link /proc/PID/exe to see which process
# the socket belongs to

# search cycle
mypid = os.getpid()
d = "/proc"


if spec_prog:
	color("* exception domains", green)
	for i in spec_prog: color(i + " ", purple, 1)
	print

if progs:
	color("* additional programs on demand", green)
	for i in progs: color(i, blue)

flag2 = 1
color("* processes using network", green)

# count variable for policy check
global recheck; recheck = count


try:
  while 1:
  	# store time
  	speed = time.clock()

	# read up all net stat files and create a list of inode items
	netf2 = ""
	for i in netf:
		try: netf2 += open(i).read()
		except: color("error: cannot open file " + i, red); myexit(1)

	netf3 = []
	for i in netf2.splitlines():
		s = i.split()[9]
		if not s == "uid":
			netf3.append(s)
			uniq(netf3)
	
	# directory list
	for dir in os.listdir(d):
		d2 = os.path.join(d, dir, "fd")
		if os.path.isdir(d2):
			# list /proc/PID/fd
			for d3 in os.listdir(d2):
				d4 = os.path.join(d2, d3)
				# interested only in links
				if os.path.islink(d4) and os.path.exists(d4):
					flag_ok = 0
					try:
						d5 = os.readlink(d4)
						flag_ok = 1
					except: None
					if flag_ok:
						# choose the links starting with word socket only
						if d5.startswith("socket"):
							pid = dir
							# remove my PID from the list
							if not pid == mypid:
								sock = d5[8:-1]
								# if socket is one of the inodes
								if netf3.count(sock):
									f = os.path.join("/proc", pid, "exe")
									if os.path.islink(f) and os.path.exists(f):
										# store the link to binary
										flag_ok = 0
										try:
											f2 = os.readlink(f)
											flag_ok = 1
										except: None
										if flag_ok:
											if (not progs.count(f2)) and (not f2 in spec_prog):
												# print info once
												if flag2 == 0:
													flag2 = 1
													color("* new processes using network", yellow)
												# store it if not an exception
												progs.append(f2)
												color(f2, blue)
	

	# run policy check from time to time
	recheck += 1
	if recheck >= count:
		recheck = 0
		flag2 = 0
		flag_safe = 0
		check()
		# print only once
		if flag_firstrun:
			flag_firstrun = 0
			sl = ""
			# print time of sleeping period only when --once switch is not set
			if not opt_once: sl = ", sleeping " + str(count) + "s between every cycle"
			color("* whole running cycle took " + str(time.clock() - speed) + "s" + sl, green)
			if not opt_once: color("(press q to quit)", red)
		# now it's safe to enforce mode and save config on interrupt, cause check() finished running
		flag_safe = 1

		
		# if --once switch on then exit
		if opt_once:
			myfinish()


	# wait some time then rerun
	try:
#		time.sleep(1)
		# get 1 key and wait for it for 1 second
		try:
			key = getkey()
		finally:
			clearkey()
		# if key is "q" then quit
		if key == "q":
			print
			myfinish()


	except KeyboardInterrupt:
		print
		myfinish()

except KeyboardInterrupt:
	print
	myfinish()


# ----------------------------------------------------------------------

print
myfinish()
