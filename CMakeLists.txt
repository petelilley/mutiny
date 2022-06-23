cmake_minimum_required(VERSION 3.1)

set(MT_VERSION_MAJOR 0)
set(MT_VERSION_MINOR 1)
set(MT_VERSION_PATCH 0)
set(MT_VERSION "${MT_VERSION_MAJOR}.${MT_VERSION_MINOR}.${MT_VERSION_PATCH}")
message(STATUS "Mutiny compiler version ${MT_VERSION}")

project(mutiny VERSION ${MT_VERSION})

# --- Project config ---

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release") # Default to release
endif()

# Turn on warnings.
if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -pedantic)
  # add_compile_options(-Weverything)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # compile_commands.json
  set(MT_DEBUG ON)
endif()

# --- Sources ---

set(MT_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(MT_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

function(add_mutiny_sources)
  set(MT_SRC ${MT_SRC} ${ARGV} PARENT_SCOPE)
endfunction()

# Retrieve sources.
include("${MT_LIB_DIR}/CMakeLists.txt")

# --- Operating system ---

if(NOT UNIX)
  message(FATAL_ERROR "Non-unix derived operating systems not supported") # TODO Add support?
endif()

string(TOLOWER ${CMAKE_SYSTEM_NAME} MT_OS)

message(STATUS "Target operating system ${MT_OS}")

# --- Architecture ---

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
  set(MT_ARCH "x86_64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86|i686")
  set(MT_ARCH "i686")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|ARM64|arm64")
  set(MT_ARCH "arm64")
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "armv6l")
  set(MT_ARCH "armv6")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "armv7l|armv7-a")
  set(MT_ARCH "armv7")
else()
  string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} MT_ARCH)
endif()

message(STATUS "Target architecture ${MT_ARCH}")

# --- Build executable ---

add_executable(${PROJECT_NAME} ${MT_SRC})

# --- Include dirs ---

target_include_directories(${PROJECT_NAME} PRIVATE ${MT_INC_DIR})

# --- Definitions ---

# Debug.
if(MT_DEBUG)
  target_compile_definitions(${PROJECT_NAME} PRIVATE "MT_DEBUG")
endif()

# --- Install ---

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME})

# --- Configuration file ---

configure_file(
  "${MT_INC_DIR}/mutiny/config/config.hpp.in"
  "${MT_INC_DIR}/mutiny/config/config.hpp"
)