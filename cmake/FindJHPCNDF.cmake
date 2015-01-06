###################################################################################
#
# JHPCN-DF : Data compression library based on
#            Jointed Hierarchical Precision Compression Number Data Format
#
# Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
#
###################################################################################

# - Try to find JHPCNDF
# Once done, this will define
#
#  JHPCNDF_FOUND - system has JHPCNDF
#  JHPCNDF_INCLUDE_DIRS - the JHPCNDF include directories
#  JHPCNDF_LIBRARIES - link these to use JHPCNDF

include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(JHPCNDF_PKGCONF JHPCNDF)

# Include dir
find_path(JHPCNDF_INCLUDE_DIR
  NAMES jhpcndf.h
  PATHS ${TP_ROOT} ${JHPCNDF_PKGCONF_INCLUDE_DIRS}
  PATH_SUFFIXES include
)

# Finally the library itself
find_library(JHPCNDF_LIBRARY
  NAMES jhpcndf
  PATHS ${TP_ROOT} ${JHPCNDF_PKGCONF_LIBRARY_DIRS}
  PATH_SUFFIXES lib 
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(JHPCNDF_PROCESS_INCLUDES JHPCNDF_INCLUDE_DIR)
set(JHPCNDF_PROCESS_LIBS JHPCNDF_LIBRARY)
libfind_process(JHPCNDF)

