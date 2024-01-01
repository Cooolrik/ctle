// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "../ctle/property.h"
#include "../ctle/status_return.h"

#include "unit_tests.h"

using namespace ctle;

class person 
	{
		int _weight = 0;
		int _height = 0;

	public:
		person();

		prop_get_set<int> weight;
		prop_get_set<int> height;

		prop_set<float,prop::custom> height_in_meters;
		prop_get<float,prop::cpy_custom> bmi;

		const void *last_accessed_custom_prop = nullptr;
	};

person::person() 
	: weight( _weight )
	, height( _height )
	, height_in_meters( [this]( const prop_set<float,prop::custom> *prop, const float &value ) -> status
			{
			this->last_accessed_custom_prop = prop; 
			this->_height = (int) (value * 100.f); // convert to centimeters, write directly to height.v, skip the height prop method
			return status::ok;
			})
	, bmi( [this]( const prop_get<float,prop::cpy_custom> *prop, status & ) -> float
			{
			this->last_accessed_custom_prop = prop; 
			float height_m = (float)this->_height / 100.f;
			return (float)(this->_weight) / (height_m*height_m); 
			})
	{}

TEST( property , basic_test )
	{
	person p;

	p.weight = 80;

	p.height_in_meters = 1.8f;
	EXPECT_EQ( p.last_accessed_custom_prop , &p.height_in_meters );

	EXPECT_EQ( p.height , 180 );

	float bmi = p.bmi;
	EXPECT_EQ( p.last_accessed_custom_prop , &p.bmi );
	EXPECT_NEAR( bmi, 24.6913f, 0.0001f );	// 80kg / (1.8m * 1.8m) = 24.6913 kg/m^2
	}

//class folks 
//	{
//	public:
//		folks();
//		
//		property_getcref_value<std::vector<std::unique_ptr<person>>, folks> persons;
//
//		property_getcref_value<int, folks> simple_int;
//
//		void init_persons( int nums )
//			{
//			persons.v.clear();
//			persons.v.resize(nums);
//			for (size_t i = 0; i < (size_t)nums; ++i)
//				{
//				persons.v[i] = std::unique_ptr<person>( new person() );
//				}
//			simple_int.v = nums;
//			}
//
//	};
//
//folks::folks()
//	: persons( [this]( const property_getcref_value<std::vector<std::unique_ptr<person>>, folks> *prop, status &result ) -> const std::vector<std::unique_ptr<person>> &
//		{ 
//		if( prop->v.size() < 30 ) 
//			{ 
//			static const decltype(prop->v) noresult; // create a local static value with the same type as persons
//			result = status::invalid; // set invalid as result, and return the local static value which has no value
//			return noresult;
//			} 
//		return prop->v; 
//		} 
//	)
//	, simple_int( 23 , [this]( const property_getcref_value<int, folks> *prop, status & ) -> const int & { return prop->v; } )
//	{
//	}
//
//TEST(property, complicated_property_test)
//	{
//	folks f;
//
//	// simple_int should default to 23
//	EXPECT_EQ( (int)f.simple_int, 23 );
//
//	// initialize to a low value
//	f.init_persons(20);
//
//	// trigger invalid status
//	EXPECT_THROW( f.persons.get(), ctle::status_error );
//
//	// use status parameter instead 
//	status result;
//	auto &tm = f.persons.get(result);
//	EXPECT_EQ( result, status::invalid );
//
//	// make sure that the value returned is not the actual value, but rather an empty static stand-in
//	EXPECT_EQ( tm.size() , 0 );
//
//	// initialize, so we are not invalid anymore
//	f.init_persons(60);
//
//	// after init, simple_int should be 145
//	EXPECT_EQ( (int)f.simple_int, 60 );
//
//	// make sure it is possible to access the values and that all values are set
//	EXPECT_EQ( f.persons.get().size(), 60 );
//	for( size_t inx=0; inx<f.persons.get().size(); ++inx )
//		{
//		EXPECT_NE( f.persons.get()[20].get(), nullptr );
//		}
//
//	}
//
//template<typename _Ty>
//class copy
//	{
//	public:
//		using get_set =  ctle::property_get_set<_Ty>;
//	
//	};
//
//template<typename _Ty>
//class cref
//	{
//	public:
//		using get_set =  ctle::property_getcref_set<_Ty>;
//	
//	};
//
//template<typename _Ty>
//class copy_f
//	{
//	public:
//		using get_set =  ctle::property_get_set<_Ty>;
//	
//	};
//
//template<typename _Ty>
//class cref_f
//	{
//	public:
//		using get_set =  ctle::property_getcref_set<_Ty>;
//	
//	};
//
//template<typename _Ty, template<typename> class _Tmp = copy> 
//class prop_get_set : public _Tmp<_Ty>::get_set
//	{
//	public:
//		using value_type = _Ty;
//	};
//
//class test
//	{
//	public:
//		prop_get_set<int> int_prop;
//		prop_get_set<int, cref_f> int_prop2;
//
//	};
//
//#include "../ctle/util.h"
//
//class p
//	{
//	private:
//		std::unique_ptr<std::vector<int>> &href;
//		
//	public:
//		p( std::unique_ptr<std::vector<int>> &_href ) : href(_href) {}
//
//		// pointer operator which returns const _Ty &
//		// - note: sets the result of the call in the result status parameter
//		// - note: does not raise any exceptions
//		const std::vector<int> & get() const noexcept
//			{
//			if( !(this->href) )
//				{
//				return nil_object::ref<const std::vector<int>>();
//				}
//			return *(this->href.get());
//			}
//	};
//
//class t
//	{
//	private:
//		std::unique_ptr<std::vector<int>> h;
//
//	public:
//		p prop;
//
//		t() : prop(h) 
//			{
//			//h = std::make_unique<int>();
//			//*(h.get()) = 1243;
//			}
//	};
//
//
//TEST(property, testest)
//	{
//	//test f; 
//	//
//	//f.int_prop.get();
//	//f.int_prop2.get();
//	//
//
//	t tm;
//
//	const std::vector<int> &val = tm.prop.get();
//
//	if( nil_object::is_nil(val) )
//		{
//		std::cout << "match";
//		}
//
//	}
