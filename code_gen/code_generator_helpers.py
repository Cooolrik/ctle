# pds - Persistent data structure framework, Copyright (c) 2023 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

import copy
import os
from stat import S_IRUSR, S_IRGRP, S_IROTH, S_IWUSR
import importlib

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