#!/bin/sh
# tomld opensuse script to install from source

if whoami | grep -q root; then echo "error: no root privileges needed"; exit 1; fi

# check dependencies
if ! which gcc >/dev/null;  then sudo zypper in gcc;  fi
if ! which make >/dev/null; then sudo zypper in make; fi
if [ ! -x "/usr/sbin/tomoyo-loadpolicy" ]; then
	sudo zypper in tomoyo-tools;
	/usr/lib64/tomoyo/init_policy
fi

# installation
TEMP=$(mktemp -d)
cd "$TEMP"

wget log69.com/downloads/tomld.tgz

tar xf tomld.tgz

cd tomld

cp -f dist_opensuse/12.1/tomld.init .

make

sudo make install

cd
rm -rf "$TEMP"

# grub update, add tomoyo kernel parameter to boot
# change linux boot parameters in grub for tomoyo and prompt for reboot
# linux has to start with "security=tomoyo" kernel parameter to activate tomoyo
GRUB="/boot/grub/menu.lst"
KERNEL_CMDLINE="/proc/cmdline"
SETTING="/etc/sysconfig/bootloader"
ENTRY="DEFAULT_APPEND"
PARAM="security=tomoyo"

# searching for grub settings
if [ -f "$GRUB" ]; then

	# add kernel parameter to grub config
	echo "* checking grub config"
	if ! sudo grep "kernel" "$GRUB" | grep -q "$PARAM"
	then
		echo "* add kernel parameter"

		if sudo grep -q "$ENTRY" "$SETTING"
		then
			if ! sudo grep "$ENTRY" "$SETTING" | grep -q "$PARAM"
			then
				sudo sed -ir s/"$ENTRY *= *\""/"$ENTRY=\"$PARAM "/ "$SETTING"
			fi
		else
			echo "$ENTRY=\"$PARAM\"" | sudo tee -a "$SETTING"
		fi

		sudo /usr/lib/bootloader/bootloader_entry add    $(uname -r | grep -oE "[^\-]*$") $(uname -r) vmlinuz initrd
		
		sudo sed -ir s/"^default *[0-9]*$"/"default 0"/ "$GRUB"

		echo "* done"
		echo "* kernel parameter added"
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
	if ! grep -q "$PARAM" "$KERNEL_CMDLINE"
	then
		echo "* kernel parameter '$PARAM' has to be specified on boot manually"
		echo
		echo "*****************************************************"
		echo "*** reboot is needed to activate tomoyo for tomld ***"
		echo "*****************************************************"
	fi
fi
