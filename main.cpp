
#include    "main.h"
#include    "System/Application.h"

#if defined(DEBUG) || defined(_DEBUG)
int wmain(int argc, wchar_t** argv, wchar_t** evnp) {


    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


    // アプリケーション実行
    Application app(SCREEN_WIDTH, SCREEN_HEIGHT);
    app.Run();

    return 0;
}

#else

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow) {


    // 実行するコードを記述

     // アプリケーション実行
    Application app(SCREEN_WIDTH, SCREEN_HEIGHT);
    app.Run();


    return 0;

}

#endif//defined(DEBUG) || defined(_DEBUG)