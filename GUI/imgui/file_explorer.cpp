#include "file_explorer.h"
#include "imgui.h"
#include <filesystem>
#include <string>
#include <iostream>
#include <deque>

namespace FileExplorer {
    std::filesystem::path current_path = std::filesystem::current_path();
    std::string selected_file;
    std::deque<std::filesystem::directory_entry> entries;
    const size_t MAX_ENTRIES = 1000;
    
    bool update_entries() {
        entries.clear();
        try {
            if (!std::filesystem::exists(current_path) || !std::filesystem::is_directory(current_path)) {
                std::cout << ("Current path is not a valid directory: " + current_path.string()) << std::endl;
                return false;
            }

            for (const auto& entry : std::filesystem::directory_iterator(current_path)) {
                entries.push_back(entry);
                if (entries.size() >= MAX_ENTRIES) {
                    std::cout << "Warning: Directory has more than " << MAX_ENTRIES << " entries. Only the first " << MAX_ENTRIES << " will be shown." << std::endl;
                    break;
                }
            }
            return true;
        } catch (const std::exception& e) {
            std::cout << ("Error reading directory: " + std::string(e.what())) << std::endl;
            return false;
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
                try {
                    std::string name = entry.path().filename().string();
                    if (entry.is_directory()) {
                        if (ImGui::Selectable(name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
                            if (ImGui::IsMouseDoubleClicked(0)) {
                                current_path /= name;
                                if (!update_entries()) {
                                    std::cout << ("Failed to updantries after selecting directory: " + name) << std::endl;
                                }
                            }
                        }
                    } else {
                        if (ImGui::Selectable(name.c_str())) {
                            selected_file = entry.path().string();
                        }
                    }
                } catch (const std::exception& e) {
                    std::cout << ("Error processing entry: " + std::string(e.what())) << std::endl;
                }
            }
            ImGui::EndChild();

            ImGui::EndPopup();
        }
        
    }

    void initialize() {
        try {
            current_path = std::filesystem::current_path();
            if (!update_entries()) {
                std::cout << ("Failed to initialize entries") << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << ("Error initializing FileExplorer: " + std::string(e.what())) << std::endl;
        }
    }
}