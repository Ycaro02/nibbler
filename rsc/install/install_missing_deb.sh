#!/bin/bash

# Install missing deb packages
# mesa-common-dev libgl-dev libdrm-dev libglx-dev libpciaccess-dev
#   libudev-dev 

#mesa common dev
# wget http://security.ubuntu.com/ubuntu/pool/main/m/mesa/mesa-common-dev_24.0.5-1ubuntu1_amd64.deb -O mesa-common-dev_24.0.5-1ubuntu1_amd64.deb
# wget http://security.ubuntu.com/ubuntu/pool/main/libg/libglvnd/libgl-dev_1.6.0-1_amd64.deb -O libgl-dev_1.6.0-1_amd64.deb
# wget http://security.ubuntu.com/ubuntu/pool/main/libd/libdrm/libdrm-dev_2.4.101-2_amd64.deb -O libdrm-dev_2.4.101-2_amd64.deb
# wget http://security.ubuntu.com/ubuntu/pool/main/libg/libglvnd/libglx-dev_1.7.0-1build1_amd64.deb -O libglx-dev_1.7.0-1build1_amd64.deb
# wget https://www.ubuntuupdates.org/package/core/noble/main/base/libpciaccess-dev -O libpciaccess-dev_0.16-0ubuntu1_amd64.deb
# wget http://security.ubuntu.com/ubuntu/pool/main/s/systemd/libudev-dev_237-3ubuntu10.44_amd64.deb -O libudev-dev_237-3ubuntu10.44_amd64.deb



#create tmp directory
TMP_DIR=${PWD}/tmp/deb
mkdir -p ${TMP_DIR} && cd ${TMP_DIR}

function load_deb_package {
	local deb_url="${1}"
	local deb_file="${2}"
	local package_name="${3}"
	
	wget ${deb_url} -O ${deb_file}

	dpkg-deb -x ${deb_file} ${package_name}

	if [ $? -ne 0 ]; then
		echo "Failed to install ${package_name}"
		exit 1
	fi
	rm ${deb_file}
}


function install_deb_package {
	local package_name="${1}"
	cp -r ${TMP_DIR}/${package_name}/usr/lib/x86_64-linux-gnu/* ${TMP_LIB_DIR}
	cp -r ${TMP_DIR}/${package_name}/usr/include/* ${TMP_INCLUDE_DIR}
}

function load_missing_deb_package {
	echo "Installing missing deb packages $(pwd)"

	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/m/mesa/mesa-common-dev_24.0.5-1ubuntu1_amd64.deb mesa-common-dev_24.0.5-1ubuntu1_amd64.deb mesa-common-dev
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/libg/libglvnd/libgl-dev_1.6.0-1_amd64.deb libgl-dev_1.6.0-1_amd64.deb libgl-dev
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/libd/libdrm/libdrm-dev_2.4.101-2_amd64.deb libdrm-dev_2.4.101-2_amd64.deb libdrm-dev
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/libg/libglvnd/libglx-dev_1.7.0-1build1_amd64.deb libglx-dev_1.7.0-1build1_amd64.deb libglx-dev
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/libp/libpciaccess/libpciaccess-dev_0.17-3build1_amd64.deb libpciaccess-dev_0.16-0ubuntu1_amd64.deb libpciaccess-dev
	load_deb_package http://security.ubuntu.com/ubuntu/pool/main/s/systemd/libudev-dev_255.4-1ubuntu8.2_amd64.deb libudev-dev_237-3ubuntu10.44_amd64.deb libudev-dev

	cd ../..

	install_deb_package mesa-common-dev
	install_deb_package libgl-dev
	install_deb_package libdrm-dev
	install_deb_package libglx-dev
	install_deb_package libpciaccess-dev
	install_deb_package libudev-dev

	rm -rf tmp
}

