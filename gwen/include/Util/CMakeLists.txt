### === Util === ###
file(GLOB UTIL_CONTROLFACTORY_INCLUDES ControlFactory/*.h)
file(GLOB UTIL_CONTROLFACTORY_SOURCES ControlFactory/*.cpp)
source_group("ControlFactory" FILES ${UTIL_CONTROLFACTORY_INCLUDES})
source_group("ControlFactory" FILES ${UTIL_CONTROLFACTORY_SOURCES})

file(GLOB UTIL_IMPORTEXPORT_INCLUDES ImportExport/*.h)
file(GLOB UTIL_IMPORTEXPORT_SOURCES ImportExport/*.cpp)
source_group("ImportExport" FILES ${UTIL_IMPORTEXPORT_INCLUDES})
source_group("ImportExport" FILES ${UTIL_IMPORTEXPORT_SOURCES})


set(inc_Util
	${UTIL_CONTROLFACTORY_INCLUDES}
	${UTIL_IMPORTEXPORT_INCLUDES}
)

set( src_Util
	${UTIL_CONTROLFACTORY_SOURCES}
	${UTIL_IMPORTEXPORT_SOURCES}
)

include_directories("../")

# compiler specific flags
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
	add_compile_options(-pipe)
	add_compile_options(-Wall)

	if(NOT CMAKE_CROSSCOMPILING AND ONATIVE)
		add_compile_options(-march=native)
	elseif(NOT APPLE AND cpu STREQUAL "x86")
		add_compile_options(-march=pentium3)
	endif()

	set(CMAKE_C_FLAGS_DEBUG "-g -D_DEBUG -O1")
	set(CMAKE_C_FLAGS_DEBUGALL "-g -ggdb -D_DEBUG")
	set(CMAKE_C_FLAGS_PROFILE "-g -ggdb -D_DEBUG -O1 -fno-omit-frame-pointer")
	set(CMAKE_C_FLAGS_RELEASE "-O3 -ffast-math -fno-unsafe-math-optimizations -fomit-frame-pointer")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g -O3 -ffast-math -fno-unsafe-math-optimizations -fomit-frame-pointer")
	set(CMAKE_C_FLAGS_MINSIZEREL "-Os -ffast-math -fno-unsafe-math-optimizations -fomit-frame-pointer")

	set(CMAKE_CXX_FLAGS_DEBUGALL ${CMAKE_C_FLAGS_DEBUGALL})
	set(CMAKE_CXX_FLAGS_PROFILE ${CMAKE_C_FLAGS_PROFILE})

	add_compile_options(-fno-strict-aliasing)

	CHECK_CXX_COMPILER_FLAG("-fvisibility=hidden" cxx_has_fvisibility)
	if(NOT cxx_has_fvisibility)
		message(FATAL_ERROR "Compiler does not support -fvisibility")
	endif()
	add_compile_options(-fvisibility=hidden)

	# TODO fix these warnings
	add_compile_options(-Wno-sign-compare)
	add_compile_options(-Wno-switch)
	add_compile_options(-Wno-format-security)

	CHECK_CXX_COMPILER_FLAG("-Woverloaded-virtual" cxx_has_Woverload_virtual)
	if(cxx_has_Woverload_virtual)
		add_compile_options(-Woverloaded-virtual)
	endif()

	if(APPLE)
		add_definitions(-DMACOS_X=1)

		if(cpu STREQUAL "x86_64")
			add_compile_options(-arch x86_64 -mmacosx-version-min=10.6)
			set(ldflags "${ldflags} -arch x86_64 -mmacosx-version-min=10.6")
		elseif(cpu STREQUAL "x86")
			CHECK_CXX_COMPILER_FLAG("-arch i386" cxx_has_arch_i386)
			if(cxx_has_arch_i386)
				add_compile_options(-arch i386)
				set(ldflags "${ldflags} -arch i386")
			endif()

			add_compile_options(-mmacosx-version-min=10.4)
			set(ldflags "${ldflags} -mmacosx-version-min=10.4")
		elseif(cpu STREQUAL "ppc")
			CHECK_CXX_COMPILER_FLAG("-arch ppc" cxx_has_arch_ppc)
			if(cxx_has_arch_ppc)
				add_compile_options(-arch ppc)
				set(ldflags "${ldflags} -arch ppc")
			endif()

			add_compile_options(-mmacosx-version-min=10.4)
			set(ldflags "${ldflags} -mmacosx-version-min=10.4")
		else()
			message(FATAL_ERROR "Unsupported CPU architecture for OSX")
		endif()

		set(sys_libs ${sys_libs} "-framework Carbon -framework Cocoa -framework IOKit")
	elseif(WIN32)
		set(ldflags "${ldflags} -static-libgcc -static-libstdc++")
	elseif(os STREQUAL "linux")
		set(sys_libs ${sys_libs} dl)
	endif()
elseif(MSVC)
	add_compile_options(/W4)
	add_compile_options(/wd4100) # unreferenced formal parameter
	#add_compile_options(/wd4127) # conditional expression is constant
	#add_compile_options(/wd4244) # possible loss of data
	#add_compile_options(/wd4245) # signed/unsigned mismatch
	#add_compile_options(/wd4267) # possible loss of data
	#add_compile_options(/wd4714) # 'function' marked as __forceinline not inlined
	#add_compile_options(/wd4996) # 'function': was declared deprecated
	#add_compile_options(/wd4068) # unknown pragma
	#add_definitions(-D_ALLOW_KEYWORD_MACROS) # because of the "#define private public" and "#define protected public" in TypeInfo.cpp
	set(CMAKE_C_FLAGS_DEBUG "-D_DEBUG /Od /Zi /MDd")
	set(CMAKE_C_FLAGS_RELEASE "/Ox /Oy /MD")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO "/Ox /Oy /Zi /MD")
	set(CMAKE_C_FLAGS_MINSIZEREL "/Ox /Oy /Os /MD")
else()
	message(FATAL_ERROR "Unsupported compiler")
endif()

	include_directories(${CMAKE_BINARY_DIR})
	include_directories(${CMAKE_SOURCE_DIR})

	if(BUILD_STATIC)
		add_library(Util_static STATIC ${inc_Util} ${src_Util})
	else(BUILD_STATIC)
		add_library(Util_shared SHARED ${inc_Util} ${src_Util})
	endif(BUILD_STATIC)
		
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID STREQUAL "Clang" AND NOT MINGW)
	if(BUILD_STATIC)
		set_target_properties(Util_static PROPERTIES COMPILE_FLAGS "-fPIC")
	else(BUILD_STATIC)
		set_target_properties(Util_shared PROPERTIES COMPILE_FLAGS "-fPIC")
	endif(BUILD_STATIC)
endif()