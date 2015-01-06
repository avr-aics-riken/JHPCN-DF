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
# -CMAKE_BUILD_TYPE=DEBUG
#    デバッグ版をビルドする(最適化オプション無し、計時機能off、デバッグ出力on)
#
# -Dbuild_unit_tests=yes
#    UnitTestをビルドする
#
# -Dbuild_examples=yes
#    サンプルコードをビルドする
#
# -Dbuild_performance_test=yes
#    PerformanceTestおよびDumpToolをビルドする
#
# -DREAL_8_BYTE=yes
#    PerformanceTest、DumpTool、サンプルコードをdouble用にビルドする(デフォルトはfloat)
#


cmake_minimum_required(VERSION 2.6)
project(JHPCNDFlib CXX Fortran)

set(CMAKE_MODULE_PATH  ${PROJECT_SOURCE_DIR}/cmake)
include(${PROJECT_SOURCE_DIR}/cmake/CompileOptionSelector.cmake)
if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    ADD_DEFINITIONS(-DDEBUG)
else()
    CompileOptionSelector()
    ADD_DEFINITIONS(-DTIME_MEASURE)
endif()

##############################################
# 依存するパッケージを探す
##############################################

# check OpenMP support and add option
find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

option(build_examples         "build examples" OFF)
option(build_unit_tests       "build test program" OFF)
option(build_performance_test "build performance test program" OFF)

#ビルド設定の表示
message( STATUS "Destination PATH: " ${CMAKE_INSTALL_PREFIX})
message( STATUS "build examples: " build_examples)
message( STATUS "build unit test program: " build_unit_tests)
message( STATUS "build performance test program: " build_performance_test)
message( STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER})
message( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})
message( STATUS "CMAKE_Fortran_COMPILER: " ${CMAKE_Fortran_COMPILER})
message( STATUS "CMAKE_Fortran_FLAGS: " ${CMAKE_Fortran_FLAGS})


#####################################################
#      コンパイル対象ファイルの指定
#####################################################
set(LIB_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(LIB_SRC
    ${LIB_SRC_DIR}/Interface.cpp
    ${LIB_SRC_DIR}/FInterface.f90
    ${LIB_SRC_DIR}/jhpcndf.f90
    )

#####################################################
# include PATHの設定(ライブラリ用)
#####################################################
include_directories( 
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${ZLIB_INCLUDE_DIRS}
    )

#####################################################
# ライブラリ本体のビルド&インストール
#####################################################
add_library(jhpcndf STATIC ${LIB_SRC})
install (TARGETS jhpcndf DESTINATION lib)
install (FILES ${PROJECT_SOURCE_DIR}/include/jhpcndf.h  DESTINATION include)
install (FILES ${PROJECT_BINARY_DIR}/jhpcndf.mod        DESTINATION include)
install (FILES ${PROJECT_SOURCE_DIR}/cmake/FindJHPCNDFlib.cmake   DESTINATION share)

#####################################################
# サンプルのビルド
#   cmakeの実行時に -Dbuild_examples=yes が
#   指定された時のみbuildする
#####################################################
if(build_examples)
    add_subdirectory(${PROJECT_SOURCE_DIR}/examples)
endif()

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

