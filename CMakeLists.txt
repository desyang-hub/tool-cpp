cmake_minimum_required(VERSION 3.10)
project(toolcpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============ 源文件 ============
set(cpp_sources
    src/toolcpp/Thread.cpp
    src/toolcpp/ThreadPool.cpp
)

set(LIBRARY_NAME toolcpp)

# ============ 库 ============
add_library(${LIBRARY_NAME} ${cpp_sources})

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 线程库（std::thread 需要）
find_package(Threads REQUIRED)
target_link_libraries(${LIBRARY_NAME} PRIVATE Threads::Threads)

# ============ 安装 + 导出 ============
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS ${LIBRARY_NAME}
    EXPORT toolcppTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 头文件
install(DIRECTORY include/toolcpp
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# header
install(FILES include/toolcpp.h
    DESTINATION include
)

# 导出 targets
install(EXPORT toolcppTargets
    FILE toolcppTargets.cmake
    NAMESPACE toolcpp::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/toolcpp
)

# 版本文件
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/toolcppConfigVersion.cmake"
    VERSION 1.0.0
    COMPATIBILITY SameMajorVersion
)

# Config 文件
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/toolcppConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/toolcpp
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/toolcppConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/toolcppConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/toolcpp
)

# ============ main ============
add_executable(main main.cpp)
target_link_libraries(main PRIVATE ${LIBRARY_NAME})



# 告诉 CMake 去哪找
list(APPEND CMAKE_PREFIX_PATH "./build/toolcpp_install")
find_package(toolcpp REQUIRED)

add_executable(test_app test_app.cpp)
target_link_libraries(test_app PRIVATE toolcpp::toolcpp)