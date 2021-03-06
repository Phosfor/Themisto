cmake_minimum_required(VERSION 2.8.0)

project(Themisto)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-extras/")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

#set(CMAKE_C_COMPILER clang)
#set(CMAKE_CXX_COMPILER clang++)

find_package(X11 REQUIRED)
find_package(Freetype REQUIRED)
find_package(Clanlib 2.2 REQUIRED)
find_package(Boost COMPONENTS python filesystem system date_time REQUIRED)
find_package(PythonLibs REQUIRED)

find_package(FreeImage REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ZLIB REQUIRED)

find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(Box2D REQUIRED)

add_definitions(-Wall -g -msse2 -fPIC -std=c++0x)
set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_EXE_LINKER_FLAGS "-pg")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/lib)

include_directories(
	"${CMAKE_SOURCE_DIR}/include/"
	${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
	${ClanLib_INCLUDE_DIRS}
	${BOX2D_INCLUDE_DIR}
)

message(${CMAKE_CXX_COMPILER_ID})

add_subdirectory("${CMAKE_SOURCE_DIR}/src/")
