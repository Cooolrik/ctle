// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "unit_tests.h"

#include "../ctle/status_return.h"

using namespace ctle;

status_return<bool,std::string> TestReturnString()
	{
	return { true , "hej" };
	}
enum class TestEnum : uint32_t
	{
	val0 = 0,
	val15534 = 15534,
	val242398723 = 242398723
	};
status_return<TestEnum,std::unique_ptr<int>> TestReturnUniquePtr()
	{
	auto ptr = std::make_unique<int>(100);
	return { TestEnum::val242398723 , std::move(ptr) };
	}
status_return<TestEnum,std::unique_ptr<int>> TestDontReturnUniquePtr()
	{
	return { TestEnum::val0 };
	}
status_return<TestEnum,void> TestReturnJustStatus()
	{
	return TestEnum::val15534;
	}

TEST( status_return , basic_test )
	{
	auto ret = TestReturnString();
	EXPECT_TRUE( ret.status() );
	EXPECT_TRUE( ret.value() == "hej" );
	auto ret2 = TestReturnUniquePtr();
	EXPECT_TRUE( ret2.status() == TestEnum::val242398723 );
	EXPECT_TRUE( *(ret2.value().get()) == 100 );
	auto ret3 = TestDontReturnUniquePtr();
	EXPECT_TRUE( ret3.status() == TestEnum::val0 );
	EXPECT_TRUE( ret3.value().get() == nullptr );
	auto ret4 = TestReturnJustStatus();
	EXPECT_TRUE( ret4.status() == TestEnum::val15534 );
	}
