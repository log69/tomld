#!/bin/bash

# CREATE CHROOT FOR PACKAGING

if ! whoami | grep -q root; then echo Run it as root.; echo; exit 1; fi

CHROOTDIR="/chroot-sid"
MYUSER="andras"
DEBIANVER="sid"
ARCH="amd64"
PKGS="dh-make devscripts lintian autotools-dev libjasper-dev libjpeg62-dev libpng12-dev autoconf2.13"


if ! [ -x "$CHROOTDIR" ]
then
	#apt-get clean
	#apt-get update
	#apt-get install debootstrap

	echo creating chroot...
	mkdir -p "$CHROOTDIR"

	# debootstrap --variant=minbase --arch $ARCH $DEBIANVER $CHROOTDIR http://ftp.de.debian.org/debian
	debootstrap "$DEBIANVER" "$CHROOTDIR" http://ftp.de.debian.org/debian

	# chroot "$CHROOTDIR" apt-get -y install $PKGS
	chroot "$CHROOTDIR" apt-get -y --force-yes install $PKGS
else
	echo chroot directory already exists.
	echo
fi


rm -rf "$CHROOTDIR"/home/"$MYUSER"

if ! grep -q "$MYUSER" "$CHROOTDIR"/etc/passwd; then grep "$MYUSER" /etc/passwd >> "$CHROOTDIR"/etc/passwd; fi
if ! grep -q "$MYUSER" "$CHROOTDIR"/etc/shadow; then grep "$MYUSER" /etc/shadow >> "$CHROOTDIR"/etc/shadow; fi

mkdir -p "$CHROOTDIR"/home/"$MYUSER"/development/
chown -R "$MYUSER":"$MYUSER" "$CHROOTDIR"/home/"$MYUSER"

echo "copying .gnupg..."
cp -a /home/"$MYUSER"/.gnupg "$CHROOTDIR"/home/"$MYUSER"/
echo "copying development/tomld..."
cp -a /home/"$MYUSER"/development/tomld "$CHROOTDIR"/home/"$MYUSER"/development/

echo
echo "creating debian package..."
echo

if [ "$LC_ALL" == "" ]; then export LC_ALL='C'; fi

chroot "$CHROOTDIR" su andras -c /home/andras/development/tomld/debian/debian_package.sh


BASE="/home/$MYUSER/development/tomld"

echo
#echo "deleting files..."
#rm -f "$BASE"/debian/tomld_*
echo "copying files back..."
cp "$CHROOTDIR""$BASE"/debian/tomld_*  "$BASE"/debian/
echo
echo done.