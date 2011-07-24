#!/bin/sh
# change linux boot parameters in grub for tomoyo and prompt for reboot
# linux has to start with "security=tomoyo" kernel parameter to activate tomoyo

GRUB_DEFAULT="/etc/default/grub"
TDOMF="/etc/tomoyo/domain_policy.conf"
TEXCF="/etc/tomoyo/exception_policy.conf"

# add kernel parameter to grub config
echo "* searching for security=tomoyo kernel parameter in grub config"
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
	echo "* done"
	echo
	echo "*****************************************************"
	echo "*** reboot is needed to activate tomoyo for tomld ***"
	echo "*****************************************************"
else
	echo "* kernel parameter already set"
fi

# backup tomoyo config
echo "* backing up original tomoyo config"
test ! -f "$TDOMF" || mv "$TDOMF" "$TDOMF".bak.installer.$(date +%s)
test ! -f "$TEXCF" || mv "$TEXCF" "$TEXCF".bak.installer.$(date +%s)
