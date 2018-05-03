# file CMakeLists.txt
cmake_minimum_required(VERSION 2.6)

if(BUILD_STATIC)
	SET(GWEN_LIBS
		"gwen_static"
	)
else(BUILD_STATIC)
	SET(GWEN_LIBS
		"gwen"
	)
endif(BUILD_STATIC)

set(INCLUDE_LIBS
	${GWEN_LIBS}
	${SDL2TTF_LIBRARIES}
	${SDL2_LIBRARIES}
	)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(Defalt main.cpp)
target_link_libraries(Defalt ${INCLUDE_LIBS})