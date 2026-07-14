cmake_minimum_required(VERSION 3.10)
project(A_star_planner LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Eigen3
find_package(Eigen3 REQUIRED)
if(NOT Eigen3_FOUND)
    message(FATAL_ERROR "Eigen3 not found, please install it (e.g., sudo apt install libeigen3-dev)")
endif()

# 头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include ${EIGEN3_INCLUDE_DIR})

# 可执行文件（将三个源文件编译为一个可执行程序）
add_executable(a_star_demo
    src/main.cpp
    src/a_star.cpp
    src/obstacle_map.cpp
)

# 如果希望将地图类做成库，再链接（可选）
# add_library(obstacle_map src/obstacle_map.cpp)
# target_link_libraries(a_star_demo obstacle_map)

# 链接 Eigen（纯头文件库，无需链接，但建议加上以防扩展）
target_link_libraries(a_star_demo Eigen3::Eigen)