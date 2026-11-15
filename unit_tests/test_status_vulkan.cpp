// ctle Copyright (c) 2026 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <vulkan/vulkan_core.h>
#include <ctle/status.h>

#include "unit_tests.h"

using namespace ctle;

TEST( status_vulkan, automatic_adapter )
{
	status result = VK_ERROR_OUT_OF_HOST_MEMORY;
	EXPECT_EQ( result, status::vulkan_out_of_host_memory );

	result = VK_ERROR_DEVICE_LOST;
	EXPECT_EQ( result, status::vulkan_device_lost );
	EXPECT_EQ(
		status::to_status_code( VK_ERROR_COMPRESSION_EXHAUSTED_EXT ),
		status_code::vulkan_compression_exhausted_ext
	);
}

TEST( status_vulkan, success_and_unknown )
{
	EXPECT_EQ( status( VK_SUCCESS ), status::ok );
	EXPECT_EQ( status( VK_NOT_READY ), status::ok );
	EXPECT_EQ( status( VK_TIMEOUT ), status::ok );
	EXPECT_EQ( status( VK_INCOMPLETE ), status::ok );
	EXPECT_EQ( status( static_cast<VkResult>( -999 ) ), status::vulkan_unrecognized_error_code );
}
