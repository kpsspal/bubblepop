# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.0)

# Set the output folder where your program will be created
#set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

if( MSVC )
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /SAFESEH:NO")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
#else()
 # set(CMAKE_RC_COMPILER_INIT windres)
  #ENABLE_LANGUAGE(RC)
  #SET(CMAKE_RC_COMPILE_OBJECT
  #  "<CMAKE_RC_COMPILER> -O coff -i <SOURCE> -o <OBJECT>")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#get current source dir name
macro(getdirname)
get_filename_component(appname ${CMAKE_CURRENT_SOURCE_DIR} NAME)
add_definitions(-DAPPNAME=${appname})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

endmacro(getdirname)

# The version number.
macro(addversion major minor revision build filedesc)
	set (G_VERSION_MAJOR ${major})
	set (G_VERSION_MINOR ${minor})
	set (G_VERSION_REVISION ${revision})
	set (G_VERSION_BUILD ${build})
	set (G_VERSION_STRING "${G_VERSION_MAJOR}.${G_VERSION_MINOR}.${G_VERSION_REVISION}.${G_VERSION_BUILD}")
	set (G_FILENAME ${ProjectId})
	set (G_FILEDESC ${filedesc})

	configure_file ( 
	 "${CMAKE_SOURCE_DIR}/version.h.in"
	 "${CMAKE_CURRENT_BINARY_DIR}/version.h"
	  )

	configure_file (
	  "${CMAKE_SOURCE_DIR}/version.rc.in"
	  "${CMAKE_CURRENT_BINARY_DIR}/version.rc"
	  )

	set( fileslist ${fileslist} "${CMAKE_CURRENT_BINARY_DIR}/version.rc")
endmacro(addversion )

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
include_directories(${CMAKE_SOURCE_DIR})

# Project's name
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${ProjectId})
set (G_COMPANY_NAME "CompanyName")

add_subdirectory(bubblepop)
add_subdirectory(EditorApp)

