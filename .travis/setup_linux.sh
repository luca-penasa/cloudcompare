#!/bin/bash
set -e
echo "os ${TRAVIS_OS_NAME}, compiler ${COMPILER}"


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


############## BRANCH SPECIFICS


if [ $TRAVIS_BRANCH = 'vombat' ]; then # we need eigen, and boost
    sudo apt-get install -y libeigen3-dev libboost-regex-dev libboost-filesystem-dev libboost-system-dev
fi




