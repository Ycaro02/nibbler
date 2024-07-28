#!/bin/bash

# Install missing deb packages
# mesa-common-dev libgl-dev libdrm-dev libglx-dev libpciaccess-dev libudev-dev 

#create tmp directory
# PWD=$(pwd)

TMP_DIR=${PWD}/tmp
# TMP_LIB_DIR=${PWD}/tmp_lib
# TMP_INCLUDE_DIR=${PWD}/tmp_include


# Include and lib directory options
ONLY_INC="0"
ONLY_LIB="1"
INC_AND_LIB="2"

echo "Create tmp directory ${TMP_DIR} ${TMP_LIB_DIR} ${TMP_INCLUDE_DIR}"

mkdir -p ${TMP_DIR} ${TMP_LIB_DIR} ${TMP_INCLUDE_DIR} && cd ${TMP_DIR}


function install_deb_package {
	local package_name="${1}"
	local include_lib_dir="${2}"

	if [ ${include_lib_dir} == ${ONLY_INC} ]; then 
		cp -r ${TMP_DIR}/${package_name}/usr/include/* ${TMP_INCLUDE_DIR}
	fi

	if [ ${include_lib_dir} == ${ONLY_LIB} ]; then
		cp -r ${TMP_DIR}/${package_name}/usr/lib/x86_64-linux-gnu/* ${TMP_LIB_DIR}
	fi

	if [ ${include_lib_dir} == ${INC_AND_LIB} ]; then
		cp -r ${TMP_DIR}/${package_name}/usr/include/* ${TMP_INCLUDE_DIR}
		cp -r ${TMP_DIR}/${package_name}/usr/lib/x86_64-linux-gnu/* ${TMP_LIB_DIR}
	fi
}

function load_deb_package {
	local deb_url="${1}"
	local deb_file="${2}"
	local package_name="${3}"
	local include_lib_dir="${4}"
	
	wget ${deb_url} -O ${deb_file}

	dpkg-deb -x ${deb_file} ${package_name}

	if [ $? -ne 0 ]; then
		echo "Failed to install ${package_name}"
		exit 1
	fi
	rm ${deb_file}
	install_deb_package ${package_name} ${include_lib_dir}
}



function load_missing_deb_package {
	echo "Installing missing deb packages $(pwd)"

	# Mesa common dev and his dependencies (libgl)
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/m/mesa/mesa-common-dev_24.0.5-1ubuntu1_amd64.deb mesa-common-dev_24.0.5-1ubuntu1_amd64.deb mesa-common-dev ${INC_AND_LIB}
	
	# Lib gl and his dependencies
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/libg/libglvnd/libgl-dev_1.6.0-1_amd64.deb libgl-dev_1.6.0-1_amd64.deb libgl-dev ${INC_AND_LIB}
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/libg/libglvnd/libgl1_1.7.0-1build1_amd64.deb libgl1_1.7.0-1build1_amd64.deb libgl1 ${ONLY_LIB}

	# Lib glx and his dependencies
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/libg/libglvnd/libglx-dev_1.7.0-1build1_amd64.deb libglx-dev_1.7.0-1build1_amd64.deb libglx-dev ${INC_AND_LIB}
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/libg/libglvnd/libglx0_1.7.0-1build1_amd64.deb libglx0_1.7.0-1build1_amd64.deb libglx0 ${ONLY_LIB}


	# Lib drm and his dependencies
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/libd/libdrm/libdrm-dev_2.4.101-2_amd64.deb libdrm-dev_2.4.101-2_amd64.deb libdrm-dev ${INC_AND_LIB}
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/libd/libdrm/libdrm2_2.4.120-2build1_amd64.deb libdrm2_2.4.120-2build1_amd64.deb libdrm2 ${ONLY_LIB}
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/libd/libdrm/libdrm-radeon1_2.4.120-2build1_amd64.deb libdrm-radeon1_2.4.120-2build1_amd64.deb libdrm-radeon1 ${ONLY_LIB}
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/libd/libdrm/libdrm-nouveau2_2.4.120-2build1_amd64.deb libdrm-nouveau2_2.4.120-2build1_amd64.deb libdrm-nouveau2 ${ONLY_LIB}
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/libd/libdrm/libdrm-intel1_2.4.120-2build1_amd64.deb libdrm-intel1_2.4.120-2build1_amd64.deb libdrm-intel1 ${ONLY_LIB}
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/libd/libdrm/libdrm-amdgpu1_2.4.120-2build1_amd64.deb libdrm-amdgpu1_2.4.120-2build1_amd64.deb libdrm-amdgpu1 ${ONLY_LIB}

	# Lib pciaccess and his dependencies
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/libp/libpciaccess/libpciaccess-dev_0.17-3build1_amd64.deb libpciaccess-dev_0.16-0ubuntu1_amd64.deb libpciaccess-dev ${INC_AND_LIB}
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/libp/libpciaccess/libpciaccess0_0.17-3build1_amd64.deb libpciaccess0_0.16-0ubuntu1_amd64.deb libpciaccess0 ${ONLY_LIB}
	
	# Lib udev and his dependencies
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/s/systemd/libudev-dev_255.4-1ubuntu8.2_amd64.deb libudev-dev_237-3ubuntu10.44_amd64.deb libudev-dev ${INC_AND_LIB}
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/s/systemd/libudev1_255.4-1ubuntu8.2_amd64.deb libudev1_237-3ubuntu10.44_amd64.deb libudev1 ${ONLY_LIB}


	cd ../..
	rm -rf ${TMP_DIR}
}

load_missing_deb_package