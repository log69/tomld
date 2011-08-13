#!/bin/sh

if ! whoami | grep -q root; then echo "error: root privileges needed"; exit 1; fi

zypper in gcc make tomoyo-tools

/usr/lib64/tomoyo/init_policy


TEMP=$(mktemp -d)
cd "$TEMP"

wget log69.com/downloads/tomld.tgz

tar xf tomld.tgz

cd tomld

cp -f dist_opensuse/12.1/tomld.init .

make

make install

cd
rm -rf "$TEMP"
