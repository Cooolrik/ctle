// ctle Copyright (c) 2022 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "unit_tests.h"
#include <ctle/status.h>
#include <ctle/ntup.h>  
#include <ctle/hasher.h>   
#include <ctle/read_stream.h>
#include <ctle/data_source.h>
#include <ctle/write_stream.h>
#include <ctle/data_destination.h>
		
template<class _Ty> ctle::status _read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm, std::vector<_Ty> &data )
{
	u32 cnt = 0;
	ctle::status res = strm.read<u32>( &cnt, 1 );
	if( !res ) 
  		return res; 
	data.resize(cnt);
	return strm.read<_Ty>( data.data(), data.size() );
}
		
template<class _Ty> ctle::status _write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm, const std::vector<_Ty> &data )
{
	ctle::status res = strm.write<u32>( (u32)data.size() );
	if( !res ) 
  		return res; 
	return strm.write<_Ty>( data.data(), data.size() );
}
		
bool _are_equal( const variant &a , const variant &b )
{
	if( a.type() != b.type() )
		return false;
	if( a.type_count() != b.type_count() )
		return false;
	return memcmp( a.cdata(), b.cdata(), a.type_size() * a.type_count() ) == 0;
}


class variant_0 : public variant
{
public:
	std::vector<ctle::i8> data;

	virtual ~variant_0() override {}
	virtual size_t type() const override { return 0; }
	virtual const char *type_name() const override { return "i8"; }
	virtual size_t type_size() const override { return sizeof(ctle::i8); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::i8>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::i8>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::i8>( strm, this->data ); }
};

class variant_1 : public variant
{
public:
	std::vector<ctle::n_tup<i8,1>> data;

	virtual ~variant_1() override {}
	virtual size_t type() const override { return 1; }
	virtual const char *type_name() const override { return "n_tup<i8,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<i8,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<i8,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<i8,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<i8,1>>( strm, this->data ); }
};

class variant_2 : public variant
{
public:
	std::vector<ctle::n_tup<i8,2>> data;

	virtual ~variant_2() override {}
	virtual size_t type() const override { return 2; }
	virtual const char *type_name() const override { return "n_tup<i8,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<i8,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<i8,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<i8,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<i8,2>>( strm, this->data ); }
};

class variant_3 : public variant
{
public:
	std::vector<ctle::n_tup<i8,3>> data;

	virtual ~variant_3() override {}
	virtual size_t type() const override { return 3; }
	virtual const char *type_name() const override { return "n_tup<i8,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<i8,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<i8,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<i8,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<i8,3>>( strm, this->data ); }
};

class variant_4 : public variant
{
public:
	std::vector<ctle::n_tup<i8,4>> data;

	virtual ~variant_4() override {}
	virtual size_t type() const override { return 4; }
	virtual const char *type_name() const override { return "n_tup<i8,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<i8,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<i8,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<i8,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<i8,4>>( strm, this->data ); }
};

class variant_5 : public variant
{
public:
	std::vector<ctle::mn_tup<i8,1,1>> data;

	virtual ~variant_5() override {}
	virtual size_t type() const override { return 5; }
	virtual const char *type_name() const override { return "mn_tup<i8,1,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i8,1,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i8,1,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i8,1,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i8,1,1>>( strm, this->data ); }
};

class variant_6 : public variant
{
public:
	std::vector<ctle::mn_tup<i8,1,2>> data;

	virtual ~variant_6() override {}
	virtual size_t type() const override { return 6; }
	virtual const char *type_name() const override { return "mn_tup<i8,1,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i8,1,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i8,1,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i8,1,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i8,1,2>>( strm, this->data ); }
};

class variant_7 : public variant
{
public:
	std::vector<ctle::mn_tup<i8,1,3>> data;

	virtual ~variant_7() override {}
	virtual size_t type() const override { return 7; }
	virtual const char *type_name() const override { return "mn_tup<i8,1,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i8,1,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i8,1,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i8,1,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i8,1,3>>( strm, this->data ); }
};

class variant_8 : public variant
{
public:
	std::vector<ctle::mn_tup<i8,1,4>> data;

	virtual ~variant_8() override {}
	virtual size_t type() const override { return 8; }
	virtual const char *type_name() const override { return "mn_tup<i8,1,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i8,1,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i8,1,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i8,1,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i8,1,4>>( strm, this->data ); }
};

class variant_9 : public variant
{
public:
	std::vector<ctle::mn_tup<i8,2,1>> data;

	virtual ~variant_9() override {}
	virtual size_t type() const override { return 9; }
	virtual const char *type_name() const override { return "mn_tup<i8,2,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i8,2,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i8,2,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i8,2,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i8,2,1>>( strm, this->data ); }
};

class variant_10 : public variant
{
public:
	std::vector<ctle::mn_tup<i8,2,2>> data;

	virtual ~variant_10() override {}
	virtual size_t type() const override { return 10; }
	virtual const char *type_name() const override { return "mn_tup<i8,2,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i8,2,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i8,2,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i8,2,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i8,2,2>>( strm, this->data ); }
};

class variant_11 : public variant
{
public:
	std::vector<ctle::mn_tup<i8,2,3>> data;

	virtual ~variant_11() override {}
	virtual size_t type() const override { return 11; }
	virtual const char *type_name() const override { return "mn_tup<i8,2,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i8,2,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i8,2,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i8,2,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i8,2,3>>( strm, this->data ); }
};

class variant_12 : public variant
{
public:
	std::vector<ctle::mn_tup<i8,2,4>> data;

	virtual ~variant_12() override {}
	virtual size_t type() const override { return 12; }
	virtual const char *type_name() const override { return "mn_tup<i8,2,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i8,2,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i8,2,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i8,2,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i8,2,4>>( strm, this->data ); }
};

class variant_13 : public variant
{
public:
	std::vector<ctle::mn_tup<i8,3,1>> data;

	virtual ~variant_13() override {}
	virtual size_t type() const override { return 13; }
	virtual const char *type_name() const override { return "mn_tup<i8,3,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i8,3,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i8,3,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i8,3,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i8,3,1>>( strm, this->data ); }
};

class variant_14 : public variant
{
public:
	std::vector<ctle::mn_tup<i8,3,2>> data;

	virtual ~variant_14() override {}
	virtual size_t type() const override { return 14; }
	virtual const char *type_name() const override { return "mn_tup<i8,3,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i8,3,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i8,3,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i8,3,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i8,3,2>>( strm, this->data ); }
};

class variant_15 : public variant
{
public:
	std::vector<ctle::mn_tup<i8,3,3>> data;

	virtual ~variant_15() override {}
	virtual size_t type() const override { return 15; }
	virtual const char *type_name() const override { return "mn_tup<i8,3,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i8,3,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i8,3,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i8,3,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i8,3,3>>( strm, this->data ); }
};

class variant_16 : public variant
{
public:
	std::vector<ctle::mn_tup<i8,3,4>> data;

	virtual ~variant_16() override {}
	virtual size_t type() const override { return 16; }
	virtual const char *type_name() const override { return "mn_tup<i8,3,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i8,3,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i8,3,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i8,3,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i8,3,4>>( strm, this->data ); }
};

class variant_17 : public variant
{
public:
	std::vector<ctle::mn_tup<i8,4,1>> data;

	virtual ~variant_17() override {}
	virtual size_t type() const override { return 17; }
	virtual const char *type_name() const override { return "mn_tup<i8,4,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i8,4,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i8,4,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i8,4,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i8,4,1>>( strm, this->data ); }
};

class variant_18 : public variant
{
public:
	std::vector<ctle::mn_tup<i8,4,2>> data;

	virtual ~variant_18() override {}
	virtual size_t type() const override { return 18; }
	virtual const char *type_name() const override { return "mn_tup<i8,4,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i8,4,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i8,4,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i8,4,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i8,4,2>>( strm, this->data ); }
};

class variant_19 : public variant
{
public:
	std::vector<ctle::mn_tup<i8,4,3>> data;

	virtual ~variant_19() override {}
	virtual size_t type() const override { return 19; }
	virtual const char *type_name() const override { return "mn_tup<i8,4,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i8,4,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i8,4,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i8,4,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i8,4,3>>( strm, this->data ); }
};

class variant_20 : public variant
{
public:
	std::vector<ctle::mn_tup<i8,4,4>> data;

	virtual ~variant_20() override {}
	virtual size_t type() const override { return 20; }
	virtual const char *type_name() const override { return "mn_tup<i8,4,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i8,4,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i8,4,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i8,4,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i8,4,4>>( strm, this->data ); }
};

class variant_21 : public variant
{
public:
	std::vector<ctle::u8> data;

	virtual ~variant_21() override {}
	virtual size_t type() const override { return 21; }
	virtual const char *type_name() const override { return "u8"; }
	virtual size_t type_size() const override { return sizeof(ctle::u8); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::u8>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::u8>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::u8>( strm, this->data ); }
};

class variant_22 : public variant
{
public:
	std::vector<ctle::n_tup<u8,1>> data;

	virtual ~variant_22() override {}
	virtual size_t type() const override { return 22; }
	virtual const char *type_name() const override { return "n_tup<u8,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<u8,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<u8,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<u8,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<u8,1>>( strm, this->data ); }
};

class variant_23 : public variant
{
public:
	std::vector<ctle::n_tup<u8,2>> data;

	virtual ~variant_23() override {}
	virtual size_t type() const override { return 23; }
	virtual const char *type_name() const override { return "n_tup<u8,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<u8,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<u8,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<u8,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<u8,2>>( strm, this->data ); }
};

class variant_24 : public variant
{
public:
	std::vector<ctle::n_tup<u8,3>> data;

	virtual ~variant_24() override {}
	virtual size_t type() const override { return 24; }
	virtual const char *type_name() const override { return "n_tup<u8,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<u8,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<u8,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<u8,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<u8,3>>( strm, this->data ); }
};

class variant_25 : public variant
{
public:
	std::vector<ctle::n_tup<u8,4>> data;

	virtual ~variant_25() override {}
	virtual size_t type() const override { return 25; }
	virtual const char *type_name() const override { return "n_tup<u8,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<u8,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<u8,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<u8,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<u8,4>>( strm, this->data ); }
};

class variant_26 : public variant
{
public:
	std::vector<ctle::mn_tup<u8,1,1>> data;

	virtual ~variant_26() override {}
	virtual size_t type() const override { return 26; }
	virtual const char *type_name() const override { return "mn_tup<u8,1,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u8,1,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u8,1,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u8,1,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u8,1,1>>( strm, this->data ); }
};

class variant_27 : public variant
{
public:
	std::vector<ctle::mn_tup<u8,1,2>> data;

	virtual ~variant_27() override {}
	virtual size_t type() const override { return 27; }
	virtual const char *type_name() const override { return "mn_tup<u8,1,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u8,1,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u8,1,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u8,1,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u8,1,2>>( strm, this->data ); }
};

class variant_28 : public variant
{
public:
	std::vector<ctle::mn_tup<u8,1,3>> data;

	virtual ~variant_28() override {}
	virtual size_t type() const override { return 28; }
	virtual const char *type_name() const override { return "mn_tup<u8,1,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u8,1,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u8,1,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u8,1,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u8,1,3>>( strm, this->data ); }
};

class variant_29 : public variant
{
public:
	std::vector<ctle::mn_tup<u8,1,4>> data;

	virtual ~variant_29() override {}
	virtual size_t type() const override { return 29; }
	virtual const char *type_name() const override { return "mn_tup<u8,1,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u8,1,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u8,1,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u8,1,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u8,1,4>>( strm, this->data ); }
};

class variant_30 : public variant
{
public:
	std::vector<ctle::mn_tup<u8,2,1>> data;

	virtual ~variant_30() override {}
	virtual size_t type() const override { return 30; }
	virtual const char *type_name() const override { return "mn_tup<u8,2,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u8,2,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u8,2,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u8,2,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u8,2,1>>( strm, this->data ); }
};

class variant_31 : public variant
{
public:
	std::vector<ctle::mn_tup<u8,2,2>> data;

	virtual ~variant_31() override {}
	virtual size_t type() const override { return 31; }
	virtual const char *type_name() const override { return "mn_tup<u8,2,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u8,2,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u8,2,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u8,2,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u8,2,2>>( strm, this->data ); }
};

class variant_32 : public variant
{
public:
	std::vector<ctle::mn_tup<u8,2,3>> data;

	virtual ~variant_32() override {}
	virtual size_t type() const override { return 32; }
	virtual const char *type_name() const override { return "mn_tup<u8,2,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u8,2,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u8,2,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u8,2,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u8,2,3>>( strm, this->data ); }
};

class variant_33 : public variant
{
public:
	std::vector<ctle::mn_tup<u8,2,4>> data;

	virtual ~variant_33() override {}
	virtual size_t type() const override { return 33; }
	virtual const char *type_name() const override { return "mn_tup<u8,2,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u8,2,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u8,2,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u8,2,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u8,2,4>>( strm, this->data ); }
};

class variant_34 : public variant
{
public:
	std::vector<ctle::mn_tup<u8,3,1>> data;

	virtual ~variant_34() override {}
	virtual size_t type() const override { return 34; }
	virtual const char *type_name() const override { return "mn_tup<u8,3,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u8,3,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u8,3,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u8,3,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u8,3,1>>( strm, this->data ); }
};

class variant_35 : public variant
{
public:
	std::vector<ctle::mn_tup<u8,3,2>> data;

	virtual ~variant_35() override {}
	virtual size_t type() const override { return 35; }
	virtual const char *type_name() const override { return "mn_tup<u8,3,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u8,3,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u8,3,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u8,3,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u8,3,2>>( strm, this->data ); }
};

class variant_36 : public variant
{
public:
	std::vector<ctle::mn_tup<u8,3,3>> data;

	virtual ~variant_36() override {}
	virtual size_t type() const override { return 36; }
	virtual const char *type_name() const override { return "mn_tup<u8,3,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u8,3,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u8,3,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u8,3,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u8,3,3>>( strm, this->data ); }
};

class variant_37 : public variant
{
public:
	std::vector<ctle::mn_tup<u8,3,4>> data;

	virtual ~variant_37() override {}
	virtual size_t type() const override { return 37; }
	virtual const char *type_name() const override { return "mn_tup<u8,3,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u8,3,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u8,3,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u8,3,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u8,3,4>>( strm, this->data ); }
};

class variant_38 : public variant
{
public:
	std::vector<ctle::mn_tup<u8,4,1>> data;

	virtual ~variant_38() override {}
	virtual size_t type() const override { return 38; }
	virtual const char *type_name() const override { return "mn_tup<u8,4,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u8,4,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u8,4,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u8,4,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u8,4,1>>( strm, this->data ); }
};

class variant_39 : public variant
{
public:
	std::vector<ctle::mn_tup<u8,4,2>> data;

	virtual ~variant_39() override {}
	virtual size_t type() const override { return 39; }
	virtual const char *type_name() const override { return "mn_tup<u8,4,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u8,4,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u8,4,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u8,4,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u8,4,2>>( strm, this->data ); }
};

class variant_40 : public variant
{
public:
	std::vector<ctle::mn_tup<u8,4,3>> data;

	virtual ~variant_40() override {}
	virtual size_t type() const override { return 40; }
	virtual const char *type_name() const override { return "mn_tup<u8,4,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u8,4,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u8,4,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u8,4,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u8,4,3>>( strm, this->data ); }
};

class variant_41 : public variant
{
public:
	std::vector<ctle::mn_tup<u8,4,4>> data;

	virtual ~variant_41() override {}
	virtual size_t type() const override { return 41; }
	virtual const char *type_name() const override { return "mn_tup<u8,4,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u8,4,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u8,4,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u8,4,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u8,4,4>>( strm, this->data ); }
};

class variant_42 : public variant
{
public:
	std::vector<ctle::i16> data;

	virtual ~variant_42() override {}
	virtual size_t type() const override { return 42; }
	virtual const char *type_name() const override { return "i16"; }
	virtual size_t type_size() const override { return sizeof(ctle::i16); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::i16>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::i16>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::i16>( strm, this->data ); }
};

class variant_43 : public variant
{
public:
	std::vector<ctle::n_tup<i16,1>> data;

	virtual ~variant_43() override {}
	virtual size_t type() const override { return 43; }
	virtual const char *type_name() const override { return "n_tup<i16,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<i16,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<i16,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<i16,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<i16,1>>( strm, this->data ); }
};

class variant_44 : public variant
{
public:
	std::vector<ctle::n_tup<i16,2>> data;

	virtual ~variant_44() override {}
	virtual size_t type() const override { return 44; }
	virtual const char *type_name() const override { return "n_tup<i16,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<i16,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<i16,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<i16,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<i16,2>>( strm, this->data ); }
};

class variant_45 : public variant
{
public:
	std::vector<ctle::n_tup<i16,3>> data;

	virtual ~variant_45() override {}
	virtual size_t type() const override { return 45; }
	virtual const char *type_name() const override { return "n_tup<i16,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<i16,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<i16,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<i16,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<i16,3>>( strm, this->data ); }
};

class variant_46 : public variant
{
public:
	std::vector<ctle::n_tup<i16,4>> data;

	virtual ~variant_46() override {}
	virtual size_t type() const override { return 46; }
	virtual const char *type_name() const override { return "n_tup<i16,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<i16,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<i16,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<i16,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<i16,4>>( strm, this->data ); }
};

class variant_47 : public variant
{
public:
	std::vector<ctle::mn_tup<i16,1,1>> data;

	virtual ~variant_47() override {}
	virtual size_t type() const override { return 47; }
	virtual const char *type_name() const override { return "mn_tup<i16,1,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i16,1,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i16,1,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i16,1,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i16,1,1>>( strm, this->data ); }
};

class variant_48 : public variant
{
public:
	std::vector<ctle::mn_tup<i16,1,2>> data;

	virtual ~variant_48() override {}
	virtual size_t type() const override { return 48; }
	virtual const char *type_name() const override { return "mn_tup<i16,1,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i16,1,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i16,1,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i16,1,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i16,1,2>>( strm, this->data ); }
};

class variant_49 : public variant
{
public:
	std::vector<ctle::mn_tup<i16,1,3>> data;

	virtual ~variant_49() override {}
	virtual size_t type() const override { return 49; }
	virtual const char *type_name() const override { return "mn_tup<i16,1,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i16,1,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i16,1,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i16,1,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i16,1,3>>( strm, this->data ); }
};

class variant_50 : public variant
{
public:
	std::vector<ctle::mn_tup<i16,1,4>> data;

	virtual ~variant_50() override {}
	virtual size_t type() const override { return 50; }
	virtual const char *type_name() const override { return "mn_tup<i16,1,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i16,1,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i16,1,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i16,1,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i16,1,4>>( strm, this->data ); }
};

class variant_51 : public variant
{
public:
	std::vector<ctle::mn_tup<i16,2,1>> data;

	virtual ~variant_51() override {}
	virtual size_t type() const override { return 51; }
	virtual const char *type_name() const override { return "mn_tup<i16,2,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i16,2,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i16,2,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i16,2,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i16,2,1>>( strm, this->data ); }
};

class variant_52 : public variant
{
public:
	std::vector<ctle::mn_tup<i16,2,2>> data;

	virtual ~variant_52() override {}
	virtual size_t type() const override { return 52; }
	virtual const char *type_name() const override { return "mn_tup<i16,2,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i16,2,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i16,2,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i16,2,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i16,2,2>>( strm, this->data ); }
};

class variant_53 : public variant
{
public:
	std::vector<ctle::mn_tup<i16,2,3>> data;

	virtual ~variant_53() override {}
	virtual size_t type() const override { return 53; }
	virtual const char *type_name() const override { return "mn_tup<i16,2,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i16,2,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i16,2,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i16,2,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i16,2,3>>( strm, this->data ); }
};

class variant_54 : public variant
{
public:
	std::vector<ctle::mn_tup<i16,2,4>> data;

	virtual ~variant_54() override {}
	virtual size_t type() const override { return 54; }
	virtual const char *type_name() const override { return "mn_tup<i16,2,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i16,2,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i16,2,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i16,2,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i16,2,4>>( strm, this->data ); }
};

class variant_55 : public variant
{
public:
	std::vector<ctle::mn_tup<i16,3,1>> data;

	virtual ~variant_55() override {}
	virtual size_t type() const override { return 55; }
	virtual const char *type_name() const override { return "mn_tup<i16,3,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i16,3,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i16,3,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i16,3,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i16,3,1>>( strm, this->data ); }
};

class variant_56 : public variant
{
public:
	std::vector<ctle::mn_tup<i16,3,2>> data;

	virtual ~variant_56() override {}
	virtual size_t type() const override { return 56; }
	virtual const char *type_name() const override { return "mn_tup<i16,3,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i16,3,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i16,3,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i16,3,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i16,3,2>>( strm, this->data ); }
};

class variant_57 : public variant
{
public:
	std::vector<ctle::mn_tup<i16,3,3>> data;

	virtual ~variant_57() override {}
	virtual size_t type() const override { return 57; }
	virtual const char *type_name() const override { return "mn_tup<i16,3,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i16,3,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i16,3,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i16,3,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i16,3,3>>( strm, this->data ); }
};

class variant_58 : public variant
{
public:
	std::vector<ctle::mn_tup<i16,3,4>> data;

	virtual ~variant_58() override {}
	virtual size_t type() const override { return 58; }
	virtual const char *type_name() const override { return "mn_tup<i16,3,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i16,3,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i16,3,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i16,3,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i16,3,4>>( strm, this->data ); }
};

class variant_59 : public variant
{
public:
	std::vector<ctle::mn_tup<i16,4,1>> data;

	virtual ~variant_59() override {}
	virtual size_t type() const override { return 59; }
	virtual const char *type_name() const override { return "mn_tup<i16,4,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i16,4,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i16,4,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i16,4,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i16,4,1>>( strm, this->data ); }
};

class variant_60 : public variant
{
public:
	std::vector<ctle::mn_tup<i16,4,2>> data;

	virtual ~variant_60() override {}
	virtual size_t type() const override { return 60; }
	virtual const char *type_name() const override { return "mn_tup<i16,4,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i16,4,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i16,4,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i16,4,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i16,4,2>>( strm, this->data ); }
};

class variant_61 : public variant
{
public:
	std::vector<ctle::mn_tup<i16,4,3>> data;

	virtual ~variant_61() override {}
	virtual size_t type() const override { return 61; }
	virtual const char *type_name() const override { return "mn_tup<i16,4,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i16,4,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i16,4,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i16,4,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i16,4,3>>( strm, this->data ); }
};

class variant_62 : public variant
{
public:
	std::vector<ctle::mn_tup<i16,4,4>> data;

	virtual ~variant_62() override {}
	virtual size_t type() const override { return 62; }
	virtual const char *type_name() const override { return "mn_tup<i16,4,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i16,4,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i16,4,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i16,4,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i16,4,4>>( strm, this->data ); }
};

class variant_63 : public variant
{
public:
	std::vector<ctle::u16> data;

	virtual ~variant_63() override {}
	virtual size_t type() const override { return 63; }
	virtual const char *type_name() const override { return "u16"; }
	virtual size_t type_size() const override { return sizeof(ctle::u16); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::u16>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::u16>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::u16>( strm, this->data ); }
};

class variant_64 : public variant
{
public:
	std::vector<ctle::n_tup<u16,1>> data;

	virtual ~variant_64() override {}
	virtual size_t type() const override { return 64; }
	virtual const char *type_name() const override { return "n_tup<u16,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<u16,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<u16,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<u16,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<u16,1>>( strm, this->data ); }
};

class variant_65 : public variant
{
public:
	std::vector<ctle::n_tup<u16,2>> data;

	virtual ~variant_65() override {}
	virtual size_t type() const override { return 65; }
	virtual const char *type_name() const override { return "n_tup<u16,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<u16,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<u16,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<u16,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<u16,2>>( strm, this->data ); }
};

class variant_66 : public variant
{
public:
	std::vector<ctle::n_tup<u16,3>> data;

	virtual ~variant_66() override {}
	virtual size_t type() const override { return 66; }
	virtual const char *type_name() const override { return "n_tup<u16,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<u16,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<u16,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<u16,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<u16,3>>( strm, this->data ); }
};

class variant_67 : public variant
{
public:
	std::vector<ctle::n_tup<u16,4>> data;

	virtual ~variant_67() override {}
	virtual size_t type() const override { return 67; }
	virtual const char *type_name() const override { return "n_tup<u16,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<u16,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<u16,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<u16,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<u16,4>>( strm, this->data ); }
};

class variant_68 : public variant
{
public:
	std::vector<ctle::mn_tup<u16,1,1>> data;

	virtual ~variant_68() override {}
	virtual size_t type() const override { return 68; }
	virtual const char *type_name() const override { return "mn_tup<u16,1,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u16,1,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u16,1,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u16,1,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u16,1,1>>( strm, this->data ); }
};

class variant_69 : public variant
{
public:
	std::vector<ctle::mn_tup<u16,1,2>> data;

	virtual ~variant_69() override {}
	virtual size_t type() const override { return 69; }
	virtual const char *type_name() const override { return "mn_tup<u16,1,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u16,1,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u16,1,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u16,1,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u16,1,2>>( strm, this->data ); }
};

class variant_70 : public variant
{
public:
	std::vector<ctle::mn_tup<u16,1,3>> data;

	virtual ~variant_70() override {}
	virtual size_t type() const override { return 70; }
	virtual const char *type_name() const override { return "mn_tup<u16,1,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u16,1,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u16,1,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u16,1,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u16,1,3>>( strm, this->data ); }
};

class variant_71 : public variant
{
public:
	std::vector<ctle::mn_tup<u16,1,4>> data;

	virtual ~variant_71() override {}
	virtual size_t type() const override { return 71; }
	virtual const char *type_name() const override { return "mn_tup<u16,1,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u16,1,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u16,1,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u16,1,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u16,1,4>>( strm, this->data ); }
};

class variant_72 : public variant
{
public:
	std::vector<ctle::mn_tup<u16,2,1>> data;

	virtual ~variant_72() override {}
	virtual size_t type() const override { return 72; }
	virtual const char *type_name() const override { return "mn_tup<u16,2,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u16,2,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u16,2,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u16,2,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u16,2,1>>( strm, this->data ); }
};

class variant_73 : public variant
{
public:
	std::vector<ctle::mn_tup<u16,2,2>> data;

	virtual ~variant_73() override {}
	virtual size_t type() const override { return 73; }
	virtual const char *type_name() const override { return "mn_tup<u16,2,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u16,2,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u16,2,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u16,2,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u16,2,2>>( strm, this->data ); }
};

class variant_74 : public variant
{
public:
	std::vector<ctle::mn_tup<u16,2,3>> data;

	virtual ~variant_74() override {}
	virtual size_t type() const override { return 74; }
	virtual const char *type_name() const override { return "mn_tup<u16,2,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u16,2,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u16,2,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u16,2,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u16,2,3>>( strm, this->data ); }
};

class variant_75 : public variant
{
public:
	std::vector<ctle::mn_tup<u16,2,4>> data;

	virtual ~variant_75() override {}
	virtual size_t type() const override { return 75; }
	virtual const char *type_name() const override { return "mn_tup<u16,2,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u16,2,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u16,2,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u16,2,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u16,2,4>>( strm, this->data ); }
};

class variant_76 : public variant
{
public:
	std::vector<ctle::mn_tup<u16,3,1>> data;

	virtual ~variant_76() override {}
	virtual size_t type() const override { return 76; }
	virtual const char *type_name() const override { return "mn_tup<u16,3,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u16,3,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u16,3,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u16,3,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u16,3,1>>( strm, this->data ); }
};

class variant_77 : public variant
{
public:
	std::vector<ctle::mn_tup<u16,3,2>> data;

	virtual ~variant_77() override {}
	virtual size_t type() const override { return 77; }
	virtual const char *type_name() const override { return "mn_tup<u16,3,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u16,3,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u16,3,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u16,3,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u16,3,2>>( strm, this->data ); }
};

class variant_78 : public variant
{
public:
	std::vector<ctle::mn_tup<u16,3,3>> data;

	virtual ~variant_78() override {}
	virtual size_t type() const override { return 78; }
	virtual const char *type_name() const override { return "mn_tup<u16,3,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u16,3,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u16,3,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u16,3,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u16,3,3>>( strm, this->data ); }
};

class variant_79 : public variant
{
public:
	std::vector<ctle::mn_tup<u16,3,4>> data;

	virtual ~variant_79() override {}
	virtual size_t type() const override { return 79; }
	virtual const char *type_name() const override { return "mn_tup<u16,3,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u16,3,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u16,3,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u16,3,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u16,3,4>>( strm, this->data ); }
};

class variant_80 : public variant
{
public:
	std::vector<ctle::mn_tup<u16,4,1>> data;

	virtual ~variant_80() override {}
	virtual size_t type() const override { return 80; }
	virtual const char *type_name() const override { return "mn_tup<u16,4,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u16,4,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u16,4,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u16,4,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u16,4,1>>( strm, this->data ); }
};

class variant_81 : public variant
{
public:
	std::vector<ctle::mn_tup<u16,4,2>> data;

	virtual ~variant_81() override {}
	virtual size_t type() const override { return 81; }
	virtual const char *type_name() const override { return "mn_tup<u16,4,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u16,4,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u16,4,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u16,4,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u16,4,2>>( strm, this->data ); }
};

class variant_82 : public variant
{
public:
	std::vector<ctle::mn_tup<u16,4,3>> data;

	virtual ~variant_82() override {}
	virtual size_t type() const override { return 82; }
	virtual const char *type_name() const override { return "mn_tup<u16,4,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u16,4,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u16,4,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u16,4,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u16,4,3>>( strm, this->data ); }
};

class variant_83 : public variant
{
public:
	std::vector<ctle::mn_tup<u16,4,4>> data;

	virtual ~variant_83() override {}
	virtual size_t type() const override { return 83; }
	virtual const char *type_name() const override { return "mn_tup<u16,4,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u16,4,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u16,4,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u16,4,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u16,4,4>>( strm, this->data ); }
};

class variant_84 : public variant
{
public:
	std::vector<ctle::i32> data;

	virtual ~variant_84() override {}
	virtual size_t type() const override { return 84; }
	virtual const char *type_name() const override { return "i32"; }
	virtual size_t type_size() const override { return sizeof(ctle::i32); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::i32>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::i32>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::i32>( strm, this->data ); }
};

class variant_85 : public variant
{
public:
	std::vector<ctle::n_tup<i32,1>> data;

	virtual ~variant_85() override {}
	virtual size_t type() const override { return 85; }
	virtual const char *type_name() const override { return "n_tup<i32,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<i32,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<i32,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<i32,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<i32,1>>( strm, this->data ); }
};

class variant_86 : public variant
{
public:
	std::vector<ctle::n_tup<i32,2>> data;

	virtual ~variant_86() override {}
	virtual size_t type() const override { return 86; }
	virtual const char *type_name() const override { return "n_tup<i32,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<i32,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<i32,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<i32,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<i32,2>>( strm, this->data ); }
};

class variant_87 : public variant
{
public:
	std::vector<ctle::n_tup<i32,3>> data;

	virtual ~variant_87() override {}
	virtual size_t type() const override { return 87; }
	virtual const char *type_name() const override { return "n_tup<i32,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<i32,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<i32,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<i32,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<i32,3>>( strm, this->data ); }
};

class variant_88 : public variant
{
public:
	std::vector<ctle::n_tup<i32,4>> data;

	virtual ~variant_88() override {}
	virtual size_t type() const override { return 88; }
	virtual const char *type_name() const override { return "n_tup<i32,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<i32,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<i32,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<i32,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<i32,4>>( strm, this->data ); }
};

class variant_89 : public variant
{
public:
	std::vector<ctle::mn_tup<i32,1,1>> data;

	virtual ~variant_89() override {}
	virtual size_t type() const override { return 89; }
	virtual const char *type_name() const override { return "mn_tup<i32,1,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i32,1,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i32,1,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i32,1,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i32,1,1>>( strm, this->data ); }
};

class variant_90 : public variant
{
public:
	std::vector<ctle::mn_tup<i32,1,2>> data;

	virtual ~variant_90() override {}
	virtual size_t type() const override { return 90; }
	virtual const char *type_name() const override { return "mn_tup<i32,1,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i32,1,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i32,1,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i32,1,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i32,1,2>>( strm, this->data ); }
};

class variant_91 : public variant
{
public:
	std::vector<ctle::mn_tup<i32,1,3>> data;

	virtual ~variant_91() override {}
	virtual size_t type() const override { return 91; }
	virtual const char *type_name() const override { return "mn_tup<i32,1,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i32,1,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i32,1,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i32,1,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i32,1,3>>( strm, this->data ); }
};

class variant_92 : public variant
{
public:
	std::vector<ctle::mn_tup<i32,1,4>> data;

	virtual ~variant_92() override {}
	virtual size_t type() const override { return 92; }
	virtual const char *type_name() const override { return "mn_tup<i32,1,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i32,1,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i32,1,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i32,1,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i32,1,4>>( strm, this->data ); }
};

class variant_93 : public variant
{
public:
	std::vector<ctle::mn_tup<i32,2,1>> data;

	virtual ~variant_93() override {}
	virtual size_t type() const override { return 93; }
	virtual const char *type_name() const override { return "mn_tup<i32,2,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i32,2,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i32,2,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i32,2,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i32,2,1>>( strm, this->data ); }
};

class variant_94 : public variant
{
public:
	std::vector<ctle::mn_tup<i32,2,2>> data;

	virtual ~variant_94() override {}
	virtual size_t type() const override { return 94; }
	virtual const char *type_name() const override { return "mn_tup<i32,2,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i32,2,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i32,2,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i32,2,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i32,2,2>>( strm, this->data ); }
};

class variant_95 : public variant
{
public:
	std::vector<ctle::mn_tup<i32,2,3>> data;

	virtual ~variant_95() override {}
	virtual size_t type() const override { return 95; }
	virtual const char *type_name() const override { return "mn_tup<i32,2,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i32,2,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i32,2,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i32,2,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i32,2,3>>( strm, this->data ); }
};

class variant_96 : public variant
{
public:
	std::vector<ctle::mn_tup<i32,2,4>> data;

	virtual ~variant_96() override {}
	virtual size_t type() const override { return 96; }
	virtual const char *type_name() const override { return "mn_tup<i32,2,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i32,2,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i32,2,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i32,2,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i32,2,4>>( strm, this->data ); }
};

class variant_97 : public variant
{
public:
	std::vector<ctle::mn_tup<i32,3,1>> data;

	virtual ~variant_97() override {}
	virtual size_t type() const override { return 97; }
	virtual const char *type_name() const override { return "mn_tup<i32,3,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i32,3,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i32,3,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i32,3,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i32,3,1>>( strm, this->data ); }
};

class variant_98 : public variant
{
public:
	std::vector<ctle::mn_tup<i32,3,2>> data;

	virtual ~variant_98() override {}
	virtual size_t type() const override { return 98; }
	virtual const char *type_name() const override { return "mn_tup<i32,3,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i32,3,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i32,3,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i32,3,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i32,3,2>>( strm, this->data ); }
};

class variant_99 : public variant
{
public:
	std::vector<ctle::mn_tup<i32,3,3>> data;

	virtual ~variant_99() override {}
	virtual size_t type() const override { return 99; }
	virtual const char *type_name() const override { return "mn_tup<i32,3,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i32,3,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i32,3,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i32,3,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i32,3,3>>( strm, this->data ); }
};

class variant_100 : public variant
{
public:
	std::vector<ctle::mn_tup<i32,3,4>> data;

	virtual ~variant_100() override {}
	virtual size_t type() const override { return 100; }
	virtual const char *type_name() const override { return "mn_tup<i32,3,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i32,3,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i32,3,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i32,3,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i32,3,4>>( strm, this->data ); }
};

class variant_101 : public variant
{
public:
	std::vector<ctle::mn_tup<i32,4,1>> data;

	virtual ~variant_101() override {}
	virtual size_t type() const override { return 101; }
	virtual const char *type_name() const override { return "mn_tup<i32,4,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i32,4,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i32,4,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i32,4,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i32,4,1>>( strm, this->data ); }
};

class variant_102 : public variant
{
public:
	std::vector<ctle::mn_tup<i32,4,2>> data;

	virtual ~variant_102() override {}
	virtual size_t type() const override { return 102; }
	virtual const char *type_name() const override { return "mn_tup<i32,4,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i32,4,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i32,4,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i32,4,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i32,4,2>>( strm, this->data ); }
};

class variant_103 : public variant
{
public:
	std::vector<ctle::mn_tup<i32,4,3>> data;

	virtual ~variant_103() override {}
	virtual size_t type() const override { return 103; }
	virtual const char *type_name() const override { return "mn_tup<i32,4,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i32,4,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i32,4,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i32,4,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i32,4,3>>( strm, this->data ); }
};

class variant_104 : public variant
{
public:
	std::vector<ctle::mn_tup<i32,4,4>> data;

	virtual ~variant_104() override {}
	virtual size_t type() const override { return 104; }
	virtual const char *type_name() const override { return "mn_tup<i32,4,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i32,4,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i32,4,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i32,4,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i32,4,4>>( strm, this->data ); }
};

class variant_105 : public variant
{
public:
	std::vector<ctle::u32> data;

	virtual ~variant_105() override {}
	virtual size_t type() const override { return 105; }
	virtual const char *type_name() const override { return "u32"; }
	virtual size_t type_size() const override { return sizeof(ctle::u32); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::u32>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::u32>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::u32>( strm, this->data ); }
};

class variant_106 : public variant
{
public:
	std::vector<ctle::n_tup<u32,1>> data;

	virtual ~variant_106() override {}
	virtual size_t type() const override { return 106; }
	virtual const char *type_name() const override { return "n_tup<u32,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<u32,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<u32,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<u32,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<u32,1>>( strm, this->data ); }
};

class variant_107 : public variant
{
public:
	std::vector<ctle::n_tup<u32,2>> data;

	virtual ~variant_107() override {}
	virtual size_t type() const override { return 107; }
	virtual const char *type_name() const override { return "n_tup<u32,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<u32,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<u32,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<u32,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<u32,2>>( strm, this->data ); }
};

class variant_108 : public variant
{
public:
	std::vector<ctle::n_tup<u32,3>> data;

	virtual ~variant_108() override {}
	virtual size_t type() const override { return 108; }
	virtual const char *type_name() const override { return "n_tup<u32,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<u32,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<u32,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<u32,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<u32,3>>( strm, this->data ); }
};

class variant_109 : public variant
{
public:
	std::vector<ctle::n_tup<u32,4>> data;

	virtual ~variant_109() override {}
	virtual size_t type() const override { return 109; }
	virtual const char *type_name() const override { return "n_tup<u32,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<u32,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<u32,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<u32,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<u32,4>>( strm, this->data ); }
};

class variant_110 : public variant
{
public:
	std::vector<ctle::mn_tup<u32,1,1>> data;

	virtual ~variant_110() override {}
	virtual size_t type() const override { return 110; }
	virtual const char *type_name() const override { return "mn_tup<u32,1,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u32,1,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u32,1,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u32,1,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u32,1,1>>( strm, this->data ); }
};

class variant_111 : public variant
{
public:
	std::vector<ctle::mn_tup<u32,1,2>> data;

	virtual ~variant_111() override {}
	virtual size_t type() const override { return 111; }
	virtual const char *type_name() const override { return "mn_tup<u32,1,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u32,1,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u32,1,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u32,1,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u32,1,2>>( strm, this->data ); }
};

class variant_112 : public variant
{
public:
	std::vector<ctle::mn_tup<u32,1,3>> data;

	virtual ~variant_112() override {}
	virtual size_t type() const override { return 112; }
	virtual const char *type_name() const override { return "mn_tup<u32,1,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u32,1,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u32,1,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u32,1,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u32,1,3>>( strm, this->data ); }
};

class variant_113 : public variant
{
public:
	std::vector<ctle::mn_tup<u32,1,4>> data;

	virtual ~variant_113() override {}
	virtual size_t type() const override { return 113; }
	virtual const char *type_name() const override { return "mn_tup<u32,1,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u32,1,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u32,1,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u32,1,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u32,1,4>>( strm, this->data ); }
};

class variant_114 : public variant
{
public:
	std::vector<ctle::mn_tup<u32,2,1>> data;

	virtual ~variant_114() override {}
	virtual size_t type() const override { return 114; }
	virtual const char *type_name() const override { return "mn_tup<u32,2,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u32,2,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u32,2,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u32,2,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u32,2,1>>( strm, this->data ); }
};

class variant_115 : public variant
{
public:
	std::vector<ctle::mn_tup<u32,2,2>> data;

	virtual ~variant_115() override {}
	virtual size_t type() const override { return 115; }
	virtual const char *type_name() const override { return "mn_tup<u32,2,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u32,2,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u32,2,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u32,2,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u32,2,2>>( strm, this->data ); }
};

class variant_116 : public variant
{
public:
	std::vector<ctle::mn_tup<u32,2,3>> data;

	virtual ~variant_116() override {}
	virtual size_t type() const override { return 116; }
	virtual const char *type_name() const override { return "mn_tup<u32,2,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u32,2,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u32,2,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u32,2,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u32,2,3>>( strm, this->data ); }
};

class variant_117 : public variant
{
public:
	std::vector<ctle::mn_tup<u32,2,4>> data;

	virtual ~variant_117() override {}
	virtual size_t type() const override { return 117; }
	virtual const char *type_name() const override { return "mn_tup<u32,2,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u32,2,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u32,2,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u32,2,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u32,2,4>>( strm, this->data ); }
};

class variant_118 : public variant
{
public:
	std::vector<ctle::mn_tup<u32,3,1>> data;

	virtual ~variant_118() override {}
	virtual size_t type() const override { return 118; }
	virtual const char *type_name() const override { return "mn_tup<u32,3,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u32,3,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u32,3,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u32,3,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u32,3,1>>( strm, this->data ); }
};

class variant_119 : public variant
{
public:
	std::vector<ctle::mn_tup<u32,3,2>> data;

	virtual ~variant_119() override {}
	virtual size_t type() const override { return 119; }
	virtual const char *type_name() const override { return "mn_tup<u32,3,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u32,3,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u32,3,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u32,3,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u32,3,2>>( strm, this->data ); }
};

class variant_120 : public variant
{
public:
	std::vector<ctle::mn_tup<u32,3,3>> data;

	virtual ~variant_120() override {}
	virtual size_t type() const override { return 120; }
	virtual const char *type_name() const override { return "mn_tup<u32,3,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u32,3,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u32,3,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u32,3,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u32,3,3>>( strm, this->data ); }
};

class variant_121 : public variant
{
public:
	std::vector<ctle::mn_tup<u32,3,4>> data;

	virtual ~variant_121() override {}
	virtual size_t type() const override { return 121; }
	virtual const char *type_name() const override { return "mn_tup<u32,3,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u32,3,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u32,3,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u32,3,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u32,3,4>>( strm, this->data ); }
};

class variant_122 : public variant
{
public:
	std::vector<ctle::mn_tup<u32,4,1>> data;

	virtual ~variant_122() override {}
	virtual size_t type() const override { return 122; }
	virtual const char *type_name() const override { return "mn_tup<u32,4,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u32,4,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u32,4,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u32,4,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u32,4,1>>( strm, this->data ); }
};

class variant_123 : public variant
{
public:
	std::vector<ctle::mn_tup<u32,4,2>> data;

	virtual ~variant_123() override {}
	virtual size_t type() const override { return 123; }
	virtual const char *type_name() const override { return "mn_tup<u32,4,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u32,4,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u32,4,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u32,4,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u32,4,2>>( strm, this->data ); }
};

class variant_124 : public variant
{
public:
	std::vector<ctle::mn_tup<u32,4,3>> data;

	virtual ~variant_124() override {}
	virtual size_t type() const override { return 124; }
	virtual const char *type_name() const override { return "mn_tup<u32,4,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u32,4,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u32,4,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u32,4,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u32,4,3>>( strm, this->data ); }
};

class variant_125 : public variant
{
public:
	std::vector<ctle::mn_tup<u32,4,4>> data;

	virtual ~variant_125() override {}
	virtual size_t type() const override { return 125; }
	virtual const char *type_name() const override { return "mn_tup<u32,4,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u32,4,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u32,4,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u32,4,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u32,4,4>>( strm, this->data ); }
};

class variant_126 : public variant
{
public:
	std::vector<ctle::i64> data;

	virtual ~variant_126() override {}
	virtual size_t type() const override { return 126; }
	virtual const char *type_name() const override { return "i64"; }
	virtual size_t type_size() const override { return sizeof(ctle::i64); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::i64>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::i64>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::i64>( strm, this->data ); }
};

class variant_127 : public variant
{
public:
	std::vector<ctle::n_tup<i64,1>> data;

	virtual ~variant_127() override {}
	virtual size_t type() const override { return 127; }
	virtual const char *type_name() const override { return "n_tup<i64,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<i64,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<i64,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<i64,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<i64,1>>( strm, this->data ); }
};

class variant_128 : public variant
{
public:
	std::vector<ctle::n_tup<i64,2>> data;

	virtual ~variant_128() override {}
	virtual size_t type() const override { return 128; }
	virtual const char *type_name() const override { return "n_tup<i64,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<i64,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<i64,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<i64,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<i64,2>>( strm, this->data ); }
};

class variant_129 : public variant
{
public:
	std::vector<ctle::n_tup<i64,3>> data;

	virtual ~variant_129() override {}
	virtual size_t type() const override { return 129; }
	virtual const char *type_name() const override { return "n_tup<i64,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<i64,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<i64,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<i64,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<i64,3>>( strm, this->data ); }
};

class variant_130 : public variant
{
public:
	std::vector<ctle::n_tup<i64,4>> data;

	virtual ~variant_130() override {}
	virtual size_t type() const override { return 130; }
	virtual const char *type_name() const override { return "n_tup<i64,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<i64,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<i64,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<i64,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<i64,4>>( strm, this->data ); }
};

class variant_131 : public variant
{
public:
	std::vector<ctle::mn_tup<i64,1,1>> data;

	virtual ~variant_131() override {}
	virtual size_t type() const override { return 131; }
	virtual const char *type_name() const override { return "mn_tup<i64,1,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i64,1,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i64,1,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i64,1,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i64,1,1>>( strm, this->data ); }
};

class variant_132 : public variant
{
public:
	std::vector<ctle::mn_tup<i64,1,2>> data;

	virtual ~variant_132() override {}
	virtual size_t type() const override { return 132; }
	virtual const char *type_name() const override { return "mn_tup<i64,1,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i64,1,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i64,1,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i64,1,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i64,1,2>>( strm, this->data ); }
};

class variant_133 : public variant
{
public:
	std::vector<ctle::mn_tup<i64,1,3>> data;

	virtual ~variant_133() override {}
	virtual size_t type() const override { return 133; }
	virtual const char *type_name() const override { return "mn_tup<i64,1,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i64,1,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i64,1,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i64,1,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i64,1,3>>( strm, this->data ); }
};

class variant_134 : public variant
{
public:
	std::vector<ctle::mn_tup<i64,1,4>> data;

	virtual ~variant_134() override {}
	virtual size_t type() const override { return 134; }
	virtual const char *type_name() const override { return "mn_tup<i64,1,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i64,1,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i64,1,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i64,1,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i64,1,4>>( strm, this->data ); }
};

class variant_135 : public variant
{
public:
	std::vector<ctle::mn_tup<i64,2,1>> data;

	virtual ~variant_135() override {}
	virtual size_t type() const override { return 135; }
	virtual const char *type_name() const override { return "mn_tup<i64,2,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i64,2,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i64,2,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i64,2,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i64,2,1>>( strm, this->data ); }
};

class variant_136 : public variant
{
public:
	std::vector<ctle::mn_tup<i64,2,2>> data;

	virtual ~variant_136() override {}
	virtual size_t type() const override { return 136; }
	virtual const char *type_name() const override { return "mn_tup<i64,2,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i64,2,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i64,2,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i64,2,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i64,2,2>>( strm, this->data ); }
};

class variant_137 : public variant
{
public:
	std::vector<ctle::mn_tup<i64,2,3>> data;

	virtual ~variant_137() override {}
	virtual size_t type() const override { return 137; }
	virtual const char *type_name() const override { return "mn_tup<i64,2,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i64,2,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i64,2,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i64,2,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i64,2,3>>( strm, this->data ); }
};

class variant_138 : public variant
{
public:
	std::vector<ctle::mn_tup<i64,2,4>> data;

	virtual ~variant_138() override {}
	virtual size_t type() const override { return 138; }
	virtual const char *type_name() const override { return "mn_tup<i64,2,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i64,2,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i64,2,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i64,2,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i64,2,4>>( strm, this->data ); }
};

class variant_139 : public variant
{
public:
	std::vector<ctle::mn_tup<i64,3,1>> data;

	virtual ~variant_139() override {}
	virtual size_t type() const override { return 139; }
	virtual const char *type_name() const override { return "mn_tup<i64,3,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i64,3,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i64,3,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i64,3,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i64,3,1>>( strm, this->data ); }
};

class variant_140 : public variant
{
public:
	std::vector<ctle::mn_tup<i64,3,2>> data;

	virtual ~variant_140() override {}
	virtual size_t type() const override { return 140; }
	virtual const char *type_name() const override { return "mn_tup<i64,3,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i64,3,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i64,3,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i64,3,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i64,3,2>>( strm, this->data ); }
};

class variant_141 : public variant
{
public:
	std::vector<ctle::mn_tup<i64,3,3>> data;

	virtual ~variant_141() override {}
	virtual size_t type() const override { return 141; }
	virtual const char *type_name() const override { return "mn_tup<i64,3,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i64,3,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i64,3,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i64,3,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i64,3,3>>( strm, this->data ); }
};

class variant_142 : public variant
{
public:
	std::vector<ctle::mn_tup<i64,3,4>> data;

	virtual ~variant_142() override {}
	virtual size_t type() const override { return 142; }
	virtual const char *type_name() const override { return "mn_tup<i64,3,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i64,3,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i64,3,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i64,3,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i64,3,4>>( strm, this->data ); }
};

class variant_143 : public variant
{
public:
	std::vector<ctle::mn_tup<i64,4,1>> data;

	virtual ~variant_143() override {}
	virtual size_t type() const override { return 143; }
	virtual const char *type_name() const override { return "mn_tup<i64,4,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i64,4,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i64,4,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i64,4,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i64,4,1>>( strm, this->data ); }
};

class variant_144 : public variant
{
public:
	std::vector<ctle::mn_tup<i64,4,2>> data;

	virtual ~variant_144() override {}
	virtual size_t type() const override { return 144; }
	virtual const char *type_name() const override { return "mn_tup<i64,4,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i64,4,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i64,4,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i64,4,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i64,4,2>>( strm, this->data ); }
};

class variant_145 : public variant
{
public:
	std::vector<ctle::mn_tup<i64,4,3>> data;

	virtual ~variant_145() override {}
	virtual size_t type() const override { return 145; }
	virtual const char *type_name() const override { return "mn_tup<i64,4,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i64,4,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i64,4,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i64,4,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i64,4,3>>( strm, this->data ); }
};

class variant_146 : public variant
{
public:
	std::vector<ctle::mn_tup<i64,4,4>> data;

	virtual ~variant_146() override {}
	virtual size_t type() const override { return 146; }
	virtual const char *type_name() const override { return "mn_tup<i64,4,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<i64,4,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<i64,4,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<i64,4,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<i64,4,4>>( strm, this->data ); }
};

class variant_147 : public variant
{
public:
	std::vector<ctle::u64> data;

	virtual ~variant_147() override {}
	virtual size_t type() const override { return 147; }
	virtual const char *type_name() const override { return "u64"; }
	virtual size_t type_size() const override { return sizeof(ctle::u64); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::u64>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::u64>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::u64>( strm, this->data ); }
};

class variant_148 : public variant
{
public:
	std::vector<ctle::n_tup<u64,1>> data;

	virtual ~variant_148() override {}
	virtual size_t type() const override { return 148; }
	virtual const char *type_name() const override { return "n_tup<u64,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<u64,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<u64,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<u64,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<u64,1>>( strm, this->data ); }
};

class variant_149 : public variant
{
public:
	std::vector<ctle::n_tup<u64,2>> data;

	virtual ~variant_149() override {}
	virtual size_t type() const override { return 149; }
	virtual const char *type_name() const override { return "n_tup<u64,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<u64,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<u64,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<u64,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<u64,2>>( strm, this->data ); }
};

class variant_150 : public variant
{
public:
	std::vector<ctle::n_tup<u64,3>> data;

	virtual ~variant_150() override {}
	virtual size_t type() const override { return 150; }
	virtual const char *type_name() const override { return "n_tup<u64,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<u64,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<u64,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<u64,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<u64,3>>( strm, this->data ); }
};

class variant_151 : public variant
{
public:
	std::vector<ctle::n_tup<u64,4>> data;

	virtual ~variant_151() override {}
	virtual size_t type() const override { return 151; }
	virtual const char *type_name() const override { return "n_tup<u64,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<u64,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<u64,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<u64,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<u64,4>>( strm, this->data ); }
};

class variant_152 : public variant
{
public:
	std::vector<ctle::mn_tup<u64,1,1>> data;

	virtual ~variant_152() override {}
	virtual size_t type() const override { return 152; }
	virtual const char *type_name() const override { return "mn_tup<u64,1,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u64,1,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u64,1,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u64,1,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u64,1,1>>( strm, this->data ); }
};

class variant_153 : public variant
{
public:
	std::vector<ctle::mn_tup<u64,1,2>> data;

	virtual ~variant_153() override {}
	virtual size_t type() const override { return 153; }
	virtual const char *type_name() const override { return "mn_tup<u64,1,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u64,1,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u64,1,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u64,1,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u64,1,2>>( strm, this->data ); }
};

class variant_154 : public variant
{
public:
	std::vector<ctle::mn_tup<u64,1,3>> data;

	virtual ~variant_154() override {}
	virtual size_t type() const override { return 154; }
	virtual const char *type_name() const override { return "mn_tup<u64,1,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u64,1,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u64,1,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u64,1,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u64,1,3>>( strm, this->data ); }
};

class variant_155 : public variant
{
public:
	std::vector<ctle::mn_tup<u64,1,4>> data;

	virtual ~variant_155() override {}
	virtual size_t type() const override { return 155; }
	virtual const char *type_name() const override { return "mn_tup<u64,1,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u64,1,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u64,1,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u64,1,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u64,1,4>>( strm, this->data ); }
};

class variant_156 : public variant
{
public:
	std::vector<ctle::mn_tup<u64,2,1>> data;

	virtual ~variant_156() override {}
	virtual size_t type() const override { return 156; }
	virtual const char *type_name() const override { return "mn_tup<u64,2,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u64,2,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u64,2,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u64,2,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u64,2,1>>( strm, this->data ); }
};

class variant_157 : public variant
{
public:
	std::vector<ctle::mn_tup<u64,2,2>> data;

	virtual ~variant_157() override {}
	virtual size_t type() const override { return 157; }
	virtual const char *type_name() const override { return "mn_tup<u64,2,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u64,2,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u64,2,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u64,2,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u64,2,2>>( strm, this->data ); }
};

class variant_158 : public variant
{
public:
	std::vector<ctle::mn_tup<u64,2,3>> data;

	virtual ~variant_158() override {}
	virtual size_t type() const override { return 158; }
	virtual const char *type_name() const override { return "mn_tup<u64,2,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u64,2,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u64,2,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u64,2,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u64,2,3>>( strm, this->data ); }
};

class variant_159 : public variant
{
public:
	std::vector<ctle::mn_tup<u64,2,4>> data;

	virtual ~variant_159() override {}
	virtual size_t type() const override { return 159; }
	virtual const char *type_name() const override { return "mn_tup<u64,2,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u64,2,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u64,2,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u64,2,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u64,2,4>>( strm, this->data ); }
};

class variant_160 : public variant
{
public:
	std::vector<ctle::mn_tup<u64,3,1>> data;

	virtual ~variant_160() override {}
	virtual size_t type() const override { return 160; }
	virtual const char *type_name() const override { return "mn_tup<u64,3,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u64,3,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u64,3,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u64,3,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u64,3,1>>( strm, this->data ); }
};

class variant_161 : public variant
{
public:
	std::vector<ctle::mn_tup<u64,3,2>> data;

	virtual ~variant_161() override {}
	virtual size_t type() const override { return 161; }
	virtual const char *type_name() const override { return "mn_tup<u64,3,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u64,3,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u64,3,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u64,3,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u64,3,2>>( strm, this->data ); }
};

class variant_162 : public variant
{
public:
	std::vector<ctle::mn_tup<u64,3,3>> data;

	virtual ~variant_162() override {}
	virtual size_t type() const override { return 162; }
	virtual const char *type_name() const override { return "mn_tup<u64,3,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u64,3,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u64,3,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u64,3,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u64,3,3>>( strm, this->data ); }
};

class variant_163 : public variant
{
public:
	std::vector<ctle::mn_tup<u64,3,4>> data;

	virtual ~variant_163() override {}
	virtual size_t type() const override { return 163; }
	virtual const char *type_name() const override { return "mn_tup<u64,3,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u64,3,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u64,3,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u64,3,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u64,3,4>>( strm, this->data ); }
};

class variant_164 : public variant
{
public:
	std::vector<ctle::mn_tup<u64,4,1>> data;

	virtual ~variant_164() override {}
	virtual size_t type() const override { return 164; }
	virtual const char *type_name() const override { return "mn_tup<u64,4,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u64,4,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u64,4,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u64,4,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u64,4,1>>( strm, this->data ); }
};

class variant_165 : public variant
{
public:
	std::vector<ctle::mn_tup<u64,4,2>> data;

	virtual ~variant_165() override {}
	virtual size_t type() const override { return 165; }
	virtual const char *type_name() const override { return "mn_tup<u64,4,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u64,4,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u64,4,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u64,4,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u64,4,2>>( strm, this->data ); }
};

class variant_166 : public variant
{
public:
	std::vector<ctle::mn_tup<u64,4,3>> data;

	virtual ~variant_166() override {}
	virtual size_t type() const override { return 166; }
	virtual const char *type_name() const override { return "mn_tup<u64,4,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u64,4,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u64,4,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u64,4,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u64,4,3>>( strm, this->data ); }
};

class variant_167 : public variant
{
public:
	std::vector<ctle::mn_tup<u64,4,4>> data;

	virtual ~variant_167() override {}
	virtual size_t type() const override { return 167; }
	virtual const char *type_name() const override { return "mn_tup<u64,4,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<u64,4,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<u64,4,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<u64,4,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<u64,4,4>>( strm, this->data ); }
};

class variant_168 : public variant
{
public:
	std::vector<ctle::f32> data;

	virtual ~variant_168() override {}
	virtual size_t type() const override { return 168; }
	virtual const char *type_name() const override { return "f32"; }
	virtual size_t type_size() const override { return sizeof(ctle::f32); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::f32>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::f32>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::f32>( strm, this->data ); }
};

class variant_169 : public variant
{
public:
	std::vector<ctle::n_tup<f32,1>> data;

	virtual ~variant_169() override {}
	virtual size_t type() const override { return 169; }
	virtual const char *type_name() const override { return "n_tup<f32,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<f32,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<f32,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<f32,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<f32,1>>( strm, this->data ); }
};

class variant_170 : public variant
{
public:
	std::vector<ctle::n_tup<f32,2>> data;

	virtual ~variant_170() override {}
	virtual size_t type() const override { return 170; }
	virtual const char *type_name() const override { return "n_tup<f32,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<f32,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<f32,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<f32,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<f32,2>>( strm, this->data ); }
};

class variant_171 : public variant
{
public:
	std::vector<ctle::n_tup<f32,3>> data;

	virtual ~variant_171() override {}
	virtual size_t type() const override { return 171; }
	virtual const char *type_name() const override { return "n_tup<f32,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<f32,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<f32,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<f32,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<f32,3>>( strm, this->data ); }
};

class variant_172 : public variant
{
public:
	std::vector<ctle::n_tup<f32,4>> data;

	virtual ~variant_172() override {}
	virtual size_t type() const override { return 172; }
	virtual const char *type_name() const override { return "n_tup<f32,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<f32,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<f32,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<f32,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<f32,4>>( strm, this->data ); }
};

class variant_173 : public variant
{
public:
	std::vector<ctle::mn_tup<f32,1,1>> data;

	virtual ~variant_173() override {}
	virtual size_t type() const override { return 173; }
	virtual const char *type_name() const override { return "mn_tup<f32,1,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f32,1,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f32,1,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f32,1,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f32,1,1>>( strm, this->data ); }
};

class variant_174 : public variant
{
public:
	std::vector<ctle::mn_tup<f32,1,2>> data;

	virtual ~variant_174() override {}
	virtual size_t type() const override { return 174; }
	virtual const char *type_name() const override { return "mn_tup<f32,1,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f32,1,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f32,1,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f32,1,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f32,1,2>>( strm, this->data ); }
};

class variant_175 : public variant
{
public:
	std::vector<ctle::mn_tup<f32,1,3>> data;

	virtual ~variant_175() override {}
	virtual size_t type() const override { return 175; }
	virtual const char *type_name() const override { return "mn_tup<f32,1,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f32,1,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f32,1,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f32,1,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f32,1,3>>( strm, this->data ); }
};

class variant_176 : public variant
{
public:
	std::vector<ctle::mn_tup<f32,1,4>> data;

	virtual ~variant_176() override {}
	virtual size_t type() const override { return 176; }
	virtual const char *type_name() const override { return "mn_tup<f32,1,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f32,1,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f32,1,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f32,1,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f32,1,4>>( strm, this->data ); }
};

class variant_177 : public variant
{
public:
	std::vector<ctle::mn_tup<f32,2,1>> data;

	virtual ~variant_177() override {}
	virtual size_t type() const override { return 177; }
	virtual const char *type_name() const override { return "mn_tup<f32,2,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f32,2,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f32,2,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f32,2,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f32,2,1>>( strm, this->data ); }
};

class variant_178 : public variant
{
public:
	std::vector<ctle::mn_tup<f32,2,2>> data;

	virtual ~variant_178() override {}
	virtual size_t type() const override { return 178; }
	virtual const char *type_name() const override { return "mn_tup<f32,2,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f32,2,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f32,2,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f32,2,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f32,2,2>>( strm, this->data ); }
};

class variant_179 : public variant
{
public:
	std::vector<ctle::mn_tup<f32,2,3>> data;

	virtual ~variant_179() override {}
	virtual size_t type() const override { return 179; }
	virtual const char *type_name() const override { return "mn_tup<f32,2,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f32,2,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f32,2,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f32,2,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f32,2,3>>( strm, this->data ); }
};

class variant_180 : public variant
{
public:
	std::vector<ctle::mn_tup<f32,2,4>> data;

	virtual ~variant_180() override {}
	virtual size_t type() const override { return 180; }
	virtual const char *type_name() const override { return "mn_tup<f32,2,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f32,2,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f32,2,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f32,2,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f32,2,4>>( strm, this->data ); }
};

class variant_181 : public variant
{
public:
	std::vector<ctle::mn_tup<f32,3,1>> data;

	virtual ~variant_181() override {}
	virtual size_t type() const override { return 181; }
	virtual const char *type_name() const override { return "mn_tup<f32,3,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f32,3,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f32,3,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f32,3,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f32,3,1>>( strm, this->data ); }
};

class variant_182 : public variant
{
public:
	std::vector<ctle::mn_tup<f32,3,2>> data;

	virtual ~variant_182() override {}
	virtual size_t type() const override { return 182; }
	virtual const char *type_name() const override { return "mn_tup<f32,3,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f32,3,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f32,3,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f32,3,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f32,3,2>>( strm, this->data ); }
};

class variant_183 : public variant
{
public:
	std::vector<ctle::mn_tup<f32,3,3>> data;

	virtual ~variant_183() override {}
	virtual size_t type() const override { return 183; }
	virtual const char *type_name() const override { return "mn_tup<f32,3,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f32,3,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f32,3,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f32,3,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f32,3,3>>( strm, this->data ); }
};

class variant_184 : public variant
{
public:
	std::vector<ctle::mn_tup<f32,3,4>> data;

	virtual ~variant_184() override {}
	virtual size_t type() const override { return 184; }
	virtual const char *type_name() const override { return "mn_tup<f32,3,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f32,3,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f32,3,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f32,3,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f32,3,4>>( strm, this->data ); }
};

class variant_185 : public variant
{
public:
	std::vector<ctle::mn_tup<f32,4,1>> data;

	virtual ~variant_185() override {}
	virtual size_t type() const override { return 185; }
	virtual const char *type_name() const override { return "mn_tup<f32,4,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f32,4,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f32,4,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f32,4,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f32,4,1>>( strm, this->data ); }
};

class variant_186 : public variant
{
public:
	std::vector<ctle::mn_tup<f32,4,2>> data;

	virtual ~variant_186() override {}
	virtual size_t type() const override { return 186; }
	virtual const char *type_name() const override { return "mn_tup<f32,4,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f32,4,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f32,4,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f32,4,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f32,4,2>>( strm, this->data ); }
};

class variant_187 : public variant
{
public:
	std::vector<ctle::mn_tup<f32,4,3>> data;

	virtual ~variant_187() override {}
	virtual size_t type() const override { return 187; }
	virtual const char *type_name() const override { return "mn_tup<f32,4,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f32,4,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f32,4,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f32,4,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f32,4,3>>( strm, this->data ); }
};

class variant_188 : public variant
{
public:
	std::vector<ctle::mn_tup<f32,4,4>> data;

	virtual ~variant_188() override {}
	virtual size_t type() const override { return 188; }
	virtual const char *type_name() const override { return "mn_tup<f32,4,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f32,4,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f32,4,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f32,4,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f32,4,4>>( strm, this->data ); }
};

class variant_189 : public variant
{
public:
	std::vector<ctle::f64> data;

	virtual ~variant_189() override {}
	virtual size_t type() const override { return 189; }
	virtual const char *type_name() const override { return "f64"; }
	virtual size_t type_size() const override { return sizeof(ctle::f64); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::f64>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::f64>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::f64>( strm, this->data ); }
};

class variant_190 : public variant
{
public:
	std::vector<ctle::n_tup<f64,1>> data;

	virtual ~variant_190() override {}
	virtual size_t type() const override { return 190; }
	virtual const char *type_name() const override { return "n_tup<f64,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<f64,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<f64,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<f64,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<f64,1>>( strm, this->data ); }
};

class variant_191 : public variant
{
public:
	std::vector<ctle::n_tup<f64,2>> data;

	virtual ~variant_191() override {}
	virtual size_t type() const override { return 191; }
	virtual const char *type_name() const override { return "n_tup<f64,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<f64,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<f64,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<f64,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<f64,2>>( strm, this->data ); }
};

class variant_192 : public variant
{
public:
	std::vector<ctle::n_tup<f64,3>> data;

	virtual ~variant_192() override {}
	virtual size_t type() const override { return 192; }
	virtual const char *type_name() const override { return "n_tup<f64,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<f64,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<f64,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<f64,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<f64,3>>( strm, this->data ); }
};

class variant_193 : public variant
{
public:
	std::vector<ctle::n_tup<f64,4>> data;

	virtual ~variant_193() override {}
	virtual size_t type() const override { return 193; }
	virtual const char *type_name() const override { return "n_tup<f64,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::n_tup<f64,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::n_tup<f64,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::n_tup<f64,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::n_tup<f64,4>>( strm, this->data ); }
};

class variant_194 : public variant
{
public:
	std::vector<ctle::mn_tup<f64,1,1>> data;

	virtual ~variant_194() override {}
	virtual size_t type() const override { return 194; }
	virtual const char *type_name() const override { return "mn_tup<f64,1,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f64,1,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f64,1,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f64,1,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f64,1,1>>( strm, this->data ); }
};

class variant_195 : public variant
{
public:
	std::vector<ctle::mn_tup<f64,1,2>> data;

	virtual ~variant_195() override {}
	virtual size_t type() const override { return 195; }
	virtual const char *type_name() const override { return "mn_tup<f64,1,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f64,1,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f64,1,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f64,1,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f64,1,2>>( strm, this->data ); }
};

class variant_196 : public variant
{
public:
	std::vector<ctle::mn_tup<f64,1,3>> data;

	virtual ~variant_196() override {}
	virtual size_t type() const override { return 196; }
	virtual const char *type_name() const override { return "mn_tup<f64,1,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f64,1,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f64,1,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f64,1,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f64,1,3>>( strm, this->data ); }
};

class variant_197 : public variant
{
public:
	std::vector<ctle::mn_tup<f64,1,4>> data;

	virtual ~variant_197() override {}
	virtual size_t type() const override { return 197; }
	virtual const char *type_name() const override { return "mn_tup<f64,1,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f64,1,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f64,1,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f64,1,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f64,1,4>>( strm, this->data ); }
};

class variant_198 : public variant
{
public:
	std::vector<ctle::mn_tup<f64,2,1>> data;

	virtual ~variant_198() override {}
	virtual size_t type() const override { return 198; }
	virtual const char *type_name() const override { return "mn_tup<f64,2,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f64,2,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f64,2,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f64,2,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f64,2,1>>( strm, this->data ); }
};

class variant_199 : public variant
{
public:
	std::vector<ctle::mn_tup<f64,2,2>> data;

	virtual ~variant_199() override {}
	virtual size_t type() const override { return 199; }
	virtual const char *type_name() const override { return "mn_tup<f64,2,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f64,2,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f64,2,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f64,2,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f64,2,2>>( strm, this->data ); }
};

class variant_200 : public variant
{
public:
	std::vector<ctle::mn_tup<f64,2,3>> data;

	virtual ~variant_200() override {}
	virtual size_t type() const override { return 200; }
	virtual const char *type_name() const override { return "mn_tup<f64,2,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f64,2,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f64,2,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f64,2,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f64,2,3>>( strm, this->data ); }
};

class variant_201 : public variant
{
public:
	std::vector<ctle::mn_tup<f64,2,4>> data;

	virtual ~variant_201() override {}
	virtual size_t type() const override { return 201; }
	virtual const char *type_name() const override { return "mn_tup<f64,2,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f64,2,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f64,2,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f64,2,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f64,2,4>>( strm, this->data ); }
};

class variant_202 : public variant
{
public:
	std::vector<ctle::mn_tup<f64,3,1>> data;

	virtual ~variant_202() override {}
	virtual size_t type() const override { return 202; }
	virtual const char *type_name() const override { return "mn_tup<f64,3,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f64,3,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f64,3,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f64,3,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f64,3,1>>( strm, this->data ); }
};

class variant_203 : public variant
{
public:
	std::vector<ctle::mn_tup<f64,3,2>> data;

	virtual ~variant_203() override {}
	virtual size_t type() const override { return 203; }
	virtual const char *type_name() const override { return "mn_tup<f64,3,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f64,3,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f64,3,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f64,3,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f64,3,2>>( strm, this->data ); }
};

class variant_204 : public variant
{
public:
	std::vector<ctle::mn_tup<f64,3,3>> data;

	virtual ~variant_204() override {}
	virtual size_t type() const override { return 204; }
	virtual const char *type_name() const override { return "mn_tup<f64,3,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f64,3,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f64,3,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f64,3,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f64,3,3>>( strm, this->data ); }
};

class variant_205 : public variant
{
public:
	std::vector<ctle::mn_tup<f64,3,4>> data;

	virtual ~variant_205() override {}
	virtual size_t type() const override { return 205; }
	virtual const char *type_name() const override { return "mn_tup<f64,3,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f64,3,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f64,3,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f64,3,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f64,3,4>>( strm, this->data ); }
};

class variant_206 : public variant
{
public:
	std::vector<ctle::mn_tup<f64,4,1>> data;

	virtual ~variant_206() override {}
	virtual size_t type() const override { return 206; }
	virtual const char *type_name() const override { return "mn_tup<f64,4,1>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f64,4,1>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f64,4,1>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f64,4,1>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f64,4,1>>( strm, this->data ); }
};

class variant_207 : public variant
{
public:
	std::vector<ctle::mn_tup<f64,4,2>> data;

	virtual ~variant_207() override {}
	virtual size_t type() const override { return 207; }
	virtual const char *type_name() const override { return "mn_tup<f64,4,2>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f64,4,2>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f64,4,2>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f64,4,2>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f64,4,2>>( strm, this->data ); }
};

class variant_208 : public variant
{
public:
	std::vector<ctle::mn_tup<f64,4,3>> data;

	virtual ~variant_208() override {}
	virtual size_t type() const override { return 208; }
	virtual const char *type_name() const override { return "mn_tup<f64,4,3>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f64,4,3>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f64,4,3>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f64,4,3>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f64,4,3>>( strm, this->data ); }
};

class variant_209 : public variant
{
public:
	std::vector<ctle::mn_tup<f64,4,4>> data;

	virtual ~variant_209() override {}
	virtual size_t type() const override { return 209; }
	virtual const char *type_name() const override { return "mn_tup<f64,4,4>"; }
	virtual size_t type_size() const override { return sizeof(ctle::mn_tup<f64,4,4>); }
	virtual size_t type_count() const override { return this->data.size(); }
	virtual bool is_equal( const variant &other ) const override { return _are_equal( *this, other ); }
	virtual void clear() override { this->data.clear(); }
	virtual const void *cdata() const override { return this->data.data(); }
	virtual void random() { this->data = random_vector<ctle::mn_tup<f64,4,4>>(); }
	virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override { return _read_from_stream<ctle::mn_tup<f64,4,4>>( strm, this->data ); }
	virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override { return _write_to_stream<ctle::mn_tup<f64,4,4>>( strm, this->data ); }
};


std::unique_ptr<variant> random_variant()
{
	return new_variant( rand() % 210 );
}
std::unique_ptr<variant> new_variant( size_t inx )
{
	switch( inx )
	{
		case 0: return std::make_unique<variant_0>();
		case 1: return std::make_unique<variant_1>();
		case 2: return std::make_unique<variant_2>();
		case 3: return std::make_unique<variant_3>();
		case 4: return std::make_unique<variant_4>();
		case 5: return std::make_unique<variant_5>();
		case 6: return std::make_unique<variant_6>();
		case 7: return std::make_unique<variant_7>();
		case 8: return std::make_unique<variant_8>();
		case 9: return std::make_unique<variant_9>();
		case 10: return std::make_unique<variant_10>();
		case 11: return std::make_unique<variant_11>();
		case 12: return std::make_unique<variant_12>();
		case 13: return std::make_unique<variant_13>();
		case 14: return std::make_unique<variant_14>();
		case 15: return std::make_unique<variant_15>();
		case 16: return std::make_unique<variant_16>();
		case 17: return std::make_unique<variant_17>();
		case 18: return std::make_unique<variant_18>();
		case 19: return std::make_unique<variant_19>();
		case 20: return std::make_unique<variant_20>();
		case 21: return std::make_unique<variant_21>();
		case 22: return std::make_unique<variant_22>();
		case 23: return std::make_unique<variant_23>();
		case 24: return std::make_unique<variant_24>();
		case 25: return std::make_unique<variant_25>();
		case 26: return std::make_unique<variant_26>();
		case 27: return std::make_unique<variant_27>();
		case 28: return std::make_unique<variant_28>();
		case 29: return std::make_unique<variant_29>();
		case 30: return std::make_unique<variant_30>();
		case 31: return std::make_unique<variant_31>();
		case 32: return std::make_unique<variant_32>();
		case 33: return std::make_unique<variant_33>();
		case 34: return std::make_unique<variant_34>();
		case 35: return std::make_unique<variant_35>();
		case 36: return std::make_unique<variant_36>();
		case 37: return std::make_unique<variant_37>();
		case 38: return std::make_unique<variant_38>();
		case 39: return std::make_unique<variant_39>();
		case 40: return std::make_unique<variant_40>();
		case 41: return std::make_unique<variant_41>();
		case 42: return std::make_unique<variant_42>();
		case 43: return std::make_unique<variant_43>();
		case 44: return std::make_unique<variant_44>();
		case 45: return std::make_unique<variant_45>();
		case 46: return std::make_unique<variant_46>();
		case 47: return std::make_unique<variant_47>();
		case 48: return std::make_unique<variant_48>();
		case 49: return std::make_unique<variant_49>();
		case 50: return std::make_unique<variant_50>();
		case 51: return std::make_unique<variant_51>();
		case 52: return std::make_unique<variant_52>();
		case 53: return std::make_unique<variant_53>();
		case 54: return std::make_unique<variant_54>();
		case 55: return std::make_unique<variant_55>();
		case 56: return std::make_unique<variant_56>();
		case 57: return std::make_unique<variant_57>();
		case 58: return std::make_unique<variant_58>();
		case 59: return std::make_unique<variant_59>();
		case 60: return std::make_unique<variant_60>();
		case 61: return std::make_unique<variant_61>();
		case 62: return std::make_unique<variant_62>();
		case 63: return std::make_unique<variant_63>();
		case 64: return std::make_unique<variant_64>();
		case 65: return std::make_unique<variant_65>();
		case 66: return std::make_unique<variant_66>();
		case 67: return std::make_unique<variant_67>();
		case 68: return std::make_unique<variant_68>();
		case 69: return std::make_unique<variant_69>();
		case 70: return std::make_unique<variant_70>();
		case 71: return std::make_unique<variant_71>();
		case 72: return std::make_unique<variant_72>();
		case 73: return std::make_unique<variant_73>();
		case 74: return std::make_unique<variant_74>();
		case 75: return std::make_unique<variant_75>();
		case 76: return std::make_unique<variant_76>();
		case 77: return std::make_unique<variant_77>();
		case 78: return std::make_unique<variant_78>();
		case 79: return std::make_unique<variant_79>();
		case 80: return std::make_unique<variant_80>();
		case 81: return std::make_unique<variant_81>();
		case 82: return std::make_unique<variant_82>();
		case 83: return std::make_unique<variant_83>();
		case 84: return std::make_unique<variant_84>();
		case 85: return std::make_unique<variant_85>();
		case 86: return std::make_unique<variant_86>();
		case 87: return std::make_unique<variant_87>();
		case 88: return std::make_unique<variant_88>();
		case 89: return std::make_unique<variant_89>();
		case 90: return std::make_unique<variant_90>();
		case 91: return std::make_unique<variant_91>();
		case 92: return std::make_unique<variant_92>();
		case 93: return std::make_unique<variant_93>();
		case 94: return std::make_unique<variant_94>();
		case 95: return std::make_unique<variant_95>();
		case 96: return std::make_unique<variant_96>();
		case 97: return std::make_unique<variant_97>();
		case 98: return std::make_unique<variant_98>();
		case 99: return std::make_unique<variant_99>();
		case 100: return std::make_unique<variant_100>();
		case 101: return std::make_unique<variant_101>();
		case 102: return std::make_unique<variant_102>();
		case 103: return std::make_unique<variant_103>();
		case 104: return std::make_unique<variant_104>();
		case 105: return std::make_unique<variant_105>();
		case 106: return std::make_unique<variant_106>();
		case 107: return std::make_unique<variant_107>();
		case 108: return std::make_unique<variant_108>();
		case 109: return std::make_unique<variant_109>();
		case 110: return std::make_unique<variant_110>();
		case 111: return std::make_unique<variant_111>();
		case 112: return std::make_unique<variant_112>();
		case 113: return std::make_unique<variant_113>();
		case 114: return std::make_unique<variant_114>();
		case 115: return std::make_unique<variant_115>();
		case 116: return std::make_unique<variant_116>();
		case 117: return std::make_unique<variant_117>();
		case 118: return std::make_unique<variant_118>();
		case 119: return std::make_unique<variant_119>();
		case 120: return std::make_unique<variant_120>();
		case 121: return std::make_unique<variant_121>();
		case 122: return std::make_unique<variant_122>();
		case 123: return std::make_unique<variant_123>();
		case 124: return std::make_unique<variant_124>();
		case 125: return std::make_unique<variant_125>();
		case 126: return std::make_unique<variant_126>();
		case 127: return std::make_unique<variant_127>();
		case 128: return std::make_unique<variant_128>();
		case 129: return std::make_unique<variant_129>();
		case 130: return std::make_unique<variant_130>();
		case 131: return std::make_unique<variant_131>();
		case 132: return std::make_unique<variant_132>();
		case 133: return std::make_unique<variant_133>();
		case 134: return std::make_unique<variant_134>();
		case 135: return std::make_unique<variant_135>();
		case 136: return std::make_unique<variant_136>();
		case 137: return std::make_unique<variant_137>();
		case 138: return std::make_unique<variant_138>();
		case 139: return std::make_unique<variant_139>();
		case 140: return std::make_unique<variant_140>();
		case 141: return std::make_unique<variant_141>();
		case 142: return std::make_unique<variant_142>();
		case 143: return std::make_unique<variant_143>();
		case 144: return std::make_unique<variant_144>();
		case 145: return std::make_unique<variant_145>();
		case 146: return std::make_unique<variant_146>();
		case 147: return std::make_unique<variant_147>();
		case 148: return std::make_unique<variant_148>();
		case 149: return std::make_unique<variant_149>();
		case 150: return std::make_unique<variant_150>();
		case 151: return std::make_unique<variant_151>();
		case 152: return std::make_unique<variant_152>();
		case 153: return std::make_unique<variant_153>();
		case 154: return std::make_unique<variant_154>();
		case 155: return std::make_unique<variant_155>();
		case 156: return std::make_unique<variant_156>();
		case 157: return std::make_unique<variant_157>();
		case 158: return std::make_unique<variant_158>();
		case 159: return std::make_unique<variant_159>();
		case 160: return std::make_unique<variant_160>();
		case 161: return std::make_unique<variant_161>();
		case 162: return std::make_unique<variant_162>();
		case 163: return std::make_unique<variant_163>();
		case 164: return std::make_unique<variant_164>();
		case 165: return std::make_unique<variant_165>();
		case 166: return std::make_unique<variant_166>();
		case 167: return std::make_unique<variant_167>();
		case 168: return std::make_unique<variant_168>();
		case 169: return std::make_unique<variant_169>();
		case 170: return std::make_unique<variant_170>();
		case 171: return std::make_unique<variant_171>();
		case 172: return std::make_unique<variant_172>();
		case 173: return std::make_unique<variant_173>();
		case 174: return std::make_unique<variant_174>();
		case 175: return std::make_unique<variant_175>();
		case 176: return std::make_unique<variant_176>();
		case 177: return std::make_unique<variant_177>();
		case 178: return std::make_unique<variant_178>();
		case 179: return std::make_unique<variant_179>();
		case 180: return std::make_unique<variant_180>();
		case 181: return std::make_unique<variant_181>();
		case 182: return std::make_unique<variant_182>();
		case 183: return std::make_unique<variant_183>();
		case 184: return std::make_unique<variant_184>();
		case 185: return std::make_unique<variant_185>();
		case 186: return std::make_unique<variant_186>();
		case 187: return std::make_unique<variant_187>();
		case 188: return std::make_unique<variant_188>();
		case 189: return std::make_unique<variant_189>();
		case 190: return std::make_unique<variant_190>();
		case 191: return std::make_unique<variant_191>();
		case 192: return std::make_unique<variant_192>();
		case 193: return std::make_unique<variant_193>();
		case 194: return std::make_unique<variant_194>();
		case 195: return std::make_unique<variant_195>();
		case 196: return std::make_unique<variant_196>();
		case 197: return std::make_unique<variant_197>();
		case 198: return std::make_unique<variant_198>();
		case 199: return std::make_unique<variant_199>();
		case 200: return std::make_unique<variant_200>();
		case 201: return std::make_unique<variant_201>();
		case 202: return std::make_unique<variant_202>();
		case 203: return std::make_unique<variant_203>();
		case 204: return std::make_unique<variant_204>();
		case 205: return std::make_unique<variant_205>();
		case 206: return std::make_unique<variant_206>();
		case 207: return std::make_unique<variant_207>();
		case 208: return std::make_unique<variant_208>();
		case 209: return std::make_unique<variant_209>();
		default: return std::unique_ptr<variant>(); // invalid
	}
}
