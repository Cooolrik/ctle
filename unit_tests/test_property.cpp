// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "../ctle/property.h"

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
		[this]( const property_getcref_set_value<int,person> *prop ) -> const int & 
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
		[this]( const property_getcref_set_value<int,person> *prop ) -> const int & 
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

	bmi( [this]( const property_get<float> *prop ) -> float
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

		void init_persons()
			{
			persons.v.resize(60);
			for (size_t i = 0; i < 60; ++i)
				{
				persons.v[i] = std::make_unique<person>();
				
				}
			}

	};

folks::folks()
	: persons( [this]( const auto *prop ) -> const auto & { return prop->v; } )
	, simple_int( 23 , [this]( const auto *prop ) -> const auto & { return prop->v; } )
	{
	}

TEST(property, complicated_property_test)
	{
	folks f;
	f.init_persons();

	EXPECT_EQ( (int)f.simple_int, 23 );

	// make sure it is possible to access the values and that all values are set
	EXPECT_EQ( f.persons.get().size(), 60 );
	for( size_t inx=0; inx<f.persons.get().size(); ++inx )
		{
		EXPECT_NE( f.persons.get()[20].get(), nullptr );
		}

	}