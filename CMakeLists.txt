if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

cmake_minimum_required(VERSION 3.10)
project(gexpro)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(gexpro_VERSION_MAJOR 1)
set(gexpro_VERSION_MINOR 0)

subdirs(gexpro src)
