#!/bin/sh

TEMP=$(mktemp -d)
cd "$TEMP"

wget http://log69.com/downloads/tomld.tgz
tar xf tomld.tgz
if grep -q amd64 /proc/version; then
	dpkg -i tomld/debian/tomld*amd64*.deb
else
	dpkg -i tomld/debian/tomld*i386*.deb
fi

cd
rm -rf "$TEMP"
