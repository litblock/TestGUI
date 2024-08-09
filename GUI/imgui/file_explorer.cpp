#include "file_explorer.h"
#include "imgui.h"
#include <filesystem>
#include <string>
#include <iostream>

namespace FileExplorer {
    void RenderFileExplorer(std::filesystem::path& current_path, std::string& selected_file) {
        std::cout << "Rendering file explorer" << std::endl;
        
        if (ImGui::BeginPopupModal("File Browser")) {
            if (ImGui::Button("..")) {
                current_path = current_path.parent_path();
            }
            std::cout << "popup" << std::endl;
            for (const auto& entry : std::filesystem::directory_iterator(current_path)) {
                if (entry.is_directory()) {
                    if (ImGui::Selectable(entry.path().filename().string().c_str(), false, ImGuiSelectableFlags_DontClosePopups)) {
                        current_path = entry.path();
                    }
                } else {
                    if (ImGui::Selectable(entry.path().filename().string().c_str())) {
                        selected_file = entry.path().string();
                        ImGui::CloseCurrentPopup();
                    }
                }
            }

            if (ImGui::Button("Cancel")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
}