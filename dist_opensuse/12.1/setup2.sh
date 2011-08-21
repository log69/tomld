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

# grub update
# change linux boot parameters in grub for tomoyo and prompt for reboot
# linux has to start with "security=tomoyo" kernel parameter to activate tomoyo
GRUB_DEFAULT="/boot/grub/menu.lst"
KERNEL_CMDLINE="/proc/cmdline"

# searching for grub settings
if [ -f "$GRUB_DEFAULT" ]; then

	# add kernel parameter to grub config
	echo "* checking grub config"
	if ! sudo grep "kernel" "$GRUB_DEFAULT" | grep -q "security=tomoyo"
	then
		sudo sed -i s/"root="/"security=tomoyo root="/ "$GRUB_DEFAULT"
		echo "* kernel parameter added"

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
