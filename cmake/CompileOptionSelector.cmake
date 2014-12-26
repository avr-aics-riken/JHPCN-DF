##
## Compile option selector
## 
##   add options for agressive optimization and detailed report
## 

macro (CompileOptionSelector)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "-g -march=native -O3")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
        set(CMAKE_CXX_FLAGS "-g -xHost -O3 -opt-report 2 -vec-report5")
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
