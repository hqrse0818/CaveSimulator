#pragma once

#include <Windows.h>
#include <cstdint>

constexpr uint64_t BaseFrameRate = 120;


// アプリケーションクラス
// ハードなどに応じて画面サイズや振る舞いを変えられるように
class Application
{
    
public:
    Application(uint32_t width, uint32_t height);
    ~Application();
    // ループ開始
    void Run();

    // 幅を取得
    uint32_t GetWidth() {
        return mWidth;
    }

    // 高さを取得
    uint32_t GetHeight() {
        return mHeight;
    }

    // ウインドウハンドルを返す
    HWND GetWindow() {
        return mhWnd;
    }

private:
   // インスタンスハンドル
    HINSTANCE   mhInst;
    // ウィンドウハンドル
    HWND        mhWnd;
    // ウィンドウ横幅
    uint32_t    mWidth;
    // ウィンドウ縦幅
    uint32_t    mHeight;

private:
    // アプリケーション初期処理
    bool InitApp();
    // アプリケーション終了処理
    void TermApp();
    // ウィンドウ初期化
    bool InitWnd();
    // ウィンドウ削除
    void TermWnd();
    // ループ処理
    void MainLoop();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
};