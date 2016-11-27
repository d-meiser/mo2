cmake_minimum_required(VERSION 3.0.2)
cmake_policy(SET CMP0048 OLD)
project(Mo2)


include(CheckCXXCompilerFlag)
include(GenerateExportHeader)


list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

enable_testing()
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

option(MO_BUILD_TESTS "Whether to build tests" OFF)
option(MO_ENABLE_COVERAGE "Instrument tests for coverage" OFF)
option(MO_WITH_ADDRESS_SANITIZER "Whether to build with address sanitizer enabled" OFF)
option(MO_WITH_WALL_FLAGS "Whether to compile with all warning flags." ON)
option(MO_WITH_PEDANTIC_FLAGS "Whether to compile with pedantic flags." OFF)
option(MO_WITH_WARNING_AS_ERRORS "Whether to treat compiler warnings as errors." ON)


set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)


if (MO_WITH_ADDRESS_SANITIZER)
  set(OLD_CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS})
  set(CMAKE_REQUIRED_FLAGS "-fsanitize=address")
  CHECK_CXX_COMPILER_FLAG(-fsanitize=address HAVE_ADDRESS_SANITIZER)
  if (HAVE_ADDRESS_SANITIZER)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  endif ()
  set(CMAKE_REQUIRED_FLAGS ${OLD_CMAKE_REQUIRED_FLAGS})
endif ()

if (CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
  if (MO_ENABLE_COVERAGE)
    add_definitions("-ftest-coverage -fprofile-arcs")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage -fprofile-arcs")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ftest-coverage -fprofile-arcs")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -ftest-coverage -fprofile-arcs")
    set(COV_LIBRARIES gcov)
  endif ()
  if (MO_WITH_WALL_FLAGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
  endif ()
  if (MO_WITH_PEDANTIC_FLAGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
  endif ()
  if (MO_WITH_WARNING_AS_ERRORS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)
endif ()

add_subdirectory(src)
if (MO_BUILD_TESTS)
  add_subdirectory(googletest-release-1.8.0)
  add_subdirectory(tests)
endif ()

configure_file(mo2_config.h.in ${PROJECT_BINARY_DIR}/mo2_config.h)
generate_export_header(mo_lib)

set(HEADERS
    ${PROJECT_BINARY_DIR}/mo2_config.h
    )
install(FILES ${HEADERS} DESTINATION include)
