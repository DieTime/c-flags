from typing import List
import glob
import re
from os import path, makedirs


def find_last_index(lines: List[str], regex: str) -> int:
    """Find the last occurrence of a string in an array by regex"""
    for rev_index, line in enumerate(reversed(lines)):
        if re.match(regex, line):
            return len(lines) - rev_index - 1


def get_headers_paths(lib_dir: str) -> List[str]:
    """Get all header files (*.h) paths from directory"""
    return sorted(glob.glob(path.join(lib_dir, "*.h")))


def get_sources_paths(lib_dir: str) -> List[str]:
    """Get all source files (*.c) paths from directory"""
    return sorted(glob.glob(path.join(lib_dir, "*.c")))


def is_std_include(line: str) -> bool:
    """Checks if the import is from the standard library, e.g. #include <stdio.h>"""
    return re.match(r'#include\s+<.+>', line)


def grab_std_includes(headers: List[str], sources: List[str]) -> List[str]:
    """Grab all std includes from headers and sources"""
    total_std_includes: List[str] = []

    for header in headers + sources:
        with open(header) as file:
            lines = file.readlines()

            std_include_lines = filter(is_std_include, lines)
            total_std_includes += std_include_lines

    std_includes_without_dup = list(set(total_std_includes))
    return sorted(std_includes_without_dup)


def grab_header_body(header: str) -> List[str]:
    """Grab header file body between `last include` and `#ifdef __cplusplus`"""
    with open(header) as file:
        lines = file.readlines()

        start = find_last_index(lines, '#include\s+.+')
        end = find_last_index(lines, '#ifdef\s+__cplusplus.*')

        return lines[start+1:end]


def grab_source_body(source_file: str) -> List[str]:
    """Grab source file body between `last include` and `end of file`"""
    with open(source_file) as file:
        lines = file.readlines()

        start = find_last_index(lines, '#include\s+.+')

        return lines[start+1:]


def generate_preamble(definition: str) -> List[str]:
    """Generate header file preamble lines"""
    return [
        f'#ifndef {definition}\n',
        f'#define {definition}\n',
        '\n'
        '#ifdef __cplusplus\n',
        'extern "C" {\n',
        '#endif\n',
        '\n'
    ]


def generate_conclusion(definition: str) -> List[str]:
    """Generate header file conclusion lines"""
    return [
        '\n',
        '#ifdef __cplusplus\n',
        '}\n',
        '#endif\n',
        '\n',
        f'#endif // {definition}\n'
    ]


def cleanup_double_endline(text: str) -> str:
    """Removes unnecessary empty lines in text"""
    return text.replace('\n\n\n', '\n')


def write_lines(lines: List[str], dest: str):
    """Write lines to output file creating directory if not exists"""
    makedirs(path.dirname(dest), exist_ok=True)

    with open(dest, 'w') as f:
        single_header = cleanup_double_endline(''.join(lines))
        f.write(single_header)


if __name__ == '__main__':
    LIB_SRC_DIR = '../lib'
    SINGLE_HEADER_DEST = '../single-header/c-flags.h'
    DEFINITION = 'C_FLAGS_SINGLE_HEADER_H'
    SINGLE_HEADER_LINES = []

    headers = get_headers_paths(LIB_SRC_DIR)
    sources = get_sources_paths(LIB_SRC_DIR)

    SINGLE_HEADER_LINES += generate_preamble(DEFINITION)
    SINGLE_HEADER_LINES += grab_std_includes(headers, sources)

    for header in headers:
        SINGLE_HEADER_LINES += grab_header_body(header)

    for source in sources:
        SINGLE_HEADER_LINES += grab_source_body(source)

    SINGLE_HEADER_LINES += generate_conclusion(DEFINITION)

    write_lines(SINGLE_HEADER_LINES, SINGLE_HEADER_DEST)
