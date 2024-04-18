#pragma once

#include "../main.h"
#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_dx11.h"
#include "../imgui/backends/imgui_impl_win32.h"
#include <SimpleMath.h>

#include <string>
#include <list>

constexpr float FontSize = 12.0f;
constexpr DirectX::SimpleMath::Vector2 FPSWindowSize = { 90.0f, 60.0f };

class GUI
{
private:
	static ImGuiIO* io;
	static bool show_another_window;
	static ImVec4 clear_color;

	static std::list<std::string> LogList;

public:
	// ImGui������
	static void InitImGui(HWND hWnd, ID3D11Device* _device, ID3D11DeviceContext* _dcon);

	// ImGui�X�V
	static void UpdateImGui();

	// �T���v���E�B���h�E�̕\��
	static void UpdateSampleWindow();

	// ImGui�̕\��
	static void RenderImGui();

	static void ShowFrameRate(float _Posx, float _Posy);

	static void PushLog(std::string _Log);
	static void PushLog(const char* _Log);
	static void ShowLog();

	// ImGui�̔j��
	static void UninitImGui();
};