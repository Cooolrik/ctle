// ctle Copyright (c) 2026 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <ctle/status.h>
#include <ctle/status_stl.h>
#include <ctle/status_vulkan.h>

#include "unit_tests.h"

using namespace ctle;

TEST( status_adapters, explicit_after_status )
{
	status result = std::errc::permission_denied;
	EXPECT_EQ( result, status::stl_permission_denied );

	result = VK_ERROR_DEVICE_LOST;
	EXPECT_EQ( result, status::vulkan_device_lost );
}
