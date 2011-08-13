#!/bin/sh
# update tomld debian package from net with latest version


TEMP=$(mktemp -d)
cd "$TEMP"

wget http://log69.com/downloads/tomld.tgz
tar xf tomld.tgz
if uname -m | grep -iq "x86_64"; then
	if lsb_release -d | grep -iq "ubuntu"; then
		sudo dpkg -i tomld/debian/tomld*amd64*.deb
	else
		su -c "dpkg -i tomld/debian/tomld*amd64*.deb"
	fi
else
	if lsb_release -d | grep -iq "ubuntu"; then
		sudo dpkg -i tomld/debian/tomld*i386*.deb
	else
		su -c "dpkg -i tomld/debian/tomld*i386*.deb"
	fi
fi

cd
rm -rf "$TEMP"
