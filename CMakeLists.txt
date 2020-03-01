#CMake is a chore, I don't exactly like using it, but it's the best option I can think of for
#doing anything where I want my code to be cross-platform and support/be supported by vcpkg
cmake_minimum_required (VERSION 3.8)

project ("Elated" C CXX)

add_custom_target(copy_patterns ALL
  COMMAND ${CMAKE_COMMAND} -DSRCDIR=${CMAKE_CURRENT_SOURCE_DIR}/data
    -DDSTDIR=${CMAKE_CURRENT_BINARY_DIR}/data
    -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/copy_patterns.cmake
  COMMENT "Copying pattern files to build tree")

find_package(glfw3 REQUIRED CONFIG)
find_package(glad REQUIRED CONFIG)
find_package(duktape REQUIRED CONFIG)

file(GLOB_RECURSE HEADERS src/*.hpp src/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp src/*.c)

add_executable(Elated ${HEADERS} ${SOURCES})
target_link_libraries(Elated PRIVATE glad::glad glfw ${DUKTAPE_LIBRARIES})

#c++ why
if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRTD.lib")
endif()