#!/bin/bash

# Get the current directory
PWD=$(pwd)

# Tmp directories for lib and include files for deb packages
TMP_LIB_DIR=${PWD}/tmp_lib
TMP_INCLUDE_DIR=${PWD}/tmp_include

# Load missing deb packages function
source ${PWD}/rsc/install/install_missing_deb.sh

load_missing_deb_package

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

# Create directories
display_color_msg ${YELLOW} "Create directories ${DEPS_DIR} and ${INSTALL_DIR}."
mkdir -p ${DEPS_DIR}
mkdir -p ${INSTALL_DIR}/lib/pkgconfig ${INSTALL_DIR}/include

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

#debug
# ls -lR ${INSTALL_DIR}
# exit 1

# Set environment variables for dependencies
export PKG_CONFIG_PATH="${INSTALL_DIR}/lib/pkgconfig"
export CFLAGS="-I${INSTALL_DIR}/include"
export CXXFLAGS="-I${INSTALL_DIR}/include -L${INSTALL_DIR}/lib"
export LDFLAGS="-L${INSTALL_DIR}/lib"

export LD_LIBRARY_PATH="${INSTALL_DIR}/lib:${LD_LIBRARY_PATH}"

# # Use pkg-config to get the correct flags
# CXXFLAGS+=" $(pkg-config --cflags gl)"
# LDFLAGS+=" $(pkg-config --libs gl)"

# Print the environment variables for debugging
echo "PKG_CONFIG_PATH=${PKG_CONFIG_PATH}"
echo "CFLAGS=${CFLAGS}"
echo "CXXFLAGS=${CXXFLAGS}"
echo "LDFLAGS=${LDFLAGS}"
echo "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}"

# Cut script execution if a command fails
set -e 

function all_deps_install {

	# load libX11
	load_lib "https://www.x.org/archive/individual/lib/libX11-1.7.2.tar.gz"
	# load libXext
	load_lib "https://www.x.org/archive/individual/lib/libXext-1.3.4.tar.gz"
	# Load libXrandr
	load_lib "https://www.x.org/archive/individual/lib/libXrandr-1.5.3.tar.gz"

	# Load libXinerama
	load_lib "https://www.x.org/archive/individual/lib/libXinerama-1.1.4.tar.gz"
	
	echo "Before GLU"
	ls -lR ${INSTALL_DIR}/lib
	echo "\n\n\nldd deps\n"
	ldd ${INSTALL_DIR}/lib/libGL.so
	echo "\n\n"
	# Load libGLU
	load_lib "ftp://ftp.freedesktop.org/pub/mesa/glu/glu-9.0.1.tar.gz"
	# Load freeglut
	load_lib_cmake "https://sourceforge.net/projects/freeglut/files/freeglut/3.4.0/freeglut-3.4.0.tar.gz" "freeglut-3.4.0"

	
	# Deps for Xcursor and Xi: libXfixes
	load_lib "https://www.x.org/archive/individual/lib/libXfixes-5.0.3.tar.gz"

	# Load libXcursor and libXi
	load_lib "https://www.x.org/archive/individual/lib/libXcursor-1.2.0.tar.gz"
	load_lib "https://www.x.org/archive/individual/lib/libXi-1.7.10.tar.gz"

	# Load FreeType, needed in SFML, SDL2_ttf and Raylib
	load_lib "https://sourceforge.net/projects/freetype/files/freetype2/2.11.0/freetype-2.11.0.tar.gz/download"

	# 2 package needed for github actions, that will install :
	# libdrm-dev libgl-dev libglx-dev libpciaccess-dev libudev-dev mesa-common-dev

	# Libudev already on 42 computer but need to be install on github actions 
	#libudev: https://www.freedesktop.org/software/systemd/systemd-248.tar.gz
	# Try to work on mesa loading (openGl open source implementation) compiling with meson
	# load_lib "https://mesa.freedesktop.org/archive/mesa-21.2.3.tar.xz"
}

function load_deps_SFML {
	# Download and install dependencies
	display_double_color_msg ${BLUE} "Download and install dependencies for " ${RED} "SFML"
	load_lib "https://downloads.xiph.org/releases/ogg/libogg-1.3.5.tar.gz"
	load_lib "https://downloads.xiph.org/releases/vorbis/libvorbis-1.3.7.tar.gz" "--with-ogg=${INSTALL_DIR}"
	load_lib "https://downloads.xiph.org/releases/flac/flac-1.3.3.tar.xz" "--disable-cpplibs"
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
		display_color_msg ${YELLOW} "Download and install SDL2..."
		# Download and install SDL2
		wget ${sdl_archive} >> $FD_OUT 2>&1
		tar -xvf ${sdl_dir_version}.tar.gz >> $FD_OUT 2>&1
		rm -rf ${sdl_dir_version}.tar.gz
		mv ${sdl_dir_version} ${sdl_dir}
		cd ${sdl_dir}
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

all_deps_install 
load_SFML "https://github.com/SFML/SFML.git" "2.6.1"
load_SDL2 "https://github.com/libsdl-org/SDL/releases/download/release-2.30.5/SDL2-2.30.5.tar.gz" "SDL2-2.30.5"
load_SDL2_TTF "https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.22.0/SDL2_ttf-2.22.0.tar.gz" "2.22.0"
load_raylib "https://github.com/raysan5/raylib.git" "4.5.0"




# Old code for SFML deps
# echo "Dowlnoading libsndfile..."
# install_library "https://github.com/libsndfile/libsndfile/releases/download/1.2.2/libsndfile-1.2.2.tar.xz" "libsndfile" "tar.xz" >> $FD_OUT 2>&1
#   -DSNDFILE_INCLUDE_DIR=$INSTALL_DIR/include \
#   -DSNDFILE_LIBRARY=$INSTALL_DIR/lib/libsndfile.so ..