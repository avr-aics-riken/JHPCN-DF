###################################################################################
#
# JHPCN-DF : Data compression library based on
#            Jointed Hierarchical Precision Compression Number Data Format
#
# Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
#
###################################################################################

#
# cmake実行時に指定できるオプション
#
# -DCMAKE_INSTALL_PREFIX=<prefix>
#    インストール先のPATHをprefix以下へ変更する(デフォルトは/usr/local)
#
# -DCMAKE_PREFIX_PATH=<path>
#    依存するライブラリを探すパスを指定する(デフォルトは無指定)
#
# -DCMAKE_BUILD_TYPE=DEBUG
#    デバッグ版をビルドする(最適化オプション無し、計時機能off、デバッグ出力on)
#
# -Dbuild_unit_tests={yes|no}
#    UnitTestをビルドする(デフォルトno)
#
# -Dbuild_performance_test={yes|no}
#    PerformanceTestおよびDumpToolをビルドする(デフォルトno)
#
# -Dwith_sse={yes|no}
#    SSE命令を生成するオプションを指定する。(デフォルトyes)
#
# -Dwith_OpenMP={yes|no}
#    OpenMPによる並列化を行う (デフォルトyes)
#
# -Dwith_Fortran_interface={yes|no}
#    Fortran用インタフェースルーチンをビルドする (デフォルトyes)

cmake_minimum_required(VERSION 2.8.10)

#ビルドオプション設定
option(with_Fortran_interface   "enable Fortran interface" ON)
option(build_unit_tests       "build test program" OFF)
option(build_performance_test "build performance test program" OFF)
option(with_sse               "use sse instructions" ON)
option(with_OpenMP            "enable OpenMP directives" ON)
option(use_lz4                "enable lz4" OFF)


if(with_Fortran_interface)
  project(JHPCNDFlib CXX Fortran)
else()
  project(JHPCNDFlib CXX)
endif()

set(CMAKE_MODULE_PATH  ${PROJECT_SOURCE_DIR}/cmake)
include(${PROJECT_SOURCE_DIR}/cmake/CompileOptionSelector.cmake)

#コンパイルオプションの指定
if(build_unit_tests)
CPP11()
endif()
if(with_Fortran_interface)
FreeForm()
endif()
AddOptimizeOption()
if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
  ADD_DEFINITIONS(-DDEBUG)
else()
  if(with_OpenMP)
    ADD_DEFINITIONS(-DTIME_MEASURE)
  endif()
endif()
if(with_sse)
  AddSSE()
endif()
if(with_OpenMP)
  ADD_DEFINITIONS(-DUSE_OPENMP)
endif()

##############################################
# 依存するパッケージを探す
##############################################
# check OpenMP support and add options
if(with_OpenMP)
  if(CMAKE_CXX_COMPILER MATCHES ".*FCCpx$")
    FindOpenMP_quick_hack_fx10()
  else()
    find_package(OpenMP REQUIRED)
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# zlib
if(CMAKE_CXX_COMPILER MATCHES ".*FCCpx$")
    FindZlib_quick_hack_fx10()
else()
    find_package (ZLIB REQUIRED)
endif()

#lz4
if(use_lz4)
  find_package(LZ4 REQUIRED)
  ADD_DEFINITIONS(-DUSE_LZ4)
endif()

#ビルド設定の表示
message( STATUS "Destination PATH: "               ${CMAKE_INSTALL_PREFIX})
message( STATUS "build unit test program: "        ${build_unit_tests})
message( STATUS "build performance test program: " ${build_performance_test})
message( STATUS "CMAKE_CXX_COMPILER: "             ${CMAKE_CXX_COMPILER})
message( STATUS "CMAKE_CXX_FLAGS: "                ${CMAKE_CXX_FLAGS})
if(with_Fortran_interface)
message( STATUS "CMAKE_Fortran_COMPILER: "         ${CMAKE_Fortran_COMPILER})
message( STATUS "CMAKE_Fortran_FLAGS: "            ${CMAKE_Fortran_FLAGS})
endif()


#####################################################
#      コンパイル対象ファイルの指定
#####################################################
set(LIB_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(LIB_SRC ${LIB_SRC_DIR}/Interface.cpp)
if(with_Fortran_interface)
  list(APPEND LIB_SRC ${LIB_SRC_DIR}/FInterface.f90)
  list(APPEND LIB_SRC ${LIB_SRC_DIR}/jhpcndf.f90)
endif()

#####################################################
# include PATHの設定(ライブラリ用)
#####################################################
include_directories( 
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${ZLIB_INCLUDE_DIRS}
    ${LZ4_INCLUDE_DIRS}
    )

#####################################################
# ライブラリ本体のビルド&インストール
#####################################################
add_library(JHPCNDF STATIC ${LIB_SRC})
install (TARGETS JHPCNDF DESTINATION lib)
install (FILES ${PROJECT_SOURCE_DIR}/include/jhpcndf.h  DESTINATION include)
if(with_Fortran_interface)
install (FILES ${PROJECT_BINARY_DIR}/jhpcndf.mod        DESTINATION include)
endif()
install (FILES ${PROJECT_SOURCE_DIR}/cmake/FindJHPCNDF.cmake   DESTINATION share)
install (FILES ${PROJECT_SOURCE_DIR}/cmake/FindLZ4.cmake       DESTINATION share)
install (FILES ${PROJECT_SOURCE_DIR}/cmake/LibFindMacros.cmake DESTINATION share)
install (FILES ${PROJECT_SOURCE_DIR}/cmake/Toolchain_K.cmake   DESTINATION share)

#####################################################
# 単体テストプログラムのビルド&インストール
#   cmakeの実行時に -Dbuild_unit_tests=yes が
#   指定された時のみbuildする
#####################################################
if(build_unit_tests)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests/UnitTest)
endif()

#####################################################
# 性能テストプログラムのビルド&インストール
#   cmakeの実行時に -Dbuild_performance_test=yes が
#   指定された時のみbuildする
#####################################################
if(build_performance_test)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests/PerformanceTest)
endif()

