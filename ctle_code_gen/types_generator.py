# ctle Copyright (c) 2022 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

from .formatted_output import formatted_output

int_bit_sizes = [8,16,32,64]
real_bit_sizes = [32,64]
real_names = ['float','double']
vector_names = ['x','y','z','w']
color_names = ['r','g','b','a']

def list_types( out:formatted_output ):
	out.comment_ln('Standard integer and real values short-hand')
	for s in int_bit_sizes:
		out.ln(f'using i{s} = std::int{s}_t;')
		out.ln(f'using u{s} = std::uint{s}_t;')
	out.ln('using f32 = float;')
	out.ln('using f64 = double;')
	out.ln()

def define_n_tuples( out:formatted_output ):
	out.comment_ln(f'n-tuple template, for 1-4 dimensions')
	out.ln('template<class _Ty, size_t _Size> class n_tup;')
	out.ln()

	for d in range(1,5):
		out.comment_ln(f'{d} dimensional n-tuple template')
		out.ln(f'template<class _Ty> class n_tup<_Ty,{d}>')
		with out.blk( add_semicolon=True ):
			out.ln('public:')
			out.ln('using value_type = typename _Ty;')
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

			# copy ctor
			s = 'n_tup( const n_tup &other ) noexcept { '
			for td in range(d):
				s += f'{vector_names[td]} = other.{vector_names[td]}; '
			s += '}'
			out.ln(s)

			# copy operator
			s = 'n_tup &operator=( const n_tup &other ) noexcept { '
			for td in range(d):
				s += f'{vector_names[td]} = other.{vector_names[td]}; '
			s += 'return *this; }'
			out.ln(s)

			# dtor
			out.ln('~n_tup() noexcept {}')
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
			out.ln('const _Ty *operator&() const noexcept { return &this->x; }')
			out.ln('_Ty *operator&() noexcept { return &this->x; }')
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
					'btype': btype,
					})
	for rtype in ['float','double']:
		# add the basic type
		types.append({ 
			'otype': rtype,
			'tuple_cnt': 0
			})
		# add the tuple variants
		for d in range(1,5):
			otype = f'{rtype[0]}tup{d}'
			types.append({ 
				'otype': otype,
				'tuple_cnt': d,
				'btype': rtype,
				})

	out.comment_ln(f'short-hand defines of common n-tuples')
	for otype in types:
		if otype['tuple_cnt'] > 0:
			out.ln(f'using {otype["otype"]} = n_tup<{otype["btype"]},{otype["tuple_cnt"]}>; ')
	out.ln()

	out.comment_ln(f'Parse types from strings.')
	out.ln('template<class _Ty> _Ty from_string( const string_span<char> &str );')
	out.ln('template<class _Ty> inline _Ty from_string( const char *start, const char *end ) { return from_string<_Ty>( string_span<char>(start,end) ); }')
	out.ln('template<class _Ty> inline _Ty from_string( const std::basic_string<char> &str ) { return from_string<_Ty>( string_span<char>(str.data(), str.data() + str.size()) ); }')
	out.ln()

	out.comment_ln(f'Print types to strings.')
	for otype in types:
		str = f'inline std::string to_string( const {otype["otype"]} &val ) {{ return '
		if otype['tuple_cnt'] == 0:
			str += 'std::to_string(val)'
		else:
			d = otype['tuple_cnt']
			for v in range(d):
				str += f'std::to_string(val.{vector_names[v]})'
				if v < d-1:
					str += ' + " " + '
		str += '; }'
		out.ln(str)
	out.ln()

	out.ln('#ifdef CTLE_IMPLEMENTATION', no_indent=True)		

	for bs in int_bit_sizes:
		for sign in ['i','u']:
			otype = f'{sign}{bs}'
			ttype = f'{sign}64'
			out.ln(f'template<> {otype} from_string( const string_span<char> &str )')
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
					str += f'val > ({ttype})std::numeric_limits<{otype}>::max() ) {{ throw std::out_of_range("In ctle::from_string(): the value is out of range for the destination type ({otype})."); }}'
					out.ln(str)
				out.ln(f'return ({otype})val;')
			out.ln()

	for otype in ['float','double']:
		out.ln(f'template<> {otype} from_string( const string_span<char> &str )')
		with out.blk():
			out.ln(f'return std::sto{otype[0]}( std::string( str.start, str.end ) );')
		out.ln()

	def print_from_string_for_tuple( out, otype:str, d:int, btype:str ):
		out.ln(f'template<> {otype} from_string( const string_span<char> &str )')
		with out.blk():
			out.ln(f'std::vector<string_span<char>> toks;')
			out.ln(f'if( !lex_t<char>( &toks, str ) || toks.size() != {d} ) {{ throw std::out_of_range("In ctle::from_string(): Invalid number of values, it should be exactly {d} values, separated by whitespace."); }}')
			out.ln(f'return {otype}( ')
			for v in range(d):
				str = f'\tfrom_string<{btype}>(toks[{v}])'
				if v < d-1:
					str += ','
				out.ln(str)
			out.ln('\t);')
		out.ln()

	for bs in int_bit_sizes:
		for sign in ['i','u']:
			for d in range(1,5):
				otype = f'{sign}{bs}tup{d}'
				print_from_string_for_tuple( out, otype, d, f'{sign}{bs}' )
	for bs in ['float','double']:
		for d in range(1,5):
			otype = f'{bs[0]}tup{d}'
			print_from_string_for_tuple( out, otype, d, bs )

	out.ln('#endif//CTLE_IMPLEMENTATION', no_indent=True)		

	out.ln()

def generate_types( path:str ):

	out = formatted_output()

	out.generate_license_header()
	out.ln()
	out.ln('#pragma once')
	out.ln()
	out.ln('#include <cinttypes>')
	out.ln('#include <limits>')
	out.ln()
	out.ln('#include "string_funcs.h"')
	out.ln('#include "status.h"')	
	out.ln('#include "status_return.h"')	
	out.ln()

	out.ln('namespace ctle')
	with out.blk():

		list_types(out)
		define_n_tuples(out)
			
	out.write_lines_to_file( path )
