cmake_minimum_required(VERSION 2.8.0)

project(Themisto)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-extras/")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

find_package(X11 REQUIRED)
find_package(Freetype REQUIRED)
find_package(Clanlib 2.2 REQUIRED)
find_package(Boost COMPONENTS filesystem system date_time REQUIRED)

find_package(FreeImage REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ZLIB REQUIRED)

find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(Box2D REQUIRED)

add_definitions(-Wall -g -O2 -msse2)
#add_definitions(-Wall -pg -g)
set(CMAKE_BUILD_TYPE "Release")
#set(CMAKE_EXE_LINKER_FLAGS "-pg")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

include_directories(
	"${CMAKE_SOURCE_DIR}/include/"
	${Boost_INCLUDE_DIRS}
	${ClanLib_INCLUDE_DIRS}
	${BOX2D_INCLUDE_DIR}
)

add_subdirectory("${CMAKE_SOURCE_DIR}/src/")
