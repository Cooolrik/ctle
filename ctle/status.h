// ctle Copyright (c) 2022 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#pragma once

#include <unordered_map>

namespace ctle
	{
	enum class status_code : int
		{
		ok                                     = 0, // ok, no error, successful

		// general errors
		undefined_error                        = -100, // undefined error
		invalid_param                          = -101, // invalid parameter in method call
		not_initialized                        = -102, // the system is not initialized
		already_initialized                    = -103, // the system or class is already initialized or in a specific state
		cant_allocate                          = -104, // cant allocate memory
		cant_open                              = -105, // cant open file or handle
		cant_read                              = -106, // cant read from file or handle
		corrupted                              = -107, // a file, data or object is corrupted 
		invalid                                = -108, // invalid file, wrong file format, or failed validation
		cant_write                             = -109, // cant write to file or handle
		not_found                              = -110, // one or multiple objects are missing or not found
		
		// stl portable errors (from errc)
		stl_unrecognized_error_code            = -1000, // unknown/unrecognized STL error, which could not be mapped to a specific error value
		stl_address_family_not_supported       = -1001, // errc::address_family_not_supported
		stl_address_in_use                     = -1002, // errc::address_in_use
		stl_address_not_available              = -1003, // errc::address_not_available
		stl_already_connected                  = -1004, // errc::already_connected
		stl_argument_list_too_long             = -1005, // errc::argument_list_too_long
		stl_argument_out_of_domain             = -1006, // errc::argument_out_of_domain
		stl_bad_address                        = -1007, // errc::bad_address
		stl_bad_file_descriptor                = -1008, // errc::bad_file_descriptor
		stl_bad_message                        = -1009, // errc::bad_message
		stl_broken_pipe                        = -1010, // errc::broken_pipe
		stl_connection_aborted                 = -1011, // errc::connection_aborted
		stl_connection_already_in_progress     = -1012, // errc::connection_already_in_progress
		stl_connection_refused                 = -1013, // errc::connection_refused
		stl_connection_reset                   = -1014, // errc::connection_reset
		stl_cross_device_link                  = -1015, // errc::cross_device_link
		stl_destination_address_required       = -1016, // errc::destination_address_required
		stl_device_or_resource_busy            = -1017, // errc::device_or_resource_busy
		stl_directory_not_empty                = -1018, // errc::directory_not_empty
		stl_executable_format_error            = -1019, // errc::executable_format_error
		stl_file_exists                        = -1020, // errc::file_exists
		stl_file_too_large                     = -1021, // errc::file_too_large
		stl_filename_too_long                  = -1022, // errc::filename_too_long
		stl_function_not_supported             = -1023, // errc::function_not_supported
		stl_host_unreachable                   = -1024, // errc::host_unreachable
		stl_identifier_removed                 = -1025, // errc::identifier_removed
		stl_illegal_byte_sequence              = -1026, // errc::illegal_byte_sequence
		stl_inappropriate_io_control_operation = -1027, // errc::inappropriate_io_control_operation
		stl_interrupted                        = -1028, // errc::interrupted
		stl_invalid_argument                   = -1029, // errc::invalid_argument
		stl_invalid_seek                       = -1030, // errc::invalid_seek
		stl_io_error                           = -1031, // errc::io_error
		stl_is_a_directory                     = -1032, // errc::is_a_directory
		stl_message_size                       = -1033, // errc::message_size
		stl_network_down                       = -1034, // errc::network_down
		stl_network_reset                      = -1035, // errc::network_reset
		stl_network_unreachable                = -1036, // errc::network_unreachable
		stl_no_buffer_space                    = -1037, // errc::no_buffer_space
		stl_no_child_process                   = -1038, // errc::no_child_process
		stl_no_link                            = -1039, // errc::no_link
		stl_no_lock_available                  = -1040, // errc::no_lock_available
		stl_no_message_available               = -1041, // errc::no_message_available
		stl_no_message                         = -1042, // errc::no_message
		stl_no_protocol_option                 = -1043, // errc::no_protocol_option
		stl_no_space_on_device                 = -1044, // errc::no_space_on_device
		stl_no_stream_resources                = -1045, // errc::no_stream_resources
		stl_no_such_device_or_address          = -1046, // errc::no_such_device_or_address
		stl_no_such_device                     = -1047, // errc::no_such_device
		stl_no_such_file_or_directory          = -1048, // errc::no_such_file_or_directory
		stl_no_such_process                    = -1049, // errc::no_such_process
		stl_not_a_directory                    = -1050, // errc::not_a_directory
		stl_not_a_socket                       = -1051, // errc::not_a_socket
		stl_not_a_stream                       = -1052, // errc::not_a_stream
		stl_not_connected                      = -1053, // errc::not_connected
		stl_not_enough_memory                  = -1054, // errc::not_enough_memory
		stl_not_supported                      = -1055, // errc::not_supported
		stl_operation_canceled                 = -1056, // errc::operation_canceled
		stl_operation_in_progress              = -1057, // errc::operation_in_progress
		stl_operation_not_permitted            = -1058, // errc::operation_not_permitted
		stl_operation_not_supported            = -1059, // errc::operation_not_supported
		stl_operation_would_block              = -1060, // errc::operation_would_block
		stl_owner_dead                         = -1061, // errc::owner_dead
		stl_permission_denied                  = -1062, // errc::permission_denied
		stl_protocol_error                     = -1063, // errc::protocol_error
		stl_protocol_not_supported             = -1064, // errc::protocol_not_supported
		stl_read_only_file_system              = -1065, // errc::read_only_file_system
		stl_resource_deadlock_would_occur      = -1066, // errc::resource_deadlock_would_occur
		stl_resource_unavailable_try_again     = -1067, // errc::resource_unavailable_try_again
		stl_result_out_of_range                = -1068, // errc::result_out_of_range
		stl_state_not_recoverable              = -1069, // errc::state_not_recoverable
		stl_stream_timeout                     = -1070, // errc::stream_timeout
		stl_text_file_busy                     = -1071, // errc::text_file_busy
		stl_timed_out                          = -1072, // errc::timed_out
		stl_too_many_files_open_in_system      = -1073, // errc::too_many_files_open_in_system
		stl_too_many_files_open                = -1074, // errc::too_many_files_open
		stl_too_many_links                     = -1075, // errc::too_many_links
		stl_too_many_symbolic_link_levels      = -1076, // errc::too_many_symbolic_link_levels
		stl_value_too_large                    = -1077, // errc::value_too_large
		stl_wrong_protocol_type                = -1078, // errc::wrong_protocol_type

		// Vulkan errors
		vulkan_unrecognized_error_code                      = -2000, // unknown/unrecognized Vulkan error, which could not be mapped to a specific error value
		vulkan_out_of_host_memory                           = -2001, // Vulkan error code VkResult::VK_ERROR_OUT_OF_HOST_MEMORY
		vulkan_out_of_device_memory                         = -2002, // Vulkan error code VkResult::VK_ERROR_OUT_OF_DEVICE_MEMORY
		vulkan_initialization_failed                        = -2003, // Vulkan error code VkResult::VK_ERROR_INITIALIZATION_FAILED
		vulkan_device_lost                                  = -2004, // Vulkan error code VkResult::VK_ERROR_DEVICE_LOST
		vulkan_memory_map_failed                            = -2005, // Vulkan error code VkResult::VK_ERROR_MEMORY_MAP_FAILED
		vulkan_layer_not_present                            = -2006, // Vulkan error code VkResult::VK_ERROR_LAYER_NOT_PRESENT
		vulkan_extension_not_present                        = -2007, // Vulkan error code VkResult::VK_ERROR_EXTENSION_NOT_PRESENT
		vulkan_feature_not_present                          = -2008, // Vulkan error code VkResult::VK_ERROR_FEATURE_NOT_PRESENT
		vulkan_incompatible_driver                          = -2009, // Vulkan error code VkResult::VK_ERROR_INCOMPATIBLE_DRIVER
		vulkan_too_many_objects                             = -2010, // Vulkan error code VkResult::VK_ERROR_TOO_MANY_OBJECTS
		vulkan_format_not_supported                         = -2011, // Vulkan error code VkResult::VK_ERROR_FORMAT_NOT_SUPPORTED
		vulkan_fragmented_pool                              = -2012, // Vulkan error code VkResult::VK_ERROR_FRAGMENTED_POOL
		vulkan_unknown                                      = -2013, // Vulkan error code VkResult::VK_ERROR_UNKNOWN
		vulkan_out_of_pool_memory                           = -2014, // Vulkan error code VkResult::VK_ERROR_OUT_OF_POOL_MEMORY
		vulkan_invalid_external_handle                      = -2015, // Vulkan error code VkResult::VK_ERROR_INVALID_EXTERNAL_HANDLE
		vulkan_fragmentation                                = -2016, // Vulkan error code VkResult::VK_ERROR_FRAGMENTATION
		vulkan_invalid_opaque_capture_address               = -2017, // Vulkan error code VkResult::VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS
		vulkan_surface_lost_khr                             = -2018, // Vulkan error code VkResult::VK_ERROR_SURFACE_LOST_KHR
		vulkan_native_window_in_use_khr                     = -2019, // Vulkan error code VkResult::VK_ERROR_NATIVE_WINDOW_IN_USE_KHR
		vulkan_out_of_date_khr                              = -2020, // Vulkan error code VkResult::VK_ERROR_OUT_OF_DATE_KHR
		vulkan_incompatible_display_khr                     = -2021, // Vulkan error code VkResult::VK_ERROR_INCOMPATIBLE_DISPLAY_KHR
		vulkan_validation_failed_ext                        = -2022, // Vulkan error code VkResult::VK_ERROR_VALIDATION_FAILED_EXT
		vulkan_invalid_shader_nv                            = -2023, // Vulkan error code VkResult::VK_ERROR_INVALID_SHADER_NV
		vulkan_invalid_drm_format_modifier_plane_layout_ext = -2024, // Vulkan error code VkResult::VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT
		vulkan_not_permitted_khr                            = -2025, // Vulkan error code VkResult::VK_ERROR_NOT_PERMITTED_KHR
		vulkan_full_screen_exclusive_mode_lost_ext          = -2026, // Vulkan error code VkResult::VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT
		vulkan_compression_exhausted_ext                    = -2027, // Vulkan error code VkResult::VK_ERROR_COMPRESSION_EXHAUSTED_EXT
		};

	// status maps a number of error values into one error enum
	// and has ctors which converts these error values into a status_code value
	class status
		{
		private:
			status_code svalue = status_code::ok;

		public:
			status() = default;
			status( const status &other ) = default;

			status( const status_code &_value ) noexcept : svalue( _value ) {}
			const status &operator = ( const status_code &_value ) noexcept { this->svalue = _value; return *this; }

#ifdef _SYSTEM_ERROR_
			// convert from STL std::errc
			static status_code to_status_code( std::errc _value ) noexcept;
			status( const std::errc &_value ) noexcept : svalue( to_status_code(_value) ) {}
			const status &operator = ( const std::errc &_value ) noexcept { this->svalue = to_status_code(_value); return *this; }
#endif//_SYSTEM_ERROR_

#ifdef VULKAN_CORE_H_
			// convert from Vulkan error: VkResult
			static status_code to_status_code( VkResult _value ) noexcept;
			status( const VkResult &_value ) noexcept : svalue( to_status_code(_value) ) {}
			const status &operator = ( const VkResult &_value ) noexcept { this->svalue = to_status_code(_value); return *this; }
#endif//VULKAN_CORE_H_

			// use as a bool
			operator bool() const { return svalue == status_code::ok; }

			// get the status_code value 
			status_code value() const { return svalue; }

			// get the name of the status code as a string 
			std::string name() const;

			// get a description of the status code value
			std::string description() const;
		};

#ifdef CTLE_IMPLEMENTATION

	struct status_code_string_description
		{
		const char *name;
		const char *description;
		};

	static const std::unordered_map<status_code, status_code_string_description> status_code_string_descriptions = 
		{
			// ok, no error
			{ status_code::ok, { "ok","ok, no error, successful" } },

			// general errors
			{ status_code::undefined_error, { "undefined_error","undefined error" } },
			{ status_code::invalid_param, { "invalid_param","invalid parameter in method call" } },
			{ status_code::not_initialized, { "not_initialized","the system is not initialized" } },
			{ status_code::already_initialized, { "already_initialized","the system or class is already initialized or in a specific state" } },
			{ status_code::cant_allocate, { "cant_allocate","cant allocate memory" } },
			{ status_code::cant_open, { "cant_open","cant open file or handle" } },
			{ status_code::cant_read, { "cant_read","cant read from file or handle" } },
			{ status_code::corrupted, { "corrupted","a file, data or object is corrupted { " } },
			{ status_code::invalid, { "invalid","invalid file, wrong file format, or failed validation" } },
			{ status_code::cant_write, { "cant_write","cant write to file or handle" } },

			// stl portable errors (from errc)
			{ status_code::stl_unrecognized_error_code, { "stl_unrecognized_error_code","unknown/unrecognized STL error, which could not be mapped to a specific error value" } },
			{ status_code::stl_address_family_not_supported, { "stl_address_family_not_supported","errc::address_family_not_supported" } },
			{ status_code::stl_address_in_use, { "stl_address_in_use","errc::address_in_use" } },
			{ status_code::stl_address_not_available, { "stl_address_not_available","errc::address_not_available" } },
			{ status_code::stl_already_connected, { "stl_already_connected","errc::already_connected" } },
			{ status_code::stl_argument_list_too_long, { "stl_argument_list_too_long","errc::argument_list_too_long" } },
			{ status_code::stl_argument_out_of_domain, { "stl_argument_out_of_domain","errc::argument_out_of_domain" } },
			{ status_code::stl_bad_address, { "stl_bad_address","errc::bad_address" } },
			{ status_code::stl_bad_file_descriptor, { "stl_bad_file_descriptor","errc::bad_file_descriptor" } },
			{ status_code::stl_bad_message, { "stl_bad_message","errc::bad_message" } },
			{ status_code::stl_broken_pipe, { "stl_broken_pipe","errc::broken_pipe" } },
			{ status_code::stl_connection_aborted, { "stl_connection_aborted","errc::connection_aborted" } },
			{ status_code::stl_connection_already_in_progress, { "stl_connection_already_in_progress","errc::connection_already_in_progress" } },
			{ status_code::stl_connection_refused, { "stl_connection_refused","errc::connection_refused" } },
			{ status_code::stl_connection_reset, { "stl_connection_reset","errc::connection_reset" } },
			{ status_code::stl_cross_device_link, { "stl_cross_device_link","errc::cross_device_link" } },
			{ status_code::stl_destination_address_required, { "stl_destination_address_required","errc::destination_address_required" } },
			{ status_code::stl_device_or_resource_busy, { "stl_device_or_resource_busy","errc::device_or_resource_busy" } },
			{ status_code::stl_directory_not_empty, { "stl_directory_not_empty","errc::directory_not_empty" } },
			{ status_code::stl_executable_format_error, { "stl_executable_format_error","errc::executable_format_error" } },
			{ status_code::stl_file_exists, { "stl_file_exists","errc::file_exists" } },
			{ status_code::stl_file_too_large, { "stl_file_too_large","errc::file_too_large" } },
			{ status_code::stl_filename_too_long, { "stl_filename_too_long","errc::filename_too_long" } },
			{ status_code::stl_function_not_supported, { "stl_function_not_supported","errc::function_not_supported" } },
			{ status_code::stl_host_unreachable, { "stl_host_unreachable","errc::host_unreachable" } },
			{ status_code::stl_identifier_removed, { "stl_identifier_removed","errc::identifier_removed" } },
			{ status_code::stl_illegal_byte_sequence, { "stl_illegal_byte_sequence","errc::illegal_byte_sequence" } },
			{ status_code::stl_inappropriate_io_control_operation, { "stl_inappropriate_io_control_operation","errc::inappropriate_io_control_operation" } },
			{ status_code::stl_interrupted, { "stl_interrupted","errc::interrupted" } },
			{ status_code::stl_invalid_argument, { "stl_invalid_argument","errc::invalid_argument" } },
			{ status_code::stl_invalid_seek, { "stl_invalid_seek","errc::invalid_seek" } },
			{ status_code::stl_io_error, { "stl_io_error","errc::io_error" } },
			{ status_code::stl_is_a_directory, { "stl_is_a_directory","errc::is_a_directory" } },
			{ status_code::stl_message_size, { "stl_message_size","errc::message_size" } },
			{ status_code::stl_network_down, { "stl_network_down","errc::network_down" } },
			{ status_code::stl_network_reset, { "stl_network_reset","errc::network_reset" } },
			{ status_code::stl_network_unreachable, { "stl_network_unreachable","errc::network_unreachable" } },
			{ status_code::stl_no_buffer_space, { "stl_no_buffer_space","errc::no_buffer_space" } },
			{ status_code::stl_no_child_process, { "stl_no_child_process","errc::no_child_process" } },
			{ status_code::stl_no_link, { "stl_no_link","errc::no_link" } },
			{ status_code::stl_no_lock_available, { "stl_no_lock_available","errc::no_lock_available" } },
			{ status_code::stl_no_message_available, { "stl_no_message_available","errc::no_message_available" } },
			{ status_code::stl_no_message, { "stl_no_message","errc::no_message" } },
			{ status_code::stl_no_protocol_option, { "stl_no_protocol_option","errc::no_protocol_option" } },
			{ status_code::stl_no_space_on_device, { "stl_no_space_on_device","errc::no_space_on_device" } },
			{ status_code::stl_no_stream_resources, { "stl_no_stream_resources","errc::no_stream_resources" } },
			{ status_code::stl_no_such_device_or_address, { "stl_no_such_device_or_address","errc::no_such_device_or_address" } },
			{ status_code::stl_no_such_device, { "stl_no_such_device","errc::no_such_device" } },
			{ status_code::stl_no_such_file_or_directory, { "stl_no_such_file_or_directory","errc::no_such_file_or_directory" } },
			{ status_code::stl_no_such_process, { "stl_no_such_process","errc::no_such_process" } },
			{ status_code::stl_not_a_directory, { "stl_not_a_directory","errc::not_a_directory" } },
			{ status_code::stl_not_a_socket, { "stl_not_a_socket","errc::not_a_socket" } },
			{ status_code::stl_not_a_stream, { "stl_not_a_stream","errc::not_a_stream" } },
			{ status_code::stl_not_connected, { "stl_not_connected","errc::not_connected" } },
			{ status_code::stl_not_enough_memory, { "stl_not_enough_memory","errc::not_enough_memory" } },
			{ status_code::stl_not_supported, { "stl_not_supported","errc::not_supported" } },
			{ status_code::stl_operation_canceled, { "stl_operation_canceled","errc::operation_canceled" } },
			{ status_code::stl_operation_in_progress, { "stl_operation_in_progress","errc::operation_in_progress" } },
			{ status_code::stl_operation_not_permitted, { "stl_operation_not_permitted","errc::operation_not_permitted" } },
			{ status_code::stl_operation_not_supported, { "stl_operation_not_supported","errc::operation_not_supported" } },
			{ status_code::stl_operation_would_block, { "stl_operation_would_block","errc::operation_would_block" } },
			{ status_code::stl_owner_dead, { "stl_owner_dead","errc::owner_dead" } },
			{ status_code::stl_permission_denied, { "stl_permission_denied","errc::permission_denied" } },
			{ status_code::stl_protocol_error, { "stl_protocol_error","errc::protocol_error" } },
			{ status_code::stl_protocol_not_supported, { "stl_protocol_not_supported","errc::protocol_not_supported" } },
			{ status_code::stl_read_only_file_system, { "stl_read_only_file_system","errc::read_only_file_system" } },
			{ status_code::stl_resource_deadlock_would_occur, { "stl_resource_deadlock_would_occur","errc::resource_deadlock_would_occur" } },
			{ status_code::stl_resource_unavailable_try_again, { "stl_resource_unavailable_try_again","errc::resource_unavailable_try_again" } },
			{ status_code::stl_result_out_of_range, { "stl_result_out_of_range","errc::result_out_of_range" } },
			{ status_code::stl_state_not_recoverable, { "stl_state_not_recoverable","errc::state_not_recoverable" } },
			{ status_code::stl_stream_timeout, { "stl_stream_timeout","errc::stream_timeout" } },
			{ status_code::stl_text_file_busy, { "stl_text_file_busy","errc::text_file_busy" } },
			{ status_code::stl_timed_out, { "stl_timed_out","errc::timed_out" } },
			{ status_code::stl_too_many_files_open_in_system, { "stl_too_many_files_open_in_system","errc::too_many_files_open_in_system" } },
			{ status_code::stl_too_many_files_open, { "stl_too_many_files_open","errc::too_many_files_open" } },
			{ status_code::stl_too_many_links, { "stl_too_many_links","errc::too_many_links" } },
			{ status_code::stl_too_many_symbolic_link_levels, { "stl_too_many_symbolic_link_levels","errc::too_many_symbolic_link_levels" } },
			{ status_code::stl_value_too_large, { "stl_value_too_large","errc::value_too_large" } },
			{ status_code::stl_wrong_protocol_type, { "stl_wrong_protocol_type","errc::wrong_protocol_type" } },

			// Vulkan errors
			{ status_code::vulkan_unrecognized_error_code, { "vulkan_unrecognized_error_code","unknown/unrecognized Vulkan error, which could not be mapped to a specific error value" } },
			{ status_code::vulkan_out_of_host_memory, { "vulkan_out_of_host_memory","Vulkan error code VK_ERROR_OUT_OF_HOST_MEMORY" } },
			{ status_code::vulkan_out_of_device_memory, { "vulkan_out_of_device_memory","Vulkan error code VK_ERROR_OUT_OF_DEVICE_MEMORY" } },
			{ status_code::vulkan_initialization_failed, { "vulkan_initialization_failed","Vulkan error code VK_ERROR_INITIALIZATION_FAILED" } },
			{ status_code::vulkan_device_lost, { "vulkan_device_lost","Vulkan error code VK_ERROR_DEVICE_LOST" } },
			{ status_code::vulkan_memory_map_failed, { "vulkan_memory_map_failed","Vulkan error code VK_ERROR_MEMORY_MAP_FAILED" } },
			{ status_code::vulkan_layer_not_present, { "vulkan_layer_not_present","Vulkan error code VK_ERROR_LAYER_NOT_PRESENT" } },
			{ status_code::vulkan_extension_not_present, { "vulkan_extension_not_present","Vulkan error code VK_ERROR_EXTENSION_NOT_PRESENT" } },
			{ status_code::vulkan_feature_not_present, { "vulkan_feature_not_present","Vulkan error code VK_ERROR_FEATURE_NOT_PRESENT" } },
			{ status_code::vulkan_incompatible_driver, { "vulkan_incompatible_driver","Vulkan error code VK_ERROR_INCOMPATIBLE_DRIVER" } },
			{ status_code::vulkan_too_many_objects, { "vulkan_too_many_objects","Vulkan error code VK_ERROR_TOO_MANY_OBJECTS" } },
			{ status_code::vulkan_format_not_supported, { "vulkan_format_not_supported","Vulkan error code VK_ERROR_FORMAT_NOT_SUPPORTED" } },
			{ status_code::vulkan_fragmented_pool, { "vulkan_fragmented_pool","Vulkan error code VK_ERROR_FRAGMENTED_POOL" } },
			{ status_code::vulkan_unknown, { "vulkan_unknown","Vulkan error code VK_ERROR_UNKNOWN" } },
			{ status_code::vulkan_out_of_pool_memory, { "vulkan_out_of_pool_memory","Vulkan error code VK_ERROR_OUT_OF_POOL_MEMORY" } },
			{ status_code::vulkan_invalid_external_handle, { "vulkan_invalid_external_handle","Vulkan error code VK_ERROR_INVALID_EXTERNAL_HANDLE" } },
			{ status_code::vulkan_fragmentation, { "vulkan_fragmentation","Vulkan error code VK_ERROR_FRAGMENTATION" } },
			{ status_code::vulkan_invalid_opaque_capture_address, { "vulkan_invalid_opaque_capture_address","Vulkan error code VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS" } },
			{ status_code::vulkan_surface_lost_khr, { "vulkan_surface_lost_khr","Vulkan error code VK_ERROR_SURFACE_LOST_KHR" } },
			{ status_code::vulkan_native_window_in_use_khr, { "vulkan_native_window_in_use_khr","Vulkan error code VK_ERROR_NATIVE_WINDOW_IN_USE_KHR" } },
			{ status_code::vulkan_out_of_date_khr, { "vulkan_out_of_date_khr","Vulkan error code VK_ERROR_OUT_OF_DATE_KHR" } },
			{ status_code::vulkan_incompatible_display_khr, { "vulkan_incompatible_display_khr","Vulkan error code VK_ERROR_INCOMPATIBLE_DISPLAY_KHR" } },
			{ status_code::vulkan_validation_failed_ext, { "vulkan_validation_failed_ext","Vulkan error code VK_ERROR_VALIDATION_FAILED_EXT" } },
			{ status_code::vulkan_invalid_shader_nv, { "vulkan_invalid_shader_nv","Vulkan error code VK_ERROR_INVALID_SHADER_NV" } },
			{ status_code::vulkan_invalid_drm_format_modifier_plane_layout_ext, { "vulkan_invalid_drm_format_modifier_plane_layout_ext","Vulkan error code VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT" } },
			{ status_code::vulkan_not_permitted_khr, { "vulkan_not_permitted_khr","Vulkan error code VK_ERROR_NOT_PERMITTED_KHR" } },
			{ status_code::vulkan_full_screen_exclusive_mode_lost_ext, { "vulkan_full_screen_exclusive_mode_lost_ext","Vulkan error code VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT" } },
			{ status_code::vulkan_compression_exhausted_ext, { "vulkan_compression_exhausted_ext","Vulkan error code VK_ERROR_COMPRESSION_EXHAUSTED_EXT" } },
		};

	// get the name of the status code as a string 
	std::string status::name() const
		{
		auto it = status_code_string_descriptions.find( this->svalue );
		if( it == status_code_string_descriptions.end() )
			{
			return "";
			}
		return it->second.name;
		}
	
	// get a description of the status code value
	std::string status::description() const
		{
		auto it = status_code_string_descriptions.find( this->svalue );
		if( it == status_code_string_descriptions.end() )
			{
			return "";
			}
		return it->second.description;
		}

#ifdef _SYSTEM_ERROR_
	static const std::unordered_map<std::errc, status_code> errc_to_status_code_mapping =
		{
			{ std::errc::address_family_not_supported               , status_code::stl_address_family_not_supported },
			{ std::errc::address_in_use                             , status_code::stl_address_in_use },
			{ std::errc::address_not_available                      , status_code::stl_address_not_available },
			{ std::errc::already_connected                          , status_code::stl_already_connected },
			{ std::errc::argument_list_too_long                     , status_code::stl_argument_list_too_long },
			{ std::errc::argument_out_of_domain                     , status_code::stl_argument_out_of_domain },
			{ std::errc::bad_address                                , status_code::stl_bad_address },
			{ std::errc::bad_file_descriptor                        , status_code::stl_bad_file_descriptor },
			{ std::errc::bad_message                                , status_code::stl_bad_message },
			{ std::errc::broken_pipe                                , status_code::stl_broken_pipe },
			{ std::errc::connection_aborted                         , status_code::stl_connection_aborted },
			{ std::errc::connection_already_in_progress             , status_code::stl_connection_already_in_progress },
			{ std::errc::connection_refused                         , status_code::stl_connection_refused },
			{ std::errc::connection_reset                           , status_code::stl_connection_reset },
			{ std::errc::cross_device_link                          , status_code::stl_cross_device_link },
			{ std::errc::destination_address_required               , status_code::stl_destination_address_required },
			{ std::errc::device_or_resource_busy                    , status_code::stl_device_or_resource_busy },
			{ std::errc::directory_not_empty                        , status_code::stl_directory_not_empty },
			{ std::errc::executable_format_error                    , status_code::stl_executable_format_error },
			{ std::errc::file_exists                                , status_code::stl_file_exists },
			{ std::errc::file_too_large                             , status_code::stl_file_too_large },
			{ std::errc::filename_too_long                          , status_code::stl_filename_too_long },
			{ std::errc::function_not_supported                     , status_code::stl_function_not_supported },
			{ std::errc::host_unreachable                           , status_code::stl_host_unreachable },
			{ std::errc::identifier_removed                         , status_code::stl_identifier_removed },
			{ std::errc::illegal_byte_sequence                      , status_code::stl_illegal_byte_sequence },
			{ std::errc::inappropriate_io_control_operation         , status_code::stl_inappropriate_io_control_operation },
			{ std::errc::interrupted                                , status_code::stl_interrupted },
			{ std::errc::invalid_argument                           , status_code::stl_invalid_argument },
			{ std::errc::invalid_seek                               , status_code::stl_invalid_seek },
			{ std::errc::io_error                                   , status_code::stl_io_error },
			{ std::errc::is_a_directory                             , status_code::stl_is_a_directory },
			{ std::errc::message_size                               , status_code::stl_message_size },
			{ std::errc::network_down                               , status_code::stl_network_down },
			{ std::errc::network_reset                              , status_code::stl_network_reset },
			{ std::errc::network_unreachable                        , status_code::stl_network_unreachable },
			{ std::errc::no_buffer_space                            , status_code::stl_no_buffer_space },
			{ std::errc::no_child_process                           , status_code::stl_no_child_process },
			{ std::errc::no_link                                    , status_code::stl_no_link },
			{ std::errc::no_lock_available                          , status_code::stl_no_lock_available },
			{ std::errc::no_message_available                       , status_code::stl_no_message_available },
			{ std::errc::no_message                                 , status_code::stl_no_message },
			{ std::errc::no_protocol_option                         , status_code::stl_no_protocol_option },
			{ std::errc::no_space_on_device                         , status_code::stl_no_space_on_device },
			{ std::errc::no_stream_resources                        , status_code::stl_no_stream_resources },
			{ std::errc::no_such_device_or_address                  , status_code::stl_no_such_device_or_address },
			{ std::errc::no_such_device                             , status_code::stl_no_such_device },
			{ std::errc::no_such_file_or_directory                  , status_code::stl_no_such_file_or_directory },
			{ std::errc::no_such_process                            , status_code::stl_no_such_process },
			{ std::errc::not_a_directory                            , status_code::stl_not_a_directory },
			{ std::errc::not_a_socket                               , status_code::stl_not_a_socket },
			{ std::errc::not_a_stream                               , status_code::stl_not_a_stream },
			{ std::errc::not_connected                              , status_code::stl_not_connected },
			{ std::errc::not_enough_memory                          , status_code::stl_not_enough_memory },
			{ std::errc::not_supported                              , status_code::stl_not_supported },
			{ std::errc::operation_canceled                         , status_code::stl_operation_canceled },
			{ std::errc::operation_in_progress                      , status_code::stl_operation_in_progress },
			{ std::errc::operation_not_permitted                    , status_code::stl_operation_not_permitted },
			{ std::errc::operation_not_supported                    , status_code::stl_operation_not_supported },
			{ std::errc::operation_would_block                      , status_code::stl_operation_would_block },
			{ std::errc::owner_dead                                 , status_code::stl_owner_dead },
			{ std::errc::permission_denied                          , status_code::stl_permission_denied },
			{ std::errc::protocol_error                             , status_code::stl_protocol_error },
			{ std::errc::protocol_not_supported                     , status_code::stl_protocol_not_supported },
			{ std::errc::read_only_file_system                      , status_code::stl_read_only_file_system },
			{ std::errc::resource_deadlock_would_occur              , status_code::stl_resource_deadlock_would_occur },
			{ std::errc::resource_unavailable_try_again             , status_code::stl_resource_unavailable_try_again },
			{ std::errc::result_out_of_range                        , status_code::stl_result_out_of_range },
			{ std::errc::state_not_recoverable                      , status_code::stl_state_not_recoverable },
			{ std::errc::stream_timeout                             , status_code::stl_stream_timeout },
			{ std::errc::text_file_busy                             , status_code::stl_text_file_busy },
			{ std::errc::timed_out                                  , status_code::stl_timed_out },
			{ std::errc::too_many_files_open_in_system              , status_code::stl_too_many_files_open_in_system },
			{ std::errc::too_many_files_open                        , status_code::stl_too_many_files_open },
			{ std::errc::too_many_links                             , status_code::stl_too_many_links },
			{ std::errc::too_many_symbolic_link_levels              , status_code::stl_too_many_symbolic_link_levels },
			{ std::errc::value_too_large                            , status_code::stl_value_too_large },
			{ std::errc::wrong_protocol_type                        , status_code::stl_wrong_protocol_type },
		};

	status_code status::to_status_code( std::errc value ) noexcept
		{
		auto it = errc_to_status_code_mapping.find( value );
		if( it == errc_to_status_code_mapping.end() )
			return status_code::stl_unrecognized_error_code;
		return it->second;
		}

#endif//_SYSTEM_ERROR_

#ifdef VULKAN_CORE_H_
	static const std::unordered_map<VkResult, status_code> vkresult_to_status_code_mapping =
		{
			{ VkResult::VK_ERROR_OUT_OF_HOST_MEMORY                           , status_code::vulkan_out_of_host_memory                            }, 
			{ VkResult::VK_ERROR_OUT_OF_DEVICE_MEMORY                         , status_code::vulkan_out_of_device_memory                          }, 
			{ VkResult::VK_ERROR_INITIALIZATION_FAILED                        , status_code::vulkan_initialization_failed                         }, 
			{ VkResult::VK_ERROR_DEVICE_LOST                                  , status_code::vulkan_device_lost                                   }, 
			{ VkResult::VK_ERROR_MEMORY_MAP_FAILED                            , status_code::vulkan_memory_map_failed                             }, 
			{ VkResult::VK_ERROR_LAYER_NOT_PRESENT                            , status_code::vulkan_layer_not_present                             }, 
			{ VkResult::VK_ERROR_EXTENSION_NOT_PRESENT                        , status_code::vulkan_extension_not_present                         }, 
			{ VkResult::VK_ERROR_FEATURE_NOT_PRESENT                          , status_code::vulkan_feature_not_present                           }, 
			{ VkResult::VK_ERROR_INCOMPATIBLE_DRIVER                          , status_code::vulkan_incompatible_driver                           }, 
			{ VkResult::VK_ERROR_TOO_MANY_OBJECTS                             , status_code::vulkan_too_many_objects                              }, 
			{ VkResult::VK_ERROR_FORMAT_NOT_SUPPORTED                         , status_code::vulkan_format_not_supported                          }, 
			{ VkResult::VK_ERROR_FRAGMENTED_POOL                              , status_code::vulkan_fragmented_pool                               }, 
			{ VkResult::VK_ERROR_UNKNOWN                                      , status_code::vulkan_unknown                                       }, 
			{ VkResult::VK_ERROR_OUT_OF_POOL_MEMORY                           , status_code::vulkan_out_of_pool_memory                            }, 
			{ VkResult::VK_ERROR_INVALID_EXTERNAL_HANDLE                      , status_code::vulkan_invalid_external_handle                       }, 
			{ VkResult::VK_ERROR_FRAGMENTATION                                , status_code::vulkan_fragmentation                                 }, 
			{ VkResult::VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS               , status_code::vulkan_invalid_opaque_capture_address                }, 
			{ VkResult::VK_ERROR_SURFACE_LOST_KHR                             , status_code::vulkan_surface_lost_khr                              }, 
			{ VkResult::VK_ERROR_NATIVE_WINDOW_IN_USE_KHR                     , status_code::vulkan_native_window_in_use_khr                      }, 
			{ VkResult::VK_ERROR_OUT_OF_DATE_KHR                              , status_code::vulkan_out_of_date_khr                               }, 
			{ VkResult::VK_ERROR_INCOMPATIBLE_DISPLAY_KHR                     , status_code::vulkan_incompatible_display_khr                      }, 
			{ VkResult::VK_ERROR_VALIDATION_FAILED_EXT                        , status_code::vulkan_validation_failed_ext                         }, 
			{ VkResult::VK_ERROR_INVALID_SHADER_NV                            , status_code::vulkan_invalid_shader_nv                             }, 
			{ VkResult::VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT , status_code::vulkan_invalid_drm_format_modifier_plane_layout_ext  }, 
			{ VkResult::VK_ERROR_NOT_PERMITTED_KHR                            , status_code::vulkan_not_permitted_khr                             }, 
			{ VkResult::VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT          , status_code::vulkan_full_screen_exclusive_mode_lost_ext           }, 
			{ VkResult::VK_ERROR_COMPRESSION_EXHAUSTED_EXT                    , status_code::vulkan_compression_exhausted_ext                     }, 
		};
	
	status_code status::to_status_code( VkResult value ) noexcept
		{
		if( value >= VK_SUCCESS )	
			return status_code::ok;
		auto it = vkresult_to_status_code_mapping.find( value );
		if( it == vkresult_to_status_code_mapping.end() )
			return status_code::vulkan_unrecognized_error_code;
		return it->second;
		}
#endif//VULKAN_CORE_H_

#endif//CTLE_IMPLEMENTATION

	}

// stream operator for writing a status to a stream
inline std::ostream &operator<<( std::ostream &os, const ctle::status &_status )
	{
	os << _status.name() << std::string(" (\"") << _status.description() << std::string("\")");
	return os;
	}