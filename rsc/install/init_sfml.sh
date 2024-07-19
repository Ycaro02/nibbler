#!/bin/bash

# Variables
# Get the current directory
PWD=$(pwd)
BASE_DIR="$PWD/rsc/lib"
DEPS_DIR="$BASE_DIR/deps"
INSTALL_DIR="$BASE_DIR/install"

# Load the color script and utils functions
# Need to declare PWD and DEPS/INSTALL_DIR before loading the script
source ${PWD}/rsc/install/install_utils.sh

FD_OUT="/dev/stdout"

# Update FD_OUT if -q option is passed
handle_quiet_opt "${@}"

function load_deps_SFML {
	# Create directories
	display_color_msg ${YELLOW} "Create directories ${DEPS_DIR} and ${INSTALL_DIR}."
	mkdir -p ${DEPS_DIR}
	mkdir -p ${INSTALL_DIR}

	# set -e  # Exit immediately if a command exits with a non-zero status.

	# Need libudev and libxrandr but they are already installed in the system (TOCHECK on 42 compute)
	# libudev should be in systemd package, libxrandr (should be aldready installed for running minilibx)

	# Download and install dependencies
	display_double_color_msg ${BLUE} "Download and install dependencies for " ${RED} "SFML"
	load_lib "https://downloads.xiph.org/releases/ogg/libogg-1.3.5.tar.gz"
	load_lib "https://downloads.xiph.org/releases/vorbis/libvorbis-1.3.7.tar.gz" "--with-ogg=${INSTALL_DIR}"
	load_lib "https://downloads.xiph.org/releases/flac/flac-1.3.3.tar.xz" "--disable-cpplibs"
	load_lib "https://sourceforge.net/projects/freetype/files/freetype2/2.11.0/freetype-2.11.0.tar.gz/download"
	load_lib_cmake "https://github.com/kcat/openal-soft/archive/refs/tags/1.23.1.tar.gz" "openal-soft"
}

function load_SFML {
	SFML_REPO="${1}"
	SFML_VERSION="${2}"
	SFML_DIR="${BASE_DIR}/SFML"
	BUILD_DIR="${SFML_DIR}/build"

	# Clone SFML repository if it doesn't exist
	if [ ! -d "${SFML_DIR}" ]; then
		display_color_msg ${CYAN} "Clone SFML repo..."
		git clone -b $SFML_VERSION --depth 1 $SFML_REPO ${SFML_DIR} >> $FD_OUT 2>&1
	fi

	# Create build directory
	mkdir -p ${BUILD_DIR}
	cd ${BUILD_DIR}

	# Configure CMake with local dependencies
	cmake .. -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
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
		-DBUILD_SHARED_LIBS=ON \
		-DOPENAL_INCLUDE_DIR=${DEPS_DIR}/openal-soft-1.23.1/include/AL \
		-DOPENAL_LIBRARY=${DEPS_DIR}/openal-soft-1.23.1/build/libopenal.so \
		>> $FD_OUT 2>&1


	# Compile and install SFML
	display_color_msg ${YELLOW} "Compile and install SFML in ${INSTALL_DIR}..."
	make -s -j$(nproc) >> $FD_OUT 2>&1
	make -s install >> $FD_OUT 2>&1

	# cd ${BASE_DIR}
	# Clear downloaded files
	rm -rf ${DEPS_DIR}/*.tar.*

	display_color_msg ${GREEN} "SFML instalation done in ${INSTALL_DIR}."
}


function load_SDL2 {
	cd ${BASE_DIR}

	display_color_msg ${YELLOW} "Download and install SDL2..."
	# Download and install SDL2
	wget https://github.com/libsdl-org/SDL/releases/download/release-2.30.5/SDL2-2.30.5.tar.gz
	tar -xvf SDL2-2.30.5.tar.gz
	rm -rf SDL2-2.30.5.tar.gz
	cd SDL2-2.30.5
	mkdir build
	cd build
	cmake .. -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} -DCMAKE_PREFIX_PATH=${INSTALL_DIR}
	make -s -j$(nproc)
	make -s install
	display_color_msg ${GREEN} "SDL2 instalation done in ${INSTALL_DIR}."
}

load_deps_SFML
load_SFML "https://github.com/SFML/SFML.git" "2.6.1"
load_SDL2 >> $FD_OUT 2>&1
# Old code
# echo "Dowlnoading libsndfile..."
# install_library "https://github.com/libsndfile/libsndfile/releases/download/1.2.2/libsndfile-1.2.2.tar.xz" "libsndfile" "tar.xz" >> $FD_OUT 2>&1
#   -DSNDFILE_INCLUDE_DIR=$INSTALL_DIR/include \
#   -DSNDFILE_LIBRARY=$INSTALL_DIR/lib/libsndfile.so ..