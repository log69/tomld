#!/bin/sh
# tomld opensuse script to install from source

if whoami | grep -q root; then echo "error: no root privileges needed"; exit 1; fi

if ! which gcc;  then sudo zypper in gcc;  fi
if ! which make; then sudo zypper in make; fi
if [ -x "/usr/sbin/tomoyo-loadpolicy" ]; then
	sudo zypper in tomoyo-tools;
	/usr/lib64/tomoyo/init_policy
fi


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
