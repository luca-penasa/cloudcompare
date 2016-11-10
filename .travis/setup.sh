#!/bin/bash
set -e
echo "os ${OS}, compiler ${COMPILER}, bits ${BITS}"

if [ $OS = "MXEWIN" ]; then


	if [ $BITS == 64 ]; then
		mxearchname="x86-64-w64-mingw32.shared"
		cmakename=x86_64-w64-mingw32.shared-cmake
		echo "compiling for win on mxe 64 bits"		
	fi

	if [ $BITS == 32 ]; then
		mxearchname="i686-w64-mingw32.shared"
		cmakename=$mxearchname-cmake
		echo "compiling for win on mxe 34 bits"		
	fi


	echo "deb http://pkg.mxe.cc/repos/apt/debian wheezy main" | sudo tee -a /etc/apt/sources.list
	sudo  apt-key adv --keyserver keyserver.ubuntu.com --recv-keys D43A795B73B16ABE9643FE1AFD8FFF16DB45C6AB
	sudo apt-get update
	sudo apt-get install -y $mxearchname-qtbase

	if [[ $TRAVIS_BRANCH == 'to_vombat' ]]; then # on this branch we also need boost and eigen
		sudo apt-get install -y $mxearchname-eigen mxe-i686-w64-mingw32.shared-boost
	fi
fi


if [ $OS = "LINUX" ]; then
	# Install dependencies
	sudo add-apt-repository --yes ppa:george-edison55/cmake-3.x
    sudo add-apt-repository --yes ppa:beineri/opt-qt551-trusty
    sudo apt-get update -qq
    sudo apt-get install -qy cmake qt55base libgdal1-dev libfreenect-dev libvxl1-dev
    # CC submodules
	git submodule init && git submodule update

	if [ $COMPILER = CLANG ]; then
		export CXX="clang++"
		export CC="clang"

	elif [ $COMPILER = GCC ]; then
		export CXX="g++"
		export CC="gcc"
	fi


fi

