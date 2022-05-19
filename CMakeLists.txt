# Project
cmake_minimum_required(VERSION 3.16)
project(SeeCheatSheet VERSION 3.0)

# TODO: boost and source-highlight download

# Global compile options
set(CMAKE_CXX_STANDARD 17)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -fstandalone-debug)
    add_link_options(-rdynamic)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-Wall -Wextra -g3)
    add_link_options(-rdynamic)
  endif()
endif()
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
configure_file(utils/project.h.in utils/project.hpp @ONLY)

add_subdirectory(third_party/unicode_display_width)

set(BUILD_SHARED_LIBS
    OFF
    CACHE BOOL "build md4c with static library" FORCE)
add_subdirectory(third_party/md4c)
include_directories(third_party/md4c/src)

set(WITH_TESTS
    OFF
    CACHE BOOL "disable eternal tests" FORCE)
add_subdirectory(third_party/eternal)
target_include_directories(eternal INTERFACE ${eternal_SOURCE_DIR}/include)

# Target: utils
file(GLOB UTILS_FILES CONFIGURE_DEPENDS utils/*.cpp)
add_library(utils STATIC ${UTILS_FILES})
target_link_libraries(utils PRIVATE boost_locale unicode_display_width)

# Target: md2ansi
file(GLOB MD2ANSI_FILES md2ansi/*.cpp)
add_library(md2ansi STATIC ${MD2ANSI_FILES})
target_link_libraries(md2ansi PRIVATE utils md4c md4c-html eternal boost_locale
                                      source-highlight)

# Target: test_bin
file(GLOB TEST_FILES test/*.cpp)
add_executable(test_bin ${TEST_FILES})
target_link_libraries(test_bin PRIVATE utils md2ansi pthread gtest gtest_main)

# Target: test
add_custom_target(
  test
  ./test_bin
  DEPENDS test_bin)

# Target: see
add_executable(see see.cpp)
target_link_libraries(see PRIVATE utils md2ansi unicode_display_width)

# Target: prof
add_custom_target(
  prof
  env
  LD_PRELOAD=/usr/lib/libtcmalloc_and_profiler.so
  CPUPROFILE=prof.out
  CPUPROFILE_FREQUENCY=1000
  ./see
  -p
  .
  >/dev/null
  COMMAND pprof --web ./see prof.out
  DEPENDS see)

# Tartget: doc
find_package(Doxygen)
if(DOXYGEN_FOUND)
  set(DOXYGEN_EXCLUDE_PATTERNS */build/* */third_party/* */test/*)
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
  set(DOXYGEN_HTML_OUTPUT ${CMAKE_BINARY_DIR}/doc)
  set(DOXYGEN_REFERENCED_BY_RELATION YES)
  set(DOXYGEN_REFERENCES_RELATION YES)
  set(DOXYGEN_CALL_GRAPH YES)
  set(DOXYGEN_CALLER_GRAPH YES)
  set(DOXYGEN_DOT_IMAGE_FORMAT png:cairo:cairo)
  set(DOXYGEN_UML_LOOK YES)
  set(DOXYGEN_EXTRACT_ALL YES)
  set(DOXYGEN_EXTRACT_STATIC YES)
  set(DOXYGEN_EXTRACT_PRIVATE YES)
  set(DOXYGEN_SOURCE_BROWSER YES)
  set(DOXYGEN_STRIP_CODE_COMMENTS NO)
  set(DOXYGEN_SORT_MEMBER_DOCS NO)
  doxygen_add_docs(doc ${PROJECT_SOURCE_DIR} ALL)
endif()

# Install
install(TARGETS see RUNTIME)
