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
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# Make the write and read data dir for PhysFS, because it doesn't have access   
add_custom_command(
    TARGET LangulusModAssetsImagesTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:LangulusModAssetsImagesTest>/data/assets/images"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/pattern.png" "$<TARGET_FILE_DIR:LangulusModAssetsImagesTest>/data/assets/images/pattern.png"
)