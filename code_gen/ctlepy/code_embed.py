import re, difflib, traceback, sys
from enum import Enum
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

def print_info( text:str ):
    print( f'\033[94m{text}\033[0m' )
def print_warn( text:str ):
    print( f'\033[93mWarning: {text}\033[0m', file=sys.stderr )
def print_err( text:str ):
    print( f'\033[91mError: {text}\033[0m', file=sys.stderr )
    
class CaseStyle(Enum):
    SNAKE = 'snake_case'
    CAMEL = 'camelCase'
    PASCAL = 'PascalCase'
    KEBAB = 'kebab-case'
    UPPER = 'UPPER_CASE'
    TITLE = 'Title Case'

def case_convert(symbol: str, style: CaseStyle) -> str:
    '''
    Takes in a symbol defined using snake case, and returns in the selected case style.
    '''
    parts = symbol.strip().lower().split('_')
    if style == CaseStyle.SNAKE:
        return '_'.join(parts)
    elif style == CaseStyle.CAMEL:
        return parts[0] + ''.join(word.capitalize() for word in parts[1:])
    elif style == CaseStyle.PASCAL:
        return ''.join(word.capitalize() for word in parts)
    elif style == CaseStyle.KEBAB:
        return '-'.join(parts)
    elif style == CaseStyle.UPPER:
        return '_'.join(parts).upper()
    elif style == CaseStyle.TITLE:
        return ' '.join(word.capitalize() for word in parts)
    else:
        raise ValueError(f'Unsupported case style: {style}')

def find_matching_files( main_file:Path, exts: list[str] ) -> list[Path]:
    '''
    Find files related to main_file using a list of extensions
    '''
    file_list = []
    for ext in exts:
        variant = main_file.with_suffix(ext)
        if variant.exists():
            file_list.append(variant)
    return file_list

def parse_pattern(p):
    '''Interprets a pattern, assumes a verbatim lable if not prefixed with 'regex:' '''
    return p[6:] if p.startswith('regex:') else re.escape(p)

@dataclass
class StyleType:
    case: CaseStyle
    prefix: str = ''
    postfix: str = ''

    def style_symbol( self, symbol: str ) -> str:
        '''Returns the symbol styled as this style type'''
        return self.prefix + case_convert( symbol=(symbol), style=self.case ) + self.postfix

@dataclass
class CodeBlock:
    begin: str
    end: str
    def get_begin_re( self ): return parse_pattern( self.begin )
    def get_end_re( self ): return parse_pattern( self.end )

@dataclass
class GenBehavior:
    property_style: StyleType = field(default_factory=lambda: StyleType( case=CaseStyle.PASCAL ))
    variable_style: StyleType = field(default_factory=lambda: StyleType( case=CaseStyle.CAMEL, postfix='_' ))
    method_style: StyleType = field(default_factory=lambda: StyleType( case=CaseStyle.PASCAL ))
    class_style: StyleType = field(default_factory=lambda: StyleType( case=CaseStyle.PASCAL ))
    main_code_block: CodeBlock = field(default_factory=lambda: CodeBlock( begin='/*CODE_GEN_BLOCK', end='*/' ))    

    def find_files( self, main_file:Path ) -> list[Path]:
        '''
        Find files related to the main file, such as header declarations, or inline code files.
        The list will contain the main file, if it exists.
        If main_file is a C++ file, will look for other C++ files matching the title of the main file, and in the same folder as main file.
        '''
        cpp_files = ['.cpp','.cxx','.cc','.h','.hxx','.inl']
        if main_file.suffix in cpp_files:
            return find_matching_files(main_file, cpp_files)

        return [main_file] if main_file.exists() else []

def find_embedded_code_block( text:str, start_pattern:str, end_pattern:str):
    '''
    Extracts the first block of text between start_pattern and end_pattern. text (str): The input text to search.
    start_pattern: Pattern marking the start of the block. Prefix with 'regex:' for regex. end_pattern: Pattern 
    marking the end of the block. Prefix with 'regex:' for regex. Returns the matched block, or None if no match is found.
    '''
    start_re = parse_pattern(start_pattern)
    end_re = parse_pattern(end_pattern)

    pattern = f'{start_re}(.*?){end_re}'

    match = re.search(pattern, text, re.DOTALL)
    if match:
        return match.group(1).strip()
    return None

def extract_blocks(text, tag_dict):
    '''
    Extracts and labels blocks of text between start/end tag pairs, along with surrounding non-tagged plaintext.
    text: The full input text. tag_dict: Keys are block names, values are (start_tag, end_tag) tuples, which can be regex
    Returns a list of dict: Each dict has 'type' and 'content' keys.
    '''
    named_patterns = []
    for name, (start, end) in tag_dict.items():
        start_re = parse_pattern(start)
        end_re = parse_pattern(end)
        named_patterns.append(f'(?P<{name}>{start_re}.*?{end_re})')

    combined_pattern = '|'.join(named_patterns)
    blocks = []
    last_end = 0

    for match in re.finditer(combined_pattern, text, re.DOTALL):
        start, end = match.span()
        if start > last_end:
            blocks.append({'type': 'plaintext', 'content': text[last_end:start]})
        for name in match.groupdict():
            if match.group(name):
                blocks.append({'type': name, 'content': match.group(name)})
                break
        last_end = end

    if last_end < len(text):
        blocks.append({'type': 'plaintext', 'content': text[last_end:]})

    return blocks

def run_code(code:str, start_line:int, source_file_path:Path):
    '''
    Runs the supplied code block in the current context. 
    If an error occurs, prints the error and the line, and exits
    '''
    try:
        exec(code)
    except Exception as e:
        tb = traceback.extract_tb(e.__traceback__)
        source_lines = code.splitlines()
        print_info('Traceback (most recent call last):')
        for entry in tb:
            if entry.lineno != None:
                if entry.filename == '<string>':
                    print_info(f'  File "{source_file_path}", line {entry.lineno + start_line}')
                    if entry.line != None and len(entry.line.strip()) > 0:
                        print_info(f'    {entry.line}')
                    else:
                        print_info(f'    {source_lines[entry.lineno-1]}')
                elif entry.filename != None:
                    print_info(f'  File "{entry.filename}", line {entry.lineno}')
                    if entry.line != None:
                        print_info(f'    {entry.line}')
            elif entry.line != None:
                print_info('  File: <no file found>')
                print_info(f'    {entry.line}')
        print_err(f'{e}')
        exit(-1)

def run_main_file_code_block(main_file:Path):
    '''Finds the code block in the main file, and runs it using run_code()'''
    with open(main_file ,"r") as file:
        file_conts = file.read()
        code = find_embedded_code_block(file_conts,beh.main_code_block.begin, beh.main_code_block.end)
    if code == None:
        print_err(f'Could not find a code block in file: "{main_file}"')
        exit(-2)
    run_code(code, 0, Path(main_file))

#####################################################################

class Access(Enum):
    NONE = 0x0
    PRIVATE = 0x1
    PROTECTED = 0x2
    PUBLIC = 0x3

class RefType(Enum):
    COPY = 0x10
    REF = 0x20
    OPERATOR = 0x30

@dataclass
class Property:
    name:str
    type:str
    default_value:Any
    static:bool = False
    const:bool = False
    gets:list



if __name__ == "__main__":
    main_file = Path('C:/dev/gen/example.cpp')

    beh = GenBehavior()
    file_list = beh.find_files(main_file=main_file)

    run_main_file_code_block(main_file=main_file)

# with open("C:/dev/gen/test.h","r") as file:
    # contents = file.read()

# tag_dict = {
    # "getset_block": ("//<GETSET", "//>"),
    # "prop_block": ("//<PROP", "//>"),
    # "ser_block": ("//<SER", "//>"),
# }

# mod = ''
# segments = extract_blocks(contents, tag_dict)
# for segment in segments:
    # type = segment['type']
    # #print(type)
    # if type == 'plaintext':
        # mod += segment['content']
    # else:
        # mod += tag_dict[type][0] + '\n' + 'modified' + '\n' + tag_dict[type][1]

# #with open("C:/dev/gen/test.out.h","w") as file:
# #    file.write( mod )

# #with open("C:/dev/gen/test.h") as f1, open("C:/dev/gen/test.out.h") as f2:
# #    original = f1.readlines()
# #    modified = f2.readlines()

# original = contents.splitlines()
# modified = mod.splitlines()

# diff = difflib.unified_diff(
    # original,
    # modified,
    # fromfile='test.h',
    # tofile='test.h',
    # lineterm=''
# )

# with open("C:/dev/gen/changes.patch", "w") as patch_file:
    # for ln in diff:
        # patch_file.write( ln.rstrip() + '\n' )