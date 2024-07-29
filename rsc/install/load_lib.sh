#!/bin/bash

function setup_deb_packages {

	# Tmp directories for lib and include files for deb packages
	local pwd_save=${PWD}
	TMP_DIR=${PWD}/tmp
	TMP_LIB_DIR=${PWD}/tmp_lib
	TMP_INCLUDE_DIR=${PWD}/tmp_include

	# Load missing deb packages function
	# Need to declare PWD and TMP_LIB_DIR/TMP_INCLUDE_DIR before loading the script
	source ${PWD}/rsc/install/install_missing_deb.sh

	# Load and install deb packages
	load_missing_deb_package

	# Go back to the original directory
	cd ${pwd_save}

	# Create directories
	display_color_msg ${YELLOW} "Create directories ${DEPS_DIR} and ${INSTALL_DIR}."
	mkdir -p ${DEPS_DIR} ${INSTALL_DIR}/lib/pkgconfig ${INSTALL_DIR}/include

	if [ ! -d ${TMP_LIB_DIR} ]; then
		display_color_msg ${RED} "TMP_LIB_DIR not found."
		exit 1
	fi

	if [ ! -d ${TMP_INCLUDE_DIR} ]; then
		display_color_msg ${RED} "TMP_INCLUDE_DIR not found."
		exit 1
	fi

	# Copy lib and include files to install directory
	display_color_msg ${MAGENTA} "Copy TMP lib and include files to install directory."
	cp -r ${TMP_LIB_DIR}/* ${INSTALL_DIR}/lib
	cp -r ${TMP_INCLUDE_DIR}/* ${INSTALL_DIR}/include

	rm -rf ${TMP_LIB_DIR} ${TMP_INCLUDE_DIR} ${TMP_DIR}
}

# Download and install dependencies
function load_deps_SFML {
	# Load SFML sound dependencies
	display_double_color_msg ${BLUE} "Download and install dependencies for " ${RED} "SFML"
	load_lib "https://downloads.xiph.org/releases/ogg/libogg-1.3.5.tar.gz"
	load_lib "https://downloads.xiph.org/releases/vorbis/libvorbis-1.3.7.tar.gz" "--with-ogg=${INSTALL_DIR}"
	load_lib "https://downloads.xiph.org/releases/flac/flac-1.3.3.tar.xz" "--disable-cpplibs"
	load_lib_cmake "https://github.com/kcat/openal-soft/archive/refs/tags/1.23.1.tar.gz" "openal-soft"
	
	# Load FreeType, needed in SFML, SDL2_ttf and Raylib (for text rendering)
	load_lib "https://sourceforge.net/projects/freetype/files/freetype2/2.11.0/freetype-2.11.0.tar.gz/download"
}

function load_SFML {
	local sfm_repo="${1}"
	local sfml_version="${2}"
	local sfml_dir="${BASE_DIR}/SFML"
	local build_dir="${sfml_dir}/build"

	# Clone SFML repository if it doesn't exist
	if [ ! -d "${sfml_dir}" ]; then
		load_deps_SFML
		display_color_msg ${CYAN} "Get SFML repo..."
		git clone -b ${sfml_version} --depth 1 ${sfm_repo} ${sfml_dir} >> $FD_OUT 2>&1
		# Create build directory
		mkdir -p ${build_dir}
		cd ${build_dir}
		# Configure CMake with local dependencies
		cmake .. -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
			-DCMAKE_PREFIX_PATH=${INSTALL_DIR} \
			-DOPENAL_INCLUDE_DIR=${DEPS_DIR}/openal-soft-1.23.1/include/AL \
			-DOPENAL_LIBRARY=${DEPS_DIR}/openal-soft-1.23.1/build/libopenal.so \
			-DBUILD_SHARED_LIBS=ON \
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

# Function to download and install SDL2_ttf
function load_SDL2_TTF {
	local url="${1}"
    local ttf_version="${2}"

    display_color_msg ${YELLOW} "Downloading SDL2_ttf version: ${ttf_version}"

	cd ${BASE_DIR}

    # Download SDL2_ttf
    wget ${url} >> $FD_OUT 2>&1

    tar -xzf SDL2_ttf-${ttf_version}.tar.gz >> $FD_OUT 2>&1
    cd SDL2_ttf-${ttf_version}

	mkdir build
	cd build
	cmake .. -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
		-DCMAKE_PREFIX_PATH=${INSTALL_DIR} \
		-DFREETYPE_INCLUDE_DIRS=${INSTALL_DIR}/include/freetype2 \
		-DFREETYPE_LIBRARY=${INSTALL_DIR}/lib/libfreetype.so \
		-DSDL2_INCLUDE_DIR=${INSTALL_DIR}/include/SDL2 \
		>> $FD_OUT 2>&1

	make -s -j$(nproc) >> $FD_OUT 2>&1
	make -s install >> $FD_OUT 2>&1

    # Clean up
    cd ../..
    rm -rf SDL2_ttf-${ttf_version}.tar.gz

	display_color_msg ${GREEN} "SDL2_ttf version ${ttf_version} installed successfully."
}

function load_SDL2 {
	local sdl_archive="${1}"
	local sdl_dir_version="${2}"
	local sdl_dir="${BASE_DIR}/SDL2"
	
	cd ${BASE_DIR}

	if [ ! -d "${sdl_dir}" ]; then
        display_color_msg ${CYAN} "Get SDL2 repo..."

		# Download and install SDL2
		wget ${sdl_archive} >> $FD_OUT 2>&1
		tar -xvf ${sdl_dir_version}.tar.gz >> $FD_OUT 2>&1
		rm -rf ${sdl_dir_version}.tar.gz
		mv ${sdl_dir_version} ${sdl_dir}
		cd ${sdl_dir}
		display_color_msg ${YELLOW} "Compile and install SDL2 in ${INSTALL_DIR}..."
		mkdir build
		cd build
		cmake .. -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
			-DCMAKE_PREFIX_PATH=${INSTALL_DIR} \
            -DCMAKE_INCLUDE_PATH=${INSTALL_DIR}/include \
            -DCMAKE_LIBRARY_PATH=${INSTALL_DIR}/lib \
            -DBUILD_SHARED_LIBS=ON \
			>> $FD_OUT 2>&1

		make -s -j$(nproc) >> $FD_OUT 2>&1
		make -s install >> $FD_OUT 2>&1
		display_color_msg ${GREEN} "SDL2 installation done in ${INSTALL_DIR}."
	fi

}

function load_raylib {
    local raylib_repo="${1}"
    local raylib_version="${2}"
    local raylib_dir="${BASE_DIR}/raylib"
    local build_dir="${raylib_dir}/build"

    # Clone Raylib repository if it doesn't exist
    if [ ! -d "${raylib_dir}" ]; then
        display_color_msg ${CYAN} "Get Raylib repo..."
        git clone -b ${raylib_version} --depth 1 ${raylib_repo} ${raylib_dir} >> $FD_OUT 2>&1
		# Create build directory
		mkdir -p ${build_dir}
		cd ${build_dir}

		# Configure CMake with local dependencies
		cmake .. -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
			-DCMAKE_PREFIX_PATH=${INSTALL_DIR} \
			-DBUILD_SHARED_LIBS=ON \
			-DBUILD_GLFW=OFF \
			>> $FD_OUT 2>&1

		# Compile and install Raylib
		display_color_msg ${YELLOW} "Compile and install Raylib in ${INSTALL_DIR}..."
		make -s -j$(nproc) >> $FD_OUT 2>&1
		make -s install >> $FD_OUT 2>&1

		display_color_msg ${GREEN} "Raylib installation done in ${INSTALL_DIR}."
    fi

}


# Get the current directory
PWD=$(pwd)

# Variables
BASE_DIR="$PWD/rsc/lib"
DEPS_DIR="$BASE_DIR/deps"
INSTALL_DIR="$BASE_DIR/install"

# Load the color script and utils functions
# Need to declare PWD and DEPS/INSTALL_DIR before loading the script
source ${PWD}/rsc/install/install_utils.sh

FD_OUT="/dev/stdout"

# Update FD_OUT if -q option is passed
handle_quiet_opt "${@}"

# Setup deb packages (openGL lib and libudev)
setup_deb_packages

# Set environment variables for dependencies
export PKG_CONFIG_PATH="${INSTALL_DIR}/lib/pkgconfig"
export CFLAGS="-I${INSTALL_DIR}/include"
export CXXFLAGS="-I${INSTALL_DIR}/include -L${INSTALL_DIR}/lib"
export LDFLAGS="-L${INSTALL_DIR}/lib"

# Cut script execution if any command fails
set -e 

# load_dependencies
load_SFML "https://github.com/SFML/SFML.git" "2.6.1"
load_SDL2 "https://github.com/libsdl-org/SDL/releases/download/release-2.30.5/SDL2-2.30.5.tar.gz" "SDL2-2.30.5"
load_SDL2_TTF "https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.22.0/SDL2_ttf-2.22.0.tar.gz" "2.22.0"
load_raylib "https://github.com/raysan5/raylib.git" "4.5.0"
