project(LangulusModAssetsImagesTest)

add_executable(LangulusModAssetsImagesTest
	Main.cpp
	TestImages.cpp
)

target_link_libraries(LangulusModAssetsImagesTest
	PRIVATE		Langulus
				Catch2
)

add_test(
	NAME		LangulusModAssetsImagesTest
	COMMAND		LangulusModAssetsImagesTest
)

langulus_copy_dlls(LangulusModAssetsImagesTest ON LangulusModAssetsImagesTest)
langulus_copy_dlls_advanced(LangulusModAssetsImagesTest
	TO			LangulusModAssetsImagesTest
	FROM		LangulusModGLFW
				LangulusModVulkan
				LangulusModAssetsImages
				LangulusModFileSystem
)


# Make the write and read data dir for PhysFS, because it doesn't have access   
add_custom_command(
    TARGET LangulusModAssetsImagesTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:LangulusModAssetsImagesTest>/data/cache"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:LangulusModAssetsImagesTest>/data/assets/images"
)