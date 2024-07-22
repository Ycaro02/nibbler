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
	local sfm_repo="${1}"
	local sfml_version="${2}"
	local sfml_dir="${BASE_DIR}/SFML"
	local build_dir="${sfml_dir}/build"

	# Clone SFML repository if it doesn't exist
	if [ ! -d "${sfml_dir}" ]; then
		load_deps_SFML
		display_color_msg ${CYAN} "Clone SFML repo..."
		git clone -b ${sfml_version} --depth 1 ${sfm_repo} ${sfml_dir} >> $FD_OUT 2>&1
		# Create build directory
		mkdir -p ${build_dir}
		cd ${build_dir}
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
	fi
}


function load_SDL2 {
	local sdl_archive="${1}"
	local sdl_dir_version="${2}"
	local sdl_dir="${BASE_DIR}/SDL2"
	
	cd ${BASE_DIR}

	if [ ! -d "${sdl_dir}" ]; then
		display_color_msg ${YELLOW} "Download and install SDL2..."
		# Download and install SDL2
		wget ${sdl_archive} >> $FD_OUT 2>&1
		tar -xvf ${sdl_dir_version}.tar.gz >> $FD_OUT 2>&1
		rm -rf ${sdl_dir_version}.tar.gz
		mv ${sdl_dir_version} ${local sdl_dir}
		cd ${local sdl_dir}
		mkdir build
		cd build
		cmake .. -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} -DCMAKE_PREFIX_PATH=${INSTALL_DIR} >> $FD_OUT 2>&1
		make -s -j$(nproc) >> $FD_OUT 2>&1
		make -s install >> $FD_OUT 2>&1
		display_color_msg ${GREEN} "SDL2 instalation done in ${INSTALL_DIR}."
	fi

}

function load_raylib {
    local raylib_repo="${1}"
    local raylib_version="${2}"
    local raylib_dir="${BASE_DIR}/raylib"
    local build_dir="${raylib_dir}/build"

    # Clone Raylib repository if it doesn't exist
    if [ ! -d "${raylib_dir}" ]; then
        display_color_msg ${CYAN} "Clone Raylib repo..."
        git clone -b ${raylib_version} --depth 1 ${raylib_repo} ${raylib_dir} >> $FD_OUT 2>&1
    fi

    # Create build directory
    mkdir -p ${build_dir}
    cd ${build_dir}

    # Configure CMake with local dependencies
    cmake .. -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
        -DCMAKE_PREFIX_PATH=${INSTALL_DIR} \
		-DBUILD_SHARED_LIBS=ON \
        >> $FD_OUT 2>&1

    # Compile and install Raylib
    display_color_msg ${YELLOW} "Compile and install Raylib in ${INSTALL_DIR}..."
    make -s -j$(nproc) >> $FD_OUT 2>&1
    make -s install >> $FD_OUT 2>&1

    display_color_msg ${GREEN} "Raylib installation done in ${INSTALL_DIR}."
}

load_SFML "https://github.com/SFML/SFML.git" "2.6.1"
load_SDL2 "https://github.com/libsdl-org/SDL/releases/download/release-2.30.5/SDL2-2.30.5.tar.gz" "SDL2-2.30.5"
load_raylib "https://github.com/raysan5/raylib.git" "4.5.0"

# Old code for SFML deps
# echo "Dowlnoading libsndfile..."
# install_library "https://github.com/libsndfile/libsndfile/releases/download/1.2.2/libsndfile-1.2.2.tar.xz" "libsndfile" "tar.xz" >> $FD_OUT 2>&1
#   -DSNDFILE_INCLUDE_DIR=$INSTALL_DIR/include \
#   -DSNDFILE_LIBRARY=$INSTALL_DIR/lib/libsndfile.so ..