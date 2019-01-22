# file CMakeLists.txt
cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

project(Gwen)	

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

option(BUILD_STATIC						"Build a static lib istead of sharead library" ON)
option(BUILD_FORM_BUILDE				"suport for XML interface Builde" ON)
option(BUILD_WHIT_SAMPLES				"gui samples" ON )
option(BUILD_WHIT_UTILS					"Build Whit a utils" ON )
option(BUILD_UNIT_TEST					"Build a test set" OFF )
option(USE_PRECOMPILED_HEADERS			"Use precompiled headers to speedup buld" ON)

#================================== Custom CMake Modules ==================================#
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules")
#include(MSVCMultipleProcessCompile) # /MP
#include(PrecompiledHeader)

#================================== Find for the libraryes ==================================#

#if (BUILD_WHIT_GL_RENDER)
#search for OpenGL for render Graphic
	#find_package(OPENGL REQUIRED)
	#include_directories(${OPENGL_INCLUDE_DIR})

	#find_package(GLEW REQUIRED)
	#include_directories(${OPENGL_INCLUDE_DIR})
#endif(BUILD_WHIT_GL_RENDER)

#search for sdl for low level interface 
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

find_package(SDL2ttf REQUIRED)
include_directories(${SDL2TTF_INCLUDE_DIR})
include_directories(include/)

if(BUILD_FORM_BUILDE)
	add_subdirectory(include/GwenBuilder)
endif(BUILD_FORM_BUILDE)
if(BUILD_WHIT_SAMPLES)
	add_subdirectory(Samples/builder)
	add_subdirectory(Samples/defalt)
endif(BUILD_WHIT_SAMPLES)
if(BUILD_UNIT_TEST)
	add_subdirectory(include/UnitTest)
endif(BUILD_UNIT_TEST)
if(BUILD_WHIT_UTILS)
	add_subdirectory(include/Util)
endif(BUILD_WHIT_UTILS)

##
## Gwen Lib
##
file(GLOB GWEN_INCLUDES include/Gwen/*.h)
file(GLOB GWEN_SOURCES src/Gwen/*.cpp)
source_group("" FILES ${GWEN_INCLUDES})
source_group("" FILES ${GWEN_SOURCES})

file(GLOB CONTROLS_INCLUDES include/Gwen/Controls/*.h)
file(GLOB CONTROLS_SOURCES src/Gwen/Controls/*.cpp)
source_group("Controls" FILES ${CONTROLS_INCLUDES})
source_group("Controls" FILES ${CONTROLS_SOURCES})

file(GLOB CONTROLS_DLG_INCLUDES include/Gwen/Controls/Dialogs/*.h)
file(GLOB CONTROLS_DLG_SOURCES src/Gwen/Controls/Dialogs/*.cpp)
source_group("Controls\\Dialogs" FILES ${CONTROLS_DLG_INCLUDES})
source_group("Controls\\Dialogs" FILES ${CONTROLS_DLG_SOURCES})

file(GLOB CONTROLS_LYT_INCLUDES include/Gwen/Controls/Layout/*.h)
file(GLOB CONTROLS_LYT_SOURCES src/Gwen/Controls/Layout/*.cpp)
source_group("Controls\\Layout" FILES ${CONTROLS_LYT_INCLUDES})
source_group("Controls\\Layout" FILES ${CONTROLS_LYT_SOURCES})

file(GLOB CONTROLS_PPT_INCLUDES include/Gwen/Controls/Property/*.h)
file(GLOB CONTROLS_PPT_SOURCES src/Gwen/Controls/Property/*.cpp)
source_group("Controls\\Property" FILES ${CONTROLS_PPT_INCLUDES})
source_group("Controls\\Property" FILES ${CONTROLS_PPT_SOURCES})

file(GLOB FONT_INCLUDES include/Gwen/Font/*.h)
file(GLOB FONT_SOURCES src/Gwen/Font/*.cpp)
source_group("Fonts" FILES ${FONT_INCLUDES})
source_group("Fonts" FILES ${FONT_SOURCES})

file(GLOB INPUT_INCLUDES include/Gwen/Input/*.h)
file(GLOB INPUT_SOURCES src/Gwen/Input/*.cpp)
source_group("Input" FILES ${INPUT_INCLUDES})
source_group("Input" FILES ${INPUT_SOURCES})

file(GLOB PLATF_INCLUDES include/Gwen/Platforms/*.h)
file(GLOB PLATF_SOURCES src/Gwen/Platforms/*.cpp)
source_group("Platforms" FILES ${PLATF_INCLUDES})
source_group("Platforms" FILES ${PLATF_SOURCES})

file(GLOB RENDERERS_INCLUDES include/Gwen/Renderers/*.h)
file(GLOB RENDERERS_SOURCES src/Gwen/Renderers/*.cpp)
source_group("Renderers" FILES ${RENDERERS_INCLUDES})
source_group("Renderers" FILES ${RENDERERS_SOURCES})

file(GLOB SKINS_INCLUDES include/Gwen/Skins/*.h)
file(GLOB SKINS_SOURCES src/Gwen/Skins/*.cpp)
source_group("Skins" FILES ${SKINS_INCLUDES})
source_group("Skins" FILES ${SKINS_SOURCES})

file(GLOB TEXTURE_INCLUDES include/Gwen/texture/*.h)
file(GLOB TEXTURE_SOURCES src/Gwen/texture/*.cpp)
source_group("texture" FILES ${TEXTURE_INCLUDES})
source_group("texture" FILES ${TEXTURE_SOURCES})

#contrib
file(GLOB CONTRIB_TINYXML_INCLUDES src/contrib/tinyxml2/*.h)
file(GLOB CONTRIB_TINYXML_SOURCES src/contrib/tinyxml2/*.cpp)
source_group("contrib\\tinyxml2" FILES ${CONTRIB_TINYXML_INCLUDES})
source_group("contrib\\tinyxml2" FILES ${CONTRIB_TINYXML_SOURCES})


set(INC_GWEN
	${GWEN_INCLUDES}
	${CONTROLS_INCLUDES}
	${CONTROLS_DLG_INCLUDES}
	${CONTROLS_LYT_INCLUDES}
	${CONTROLS_PPT_INCLUDES}
	${FONT_INCLUDES}
	${INPUT_INCLUDES}
	${PLATF_INCLUDES}
	${RENDERERS_INCLUDES}
	${CONTRIB_TINYXML_INCLUDES}
	${SKINS_INCLUDES}
	${TEXTURE_INCLUDES}
)

set( SRC_GWEN
	${GWEN_SOURCES}
	${CONTROLS_SOURCES}
	${CONTROLS_DLG_SOURCES}
	${CONTROLS_LYT_SOURCES}
	${FONT_SOURCES}
	${INPUT_SOURCES}
	${PLATF_SOURCES}
	${RENDERERS_SOURCES}
	${CONTRIB_TINYXML_SOURCES}
	${SKINS_SOURCES}
	${TEXTURE_SOURCES}
)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/Gwen)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Gwen)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/contrib/tinyxml2)


#set up precompiled Header
if(USE_PRECOMPILED_HEADERS)
	set(GWEN_PRECOMPILED_SOURCES ${SRC_GWEN})
	#remove sources that not include the precompiled header 
	list(REMOVE_ITEM GWEN_PRECOMPILED_SOURCES ${CONTRIB_TINYXML_SOURCES})
	list(REMOVE_ITEM GWEN_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/Gwen/precompiled.cpp)
	
if(MSVC)
	   		set_source_files_properties( ${GWEN_PRECOMPILED_SOURCES} PROPERTIES
									COMPILE_FLAGS "/Yuprecompiled.h"
									OBJECT_DEPENDS "precompiled.pch"
		)
		
		set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/Gwen/precompiled.cpp PROPERTIES
									COMPILE_FLAGS "/Ycprecompiled.h"
									OBJECT_OUTPUTS "precompiled.pch"
		)
	else()
		foreach( src_file ${GWEN_PRECOMPILED_SOURCES} )
		#message(STATUS "-include precompiled.h for ${src_file}")
		set_source_files_properties(
			${src_file}
			PROPERTIES
			COMPILE_FLAGS "-include ${CMAKE_CURRENT_SOURCE_DIR}/src/Gwen/precompiled.h"
			)
		endforeach()
		
		# precompiled magic for GCC/clang, adapted from https://gist.github.com/573926
		STRING(TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" _flags_var_name)
		SET(_compiler_FLAGS ${${_flags_var_name}})
		GET_DIRECTORY_PROPERTY(_directory_flags INCLUDE_DIRECTORIES)
		FOREACH(item ${_directory_flags})
		LIST(APPEND _compiler_FLAGS " -I${item}")
		ENDFOREACH(item)
		
		add_custom_target(precomp_header_renderer ALL
	                  COMMAND ${CMAKE_CXX_COMPILER} ${_compiler_FLAGS} -x c++-header precompiled.h -o precompiled.h.gch
	                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	                  COMMENT "Creating Gwen/precompiled.h.gch for renderer"
	                  )
	endif()
	
endif(USE_PRECOMPILED_HEADERS)

if(BUILD_STATIC)
	add_library(gwen_static STATIC ${SRC_GWEN} ${INC_GWEN})
else(BUILD_STATIC)
	add_library(gwen SHARED ${SRC_GWEN} ${INC_GWEN})
endif(BUILD_STATIC)