import os

files = ['index.html', 'style.css', 'script.js']
header_content = "#pragma once\n\n"

for f in files:
    with open(f"../{f}", "r", encoding="utf-8") as file:
        content = file.read()
    var_name = f.replace('.', '_')
    header_content += f"const char* {var_name} = R\"rawliteral(\n"
    header_content += content
    header_content += "\n)rawliteral\";\n\n"

with open("frontend.h", "w", encoding="utf-8") as file:
    file.write(header_content)
print("frontend.h created successfully.")
