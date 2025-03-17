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


# START QT OPTIONS
list(APPEND CMAKE_PREFIX_PATH ${MINIHOME_QT_DIR})
set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/resources/ui)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core 
    Gui
    Widgets
    OpenGLWidgets
)
qt_standard_project_setup()
# END QT OPTIONS

# START VTK OPTIONS
set(VTK_DIR ${MINIHOME_VTK_DIR})
find_package(VTK REQUIRED COMPONENTS
    CommonColor    
    CommonCore
    CommonDataModel
    FiltersSources
    GUISupportQt
    InteractionStyle
    RenderingAnnotation
    RenderingContextOpenGL2
    RenderingCore
    RenderingFreeType
    RenderingGL2PSOpenGL2
    RenderingOpenGL2
    RenderingQt
)
# END VTK OPTIONS

# START OpenCASCADE OPTIONS
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(OpenCASCADE_DIR ${MINIHOME_OCCT_DEBUG_DIR})
else()
    set(OpenCASCADE_DIR ${MINIHOME_OCCT_RELEASE_DIR})
endif()
find_package(OpenCASCADE REQUIRED)
# END OpenCASCADE OPTIONS

add_subdirectory(third/fmt)

file(GLOB_RECURSE SRCS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADER_SRCS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE UI_SRCS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/resources/ui/*.ui")

qt_add_executable(
    ${PROJECT_NAME}
    ${HEADER_SRCS}
    ${SRCS}
    ${UI_SRCS}
)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/ui
    ${PROJECT_SOURCE_DIR}/include/interactor
    ${PROJECT_SOURCE_DIR}/include/draw
    ${PROJECT_SOURCE_DIR}/include/topology
    ${PROJECT_SOURCE_DIR}/include/core
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    ${VTK_LIBRARIES}
    ${OpenCASCADE_LIBRARIES}
    fmt::fmt
    Qt6::Core 
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(${PROJECT_NAME})
vtk_module_autoinit(
  TARGETS ${PROJECT_NAME}
  MODULES ${VTK_LIBRARIES}
)