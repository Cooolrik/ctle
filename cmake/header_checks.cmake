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

add_library(ctle_header_checks OBJECT ${CTLE_CORE_HEADER_CHECK_SOURCES})
add_library(ctle_adapter_header_checks OBJECT ${CTLE_ADAPTER_HEADER_CHECK_SOURCES})

foreach(target IN ITEMS ctle_header_checks ctle_adapter_header_checks)
	target_link_libraries(${target} PRIVATE ${PROJECT_NAME})
	# Match the existing test framework's minimum; these checks do not establish C++14 support.
	target_compile_features(${target} PRIVATE cxx_std_17)
	set_target_properties(${target} PROPERTIES UNITY_BUILD OFF DISABLE_PRECOMPILE_HEADERS ON)
endforeach()

# Only the explicit adapters receive SDK include paths; core headers must not require them.
target_include_directories(ctle_adapter_header_checks PRIVATE
	"${glm_SOURCE_DIR}"
	"${vulkan_headers_SOURCE_DIR}/include"
	)

add_dependencies(ctle_header_checks ctle_adapter_header_checks)
add_dependencies(unit_tests ctle_header_checks)
