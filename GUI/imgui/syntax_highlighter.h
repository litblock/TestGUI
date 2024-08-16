#ifndef SYNTAX_HIGHLIGHTER_H
#define SYNTAX_HIGHLIGHTER_H

#include "imgui.h"
#include <vector>
#include <regex>
#include <string>

class SyntaxHighlighter {
public:
    SyntaxHighlighter();
    void render(const std::string& line_text, ImDrawList* draw_list, ImVec2 text_pos);

private:
    struct SyntaxRule {
        std::regex pattern;
        ImVec4 color;
    };
    std::vector<SyntaxRule> syntax_rules;
};

#endif 