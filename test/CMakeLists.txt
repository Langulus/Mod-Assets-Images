file(GLOB_RECURSE
	LANGULUS_MOD_ASSETS_IMAGES_TEST_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	*.cpp
)

add_langulus_test(LangulusModAssetsImagesTest
	SOURCES			${LANGULUS_MOD_ASSETS_IMAGES_TEST_SOURCES}
	LIBRARIES		Langulus
	DEPENDENCIES    LangulusModAssetsImages
					LangulusModFileSystem
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