#include "imguiManager.h"
#include "../main.h"
#include "../Utility.h"

ImGuiIO* GUI::io = nullptr;
bool GUI::show_another_window = false;
ImVec4 GUI::clear_color = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
std::list<std::string> GUI::LogList{};

using namespace std;

void GUI::InitImGui(HWND hWnd, ID3D11Device* _device, ID3D11DeviceContext* _dcon)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	io = &ImGui::GetIO(); (void)io;
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	io->Fonts->AddFontFromFileTTF("asset/arial.ttf", FontSize, nullptr, io->Fonts->GetGlyphRangesJapanese());

	// ImGui::StyleColorsDark();
	// ImGui::StyleColorsLight();
	ImGui::StyleColorsClassic();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(_device, _dcon);
}

void GUI::UpdateImGui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void GUI::UpdateSampleWindow()
{
	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Another Window", &show_another_window);

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
	ImGui::End();
}

void GUI::UninitImGui()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void GUI::RenderImGui()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void GUI::ShowFrameRate(float _Posx, float _Posy)
{
	ImGui::SetNextWindowPos(ImVec2(_Posx, _Posy), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(FPSWindowSize.x, FPSWindowSize.y), ImGuiCond_Once);
	string text;
	float fps = io->Framerate;
	text = to_string(fps);

	ImGui::Begin("FPS");
	ImGui::Text(text.c_str());
	ImGui::End();
}

void GUI::PushLog(std::string _Log)
{
	LogList.push_back(_Log);
}

void GUI::PushLog(const char* _Log)
{
	LogList.push_back(_Log);
}

void GUI::ShowLog()
{
	ImGui::SetNextWindowPos(ImVec2(MAX_SPRITE_WIDTH, MAX_SPRITE_HEIGHT), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(SCREEN_WIDTH - MAX_SPRITE_WIDTH), static_cast<float>(SCREEN_HEIGHT - MAX_SPRITE_HEIGHT)), ImGuiCond_Once);
	ImGui::Begin("Log");
	for (auto log : LogList)
	{
		ImGui::Text(log.c_str());
	}
	ImGui::End();
}
