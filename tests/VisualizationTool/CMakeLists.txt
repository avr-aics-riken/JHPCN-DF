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

# zlib
find_package (ZLIB REQUIRED)
# check OpenMP support and add option
find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#VTK
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

# includeファイルの指定
include_directories(
    ${PROJECT_SOURCE_DIR}/../../include
    ${PROJECT_SOURCE_DIR}/../../src
    ${ZLIB_INCLUDE_DIRS}
    )

# ビルド対象の指定
add_executable(VisualizationTool VisualizationTool.cpp)
 
#リンクするライブラリの指定
if(VTK_LIBRARIES)
    target_link_libraries(VisualizationTool ${VTK_LIBRARIES})
else()
    target_link_libraries(VisualizationTool vtkHybrid vtkWidgets)
endif()
target_link_libraries(VisualizationTool jhpcndf ${ZLIB_LIBRARIES})
