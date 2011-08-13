#!/bin/bash
# create debian package of tomld


# cd to sources
cd $(dirname $0);
DEB=$(pwd)
cd ..
BASE=$(pwd)
TEMP=$(mktemp -d)

# determine latest upstream version
VER=$(grep -E "^char *\*ver *= *" ./tomld.c | grep -Eo "[0-9]+[0-9\.]*[0-9]+")

# create tomld.tgz
echo
echo "* creating tomld.tgz..."
(make clean) &>/dev/null
mkdir "$TEMP"/tomld-"$VER"
rsync -a --exclude ".git" ./* "$TEMP"/tomld-"$VER"/
cd "$TEMP"
tar cfz tomld_"$VER".orig.tar.gz tomld-"$VER" --exclude ".git"

cd tomld-"$VER"

dh_make -f ../tomld_"$VER".orig.tar.gz --single -e mail@log69.com -p tomld -c gpl

cd debian


#rm -f compat watch.ex init.d.ex postinst.ex
rm -f preinst.ex prerm.ex
rm -f tomld.default.ex tomld.doc-base.EX cron.d.ex
rm -f emacsen-install.ex emacsen-remove.ex emacsen-startup.ex
rm -f init.d.ex init.d.lsb.ex manpage.1.ex manpage.sgml.ex manpage.xml.ex
rm -f README.Debian README.source tomld.cron.d.ex menu.ex


# create application menu with .desktop file
#cp ../tomld.desktop .
#echo "" >> rules
#echo "tomld.desktop:" >> rules
#echo "dh_install tomld.desktop usr/share/applications" >> rules
#echo "dh_installmenu" >> rules

# create debian menu
#echo -n '?package(tomld):needs="text" section="Applications/System/Administration"' > menu.ex
#echo ' title="Tomld learning mode" command="so-to-root -X -- /usr/sbin/tomld --learn"' >> menu.ex
#mv menu.ex menu

# manage postinst
grep -E -m 1 -B 1000 "^ *configure\)" postinst.ex > postinst
cat ../dist_debian/postinst.sh | tail -n+4 >> postinst
grep -E -m 1 -A 1000 "^ *configure\)" postinst.ex | tail -n+2 >> postinst
rm postinst.ex

# manage postrm
grep -E -m 1 -B 1000 "^ *purge.remove.*\)" postrm.ex > postrm
cat ../dist_debian/postrm.sh | tail -n+4 >> postrm
grep -E -m 1 -A 1000 "^ *purge.remove.*\)" postrm.ex | tail -n+2 >> postrm
rm postrm.ex

# manage .desktop files (in rules file, lines must start with tabulator and not space)
cp ../tomld_learn.desktop .
cp ../tomld_notify.desktop .
cp ../tomld.svg .
echo "" >> rules
echo "override_dh_auto_install:" >> rules
echo "	dh_install ./tomld_learn.desktop usr/share/applications" >> rules
echo "	dh_install ./tomld_notify.desktop usr/share/applications" >> rules
echo "	dh_install ./tomld.svg usr/share/pixmaps" >> rules
echo "	dh_auto_install" >> rules



> dirs
echo "usr/sbin" 			>> dirs
echo "etc/init.d" 			>> dirs
echo "etc/tomld" 			>> dirs
echo "usr/share/man/man1" 	>> dirs
echo "usr/share/pixmaps" 	>> dirs
echo "usr/share/applications" 	>> dirs


# AUTHORS and COPYRIGHT not needed because of copyright file
# COPYING and LICENSE not needed either
> docs
echo "README"		>> docs
echo "FAQ"			>> docs

cp ../dist_debian/control .
#cp ../tomld.manual ./tomld.1
#echo "debian/tomld.1" > ./tomld.manpages
cp ../tomld.init ./init.d


echo "This work was packaged for Debian by:" > copyright.new
echo 				>> copyright.new
echo "    Andras Horvath <mail@log69.com> on $(date -R)" >> copyright.new
echo >> copyright.new
echo "It was downloaded from http://log69.com" 	>> copyright.new
echo 				>> copyright.new
echo "Upstream Authors:" 	>> copyright.new
echo 				>> copyright.new
cat ../AUTHORS | sed -r s/"^"/"    "/	>> copyright.new
echo 				>> copyright.new
echo "Copyright:" 		>> copyright.new
echo 				>> copyright.new
cat ../COPYRIGHT | sed -r s/"^"/"    "/	>> copyright.new
echo 				>> copyright.new
echo "License:" 		>> copyright.new
echo 				>> copyright.new
cat ../LICENSE | sed -r s/"^"/"    "/	>> copyright.new
echo 				>> copyright.new
echo "On Debian systems, the complete text of the GNU General" >> copyright.new
echo "Public License can be found in \`/usr/share/common-licenses/GPL'." >> copyright.new
echo 				>> copyright.new
echo "The Debian packaging is Copyright 2011, Andras Horvath <mail@log69.com>" >> copyright.new
echo "and is licensed under the GPL, see above." >> copyright.new
mv copyright.new copyright


echo -n "New release? [y/N] "; read -n1 KEY; echo
if [ "$KEY" != "y" ]
then

	cp -f "$DEB"/changelog .

else
# changelog file

	cp -f "$DEB"/changelog .

	head -n1 changelog > changelog.new
	echo >> changelog.new
	echo "  * New upstream release" >> changelog.new
	echo >> changelog.new
	echo " -- Andras Horvath <mail@log69.com>  "$(date -R) >> changelog.new
#	tail -n2 changelog >> changelog.new
	echo >> changelog.new
	cat changelog >> changelog.new
	
	mv changelog.new changelog
	ne changelog

	cp changelog "$DEB"

fi
sed -i "0,/Andras Horvath <mail@log69.com>.*/s/Andras Horvath <mail@log69.com>.*/Andras Horvath <mail@log69.com>  $(date -R)/" changelog



grep -i "^version=" watch.ex > watch
echo "http://log69.com/tomld.html downloads/tomld_v(.*)\.tar\.gz" >> watch
rm -f watch.ex


cd ..


# create 64 bit build .deb package
debuild -k7CA53418 -us -uc
cp -f ../tomld_*  "$DEB"/


# cleanup
make clean &>/dev/null
rm -rf debian/tomld/
rm -f debian/tomld.*
rm -f ../tomld_*.dsc ../tomld_*.changes

# create another orig with modified Makefile
cd ..
mv tomld-"$VER" tomld.bak
tar xf tomld_"$VER".orig.tar.gz
sed -i s/"CFLAGS = "/"CFLAGS = -m32 "/ tomld-"$VER"/Makefile
cp -f tomld-"$VER"/Makefile tomld.bak/Makefile
tar cfz tomld_"$VER".orig.tar.gz tomld-"$VER"
rm -rf tomld-"$VER"
mv tomld.bak tomld-"$VER"
cd tomld-"$VER"

# create 32 bit build .deb package
linux32 debuild -k7CA53418 -ai386 -us -uc
#cp -f ../tomld_*i386.deb  "$DEB"/

# change dependency in 32 bit package from libc-i386 to libc
cd ..
TEMP2=$(mktemp -d)
NAME=$(ls tomld_*i386.deb)
cp "$NAME" "$TEMP2"
cd "$TEMP2"
mkdir -p extract/DEBIAN
dpkg-deb -x "$NAME" extract/
dpkg-deb -e "$NAME" extract/DEBIAN
sed -i s/" libc6-i386 "/" libc6 "/ extract/DEBIAN/control
mkdir build
dpkg-deb -b extract/ build/
cp -f build/"$NAME" "$DEB"/
cd


rm -rf "$TEMP" "$TEMP2"

exit 0
