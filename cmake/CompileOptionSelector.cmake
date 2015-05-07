###################################################################################
#
# JHPCN-DF : Data compression library based on
#            Jointed Hierarchical Precision Compression Number Data Format
#
# Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
#
###################################################################################

##
## Compile option selector
## 
macro (AddOptimizeOption)
  if(CMAKE_CXX_COMPILER MATCHES ".*FCCpx$")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Kfast -Kocl -Krestp=arg")
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "PGI")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fastsse")
  else()
    message("using default option")
  endif()
endmacro()

macro (AddSSE)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse3")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    endif()
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -xHost")
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "PGI")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fastsse")
  endif()
endmacro()

macro (FreeForm)
  if(CMAKE_Fortran_COMPILER MATCHES ".*frtpx$")
    set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -Free")
  elseif(CMAKE_Fortran_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -ffree-form")
  elseif(CMAKE_Fortran_COMPILER_ID STREQUAL "Intel")
    set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -free")
  elseif(CMAKE_Fortran_COMPILER_ID STREQUAL "PGI")
    set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -Mfree")
  endif()
endmacro()

macro(C99)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
endmacro()

macro(CPP11)
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
  if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  endif()
endmacro()

macro(FindZlib_quick_hack_fx10)
  set(ZLIB_INCLUDE_DIRS "/opt/FJSVXosDevkit/sparc64fx/target/include")
  set(ZLIB_LIBRARIES    "/opt/FJSVXosDevkit/sparc64fx/target/lib64/libz.so.1")
endmacro()

macro(FindOpenMP_quick_hack_fx10)
  set(OpenMP_CXX_FLAGS     "-Kopenmp")
  set(OpenMP_C_FLAGS       "-Kopenmp")
  set(OpenMP_Fortran_FLAGS "-Kopenmp")
endmacro()
