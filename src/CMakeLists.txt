find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})

# include library
include_directories(${gexpro_SOURCE_DIR}/gexpro)
link_directories(${gexpro_SOURCE_DIR}/gexpro)

add_executable(mkgexpro mkgexpro.cpp)
target_link_libraries(mkgexpro gexpro ${ARMADILLO_LIBRARIES})
