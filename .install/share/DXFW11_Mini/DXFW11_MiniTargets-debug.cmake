#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "DXFW11_Mini::DXFW11_Mini" for configuration "Debug"
set_property(TARGET DXFW11_Mini::DXFW11_Mini APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(DXFW11_Mini::DXFW11_Mini PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/DXFW11_Mini.lib"
  )

list(APPEND _cmake_import_check_targets DXFW11_Mini::DXFW11_Mini )
list(APPEND _cmake_import_check_files_for_DXFW11_Mini::DXFW11_Mini "${_IMPORT_PREFIX}/lib/DXFW11_Mini.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
