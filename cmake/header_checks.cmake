# Compile headers separately without the test harness's includes or linked implementations.
set(CTLE_CORE_HEADER_CHECK_SOURCES)
set(CTLE_ADAPTER_HEADER_CHECK_SOURCES)
set(CTLE_EXTERNAL_ADAPTER_HEADERS ntup_glm.h status_vulkan.h)

foreach(header IN LISTS CTLE_FILE_SET)
	get_filename_component(extension "${header}" LAST_EXT)
	if(NOT extension STREQUAL ".h")
		continue()
	endif()

	get_filename_component(header_name "${header}" NAME)
	get_filename_component(header_stem "${header}" NAME_WE)
	set(declaration_source "${CMAKE_CURRENT_BINARY_DIR}/header_checks/${header_stem}.cpp")
	set(implementation_source "${CMAKE_CURRENT_BINARY_DIR}/header_checks/${header_stem}_implementation.cpp")

	file(GENERATE OUTPUT "${declaration_source}"
		CONTENT "#include <ctle/${header_name}>\n")
	file(GENERATE OUTPUT "${implementation_source}"
		CONTENT "#define CTLE_IMPLEMENTATION\n#include <ctle/${header_name}>\n")

	if(header_name IN_LIST CTLE_EXTERNAL_ADAPTER_HEADERS)
		list(APPEND CTLE_ADAPTER_HEADER_CHECK_SOURCES "${declaration_source}" "${implementation_source}")
	else()
		list(APPEND CTLE_CORE_HEADER_CHECK_SOURCES "${declaration_source}" "${implementation_source}")
	endif()
endforeach()

add_custom_target(ctle_header_checks)
add_custom_target(ctle_adapter_header_checks)

# Visual Studio archives OBJECT targets. Keep each translation unit in its own target:
# combining implementation checks would put duplicate CTLE_IMPLEMENTATION symbols in one archive.
foreach(source IN LISTS CTLE_CORE_HEADER_CHECK_SOURCES CTLE_ADAPTER_HEADER_CHECK_SOURCES)
	get_filename_component(source_stem "${source}" NAME_WE)
	set(target "ctle_header_check_${source_stem}")
	add_library(${target} OBJECT "${source}")
	target_link_libraries(${target} PRIVATE ${PROJECT_NAME})
	# Per-header checks use C++17 or newer; the C++14 all-header application is separate.
	target_compile_features(${target} PRIVATE cxx_std_17)
	set_target_properties(${target} PROPERTIES UNITY_BUILD OFF DISABLE_PRECOMPILE_HEADERS ON)

	if(source IN_LIST CTLE_ADAPTER_HEADER_CHECK_SOURCES)
		# Only the explicit adapters receive SDK include paths; core headers must not require them.
		target_include_directories(${target} PRIVATE
			"${glm_SOURCE_DIR}"
			"${vulkan_headers_SOURCE_DIR}/include"
			)
		add_dependencies(ctle_adapter_header_checks ${target})
	else()
		add_dependencies(ctle_header_checks ${target})
	endif()
endforeach()

add_dependencies(ctle_header_checks ctle_adapter_header_checks)
add_dependencies(unit_tests ctle_header_checks)
