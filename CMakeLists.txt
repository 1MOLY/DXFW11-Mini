cmake_minimum_required(VERSION 4.3)
project(DXFW11_Mini)

set(CMAKE_CXX_STANDARD 23)

# -----------------------------------------------------------------------------

include_directories(include)

add_library(DXFW11_Mini STATIC
		src/dxfw11Mini.cpp
		include/dxfw11Mini.hpp
		src/core/_util.cpp
		include/core/_util.hpp
		src/core/_tex.cpp
		include/core/_tex.hpp
		include/com/pch.hpp
)

# -----------------------------------------------------------------------------

target_precompile_headers(DXFW11_Mini PUBLIC
		include/com/pch.hpp
)

target_include_directories(DXFW11_Mini PUBLIC

)

target_link_libraries(DXFW11_Mini PUBLIC
	d3d11 dxgi dwmapi d3dcompiler
)

# -----------------------------------------------------------------------------

# RUNTIME_OUTPUT_DIRECTORY_xxx = exe, dll ...
# ARCHIVE_OUTPUT_DIRECTORY_xxx = lib ...

set_target_properties(DXFW11_Mini PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/.lib"
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/.lib"
)



# -+-+-+-+-+-+-+- Testing -+-+-+-+-+-+-+-

add_executable(Tests_
		tests_/main.cpp
)

add_dependencies(Tests_ DXFW11_Mini)

target_link_libraries(Tests_ PRIVATE
	DXFW11_Mini
)

set_target_properties(DXFW11_Mini PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/.lib"
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/.lib"
)