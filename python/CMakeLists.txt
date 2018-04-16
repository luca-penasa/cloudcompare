#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

#find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

FIND_PACKAGE(PyQt5 REQUIRED)
SET(PYQT_SIP_FLAGS ${PYQT5_SIP_FLAGS})
SET(PYQT_SIP_DIR ${PYQT5_SIP_DIR})
SEPARATE_ARGUMENTS(PYQT_SIP_FLAGS) # convert space separated values to a list


find_package(SIP REQUIRED)

if(NOT DEFINED LIB_SUFFIX)
    set(LIB_SUFFIX "")
endif()

if(EXISTS /etc/debian_version)
    # Running on a debian-based system, which requires special handling for python modules.
    set(PYTHON_SITE_PACKAGES_DIR lib${LIB_SUFFIX}/python3/dist-packages CACHE STRING "Directory to install Python bindings to")
else()
    set(PYTHON_SITE_PACKAGES_DIR lib${LIB_SUFFIX}/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages CACHE STRING "Directory to install Python bindings to")
endif()

include_directories(python/ src/ ${SIP_INCLUDE_DIR} ${PYTHON_INCLUDE_DIR})
#set(sip_includes ${PYQT5_SIP_DIR})

INCLUDE_DIRECTORIES("../libs/qCC_db/")
INCLUDE_DIRECTORIES("../CC/include/")


cmake_policy(SET CMP0071 OLD)
message("flags ${PYQT_SIP_FLAGS}")
#set(SIP_EXTRA_FILES_DEPEND python/core/cc.sip)
#set(SIP_EXTRA_SOURCE_FILES python/PythonMessage.cpp)
set(SIP_EXTRA_OPTIONS ${PYQT_SIP_FLAGS} -g -I ${PYQT5_SIP_DIR}) # -g means always release the GIL before calling C++ methods.
add_sip_python_module(cc core/cc.sip QCC_DB_LIB QCC_IO_LIB)
