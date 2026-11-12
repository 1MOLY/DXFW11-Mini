# =================================================================================================

	###    #   #  ####   #   #   #      #                          #   #    #             #
	#  #    # #   #      #   #  ##     ##                          ## ##
	#  #     #    ###    # # #   #      #                          # # #   ##    ###     ##
	#  #    # #   #      # # #   #      #            #####         # # #    #    #  #     #
	#  #    # #   #      ## ##   #      #                          #   #    #    #  #     #
	###    #   #  #      #   #  ###    ###                         #   #   ###   #  #    ###

# =================================================================================================

cmake_minimum_required(VERSION 4.3)
project(DXFW11_Mini)

set(CMAKE_CXX_STANDARD 23)

# **********************| INCLUDES |**********************

# - - - - - Project - - - - -

include_directories(include)

# - - - - - FetchContent - - - - -

include(FetchContent)

FetchContent_Declare(
		imgui
		GIT_REPOSITORY https://github.com/ocornut/imgui.git
		GIT_TAG        v1.93.0
)

FetchContent_MakeAvailable(imgui)

# **********************| Targets - Dependencies |**********************

add_library(imgui STATIC

		${imgui_SOURCE_DIR}/imgui.h
		${imgui_SOURCE_DIR}/imconfig.h
		${imgui_SOURCE_DIR}/imgui_internal.h

		${imgui_SOURCE_DIR}/imgui.cpp
		${imgui_SOURCE_DIR}/imgui_demo.cpp
		${imgui_SOURCE_DIR}/imgui_draw.cpp
		${imgui_SOURCE_DIR}/imgui_tables.cpp
		${imgui_SOURCE_DIR}/imgui_widgets.cpp

		${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
		${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
)

add_library(imgui::imgui ALIAS imgui)

target_include_directories(imgui PUBLIC
		${imgui_SOURCE_DIR}
		${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC
	d3d11
)

# **********************| Targets |**********************

add_library(DXFW11_Mini STATIC

		include/dxfw11Mini.hpp
		include/core/_util.hpp
		include/core/_tex.hpp
		include/com/pch.hpp

		src/dxfw11Mini.cpp
		src/core/_util.cpp
		src/core/_tex.cpp
)

# **********************| Target - Options |**********************

target_compile_definitions(DXFW11_Mini PRIVATE
		WIN32_LEAN_AND_MEAN
		DXFW_TRACING # For Debugging
)

target_precompile_headers(DXFW11_Mini PUBLIC
		include/com/pch.hpp
)

target_link_libraries(DXFW11_Mini PUBLIC
		d3d11
		dxgi
		dwmapi
		d3dcompiler
		imgui::imgui
)

# **********************| Target - Properties |**********************

# RUNTIME_OUTPUT_DIRECTORY_xxx = exe, dll ...
# ARCHIVE_OUTPUT_DIRECTORY_xxx = lib ...

set_target_properties(DXFW11_Mini PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/.lib"
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/.lib"
)

# =================================================================================================

# **********************| Testing |**********************

add_executable(Tests_
		tests_/main.cpp
)

target_link_libraries(Tests_ PRIVATE
		DXFW11_Mini
)

target_compile_definitions(Tests_ PRIVATE
		DXFW_TRACING # For Debugging
)

set_target_properties(Tests_ PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/.lib/tests"
		RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/.lib/tests"
)

# =================================================================================================
