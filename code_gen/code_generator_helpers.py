# ctle Copyright (c) 2023 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

import copy
import os
from stat import S_IRUSR, S_IRGRP, S_IROTH, S_IWUSR
import importlib
import textwrap

class formatted_output:
	def __init__(self, indentation:int = 0, tab_str:str = '\t', indent_braces:bool = True, comment_wrap:int = 80 ) -> None:
		self.lines = []
		self.indent_braces = indent_braces
		self.indentation = indentation
		self.tab_str = tab_str
		self.comment_wrap = comment_wrap

	def begin_block(self):
		st = self.tab_str * self.indentation
		if self.indent_braces:
			st += self.tab_str
		st += '{'
		self.lines.append(st)
		self.indentation += 1	

	def end_block(self, add_semicolon:bool = False ):
		self.indentation -= 1
		st = self.tab_str * self.indentation
		if self.indent_braces:
			st += self.tab_str 
		st += '}'
		if add_semicolon:
			st += ';'
		self.lines.append(st)

	def ln(self, lin:str = None ):
		if lin != None:
			self.lines.append( self.tab_str * self.indentation + lin )
		else:
			self.lines.append('')

	# single line indent, only applies to indented braces
	def indented_ln(self, lin:str = None ):
		if self.indent_braces:
			self.ln( self.tab_str + lin )
		else:
			self.ln( lin )

	def comment_ln(self, comm:str):
		lines = textwrap.wrap( comm , width = self.comment_wrap )
		for lin in lines:
			self.ln('// ' + textwrap.dedent(lin) )

	def get_lines(self) -> list[str]:
		return self.lines

# reads a file and output lines
def inline_file( path ):
	inlined_file = open(path, 'r')
	lines = inlined_file.readlines()
	strip_lines = []
	for str in lines:
		strip_lines.append( str.rstrip() )
	inlined_file.close()
	return strip_lines

def generate_header():
	lines = []
	lines.append('// ctle Copyright (c) 2022 Ulrik Lindahl')
	lines.append('// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE')
	lines.append('')
	return lines

def generate_push_and_disable_warnings( vswarns , gccwarns , comment = 'disable warnings in code we cannot control' ):
	lines = []
	lines.append(f'// {comment}')
	lines.append('#ifdef _MSC_VER')
	lines.append('#pragma warning( push )')
	for w in vswarns:
		lines.append(f'#pragma warning( disable : {w} )')
	lines.append('#elif defined(__GNUC__)')
	lines.append('#pragma GCC diagnostic push')
	for w in gccwarns:
		lines.append(f'#pragma GCC diagnostic ignored "{w}"')	
	lines.append('#endif')
	return lines

def generate_pop_warnings( comment = 're-enable warnings again' ):
	lines = []
	lines.append(f'// {comment}')
	lines.append('#ifdef _MSC_VER')
	lines.append('#pragma warning( pop )')
	lines.append('#elif defined(__GNUC__)')
	lines.append('#pragma GCC diagnostic pop')
	lines.append('#endif')
	return lines

def write_lines_to_file( path , lines ):
	# make into one long string
	new_text = ''
	for line in lines:
		new_text += line + '\n'

	# if the file already exists, check if we have an update
	if os.path.exists(path):
		# try reading in the current file if one exists
		with open(path,'r') as f:
			existing_text = f.read()
			f.close()

		#if no difference was found, return
		if new_text == existing_text:
			print( '\tSkipping: ' + path + ', (it is identical)...')
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