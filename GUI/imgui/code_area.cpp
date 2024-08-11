#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <GLES2/gl2.h>
#include <stdio.h>
#include <iostream>
#include <filesystem>
#include "code_area.h"


CodeArea::CodeArea() {
    file_name = "Untitled";
    file_path = "";
}

void CodeArea::render() {
    ImGui::Begin(file_name.c_str());
    ImGui::Text("This is a test window for CodeArea.");
    ImGui::End();
}

void CodeArea::load_file(const std::string& file_path) {
    this->file_path = file_path;
    this->file_name = std::filesystem::path(file_path).filename().string() + get_file_extension();
    
}

std::string CodeArea::get_file_extension() const {
    return std::filesystem::path(file_path).extension().string();
}
