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
#include <unordered_map>
#include <cmath>
//#include "syntax_highlighter.h"

//goofy bug since ig the line number currently is part of column so its offset by a bit. 
CodeArea::CodeArea() : CodeArea("Untitled", "", 1, 0, true) {}

CodeArea::CodeArea(std::string name, std::string path, int cursor_line, int cursor_column, bool show)
    : file_name(std::move(name)),
      file_path(std::move(path)),
      cursor_line(cursor_line),
      cursor_column(cursor_column), 
      show(show) {
    code_lines[1] = "";
}

bool window_focused = false;
//bool show = true;

void CodeArea::render() {
    //std::cout << show << std::endl;
    if (!show) {
        //close();
        return;
    }
    ImGui::Begin(file_name.c_str(), &show, ImGuiWindowFlags_HorizontalScrollbar);
    //ImGui::SetScrollX(ImGui::GetScrollX() + 10.0f);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 cursor_screen_pos = ImGui::GetCursorScreenPos();

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
            if (cursor_line == static_cast<int>(code_lines.size()) && cursor_column == static_cast<int>(code_lines[cursor_line].length())) {
                std::cout << code_lines.size() << std::endl;
                std::cout << cursor_line << std::endl;
                std::cout << "end the file" << std::endl;
                code_lines.insert({cursor_line + 1, ""});
                cursor_line++;
                cursor_column = 0;
            } else {
                if (cursor_column == 0 && cursor_line < static_cast<int>(code_lines.size())) {
                    std::cout << "beginning of a full line" << std::endl;
                    std::string next = "";
                    for (int i = cursor_line; i < static_cast<int>(code_lines.size()); i++) {
                        std::cout << next << std::endl;
                        std::string temp = code_lines[i];
                        code_lines[i] = next;
                        next = temp;
                    }
                    code_lines.insert({cursor_line + 1, next});
                    cursor_line++;
                    cursor_column = 0;
                } else {
                    std::cout << "middle of a line" << std::endl;
                    std::cout << cursor_column << std::endl;
                    std::string previous = code_lines[cursor_line].substr(0, cursor_column);
                    std::cout << previous << std::endl;
                    std::string next = code_lines[cursor_line].substr(cursor_column);
                    std::cout << next << std::endl;
                    code_lines[cursor_line] = previous;
                    for (int i = cursor_line + 1; i < static_cast<int>(code_lines.size()); i++) {
                        std::string temp = code_lines[i];
                        code_lines[i] = next;
                        next = temp;
                    }
                    code_lines.insert({code_lines.size() + 1, next});
                    cursor_line++;
                    cursor_column = 0;
                }
                
            }
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Tab))) {
            // i think tab needs to be its own thing to allow for formatting (create a tab line)
            code_lines[cursor_line].insert(cursor_column, "    ");
            cursor_column += 4;
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Space))) {
            code_lines[cursor_line].insert(cursor_column, " ");
            cursor_column++;
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Backspace))) {
            if (cursor_column > 0 && cursor_line < static_cast<int>(code_lines.size())) {
                code_lines[cursor_line].erase(cursor_column - 1, 1);
                cursor_column--;
            }
        }

        bool shift_pressed = ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_LeftShift)) || ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_RightShift));

        if (shift_pressed) {
            for (int key = ImGuiKey_A; key <= ImGuiKey_Z; key++) {
                if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(static_cast<ImGuiKey>(key)))) {
                    char c = 'A' + (key - ImGuiKey_A);
                    if (cursor_line < static_cast<int>(code_lines.size())) {
                        code_lines[cursor_line].insert(cursor_column, 1, c);
                        cursor_column++;
                    } else {
                        code_lines[cursor_line] += c;
                        cursor_column++;
                    }
                }
            }
        } else {
            for (int key = ImGuiKey_A; key <= ImGuiKey_Z; key++) {
                if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(static_cast<ImGuiKey>(key)))) {
                    char c = 'a' + (key - ImGuiKey_A); 
                    if (cursor_line < static_cast<int>(code_lines.size())) {
                        code_lines[cursor_line].insert(cursor_column, 1, c);
                        cursor_column++;
                    } else {
                        code_lines[cursor_line] += c;
                        cursor_column++;
                    }
                }
            }
        }
        
        if (shift_pressed) {
            for (int key = ImGuiKey_0; key <= ImGuiKey_9; key++) {
                if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(static_cast<ImGuiKey>(key)))) {
                    char c = get_shifted_char(static_cast<ImGuiKey>(key)); 
                    if (cursor_line < static_cast<int>(code_lines.size())) {
                        code_lines[cursor_line].insert(cursor_column, 1, c);
                        cursor_column++;
                    } else {
                        code_lines[cursor_line] += c;
                        cursor_column++;
                    }
                }
            }
        } else {
            for (int key = ImGuiKey_0; key <= ImGuiKey_9; key++) {
                if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(static_cast<ImGuiKey>(key)))) {
                    char c = '0' + (key - ImGuiKey_0); 
                    if (cursor_line < static_cast<int>(code_lines.size())) {
                        code_lines[cursor_line].insert(cursor_column, 1, c);
                        cursor_column++;
                    } else {
                        code_lines[cursor_line] += c;
                        cursor_column++;
                    }
                }
            }
        }

        const std::unordered_map<ImGuiKey, char> punctuation_keys = {
            {ImGuiKey_Comma, ','},
            {ImGuiKey_Period, '.'},
            {ImGuiKey_Semicolon, ';'},
            {ImGuiKey_Apostrophe, '\''}, 
            {ImGuiKey_Slash, '/'}, 
            {ImGuiKey_Backslash, '\\'}, 
            {ImGuiKey_Equal, '='},
            {ImGuiKey_Minus, '-'},
            {ImGuiKey_LeftBracket, '['},
            {ImGuiKey_RightBracket, ']'},
            {ImGuiKey_GraveAccent, '`'}
            //add more ltr
        };

        if (shift_pressed) {
            for (const auto& [key, value] : punctuation_keys) {
                if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(key))) {
                    char shift = get_shifted_char(key);
                    if (cursor_line < static_cast<int>(code_lines.size())) {
                        code_lines[cursor_line].insert(cursor_column, 1, shift);
                        cursor_column++;
                    } else {
                        code_lines[cursor_line] += shift;
                        cursor_column++;
                    }
                }
            }
        } else {
            for (const auto& [key, value] : punctuation_keys) {
                if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(key))) {
                    if (cursor_line < static_cast<int>(code_lines.size())) {
                        code_lines[cursor_line].insert(cursor_column, 1, value);
                        cursor_column++;
                    } else {
                        code_lines[cursor_line] += value;
                        cursor_column++;
                    }
                }
            }
        }
    }

    float line_height = ImGui::GetTextLineHeight();
    ImGui::Columns(2, "CodeColumns");
    ImGui::SetColumnWidth(0, 50.0f); 

    //SyntaxHighlighter highlighter;
    
    for (const auto& [line_number, line] : code_lines) {

        if (line_number == cursor_line) {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255)); 
        } else {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(128, 128, 128, 255)); 
        }

        ImGui::Text("%d", line_number);

        ImGui::PopStyleColor();

        ImGui::NextColumn();
        float padding = 7.5f;
        float scroll_x = ImGui::GetScrollX();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - padding - scroll_x);

        ImVec2 text_pos = ImGui::GetCursorScreenPos();
        if (line_number == cursor_line) {
            float caret_width = 1.0f; 
            float char_width = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, " ").x;

            ImVec2 rect_min = ImVec2(text_pos.x + ((cursor_column) * char_width), text_pos.y);
            ImVec2 rect_max = ImVec2(text_pos.x + caret_width + ((cursor_column) * char_width), text_pos.y + ImGui::GetTextLineHeight());
            draw_list->AddRectFilled(rect_min, rect_max, IM_COL32(0, 111, 255, 255)); 

            ImVec2 line_rect_min2 = ImVec2(ImGui::GetColumnOffset(1), text_pos.y);
            ImVec2 line_rect_max2 = ImVec2(ImGui::GetWindowWidth(), text_pos.y + ImGui::GetTextLineHeight());
            draw_list->AddRectFilled(line_rect_min2, line_rect_max2, IM_COL32(128, 128, 128, 100));
        }
        ImGui::SetCursorScreenPos(text_pos);
        ImGui::TextUnformatted(line.c_str());
        ImGui::NextColumn();
    }
    ImGui::ShowMetricsWindow();

    // for (int i = 0; i < static_cast<int>(code_lines.size()); ++i) {
    //     float y = cursor_screen_pos.y + i * ImGui::GetTextLineHeight();
    //     draw_list->AddLine(
    //         ImVec2(window_pos.x, y),
    //         ImVec2(window_pos.x + ImGui::GetWindowWidth(), y),
    //         IM_COL32(0, 255, 0, 128)
    //     );
    // }
    ImVec2 text_start_pos = ImGui::GetCursorScreenPos();

    move_cursor(text_start_pos, line_height);
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

char CodeArea::get_shifted_char(ImGuiKey key) {
    static std::unordered_map<ImGuiKey, char> shift_map = {
        { ImGuiKey_1, '!' }, { ImGuiKey_2, '@' }, { ImGuiKey_3, '#' }, { ImGuiKey_4, '$' }, { ImGuiKey_5, '%' },
        { ImGuiKey_6, '^' }, { ImGuiKey_7, '&' }, { ImGuiKey_8, '*' }, { ImGuiKey_9, '(' }, { ImGuiKey_0, ')' },
        { ImGuiKey_Minus, '_' }, { ImGuiKey_Equal, '+' }, { ImGuiKey_LeftBracket, '{' }, { ImGuiKey_RightBracket, '}' }, { ImGuiKey_Backslash, '|' },
        { ImGuiKey_Semicolon, ':' }, { ImGuiKey_Apostrophe, '"' }, { ImGuiKey_Comma, '<' }, { ImGuiKey_Period, '>' }, { ImGuiKey_Slash, '?' }
    };

    if (shift_map.find(key) != shift_map.end()) {
        return shift_map[key];
    }
    return static_cast<char>(key);
}

void CodeArea::close() {
    delete this;
}

void CodeArea::move_cursor(const ImVec2& content_start_pos, float line_height) {
    if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) {
        ImVec2 mouse_pos = ImGui::GetMousePos();
        ImVec2 window_pos = ImGui::GetWindowPos();
        ImVec2 content_pos = ImGui::GetCursorStartPos();
        
        float scroll_y = ImGui::GetScrollY();
        std::cout << "Scroll y: " << scroll_y << std::endl;
        float scroll_x = ImGui::GetScrollX();
        
        float line_number_width = ImGui::GetColumnOffset(1);
        float char_width = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, " ").x;

        float rel_y = (mouse_pos.y - window_pos.y - content_pos.y + scroll_y - content_start_pos.y);
        float rel_x = (mouse_pos.x - window_pos.x - content_pos.x + scroll_x - content_start_pos.x - line_number_width);

        int new_cursor_line = static_cast<int>(std::floor(rel_y / line_height)) + 1;
        int new_cursor_column = static_cast<int>(std::floor(rel_x / char_width));

        int num_lines = static_cast<int>(code_lines.size());
        new_cursor_line = std::max(1, std::min(new_cursor_line, num_lines));
        
        if (new_cursor_line <= num_lines && !code_lines.empty()) {
            new_cursor_column = std::max(0, std::min(new_cursor_column, static_cast<int>(code_lines[new_cursor_line].length())));
        } else {
            new_cursor_column = 0;
        }

        cursor_line = new_cursor_line;
        cursor_column = new_cursor_column;

        std::cout << "Content start pos: (" << content_start_pos.x << ", " << content_start_pos.y << ")\n";
        std::cout << "Mouse pos: (" << mouse_pos.x << ", " << mouse_pos.y << ")\n";
        std::cout << "Window pos: (" << window_pos.x << ", " << window_pos.y << ")\n";
        std::cout << "Content pos: (" << content_pos.x << ", " << content_pos.y << ")\n";
        std::cout << "Scroll: (" << scroll_x << ", " << scroll_y << ")\n";
        std::cout << "Relative pos: (" << rel_x << ", " << rel_y << ")\n";
        std::cout << "New cursor: line " << cursor_line << ", column " << cursor_column << "\n";
        std::cout << "Number of lines: " << num_lines << "\n";
        std::cout << "Line height: " << line_height << "\n";
    }
}