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
##   add options for agressive optimization and detailed report
## 

macro (CompileOptionSelector)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "-g -march=native -O3 -std=c++11")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
        set(CMAKE_CXX_FLAGS "-g -xHost -O3 -opt-report 2 -vec-report5 -std=c++11")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "PGI")
        set(CMAKE_CXX_FLAGS "-gopt -Minfo -fastsse")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")  #TODO check SSE option for MSVC
        message("MSVC is not well supported for now")
        message("using default compiler option")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang") #TODO check SSE option for Clang
        message("Clang is not well supported for now")
        message("using default compiler option")
    else()
        message("assume that you are using Fujitsu Compiler on K or FX10")
        set(CMAKE_CXX_COMPILER "FCCpx")   
        set(CMAKE_CXX_FLAGS "-g -O3 -Kfast -Kocl -Koptmsg=2 -Nsrc -Nsta -Krestp=arg")
    endif()
endmacro()

macro (FreeForm)
    if(CMAKE_Fortran_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -ffree-form")
    elseif(CMAKE_Fortran_COMPILER_ID STREQUAL "Intel")
        set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -free")
    elseif(CMAKE_Fortran_COMPILER_ID STREQUAL "PGI")
        set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -Mfree")
        #TODO    elseif(CMAKE_FortranCOMPILER STREQUAL "FCCpx")
        #TODO     set(CMAKE_Fortran_FLAGS "")
    endif()
endmacro()

macro(C99)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
endmacro()
