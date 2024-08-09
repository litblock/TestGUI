#ifndef FILE_EXPLORER_H
#define FILE_EXPLORER_H

#include <string>
#include <filesystem>

namespace FileExplorer {
    void RenderFileExplorer(std::filesystem::path& current_path, std::string& selected_file);
}

#endif 