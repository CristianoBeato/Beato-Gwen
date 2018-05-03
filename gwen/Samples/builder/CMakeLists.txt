# file CMakeLists.txt
cmake_minimum_required(VERSION 2.6)

if(BUILD_STATIC)
	SET(GWEN_LIBS
		"gwenBuilder_static"
		"gwen_static"
	)
else(BUILD_STATIC)
	SET(GWEN_LIBS
		"gwenBuilder"
		"gwen"
	)
endif(BUILD_STATIC)

set(INCLUDE_LIBS
	${GWEN_LIBS}
	)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(BuilderSampler main.cpp)
target_link_libraries(BuilderSampler ${INCLUDE_LIBS})