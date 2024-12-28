#include "ImguiManager.h"
ImguiManager::ImguiManager(HWND ghwnd)
{
	// Code
	// Initialize Imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();
    imguiCustomColorTheme();
    io.Fonts->AddFontFromFileTTF((std::string(RESOURCE_DIR) + "/font/Roboto/Roboto-Black.ttf").c_str(), 18.0f); // Adjust size
    io.FontGlobalScale = 1.2f; // Optional global scale multiplier
	ImGui_ImplWin32_Init(ghwnd);
	ImGui_ImplOpenGL3_Init("#version 130");

}

ImguiManager::~ImguiManager()
{
	// Code

    destroy();
}


void ImguiManager::beginFrame()
{
	// Code
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::DockSpaceOverViewport();

}

// void ImguiManager::renderGUI(Mesh& mesh)
// {
//     // Code
//     mesh.renderGUI();
// }

void ImguiManager::endFrame()
{
	// Code
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void ImguiManager::destroy()
{
	// Code
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
void ImguiManager::imguiCustomColorTheme()
{
    auto& style = ImGui::GetStyle();
    auto& colors = style.Colors;
    
    style.WindowPadding = ImVec2(0.0f, 0.0f);

    // Window Background
    colors[ImGuiCol_WindowBg] = ImVec4{ 0.35f, 0.35f, 0.35f,0.35f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.25f, 0.25f, 0.25f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };

    // Frame Background
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.25f, 0.25f, 0.25f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.05f, 0.05f, 0.05f, 1.0f };

    // Scrollbar
    colors[ImGuiCol_ScrollbarBg] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };
    colors[ImGuiCol_ScrollbarGrab] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{ 0.4f, 0.4f, 0.4f, 1.0f };

    // Resize Grip
    colors[ImGuiCol_ResizeGrip] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
    colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
    colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0.4f, 0.4f, 0.4f, 1.0f };

    // Separator
    colors[ImGuiCol_Separator] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
    colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
    colors[ImGuiCol_SeparatorActive] = ImVec4{ 0.4f, 0.4f, 0.4f, 1.0f };

    // Adjust style
    style.FrameRounding = 4.0f;
    style.WindowRounding = 4.0f;
}
