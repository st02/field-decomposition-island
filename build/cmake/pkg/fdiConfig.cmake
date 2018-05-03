get_filename_component(_dir "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(_prefix "${_dir}/../../.." ABSOLUTE)

include("${_prefix}/share/cmake/targets/fdi-targets.cmake")

# exported variables
set(FIELD-DECOMPOSITION-ISLAND_MODE "")


message(STATUS "fdi: ${_prefix}")
