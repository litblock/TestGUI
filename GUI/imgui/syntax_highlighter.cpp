#include "syntax_highlighter.h"
#include "imgui.h"

SyntaxHighlighter::SyntaxHighlighter() {
    syntax_rules = {
        {std::regex(R"(\b(if|else|for|while|return|int|float|bool|char|void)\b)"), ImVec4(0.8f, 0.2f, 0.2f, 1.0f)}, 
        {std::regex(R"(\/\/.*$)"), ImVec4(0.2f, 0.6f, 0.2f, 1.0f)}, 
        {std::regex(R"("([^"\\]|\\.)*")"), ImVec4(0.6f, 0.6f, 0.2f, 1.0f)}, 
        {std::regex(R"(\b\d+\b)"), ImVec4(0.2f, 0.2f, 0.8f, 1.0f)} 
    };
}

void SyntaxHighlighter::render(const std::string& line_text, ImDrawList* draw_list, ImVec2 text_pos) {
    size_t start = 0;
    while (start < line_text.size()) {
        bool matched = false;
        for (const auto& rule : syntax_rules) {
            std::smatch match;
            std::string remaining_text = line_text.substr(start);
            if (std::regex_search(remaining_text, match, rule.pattern) && match.position() == 0) {
                std::string token = match.str();
                draw_list->AddText(text_pos, ImGui::ColorConvertFloat4ToU32(rule.color), token.c_str());
                text_pos.x += ImGui::CalcTextSize(token.c_str()).x;
                start += token.size();
                matched = true;
                break;
            }
        }
        if (!matched) {
            std::string token(1, line_text[start]);
            draw_list->AddText(text_pos, ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), token.c_str());
            text_pos.x += ImGui::CalcTextSize(token.c_str()).x;
            start++;
        }
    }
}