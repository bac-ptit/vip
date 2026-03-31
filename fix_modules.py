import os
import re

def fix_module_structure(file_path):
    with open(file_path, 'r') as f:
        content = f.read()

    # Split into lines
    lines = content.splitlines(keepends=True)
    
    has_includes = any(line.strip().startswith('#include') for line in lines)
    if not has_includes:
        # Just remove import std; if present
        new_lines = []
        changed = False
        for line in lines:
            if line.strip() == 'import std;':
                changed = True
                continue
            new_lines.append(line)
        if changed:
            with open(file_path, 'w') as f:
                f.writelines(new_lines)
            return True
        return False

    # Find all includes
    includes = [line for line in lines if line.strip().startswith('#include')]
    
    # Find module declaration
    module_decl = None
    module_decl_idx = -1
    for i, line in enumerate(lines):
        stripped = line.strip()
        if (stripped.startswith('export module ') or (stripped.startswith('module ') and not stripped.startswith('module;'))) and not stripped.endswith(';'):
            # Multi-line module decl? Unlikely but possible. 
            # For this project, they seem to be single line.
            pass
        if (stripped.startswith('export module ') or (stripped.startswith('module ') and not stripped.startswith('module;'))):
            module_decl = line
            module_decl_idx = i
            break

    # If no module declaration, treat as non-module file but still follow Rule 1?
    # "Every file MUST start with 'module;' if it has any #include."
    # Rule 2: "ALL #include directives ... MUST be placed between 'module;' and the 'export module' or 'module' declaration."
    # If there's no module declaration, Rule 2 is hard to follow strictly.
    # But usually these are either .cppm (always modules) or .cpp (module implementation or regular).
    
    # Collect everything else
    other_lines_before_module = []
    other_lines_after_module = []
    
    found_module_decl = False
    for i, line in enumerate(lines):
        stripped = line.strip()
        if stripped.startswith('#include'):
            continue
        if stripped == 'import std;':
            continue
        if stripped == 'module;':
            continue
            
        if module_decl and line == module_decl:
            found_module_decl = True
            continue
            
        if not found_module_decl:
            if module_decl:
                other_lines_before_module.append(line)
            else:
                other_lines_after_module.append(line)
        else:
            other_lines_after_module.append(line)

    # Reconstruct
    new_lines = []
    new_lines.append('module;\n')
    
    # Ensure includes are clean (no duplicates, maybe sorted?)
    # Let's keep them as is but unique and stripped of extra newlines
    seen_includes = set()
    for inc in includes:
        if inc not in seen_includes:
            new_lines.append(inc)
            seen_includes.add(inc)
    
    if module_decl:
        # Move comments that were before module; or between includes/module decl?
        # The rules are strict: module; then includes then module decl.
        # Any top-level comments should probably go after includes but before module decl,
        # OR if they are license-like, maybe before module;?
        # Example says "module;" is line 1.
        
        # Let's put other_lines_before_module after includes
        for line in other_lines_before_module:
            new_lines.append(line)
            
        new_lines.append(module_decl)
        
        for line in other_lines_after_module:
            new_lines.append(line)
    else:
        # No module declaration found.
        # Just module; then includes then the rest.
        for line in other_lines_after_module:
            new_lines.append(line)

    # Final cleanup: ensure no trailing whitespace on empty lines, etc.
    # and exactly one module; at the top.
    
    # Check if changed
    if "".join(lines) == "".join(new_lines):
        # Even if content is same, check if 'module;' was first
        if lines and lines[0].strip() == 'module;':
            return False
        # If it wasn't first, we should fix it.
        if not lines: return False

    with open(file_path, 'w') as f:
        f.writelines(new_lines)
    return True

def main():
    files_to_process = []
    for root, dirs, files in os.walk('.'):
        # Exclude models/ directory as requested
        if 'models' in root.split(os.sep):
            continue
            
        if 'build' in dirs:
            dirs.remove('build')
        if 'cmake-build-debug' in dirs:
            dirs.remove('cmake-build-debug')
        if '.git' in dirs:
            dirs.remove('.git')
            
        for file in files:
            if file.endswith('.cppm') or file.endswith('.cpp'):
                files_to_process.append(os.path.join(root, file))

    count = 0
    for file_path in files_to_process:
        # Skip models/ double check
        if 'models/' in file_path:
            continue
        if fix_module_structure(file_path):
            print(f"Fixed: {file_path}")
            count += 1
    
    print(f"Total files fixed: {count}")

if __name__ == "__main__":
    main()
