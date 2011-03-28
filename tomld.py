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
# 28/03/2011 - tomld v0.22 - speedup info and remove functions
#                          - add full recursive directory subtitution with wildcards
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
#	3)	while counter-- goto 2
#	4)	check prolicy for domain and all its subdomains
#		if disabled mode
#			turn on learning mode
#		if learning mode
#			no change
#		if permissive mode
#			no change
#		if enforcing mode
#			no change
#	5) get recent log
#	6) convert logs to rules and add them to policy
#	7) switch back domains with access deny from enforcing to learning mode until exit only
#	8) reshape policy rules
#	9) goto 2
#	10) on exit, turn all old domains with profile 1-2 into enforcing mode



import os, sys
import time, re
import platform
import termios, fcntl, select


# **************************
# ******* VARIABLES ********
# **************************

# program version
ver = "0.22"

# home dir
home = "/home"

# policy check interval
global count; count = 10

# max entries variable in profile config
global maxe; maxe = 10000

# tomoyo kernel parameter
global tkern; tkern = "security=tomoyo"
# tomoyo package names (debian, ubuntu)
global tpak1d; tpak1d = "linux-patch-tomoyo1.7"
global tpak2d; tpak2d = "tomoyo-tools"

# tomoyo files
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

# this stores the kernel time of last line of the system log
# to identify it and make sure not to read it twice
global tmarkf
global tmark; tmark = "/var/local/tomld.logmark"
# tomld pid file
global pidf;  pidf  = "/var/run/tomld.pid"

# options
global opt_color;		opt_color		= 0
global opt_reset;		opt_reset		= 0
global opt_clear;		opt_clear		= 0
global opt_info;		opt_info		= 0
global opt_info2;		opt_info2		= ""
global opt_remove;		opt_remove		= 0
global opt_remove2;		opt_remove2		= 0
global opt_yes;			opt_yes			= 0
global opt_keep;		opt_keep		= 0
global opt_recursive;	opt_recursive	= 0
global opt_once;		opt_once		= 0

global flag_reset;  flag_reset  = 0
global flag_check;  flag_check  = 0
global flag_check2; flag_check2 = 0
global flag_check3; flag_check3 = 0
global flag_firstrun; flag_firstrun = 0
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

# other
# all binaries with domains
global progs; progs = []
# binaries only with newly created learning mode
# these won't have enforcing mode turned on on exit
global progl; progl = []
global speed; speed = 0

# supported platforms
supp = ["debian 6.", "ubuntu 10.10."]

# this will contain the dirs to be fully wildcarded resursively with --recursive switch on
global specr;  specr  = []
# this will show the full dir depths in numbers to all recursive dirs
global specr2_count; specr2_count = []

# special dirs - the leaf dirs or files get wildcarded in any subdirs of them
global spec; spec = [home + "/", "/usr/share/", "/etc/fonts/", "/var/cache/"]

# just like the above, but these are the exception dirs
# exact match needs on these dirs for the leaf dir or file not to get wildcarded
global spec_ex; spec_ex = ["/etc/", home + "/\*/"]

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
	print "    -r   --remove [pattern] remove domains by pattern"
	print "         --yes              auto confirm with yes"
	print "    -k   --keep             don't change domain's mode for this run"
	print "                            (learning mode domains will stay so on exit)"
	print "    -R   --recursive [dirs] replace subdirs of these dirs in rules with wildcards"
	print "    -1   --once             quit after first cycle"
	print "                            (might be useful for scripts)"
	print
	print "*executables are additonal programs to create domains for"
	print
	print "supported platforms are:"
	for i in supp: print "  ", i[:-1]
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
	print "- the program can be stopped any time by Ctrl+C whereafter all old domains (learning and permissive)",
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
	print "- stop it now, cause the domains will stay in learning mode and the tomoyo module will",
	print "collect all rules, even if the whole system is restarted"
	print "- reboot the system or at least all the services and applications that have domains now"
	print "- hours or days later let's run it the 3rd time, now the access denied logs will be converted to rules,",
	print "and on exit all remaining domains will be turned into enforcing mode"
	print


# sort and uniq a list
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
def compare(d1, d2):
	e1 = d1.split("/")
	e2 = d2.split("/")
	l1 = len(e1)-1
	l2 = len(e2)-1
	if l1 == 0 or l2 == 0: return 0
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
def comparer(d1, d2):
	e1 = d1.split("/")
	e2 = d2.split("/")
	l1 = len(e1)-1
	l2 = len(e2)-1
	if l1 == 0 or l2 == 0 or l1 < l2: return ""
	# compare the dirs
	flag = 0
	for i in range(0, l2):
		c1 = e1[i]
		c2 = e2[i]
		if (not c1 == "\*") and (not c2 == "\*") and (not c1 == c2):
			return ""
	return d2
#	return 1


# check if package is installed (debian)
def package_d(name):
	# debian or ubuntu
	pl = platform.linux_distribution()[0].lower()
	if pl == "debian" or pl == "ubuntu":
		f = "/var/lib/dpkg/status"
		if os.path.isfile(f):
			try: f2 = open(f).read()
			except: color("error: cannot open file " + f2, red); myexit(1)
			p = re.search("^Package: " + name + "$\n^Status: install ok installed$", f2, re.MULTILINE)
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
				tdomf2 = tdomf + "\n\n<kernel>"
				r2 = re.findall(re.escape(i) + ".*?^(?=<kernel>)", tdomf2, re.M + re.I + re.DOTALL)
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
		f.write("")
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
			s = re.search("^" + r2, tprof, re.M)
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
	x = re.findall("^<kernel>.* +" + prog3 + " *$\n+use_profile +3 *$", tdomf, re.MULTILINE)
	# turn back learning mode form enforcing mode for binary
	if x:
		color(prog3 + "  ", blue, 1)
		color("switch back domain from enforcing to learning mode", red)
		for i in x:
			sd1 = re.sub("use_profile +3 *", "use_profile 1", i)
			sd2 = re.sub(i, sd1, tdomf)
			tdomf = sd2


# turn on enforcing mode for old domains only with profile 1-2 before exiting
def enforce():
	if not opt_keep:
		global progl
		global tdomf
		# look up all domain with profile 1 or 2
		x = re.findall("^<kernel> +/[^ ]+.*$\n+use_profile +[1-2] *$", tdomf, re.MULTILINE)
		x2 = []
		if x:
			# check if it's not a standalone exception and not a domain with newly created learning mode
			for i in x:
				p = re.search("/[^ ]*$", i, re.MULTILINE).group()[:-1]
				p2 = re.search("^<kernel> +" + p + " *$", p, re.MULTILINE)
				if not p in progl:
					if not p in spec_prog or ((p in spec_prog) and (not p2)):
						x2.append(i)
		# check if there are old domains with enforcing mode
		flag_old = 0
		if re.search("^<kernel> +/[^ ]+.*$\n+use_profile +3 *$", tdomf, re.MULTILINE): flag_old = 1
		# turn on enforcing mode for domains
		if x2:
			flag_old2 = 0
			for i in x2:
				p = re.search("^<kernel> +/[^ ]+ *$", i, re.MULTILINE)
				if p:
					if not flag_old2: color("* turn on enforcing mode for old domains", green); flag_old2 = 1
					# get only binary name
					p2 = re.search("/[^ ]+", p.group(), re.MULTILINE)
					# print it
					color(p2.group(), blue)
				sd1 = re.sub("use_profile +[1-2] *", "use_profile 3", i)
				sd2 = re.sub(i, sd1, tdomf)
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
		r1 = re.findall("^<kernel>.*" + text + ".*$", tdomf, re.M + re.I)
		if r1:
			print
			# how many domains are found?
			for i in r1:
				color(i, blue)
				# it's needed to mark the last domain's end too
				tdomf2 = tdomf + "\n\n<kernel>"
				r2 = re.findall("^" + i + ".*?^(?=<kernel>)", tdomf2, re.M + re.I + re.DOTALL)
				if r2:
					for i2 in r2:
						i3 = re.search("use_profile.*", i2, re.M + re.DOTALL)
						if i3: color(i3.group(), red)
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
			r3 = []
			for i in r:
				r2 = re.search("[^ ]+$", i, re.M)
				if r2:
					r4 = r2.group()
					r5 = r4
					if r4[-1] == "\n": r5 = r4[:-1]
					if not r5 in r3:
						r3.append(r5)
			# print result
			print
			r3.sort()
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
		r1 = re.findall("^<kernel>.*" + text + ".*$", tdomf, re.M + re.I)
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
					r2 = re.findall("^" + i + ".*?^(?=<kernel>)", tdomf2, re.M + re.I + re.DOTALL)
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
						r6 = re.sub(".* +" + p3, "", texcf)
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


# print domain stat
def stat():
	global tdomf
	d = re.findall("^<kernel>.+$\n+use_profile +[1-3] *$", tdomf, re.M)
	r = re.findall("^allow_", tdomf, re.M)
	color(str(len(d)) + " active domains, " + str(len(r)) + " rules")


# my finish
def myfinish():
	# check if everything was run at least once before turning on enforcing mode
	if flag_safe:
		# turn on enforcing mode for all old domains before exiting
		enforce()
		# save config
		save()
		stat()
	else:
		color("* haven't finished to run at least once", red)
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
	global flag_firstrun
	if not flag_check:
		flag_check = 1
		x = re.findall("^<kernel> +/[^ ]+ *$\n+use_profile +[1-3] *$", tdomf, re.MULTILINE)
		if x:
			for i in x:
				p = re.search("^<kernel> .*$", i, re.MULTILINE).group()[9:]
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
	if not flag_firstrun: color("* checking policy and rules", yellow)
	# print dots
	else: sys.stdout.write("."); sys.stdout.flush()

	for prog in progs:
		# does the domain exist for the program?
		x = re.search("^initialize_domain *" + prog + " *$", texcf, re.MULTILINE)
		if x:
			# pritn only once
			if not flag_firstrun:
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
		x = re.findall("^<kernel> +" + prog + " *$\n+use_profile +[0-9]+ *$", tdomf, re.MULTILINE)
		if not x:
			color(", no rule", "", 1)
			print ", ",; color("create rule with learning mode on", green, 1)
			if flag_firstrun: print
			# create a rule for it
			tdomf += "\n<kernel> " + prog + "\n\nuse_profile 1\n\n"
			# store prog name to know not to turn on enforcing mode for these ones
			progl.append(prog)
		else:
			# only 1 rule should exist
			if len(x) > 1: error_conf("101")
			# print only once
			if not flag_firstrun:
				color(", rule exists", "", 1)
			# search for all subdomains too
			xx = re.findall("^<kernel> +" + prog + ".*$\n+use_profile +[0-9]+ *$", tdomf, re.MULTILINE)
			if not xx: error_conf("102")

			# cycle through the domain and its subdomains
			for d in xx:
				# get subdomain binary
				prog2 = re.search(prog + ".*$", d, re.MULTILINE).group()
				# get mode
				x2 = re.findall("use_profile +[0-9]+", d, re.MULTILINE)
				if len(x2) > 1: error_conf("103")
				x3 = re.findall("[0-9]+", x2[0], re.MULTILINE)
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
						if flag_firstrun: print
					# if in disabled mode, turn on learning mode for the domain or subdomain
					sd1 = re.sub("use_profile +0 *", "use_profile 1", d)
					sd2 = re.sub(d, sd1, tdomf)
					tdomf = sd2

				if profile == 1:
					if not flag_firstrun:
						if prog2 == prog: color(", learning mode on", "", 1)

				if profile == 2:
					if not flag_firstrun:
						if prog2 == prog: color(", permissive mode on", "", 1)

				if profile == 3:
					# enfocing mode should never be switched back off once it's on
					if not flag_firstrun:
						if prog2 == prog: print ", ",; color("enforcing mode on", purple, 1)

		# new line
		if not flag_firstrun: print


# ----------------------------------------------------------------------

	# ************************
	# ******* GET LOG ********
	# ************************

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
	s = re.findall(".* TOMOYO-ERROR: +.*$", log, re.MULTILINE)
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
		log3 = re.findall(".* TOMOYO-ERROR: Access \'.*$", logall, re.M)
		log4 = re.findall(".* TOMOYO-ERROR: Domain \'.*$", logall, re.M)

		# log type 1
		if log3:
			# collect logs and create rules from it
			for i in log3:
				s1 = re.sub(".* TOMOYO-ERROR: Access \'", "", i)
				s2 = re.sub("\' denied for .*$", "", s1)
				prog2 = re.sub(".*\' denied for ", "", s1)
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
					xx = re.findall("^<kernel>.* " + prog4[i] + "$\n+use_profile +3 *$", tdomf, re.MULTILINE)
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
								s = re.sub(d, d + "\n" + rule4[i], tdomf)
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
				s1 = re.sub(".* TOMOYO-ERROR: Domain \'", "", i)
				s2 = re.sub("\' not defined\..*$", "", s1)
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

	# ******************************
	# ******* RESHAPE RULES ********
	# ******************************


	# sort and uniq all rules in all domains
	tdomf3 = ""
	tdomf2 = tdomf + "\n\n<kernel>"
	r = re.findall("^<kernel>.*?^(?=<kernel>)", tdomf2, re.M + re.DOTALL)
	if r:
		# cycle through domains
		for i in r:
			# cycle through lines of domain
			r2 = []
			for i2 in i.splitlines(1):
				if not i2 == "\n":
					if not i2[0:6] == "allow_":
						tdomf3 += i2
					else:
						if i2 not in r2:
							r2.append(i2)
				
			r2.sort()
			tdomf3 +=  "".join(r2)
			tdomf3 += "\n"

		tdomf = tdomf3


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

				# more than 1 parameter? it means 1 or 2 dirs
				if l2 == 2:
					ind1 = 0
					# is the dir in specr?
					for i3 in specr:
						dir1 = comparer(r[1], i3)
						if dir1: break
						ind1 += 1
					# if so
					if dir1:
						c = specr2_count[ind1]
						if c > 0:
							s = ""
							for i4 in range(1, c):
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
						dir1 = comparer(r[1], i3)
						if dir1: break
						ind1 += 1
					# is the dir2 in specr?
					for i3 in specr:
						dir2 = comparer(r[2], i3)
						if dir2: break
						ind2 += 1

					# if any of them yes and refer to the same recursive dir
					if dir1 or dir2:
				
#						if dir1 and (not dir2):
#							c = specr2_count[ind1]
#							if c > 0:
#								s = ""
#								for i4 in range(1, c):
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
#								for i4 in range(1, c):
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
								for i4 in range(1, c):
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


	# the spec predefined dirs and those dirs that have files newly created in them
	# will be wildcarded

	# there are 2 runs: first is to collect all the dir names that have create entries
	# second run is to make all entries containing the former ones wildcarded
	# and do this overall because it cannot be known if one non-uniq filname will be reused
	# by other processes
	spec2 = []
	spec3 = []

	# these are the special create entries, where the place of the file will be wildcarded
	# because it cannot be determined fully yet if the file being created has a uniq filename
	# or a contantly changing one (temporary name)
	cre  = ["allow_create", "allow_mksock", "allow_rename", "allow_unlink", "allow_mkdir"]

	# allow_mkdir will also have a special handling, cause usually files are created in the
	# new dir too, and it cannot be surely told if the dir itself has a uniq name too,
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
					r = re.search("^/.+/", i2[i3], re.MULTILINE)
					if r:
						r2 = r.group()
						flag = 0
						for i4 in spec2:
							if compare(r2, i4):
								flag = 1
								break
						if not flag:
							spec2.append(r2)

			# is it a special allow_mkdir line?
			if i2[0] in cre2:
				# check params from number 2 (first is only the allow_)
				for i3 in range(1, l):
					# get the dir name only (cannot be root dir /)
					r = re.search("^/.+/", i2[i3], re.MULTILINE)
					if r:
						r2 = r.group()
						flag = 0
						for i4 in spec3:
							if compare(r2, i4):
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
				if s == "allow_ioctl":
					for i3 in range(1, l):
						param = i2[i3]
						r = re.sub("socket:\[[0-9]+\]", "socket:[\\\\$]", param)
						param = r

						s += " " + param

				# handle params that are dirs
				else:
					# check params from number 2 (first is only the allow_)
					for i3 in range(1, l):

						# is param dir among spec or spec2 or specr dirs?
						# only one of them is enough to find
						
						param = i2[i3]
						param2 = ""
						flag  = 0
						flag3 = 0


						flag_ex = 0
						# check dir in exception						
						if not flag_ex:
							r = re.search("^/.+/", param, re.MULTILINE)
							if r:
								r5 = r.group()
								for i5 in spec_ex:
									if compare(r5, i5):
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
								# get the dir name
								r = re.search("^/.+/", param, re.MULTILINE)
								if r:
									# let's watch out for the "\*" wildcard and compare dirs like that
									r5 = r.group()
									for i5 in spec2:
										if compare(r5, i5):
											flag = 1
											break

						# is it in spec3?
						if not flag_ex:
							if not flag3:
								# get the dir name
								r = re.search("^/.+/", param, re.MULTILINE)
								if r:
									# let's watch out for the "\*" wildcard and compare dirs like that
									r5 = r.group()
									for i5 in spec3:
										if compare(r5, i5):
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
						if re.search("/lib.+[\.0-9]*\.so[\.0-9]*$", param, re.MULTILINE):
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
						i4 = re.sub("^" + home + "/[^/]+/", home + "/\\\\*/", param)
						param = i4

						s += " " + param

				i = s + "\n"

		tdomf2 += i

	tdomf = tdomf2

	# iterate through all the rules and reshape them
	# all entries with allow_create will be recreated with allow_unlink and allow_read/write entries too
	# cause there are deny logs frequently coming back for the created files trying to be written and unlinked
	# what is created should be allowed to be written or unlinked
	tdomf2 = ""
	for i in tdomf.splitlines(1):
		
		# operate only on rules
		if i[0:13] == "allow_create ":
			i2 = re.sub("allow_create ", "allow_read/write ", i)
			i3 = re.sub("allow_create ", "unlink ", i)
			i = i + i2 + i3

		tdomf2 += i

	tdomf = tdomf2
	
	save()


# ----------------------------------------------------------------------

# *********************
# ******* INIT ********
# *********************

# check command line options
opt_all = ["-v", "--version", "-h", "--help", "--clear", "--reset", "-c", "--color", \
		   "-i", "--info", "-r", "--remove", "--yes", "-k", "--keep", "-R", "--recursive", \
		   "-1", "--once"]
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
				print d2
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
print "platform is", pl

# check if it's supported platform
flag = 0
for i in supp:
	l1 = len(i)
	l2 = len(pl)
	if l1 < l2:
		if pl.startswith(i): flag = 1
	else:
		if i.startswith(pl): flag = 1
if flag == 0:
	color("error: this platform is unsupported", red)
	print "supported platforms are:"
	for i in supp: print "  ", i[:-1]
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
				p0 = re.search("^GRUB_CMDLINE_LINUX=\".*" + tkern + ".*\"$", f2, re.MULTILINE)
				if not p0:
					p = re.search("^GRUB_CMDLINE_LINUX=\".*\"$", f2, re.MULTILINE)
					if p:
						p1 = p.group()
						p2 = re.sub("\"$", " " + tkern + "\"", p1)
						p3 = re.sub(p1, p2, f2)
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
					d5 = os.readlink(d4)
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
									f2 = os.readlink(f)
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
		check()
		# print only once
		if not flag_firstrun:
			sl = ""
			# print time of sleeping period only when --once switch is not set
			if not opt_once: sl = ", sleeping " + str(count) + "s between every cycle"
			color("* whole running cycle took " + str(time.clock() - speed) + "s" + sl, green)
			flag_firstrun = 1
		# now it's safe to enforce mode on interrupt
		flag_safe = 1

		
		# if --once switch on then exit
		if opt_once:
			myfinish()


	# wait some time then rerun
	try:
		time.sleep(1)
	except KeyboardInterrupt:
		print
		myfinish()

except KeyboardInterrupt:
	print
	myfinish()


# ----------------------------------------------------------------------

print
flag_safe = 1
myfinish()
