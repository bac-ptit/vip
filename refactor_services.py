import os
import re

directory = 'services/'
stl_headers = [
    'string', 'vector', 'memory', 'optional', 'ranges', 'mutex',
    'shared_mutex', 'stdexcept', 'map', 'unordered_map', 'iostream',
    'filesystem', 'print', 'chrono', 'atomic', 'any', 'variant', 'string_view'
]

stl_pattern = re.compile(r'#include <(' + '|'.join(stl_headers) + r')>')

for filename in os.listdir(directory):
    if filename.endswith('.cc'):
        filepath = os.path.join(directory, filename)
        print(f"Processing {filepath}")
        with open(filepath, 'r') as f:
            lines = f.readlines()

        new_lines = []
        found_module_service = False
        import_std_added = False
        
        for i, line in enumerate(lines):
            stripped = line.strip()
            if not found_module_service:
                if stl_pattern.search(line):
                    # print(f"Removing STL header: {line.strip()}")
                    continue
                if stripped.startswith('module service;'):
                    # print(f"Found module service; at line {i}")
                    found_module_service = True
                    new_lines.append(line)
                    continue
                new_lines.append(line)
            else:
                if not import_std_added:
                    if stripped == 'import std;':
                        # print(f"Found already existing import std; at line {i}")
                        import_std_added = True
                        new_lines.append(line)
                        continue
                    if stripped == '':
                        # Skip multiple empty lines after module service; until we insert import std;
                        continue
                    
                    # print(f"Inserting import std; before line {i}: {stripped}")
                    new_lines.append('import std;\n\n')
                    import_std_added = True
                    new_lines.append(line)
                else:
                    new_lines.append(line)
        
        if not import_std_added and found_module_service:
            # If we never added it (e.g., file ends with module service; and whitespace)
            new_lines.append('\nimport std;\n')

        # print(f"Final new_lines count for {filepath}: {len(new_lines)} (original: {len(lines)})")
        with open(filepath, 'w') as f:
            f.writelines(new_lines)
