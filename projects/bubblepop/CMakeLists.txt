
getdirname()

# files list 
file(GLOB fileslist "*.cpp" "*.h")
set(G_RESOURCES_EXTRA "
501 RCDATA STRINGIZE(${CMAKE_CURRENT_SOURCE_DIR}/Holstein.DDS)
502 RCDATA STRINGIZE(${CMAKE_CURRENT_SOURCE_DIR}/TextVertexShader.vertexshader)
503 RCDATA STRINGIZE(${CMAKE_CURRENT_SOURCE_DIR}/TextVertexShader.fragmentshader)
")

addversion(1 0 0 0 ${ProjectId})
set( fileslist ${fileslist} "${CMAKE_SOURCE_DIR}/main.cpp")
set( fileslist ${fileslist} "${CMAKE_SOURCE_DIR}/settings.cpp")

if( MSVC )
	set( freeglutdir "${CMAKE_CURRENT_SOURCE_DIR}/deps/freeglut")
	set( glewdir "${CMAKE_CURRENT_SOURCE_DIR}/deps/glew")
	set( freetypedir "${CMAKE_CURRENT_SOURCE_DIR}/deps/freetype")
else()
	set( freeglutdir "${CMAKE_CURRENT_SOURCE_DIR}/deps/freeglutmingw")
	set( glewdir "${CMAKE_CURRENT_SOURCE_DIR}/deps/glewmingw")
	set( freetypedir "${CMAKE_CURRENT_SOURCE_DIR}/deps/freetypemingw")
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/glm-0.9.4.4")
include_directories(${freeglutdir}/include)
link_directories(${freeglutdir}/lib)

include_directories(${glewdir}/include)
if( MSVC )
link_directories(${glewdir}/lib/Release/Win32)
else()
link_directories(${glewdir}/lib)
endif()


include_directories(${freetypedir}/include)
if( MSVC )
link_directories(${freetypedir}/win32)
else()
link_directories(${freetypedir}/lib)
include_directories(${freetypedir}/include/freetype2)
endif()

find_package(OpenGL  REQUIRED)
#find_package(GLUT)
#find_package(Freetype REQUIRED)

add_executable (${appname} WIN32 ${fileslist})
TARGET_LINK_LIBRARIES(${appname} freeglut.lib glew32.lib)
if( MSVC )
TARGET_LINK_LIBRARIES(${appname} glew32.lib)
else()
TARGET_LINK_LIBRARIES(${appname} libglew32 shlwapi libfreetype.dll)
endif()

target_link_libraries(${appname} ${OPENGL_LIBRARIES} )
if( MSVC )

add_custom_command(TARGET ${appname} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "\"${freeglutdir}/bin/freeglut.dll\""      # <--this is in-file
        \"${CMAKE_BINARY_DIR}/bin/\$\(Configuration\)\")                 # <--this is out-file path
		 
add_custom_command(TARGET ${appname} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "\"${glewdir}/bin/Release/Win32/glew32.dll\""      # <--this is in-file
        \"${CMAKE_BINARY_DIR}/bin/\$\(Configuration\)\")                 # <--this is out-file path

		add_custom_command(TARGET ${appname} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "\"${freetypedir}/win32/freetype.dll\""      # <--this is in-file
        \"${CMAKE_BINARY_DIR}/bin/\$\(Configuration\)\")                 # <--this is out-file path
else()

add_custom_command(TARGET ${appname} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "\"${glewdir}/lib/glew32.dll\""      # <--this is in-file
        \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}\")                 # <--this is out-file path

		add_custom_command(TARGET ${appname} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "\"${freetypedir}/bin/freetype6.dll\""      # <--this is in-file
        \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}\")                 # <--this is out-file path


endif()