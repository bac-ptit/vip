import os
import re

files = [
    "repositories/admin_repository.cppm",
    "repositories/admin_repository.cpp",
    "repositories/product_repository.cppm",
    "repositories/media_repository.cppm",
    "repositories/floor_plans_repository.cppm",
    "repositories/project_info_repository.cppm",
    "repositories/registrations_repository.cppm",
    "repositories/registration_interests_repository.cppm",
    "repositories/admin_sessions_repository.cppm",
    "repositories/activity_logs_repository.cppm",
    "repositories/site_settings_repository.cppm"
]

def update_file(file_path):
    with open(file_path, 'r') as f:
        content = f.read()

    # 1. Add #include <ranges> if not present and if it looks like we need it
    if "std::views::transform" in content or "FindAll" in content or "FindActive" in content:
        if "#include <ranges>" not in content:
            # Find the last include or the module line
            if "module;" in content:
                content = content.replace("module;", "module;\n#include <ranges>")
            elif "#include" in content:
                content = re.sub(r'(#include <.*?>)', r'\1\n#include <ranges>', content, count=1)
            else:
                content = "#include <ranges>\n" + content

    # 2. Replace std::string_view/const std::string& with std::string in Task<> functions
    # We look for functions returning Task<> and their parameters
    # This is complex with regex, but we can try common patterns
    
    # Replace parameters in declarations and implementations
    content = content.replace("std::string_view", "std::string")
    content = content.replace("const std::string&", "std::string")

    # 3. Use std::move() when passing these strings to mapper methods or other coroutines
    # common patterns:
    # mapper.findByPrimaryKey(std::string{id}) -> mapper.findByPrimaryKey(std::move(id))
    # mapper.findByPrimaryKey(std::string(id)) -> mapper.findByPrimaryKey(std::move(id))
    # std::string{id} -> std::move(id)
    # std::string(id) -> std::move(id)
    content = re.sub(r'std::string\{(\w+)\}', r'std::move(\1)', content)
    content = re.sub(r'std::string\((\w+)\)', r'std::move(\1)', content)
    
    # 4. Add [[nodiscard]] to methods if missing (especially in .cpp)
    # Look for drogon::Task< and ensure [[nodiscard]] is before it
    lines = content.split('\n')
    new_lines = []
    for line in lines:
        if "drogon::Task<" in line and "[[nodiscard]]" not in line and "co_return" not in line and "co_await" not in line:
            # Check if it's a function definition/declaration
            if line.strip().startswith("drogon::Task<") or line.strip().startswith("Task<"):
                line = line.replace("drogon::Task<", "[[nodiscard]] drogon::Task<")
                line = line.replace("Task<", "[[nodiscard]] Task<")
        new_lines.append(line)
    content = '\n'.join(new_lines)

    # 5. Modernize collection transformations
    # Pattern for FindAll/FindActive/...
    # std::vector<domain::Type> domain_results;
    # domain_results.reserve(results.size());
    # for (auto& r : results) {
    #   domain_results.emplace_back(std::move(r));
    # }
    # co_return domain_results;
    
    # We'll use a more generic approach to find these blocks
    # This is tricky with regex, let's try a common structure
    
    def transform_ranges(match):
        results_var = match.group(1)
        target_type = match.group(2)
        return f"co_return {results_var} | std::views::transform([](auto& item) {{ return {target_type}{{std::move(item)}}; }}) | std::ranges::to<std::vector>();"

    # Match the loop pattern
    # Group 1: results variable name
    # Group 2: target type
    pattern = re.compile(r'auto\s+(\w+)\s*\{\s*co_await\s+mapper\.(?:findAll|findBy)\(.*\)\s*\};\s*std::vector<([\w:]+)>\s+\w+;\s+\w+\.reserve\(\w+\.size\(\)\);\s*for\s*\(auto&\s+\w+\s*:\s+\w+\)\s*\{\s*\w+\.emplace_back\(std::move\(\w+\)\);\s*\}\s*co_return\s+\w+;')
    # The above is too specific and might fail due to whitespace or minor variations.
    
    # Let's try to find the loop and replace it.
    # We can use a simpler regex to identify the pattern and then replace the whole block.
    
    # I'll manually handle the most common ones in this script or just use the generalist.
    # Actually, the generalist is better at this than a script.
    
    with open(file_path, 'w') as f:
        f.write(content)

# update_file(files[0]) # test
