cmake_minimum_required(VERSION 3.24)
project(LangulusModAssetsImages
    VERSION         1.0.0
    DESCRIPTION     "Langulus plug-in module for generating and managing image assets"
    HOMEPAGE_URL    https://langulus.com
)

# Configure ZLIB library														
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ZLIB_USE_STATIC_LIBS ON CACHE BOOL "" FORCE)
set(ZLIB_BUILD_TESTS OFF CACHE BOOL "" FORCE)

fetch_external_module(
    zlib
    GIT_REPOSITORY  https://github.com/madler/zlib.git
    GIT_TAG         v1.3
)
include_directories(${zlib_SOURCE_DIR})
include_directories(${zlib_BINARY_DIR})

# Configure PNG library                                                         
set(PNG_SHARED OFF CACHE BOOL "" FORCE)
set(PNG_TOOLS OFF CACHE BOOL "" FORCE)
set(PNG_TESTS OFF CACHE BOOL "" FORCE)
set(PNG_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(ZLIB_INCLUDE_DIRS "${zlib_SOURCE_DIR};${zlib_BINARY_DIR}" CACHE STRING "" FORCE)
set(ZLIB_LIBRARIES $<TARGET_FILE:zlibstatic> CACHE STRING "" FORCE)

fetch_external_module(
    png
    GIT_REPOSITORY  https://github.com/glennrp/libpng.git
    GIT_TAG         libpng16
)

# Build the module                                                              
add_library(LangulusModAssetsImages SHARED
    source/ImageLibrary.cpp
    source/Image.cpp
    source/Font.cpp
    source/PNG.cpp
)

target_include_directories(LangulusModAssetsImages
    PRIVATE     ${png_SOURCE_DIR}
                ${png_BINARY_DIR}
)

target_link_libraries(LangulusModAssetsImages 
    PRIVATE     Langulus
                png_static
)

if(LANGULUS_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()