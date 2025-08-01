# ctle Copyright (c) 2023 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

import copy
import os
from stat import S_IRUSR, S_IRGRP, S_IROTH, S_IWUSR
import importlib
import textwrap
from contextlib import contextmanager

# default values for the license header
default_project_name:str='ctle'
default_copyright_holder:str = '2022 Ulrik Lindahl'
default_license_type:str = 'MIT'
default_license_link:str = 'https://github.com/Cooolrik/ctle/blob/main/LICENSE'

def set_default_license_header_values( 
		project_name:str = None, 
		copyright_holder:str = None, 
		license_type:str = None, 
		license_link:str = None
		):
	'''Set the default values for the license header and header guard. This is used when generating the license header and header guard in the formatted_output class.'''
	global default_project_name
	global default_copyright_holder
	global default_license_type
	global default_license_link

	default_project_name = project_name if project_name else default_project_name
	default_copyright_holder = copyright_holder if copyright_holder else default_copyright_holder
	default_license_type = license_type if license_type else default_license_type
	default_license_link = license_link if license_link else default_license_link

class formatted_output:
	'''formatted_output defined a class which can be used to more easily generate formatted output of C/C++ files. The class supports different kinds of indentation, and wraps comments.'''

	def __init__(self, indentation:int = 0, tab_str:str = '\t', indent_braces:bool = False, comment_wrap:int = 120, indent_namespace:bool = False ) -> None:
		self.lines = []
		self.indent_braces = indent_braces
		self.indentation = indentation
		self.tab_str = tab_str
		self.comment_wrap = comment_wrap
		self.indent_namespace = indent_namespace

	@contextmanager
	def blk(self, add_semicolon:bool = False, no_indent = False):
		'''Add an indented block of code'''
		try:
			st = self.tab_str * self.indentation
			if self.indent_braces:
				st += self.tab_str
			st += '{'
			self.lines.append(st)
			if not no_indent:
				self.indentation += 1	
			yield self

		finally:
			if not no_indent:
				self.indentation -= 1
			st = self.tab_str * self.indentation
			if self.indent_braces:
				st += self.tab_str 
			st += '}'
			if add_semicolon:
				st += ';'
			self.lines.append(st)

	@contextmanager
	def tab(self, prefix_ln:str = None):
		'''Add an indented block of code, only indent, no braces'''
		try:
			if prefix_ln:
				self.ln(prefix_ln)
			self.indentation += 1	
			yield self

		finally:
			self.indentation -= 1

	@contextmanager
	def ns(self, name:str, add_empty_line:bool = True):
		'''Add a namespace block of code'''
		try:
			self.ln('namespace ' + name)
			if self.indent_braces:
				self.ln( self.tab_str + '{')
			else:
				self.ln('{')
			if add_empty_line:
				self.ln()
			if self.indent_namespace:
				self.indentation += 1	
			yield self

		finally:
			if add_empty_line:
				self.ln()
			if self.indent_namespace:
				self.indentation -= 1
			if self.indent_braces:
				self.ln( self.tab_str + '}')
			else:
				self.ln('}')
			self.comment_ln('namespace ' + name)

	def ln(self, lin:str = None, no_indent = False, append = False, intent_adjust = 0 ):
		'''Add a single line, at current indentation, (or no indentation if no_indent is True).'''
		if append:
			if lin:
				self.lines[-1] += lin
		else:				
			if lin != None:
				if no_indent:
					self.lines.append( lin )
				else:
					self.lines.append( self.tab_str * (self.indentation + intent_adjust) + lin )
			else:
				self.lines.append('')
   
	# single line block
	def blk_ln(self, lin:str, add_comma:bool = False ):
		'''Add a single-line block, at the current indentation. Optionally add a comma at the end.'''
		st = ''
		if self.indent_braces:
			st += self.tab_str
		st += '{ ' + lin + ' }'
		if add_comma:
			st += ','
		self.ln( st )

	def comment_ln(self, comm:str=''):
		'''Add a commend, which is wrapped if longer than the comment_wrap length'''
		if len(comm)>0:
			lines = textwrap.wrap( comm , width = self.comment_wrap , initial_indent='// ' , subsequent_indent='// ' )
			for lin in lines:
				self.ln(lin)
		else:
			self.ln('//')

	def generate_license_header(
			self, 
			project_name:str=None, 
			copyright_holder:str = None, 
			license_type:str = None, 
			license_link:str = None, 
			autogeneration_warning:bool=True 
			):
		'''Generates the license header and injects into the generated c++ file'''
		project_name = project_name if project_name else default_project_name
		copyright_holder = copyright_holder if copyright_holder else default_copyright_holder
		license_type = license_type if license_type else default_license_type
		license_link = license_link if license_link else default_license_link

		self.comment_ln(f'{project_name} Copyright (c) {copyright_holder}')
		self.comment_ln(f'Licensed under the {license_type} license {license_link}')
		if autogeneration_warning:
			self.comment_ln()		
			self.comment_ln(f'Warning: This file is generated by {project_name}. Any manual edits will be overwritten by re-generation.')

	def begin_header_guard(self, file_name:str, project_name:str=None, prefix:str='', postfix:str='') -> str:
		'''Beings the header guard for the file'''
		project_name = project_name if project_name else default_project_name
		
		def todef(s:str):
			return s.upper().replace("-", "_").replace(" ", "_").replace(".", "_")
		
		guard_def = todef(f'{prefix}_{project_name}_{file_name}_{postfix}')
		self.ln('#pragma once')
		self.ln(f'#ifndef {guard_def}')
		self.ln(f'#define {guard_def}')
		return guard_def

	def end_header_guard(self, guard_def:str):
		'''Ends the header guard for the file'''
		self.ln(f'#endif//{guard_def}')

	@contextmanager
	def header_guard(self, file_name:str, project_name:str=default_project_name, prefix:str='', postfix:str='', add_empty_line:bool = True):
		'''Add a header guard surrounding the block of code.'''
		try:
			guard_str = self.begin_header_guard(file_name, project_name, prefix, postfix)
			if add_empty_line:
				self.ln()
			yield self

		finally:
			if add_empty_line:
				self.ln()
			end_header_guard = self.end_header_guard(guard_str)

	def inline_file(self, path:str):
		'''Injects the contents of another file, indented at the current indentation level'''
		inlined_file = open(path, 'r')
		lines = inlined_file.readlines()
		for str in lines:
			self.ln( str.rstrip() )
		inlined_file.close()

	def generate_push_and_disable_warnings(self, vswarns:list[str] , gccwarns:list[str] , comment:str = 'Disable warnings that are caused by code which we cannot control' ):
		'''Insert a block of warning disable pragmas for VC and GCC. Pushes all warnings before disabling.'''
		self.comment_ln(comment)
		self.ln('#ifdef _MSC_VER')
		self.ln('#pragma warning( push )')
		for w in vswarns:
			self.ln(f'#pragma warning( disable : {w} )')
		self.ln('#elif defined(__GNUC__)')
		self.ln('#pragma GCC diagnostic push')
		for w in gccwarns:
			self.ln(f'#pragma GCC diagnostic ignored "{w}"')	
		self.ln('#endif')

	def generate_pop_warnings(self, comment:str = 'Re-enable warnings again' ):
		'''Insert a block which pops the disabled warnings for VC or GCC'''
		self.comment_ln(comment)
		self.ln('#ifdef _MSC_VER')
		self.ln('#pragma warning( pop )')
		self.ln('#elif defined(__GNUC__)')
		self.ln('#pragma GCC diagnostic pop')
		self.ln('#endif')

	def write_lines_to_file( self, path:str ):
		'''Write the formatted text to a file, but only if the file is not already exactly identical to the formatted text'''

		# make into one long string
		new_text = ''
		for line in self.lines:
			new_text += line + '\n'

		# if the file already exists, check if we have an update
		if os.path.exists(path):
			# try reading in the current file if one exists
			with open(path,'r') as f:
				existing_text = f.read()
				f.close()

			# if no difference was found, return (but make sure it is read-only)
			if new_text == existing_text:
				print( '\tSkipping: ' + path + ', (it is identical)...')
				os.chmod(path, S_IRUSR|S_IRGRP|S_IROTH)
				return

			# if a difference was found, remove the old file
			os.chmod(path, S_IWUSR)
			os.remove( path ) 

		# we should write to the new file
		print( '\tWriting: ' + path + '...')
		with open(path,'w') as f:
			f.write(new_text)
			f.close()

		# change mode of file to readonly
		os.chmod(path, S_IRUSR|S_IRGRP|S_IROTH)


