cmake_minimum_required(VERSION 3.15)
project(MiniHome LANGUAGES CXX VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MINIHOME_QT_DIR "D:/Qt/6.8.2/msvc2022_64")
set(MINIHOME_VTK_DIR "D:/VTK-9.4.1/install/lib/cmake/vtk-9.4")
set(MINIHOME_OCCT_DEBUG_DIR "D:/OCCT-7_9_0_beta2/install/cmake")
set(MINIHOME_OCCT_RELEASE_DIR "D:/OCCT-7_9_0_beta2/install/cmake")

add_compile_options(/EHsc)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS /utf-8)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/${CMAKE_BUILD_TYPE}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/${CMAKE_BUILD_TYPE}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/${CMAKE_BUILD_TYPE}/lib)
endif()

add_subdirectory(third/fmt)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/third
    ${CMAKE_CURRENT_SOURCE_DIR}/third/boost
)

add_subdirectory(MHCore)

add_subdirectory(MHWindow)
add_dependencies(MHWindow MHCore)

add_subdirectory(MHApp)
add_dependencies(MiniHome MHCore MHWindow)