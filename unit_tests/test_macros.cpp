// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "../ctle/status.h"
#include "../ctle/status_return.h"
#include "../ctle/log.h"

#include "unit_tests.h"

#include "../ctle/_macros.inl"

using namespace ctle;

status ReturnsStatus()
	{
	return status::cant_access;
	}

status ChecksStatus()
	{
	ctStatusCall( ReturnsStatus() );
	return status::ok;
	}

status_return<status,std::string> ReturnsStatusReturnError()
	{
	return status::already_exists;
	}

status ChecksStatusReturnError()
	{
	ctStatusReturnCall( return_str , ReturnsStatusReturnError() );
	
	// this should never happen:
	if( return_str == "")
		return status::corrupted;
	return status::ok;
	}

status_return<status,std::string> ReturnsStatusReturnSuccessfulValue()
	{
	return std::string("successful value");
	}

status ChecksStatusReturnErrorSuccessfulValue()
	{
	ctStatusReturnCall( return_str , ReturnsStatusReturnSuccessfulValue() );
	if( return_str == "successful value" )
		return status::ok;
	return status::cant_read;
	}

status TriggersValidation( bool should_trigger )
	{
	ctValidate( !should_trigger, status::invalid_param ) << "The param was set to trigger" << ctValidateEnd;
	return status::ok;
	}

void TestSanityCheck()
	{
	// sanity check should succeed and continue
	int value = 1;
	ctSanityCheck( value )

	// sanity check should trigger, and return a runtime_error 
	value = 0;
	try
		{
		ctSanityCheck( value )
		}
	catch( std::runtime_error &ex )
		{
		const char *ptr = ex.what();
		EXPECT_NE( strstr(ptr,"TestSanityCheck(") , nullptr );
		value = 1245;
		}
	EXPECT_EQ( value, 1245 );
	}


static std::string last_string;
static void _log_function( log_level, const char *, const char *message )
	{
	last_string = message;
	}

TEST( macros , basic_test )
	{
	// override log function so we don't write errors to the log 
	auto old_log_func = ctle::get_global_log_function();
	ctle::set_global_log_function(&_log_function);
	auto old_log_level = ctle::get_global_log_level();
	ctle::set_global_log_level( ctle::log_level::error );

	EXPECT_EQ( ChecksStatus() , status::cant_access );
	EXPECT_EQ( ChecksStatusReturnError() , status::already_exists );
	EXPECT_EQ( ChecksStatusReturnErrorSuccessfulValue() , status::ok );

	TestSanityCheck();

	// check that the validation triggers when it should
	EXPECT_EQ( TriggersValidation(true), status::invalid_param );	// should trigger
	EXPECT_EQ( TriggersValidation(false), status::ok );	// shouldn't trigger

	// run the ctLogThis macro
	ctle::set_global_log_level( ctle::log_level::verbose );
	ctLogThis
	std::stringstream ss;
	ss << this;
	EXPECT_EQ( last_string , ss.str() );

	// run the other log macros
	ctLogError << "Error text" << ctLogEnd;
	EXPECT_EQ( last_string , "Error text" );
	ctLogWarning << "Warning text" << ctLogEnd;
	EXPECT_EQ( last_string , "Warning text" );
	ctLogInfo << "Info text" << ctLogEnd;
	EXPECT_EQ( last_string , "Info text" );
	ctLogDebug << "Debug text" << ctLogEnd;
	EXPECT_EQ( last_string , "Debug text" );
	ctLogVerbose << "Verbose text" << ctLogEnd;
	EXPECT_EQ( last_string , "Verbose text" );

	std::string str = "Hello my " _CTLE_STRINGIZE( 12345 ) " friends.";
	EXPECT_EQ( str , "Hello my 12345 friends." );

	// restore old log function
	ctle::set_global_log_function( old_log_func );
	ctle::set_global_log_level( old_log_level );
	}

// make sure the macros were created by _macros.inl
#if !defined(_CTLE_MACROS_INCLUDED) || !defined(_CTLE_STRINGIZE_DETAIL) || !defined(_CTLE_STRINGIZE) || !defined(ctLogLevel) || !defined(ctStatusCall)
#error The _macros.inl file failed to create the macros
#endif

#include "../ctle/_undef_macros.inl"

// make sure the macros were removed by _undef_macros.inl
#if defined(_CTLE_MACROS_INCLUDED) || defined(_CTLE_STRINGIZE_DETAIL) || defined(_CTLE_STRINGIZE) || defined(ctLogLevel) || defined(ctStatusCall)
#error The _undef_macros.inl file failed to remove the macros
#endif

#include "../ctle/_macros.inl"

// make sure the macros were created by _macros.inl again
#if !defined(_CTLE_MACROS_INCLUDED) || !defined(_CTLE_STRINGIZE_DETAIL) || !defined(_CTLE_STRINGIZE) || !defined(ctLogLevel) || !defined(ctStatusCall)
#error The second include of _macros.inl file failed to create the macros
#endif


