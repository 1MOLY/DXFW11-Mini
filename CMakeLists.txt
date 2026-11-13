# =================================================================================================

###    #   #  ####   #   #   #      #                          #   #    #             #
#  #    # #   #      #   #  ##     ##                          ## ##
#  #     #    ###    # # #   #      #                          # # #   ##    ###     ##
#  #    # #   #      # # #   #      #            #####         # # #    #    #  #     #
#  #    # #   #      ## ##   #      #                          #   #    #    #  #     #
###    #   #  #      #   #  ###    ###                         #   #   ###   #  #    ###

cmake_minimum_required(VERSION 4.3)
project(DXFW11_Mini VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

# **********************| SETTINGS |**********************

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(DXFW_BUILD_TESTS "Build DXFW11-Mini tests" OFF)

# **********************| DEPENDENCIES |**********************

find_package(imgui CONFIG REQUIRED)

# **********************| Targets |**********************

add_library(DXFW11_Mini STATIC
		src/dxfw11Mini.cpp
		src/core/_util.cpp
		src/core/_tex.cpp
)

add_library(DXFW11_Mini::DXFW11_Mini
		ALIAS DXFW11_Mini
)

# **********************| Target - Options |**********************

target_compile_features(DXFW11_Mini PUBLIC
		cxx_std_23
)

target_compile_definitions(DXFW11_Mini PRIVATE
		WIN32_LEAN_AND_MEAN
		$<$<CONFIG:Debug>:DXFW_TRACING>
)

target_include_directories(DXFW11_Mini PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_precompile_headers(DXFW11_Mini PRIVATE
		include/com/pch.hpp
)

target_link_libraries(DXFW11_Mini PUBLIC
		imgui::imgui
		d3d11
		dxgi
		dwmapi
		d3dcompiler
)

# **********************| INSTALLATION |**********************

install(TARGETS DXFW11_Mini
		EXPORT DXFW11_MiniTargets
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

configure_package_config_file(
		cmake/DXFW11_MiniConfig.cmake.in
		"${CMAKE_CURRENT_BINARY_DIR}/DXFW11_MiniConfig.cmake"
		INSTALL_DESTINATION share/DXFW11_Mini
)

write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/DXFW11_MiniConfigVersion.cmake"
		VERSION ${PROJECT_VERSION}
		COMPATIBILITY SameMajorVersion
)

install(FILES
		"${CMAKE_CURRENT_BINARY_DIR}/DXFW11_MiniConfig.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/DXFW11_MiniConfigVersion.cmake"
		DESTINATION share/DXFW11_Mini
)

install(EXPORT DXFW11_MiniTargets
		FILE DXFW11_MiniTargets.cmake
		NAMESPACE DXFW11_Mini::
		DESTINATION share/DXFW11_Mini
)

# **********************| TESTING |**********************

if(DXFW_BUILD_TESTS)
	add_executable(Tests_
			tests_/main.cpp
	)

	target_link_libraries(Tests_ PRIVATE
			DXFW11_Mini
	)
endif()