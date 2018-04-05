cmake_minimum_required(VERSION 3.0)

option( INSTALL_QPCLSAIO_PLUGIN "Check to install qPCL plugin" OFF )

# CloudCompare 'PCL' bridge plugin
if (INSTALL_QPCLSAIO_PLUGIN)
	project( QPCLSA_IO_PLUGIN )
	add_subdirectory(minipcl)
	include_directories(minipcl/minipcl)

	#required by miniPCL
	find_package(Eigen3 REQUIRED)
	include_directories(${EIGEN3_INCLUDE_DIR})

	FIND_PACKAGE( Boost 1.40 COMPONENTS filesystem system iostreams REQUIRED )
	INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

	file( GLOB_RECURSE header_list utils/*.h )
	file( GLOB_RECURSE source_list utils/*.cpp )

	add_library( PclUtils STATIC ${header_list} ${source_list} )

	target_link_libraries( PclUtils minipcl ${Boost_LIBRARIES} )

	include_directories(utils)

	set( CC_IS_IO_PLUGIN 1 )

	include( ../CMakePluginTpl.cmake )

	target_link_libraries(${PROJECT_NAME} PclUtils)

endif()
