#!/bin/bash

# CREATE CHROOT FOR PACKAGING

if ! whoami | grep -q root; then echo Run it as root.; echo; exit 1; fi

CHROOTDIR="/chroot-ubuntu-natty"
MYUSER="andras"
UBUNTUVER="natty"
ARCH="amd64"
PKGS="dh-make devscripts lintian autotools-dev autoconf libc6-dev-i386 rsync nano"


if ! [ -x "$CHROOTDIR" ]
then
	#apt-get clean
	#apt-get update
	#apt-get install debootstrap

	echo creating chroot...
	mkdir -p "$CHROOTDIR"

	debootstrap --arch "$ARCH" "$UBUNTUVER" "$CHROOTDIR" http://de.archive.ubuntu.com/ubuntu

	mount --bind /proc "$CHROOTDIR"/proc
	chroot "$CHROOTDIR" apt-get update
	chroot "$CHROOTDIR" apt-get -y --force-yes install $PKGS
else
	echo chroot directory already exists.
	echo
fi

rm -rf "$CHROOTDIR"/home/"$MYUSER"

if ! grep -q "$MYUSER" "$CHROOTDIR"/etc/passwd; then grep "$MYUSER" /etc/passwd >> "$CHROOTDIR"/etc/passwd; fi
if ! grep -q "$MYUSER" "$CHROOTDIR"/etc/group;  then grep "$MYUSER" /etc/group  >> "$CHROOTDIR"/etc/group;  fi
if ! grep -q "$MYUSER" "$CHROOTDIR"/etc/shadow; then grep "$MYUSER" /etc/shadow >> "$CHROOTDIR"/etc/shadow; fi

mkdir -p "$CHROOTDIR"/home/"$MYUSER"/development/
chown -R "$MYUSER":"$MYUSER" "$CHROOTDIR"/home/"$MYUSER"

echo "copying .gnupg..."
cp -a /home/"$MYUSER"/.gnupg "$CHROOTDIR"/home/"$MYUSER"/
ln -sf /home/"$MYUSER"/.gnupg "$CHROOTDIR"/root/
echo "copying development/tomld..."
cp -a /home/"$MYUSER"/development/tomld "$CHROOTDIR"/home/"$MYUSER"/development/

echo
echo "creating ubuntu package..."
echo

if [ "$LC_ALL" == "" ]; then export LC_ALL='C'; fi

ln -sf /usr/bin/strip "$CHROOTDIR"/usr/bin/i486-linux-gnu-strip
chroot "$CHROOTDIR" su andras -c /home/andras/development/tomld/dist_ubuntu/package.sh


BASE="/home/$MYUSER/development/tomld"


cd "$BASE"/dist_ubuntu/
echo
echo "deleting files..."
rm -f tomld_*.*
echo "copying files back..."
cp -f "$CHROOTDIR""$BASE"/dist_ubuntu/tomld_*  ./
#cp -f "$CHROOTDIR""$BASE"/dist_ubuntu/tomld_*.deb  ./
cp -f "$CHROOTDIR""$BASE"/dist_ubuntu/changelog  ./
chown andras:andras tomld_* changelog

echo
echo done.
