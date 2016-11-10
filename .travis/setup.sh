#!/bin/bash
echo "os ${OS}, compiler ${COMPILER}, bits ${BITS}"
echo "deb http://pkg.mxe.cc/repos/apt/debian wheezy main" | sudo tee -a /etc/apt/sources.list
sudo  apt-key adv --keyserver keyserver.ubuntu.com --recv-keys D43A795B73B16ABE9643FE1AFD8FFF16DB45C6AB
sudo apt-get update
sudo apt-get install -y mxe-i686-w64-mingw32.shared-qtbase

if [[ $TRAVIS_BRANCH == 'to_vombat' ]]; then # on this branch we also need boost and eigen
	sudo apt-get install -y mxe-i686-w64-mingw32.shared-eigen mxe-i686-w64-mingw32.shared-boost
fi

