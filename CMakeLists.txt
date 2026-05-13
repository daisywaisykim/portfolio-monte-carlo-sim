cmake_minimum_required(VERSION 3.10.0)
project(portfolio-monte-carlo-sim VERSION 0.1.0 LANGUAGES C CXX)

# Set C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set CMAKE_TOOLCHAIN_FILE to use the vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

# Set CMAKE_PREFIX_PATH to include the vcpkg installation directory
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET})

# Find dependencies (via vcpkg)
find_package(Eigen3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

# Define the executable
add_executable(${PROJECT_NAME} src/main.cpp src/core/simulator.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen fmt::fmt)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
