#ifndef CODE_AREA_H
#define CODE_AREA_H

#include <string>

class CodeArea {
    public:
        std::string file_name;
        std::string file_path;

        CodeArea();
        void render();
}
#endif 