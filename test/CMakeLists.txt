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