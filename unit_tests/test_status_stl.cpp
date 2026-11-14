// ctle Copyright (c) 2026 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <system_error>
#include <ctle/status.h>

#include "unit_tests.h"

using namespace ctle;

TEST( status_stl, automatic_adapter )
{
	status result = std::errc::permission_denied;
	EXPECT_EQ( result, status::stl_permission_denied );

	result = std::errc::no_such_file_or_directory;
	EXPECT_EQ( result, status::stl_no_such_file_or_directory );
	EXPECT_EQ( status::to_status_code( std::errc::invalid_argument ), status_code::stl_invalid_argument );
}

TEST( status_stl, success_unknown_and_aliases )
{
	EXPECT_EQ( status( std::errc{} ), status::ok );
	EXPECT_EQ( status( static_cast<std::errc>( -1 ) ), status::stl_unrecognized_error_code );

	const auto would_block = status::to_status_code( std::errc::operation_would_block );
	const auto try_again = status::to_status_code( std::errc::resource_unavailable_try_again );
	EXPECT_EQ( would_block, status_code::stl_operation_would_block );
	if( std::errc::operation_would_block == std::errc::resource_unavailable_try_again )
	{
		EXPECT_EQ( try_again, would_block );
	}
	else
	{
		EXPECT_EQ( try_again, status_code::stl_resource_unavailable_try_again );
	}
}
