#include <chrono>
#include <thread>
#include "Application.h"
#include "fpscontrol.h"
#include "../DirectX/renderer.h"
#include "manager.h"
#include "Input.h"
#include "Time.h"
#include "TextureCreate.h"

#include "imguiManager.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


const auto ClassName = TEXT("就職作品");     //!< ウィンドウクラス名.
const auto WindowName = TEXT("Cave Simulator");    //!< ウィンドウ名.

Application::Application(uint32_t width, uint32_t height)
: mhInst   (nullptr)
, mhWnd    (nullptr)
, mWidth   (width)
, mHeight  (height)
{

}


Application::~Application()
{

}


void Application::Run()
{
    if (InitApp())
    { 
        MainLoop(); 
    }

    TermApp();
}

bool Application::InitApp()
{
    // ウィンドウの初期化.
    if (!InitWnd())
    { 
        return false; 
    }

    // 正常終了.
    return true;
}

void Application::TermApp()
{
    // ウィンドウの終了処理.
    TermWnd();
}

bool Application::InitWnd()
{
    // インスタンスハンドルを取得.
    auto hInst = GetModuleHandle(nullptr);
    if (hInst == nullptr)
    { 
        return false; 
    }
#ifdef DEBUG || _DEBUG
    // デバッグモード時の処理を書く
#endif // DEBUG || _DEBUG

    // ウィンドウの設定.
    WNDCLASSEX wc = {};
    wc.cbSize           = sizeof(WNDCLASSEX);
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = WndProc;
    wc.hIcon            = LoadIcon(hInst, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(hInst, IDC_ARROW);
    wc.hbrBackground    = GetSysColorBrush(COLOR_BACKGROUND);
    wc.lpszMenuName     = nullptr;
    wc.lpszClassName    = ClassName;
    wc.hIconSm          = LoadIcon(hInst, IDI_APPLICATION);

    // ウィンドウの登録.
    if (!RegisterClassEx(&wc))
    { return false; }

    // インスタンスハンドル設定.
    mhInst = hInst;

    // ウィンドウのサイズを設定.
    RECT rc = {};
    rc.right  = static_cast<LONG>(mWidth);
    rc.bottom = static_cast<LONG>(mHeight);

    // ウィンドウサイズを調整.
    auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
    AdjustWindowRect(&rc, style, FALSE);

    // ウィンドウを生成.
    mhWnd = CreateWindowEx(
        0,
        ClassName,
        WindowName,
        style,
        0,
        0,
        rc.right  - rc.left,
        rc.bottom - rc.top,
        nullptr,
        nullptr,
        mhInst,
        nullptr);

    if (mhWnd == nullptr)
    { return false; }
 
    // ウィンドウを表示.
    ShowWindow(mhWnd, SW_SHOWNORMAL);

    // ウィンドウを更新.
    UpdateWindow(mhWnd);

    // ウィンドウにフォーカスを設定.
    SetFocus(mhWnd);

    // 正常終了.
    return true;
}

void Application::TermWnd()
{
    // ウィンドウの登録を解除.
    if (mhInst != nullptr)
    { UnregisterClass(ClassName, mhInst); }

    mhInst = nullptr;
    mhWnd  = nullptr;
}

void Application::MainLoop()
{
    MSG msg = {};

    // FPS調整クラス
    FPS fpsrate(BaseFrameRate);

    // Inputにウィンドウハンドルをセット
    Input::SetHWND(mhWnd);

    // ゲームの初期処理
    Manager::Init(this);

    while(WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            uint64_t delta_time = 0;
            // ゲーム内時間の更新処理
            Time->Update();

            // デルタタイムを計算
            delta_time = fpsrate.CalcDelta();

            Manager::Update(delta_time);        // ゲーム更新
            Manager::Draw(delta_time);		    // ゲーム描画

            // 規定時間まで時間調整
            fpsrate.Wait();
        }
    }
    // ゲームの終了処理
    Manager::Uninit();

}

LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wp, lp))
    {
        return DefWindowProc(hWnd, msg, wp, lp);
    }
    static bool MouseOutOfWindow = false;
    switch(msg)
    {
        case WM_DESTROY:
            { PostQuitMessage(0); }
            break;

        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
        case WM_KEYDOWN:

            // Inputの設定
            Input::SetKeyDownState(LOWORD(wp));

            if (LOWORD(wp) == VK_ESCAPE)
            {
                ShowCursor(true);
                int result;
                result = MessageBox(hWnd, "ゲームを終了しますか？", "終了確認", MB_YESNO | MB_ICONQUESTION);
                if (result == IDYES)
                {
                    PostMessage(hWnd, WM_CLOSE, wp, lp);
                }
                else if (result == IDNO)
                {
                    ShowCursor(false);
                }
            }
            break;

        case WM_KEYUP:
            // Inputの設定
            Input::SetKeyUpState(LOWORD(wp));
            break;

        case WM_MOUSEMOVE:
            Input::SetMousePoint(static_cast<unsigned int>(lp));
            break;

        case WM_MOUSELEAVE:
            Input::SetCursorPointCenter();
            MouseOutOfWindow = true;
            break;
        case WM_MOUSEWHEEL:
        {
            // マウスホイールの設定
            float wheelData = GET_WHEEL_DELTA_WPARAM(wp);
            Input::SetMouseWheel(wheelData);
        }
            break;

        case WM_LBUTTONDOWN:
            Input::SetKeyDownState(KEYCODE_MOUSE_LEFT);
            break;

        case WM_LBUTTONUP:
            Input::SetKeyUpState(KEYCODE_MOUSE_LEFT);
            break;

        case WM_RBUTTONDOWN:
            Input::SetKeyDownState(KEYCODE_MOUSE_RIGHT);
            break;

        case WM_RBUTTONUP:
            Input::SetKeyUpState(KEYCODE_MOUSE_RIGHT);
            break;

        

        default:
            break;
    }

    return DefWindowProc(hWnd, msg, wp, lp);
}
