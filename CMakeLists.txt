cmake_minimum_required(VERSION 3.10)

project(toolcpp)

set(CMAKE_CXX_STANDARD 11)

# find_package(OpenCV REQUIRED)
# include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} main.cpp)
# target_link_libraries(${PROJECT_NAME})