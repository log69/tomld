#!/bin/sh
# change linux boot parameters in grub for tomoyo and prompt for reboot
# linux has to start with "security=tomoyo" kernel parameter to activate tomoyo

GRUB_DEFAULT="/etc/default/grub"
KERNEL_CMDLINE="/proc/cmdline"
REBOOT_NOTIFY="/usr/share/update-notifier/notify-reboot-required"

# searching for grub settings
if [ -f "$GRUB_DEFAULT" ]; then

	# add kernel parameter to grub config
	echo "* checking grub config"
	if ! grep -E "^ *GRUB_CMDLINE_LINUX" "$GRUB_DEFAULT" | grep -q "security=tomoyo"
	then
		if grep -qE "^ *GRUB_CMDLINE_LINUX *=" "$GRUB_DEFAULT"
		then
			sed -i -r s/"^ *GRUB_CMDLINE_LINUX *= *\""/"GRUB_CMDLINE_LINUX=\"security=tomoyo "/ "$GRUB_DEFAULT"
		else
			echo "GRUB_CMDLINE_LINUX=\"security=tomoyo\"" >> "$GRUB_DEFAULT"
		fi
		echo "* kernel parameter added"

		# update grub
		if which update-grub >/dev/null; then
			echo "* updating grub"
			update-grub
		fi

		# prompt for reboot
		if [ -x "$REBOOT_NOTIFY" ]; then "$REBOOT_NOTIFY"; fi
		
		echo "* done"
		echo
		echo "*****************************************************"
		echo "*** reboot is needed to activate tomoyo for tomld ***"
		echo "*****************************************************"

	else
		echo "* kernel parameter already set"
	fi

else
	echo "* grub settings not found"

	# kernel started with tomoyo enabled?
	if ! grep -q "security=tomoyo" "$KERNEL_CMDLINE"
	then
		echo "* kernel parameter 'tomoyo=security' has to be specified on boot manually"
		echo
		echo "*****************************************************"
		echo "*** reboot is needed to activate tomoyo for tomld ***"
		echo "*****************************************************"
	fi
fi
