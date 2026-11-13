# Development revision: 2

set(SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../..")

vcpkg_cmake_configure(
		SOURCE_PATH "${SOURCE_PATH}"
		OPTIONS
			-DDXFW_BUILD_TESTS=OFF
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(
		PACKAGE_NAME DXFW11_Mini
		CONFIG_PATH share/DXFW11_Mini
)

file(REMOVE_RECURSE
		"${CURRENT_PACKAGES_DIR}/debug/include"
		"${CURRENT_PACKAGES_DIR}/debug/share"
)

vcpkg_install_copyright(
		FILE_LIST "${SOURCE_PATH}/LICENCE.txt"
)