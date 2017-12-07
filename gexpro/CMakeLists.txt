find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})

add_library(gexpro gexpro.cpp)
target_link_libraries(gexpro ${ARMADILLO_LIBRARIES})
