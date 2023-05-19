cmake_minimum_required(VERSION 3.24)
project(LangulusModAssetsImages)

# Build the module                                                          
add_library(LangulusModAssetsImages SHARED
    source/ImageLibrary.cpp
    source/Image.cpp
    source/Font.cpp
    source/PNG.cpp
)

target_link_libraries(LangulusModAssetsImages 
    PRIVATE     Langulus
)

if(LANGULUS_ENABLE_TESTING)
    enable_testing()
	add_subdirectory(test)
endif()