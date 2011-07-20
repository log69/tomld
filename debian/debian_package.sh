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
cp ./* "$TEMP"/tomld-"$VER"/
cd "$TEMP"
tar cfz tomld_"$VER".tar.gz tomld-"$VER" --exclude ".git"

cd tomld-"$VER"

dh_make -f ../tomld_"$VER".tar.gz --single -e mail@log69.com -p tomld -c gpl

cd debian


#rm -f compat watch.ex
rm -f postinst.ex postrm.ex preinst.ex prerm.ex
rm -f tomld.default.ex tomld.doc-base.EX cron.d.ex
rm -f emacsen-install.ex emacsen-remove.ex emacsen-startup.ex
rm -f init.d.ex init.d.lsb.ex manpage.1.ex manpage.sgml.ex manpage.xml.ex
rm -f menu.ex README.Debian README.source tomld.cron.d.ex

echo "usr/sbin" 			> dirs

cp ../tomld.control ./control
cp ../tomld.manual ./tomld.1
echo "debian/tomld.1" > ./tomld.manpages

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


#echo "3.0 (quilt)" > source/format


# changelog file

	head -n1 changelog > changelog.new
	echo >> changelog.new
	echo "  * New upstream release" >> changelog.new
	echo >> changelog.new
	echo " -- Andras Horvath <mail@log69.com>  "$(date -R) >> changelog.new
#	tail -n2 changelog >> changelog.new
	mv changelog.new changelog



sed -i s/"dh_installman"/"dh_installman debian\/tomld.1"/ rules

grep -i "^version=" watch.ex > watch
echo "http://log69.com/tomld.html downloads/tomld_v(.*)\.tar\.gz" >> watch
rm -f watch.ex

cd ..


#dpkg-buildpackage -rfakeroot -k7CA53418
dpkg-buildpackage -k7CA53418


cd ..
# echo
# echo "--- LINTIAN OUTPUT ---"
# lintian *.deb
# lintian *.changes
# echo "----------------------"


cp tomld_*  "$DEB"/
chown andras:andras "$DEB"/tomld_*
rm -rf "$TEMP"


exit 0
