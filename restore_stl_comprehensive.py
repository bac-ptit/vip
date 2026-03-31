import os
import re

HEADER_MAP = {
    'string': 'string',
    'string_view': 'string_view',
    'vector': 'vector',
    'map': 'map',
    'unordered_map': 'unordered_map',
    'set': 'set',
    'unordered_set': 'unordered_set',
    'list': 'list',
    'deque': 'deque',
    'array': 'array',
    'shared_ptr': 'memory',
    'unique_ptr': 'memory',
    'make_shared': 'memory',
    'make_unique': 'memory',
    'function': 'functional',
    'bind': 'functional',
    'move': 'utility',
    'forward': 'utility',
    'pair': 'utility',
    'tuple': 'utility',
    'optional': 'optional',
    'variant': 'variant',
    'expected': 'expected',
    'unexpected': 'expected',
    'cout': 'iostream',
    'cerr': 'iostream',
    'endl': 'iostream',
    'format': 'format',
    'exception': 'exception',
    'runtime_error': 'stdexcept',
    'invalid_argument': 'stdexcept',
    'out_of_range': 'stdexcept',
    'logic_error': 'stdexcept',
    'chrono': 'chrono',
    'ranges': 'ranges',
    'views': 'ranges',
    'sort': 'algorithm',
    'find': 'algorithm',
    'copy': 'algorithm',
    'transform': 'algorithm',
    'reverse': 'algorithm',
    'max': 'algorithm',
    'min': 'algorithm',
    'any': 'any',
    'atomic': 'atomic',
    'bitset': 'bitset',
    'compare': 'compare',
    'concepts': 'concepts',
    'filesystem': 'filesystem',
    'fstream': 'fstream',
    'future': 'future',
    'mutex': 'mutex',
    'numeric': 'numeric',
    'span': 'span',
    'sstream': 'sstream',
    'thread': 'thread',
    'type_traits': 'type_traits',
    'accumulate': 'numeric',
    'iota': 'numeric',
    'uint32_t': 'cstdint',
    'int32_t': 'cstdint',
    'uint64_t': 'cstdint',
    'int64_t': 'cstdint',
    'size_t': 'cstddef',
    'regex': 'regex',
    'regex_match': 'regex',
    'regex_replace': 'regex',
}

def get_required_headers(content):
    headers = set()
    # Match std::identifier
    matches = re.findall(r'std::([a-zA-Z0-9_]+)', content)
    for match in matches:
        if match in HEADER_MAP:
            headers.add(HEADER_MAP[match])
    
    # Special regex-based detections
    if re.search(r'std::chrono\b', content):
        headers.add('chrono')
    if re.search(r'std::ranges\b', content):
        headers.add('ranges')
    if re.search(r'std::views\b', content):
        headers.add('ranges')
    if re.search(r'std::filesystem\b', content):
        headers.add('filesystem')
    if 'std::exception' in content:
        headers.add('exception')
    
    return sorted(list(headers))

def process_file(filepath):
    with open(filepath, 'r') as f:
        content = f.read()
    
    has_import_std = 'import std;' in content
    required_headers = get_required_headers(content)
    
    if not has_import_std and not required_headers:
        return

    # Check which headers are already included
    already_included = set(re.findall(r'#include <([^>]+)>', content))
    missing_headers = [h for h in required_headers if h not in already_included]
    
    if not has_import_std and not missing_headers:
        return

    print(f"Processing {filepath}...")
    
    header_lines = [f'#include <{h}>' for h in missing_headers]
    
    if filepath.endswith('.cppm'):
        lines = content.splitlines(keepends=True)
        # Remove import std;
        lines = [l for l in lines if l.strip() != 'import std;']
        
        # Find where to insert headers
        module_fragment_line_idx = -1
        for i, line in enumerate(lines):
            if line.strip() == 'module;':
                module_fragment_line_idx = i
                break
        
        if module_fragment_line_idx != -1:
            # Insert missing headers after module;
            for h_line in reversed(header_lines):
                lines.insert(module_fragment_line_idx + 1, h_line + '\n')
        else:
            # Check if it's a module unit but missing module; fragment
            module_decl_idx = -1
            for i, line in enumerate(lines):
                if line.startswith('export module') or line.startswith('module '):
                    module_decl_idx = i
                    break
            
            if module_decl_idx != -1:
                # Prepend module; fragment
                new_header_block = ['module;\n'] + [l + '\n' for l in header_lines] + ['\n']
                lines = new_header_block + lines
            else:
                # Not a module unit, just a regular file with .cppm extension?
                # Prepend headers
                lines = [l + '\n' for l in header_lines] + ['\n'] + lines
        
        content = "".join(lines)
    else:
        # Non-module files (.cc, .cpp, .h)
        # Remove import std;
        content = content.replace('import std;', '')
        
        if missing_headers:
            lines = content.splitlines(keepends=True)
            # Find first #include or first non-comment line
            insert_idx = 0
            for i, line in enumerate(lines):
                if line.startswith('#include') or line.startswith('#pragma once'):
                    insert_idx = i
                    if line.startswith('#pragma once'):
                        insert_idx = i + 1
                    break
                elif line.strip() and not line.strip().startswith('//') and not line.strip().startswith('/*'):
                    insert_idx = i
                    break
            
            for h_line in reversed(header_lines):
                lines.insert(insert_idx, h_line + '\n')
            content = "".join(lines)
    
    with open(filepath, 'w') as f:
        f.write(content)

def main():
    extensions = ('.cc', '.cpp', '.h', '.cppm')
    for root, dirs, files in os.walk('.'):
        if 'build' in dirs:
            dirs.remove('build')
        if 'cmake-build-debug' in dirs:
            dirs.remove('cmake-build-debug')
            
        for file in files:
            if file.endswith(extensions):
                filepath = os.path.join(root, file)
                process_file(filepath)

if __name__ == "__main__":
    main()
