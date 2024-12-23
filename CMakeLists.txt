# python_bindings/CMakeLists.txt

cmake_minimum_required(VERSION 3.4...3.18)
project(detect_module)

# 设置 pybind11 的路径
set(CMAKE_PREFIX_PATH "/home/nullskymc/.virtualenvs/genTXT/lib/python3.10/site-packages/pybind11/share/cmake/pybind11" ${CMAKE_PREFIX_PATH})

# 添加 /usr/local/lib 到库搜索路径
link_directories(/usr/local/lib)

# 添加 /usr/local/include 到包含目录
include_directories(/usr/local/include ${CMAKE_CURRENT_SOURCE_DIR}/../include)

find_package(pybind11 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(LAPACK REQUIRED)
find_package(BLAS REQUIRED)

# 检查是否找到 LAPACK 和 BLAS
if (LAPACK_FOUND)
    message(STATUS "Found LAPACK: ${LAPACK_LIBRARIES}")
else()
    message(FATAL_ERROR "Could not find LAPACK")
endif()

if (BLAS_FOUND)
    message(STATUS "Found BLAS: ${BLAS_LIBRARIES}")
else()
    message(FATAL_ERROR "Could not find BLAS")
endif()

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
)

# 打印调试信息
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Adding detect_module with sources:")
message(STATUS "  bindings.cpp")
message(STATUS "  ${CMAKE_CURRENT_SOURCE_DIR}/../src/detect.cpp")
message(STATUS "  ${CMAKE_CURRENT_SOURCE_DIR}/../src/compute.cpp")
message(STATUS "  ${CMAKE_CURRENT_SOURCE_DIR}/../src/lsd.cpp")
message(STATUS "  ${CMAKE_CURRENT_SOURCE_DIR}/../src/cvcannyapi.cpp")
message(STATUS "  ${CMAKE_CURRENT_SOURCE_DIR}/../src/unitily.cpp")