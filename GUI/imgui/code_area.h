#ifndef CODE_AREA_H
#define CODE_AREA_H

#include <string>
#include <map>


class CodeArea {
    public:
        CodeArea();
        CodeArea(std::string name, std::string path, int cursor_line, int cursor_column, bool show);
        void render();
        void load_file(const std::string& file_path);
        void save_file(std::string file_path);
        void add_line(int line_number, const std::string& code);
        void remove_line(int line_number);
        std::string get_file_extension() const;
        std::string get_line(int line_number) const;
        void refresh();
        void set_cursor_line(int line);
        void move_cursor(const ImVec2& text_start_pos, float line_height);
        void close();

        char get_shifted_char(ImGuiKey key);
    private:
        std::string file_name;
        std::string file_path;
        std::map<int, std::string> code_lines;
        int cursor_line = 1;
        int cursor_column = 0;
        bool show;
        ImVec2 text_pos;
};

#endif 