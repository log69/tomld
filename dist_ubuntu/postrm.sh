#!/bin/sh
# change linux boot parameters in grub for tomoyo and prompt for reboot
# linux has to start with "security=tomoyo" kernel parameter to activate tomoyo

GRUB_DEFAULT="/etc/default/grub"
REBOOT_NOTIFY="/usr/share/update-notifier/notify-reboot-required"

# searching for grub settings
if [ -f "$GRUB_DEFAULT" ]; then

	# add kernel parameter to grub config
	echo "* checking grub config"
	if grep -E "^ *GRUB_CMDLINE_LINUX" "$GRUB_DEFAULT" | grep -qs "security=tomoyo"
	then
		sed -i s/"security=tomoyo"/""/g "$GRUB_DEFAULT"
		echo "* kernel parameter removed"

		# update grub
		if which update-grub >/dev/null; then
			echo "* updating grub"
			update-grub
		fi

		# prompt for reboot
		if [ -x "$REBOOT_NOTIFY" ]; then "$REBOOT_NOTIFY"; fi
		
		echo "* done"
		echo
		echo "*********************************************"
		echo "*** reboot is needed to deactivate tomoyo ***"
		echo "*********************************************"

	else
		echo "* kernel parameter not set"
	fi

else
	echo "* grub settings not found"

	# kernel started with tomoyo enabled?
	if ! grep -qs "security=tomoyo" "$KERNEL_CMDLINE"
	then
		echo "* kernel parameter not set"
		echo
		echo "*********************************************"
		echo "*** reboot is needed to deactivate tomoyo ***"
		echo "*********************************************"
	fi
fi
