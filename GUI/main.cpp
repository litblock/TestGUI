#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <cstdlib> 
#include <GLES2/gl2.h>
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include "file_explorer.h"
#include <filesystem>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    ImGui::GetIO().DisplaySize = ImVec2((float)width, (float)height);
}

void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void window_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    ImGui::GetIO().DisplaySize = ImVec2((float)width, (float)height);
}

bool show_window = true;
bool open_popup = false;

void render(GLFWwindow* window) {
    std::filesystem::path current_path = std::filesystem::current_path();
    std::string selected_file;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
            glfwWaitEvents();
            continue;
        } else {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);
            ImGui::GetIO().DisplaySize = ImVec2((float)width, (float)height);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();
        ImGui::ShowDemoWindow();
        
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open", "Ctrl+O")) {
                    open_popup = true;
                    std::cout << "Open clicked" << std::endl;
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {

                }
                if (ImGui::MenuItem("Exit", "Ctrl+Q")) {
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Undo", "Ctrl+Z")) {

                }
                if (ImGui::MenuItem("Redo", "Ctrl+Y")) {

                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }


        if (open_popup) {
            ImGui::OpenPopup("test");
            open_popup = false;
        }

        if (ImGui::BeginPopupModal("test")) {
            ImGui::Text("Hello");
            if (ImGui::Button("Close")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        if (ImGui::BeginPopupModal("File Browser")) {
            std::cout << "Rendering File Browser Popup" << std::endl;
            FileExplorer::RenderFileExplorer(current_path, selected_file);
            if (ImGui::Button("Close")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        if (show_window) {
            ImGui::Begin("Another Window", &show_window);
            ImGui::Text("Hello, world!");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            if (ImGui::Button("Click Me")) {
                std::cout << "Button clicked" << std::endl;
                show_window = false;
            }

            ImGui::End();
        }
        
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        //std::cout << display_w << " " << display_h << std::endl;
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}
int main() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(1280, 800, "TextEditor", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); 
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    float size_pixels = 16.0f;
    io.Fonts->AddFontFromFileTTF("JetBrainsMono-Medium.ttf", size_pixels);

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 120");

    render(window);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}