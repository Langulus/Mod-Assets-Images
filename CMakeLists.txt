cmake_minimum_required(VERSION 3.24)
project(LangulusModAssetsImages
    VERSION         1.0.0
    DESCRIPTION     "Langulus plug-in module for generating and managing image assets"
    HOMEPAGE_URL    https://langulus.com
)

# Configure ZLIB library                                                        
set(ZLIB_USE_STATIC_LIBS ON)
set(ZLIB_BUILD_TESTS OFF)

fetch_external_module(
    zlib
    GIT_REPOSITORY  https://github.com/madler/zlib.git
    GIT_TAG         04f42ce # master branch
    FIND_PACKAGE_ARGS NAMES ZLIB
)
include_directories(${zlib_SOURCE_DIR})
include_directories(${zlib_BINARY_DIR})

# Configure PNG library                                                         
option(PNG_SHARED OFF)
option(PNG_EXECUTABLES OFF)
option(PNG_TESTS OFF)

fetch_external_module(
    png
    GIT_REPOSITORY  https://github.com/glennrp/libpng.git
    GIT_TAG         e519af8 # libpng16 branch
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
                zlibstatic
                png_static
)

if(LANGULUS_TESTING)
    enable_testing()
	add_subdirectory(test)
endif()