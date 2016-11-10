echo "travis build dir is ${TRAVIS_BUILD_DIR}"
cd ${TRAVIS_BUILD_DIR}
mkdir build
cd build

PATH=${PATH}:/usr/lib/mxe/usr/bin:/usr/lib/mxe/tools

if [[ $TRAVIS_BRANCH == 'to_vombat' ]]; then # on this branch we also need boost and eigen
	i686-w64-mingw32.shared-cmake -DCMAKE_BUILD_TYPE=Release .. -DCMAKE_INSTALL_PREFIX=${TRAVIS_BUILD_DIR} -DOPTION_BUILD_CCVIEWER=OFF -DINSTALL_vombat_PLUGIN=ON
else # default compile options for all branches

	i686-w64-mingw32.shared-cmake -DCMAKE_BUILD_TYPE=Release .. -DCMAKE_INSTALL_PREFIX=${TRAVIS_BUILD_DIR} -DOPTION_BUILD_CCVIEWER=OFF	
fi



make install -j 3
cd ${TRAVIS_BUILD_DIR}
#- wget https://raw.githubusercontent.com/mxe/mxe/master/tools/copydlldeps.sh; chmod a+x copydlldeps.sh
copydlldeps.sh -c -F ./CloudCompare -d ./CloudCompare -R /usr/lib/mxe
name=cloudcompare-${TRAVIS_BRANCH}-${TRAVIS_COMMIT}
mv CloudCompare ${name}
tar -zcvf ${name}.tar.gz ${name}
curl -X PUT -u ${OWNCLOUD_USER}:${OWNCLOUD_PASSWORD} "https://ns392452.ip-176-31-109.eu/owncloud/remote.php/webdav/cloudcompare-builds/${name}.tar.gz" --data-binary @"${name}.tar.gz" --insecure
