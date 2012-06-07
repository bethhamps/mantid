# Generated by CMake 2.8.8

IF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.5)
   MESSAGE(FATAL_ERROR "CMake >= 2.6.0 required")
ENDIF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.5)
CMAKE_POLICY(PUSH)
CMAKE_POLICY(VERSION 2.6)
#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# MESSAGE (STATUS "CMAKE_LIBRARY_PATH: ${CMAKE_LIBRARY_PATH}")

# Commands may need to know the format version.
SET(CMAKE_IMPORT_FILE_VERSION 1)

# Create imported target hdf5
ADD_LIBRARY(hdf5 SHARED IMPORTED)

# Create imported target hdf5_cpp
ADD_LIBRARY(hdf5_cpp SHARED IMPORTED)

# Create imported target hdf5_tools
ADD_LIBRARY(hdf5_tools SHARED IMPORTED)

# Create imported target hdf5_hl
ADD_LIBRARY(hdf5_hl SHARED IMPORTED)

# Create imported target hdf5_hl_cpp
ADD_LIBRARY(hdf5_hl_cpp SHARED IMPORTED)

# Import target "hdf5" for configuration ""
SET_PROPERTY(TARGET hdf5 APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
SET_TARGET_PROPERTIES(hdf5 PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "m"
  IMPORTED_LOCATION_NOCONFIG "${CMAKE_LIBRARY_PATH}/libhdf5.1.8.9.dylib"
  IMPORTED_SONAME_NOCONFIG "libhdf5.7.3.0.dylib"
  )

# Import target "hdf5_cpp" for configuration ""
SET_PROPERTY(TARGET hdf5_cpp APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
SET_TARGET_PROPERTIES(hdf5_cpp PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "hdf5"
  IMPORTED_LOCATION_NOCONFIG "${CMAKE_LIBRARY_PATH}/libhdf5_cpp.1.8.9.dylib"
  IMPORTED_SONAME_NOCONFIG "libhdf5_cpp.7.3.0.dylib"
  )

# Import target "hdf5_tools" for configuration ""
SET_PROPERTY(TARGET hdf5_tools APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
SET_TARGET_PROPERTIES(hdf5_tools PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "hdf5"
  IMPORTED_LOCATION_NOCONFIG "${CMAKE_LIBRARY_PATH}/libhdf5_tools.1.8.9.dylib"
  IMPORTED_SONAME_NOCONFIG "libhdf5_tools.7.3.0.dylib"
  )

# Import target "hdf5_hl" for configuration ""
SET_PROPERTY(TARGET hdf5_hl APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
SET_TARGET_PROPERTIES(hdf5_hl PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "hdf5"
  IMPORTED_LOCATION_NOCONFIG "${CMAKE_LIBRARY_PATH}/libhdf5_hl.1.8.9.dylib"
  IMPORTED_SONAME_NOCONFIG "libhdf5_hl.7.3.0.dylib"
  )

# Import target "hdf5_hl_cpp" for configuration ""
SET_PROPERTY(TARGET hdf5_hl_cpp APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
SET_TARGET_PROPERTIES(hdf5_hl_cpp PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "hdf5_hl;hdf5"
  IMPORTED_LOCATION_NOCONFIG "${CMAKE_LIBRARY_PATH}/libhdf5_hl_cpp.1.8.9.dylib"
  IMPORTED_SONAME_NOCONFIG "libhdf5_hl_cpp.7.3.0.dylib"
  )

# Commands beyond this point should not need to know the version.
SET(CMAKE_IMPORT_FILE_VERSION)
CMAKE_POLICY(POP)
