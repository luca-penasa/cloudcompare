#!/bin/bash
set -e
echo "compiler ${CXX}, and ${CC}"


mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release \
-DQT5_ROOT_PATH=/usr/local/opt/Qt5 \
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
#-DINSTALL_QGMMREG_PLUGIN=ON \ #vxl is needed
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
