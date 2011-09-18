#!/bin/sh
# update tomld debian package from net with latest version


TEMP=$(mktemp -d)
cd "$TEMP"

echo "* checking GPG key..."
if ! gpg --fingerprint 7CA53418 1>/dev/null 2>/dev/null; then
	echo "* importing GPG key..."
	if ! gpg --keyserver keyserver.ubuntu.com --recv-keys 7CA53418 1>/dev/null 2>/dev/null; then
		echo "error: could not import GPG key"
		exit 1
	fi
fi

D1="http://log69.com/downloads/SHA1SUMS_tomld"
D2="http://log69.com/downloads/SHA1SUMS_tomld.asc"
D3="http://log69.com/downloads/tomld.tgz"

echo "* downloading files..."
if wget "$D1" "$D2" "$D3" 2>&1 | grep -q "not retrieving"; then
	echo "error: could not download all files"
	exit 1
fi

echo "* checking GPG signature..."
if ! gpg -v *.asc 1>/dev/null 2>/dev/null; then
	echo "error: GPG singature check failed"
	exit 1
fi

echo "* checking hash integrity..."
if ! sha1sum -c SHA1SUMS_tomld 2>/dev/null | grep "OK$"; then
	echo "error: integrity check failed"
	exit 1
fi

tar xf tomld.tgz
if uname -m | grep -iq "x86_64"; then
	if lsb_release -d | grep -iq "ubuntu"; then
		sudo dpkg -i tomld/dist_debian/tomld*amd64*.deb
	else
		su -c "dpkg -i tomld/dist_debian/tomld*amd64*.deb"
	fi
else
	if lsb_release -d | grep -iq "ubuntu"; then
		sudo dpkg -i tomld/dist_debian/tomld*i386*.deb
	else
		su -c "dpkg -i tomld/dist_debian/tomld*i386*.deb"
	fi
fi

cd
rm -rf "$TEMP"
