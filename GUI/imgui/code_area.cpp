#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <GLES2/gl2.h>
#include <stdio.h>
#include <iostream>
#include <filesystem>
#include "code_area.h"
#include <fstream>


CodeArea::CodeArea() {
    file_name = "Untitled";
    file_path = "";
    cursor_line = 1;
    cursor_column = 0;
}

void CodeArea::render() {
    ImGui::Begin(file_name.c_str());
    for (const auto& [line_number, line] : code_lines) {
        //std::cout << "Line number: " << line_number << std::endl;
        std::cout << "Cursor line: " << cursor_line << std::endl;
        if (line_number == cursor_line) {
            //std::cout << "Cursor line: " << cursor_line << std::endl;
            std::string before = line.substr(0, cursor_column - 1);
            std::string after = line.substr(cursor_column);
            ImGui::Text("%d: %s%s%s", line_number, before.c_str(), "|", after.c_str());
        }
        else {
            ImGui::Text("%d: %s", line_number, line.c_str());
        }
        
    }
    ImGui::End();
}

void CodeArea::load_file(const std::string& file_path) {
    this->file_path = file_path;
    this->file_name = std::filesystem::path(file_path).filename().string();
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return;
    }

    code_lines.clear();
    std::string line;
    int line_number = 1;
    while (std::getline(file, line)) {
        code_lines[line_number] = line;
        line_number++;
    }

    file.close();
}

void CodeArea::add_line(int line_number, const std::string& code) {
    code_lines[line_number] = code;
}

void CodeArea::remove_line(int line_number) {
    code_lines.erase(line_number);
}

std::string CodeArea::get_file_extension() const {
    return std::filesystem::path(file_path).extension().string();
}

void CodeArea::refresh() {
    load_file(file_path);
}