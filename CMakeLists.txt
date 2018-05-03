cmake_minimum_required(VERSION "3.5")

# field decomposition island ;-)
#

project("fdi" VERSION "0.1.0" LANGUAGES CXX)

# set include search path
set(fdi_module_paths
  "${PROJECT_SOURCE_DIR}/cmake/include"
  "${PROJECT_SOURCE_DIR}/cmake/find"
)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${fdi_module_paths}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON )


SET(custom_common_flags "-Wno-unknown-pragmas -O0 -g ")
SET(custom_c_flags "${custom_common_flags}")
SET(custom_cxx_flags "${custom_common_flags} -std=c++11")
SET(CMAKE_C_FLAGS   "${custom_c_flags}")
SET(CMAKE_CXX_FLAGS "${custom_cxx_flags}")
if(${USE_MPI})
  SET(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_MPI")
endif()


# ------------------------------------------------
add_subdirectory(src)
add_subdirectory(tests)

# Generate and install the package configuration files
include("field-decomposition-island_setup_package")


