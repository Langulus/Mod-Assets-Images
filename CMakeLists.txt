cmake_minimum_required(VERSION 3.28)
project(LangulusModAssetsImages
    VERSION         1.0.0
    DESCRIPTION     "Langulus plug-in module for generating and managing image assets"
    HOMEPAGE_URL    https://langulus.com
)

# Configure ZLIB library                                                        
set(BUILD_SHARED_LIBS   OFF CACHE BOOL "" FORCE)
set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

fetch_external_module(
    zlib
    GIT_REPOSITORY  https://github.com/madler/zlib.git
    GIT_TAG         51b7f2abdade71cd9bb0e7a373ef2610ec6f9daf
)
add_library(ZLIB::ZLIB ALIAS zlibstatic)
set(ZLIB_ROOT "${zlib_SOURCE_DIR}" CACHE INTERNAL "Root for the zlib library, used by libpng")
set(ZLIB_LIBRARY zlibstatic CACHE INTERNAL "" FORCE)
set(ZLIB_INCLUDE_DIR "${zlib_SOURCE_DIR};${zlib_BINARY_DIR}" CACHE INTERNAL "" FORCE)

# Configure PNG library                                                         
set(PNG_SHARED		 OFF CACHE BOOL "" FORCE)
set(PNG_TOOLS		 OFF CACHE BOOL "" FORCE)
set(PNG_TESTS		 OFF CACHE BOOL "" FORCE)
set(SKIP_INSTALL_ALL ON  CACHE BOOL "" FORCE)

fetch_external_module(
    png
    GIT_REPOSITORY   https://github.com/glennrp/libpng.git
    GIT_TAG          1bf304c43caa94e491e4659362b1252ec50abb3e
)

file(GLOB_RECURSE
	LANGULUS_MOD_ASSETS_IMAGES_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	source/*.cpp
)

# Build the module                                                              
add_langulus_mod(LangulusModAssetsImages ${LANGULUS_MOD_ASSETS_IMAGES_SOURCES})

target_link_libraries(LangulusModAssetsImages PRIVATE png_static)

if(LANGULUS_TESTING)
	enable_testing()
    add_subdirectory(test)
endif()