#!/bin/bash

# Variables
PWD=$(pwd)
BASE_DIR="$PWD/local/sfml"
DEPS_DIR="$BASE_DIR/deps"
INSTALL_DIR="$BASE_DIR/install"

FD_OUT=""

# check for quiet option
if [ "$1" == "-q" ]; then
	FD_OUT="/dev/null"
	echo "Quiet mode enabled."
else
	FD_OUT="/dev/stdout"
	echo "Verbose mode enabled."
fi


# Create directories
mkdir -p ${DEPS_DIR}
mkdir -p ${INSTALL_DIR}

# Helper function to install a library
function install_library {
    local url=$1
    local name=$2
    local extension=$3
    local configure_flags=$4

	set -e  # Exit immediately if a command exits with a non-zero status.

    # echo "Dowlnoading $name..."
    wget $url -O ${DEPS_DIR}/$name.$extension
    if [[ $extension == "tar.gz" ]]; then
        tar -xzf ${DEPS_DIR}/$name.$extension -C ${DEPS_DIR}
    elif [[ $extension == "tar.xz" ]]; then
        tar -xf ${DEPS_DIR}/$name.$extension -C ${DEPS_DIR}
    fi

    cd ${DEPS_DIR}/$name-*
	# echo "Configure $name..."
    ./configure --prefix=${INSTALL_DIR} $configure_flags
    # echo "Compile and install $name..."
    make -s -j$(nproc)
    make -s install

    cd ${DEPS_DIR}
	set +e  # Disable exit immediately if a command exits with a non-zero status.
}

# Download and install dependencies
echo "Dowlnoading libogg..."
install_library "https://downloads.xiph.org/releases/ogg/libogg-1.3.5.tar.gz" "libogg" "tar.gz" > $FD_OUT 2>&1

echo "Dowlnoading libvorbis..."
install_library "https://downloads.xiph.org/releases/vorbis/libvorbis-1.3.7.tar.gz" "libvorbis" "tar.gz" "--with-ogg=${INSTALL_DIR}" > $FD_OUT 2>&1

echo "Dowlnoading flac..."
install_library "https://downloads.xiph.org/releases/flac/flac-1.3.3.tar.xz" "flac" "tar.xz" "--disable-cpplibs" > $FD_OUT 2>&1

echo "Dowlnoading freetype..."
install_library "https://download.savannah.gnu.org/releases/freetype/freetype-2.11.0.tar.gz" "freetype" "tar.gz" > $FD_OUT 2>&1




SFML_REPO="https://github.com/SFML/SFML.git"
SFML_DIR="${BASE_DIR}/SFML"
BUILD_DIR="${SFML_DIR}/build"

# Clone SFML repository if it doesn't exist
if [ ! -d "${SFML_DIR}" ]; then
    echo "Clone SFML repo..."
    git clone $SFML_REPO ${SFML_DIR} > $FD_OUT 2>&1
else
    echo "SFML repo already exists, update ...."
    cd ${SFML_DIR}
    git pull origin master > $FD_OUT 2>&1
    cd ${BASE_DIR}
fi

# Create build directory
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

# Configure CMake with local dependencies
echo "Configure CMake variable ..."
cmake ${SFML_DIR} -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
      -DCMAKE_PREFIX_PATH=${INSTALL_DIR} \
      -DFLAC_INCLUDE_DIR=${INSTALL_DIR}/include \
      -DFLAC_LIBRARY=${INSTALL_DIR}/lib/libFLAC.so \
      -DOGG_INCLUDE_DIR=${INSTALL_DIR}/include \
      -DOGG_LIBRARY=${INSTALL_DIR}/lib/libogg.so \
      -DVORBIS_INCLUDE_DIR=${INSTALL_DIR}/include \
      -DVORBIS_LIBRARY=${INSTALL_DIR}/lib/libvorbis.so \
      -DVORBISENC_LIBRARY=${INSTALL_DIR}/lib/libvorbisenc.so \
      -DVORBISFILE_LIBRARY=${INSTALL_DIR}/lib/libvorbisfile.so \
      -DFREETYPE_INCLUDE_DIRS=${INSTALL_DIR}/include/freetype2 \
      -DFREETYPE_LIBRARY=${INSTALL_DIR}/lib/libfreetype.so \

# Compile and install SFML
echo "Compile and install SFML in ${INSTALL_DIR}..."
make -j$(nproc) > $FD_OUT 2>&1
make install > $FD_OUT 2>&1

# cd ${BASE_DIR}
# Clear downloaded files
# rm -rf ${BASE_DIR}/*.tar.gz

echo "SFML instalation done in ${INSTALL_DIR}."


# Old code
# echo "Dowlnoading openal-soft..."
# install_library "https://github.com/kcat/openal-soft/archive/refs/tags/openal-soft-1.21.1.tar.gz" "openal-soft" "tar.gz" "-DALSOFT_EXAMPLES=OFF" > $FD_OUT 2>&1
# echo "Dowlnoading libsndfile..."
# install_library "https://github.com/libsndfile/libsndfile/releases/download/1.2.2/libsndfile-1.2.2.tar.xz" "libsndfile" "tar.xz" > $FD_OUT 2>&1
#   -DOPENAL_INCLUDE_DIR=$INSTALL_DIR/include \
#   -DOPENAL_LIBRARY=$INSTALL_DIR/lib/libopenal.so \
#   -DSNDFILE_INCLUDE_DIR=$INSTALL_DIR/include \
#   -DSNDFILE_LIBRARY=$INSTALL_DIR/lib/libsndfile.so ..
