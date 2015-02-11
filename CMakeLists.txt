cmake_minimum_required(VERSION 2.8.4)
project(yess)

################################################################################
#
# Dependencies
#
################################################################################

set(YESS_LIBRARIES_TO_LINK_WITH)

################################################################################
#
# yess headers and sources
#
################################################################################

# library headers
set(YESS_HEADERS
    yess/yess.h
)

# library sources
set(YESS_SOURCES
    yess/yess.cpp
)

################################################################################
#
# Compiler-specific workarounds
#
################################################################################

if(MSVC)
    # disable "strncpy is deprecated" warning and enable math constants
    add_definitions("/D_CRT_NONSTDC_NO_DEPRECATE /D_USE_MATH_DEFINES")
endif()


if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    if(APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    endif()
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wextra -Wcast-qual -Wcast-align -Wno-unused-parameter -Wmissing-include-dirs -Wpointer-arith -Wredundant-decls -Wshadow -fprofile-arcs -ftest-coverage")
endif()

################################################################################
#
# Building and linking
#
################################################################################

# Main yess library
add_library(yess ${YESS_HEADERS} ${YESS_SOURCES})

# linking with extra libs
target_link_libraries(yess ${YESS_LIBRARIES_TO_LINK_WITH})

################################################################################
#
# Installing
#
################################################################################

# install the binaries and include files
install(TARGETS yess
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
install(DIRECTORY yess/ DESTINATION include/yess FILES_MATCHING PATTERN "*.h")

################################################################################
#
# Uninstalling
#
################################################################################

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
    IMMEDIATE @ONLY
)

add_custom_target(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
)