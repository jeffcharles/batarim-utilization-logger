#!/bin/bash
# Provide the CMAKE_SOURCE_DIR value as the first arg to this script
# Intended to run after this has been copied into build directory and make has
# been run

set -e

CMAKE_SOURCE_DIR="$1"
VERSION="$2"
LOGFILE_POSTFIX="$3"

# Populate debian-binary
mkdir package
echo "2.0" > package/debian-binary

# Initialize control variables and files
mkdir -p package/DEBIAN
md5sums=package/DEBIAN/md5sums
touch $md5sums

install_size=0

# Copy in data, tracking important variables
data_base="package"
usr_base="${data_base}/usr"
lib_base="${usr_base}/lib"
libbatarim="${lib_base}/libbatarim.so"
bin_base="${usr_base}/bin"
console="${bin_base}/batarim-console"
logger="${bin_base}/batarim-logger"
man_base="${usr_base}/share/man/man1"
console_manpage="${man_base}/batarim-console.1"
logger_manpage="${man_base}/batarim-logger.1"

mkdir -p $lib_base
cp ../src/viewmodel/libbatarim.so $libbatarim
install_size=$(expr $install_size + $(stat -c%s $libbatarim))
md5sum $libbatarim | sed "s/package\///" >> $md5sums

mkdir -p $bin_base
cp ../src/console/batarim-console $console
install_size=$(expr $install_size + $(stat -c%s $console))
md5sum $console | sed "s/package\///" >> $md5sums
cp ../src/logger/batarim-logger $logger
install_size=$(expr $install_size + $(stat -c%s $logger))
md5sum $logger | sed "s/package\///" >> $md5sums

mkdir -p $man_base
cp "${CMAKE_SOURCE_DIR}/manpages/batarim-console.1" $console_manpage
install_size=$(expr $install_size + $(stat -c%s $console_manpage))
md5sum $console_manpage | sed "s/package\///" >> $md5sums
cp "${CMAKE_SOURCE_DIR}/manpages/batarim-logger.1" $logger_manpage
install_size=$(expr $install_size + $(stat -c%s $logger_manpage))
md5sum $logger_manpage | sed "s/package\///" >> $md5sums

# Enter information into control file
control="package/DEBIAN/control"
cp control $control

sed -i "s/Version:/& ${VERSION}/" $control

arch=$(arch)
if [[ $arch = "i686" ]]; then # Debian packages do not permit i686
    arch="i386"
elif [[ $arch = "x64_86" ]]; then # Debian packages do not permit x64_86
    arch="amd64"
fi
sed -i "s/Architecture:/& ${arch}/" $control

install_size=$(expr $install_size / 1024)
sed -i "s/Installed-Size:/& ${install_size}/" $control

# Copy post-install and pre-remove scripts (amended with correct install paths)
postinst="package/DEBIAN/postinst"
cp "${CMAKE_SOURCE_DIR}/install_scripts/setup.sh" $postinst
sed -i "s/\(log_postfix=\).*/\1\"${LOGFILE_POSTFIX}\"/" $postinst
sed -i "s/usr\/local/usr/" $postinst
sed -i "/mandb/d" $postinst # package manager already runs this anyway
cp "${CMAKE_SOURCE_DIR}/install_scripts/teardown.sh" package/DEBIAN/prerm

# Build package
dpkg-deb --build package
mv package.deb "batarim_${VERSION}_${arch}.deb"

# Clean up directory
rm -r package
rm control
rm deb_build.sh
