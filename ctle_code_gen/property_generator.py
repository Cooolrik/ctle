# ctle Copyright (c) 2022 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

from .formatted_output import formatted_output
from enum import Enum

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

class ctor_value_type(Enum):
	no_value_type = 0
	value_default_assignment = 1
	value_parameter = 2

def output_ctor( out, prop_class:str, get_type:str, set_type:bool, value_type:ctor_value_type ):
	str = prop_class + '( '
	if value_type == ctor_value_type.value_parameter:
		str += 'const _Ty &value, '
	if get_type != None:
		str += f'_get_type _get'
		if value_type != ctor_value_type.no_value_type:
			str += '= {}'
	if set_type:
		if get_type != None:
			str += ', '
		str += f'_set_type _set'
		if value_type != ctor_value_type.no_value_type:
			str += '= {}'		
	str += ' ) :'
	out.ln(str)
	with out.tab():
		str = ''
		if value_type == ctor_value_type.value_parameter:
			str += 'v(value), '
		if get_type != None:
			str += 'get_method(_get)'
		if set_type:
			if get_type != None:
				str += ', '
			str += 'set_method(_set)'
		if value_type == ctor_value_type.value_default_assignment:
			str += ' { trivially_default_constructible_identity_assign(this->v); }'
		else:
			str += ' {}'
		out.ln(str)
	out.ln()
    

def write_property_class( out, get_type:str, set_type:bool, value_type:bool ):
	prop_class = property_name(get_type, set_type, value_type)

	out.comment_ln(f'template class {prop_class}:')
	if get_type != None:
		str = 'a copy of the value' if get_type == 'get' else 'a const reference to the value.'
		out.comment_ln(f' - has a .get() method and cast operator which return {str}' )
	if set_type:
		out.comment_ln(f' - has a .set() method and assignment operator which accepts a const reference to copy from' )
	if value_type:
		out.comment_ln(' - stores the value of the property in variable .v, which can be accessed directly by the _Owner type. ')
		out.comment_ln('   The value is always initialized, either explicitly in the property ctor, using a default ctor')
		out.comment_ln('   or if the value is trivially constructable, using the = {}.')

	if value_type:
		out.ln('template<typename _Ty, typename _Owner>')
	else:
		out.ln('template<typename _Ty>')
	out.ln(f'class {prop_class}' )
	with out.blk(add_semicolon=True):
		if value_type:
			out.ln('protected:')
			with out.tab():
				out.ln('friend _Owner; // allow the owner of the property to directly modify the stored value v')
			out.ln()
        
		out.ln('public:')
		with out.tab():
			out.ln('using value_type = _Ty;')
			if get_type != None:
				out.ln(f'using _get_type = const std::function<{get_types[get_type]} (const {prop_class} *, status & )>;')
			if set_type:
				out.ln(f'using _set_type = const std::function<status({prop_class} *, const _Ty &)>;')
			out.ln()
   
			# define ctors
			if value_type:
				out.comment_ln('default ctor with default initialized value')
				output_ctor( out, prop_class, get_type, set_type, ctor_value_type.value_default_assignment )
				out.comment_ln('ctor assignment for the value')
				output_ctor( out, prop_class, get_type, set_type, ctor_value_type.value_parameter )
			else:
				out.comment_ln('ctor with explicit assignment(s) for the property')
				output_ctor( out, prop_class, get_type, set_type, ctor_value_type.no_value_type )
			
			# define convert to type operator and get method
			if get_type != None:
				out.comment_ln('get method which returns the value. This version sets the status into a provided parameter, and does not throw any exception.')
				out.ln(f'{get_types[get_type]} get(status &result) const noexcept')
				with out.blk(): 
					out.ln('result = status::ok;')
					if value_type:
						out.ln('if( !(this->get_method) )')
						with out.blk():
							out.ln('return this->v;') 
					out.ln('return this->get_method(this, result);')
				out.ln()
				out.comment_ln('get method which returns the value. This version throws a std::status_error if there is an error.')
				out.ln(f'{get_types[get_type]} get() const')
				with out.blk(): 
					if value_type:
						out.ln('if( !(this->get_method) )')
						with out.blk():
							out.ln('return this->v;') 
					out.ln('status result = status::ok;')
					out.ln(f'{get_types[get_type]} ret_val = this->get_method(this, result);')
					out.ln('if( result != status::ok )')
					with out.blk():
						out.ln('throw ctle::status_error( result );')
					out.ln('return ret_val;')
				out.ln()
				out.comment_ln('operator which return the value. This operator throws std::status_error if there is an error.')
				out.ln(f'operator {get_types[get_type]} () const')
				with out.blk(): 
					if value_type:
						out.ln('if( !(this->get_method) )')
						with out.blk():
							out.ln('return this->v;') 
					out.ln('status result = status::ok;')
					out.ln(f'{get_types[get_type]} ret_val = this->get_method(this, result);')
					out.ln('if( result != status::ok )')
					with out.blk():
						out.ln('throw ctle::status_error( result );')
					out.ln('return ret_val;')
				out.ln()
        
			# define assign operator and set method
			if set_type:
				out.comment_ln('set method which sets the property to the provided value. The status is returned from the method, and it does not throw any exception.')
				out.ln(f'status set(const _Ty &value) noexcept')
				with out.blk():
					if value_type:
						out.ln('if( !(this->set_method) )')
						with out.blk():
							out.ln('this->v = value;')
							out.ln('return status::ok;') 
					out.ln('return this->set_method(this, value);')
				out.ln()
				out.comment_ln('operator which sets the property to the provided value. This operator throws std::status_error if there is an error.')				
				out.ln(f'const {prop_class} & operator= (const _Ty &value)')
				with out.blk(): 
					if value_type:
						out.ln('if( !(this->set_method) )')
						with out.blk():
							out.ln('this->v = value;')					
					out.ln('status result = this->set_method(this, value);')
					out.ln('if( result != status::ok )')
					with out.blk():
						out.ln('throw ctle::status_error( result );')
					out.ln('return *this;')
				out.ln()
			
		out.ln('private:')
		with out.tab():
			if not value_type:
				out.ln(f'{prop_class}() = delete;')
			out.ln(f'{prop_class}( const {prop_class} & ) = delete;')
			out.ln(f'const {prop_class} & operator= ( const {prop_class} & ) = delete;')
			out.ln(f'{prop_class}( {prop_class} && ) = delete;')
			out.ln(f'const {prop_class} & operator= ( {prop_class} && ) = delete;')
			out.ln();

			if value_type:
				out.ln('_Ty v; // the stored value')
			if get_type != None:
				out.ln(f'_get_type get_method;')
			if set_type:
				out.ln(f'_set_type set_method;')


	out.ln()

def generate_property( path:str ):

	out = formatted_output()

	out.generate_license_header()
	out.ln()
	out.ln('#pragma once')
	out.ln()
	out.ln('#include <functional>')
	out.ln('#include <type_traits>')
	out.ln('#include "status.h"')
	out.ln()

	out.ln('namespace ctle')
	with out.blk():
		out.comment_ln('trivially_default_constructible_identity_assign is a conditional template function which initializes trivially constructable values using the = {} assignment. For all other types, the template is a noop and does nothing.')
		out.ln('template<typename _Ty, typename std::enable_if<std::is_trivially_default_constructible<_Ty>{},bool>::type = true> void trivially_default_constructible_identity_assign( _Ty &val ) { val = {}; }')
		out.ln('template<typename _Ty, typename std::enable_if<!std::is_trivially_default_constructible<_Ty>{},bool>::type = true> void trivially_default_constructible_identity_assign( _Ty & ) { /*noop*/ }')
		out.ln()

		out.comment_ln('The property_[...] template classes are a convenient way to implement properties in classes, where each property can be accessed as a normal variable, but can also be made read-only, write-only, read/write, and let the owner class override if a value is returned from a variable, or evaluated on-the-fly, etc.')
		out.ln()
		for value_type in [False,True]:
			for set_type in [True,False]:
				for get_type in ['get','getcref',None]:
					# skip invalid combination, need at least set or get
					if get_type == None and set_type == False:
						continue 
					write_property_class(out, get_type, set_type, value_type)
			
	out.write_lines_to_file( path )




