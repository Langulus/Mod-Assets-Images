cmake_minimum_required(VERSION 3.28)
project(LangulusModAssetsImages
    VERSION         1.0.0
    DESCRIPTION     "Langulus plug-in module for generating and managing image assets"
    HOMEPAGE_URL    https://langulus.com
)

# Configure ZLIB library														
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ZLIB_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

fetch_external_module(
    zlib
    GIT_REPOSITORY  https://github.com/Epixu/zlib.git
    GIT_TAG         master
)
add_library(ZLIB::ZLIB ALIAS zlibstatic)
target_include_directories(zlibstatic INTERFACE ${zlib_BINARY_DIR} ${zlib_SOURCE_DIR})
set(ZLIB_FOUND ON CACHE BOOL "" FORCE)
set(ZLIB_LIBRARY ZLIB::ZLIB CACHE BOOL "" FORCE)
set(ZLIB_LIBRARIES ZLIB::ZLIB CACHE BOOL "" FORCE)
set(ZLIB_INCLUDE_DIR ${zlib_SOURCE_DIR} ${zlib_BINARY_DIR} CACHE BOOL "" FORCE)
set(ZLIB_INCLUDE_DIRS ${zlib_SOURCE_DIR} ${zlib_BINARY_DIR} CACHE BOOL "" FORCE)

# Configure PNG library                                                         
set(PNG_SHARED OFF CACHE BOOL "" FORCE)
set(PNG_TOOLS OFF CACHE BOOL "" FORCE)
set(PNG_TESTS OFF CACHE BOOL "" FORCE)
set(PNG_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE)

fetch_external_module(
    png
    GIT_REPOSITORY  https://github.com/glennrp/libpng.git
    GIT_TAG         libpng16
)

file(GLOB_RECURSE
	LANGULUS_MOD_ASSETS_IMAGES_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	source/*.cpp
)

# Build the module                                                              
add_library(LangulusModAssetsImages SHARED ${LANGULUS_MOD_ASSETS_IMAGES_SOURCES})

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