#!/bin/bash
# create ubuntu package of tomld


# cd to sources
cd $(dirname $0);
DEB=$(pwd)
cd ..
BASE=$(pwd)
TEMP=$(mktemp -d)

# determine latest upstream version
VER=$(grep -E "^char *\*ver *= *" ./tomld.c | grep -Eo "[0-9]+[0-9\.]*[0-9]+")

# copy original tomld source tar balls
echo
echo "* copy original tomld source tar balls..."
cp ../tomld_"$VER".tar.gz "$TEMP"/tomld_"$VER".orig.tar.gz
cd "$TEMP"
tar xf tomld_"$VER".orig.tar.gz
cd tomld-"$VER"

dh_make -f ../tomld_"$VER".orig.tar.gz --single -e mail@log69.com -p tomld -c gpl

cd debian


#rm -f compat watch.ex init.d.ex menu.ex
rm -f preinst.ex prerm.ex postinst.ex postrm.ex
rm -f tomld.default.ex tomld.doc-base.EX cron.d.ex
rm -f emacsen-install.ex emacsen-remove.ex emacsen-startup.ex
rm -f init.d.ex init.d.lsb.ex manpage.1.ex manpage.sgml.ex manpage.xml.ex
rm -f README.Debian README.source tomld.cron.d.ex


# create debian menu
echo -n '?package(tomld):needs="text" section="Applications/System/Administration"' > menu.ex
echo ' title="Tomld temporary learning mode" command="su-to-root -X -c '/usr/sbin/tomld --learn'"' >> menu.ex
#echo -n '?package(tomld):needs="text" section="Applications/System/Administration"' > menu.ex
#echo ' title="Tomld notification" command="/usr/sbin/tomld --notify"' >> menu.ex
mv menu.ex menu

# manage postinst
#grep -E -m 1 -B 1000 "^ *configure\)" postinst.ex > postinst
#cat ../dist_ubuntu/postinst.sh | tail -n+4 >> postinst
#grep -E -m 1 -A 1000 "^ *configure\)" postinst.ex | tail -n+2 >> postinst
#rm postinst.ex

# manage postrm
#grep -E -m 1 -B 1000 "^ *purge.*remove.*\)" postrm.ex > postrm
#sed -i s/"|upgrade|"/"|"/ postrm
#cat ../dist_ubuntu/postrm.sh | tail -n+4 >> postrm
#echo "    ;;" >> postrm
#echo "    upgrade)" >> postrm
#echo "    echo \"* upgrade\"" >> postrm
#grep -E -m 1 -A 1000 "^ *purge.*remove.*\)" postrm.ex | tail -n+2 >> postrm
#rm postrm.ex

# manage extra files
echo "tomld_learn.desktop /usr/share/applications" >> tomld.install
echo "tomld_notify.desktop /usr/share/applications" >> tomld.install
echo "tomld.svg /usr/share/icons" >> tomld.install
echo "tomld.xpm /usr/share/pixmaps" >> tomld.install

# manage logrotate
cp ../tomld.logrotate .


#> dirs
#echo "usr/sbin" 			>> dirs
#echo "etc/init.d" 			>> dirs
#echo "etc/tomld" 			>> dirs
#echo "usr/share/man/man1" 	>> dirs
#echo "usr/share/pixmaps" 	>> dirs
#echo "usr/share/applications" 	>> dirs


# AUTHORS and COPYRIGHT not needed because of copyright file
# COPYING and LICENSE not needed either
> docs
echo "FAQ"			>> docs
echo "README"		>> docs
echo "TODO"			>> docs

cp ../dist_ubuntu/control .
#cp ../tomld.manual ./tomld.1
#echo "debian/tomld.1" > ./tomld.manpages
cp ../tomld.init .


echo "This work was packaged for Ubuntu by:" > copyright.new
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
echo "The complete text of the GNU General Public License can be found in" >> copyright.new
echo "\`/usr/share/common-licenses/GPL'." >> copyright.new
echo 				>> copyright.new
echo "The Ubuntu packaging is Copyright 2011, Andras Horvath <mail@log69.com>" >> copyright.new
echo "and is licensed under the GPL, see above." >> copyright.new
mv copyright.new copyright


echo -n "New release? [Y/n] "; read -n1 KEY; echo
if [ "$KEY" == "n" ]
then

	cp -f "$DEB"/changelog .

else
# changelog file

	head -n1 changelog > changelog.new
	cp -f "$DEB"/changelog .

	echo >> changelog.new
	echo "  * New upstream release" >> changelog.new
	echo >> changelog.new
	echo " -- Andras Horvath <mail@log69.com>  "$(date -R) >> changelog.new
#	tail -n2 changelog >> changelog.new
	echo >> changelog.new
	cat changelog >> changelog.new
	
	mv changelog.new changelog

	# change unstable to natty
	sed -i s/"unstable"/$(lsb_release -c | cut -f2)/g changelog
	
#	nano changelog

	cp changelog "$DEB"

fi
sed -i "0,/Andras Horvath <mail@log69.com>.*/s/Andras Horvath <mail@log69.com>.*/Andras Horvath <mail@log69.com>  $(date -R)/" changelog



grep -i "^version=" watch.ex > watch
echo "http://log69.com/tomld.html downloads/tomld_(.*)\.tar\.gz" >> watch
rm -f watch.ex

# remove comments from rules file
grep -vE "^#[^\!]|^$" rules > rules.new
mv rules.new rules
chmod +x rules

cd ..


# ----------------------------------------------------------------
# create 64 bit build .deb package
#debuild -k7CA53418 -us -uc
#debuild -k7CA53418
debuild -k7CA53418 -S
cp -f ../tomld_*  "$DEB"/


# cleanup
make clean &>/dev/null
rm -rf debian/tomld/
rm -f debian/tomld*.debhelper* tomld.substvars files
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

## create 32 bit build .deb package
#linux32 debuild -k7CA53418 -ai386 -us -uc
##cp -f ../tomld_*i386.deb  "$DEB"/
## change dependency in 32 bit package from libc-i386 to libc
#cd ..
#TEMP2=$(mktemp -d)
#NAME=$(ls tomld_*i386.deb)
#cp "$NAME" "$TEMP2"
#cd "$TEMP2"
#mkdir -p extract/DEBIAN
#dpkg-deb -x "$NAME" extract/
#dpkg-deb -e "$NAME" extract/DEBIAN
#sed -i s/" libc6-i386 "/" libc6 "/ extract/DEBIAN/control
#mkdir build
#dpkg-deb -b extract/ build/
#cp -f build/"$NAME" "$DEB"/

cd
#rm -rf "$TEMP2"
rm -rf "$TEMP"

exit 0
