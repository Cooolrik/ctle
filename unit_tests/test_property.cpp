// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "../ctle/property.h"
#include "../ctle/status_return.h"

#include "unit_tests.h"

using namespace ctle;

class person 
	{
	public:
		person();

		property_getcref_set_value<int,person> weight;

		property_getcref_set_value<int,person> height;
		property_set<float> height_in_meters;

		property_get<float> bmi;

		const void *last_accessed_prop = nullptr;
	};

person::person() : 

	weight( 
		[this]( const property_getcref_set_value<int,person> *prop, status & ) -> const int &
			{ 
			this->last_accessed_prop = prop; 
			return prop->v; 
			},
		[this]( property_getcref_set_value<int,person> *prop, const int &value ) -> status 
			{ 
			this->last_accessed_prop = prop; 
			prop->v = value; 
			return status::ok;
			}
	),
	
	height( 0,
		[this]( const property_getcref_set_value<int,person> *prop, status & ) -> const int & 
			{ 
			this->last_accessed_prop = prop; 
			return prop->v; 
			},
		[this]( property_getcref_set_value<int,person> *prop, const int &value ) -> status 
			{ 
			this->last_accessed_prop = prop; 
			prop->v = value; 
			return status::ok;
			}
	),

	height_in_meters( [this]( property_set<float> *prop, const float &value ) -> status
			{
			this->last_accessed_prop = prop; 
			this->height.v = (int) (value * 100.f); // convert to centimeters, write directly to height.v, skip the height prop method
			return status::ok;
			}
		),

	bmi( [this]( const property_get<float> *prop, status & ) -> float
			{
			this->last_accessed_prop = prop; 
			float height_m = (float)this->height.v / 100.f;
			return (float)(this->weight.v) / (height_m*height_m); 
			}
		)

	{}

TEST( property , basic_test )
	{
	person p;

	p.weight = 80;
	EXPECT_EQ( p.last_accessed_prop , &p.weight );

	p.height_in_meters = 1.8f;
	EXPECT_EQ( p.last_accessed_prop , &p.height_in_meters );

	EXPECT_EQ( p.height , 180 );
	EXPECT_EQ( p.last_accessed_prop , &p.height );

	float bmi = p.bmi;
	EXPECT_EQ( p.last_accessed_prop , &p.bmi );
	EXPECT_NEAR( bmi, 24.6913f, 0.0001f );	// 80kg / (1.8m * 1.8m) = 24.6913 kg/m^2
	}

class folks 
	{
	public:
		folks();
		
		property_getcref_value<std::vector<std::unique_ptr<person>>, folks> persons;

		property_getcref_value<int, folks> simple_int;

		void init_persons( int nums )
			{
			persons.v.clear();
			persons.v.resize(nums);
			for (size_t i = 0; i < (size_t)nums; ++i)
				{
				persons.v[i] = std::make_unique<person>();
				}
			simple_int.v = nums;
			}

	};

folks::folks()
	: persons( [this]( const auto *prop, status &result ) -> const auto & 
		{ 
		if( prop->v.size() < 30 ) 
			{ 
			static const decltype(prop->v) noresult; // create a local static value with the same type as persons
			result = status::invalid; // set invalid as result, and return the local static value which has no value
			return noresult;
			} 
		return prop->v; 
		} 
	)
	, simple_int( 23 , [this]( const auto *prop, status & ) -> const auto & { return prop->v; } )
	{
	}

TEST(property, complicated_property_test)
	{
	folks f;

	// simple_int should default to 23
	EXPECT_EQ( (int)f.simple_int, 23 );

	// initialize to a low value
	f.init_persons(20);

	// trigger invalid status
	EXPECT_THROW( f.persons.get(), ctle::status_error );

	// use status parameter instead 
	status result;
	auto &tm = f.persons.get(result);
	EXPECT_EQ( result, status::invalid );

	// make sure that the value returned is not the actual value, but rather an empty static stand-in
	EXPECT_EQ( tm.size() , 0 );

	// initialize, so we are not invalid anymore
	f.init_persons(60);

	// after init, simple_int should be 145
	EXPECT_EQ( (int)f.simple_int, 60 );

	// make sure it is possible to access the values and that all values are set
	EXPECT_EQ( f.persons.get().size(), 60 );
	for( size_t inx=0; inx<f.persons.get().size(); ++inx )
		{
		EXPECT_NE( f.persons.get()[20].get(), nullptr );
		}

	}