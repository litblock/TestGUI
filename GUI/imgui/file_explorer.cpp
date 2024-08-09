#include "file_explorer.h"
#include "imgui.h"
#include <filesystem>
#include <string>
#include <iostream>
#include <vector>

namespace FileExplorer {
    std::filesystem::path current_path = std::filesystem::current_path();
    std::string selected_file;
    std::vector<std::filesystem::directory_entry> entries;

    
    void update_entries() {
        entries.clear();
        try {
            for (const auto& entry : std::filesystem::directory_iterator(current_path)) {
                entries.push_back(entry);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error reading directory: " << e.what() << std::endl;
        }
    }

    void RenderFileExplorer() {
        //std::cout << "Rendering file explorer" << std::endl;
        if (ImGui::BeginPopupModal("File Explorer", nullptr, ImGuiWindowFlags_MenuBar)) {
            //std::cout << "popup" << std::endl;

            if (ImGui::BeginMenuBar()) {
                if (ImGui::ArrowButton("##left", ImGuiDir_Left)) {
                    
                }
                ImGui::SameLine();
                if (ImGui::ArrowButton("##right", ImGuiDir_Right)) {
                    
                }
                if (ImGui::ArrowButton("##up", ImGuiDir_Up)) {
                    current_path = current_path.parent_path();
                    update_entries();
                }
                if (ImGui::Button("Refresh emoji")) {
                    update_entries();
                }
                ImGui::SameLine(ImGui::GetWindowWidth() - 30); 
                if (ImGui::Button("X")) {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndMenuBar();
            }

            ImGui::Text("Current Path: %s", current_path.string().c_str());

            ImGui::BeginChild("Scrolling");
            for (const auto& entry : entries) {
                if (entry.is_directory()) {
                    if (ImGui::Selectable(entry.path().filename().string().c_str(), false, ImGuiSelectableFlags_DontClosePopups)) {
                        current_path = entry.path();
                        update_entries();
                    }
                } else {
                    if (ImGui::Selectable(entry.path().filename().string().c_str())) {
                        selected_file = entry.path().string();
                        std::cout << "Selected file: " << selected_file << std::endl;
                        ImGui::CloseCurrentPopup();
                    }
                }
            }
            ImGui::EndChild();

            ImGui::EndPopup();
        }
    }

    void initialize() {
        update_entries();
    }
}