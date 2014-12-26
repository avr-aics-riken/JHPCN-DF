###################################################################################
#
# JHPCN-DFlib (Jointed Hierarchical Precision Compression Number - Data Format)
#
###################################################################################

cmake_minimum_required(VERSION 2.6)
project(JHPCNDFlib CXX)

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

option(build_samples "build sample program for JHPCNDFlib" OFF)
option(build_unit_tests "build test program for JHPCNDFlib" ON)
option(build_performance_test "build performance test program for JHPCNDFlib" ON)

#ビルド設定の表示
message( STATUS "Destination PATH: " ${CMAKE_INSTALL_PREFIX})
message( STATUS "build sample program: " build_samples)
message( STATUS "build unit test program: " build_unit_tests)
message( STATUS "build performance test program: " build_performance_test)
message( STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER})
message( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})


##############################################
#      コンパイル対象ファイルの指定
##############################################
# Library
set(LIB_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(LIB_SRC
    ${LIB_SRC_DIR}/InterFace.cpp
    )

##############################################
# include PATHの設定(ライブラリ用)
##############################################
include_directories( 
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${ZLIB_INCLUDE_DIRS}
    )

##############################################
# ライブラリ本体のビルド&インストール
##############################################
add_library(jhpcndf STATIC ${LIB_SRC})
install (TARGETS jhpcndf DESTINATION lib)
install (FILES ${PROJECT_SOURCE_DIR}/include/jhpcndf.h     DESTINATION include)
#install (FILES ${PROJECT_SOURCE_DIR}/cmake/FindJHPCNDFlib.cmake   DESTINATION share)

##############################################
# サンプルのビルド
#   cmakeの実行時に -Dbuild_samples=yes が指定された時のみbuildする
##############################################
if(build_samples)
    add_subdirectory(${PROJECT_SOURCE_DIR}/samples)
endif()

##############################################
# 単体テストプログラムのビルド&インストール
#   cmakeの実行時に -Dbuild_unit_tests=yes が指定された時のみbuildする
##############################################
if(build_unit_tests)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests/UnitTest)
endif()

##############################################
# 性能テストプログラムのビルド&インストール
#   cmakeの実行時に -Dbuild_performance_tests=yes が指定された時のみbuildする
##############################################
if(build_performance_test)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests/PerformanceTest)
endif()

