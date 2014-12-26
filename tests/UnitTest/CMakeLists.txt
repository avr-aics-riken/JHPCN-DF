cmake_minimum_required(VERSION 2.6)
project(JHPCNDFlib_UnitTest CXX)

# add optimize option
include(${PROJECT_SOURCE_DIR}/../../cmake/CompileOptionSelector.cmake)
if(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    CompileOptionSelector()
endif()
if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    ADD_DEFINITIONS(-DDEBUG)
endif()

# check OpenMP support and add option
find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")


# GoogleTest libraryのBUILD
add_subdirectory(${PROJECT_SOURCE_DIR}/gtest-1.7.0)
include_directories(${PROJECT_SOURCE_DIR}/gtest-1.7.0/include)

# includeファイルの指定
include_directories(${PROJECT_SOURCE_DIR}/../../src)

# ビルド対象の指定
add_executable(UnitTest
    ${PROJECT_SOURCE_DIR}/gtest-1.7.0/src/gtest_main.cc
    ${PROJECT_SOURCE_DIR}/src/TestXOR.cpp
    ${PROJECT_SOURCE_DIR}/src/TestAND.cpp
    ${PROJECT_SOURCE_DIR}/src/TestZeroPadding.cpp
    ${PROJECT_SOURCE_DIR}/src/TestFileInfoManager.cpp
    )

#リンクするライブラリの指定
target_link_libraries(UnitTest gtest)
