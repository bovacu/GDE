cmake_minimum_required(VERSION 3.18.1)
project(Engine)
if(APPLE)
    include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists/apple/CMakeLists.txt)
elseif(WIN32)
    include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists/windows/CMakeLists.txt)
else()
    include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists/linux/CMakeLists.txt)
endif()