# ctle Copyright (c) 2022 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

from .formatted_output import formatted_output

get_types = { 
	'get': '_Ty', 
	'getcref': 'const _Ty &'
	}

def property_name( get_type:str, set_type:bool, value_type:bool ):
	str = 'property'
	if get_type != None:
		str += '_' + get_type
	if set_type:
		str += '_set'
	if value_type:
		str += '_value'
	return str

def write_property_class( out, get_type:str, set_type:bool, value_type:bool ):
	prop_class = property_name(get_type, set_type, value_type)

	if value_type:
		out.ln('template<typename _Ty, typename _Owner>')
	else:
		out.ln('template<typename _Ty>')
	out.ln(f'class {prop_class}' )
	with out.blk(add_semicolon=True):
		out.ln('public:')
		with out.tab():
			if value_type:
				out.ln('friend _Owner; // allow the owner of the property to directly modify the stored value v')
				out.ln()

			# define ctor
			str = prop_class + '( '
			if value_type:
				str += 'const _Ty &value, '
			if get_type != None:
				str += f'const std::function<{get_types[get_type]} ( const {prop_class} * )> _get'
			if set_type:
				if get_type != None:
					str += ', '
				str += f'std::function<void({prop_class} *, const _Ty &)> _set'
			str += ' ) :'
			out.ln(str)
			with out.tab():
				str = ''
				if value_type:
					str += 'v(value), '
				if get_type != None:
					str += 'get_method(_get)'
				if set_type:
					if get_type != None:
						str += ', '
					str += 'set_method(_set)'
				str += ' {}'
				out.ln(str)
			out.ln()

			# define convert to type operator and get method
			if get_type != None:
				out.ln(f'{get_types[get_type]} get() const {{ return this->get_method(this); }}')
				out.ln(f'operator {get_types[get_type]} () const {{ return this->get(); }}')
				out.ln()

			# define assign operator and set method
			if set_type:
				out.ln(f'void set(const _Ty &value) {{ this->set_method(this, value); }}')
				out.ln(f'const {prop_class} & operator= (const _Ty &value) {{ this->set(value); return *this; }}')
				out.ln()
			
		out.ln('private:')
		with out.tab():
			out.ln(f'{prop_class}() = delete;')
			out.ln(f'{prop_class}( const {prop_class} & ) = delete;')
			out.ln(f'const {prop_class} & operator= ( const {prop_class} & ) = delete;')
			out.ln(f'{prop_class}( {prop_class} && ) = delete;')
			out.ln(f'const {prop_class} & operator= ( {prop_class} && ) = delete;')
			out.ln();

			if value_type:
				out.ln('_Ty v; // the stored value')
			if get_type != None:
				out.ln(f'const std::function<{get_types[get_type]} (const {prop_class} *)> get_method;')
			if set_type:
				out.ln(f'const std::function<void({prop_class} *, const _Ty &)> set_method;')

			out.ln()

	out.ln()

def generate_property( path:str ):

	out = formatted_output()

	out.generate_license_header()
	out.ln()
	out.ln('#pragma once')
	out.ln()
	out.ln('#include <functional>')
	out.ln()

	out.ln('namespace ctle')
	with out.blk():
		out.comment_ln('The property_[...] templates is a convenient way to implement properties in classes, where each property is accessed as normal variables, but can be made read-only, write-only, read/write, and let the owner override if a value is returned from a variable, or evaluated on-the-fly, etc.')
		for value_type in [False,True]:
			for set_type in [True,False]:
				for get_type in ['get','getcref',None]:
					# skip invalid combination, need at least set or get
					if get_type == None and set_type == False:
						continue 
					write_property_class(out, get_type, set_type, value_type)
			
	out.write_lines_to_file( path )




