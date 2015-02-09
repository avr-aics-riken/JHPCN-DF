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
macro (Mandatory)
    if(CMAKE_CXX_COMPILER MATCHES ".*FCCpx$")
        set(CMAKE_CXX_FLAGS                 "-Xg -g")
        set(CMAKE_CXX_FLAGS_DEBUG           "-Xg -g")
        set(CMAKE_CXX_FLAGS_RELEASE         "-Xg -g")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-Xg -g")
        set(CMAKE_CXX_FLAGS_MINSIZEREL      "-Xg -g")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "-g")
        set(CMAKE_CXX_FLAGS_DEBUG           "-g")
        set(CMAKE_CXX_FLAGS_RELEASE         "-g")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-g")
        set(CMAKE_CXX_FLAGS_MINSIZEREL      "-g")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
        set(CMAKE_CXX_FLAGS "-g")
        set(CMAKE_CXX_FLAGS_DEBUG           "-g")
        set(CMAKE_CXX_FLAGS_RELEASE         "-g")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-g")
        set(CMAKE_CXX_FLAGS_MINSIZEREL      "-g")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "PGI")
        set(CMAKE_CXX_FLAGS "-gopt")
        set(CMAKE_CXX_FLAGS_DEBUG           "-gopt")
        set(CMAKE_CXX_FLAGS_RELEASE         "-gopt")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-gopt")
        set(CMAKE_CXX_FLAGS_MINSIZEREL      "-gopt")
    endif()
endmacro()


macro (CompileOptionSelector)
    Mandatory()
    if(CMAKE_CXX_COMPILER MATCHES ".*FCCpx$")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Kfast -Kocl -Krestp=arg -Koptmsg=2 -Nsrc -Nsta")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -O3")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -xHost -O3 -opt-report 2 -vec-report5")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "PGI")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fastsse -Minfo")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")  #TODO check SSE option for MSVC
        message("MSVC is not well supported for now")
        message("using default compiler option")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang") #TODO check SSE option for Clang
        message("Clang is not well supported for now")
        message("using default compiler option")
    else()
        message("using default option")
    endif()
endmacro()

macro (FreeForm)
    if(CMAKE_Fortran_COMPILER MATCHES ".*frtpx$")
        set(CMAKE_Fortran_FLAGS                "${CMAKE_Fortran_FLAGS}                -Free")
        set(CMAKE_Fortran_FLAGS_DEBUG          "${CMAKE_Fortran_FLAGS_DEBUG}          -Free")
        set(CMAKE_Fortran_FLAGS_RELEASE        "${CMAKE_Fortran_FLAGS_RELEASE}        -Free")
        set(CMAKE_Fortran_FLAGS_RELWITHDEBINFO "${CMAKE_Fortran_FLAGS_RELWITHDEBINFO} -Free")
        set(CMAKE_Fortran_FLAGS_MINSIZEREL     "${CMAKE_Fortran_FLAGS_MINSIZEREL}     -Free")
    elseif(CMAKE_Fortran_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_Fortran_FLAGS                 "${CMAKE_Fortran_FLAGS}                 -ffree-form")
        set(CMAKE_Fortran_FLAGS_DEBUG           "${CMAKE_Fortran_FLAGS_DEBUG}           -ffree-form")
        set(CMAKE_Fortran_FLAGS_RELEASE         "${CMAKE_Fortran_FLAGS_RELEASE}         -ffree-form")
        set(CMAKE_Fortran_FLAGS_RELWITHDEBINFO  "${CMAKE_Fortran_FLAGS_RELWITHDEBINFO}  -ffree-form")
        set(CMAKE_Fortran_FLAGS_MINSIZEREL      "${CMAKE_Fortran_FLAGS_MINSIZEREL}      -ffree-form")
    elseif(CMAKE_Fortran_COMPILER_ID STREQUAL "Intel")
        set(CMAKE_Fortran_FLAGS                 "${CMAKE_Fortran_FLAGS}                 -free")
        set(CMAKE_Fortran_FLAGS_DEBUG           "${CMAKE_Fortran_FLAGS_DEBUG}           -free")
        set(CMAKE_Fortran_FLAGS_RELEASE         "${CMAKE_Fortran_FLAGS_RELEASE}         -free")
        set(CMAKE_Fortran_FLAGS_RELWITHDEBINFO  "${CMAKE_Fortran_FLAGS_RELWITHDEBINFO}  -free")
        set(CMAKE_Fortran_FLAGS_MINSIZEREL      "${CMAKE_Fortran_FLAGS_MINSIZEREL}      -free")
    elseif(CMAKE_Fortran_COMPILER_ID STREQUAL "PGI")
        set(CMAKE_Fortran_FLAGS                 "${CMAKE_Fortran_FLAGS}                 -Mfree")
        set(CMAKE_Fortran_FLAGS_DEBUG           "${CMAKE_Fortran_FLAGS_DEBUG}           -Mfree")
        set(CMAKE_Fortran_FLAGS_RELEASE         "${CMAKE_Fortran_FLAGS_RELEASE}         -Mfree")
        set(CMAKE_Fortran_FLAGS_RELWITHDEBINFO  "${CMAKE_Fortran_FLAGS_RELWITHDEBINFO}  -Mfree")
        set(CMAKE_Fortran_FLAGS_MINSIZEREL      "${CMAKE_Fortran_FLAGS_MINSIZEREL}      -Mfree")
    endif()
endmacro()

macro(C99)
    set(CMAKE_C_FLAGS                "${CMAKE_C_FLAGS}                -std=c99")
    set(CMAKE_C_FLAGS_DEBUG          "${CMAKE_C_FLAGS_DEBUG}          -std=c99")
    set(CMAKE_C_FLAGS_RELEASE        "${CMAKE_C_FLAGS_RELEASE}        -std=c99")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -std=c99")
    set(CMAKE_C_FLAGS_MINSIZEREL     "${CMAKE_C_FLAGS_MINSIZEREL}     -std=c99")
endmacro()

macro(CPP11)
    if(NOT CMAKE_CXX_COMPILER MATCHES ".*FCCpx$")
    set(CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS}                -std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG}          -std=c++11")
    set(CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE}        -std=c++11")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -std=c++11")
    set(CMAKE_CXX_FLAGS_MINSIZEREL     "${CMAKE_CXX_FLAGS_MINSIZEREL}     -std=c++11")
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
