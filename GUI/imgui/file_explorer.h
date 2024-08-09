#ifndef FILE_EXPLORER_H
#define FILE_EXPLORER_H

#include <string>
#include <filesystem>
#include <vector>


namespace FileExplorer {
    extern std::filesystem::path current_path;
    extern std::string selected_file;
    extern std::vector<std::filesystem::directory_entry> entries;

    void update_entries();
    void RenderFileExplorer();  
    void initialize();
}

#endif 