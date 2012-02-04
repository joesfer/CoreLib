cmake_minimum_required(VERSION 2.6)

project(CoreLib)

set( CORELIB_NAME "CoreLib" )
set( CORELIB_OUTPUT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/lib )

# --------- Setup the Library output Directory -------------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CORELIB_OUTPUT_FOLDER} )

#specify app sources
function(create_source_group sourceGroupName relativeSourcePath)
	foreach(currentSourceFile ${ARGN})
		file(RELATIVE_PATH folder ${relativeSourcePath} ${currentSourceFile})
		get_filename_component(filename ${folder} NAME)
		string(REPLACE ${filename} "" folder ${folder})
		if(NOT folder STREQUAL "")
			string(REGEX REPLACE "/+$" "" folderlast ${folder})
			string(REPLACE "/" "\\" folderlast ${folderlast})
			source_group("${sourceGroupName}\\${folderlast}" FILES ${currentSourceFile})
		endif(NOT folder STREQUAL "")
	endforeach(currentSourceFile ${ARGN})
endfunction(create_source_group)

file(GLOB_RECURSE app_sources source/*.c source/*.cpp source/*.h source/*.inl source/*.hpp source/*.glsl source/*.ui)

# mimic disk folder structure on the project
create_source_group( "" "${CMAKE_CURRENT_SOURCE_DIR}/source" ${app_sources} )

# add base to include directories
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/source)

#sourceGroups( app_sources )
add_library(${CORELIB_NAME} STATIC ${app_sources})

set_target_properties( ${CORELIB_NAME} PROPERTIES PREFIX "" )
set_target_properties( ${CORELIB_NAME} PROPERTIES OUTPUT_NAME ${CORELIB_NAME} )
set_target_properties( ${CORELIB_NAME} PROPERTIES LINKER_LANGUAGE C)