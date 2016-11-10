#!/bin/bash
set -e
echo "compiler ${CXX}, and ${CC}"


if [ $OS = "MXEWIN" ]; then
	echo "compiling for win on mxe"
	echo "travis build dir is ${TRAVIS_BUILD_DIR}"
	cd ${TRAVIS_BUILD_DIR}
	mkdir build
	cd build

	PATH=${PATH}:/usr/lib/mxe/usr/bin:/usr/lib/mxe/tools

	if [ $TRAVIS_BRANCH = 'to_vombat' ]; then 
		echo "on branch ${TRAVIS_BRANCH}"
		$cmakename -DCMAKE_BUILD_TYPE=Release .. -DCMAKE_INSTALL_PREFIX=${TRAVIS_BUILD_DIR} -DOPTION_BUILD_CCVIEWER=OFF -DINSTALL_vombat_PLUGIN=ON
	else # default compile options for all branches
		$cmakename -DCMAKE_BUILD_TYPE=Release .. -DCMAKE_INSTALL_PREFIX=${TRAVIS_BUILD_DIR} -DOPTION_BUILD_CCVIEWER=OFF	
	fi



	make install -j 3
	cd ${TRAVIS_BUILD_DIR}
	#- wget https://raw.githubusercontent.com/mxe/mxe/master/tools/copydlldeps.sh; chmod a+x copydlldeps.sh
	copydlldeps.sh -c -F ./CloudCompare -d ./CloudCompare -R /usr/lib/mxe
	name=cloudcompare-${OS}-${BITS}-${TRAVIS_BRANCH}-${TRAVIS_COMMIT}
	mv CloudCompare ${name}
	tar -zcvf ${name}.tar.gz ${name}
	curl -X PUT -u ${OWNCLOUD_USER}:${OWNCLOUD_PASSWORD} "https://ns392452.ip-176-31-109.eu/owncloud/remote.php/webdav/cloudcompare-builds/${name}.tar.gz" --data-binary @"${name}.tar.gz" --insecure
fi


if [ $OS = "LINUX" ]; then
	echo "compiling for linux"
	mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release \
	  -DQT5_ROOT_PATH=/opt/qt55 \
      -DOPTION_USE_DXF_LIB=ON \
      -DOPTION_USE_SHAPE_LIB=ON \
      -DINSTALL_QANIMATION_PLUGIN=ON \
      -DINSTALL_QBLUR_PLUGIN=ON \
      -DINSTALL_QDUMMY_PLUGIN=ON \
      -DINSTALL_QEDL_PLUGIN=ON \
      -DINSTALL_QFACETS_PLUGIN=ON \
      -DINSTALL_QHPR_PLUGIN=ON \
      -DINSTALL_QPOISSON_RECON_PLUGIN=ON \
      -DINSTALL_QSRA_PLUGIN=ON \
      -DINSTALL_QSSAO_PLUGIN=ON \
      -DINSTALL_QGMMREG_PLUGIN=ON \
      -DINSTALL_QCSF_PLUGIN=ON \
      -DINSTALL_QPHOTOSCAN_IO_PLUGIN=ON \
      -DINSTALL_QM3C2_PLUGIN=ON \
      -DINSTALL_QPCV_PLUGIN=ON ..

      if [ $TRAVIS_BRANCH = 'to_vombat' ]; then 
            echo "on branch ${TRAVIS_BRANCH}"
            cmake -DINSTALL_vombat_PLUGIN=ON ..
      fi

      # -DOPTION_USE_GDAL=ON
      # -DINSTALL_QKINECT_PLUGIN=ON
      # -DLIBFREENECT_INCLUDE_DIR="/usr/include"
      # -DLIBFREENECT_LIBRARY_FILE="/usr/lib/x86_64-linux-gnu/libfreenect.so"

      # For now:
      # - qRansacSD fails on Clang
      # - no libav/FFMPEG. Version in repos is < 10.7 (no frame.h)
      # - could not use CGAL from Ubuntu (< 4.3)


	make -j2
fi


