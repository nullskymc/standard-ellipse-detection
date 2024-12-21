# python_bindings/CMakeLists.txt

cmake_minimum_required(VERSION 3.4...3.18)
project(detect_module)

# 设置 pybind11 的路径
set(CMAKE_PREFIX_PATH "/home/nullskymc/.virtualenvs/genTXT/lib/python3.10/site-packages/pybind11/share/cmake/pybind11" ${CMAKE_PREFIX_PATH})
find_package(pybind11 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(LAPACK REQUIRED)
find_package(BLAS REQUIRED)

# 添加全局包含目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

# 使用相对路径指定源文件并添加库
add_library(detect_module MODULE bindings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/detect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/compute.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/lsd.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/cvcannyapi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/unitily.cpp
)

# 设置目标属性
set_target_properties(detect_module PROPERTIES PREFIX "" SUFFIX ".so")

# 链接库，包括 LAPACK 和 BLAS
target_link_libraries(detect_module PRIVATE 
    pybind11::module 
    ${OpenCV_LIBS} 
    ${LAPACK_LIBRARIES}
    ${BLAS_LIBRARIES}
    ${OTHER_LIBS}
)

# 打印调试信息
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Adding detect_module with sources:")
message(STATUS "  bindings.cpp")
message(STATUS "  ${CMAKE_CURRENT_SOURCE_DIR}/../src/detect.cpp")
message(STATUS "  ${CMAKE_CURRENT_SOURCE_DIR}/../src/compute.cpp")
message(STATUS "  ${CMAKE_CURRENT_SOURCE_DIR}/../src/lsd.cpp")
message(STATUS "  ${CMAKE_CURRENT_SOURCE_DIR}/../src/cvcannyapi.cpp")
message(STATUS "  ${CMAKE_CURRENT_SOURCE_DIR}/../src/unitly.cpp")