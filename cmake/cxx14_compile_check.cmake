# This application checks compilation and linking only; it is never run as a test.
set(cxx14_source [=[
#if defined(_MSVC_LANG)
static_assert(_MSVC_LANG == 201402L, "This application must compile as C++14.");
#else
static_assert(__cplusplus == 201402L, "This application must compile as C++14.");
#endif

#define CTLE_IMPLEMENTATION
#include <glm/glm.hpp>
#include <picosha2.h>
#include <xxhash.h>
]=])

foreach(header IN LISTS CTLE_FILE_SET)
	get_filename_component(extension "${header}" LAST_EXT)
	if(extension STREQUAL ".h")
		get_filename_component(header_name "${header}" NAME)
		string(APPEND cxx14_source "#include <ctle/${header_name}>\n")
	endif()
endforeach()
string(APPEND cxx14_source "\nint main()\n{\n\treturn 0;\n}\n")

set(cxx14_source_path "${CMAKE_CURRENT_BINARY_DIR}/cxx14_compile_check.cpp")
file(GENERATE OUTPUT "${cxx14_source_path}" CONTENT "${cxx14_source}")

add_executable(ctle_cxx14_compile_check EXCLUDE_FROM_ALL
	"${cxx14_source_path}"
	"${xxhash_SOURCE_DIR}/xxhash.c"
	)
# Do not link GoogleTest: its C++17 requirement would upgrade this target's language mode.
find_package(Threads REQUIRED)
target_link_libraries(ctle_cxx14_compile_check PRIVATE ${PROJECT_NAME} Threads::Threads)
target_include_directories(ctle_cxx14_compile_check SYSTEM PRIVATE
	"${glm_SOURCE_DIR}"
	"${picosha2_SOURCE_DIR}"
	"${xxhash_SOURCE_DIR}"
	"${vulkan_headers_SOURCE_DIR}/include"
	)
set_target_properties(ctle_cxx14_compile_check PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	UNITY_BUILD OFF
	DISABLE_PRECOMPILE_HEADERS ON
	)

if(MSVC)
	target_compile_options(ctle_cxx14_compile_check PRIVATE
		$<$<COMPILE_LANGUAGE:CXX>:/permissive->
		$<$<COMPILE_LANGUAGE:CXX>:/WX>
		)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(ctle_cxx14_compile_check PRIVATE
		$<$<COMPILE_LANGUAGE:CXX>:-pedantic-errors>)
endif()

if(WIN32)
	target_link_libraries(ctle_cxx14_compile_check PRIVATE ws2_32)
endif()
