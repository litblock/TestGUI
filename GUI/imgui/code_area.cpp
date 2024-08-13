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
#include <string>


CodeArea::CodeArea() : CodeArea("Untitled", "", 1, 0) {}

CodeArea::CodeArea(std::string name, std::string path, int cursor_line, int cursor_column)
    : file_name(std::move(name)),
      file_path(std::move(path)),
      cursor_line(cursor_line),
      cursor_column(cursor_column) {
    code_lines[1] = "";
}

void CodeArea::render() {
    ImGui::Begin(file_name.c_str());
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    //ImVec2 window_pos = ImGui::GetWindowPos();
    //ImVec2 window_padding = ImGui::GetStyle().WindowPadding;

    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow)) && cursor_line > 1) {
        cursor_line--;
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow)) && cursor_line < static_cast<int>(code_lines.size()) - 1) {
        cursor_line++;
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_LeftArrow)) && cursor_column > 0) {
        cursor_column--;
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_RightArrow)) && cursor_column < static_cast<int>(code_lines[cursor_line].length())) {
        cursor_column++;
    }

    for (const auto& [line_number, line] : code_lines) {
        ImVec2 text_pos = ImGui::GetCursorScreenPos();
        if (line_number == cursor_line) {

            float caret_width = 1.0f; 

            ImVec2 rect_min = ImVec2(text_pos.x, text_pos.y + cursor_column * ImGui::GetTextLineHeight());
            ImVec2 rect_max = ImVec2(text_pos.x + caret_width, text_pos.y + ImGui::GetTextLineHeight());
            draw_list->AddRectFilled(rect_min, rect_max, IM_COL32(255, 255, 0, 255)); 
        }
        ImGui::Text("%d: %s", line_number, line.c_str());
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

void CodeArea::set_cursor_line(int line) {
    cursor_line = line;
}