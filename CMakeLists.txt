# CMAKE stuff
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

cmake_minimum_required(VERSION 3.9.2)
project(gexpro)

set(CMAKE_CXX_STANDARD 11)

# Compilation
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Documentation
if(CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
  option(BUILD_DOC "Build doxygen documnetation" ON)
  
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")
    add_custom_target( doc_doxygen ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM )
  else(DOXYGEN_FOUND)
    message("Doxygen needs to be installed for generating documentation")
  endif(DOXYGEN_FOUND)
endif()

# Gexpro stuff
set(gexpro_VERSION_MAJOR 1)
set(gexpro_VERSION_MINOR 0)

subdirs(gexpro src)
