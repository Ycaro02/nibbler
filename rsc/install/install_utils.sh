#!/bin/bash

source ${PWD}/rsc/sh/color.sh

# check for quiet option
function handle_quiet_opt {
	mkdir -p ${PWD}/rsc/log

	if [ "$1" == "-q" ]; then
		FD_OUT="${PWD}/rsc/log/init_sfml.log"
		rm -f ${FD_OUT}
		display_color_msg ${CYAN} "Quiet mode enabled log in ${FD_OUT}."
	else
		display_color_msg  ${CYAN} "Verbose mode enabled."
	fi
}

# Load the archive file from the url and extract it to the directory, handle tar.gz and tar.xz
function url_archive_to_directory {
	local url=$1
	local name=$2
	local extension=$3

	wget $url -O ${DEPS_DIR}/$name.$extension
	if [[ $extension == "tar.gz" ]]; then
		tar -xzf ${DEPS_DIR}/$name.$extension -C ${DEPS_DIR}
	elif [[ $extension == "tar.xz" ]]; then
		tar -xf ${DEPS_DIR}/$name.$extension -C ${DEPS_DIR}
	fi

}

# Install a library from url, name, extension and configure flags, using configure make and make install (default configure flags is empty)
function install_library {
    local url=${1}
    local name=${2}
    local extension=${3}
    local configure_flags=${4}


	url_archive_to_directory ${url} ${name} ${extension}
	cd ${DEPS_DIR}/$name-*
	
	# run configure script
    ./configure --prefix=${INSTALL_DIR} $configure_flags
	# compile and install
    make -s -j$(nproc)
    make -s install

    cd ${DEPS_DIR}
}

# Install a library from url, name, extension, using cmake and make
function cmake_install_lib {
	local url=${1}
	local name=${2}
	local extension=${3}

	url_archive_to_directory ${url} ${name} ${extension}
	cd ${DEPS_DIR}/${name}-*/build
	cmake .. 
	make -s -j$(nproc)

	cd ${DEPS_DIR}
}

# Extract extension from url, last 2 field cut by dot (tar.gz, tar.xz)
function extract_extension {
	local url=${1}
	local extension=$(echo ${url} | rev | cut -d'.' -f 1-2 | rev)
	echo ${extension}
}

# Extract name from url, last field cut by slash and dash (name-version)
function extract_name {
	local url=${1}
	local name=$(echo ${url} | rev | cut -d'/' -f 1 | rev | cut -d'-' -f 1)
	echo ${name}
}

# Load a library from a url and configure flags
function load_lib {
	local url=${1}
	local configure_flags=${2}


	# Need to check if the last word is download, if it is remove it (sourceforge url)
	local tmp_url=${url}
	local last_word=$(echo ${tmp_url} | rev | cut -d'/' -f 1 | rev)
	if [ ${last_word} == "download" ]; then
		tmp_url=$(echo ${tmp_url} | rev | cut -d'/' -f 2- | rev)
	fi

	# echo "Echo tmp url ${tmp_url}"

	local extension=$(extract_extension ${tmp_url})
	local name=$(extract_name ${tmp_url})

	# echo "Echo name ${name}"
	# echo "Echo extension ${extension}"


	display_color_msg ${MAGENTA} "Dowlnoading ${name}..."
	install_library ${url} ${name} ${extension} ${configure_flags} >> $FD_OUT 2>&1
	display_color_msg ${GREEN} "Done ${name}"
}

function load_lib_cmake {
	local url=${1}
	local name=${2}

	local extension=$(extract_extension ${url})

	display_color_msg ${MAGENTA} "Dowlnoading ${name}..."
	cmake_install_lib ${url} ${name} ${extension} >> $FD_OUT 2>&1
	display_color_msg ${GREEN} "Done ${name}"
}