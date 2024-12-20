# ctle Copyright (c) 2024 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

import os
from ctlepy.file_checker import line_define
from ctlepy.file_checker import check_file_lines
from ctlepy.file_checker import fix_file
import pathlib

# Function to check both copyright and #ifdef in files
def check_file(file_path: str, is_header:bool, fix_fails:bool) -> bool:
	print(f"Checking file: {file_path}")
	defs = [
		line_define(1,r"// ctle Copyright \(c\) \d{4} Ulrik Lindahl","// ctle Copyright (c) 2024 Ulrik Lindahl"),
		line_define(2,"// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE")
	]

	if is_header:
		file_title, extension = os.path.splitext(os.path.basename(file_path))
		file_title = file_title.upper().replace("-", "_").replace(" ", "_")
		extension = extension[1:].upper()  # Remove the leading dot and convert to uppercase
		file_def = f"_CTLE_{file_title}_{extension}_"		

		defs.extend( [
			line_define(3,"#pragma once"),
			line_define(4,f"#ifndef {file_def}"),
			line_define(5,f"#define {file_def}"),
			line_define(-1,f"#endif//{file_def}"),
		] )

	if len(check_file_lines(file_path, defs)) > 0:
		if not fix_fails:
			return False
		else:
			print(f"Fixing file: {file_path}")
			return fix_file(file_path, defs)

	return True

# Function to traverse directories and check files
def check_files_in_directory(directory):
	invalid_headers = []
	for root, _, files in os.walk(directory):
		for file in files:
			if file.endswith(".cpp") or file.endswith(".h") or file.endswith(".inl"):
				file_path = os.path.join(root, file)
				if not check_file(file_path, file.endswith(".h"), False):
					invalid_headers.append(file_path)
	return invalid_headers

if __name__ == "__main__":
	ctle_root = pathlib.Path(__file__).parent.parent.resolve()

	invalid_headers = []
	source_directories = ["ctle","unit_tests"]
	for dir in source_directories:
		invalid_headers.extend(check_files_in_directory(ctle_root / dir))

	if len(invalid_headers) > 0:
		print("\033[31mError: Invalid headers in files:")
		for f in invalid_headers:
			print(f)
		print("\033[0m")
		exit(-1)
	else:
		print("All files checks out ok")
		exit(0)
