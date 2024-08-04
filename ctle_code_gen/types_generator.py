# ctle Copyright (c) 2022 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

from .formatted_output import formatted_output

int_bit_sizes = [8,16,32,64]
real_bit_sizes = [32,64]
vector_names = ['x','y','z','w']
color_names = ['r','g','b','a']

# forward definition of all ctle classes
fwd_classes = [
    ['status.h', ['enum class status_code : int','status']],
	['data_source.h', ['file_data_source']],
	['data_destination.h', ['file_data_destination']],
	['hasher.h', ['hasher_sha256', 'hasher_xxh64', 'hasher_xxh128', 'template <size_t _Size> class hasher_noop']],
	['read_stream.h', ['template<class _DataSourceTy, class _HashTy = hasher_noop<64>> class read_stream']],
	['write_stream.h', ['template<class _DataDestTy, class _HashTy = hasher_noop<64>> class write_stream']],
	['ntup.h', ['template<class _Ty, size_t _Size> class n_tup','template<class _Ty, size_t _Size> class nm_tup']],
	['bimap.h', ['template<class _Kty, class _Vty> class bimap']],
	['bitmap_font.h', ['enum class bitmap_font_flags : int']],
	['file_funcs.h', ['enum class access_mode : unsigned int','_file_object']],
	['hash.h', ['template<size_t _Size> struct hash']],
	['idx_vector.h', ['template <class _Ty, class _IdxTy = std::vector<i32>, class _VecTy = std::vector<_Ty>> class idx_vector']]
]

def generate_types_dict():
	global types
    
	# setup dictionary of types
	types = []
	for bs in int_bit_sizes:
		for sign in ['i','u']:
      
			# add the basic type
			btype = f'{sign}{bs}'
			types.append({ 
				'otype': btype,
				'tuple_cnt': 0
				})
   
			# add the tuple variants
			for d in range(1,5):
				otype = f'{btype}tup{d}'
				types.append({ 
					'otype': otype,
					'tuple_cnt': d,
					'subtype_cnt': 0,
					'btype': btype,
					})
    
			# add the tuple-of-tuple variants
			for d in range(1,5):
				for m in range(1,5):
					otype = f'{btype}tup{d}x{m}'
					subtype = f'{btype}tup{d}'
					types.append({ 
						'otype': otype,
						'tuple_cnt': m,
						'subtype_cnt': d,
						'subtype' : subtype,
						'btype': btype,
						})
    
	for bs in real_bit_sizes:
		btype = f'f{bs}'

		# add the basic type
		types.append({ 
			'otype': btype,
			'tuple_cnt': 0
			})

		# add the tuple variants
		for d in range(1,5):
			otype = f'{btype}tup{d}'
			types.append({ 
				'otype': otype,
				'tuple_cnt': d,
				'subtype_cnt': 0,
				'btype': btype,
				})

		# add the tuple-of-tuple variants
		for d in range(1,5):
			for m in range(1,5):
				otype = f'{btype}tup{d}x{m}'
				subtype = f'{btype}tup{d}'
				types.append({ 
					'otype': otype,
					'tuple_cnt': m,
					'subtype_cnt': d,
					'subtype' : subtype,
					'btype': btype,
					})   

def list_types( out:formatted_output ):
	global types

	out.generate_license_header()
	out.ln()
	out.ln('#pragma once')
	out.ln()
	out.ln('#include <cinttypes>')
	out.ln()

	out.ln('namespace ctle')
	with out.blk( no_indent = True ):
		out.comment_ln('Standard integer and real values short-hand')
		for s in int_bit_sizes:
			out.ln(f'using i{s} = std::int{s}_t;')
			out.ln(f'using u{s} = std::uint{s}_t;')
		out.ln('using f32 = float;')
		out.ln('using f64 = double;')
		out.ln()

		for fl in fwd_classes:
			out.comment_ln(f'from {fl[0]}')
			for cl in fl[1]:
				if cl.startswith('template') or cl.startswith('enum'):
					out.ln(f'{cl};')
				else:
					out.ln(f'class {cl};')
			out.ln()

		out.comment_ln(f'short-hand defines of common n-tuples')
		for otype in types:
			if otype['tuple_cnt'] > 0:
				if otype['subtype_cnt'] > 0:
					out.ln(f'using {otype["otype"]} = nm_tup<n_tup<{otype["btype"]},{otype["subtype_cnt"]}>,{otype["tuple_cnt"]}>; ')
				else:
					out.ln(f'using {otype["otype"]} = n_tup<{otype["btype"]},{otype["tuple_cnt"]}>; ')
		out.ln()
  
		out.comment_ln(f'short-hand aliases for float and double tuples')
		for otype in types:
			if otype['tuple_cnt'] > 0:
				if otype['btype'] == 'f32' or otype['btype'] == 'f64':
					alias = 'd' if otype['btype'] == 'f64' else 'f'
					alias += otype['otype'][3:]
					out.ln(f'using {alias} = {otype["otype"]}; ')
		out.ln()

		out.comment_ln(f'// a span of characters, with start and end pointers')
		out.ln('template<class _Ty> struct string_span;')
		out.ln()
	out.ln('//namespace ctle')
	out.ln()

	out.ln('#ifdef CTLE_IMPLEMENTATION', no_indent=True)		

	out.ln('#include "string_funcs.h"')

	out.ln('namespace ctle')
	with out.blk():
		out.ln()
		out.comment_ln(f'Print types to strings.')
		for otype in types:
			if otype['tuple_cnt'] == 0:
				str = f'template<> std::string to_string( const {otype["otype"]} &val ) {{ return '
				str += 'std::to_string(val)'
				str += '; }'
				out.ln(str)
		out.ln()
  	
		for bs in int_bit_sizes:
			for sign in ['i','u']:
				otype = f'{sign}{bs}'
				ttype = f'{sign}64'
				result_param = 'result' if bs != 64 else ''
				out.ln(f'template<> {otype} from_string( const string_span<char> &str, bool &{result_param} ) noexcept')
				with out.blk():
					str = ttype
					str += ' val = std::sto'
					str += 'u' if sign == 'u' else '' 
					str += f'll( std::string( str.start, str.end ) );'
					out.ln(str)
					if bs != 64:
						str = 'if( '
						if sign == 'i':
							str += f'val < ({ttype})std::numeric_limits<{otype}>::min() || '
						str += f'val > ({ttype})std::numeric_limits<{otype}>::max() ) {{ result = false; return {{}}; }}'
						out.ln(str)
					out.ln(f'return ({otype})val;')
				out.ln(f'template<> {otype} from_string( const string_span<char> &str )')
				with out.blk():
					out.ln('bool result = true;')
					out.ln(f'auto val = from_string<{otype}>( str, result);')
					if bs != 64:
						out.ln(f'if( !result ) {{ throw std::out_of_range("In ctle::from_string(): the value is out of range for the destination type ({otype})."); }}')
					out.ln('return val;')
				out.ln()

		for bs in real_bit_sizes:
			alias = 'd' if bs == 64 else 'f'
			otype = f'f{bs}'
			out.ln(f'template<> {otype} from_string( const string_span<char> &str, bool & ) noexcept')
			with out.blk():
				out.ln(f'return std::sto{alias}( std::string( str.start, str.end ) );')
			out.ln(f'template<> {otype} from_string( const string_span<char> &str )')
			with out.blk():
				out.ln(f'return std::sto{alias}( std::string( str.start, str.end ) );')
			out.ln()
 
  
	out.ln('//namespace ctle')
 
	out.ln('#endif//CTLE_IMPLEMENTATION', no_indent=True)		


def define_n_tuples( out:formatted_output ):
	global types

	for d in range(1,5):
		out.comment_ln(f'{d} dimensional n-tuple template')
		out.ln(f'template<class _Ty> class n_tup<_Ty,{d}>')
		with out.blk( add_semicolon=True ):
			out.ln('public:', intent_adjust=-1 )
			out.ln('using value_type = _Ty;')
			out.ln(f'static const size_t dimensions = {d};')
			out.ln()
			out.ln('#ifdef CTLE_N_TUP_ONLY_XYZW', no_indent=True)
			for td in range(d):
				out.ln(f'_Ty {vector_names[td]}; // [{td}]')
			out.ln('#else//CTLE_N_TUP_ONLY_XYZW', no_indent=True)
			for td in range(d):
				out.ln(f'union {{ _Ty {vector_names[td]}; _Ty {color_names[td]}; }}; // [{td}]')
			out.ln('#endif//CTLE_N_TUP_ONLY_XYZW', no_indent=True)
			out.ln()

			# default ctor
			out.comment_ln('ctors/dtor')
			s = 'n_tup() noexcept { '
			for td in range(d):
				s += f'{vector_names[td]} = 0; '
			s += '}'
			out.ln(s)

			# init ctor from separate values
			s = 'n_tup( '
			for td in range(d):
				s += f'_Ty _{vector_names[td]}'
				if td != d-1:
					s += ','
			s += ' ) noexcept { '
			for td in range(d):
				s += f'{vector_names[td]} = _{vector_names[td]}; '
			s += '}'
			out.ln(s)
   
			# init ctor from array
			s = 'n_tup( const _Ty *values ) noexcept { '
			for td in range(d):
				s += f'{vector_names[td]} = values[{td}]; '
			s += '}'
			out.ln(s)   

			# default copy ctor, copy operator & dtor
			out.ln('n_tup( const n_tup &other ) noexcept = default;')
			out.ln('n_tup &operator=( const n_tup &other ) noexcept = default;')
			out.ln('~n_tup() noexcept = default;')
			
			out.ln()

			# cmp operators
			out.comment_ln('cmps ops, (order of significance: x>y>z>w)')
			out.ln('bool operator<( const n_tup &rhs ) const noexcept')
			with out.blk():
				for td in range(d):
					out.ln(f'if( this->{vector_names[td]} < rhs.{vector_names[td]} ) {{ return true; }}')
					if td < d-1:
						out.ln(f'if( this->{vector_names[td]} > rhs.{vector_names[td]} ) {{ return false; }}')
					else:
						out.ln('return false;')
			out.ln('bool operator>=( const n_tup &rhs ) const noexcept { return !this->operator<(rhs); }')
			out.ln('bool operator>( const n_tup &rhs ) const noexcept')
			with out.blk():
				for td in range(d):
					out.ln(f'if( this->{vector_names[td]} > rhs.{vector_names[td]} ) {{ return true; }}')
					if td < d-1:
						out.ln(f'if( this->{vector_names[td]} < rhs.{vector_names[td]} ) {{ return false; }}')
					else:
						out.ln('return false;')
			out.ln('bool operator<=( const n_tup &rhs ) const noexcept { return !this->operator>(rhs); }')
			out.ln('bool operator==( const n_tup &rhs ) const noexcept')
			with out.blk():
				for td in range(d):
					out.ln(f'if( this->{vector_names[td]} != rhs.{vector_names[td]} ) {{ return false; }}')
				out.ln('return true;')
			out.ln('bool operator!=( const n_tup &rhs ) const noexcept { return !this->operator==(rhs); }')
			out.ln()

			# data ptr access
			out.comment_ln('data access')
			out.ln('const _Ty *data() const noexcept { return &this->x; }')
			out.ln('_Ty *data() noexcept { return &this->x; }')
			#out.ln('const _Ty *operator&() const noexcept { return &this->x; }')
			#out.ln('_Ty *operator&() noexcept { return &this->x; }')
			out.ln()

			# index access of values
			out.comment_ln('index operator')
			out.ln('const _Ty &at( size_t index ) const noexcept { return (&this->x)[index]; }')
			out.ln('_Ty &at( size_t index ) noexcept { return (&this->x)[index]; }')
			out.ln('const _Ty &operator[]( size_t index ) const noexcept { return (&this->x)[index]; }')
			out.ln('_Ty &operator[]( size_t index ) noexcept { return (&this->x)[index]; }')
			out.ln()

			# glm interops
			out.ln('#ifdef GLM_VERSION', no_indent=True)			
			out.comment_ln('conversions to/from glm (to enable, include glm.hpp before including types.h)')

			# copy ctor
			s = f'n_tup( const glm::vec<{d},_Ty> &other ) noexcept {{ '
			for td in range(d):
				s += f'{vector_names[td]} = other.{vector_names[td]}; '
			s += '}'
			out.ln(s)

			# copy operator
			s = f'n_tup &operator=( const glm::vec<{d},_Ty> &other ) noexcept {{ '
			for td in range(d):
				s += f'{vector_names[td]} = other.{vector_names[td]}; '
			s += 'return *this; }'
			out.ln(s)

			# convert to glm vector
			s = f'operator glm::vec<{d},_Ty>() const noexcept {{ return glm::vec<{d},_Ty>( '
			for td in range(d):
				s += f'{vector_names[td]} '
				if td < d-1:
					s += ', ' 
			s += '); }'
			out.ln(s)

			out.ln('#endif//GLM_VERSION', no_indent=True)			

		out.ln()

	for d in range(1,5):
		out.comment_ln(f'{d} dimensional tuple-of-tuples template')
		out.ln(f'template<class _Ty> class nm_tup<_Ty,{d}>')
		with out.blk( add_semicolon=True ):
			out.ln('public:', intent_adjust=-1 )
			out.ln('using value_type = _Ty;')
			out.ln(f'static const size_t dimensions = {d};')
			out.ln()
			out.ln(f'_Ty values[{d}]; // {d} values of tuple type')
			out.ln()

			# default ctor
			out.comment_ln('ctors/dtor')

			# init ctor from separate values
			s = 'nm_tup( '
			for td in range(d):
				s += f'const _Ty &value{td} = {{}}'
				if td != d-1:
					s += ','
			s += ' ) noexcept { '
			for td in range(d):
				s += f'values[{td}] = value{td}; '
			s += '}'
			out.ln(s)

			# init ctor from array
			s = 'nm_tup( const _Ty *_values ) noexcept { '
			for td in range(d):
				s += f'values[{td}] = _values[{td}]; '
			s += '}'
			out.ln(s)

			# default copy ctor, copy operator & dtor
			out.ln('nm_tup( const nm_tup &other ) noexcept = default;')
			out.ln('nm_tup &operator=( const nm_tup &other ) noexcept = default;')
			out.ln('~nm_tup() noexcept = default;')

			out.ln()

			# cmp operators
			out.comment_ln('cmps ops, (order of significance: 0>1>2>3)')
			out.ln('bool operator<( const nm_tup &rhs ) const noexcept')
			with out.blk():
				for td in range(d):
					out.ln(f'if( this->values[{td}] < rhs.values[{td}] ) {{ return true; }}')
					if td < d-1:
						out.ln(f'if( this->values[{td}] > rhs.values[{td}] ) {{ return false; }}')
					else:
						out.ln('return false;')
			out.ln('bool operator>=( const nm_tup &rhs ) const noexcept { return !this->operator<(rhs); }')
			out.ln('bool operator>( const nm_tup &rhs ) const noexcept')
			with out.blk():
				for td in range(d):
					out.ln(f'if( this->values[{td}] > rhs.values[{td}] ) {{ return true; }}')
					if td < d-1:
						out.ln(f'if( this->values[{td}] < rhs.values[{td}] ) {{ return false; }}')
					else:
						out.ln('return false;')
			out.ln('bool operator<=( const nm_tup &rhs ) const noexcept { return !this->operator>(rhs); }')
			out.ln('bool operator==( const nm_tup &rhs ) const noexcept')
			with out.blk():
				for td in range(d):
					out.ln(f'if( this->values[{td}] != rhs.values[{td}] ) {{ return false; }}')
				out.ln('return true;')
			out.ln('bool operator!=( const nm_tup &rhs ) const noexcept { return !this->operator==(rhs); }')
			out.ln()

			# data ptr access
			out.comment_ln('data access')
			out.ln('const _Ty *data() const noexcept { return this->values; }')
			out.ln('_Ty *data() noexcept { return this->values; }')
			#out.ln('const _Ty *operator&() const noexcept { return this->values; }')
			#out.ln('_Ty *operator&() noexcept { return this->values; }')
			out.ln()

			# index access of values
			out.comment_ln('index operator')
			out.ln('const _Ty &at( size_t index ) const noexcept { return this->values[index]; }')
			out.ln('_Ty &at( size_t index ) noexcept { return this->values[index]; }')
			out.ln('const _Ty &operator[]( size_t index ) const noexcept { return this->values[index]; }')
			out.ln('_Ty &operator[]( size_t index ) noexcept { return this->values[index]; }')
			out.ln()

			# glm interops
			out.ln('#ifdef GLM_VERSION', no_indent=True)			
			out.comment_ln('conversions to/from glm (to enable, include glm.hpp before including types.h)')

			# copy ctor
			s = f'nm_tup( const glm::mat<{d}, _Ty::dimensions, typename _Ty::value_type> &other ) noexcept {{ '
			for td in range(d):
				s += f'values[{td}] = _Ty(other[{td}]); '
			s += '}'
			out.ln(s)

			# copy operator
			s = f'nm_tup &operator=( const glm::mat<{d}, _Ty::dimensions, typename _Ty::value_type> &other ) noexcept {{ '
			for td in range(d):
				s += f'values[{td}] = _Ty(other[{td}]); '
			s += 'return *this; }'
			out.ln(s)

			# convert to glm matrix
			s = f'operator glm::mat<{d}, _Ty::dimensions, typename _Ty::value_type>() const noexcept {{ return glm::mat<{d}, _Ty::dimensions, typename _Ty::value_type>( '
			for td in range(d):
				s += f'values[{td}]'
				if td < d-1:
					s += ', ' 
			s += '); }'
			out.ln(s)

			out.ln('#endif//GLM_VERSION', no_indent=True)			

		out.ln()

	out.ln('#ifdef CTLE_IMPLEMENTATION', no_indent=True)		

	out.ln()
	out.comment_ln(f'Print types to strings.')
	for otype in types:
		if otype['tuple_cnt'] != 0:
			str = f'template<> std::string to_string( const {otype["otype"]} &val ) {{ return '
			if otype['subtype_cnt'] == 0:
				d = otype['tuple_cnt']
				for v in range(d):
					str += f'to_string(val.{vector_names[v]})'
					if v < d-1:
						str += ' + " " + '
			else:
				d = otype['tuple_cnt']
				for v in range(d):
					str += f'to_string(val.values[{v}])'
					if v < d-1:
						str += ' + " " + '
			str += '; }'
			out.ln(str)
	out.ln()

	out.ln('''
template<class _Ty> inline _Ty _n_tup_from_string( const string_span<char> &str, bool &result ) noexcept
{
	using basic_type = typename _Ty::value_type; 

	std::vector<string_span<char>> toks;
	if( !lex_t<char>( &toks, str ) || toks.size() != _Ty::dimensions )
	{ 
		result = false;
		return {};
	}

	_Ty val = {};
	for( size_t inx=0; inx<_Ty::dimensions; ++inx )
	{
		// use the non-throwing conversion, and check the result
		val[inx] = from_string<basic_type>(toks[inx], result); 
		if( !result )
  		{
			return {}; // the conversion failed, return empty 
		}
	}
	return val;
}
template<class _Ty> inline _Ty _n_tup_from_string( const string_span<char> &str )
{
	bool result = true;
	auto val = _n_tup_from_string<_Ty>(str, result);
	if( !result ) 
 	{
		throw std::out_of_range("In ctle::from_string(): Could not convert from string to value, either wrong number of items, or the values are out of range.");
	}
	return val;
}

template<class _Ty> inline _Ty _nm_tup_from_string( const string_span<char> &str, bool &result ) noexcept
{
	constexpr const size_t total_values = _Ty::dimensions * _Ty::value_type::dimensions;
	using basic_type = typename _Ty::value_type::value_type; 

	std::vector<string_span<char>> toks;
	if( !lex_t<char>( &toks, str ) || toks.size() != total_values )
	{ 
		result = false;
		return {};
	}

	_Ty val = {};
	for( size_t inx=0; inx<_Ty::dimensions; ++inx )
	{
		auto &tup = val.values[inx];
		for( size_t n=0; n<_Ty::value_type::dimensions; ++n )
		{
			// use the non-throwing conversion, and check the result
			tup[n] = from_string<basic_type>(toks[inx*_Ty::value_type::dimensions + n], result); 
			if( !result )
			{
				return {}; // the conversion failed, return empty 
			}   
		}
	}
	return val;
}
template<class _Ty> inline _Ty _nm_tup_from_string( const string_span<char> &str )
{
	bool result = true;
	auto val = _nm_tup_from_string<_Ty>(str, result);
	if( !result ) 
 	{
		throw std::out_of_range("In ctle::from_string(): Could not convert from string to value, either wrong number of items, or the values are out of range.");
	}
	return val;
}

''')

	#def print_from_string_for_tuple( out, otype:str, d:int, btype:str ):
	#	out.ln(f'template<> {otype} from_string( const string_span<char> &str, bool &result ) noexcept {{ return _n_tup_from_string<{otype}>(str, result); }}')
	#	out.ln(f'template<> {otype} from_string( const string_span<char> &str ) {{ return _n_tup_from_string<{otype}>(str); }}')

	for otype in types:
		if otype['tuple_cnt'] != 0:
			func = '_n_tup_from_string' if otype['subtype_cnt'] == 0 else '_nm_tup_from_string'
			out.ln(f'template<> {otype["otype"]} from_string( const string_span<char> &str, bool &result ) noexcept {{ return {func}<{otype["otype"]}>(str, result); }}')
			out.ln(f'template<> {otype["otype"]} from_string( const string_span<char> &str ) {{ return {func}<{otype["otype"]}>(str); }}')

	out.ln('#endif//CTLE_IMPLEMENTATION', no_indent=True)		

	out.ln()

def generate_random_value_functions( out:formatted_output ):
	out.ln()
 
	for otype in types:
		d = otype['tuple_cnt']
		if d > 0:
			str = f'using ctle::{otype["otype"]};'
			out.ln(str)
 
	for otype in types:
		d = otype['tuple_cnt']
		if d > 0:
			str = f'template<> {otype["otype"]} random_value<{otype["otype"]}>() {{ return {otype["otype"]}('
			subtype = otype["btype"] if otype['subtype_cnt'] == 0 else otype["subtype"]
			for v in range(d):
				str += f'random_value<{subtype}>()'
				if v < d-1:
					str += ', '
			str += '); }'
			out.ln(str)
	out.ln()

def generate_variant_tuple_implementation( out:formatted_output, otype, variant_index ) -> str:
	obj_type = otype['otype']
	variant_type = 'variant_' + obj_type
	out.ln( f'class {variant_type} : public variant')
	with out.blk(add_semicolon=True,no_indent=True):
		out.ln('public:')
		with out.tab():
			out.ln(f'std::vector<ctle::{obj_type}> data;')
			out.ln()
			out.ln(f'virtual ~{variant_type}() override {{}}')
			out.ln(f'virtual size_t type() const override {{ return {variant_index}; }}')
			out.ln(f'virtual const char *type_name() const override {{ return "{obj_type}"; }}')
			out.ln(f'virtual size_t type_size() const override {{ return sizeof(ctle::{obj_type}); }}')
			out.ln('virtual size_t type_count() const override { return this->data.size(); }')
			out.ln(f'virtual bool is_equal( const variant &other ) const override {{ return _are_equal( *this, other ); }}');
			out.ln('virtual void clear() override { this->data.clear(); }')
			out.ln('virtual const void *cdata() const override { return this->data.data(); }')
			out.ln(f'virtual void random() {{ this->data = random_vector<ctle::{obj_type}>(); }}')
			out.ln(f'virtual ctle::status read_from_stream( ctle::read_stream<ctle::file_data_source,ctle::hasher_xxh128> &strm ) override {{ return _read_from_stream<ctle::{obj_type}>( strm, this->data ); }}')
			out.ln(f'virtual ctle::status write_to_stream( ctle::write_stream<ctle::file_data_destination,ctle::hasher_xxh128> &strm ) const override {{ return _write_to_stream<ctle::{obj_type}>( strm, this->data ); }}')
	out.ln()
	return variant_type
  
def generate_variant_implementations( out:formatted_output ):
	variant_list = []
	for otype in types:
		variant_list.append( generate_variant_tuple_implementation( out, otype, len(variant_list) ) )
	out.ln()

	out.ln('std::unique_ptr<variant> random_variant()')
	with out.blk():
		out.ln(f'return new_variant( rand() % {len(variant_list)} );')
		
	out.ln('std::unique_ptr<variant> new_variant( size_t inx )')
	with out.blk():
		out.ln('switch( inx )')
		with out.blk():
			for inx,varnt in enumerate(variant_list):
				out.ln(f'case {inx}: return std::make_unique<{varnt}>();')
			out.ln('default: return std::unique_ptr<variant>(); // invalid')

def generate_unit_tests_variants( test_path:str ):
	out = formatted_output()
	out.generate_license_header()
	out.ln('''
#include "unit_tests.h"
#include "../ctle/status.h"
#include "../ctle/ntup.h"    
#include "../ctle/hasher.h"   
#include "../ctle/read_stream.h"
#include "../ctle/data_source.h"
#include "../ctle/write_stream.h"
#include "../ctle/data_destination.h"
		
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

''')
  
	generate_variant_implementations( out )
	out.write_lines_to_file( test_path + '/unit_tests_variants.cpp' )    

def generate_types( src_path:str, test_path:str ):

	generate_types_dict()

	## generate types.h 
	out = formatted_output()
	list_types( out )
	out.write_lines_to_file( src_path + '/types.h' )

	## generate ntuple.h 
	out = formatted_output()
	out.generate_license_header()
	out.ln()
	out.ln('#pragma once')
	out.ln()
	out.ln('#include <cinttypes>')
	out.ln('#include <limits>')
	out.ln()
	out.ln('#include "types.h"')
	out.ln('#include "string_funcs.h"')
	out.ln('#include "status.h"')	
	out.ln('#include "status_return.h"')	
	out.ln()
	out.ln('namespace ctle')
	with out.blk( no_indent = True ):
		define_n_tuples(out)
	out.ln('//namespace ctle')
	out.write_lines_to_file( src_path + '/ntup.h' )
 
 	# generate random values
	#out = formatted_output()
	#generate_random_value_functions(out)
	#out.write_lines_to_file( 'random_value_output' )

	generate_unit_tests_variants( test_path )