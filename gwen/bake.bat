del /s /q ../build
mkdir ../build
cd ../build
cmake -G "Visual Studio 15" -DCMAKE_INSTALL_PREFIX=../gwen/bin/win10-32 ../../gwen -DSDL2_INCLUDE_DIR=C:/CppLibs/SDL2/include -DSDL2_LIBRARIES=C:/CppLibs/SDL2/lib/x86/SDL2.lib -DSDL2_BINARY_DIRS=C:/CppLibs/SDL2/lib/x86
pause