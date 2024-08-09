#include "file_explorer.h"
#include "imgui.h"
#include <filesystem>
#include <string>

void RenderFileExplorer() {
    if (ImGui::BeginPopupModal("File Browser", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("File Browser");
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}