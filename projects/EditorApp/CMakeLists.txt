getdirname()

# files list 
file(GLOB fileslist "*.cpp" "*.h")
addversion(1 0 0 0 ${appname})

set( fileslist ${fileslist} "${CMAKE_SOURCE_DIR}/main.cpp")
set( fileslist ${fileslist} "${CMAKE_SOURCE_DIR}/settings.cpp")
add_executable (${appname} WIN32 ${fileslist})
TARGET_LINK_LIBRARIES(${appname} ComCtl32)
set_target_properties(${appname} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug")  