#!/bin/bash
set -e
echo "compiler ${CXX}, and ${CC}"


echo "compiling for win on mxe"
echo "travis build dir is ${TRAVIS_BUILD_DIR}"
cd ${TRAVIS_BUILD_DIR}
mkdir build
cd build

PATH=${PATH}:/usr/lib/mxe/usr/bin:/usr/lib/mxe/tools


else # default compile options for all branches
      $cmakename -DCMAKE_BUILD_TYPE=Release .. \
      -DCMAKE_INSTALL_PREFIX=${TRAVIS_BUILD_DIR} \
      -DOPTION_BUILD_CCVIEWER=OFF	\
      -DOPTION_USE_DXF_LIB=ON \
      -DOPTION_USE_SHAPE_LIB=ON \
      -DINSTALL_QANIMATION_PLUGIN=ON \
      -DINSTALL_QBLUR_PLUGIN=ON \
      -DINSTALL_QDUMMY_PLUGIN=ON \
      -DINSTALL_QEDL_PLUGIN=ON \
      -DINSTALL_QFACETS_PLUGIN=ON \
      -DINSTALL_QHPR_PLUGIN=ON \
      -DINSTALL_QPOISSON_RECON_PLUGIN=OFF \
      -DINSTALL_QSRA_PLUGIN=ON \
      -DINSTALL_QSSAO_PLUGIN=ON \
      -DINSTALL_QCSF_PLUGIN=ON \
      -DINSTALL_QPHOTOSCAN_IO_PLUGIN=ON \
      -DINSTALL_QM3C2_PLUGIN=ON \
      -DINSTALL_QPCV_PLUGIN=ON 

fi

if [ $TRAVIS_BRANCH = 'to_vombat' ]; then 
      echo "on branch ${TRAVIS_BRANCH}"
      $cmakename ..  \
      -DCMAKE_INSTALL_PREFIX=${TRAVIS_BUILD_DIR} \
      -DINSTALL_vombat_PLUGIN=ON \
fi



make install -j 3
cd ${TRAVIS_BUILD_DIR}
#- wget https://raw.githubusercontent.com/mxe/mxe/master/tools/copydlldeps.sh; chmod a+x copydlldeps.sh
copydlldeps.sh -c -F ./CloudCompare -d ./CloudCompare -R /usr/lib/mxe
name=cloudcompare-${TRAVIS_BRANCH}-${COMPILER}-${TRAVIS_COMMIT}
mv CloudCompare ${name}
tar -zcvf ${name}.tar.gz ${name}
curl -X PUT -u ${OWNCLOUD_USER}:${OWNCLOUD_PASSWORD} "https://ns392452.ip-176-31-109.eu/owncloud/remote.php/webdav/cloudcompare-builds/${name}.tar.gz" --data-binary @"${name}.tar.gz" --insecure

