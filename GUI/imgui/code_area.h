#ifndef CODE_AREA_H
#define CODE_AREA_H

#include <string>
#include <map>


class CodeArea {
    public:
        std::string file_name;
        std::string file_path;
        std::map<int, std::string> code_lines;

        CodeArea();
        void render();
        void load_file(const std::string& file_path);
        void save_file(std::string file_path);
        void add_line(int line_number, const std::string& code);
        void remove_line(int line_number);
        std::string get_file_extension() const;
        std::string get_line(int line_number) const;
        void refresh();

};


#endif 