#!/bin/bash


# Compile the wrapper lib and run the test

PWD=$(pwd)

source ${PWD}/rsc/sh/color.sh
# Set up environment not mandatory if we use -rpath in the compilation
# export LD_LIBRARY_PATH=$(pwd)/rsc/lib/SFML/build/lib:$LD_LIBRARY_PATH

# display_color_msg ${YELLOW} "Compiling the wrapper lib in rsc/wrapperlib"
mkdir -p rsc/wrapperlib

# Check for errors
function check_compile_success {
	local name="${1}"
	if [ ! -f ${name} ]; then
		display_color_msg ${RED} "Compilation ${name} failed"
		exit 1
	else
		display_color_msg ${GREEN} "Compilate ${name} successful"
	fi
}

# Compile the wrapper lib
function wrapper_lib_compile {
	local lib_name="${1}"
	local wrapper_path="${2}"
	local lib_deps="${3}"

	clang++ -O3 -shared -o rsc/wrapperlib/${lib_name} ${wrapper_path} -fPIC \
		-I./rsc/lib/install/include -L./rsc/lib/install/lib ${lib_deps} \
		-Wl,-rpath,./rsc/lib/install/lib 

	check_compile_success "rsc/wrapperlib/${lib_name}"
}


function wrapper_compile_entry {
	# Check if the wrapper lib source file exists
	if [ ! -f "${2}" ]; then
		display_color_msg ${RED} "The wrapper lib source file ${2} does not exist"
		exit 1
	fi

	local wrapper_lib_name="${1}"
	local wrapper_lib_src="${2}"
	local wrapper_lib_deps="${3}"

	wrapper_lib_compile "${wrapper_lib_name}" "${wrapper_lib_src}" "${wrapper_lib_deps}"
}

wrapper_compile_entry "${@}"

