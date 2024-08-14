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

//goofy bug since ig the line number currently is part of column so its offset by a bit. 
CodeArea::CodeArea() : CodeArea("Untitled", "", 1, 0) {}

CodeArea::CodeArea(std::string name, std::string path, int cursor_line, int cursor_column)
    : file_name(std::move(name)),
      file_path(std::move(path)),
      cursor_line(cursor_line),
      cursor_column(cursor_column) {
    code_lines[1] = "";
}

bool window_focused = false;

void CodeArea::render() {
    ImGui::Begin(file_name.c_str());
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    //ImVec2 window_pos = ImGui::GetWindowPos();
    //ImVec2 window_padding = ImGui::GetStyle().WindowPadding;

    if (ImGui::IsWindowFocused()) {
        window_focused = true;
    } else {
        window_focused = false;
    }

    if (window_focused) {
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow)) && cursor_line > 1) {
            cursor_line--;
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow)) && cursor_line < static_cast<int>(code_lines.size())) {
            cursor_line++;
            if (cursor_column > static_cast<int>(code_lines[cursor_line].length())) {
                cursor_column = static_cast<int>(code_lines[cursor_line].length());
            }
            // std::cout << cursor_line << std::endl;
            // std::cout << code_lines.size() << std::endl;
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_LeftArrow)) && cursor_column > 0) {
            cursor_column--;
            //std::cout << cursor_column << std::endl;
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_RightArrow)) && cursor_column < static_cast<int>(code_lines[cursor_line].length())) {
            cursor_column++;
            //std::cout << cursor_column << std::endl;
            //std::cout << code_lines[cursor_line].length() << std::endl;
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
            if (cursor_line == static_cast<int>(code_lines.size())) {
                code_lines.insert({cursor_line + 1, ""});
                cursor_line++;
            } else {
                if (cursor_column == 0) {
                    std::string next = "";
                    for (int i = cursor_line + 1; i <= static_cast<int>(code_lines.size()) - 1; i++) {
                        std::string temp = code_lines[i];
                        code_lines[i] = next;
                        next = temp;
                    }
                    code_lines.insert(std::make_pair(cursor_line + 1, next));
                } else {
                    std::string previous = code_lines[cursor_line].substr(0, cursor_column);
                    std::string next = code_lines[cursor_line].substr(cursor_column);
                    code_lines[cursor_line] = previous;
                    for (int i = cursor_line + 1; i <= static_cast<int>(code_lines.size()) - 1; i++) {
                        std::string temp = code_lines[i];
                        code_lines[i] = next;
                        next = temp;
                    }
                    code_lines.insert(std::make_pair(cursor_line + 1, next));
                    cursor_line++;
                }
                
            }
            // code_lines.insert({cursor_line + 1, code_lines[cursor_line].substr(cursor_column)});
            // code_lines[cursor_line] = code_lines[cursor_line].substr(0, cursor_column);
            // cursor_line++;
            // cursor_column = 0;
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Tab))) {
            // put \t ig
            code_lines[cursor_line].insert(cursor_column, "    ");
            cursor_column += 4;
        }
    }


    ImGui::Columns(2, "CodeColumns");
    ImGui::SetColumnWidth(0, 50.0f); 

    for (const auto& [line_number, line] : code_lines) {

        ImGui::Text("%d", line_number);
        ImGui::NextColumn();

        ImVec2 text_pos = ImGui::GetCursorScreenPos();
        if (line_number == cursor_line) {
            float caret_width = 1.0f; 
            float char_width = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, " ").x;

            ImVec2 rect_min = ImVec2(text_pos.x + ((cursor_column - 1) * char_width), text_pos.y);
            ImVec2 rect_max = ImVec2(text_pos.x + caret_width + ((cursor_column - 1) * char_width), text_pos.y + ImGui::GetTextLineHeight());
            draw_list->AddRectFilled(rect_min, rect_max, IM_COL32(0, 111, 255, 255)); 

            ImVec2 line_rect_min2 = ImVec2(ImGui::GetColumnOffset(1), text_pos.y);
            ImVec2 line_rect_max2 = ImVec2(ImGui::GetWindowWidth(), text_pos.y + ImGui::GetTextLineHeight());
            draw_list->AddRectFilled(line_rect_min2, line_rect_max2, IM_COL32(128, 128, 128, 100));
        }
        ImGui::Text("%s", line.c_str());
        ImGui::NextColumn();
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