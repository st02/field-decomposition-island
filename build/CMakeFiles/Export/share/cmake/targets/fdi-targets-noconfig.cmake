#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "fdi::fdi" for configuration ""
set_property(TARGET fdi::fdi APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(fdi::fdi PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libfdi.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS fdi::fdi )
list(APPEND _IMPORT_CHECK_FILES_FOR_fdi::fdi "${_IMPORT_PREFIX}/lib/libfdi.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
