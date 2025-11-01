cmake_minimum_required(VERSION 3.20)
project(SpMV VERSION 0.1.0 LANGUAGES CXX)

# =========================
# Basic config
# =========================
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed. "
                      "Create a separate build dir and delete CMakeCache.txt.")
endif()

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, defaulting to Debug")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(SPMV_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD ${SPMV_CXX_STANDARD} CACHE STRING "Default C++ standard")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(SPMV_MIN_GCC_VERSION 8.0)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${SPMV_MIN_GCC_VERSION})
    message(FATAL_ERROR "GCC version must be at least ${SPMV_MIN_GCC_VERSION}")
  endif()
else()
  message(WARNING "Unsupported compiler! Prefer GCC >= ${SPMV_MIN_GCC_VERSION}")
endif()

# =========================
# Warnings
# =========================
include(CheckCXXCompilerFlag)
macro(spmv_add_cxx_compiler_flag FLAG)
  string(REGEX REPLACE "-" "" SFLAG ${FLAG})
  check_cxx_compiler_flag(${FLAG} COMPILER_SUPPORT_${SFLAG})
  if(COMPILER_SUPPORT_${SFLAG})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAG}")
  endif()
endmacro()

spmv_add_cxx_compiler_flag("-Wall")
spmv_add_cxx_compiler_flag("-Wextra")
spmv_add_cxx_compiler_flag("-Wcast-align")
spmv_add_cxx_compiler_flag("-Wcast-qual")
spmv_add_cxx_compiler_flag("-Wconversion")
spmv_add_cxx_compiler_flag("-Wdouble-promotion")
spmv_add_cxx_compiler_flag("-Wfloat-equal")
spmv_add_cxx_compiler_flag("-Wimplicit-fallthrough")
spmv_add_cxx_compiler_flag("-Wmissing-noreturn")
spmv_add_cxx_compiler_flag("-Wnon-virtual-dtor")
spmv_add_cxx_compiler_flag("-Wshadow")
spmv_add_cxx_compiler_flag("-Wunused")
spmv_add_cxx_compiler_flag("-Wvla")
spmv_add_cxx_compiler_flag("-Wzero-as-null-pointer-constant")
spmv_add_cxx_compiler_flag("-Wunreachable-code")
spmv_add_cxx_compiler_flag("-Wmissing-noreturn")
spmv_add_cxx_compiler_flag("-Wtautological-compare")
spmv_add_cxx_compiler_flag("-Wundef")
spmv_add_cxx_compiler_flag("-Werror")
spmv_add_cxx_compiler_flag("-Wold-style-cast")
spmv_add_cxx_compiler_flag("-pedantic")

# =========================
# Library
# =========================
set(SPMV_SOURCES
  "src/SparseMatrix.cpp"
  "src/SparseMatrix_COO.cpp"
  "src/SparseMatrix_CSR.cpp"
  "src/SparseMatrix_ELL.cpp"
  "src/SparseMatrix_JDS.cpp"
)

add_library(spmv SHARED ${SPMV_SOURCES})

target_include_directories(spmv
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/spmv>
    $<INSTALL_INTERFACE:include>
)

# =========================
# Tests
# =========================
include(CTest)
enable_testing()
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
  add_subdirectory(tests)
endif()

# =========================
# Examples
# =========================
option(SPMV_BUILD_EXAMPLES "Build examples" OFF)
if (SPMV_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
