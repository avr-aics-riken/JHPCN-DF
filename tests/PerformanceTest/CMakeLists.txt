cmake_minimum_required(VERSION 2.8)
project(JHPCNDFlib_PerformanceTest)

# add optimize option
include(${PROJECT_SOURCE_DIR}/../../cmake/CompileOptionSelector.cmake)
if(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    CompileOptionSelector()
endif()
if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    ADD_DEFINITIONS(-DDEBUG)
endif()

option(REAL_8_BYTE "build test for double" OFF)
if(REAL_8_BYTE)
    add_definitions(-DREAL_TYPE=double)
endif()

# zlib
find_package (ZLIB REQUIRED)
# check OpenMP support and add option
find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

# includeファイルの指定
include_directories(
    ${PROJECT_SOURCE_DIR}/../../include
    ${PROJECT_SOURCE_DIR}/../../src
    ${ZLIB_INCLUDE_DIRS}
    )

# ビルド対象の指定
add_executable(PerformanceTest PerformanceTest.cpp)
add_executable(ValidationTool ValidationTool.cpp)
 
#リンクするライブラリの指定
target_link_libraries(PerformanceTest jhpcndf ${ZLIB_LIBRARIES})
target_link_libraries(ValidationTool jhpcndf ${ZLIB_LIBRARIES})
