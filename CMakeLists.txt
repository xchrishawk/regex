#
# CMakeLists.txt
# Chris Vig (chris@invictus.so)
#

# -- Project Configuration --

cmake_minimum_required(VERSION 2.8.7)
project(regex)

# Targets
set(MAIN_TARGET ${CMAKE_PROJECT_NAME})
set(TESTS_TARGET ${CMAKE_PROJECT_NAME}_tests)

# Directories
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Toolchain common configuration
set(CMAKE_CXX_FLAGS "-std=gnu++14 -Wall -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-Werror -O2")

# Toolchain-specific configuration
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

  # Silence unneeded warning
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-braces -Wmissing-field-initializers")

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

  # GCC does not warn about -s flag
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

else()

  # Only support GCC and Clang currently
  message(FATAL_ERROR "Unsupported C++ compiler: ${CMAKE_CXX_COMPILER_ID}")

endif()

# -- Third Party Libraries --

# Google Test (for unit testing)
find_package(GTest)

# -- Main Executable --

# Build main executable
add_executable(${MAIN_TARGET}
  ${SOURCE_DIR}/lexical_analyzer.cpp
  ${SOURCE_DIR}/main.cpp
  ${SOURCE_DIR}/syntax.cpp
  ${SOURCE_DIR}/syntax_analyzer.cpp)
target_include_directories(${MAIN_TARGET}
  PRIVATE ${SOURCE_DIR})

# Run main executable
add_custom_target(run
  COMMAND ${MAIN_TARGET}
  DEPENDS ${MAIN_TARGET}
  WORKING_DIRECTORY ${BUILD_DIR}
  COMMENT "Running ${CMAKE_PROJECT_NAME}...")

# -- Tests Executable --

if (${GTEST_FOUND})

  # Builds tests executable
  add_executable(${TESTS_TARGET} EXCLUDE_FROM_ALL
    ${TESTS_DIR}/main.cpp
    ${TESTS_DIR}/lexical_analyzer_tests.cpp
    ${SOURCE_DIR}/lexical_analyzer.cpp
    ${SOURCE_DIR}/syntax.cpp
    ${SOURCE_DIR}/syntax_analyzer.cpp)
  target_include_directories(${TESTS_TARGET}
    PRIVATE ${SOURCE_DIR}
    PRIVATE ${TESTS_DIR}
    PRIVATE ${GTEST_INCLUDE_DIRS})
  target_link_libraries(${TESTS_TARGET}
    ${GTEST_BOTH_LIBRARIES}
    pthread)

  # Run tests executable
  add_custom_target(runtests
    COMMAND ${TESTS_TARGET}
    DEPENDS ${TESTS_TARGET}
    WORKING_DIRECTORY ${BUILD_DIR}
    COMMENT "Running ${CMAKE_PROJECT_NAME} unit tests...")

endif()
