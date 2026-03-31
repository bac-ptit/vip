import os
import re

# Mapping of common std types to their headers
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
    'tuple': 'tuple',
    'optional': 'optional',
    'variant': 'variant',
    'expected': 'expected',
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
}

FILES_TO_REPLACE = [
    'controllers/api_v1_ActivityLogs.cc',
    'controllers/api_v1_ActivityLogs.h',
    'controllers/api_v1_Admin.cc',
    'controllers/api_v1_Admin.h',
    'controllers/api_v1_AdminSessions.cc',
    'controllers/api_v1_AdminSessions.h',
    'controllers/api_v1_Auth.cc',
    'controllers/api_v1_Auth.h',
    'controllers/api_v1_FloorPlans.cc',
    'controllers/api_v1_FloorPlans.h',
    'controllers/api_v1_Media.cc',
    'controllers/api_v1_Media.h',
    'controllers/api_v1_Product.cc',
    'controllers/api_v1_Product.h',
    'controllers/api_v1_ProjectInfo.cc',
    'controllers/api_v1_ProjectInfo.h',
    'controllers/api_v1_RegistrationInterests.cc',
    'controllers/api_v1_RegistrationInterests.h',
    'controllers/api_v1_Registrations.cc',
    'controllers/api_v1_Registrations.h',
    'controllers/api_v1_SiteSettings.cc',
    'controllers/api_v1_SiteSettings.h',
    'domains/admin.cpp',
    'filters/AuthFilter.cc',
    'main.cc',
    'plugins/ImgBBPlugin.cc',
    'repositories/admin_repository.cpp',
    'services/activity_logs_service.cc',
    'services/admin_service.cpp',
    'services/admin_sessions_service.cc',
    'services/floor_plans_service.cc',
    'services/media_service.cc',
    'services/product_images_service.cc',
    'services/product_service.cc',
    'services/project_info_service.cc',
    'services/registration_interests_service.cc',
    'services/registrations_service.cc',
    'services/site_settings_service.cc',
    'test/test_auth.cc',
    'test/test_main.cc',
    'utils/password_hasher.cpp',
]

def process_cc_file(filepath):
    if not os.path.exists(filepath):
        print(f"File not found: {filepath}")
        return
    
    with open(filepath, 'r') as f:
        content = f.read()
    
    if 'import std;' not in content:
        return

    # Find all std:: usages
    std_usages = re.findall(r'std::([a-zA-Z0-9_]+)', content)
    headers = set()
    for usage in std_usages:
        if usage in HEADER_MAP:
            headers.add(HEADER_MAP[usage])
    
    # Special cases
    if 'std::chrono' in content:
        headers.add('chrono')
    if 'std::ranges' in content:
        headers.add('ranges')
    if 'std::views' in content:
        headers.add('ranges')
    if 'std::exception' in content or 'std::exception&' in content:
        headers.add('exception')

    # If no headers found but import std; was there, maybe it's using something not in map
    # Add common ones just in case
    if not headers:
        headers.add('string')
        headers.add('vector')

    header_includes = "\n".join([f'#include <{h}>' for h in sorted(list(headers))])
    
    new_content = content.replace('import std;', header_includes)
    
    with open(filepath, 'w') as f:
        f.write(new_content)
    print(f"Processed: {filepath}")

CPPM_FILES = [
    'exception/custom_exception.cppm',
    'repositories/registration_interests_repository.cppm',
    'repositories/media_repository.cppm',
    'repositories/floor_plans_repository.cppm',
    'repositories/activity_logs_repository.cppm',
    'domains/site_settings.cppm',
    'domains/registrations.cppm',
    'domains/registration_interests.cppm',
    'domains/product_images.cppm',
    'domains/project_info.cppm',
    'domains/product.cppm',
    'domains/media.cppm',
    'domains/floor_plans.cppm',
    'domains/admin_sessions.cppm',
    'domains/activity_logs.cppm',
    'domains/domain.cppm',
    'services/admin_service.cppm',
    'domains/admin.cppm',
    'services/admin_sessions_service.cppm',
    'services/registration_interests_service.cppm',
    'services/activity_logs_service.cppm',
    'services/product_images_service.cppm',
    'services/floor_plans_service.cppm',
    'services/site_settings_service.cppm',
    'services/project_info_service.cppm',
    'services/media_service.cppm',
    'services/registrations_service.cppm',
    'repositories/project_info_repository.cppm',
    'repositories/product_repository.cppm',
    'repositories/admin_sessions_repository.cppm',
    'repositories/site_settings_repository.cppm',
    'repositories/registrations_repository.cppm',
    'repositories/product_images_repository.cppm',
    'repositories/admin_repository.cppm',
    'utils/password_hasher.cppm',
    'dto/site_settings_dto.cppm',
    'dto/registrations_dto.cppm',
    'dto/registration_interests_dto.cppm',
    'dto/project_info_dto.cppm',
    'dto/product_images_dto.cppm',
    'dto/product_dto.cppm',
    'dto/media_dto.cppm',
    'dto/floor_plans_dto.cppm',
    'dto/admin_sessions_dto.cppm',
    'dto/admin_dto.cppm',
    'dto/activity_logs_dto.cppm',
    'services/product_service.cppm',
    'dto/dto.cppm',
    'repositories/repo.cppm',
    'services/service.cppm',
    'utils/utils.cppm',
]

def process_cppm_file(filepath):
    if not os.path.exists(filepath):
        print(f"File not found: {filepath}")
        return
    
    with open(filepath, 'r') as f:
        lines = f.readlines()
    
    new_lines = []
    in_global_fragment = False
    import_std_found = False
    module_decl_index = -1
    
    # First pass: find import std; and module declaration
    for i, line in enumerate(lines):
        if line.strip() == 'module;':
            in_global_fragment = True
        elif line.startswith('export module') or line.startswith('module '):
            module_decl_index = i
            in_global_fragment = False
        elif line.strip() == 'import std;':
            import_std_found = True
    
    # Second pass: remove STL includes from global fragment and re-place import std;
    for i, line in enumerate(lines):
        # Remove import std; from everywhere (we'll re-add it)
        if line.strip() == 'import std;':
            continue
            
        # If in global fragment, check for STL includes
        if 'module;' in line:
            in_global_fragment = True
            new_lines.append(line)
            continue
        
        if line.startswith('export module') or line.startswith('module '):
            in_global_fragment = False
            new_lines.append(line)
            # Immediately add import std; after module declaration
            new_lines.append('import std;\n')
            continue
            
        if in_global_fragment:
            # Simple check for STL headers in global fragment
            stl_headers = [f'<{h}>' for h in HEADER_MAP.values()]
            is_stl = False
            for h in stl_headers:
                if f'#include {h}' in line:
                    is_stl = True
                    break
            if is_stl:
                continue
        
        new_lines.append(line)

    with open(filepath, 'w') as f:
        f.writelines(new_lines)
    print(f"Processed CPPM: {filepath}")

for f in FILES_TO_REPLACE:
    process_cc_file(f)

for f in CPPM_FILES:
    process_cppm_file(f)
