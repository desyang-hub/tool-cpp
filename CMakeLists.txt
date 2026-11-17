cmake_minimum_required(VERSION 3.10)

project(toolcpp)

set(CMAKE_CXX_STANDARD 11)

# find_package(OpenCV REQUIRED)
# include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

set(cpp_sources
    src/toolcpp/Thread.cpp
    src/toolcpp/ThreadPool.cpp
)

add_executable(${PROJECT_NAME} ${cpp_sources} main.cpp)
# target_link_libraries(${PROJECT_NAME})