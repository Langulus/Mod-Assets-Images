file(GLOB_RECURSE
	LANGULUS_MOD_ASSETS_IMAGES_TEST_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	*.cpp *.hpp
)

add_executable(LangulusModAssetsImagesTest ${LANGULUS_MOD_ASSETS_IMAGES_TEST_SOURCES})

target_link_libraries(LangulusModAssetsImagesTest
	PRIVATE		Langulus
				Catch2
)

add_dependencies(LangulusModAssetsImagesTest
	LangulusModAssetsImages
	LangulusModFileSystem
)

add_test(
	NAME		LangulusModAssetsImagesTest
	COMMAND		LangulusModAssetsImagesTest
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# Make the write and read data dir for PhysFS, because it doesn't have access   
add_custom_command(
    TARGET LangulusModAssetsImagesTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
		"$<TARGET_FILE_DIR:LangulusModAssetsImagesTest>/data/assets/images"
    COMMAND ${CMAKE_COMMAND} -E copy
		"${CMAKE_CURRENT_SOURCE_DIR}/pattern.png"
		"$<TARGET_FILE_DIR:LangulusModAssetsImagesTest>/data/assets/images/pattern.png"
)