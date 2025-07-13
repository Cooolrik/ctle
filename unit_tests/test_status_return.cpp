// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <ctle/fwd.h>
#include <ctle/status_return.h>
#include <ctle/status.h>

#include "unit_tests.h"

using namespace ctle;

static status_return<bool, std::string> TestReturnString()
{
	return { true , "hej" };
}
enum class TestEnum : uint32_t
{
	val0 = 0,
	val15534 = 15534,
	val242398723 = 242398723
};
static status_return<TestEnum, std::unique_ptr<int>> TestReturnUniquePtr()
{
	auto ptr = std::unique_ptr<int>( new int( 100 ) );
	return { TestEnum::val242398723 , std::move( ptr ) };
}
static status_return<TestEnum, std::unique_ptr<int>> TestDontReturnUniquePtr()
{
	return { TestEnum::val0 };
}
static status_return<TestEnum, void> TestReturnJustStatus()
{
	return TestEnum::val15534;
}

TEST( status_return, basic_test )
{
	auto ret = TestReturnString();
	EXPECT_TRUE( ret.status() );
	EXPECT_TRUE( ret.value() == "hej" );
	auto ret2 = TestReturnUniquePtr();
	EXPECT_TRUE( ret2.status() == TestEnum::val242398723 );
	EXPECT_TRUE( *( ret2.value().get() ) == 100 );
	auto ret3 = TestDontReturnUniquePtr();
	EXPECT_TRUE( ret3.status() == TestEnum::val0 );
	EXPECT_TRUE( ret3.value().get() == nullptr );
	auto ret4 = TestReturnJustStatus();
	EXPECT_TRUE( ret4.status() == TestEnum::val15534 );
}

static status_return<status, std::string> TestStatusReturnString()
{
	return { status_code::ok , "hej" };
}

static status_return<status, std::unique_ptr<int>> TestStatusReturnUniquePtr()
{
	auto ptr = std::unique_ptr<int>( new int( 1000 ) );
	return { status_code::invalid_param , std::move( ptr ) };
}
static status_return<status, std::unique_ptr<int>> TestStatusDontReturnUniquePtr()
{
	return { status_code::vulkan_extension_not_present };
}
static status_return<status, void> TestStatusReturnJustStatus()
{
	return status::stl_argument_out_of_domain;
}

TEST( status_return, interop_with_status_test )
{
	auto ret = TestStatusReturnString();
	EXPECT_TRUE( ret.status() );
	EXPECT_TRUE( ret.value() == "hej" );

	// check implicit cast to bool
	EXPECT_TRUE( ret );
	EXPECT_FALSE( !ret );
	if( ret )
	{
		EXPECT_TRUE( true ); // this should happen
	}
	else
	{
		EXPECT_TRUE( false ); // this should not happen
	}
	if( !ret )
	{
		EXPECT_TRUE( false ); // this should not happen
	}
	else
	{
		EXPECT_TRUE( true ); // this should happen
	}

	auto ret2 = TestStatusReturnUniquePtr();
	EXPECT_TRUE( ret2.status() == status_code::invalid_param );
	EXPECT_TRUE( *( ret2.value().get() ) == 1000 );
	auto ret3 = TestStatusDontReturnUniquePtr();
	EXPECT_TRUE( ret3.status() == status_code::vulkan_extension_not_present );
	EXPECT_TRUE( ret3.value().get() == nullptr );
	auto ret4 = TestStatusReturnJustStatus();
	EXPECT_TRUE( ret4.status() == status_code::stl_argument_out_of_domain );

	// check implicit cast to bool
	EXPECT_FALSE( ret4 );
	EXPECT_TRUE( !ret4 );
	if( ret4 )
	{
		EXPECT_TRUE( false ); // this should not happen
	}
	else
	{
		EXPECT_TRUE( true ); // this should happen
	}
	if( !ret4 )
	{
		EXPECT_TRUE( true ); // this should happen
	}
	else
	{
		EXPECT_TRUE( false ); // this should not happen
	}
}
