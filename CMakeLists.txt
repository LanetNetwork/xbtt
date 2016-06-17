cmake_minimum_required(VERSION 2.8.11)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to 'Release' as none was specified.")
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)

if (CMAKE_BUILD_TYPE MATCHES Debug)
	message("This is DEBUG build")
endif (CMAKE_BUILD_TYPE MATCHES Debug)

include(FindPkgConfig)
if (PKG_CONFIG_FOUND)
	pkg_check_modules(ZLIB REQUIRED zlib)
	pkg_check_modules(OPENSSL REQUIRED openssl)
	if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
		pkg_check_modules(LIBTCMALLOC_MINIMAL libtcmalloc_minimal)
		if(LIBTCMALLOC_MINIMAL_FOUND EQUAL 1)
			set(GB_LD_EXTRA "${LIBTCMALLOC_MINIMAL_LIBRARIES}")
		endif (LIBTCMALLOC_MINIMAL_FOUND EQUAL 1)
	endif(NOT CMAKE_BUILD_TYPE MATCHES Debug)
endif (PKG_CONFIG_FOUND)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake-scripts)
find_package(MySQL REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++11 -W -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb -pg")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

include_directories(inc)

add_executable(xbtt
	alerts.cpp
	bt_misc.cpp
	bt_torrent.cpp
	bt_tracker_account.cpp
	bt_tracker_url.cpp
	bvalue.cpp
	config.cpp
	connection.cpp
	database.cpp
	epoll.cpp
	server.cpp
	sha1.cpp
	socket.cpp
	sql_query.cpp
	stdafx.cpp
	stream_reader.cpp
	stream_writer.cpp
	tf_misc.cpp
	tracker_input.cpp
	transaction.cpp
	virtual_binary.cpp
	xbtt.cpp
	xcc_z.cpp
	xif_key.cpp
	xif_key_r.cpp
	xif_value.cpp)

target_link_libraries(xbtt
	pthread
	m
	dl
	${MYSQL_LIBRARIES}
	${ZLIB_LIBRARIES}
	${OPENSSL_LIBRARIES}
	${GB_LD_EXTRA})

install(TARGETS xbtt
	RUNTIME DESTINATION bin)

