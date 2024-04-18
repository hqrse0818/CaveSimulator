#pragma once
#include "../Utility.h"

// キーの総数
#define KEY_TYPENUM     270
//=============================//
//		キーマクロ定義一覧
//=============================//

#define KEYCODE_MOUSE_LEFT        0x01
#define KEYCODE_MOUSE_RIGHT        0x02
#define KEYCODE_CANCEL         0x03
#define KEYCODE_MBUTTON        0x04
#define KEYCODE_XBUTTON1       0x05
#define KEYCODE_XBUTTON2       0x06

#define KEYCODE_BACK           0x08
#define KEYCODE_TAB            0x09

#define KEYCODE_CLEAR          0x0C
#define KEYCODE_RETURN         0x0D

#define KEYCODE_SHIFT          0x10
#define KEYCODE_CONTROL        0x11
#define KEYCODE_MENU           0x12
#define KEYCODE_PAUSE          0x13
#define KEYCODE_CAPITAL        0x14

#define KEYCODE_KANA           0x15
#define KEYCODE_HANGEUL        0x15
#define KEYCODE_HANGUL         0x15
#define KEYCODE_IME_ON         0x16
#define KEYCODE_JUNJA          0x17
#define KEYCODE_FINAL          0x18
#define KEYCODE_HANJA          0x19
#define KEYCODE_KANJI          0x19
#define KEYCODE_IME_OFF        0x1A

#define KEYCODE_ESCAPE         0x1B

#define KEYCODE_CONVERT        0x1C
#define KEYCODE_NONCONVERT     0x1D
#define KEYCODE_ACCEPT         0x1E
#define KEYCODE_MODECHANGE     0x1F

#define KEYCODE_SPACE          0x20
#define KEYCODE_PRIOR          0x21
#define KEYCODE_NEXT           0x22
#define KEYCODE_END            0x23
#define KEYCODE_HOME           0x24
#define KEYCODE_LEFT           0x25
#define KEYCODE_UP             0x26
#define KEYCODE_RIGHT          0x27
#define KEYCODE_DOWN           0x28
#define KEYCODE_SELECT         0x29
#define KEYCODE_PRINT          0x2A
#define KEYCODE_EXECUTE        0x2B
#define KEYCODE_SNAPSHOT       0x2C
#define KEYCODE_INSERT         0x2D
#define KEYCODE_DELETE         0x2E
#define KEYCODE_HELP           0x2F

#define KEYCODE_0		0x30
#define KEYCODE_1		0x31
#define KEYCODE_2		0x32
#define KEYCODE_3		0x33
#define KEYCODE_4		0x34
#define KEYCODE_5		0x35
#define KEYCODE_6		0x36
#define KEYCODE_7		0x37
#define KEYCODE_8		0x38
#define KEYCODE_9		0x39

#define KEYCODE_A		  0x41 //'A'
#define KEYCODE_B		  0x42
#define KEYCODE_C		  0x43
#define KEYCODE_D		  0x44
#define KEYCODE_E		  0x45
#define KEYCODE_F		  0x46
#define KEYCODE_G		  0x47
#define KEYCODE_H		  0x48
#define KEYCODE_I		  0x49
#define KEYCODE_J		  0x4A
#define KEYCODE_K		  0x4B
#define KEYCODE_L		  0x4C
#define KEYCODE_M		  0x4D
#define KEYCODE_N		  0x4E
#define KEYCODE_O		  0x4F
#define KEYCODE_P		  0x50
#define KEYCODE_Q		  0x51
#define KEYCODE_R		  0x52
#define KEYCODE_S		  0x53
#define KEYCODE_T		  0x54
#define KEYCODE_U		  0x55
#define KEYCODE_V		  0x56
#define KEYCODE_W		  0x57
#define KEYCODE_X		  0x58
#define KEYCODE_Y		  0x59
#define KEYCODE_Z		  0x5A

#define KEYCODE_LWIN           0x5B
#define KEYCODE_RWIN           0x5C
#define KEYCODE_APPS           0x5D
#define KEYCODE_SLEEP          0x5F

#define KEYCODE_NUMPAD0        0x60
#define KEYCODE_NUMPAD1        0x61
#define KEYCODE_NUMPAD2        0x62
#define KEYCODE_NUMPAD3        0x63
#define KEYCODE_NUMPAD4        0x64
#define KEYCODE_NUMPAD5        0x65
#define KEYCODE_NUMPAD6        0x66
#define KEYCODE_NUMPAD7        0x67
#define KEYCODE_NUMPAD8        0x68
#define KEYCODE_NUMPAD9        0x69

#define KEYCODE_MULTIPLY       0x6A
#define KEYCODE_ADD            0x6B
#define KEYCODE_SEPARATOR      0x6C
#define KEYCODE_SUBTRACT       0x6D
#define KEYCODE_DECIMAL        0x6E
#define KEYCODE_DIVIDE         0x6F

#define KEYCODE_F1             0x70
#define KEYCODE_F2             0x71
#define KEYCODE_F3             0x72
#define KEYCODE_F4             0x73
#define KEYCODE_F5             0x74
#define KEYCODE_F6             0x75
#define KEYCODE_F7             0x76
#define KEYCODE_F8             0x77
#define KEYCODE_F9             0x78
#define KEYCODE_F10            0x79
#define KEYCODE_F11            0x7A
#define KEYCODE_F12            0x7B
#define KEYCODE_F13            0x7C
#define KEYCODE_F14            0x7D
#define KEYCODE_F15            0x7E
#define KEYCODE_F16            0x7F
#define KEYCODE_F17            0x80
#define KEYCODE_F18            0x81
#define KEYCODE_F19            0x82
#define KEYCODE_F20            0x83
#define KEYCODE_F21            0x84
#define KEYCODE_F22            0x85
#define KEYCODE_F23            0x86
#define KEYCODE_F24            0x87

#define KEYCODE_NAVIGATION_VIEW     0x88
#define KEYCODE_NAVIGATION_MENU     0x89
#define KEYCODE_NAVIGATION_UP       0x8A
#define KEYCODE_NAVIGATION_DOWN     0x8B
#define KEYCODE_NAVIGATION_LEFT     0x8C
#define KEYCODE_NAVIGATION_RIGHT    0x8D
#define KEYCODE_NAVIGATION_ACCEPT   0x8E
#define KEYCODE_NAVIGATION_CANCEL   0x8F

#define KEYCODE_NUMLOCK        0x90
#define KEYCODE_SCROLL         0x91

#define KEYCODE_OEM_NEC_EQUAL  0x92
#define KEYCODE_OEM_FJ_JISHO   0x92
#define KEYCODE_OEM_FJ_MASSHOU 0x93
#define KEYCODE_OEM_FJ_TOUROKU 0x94
#define KEYCODE_OEM_FJ_LOYA    0x95
#define KEYCODE_OEM_FJ_ROYA    0x96

#define KEYCODE_LSHIFT         0xA0
#define KEYCODE_RSHIFT         0xA1
#define KEYCODE_LCONTROL       0xA2
#define KEYCODE_RCONTROL       0xA3
#define KEYCODE_LMENU          0xA4
#define KEYCODE_RMENU          0xA5

#define KEYCODE_BROWSER_BACK        0xA6
#define KEYCODE_BROWSER_FORWARD     0xA7
#define KEYCODE_BROWSER_REFRESH     0xA8
#define KEYCODE_BROWSER_STOP        0xA9
#define KEYCODE_BROWSER_SEARCH      0xAA
#define KEYCODE_BROWSER_FAVORITES   0xAB
#define KEYCODE_BROWSER_HOME        0xAC

#define KEYCODE_VOLUME_MUTE         0xAD
#define KEYCODE_VOLUME_DOWN         0xAE
#define KEYCODE_VOLUME_UP           0xAF

#define KEYCODE_MEDIA_NEXT_TRACK    0xB0
#define KEYCODE_MEDIA_PREV_TRACK    0xB1
#define KEYCODE_MEDIA_STOP          0xB2
#define KEYCODE_MEDIA_PLAY_PAUSE    0xB3

#define KEYCODE_LAUNCH_MAIL         0xB4
#define KEYCODE_LAUNCH_MEDIA_SELECT 0xB5
#define KEYCODE_LAUNCH_APP1         0xB6
#define KEYCODE_LAUNCH_APP2         0xB7

#define KEYCODE_OEM_1          0xBA
#define KEYCODE_OEM_PLUS       0xBB
#define KEYCODE_OEM_COMMA      0xBC
#define KEYCODE_OEM_MINUS      0xBD
#define KEYCODE_OEM_PERIOD     0xBE
#define KEYCODE_OEM_2          0xBF
#define KEYCODE_OEM_3          0xC0

#define KEYCODE_GAMEPAD_A                         0xC3
#define KEYCODE_GAMEPAD_B                         0xC4
#define KEYCODE_GAMEPAD_X                         0xC5
#define KEYCODE_GAMEPAD_Y                         0xC6
#define KEYCODE_GAMEPAD_RIGHT_SHOULDER            0xC7
#define KEYCODE_GAMEPAD_LEFT_SHOULDER             0xC8
#define KEYCODE_GAMEPAD_LEFT_TRIGGER              0xC9
#define KEYCODE_GAMEPAD_RIGHT_TRIGGER             0xCA
#define KEYCODE_GAMEPAD_DPAD_UP                   0xCB
#define KEYCODE_GAMEPAD_DPAD_DOWN                 0xCC
#define KEYCODE_GAMEPAD_DPAD_LEFT                 0xCD
#define KEYCODE_GAMEPAD_DPAD_RIGHT                0xCE
#define KEYCODE_GAMEPAD_MENU                      0xCF
#define KEYCODE_GAMEPAD_VIEW                      0xD0
#define KEYCODE_GAMEPAD_LEFT_THUMBSTICK_BUTTON    0xD1
#define KEYCODE_GAMEPAD_RIGHT_THUMBSTICK_BUTTON   0xD2
#define KEYCODE_GAMEPAD_LEFT_THUMBSTICK_UP        0xD3
#define KEYCODE_GAMEPAD_LEFT_THUMBSTICK_DOWN      0xD4
#define KEYCODE_GAMEPAD_LEFT_THUMBSTICK_RIGHT     0xD5
#define KEYCODE_GAMEPAD_LEFT_THUMBSTICK_LEFT      0xD6
#define KEYCODE_GAMEPAD_RIGHT_THUMBSTICK_UP       0xD7
#define KEYCODE_GAMEPAD_RIGHT_THUMBSTICK_DOWN     0xD8
#define KEYCODE_GAMEPAD_RIGHT_THUMBSTICK_RIGHT    0xD9
#define KEYCODE_GAMEPAD_RIGHT_THUMBSTICK_LEFT     0xDA

#define KEYCODE_OEM_4          0xDB
#define KEYCODE_OEM_5          0xDC
#define KEYCODE_OEM_6          0xDD
#define KEYCODE_OEM_7          0xDE
#define KEYCODE_OEM_8          0xDF
#define KEYCODE_OEM_AX         0xE1
#define KEYCODE_OEM_102        0xE2

#define KEYCODE_ICO_HELP       0xE3
#define KEYCODE_ICO_00         0xE4
#define KEYCODE_PROCESSKEY     0xE5
#define KEYCODE_ICO_CLEAR      0xE6
#define KEYCODE_PACKET         0xE7

#define KEYCODE_OEM_RESET      0xE9
#define KEYCODE_OEM_JUMP       0xEA
#define KEYCODE_OEM_PA1        0xEB
#define KEYCODE_OEM_PA2        0xEC
#define KEYCODE_OEM_PA3        0xED
#define KEYCODE_OEM_WSCTRL     0xEE
#define KEYCODE_OEM_CUSEL      0xEF
#define KEYCODE_OEM_ATTN       0xF0
#define KEYCODE_OEM_FINISH     0xF1
#define KEYCODE_OEM_COPY       0xF2
#define KEYCODE_OEM_AUTO       0xF3
#define KEYCODE_OEM_ENLW       0xF4
#define KEYCODE_OEM_BACKTAB    0xF5

#define KEYCODE_ATTN           0xF6
#define KEYCODE_CRSEL          0xF7
#define KEYCODE_EXSEL          0xF8
#define KEYCODE_EREOF          0xF9
#define KEYCODE_PLAY           0xFA
#define KEYCODE_ZOOM           0xFB
#define KEYCODE_NONAME         0xFC
#define KEYCODE_PA1            0xFD
#define KEYCODE_OEM_CLEAR      0xFE

enum class KEYSTATE
{
    KEY_NONE,
    KEY_DOWN,       // キーを押した瞬間
    KEY_WHILE_DOWN, // キーを押している間
    KEY_UP,         // キーを上げた瞬間
    KEY_WHILE_UP    // キーを上げている間
};

// キーの押下状態
class Input
{
protected:
    // フレームの押下状態(取得)
    static KEYSTATE mKeyStateList[KEY_TYPENUM];

    // 現在のフレームの押下状態(判断)
    static KEYSTATE mKeyStateBufList[KEY_TYPENUM];

    // ウィンドウ上のカーソルの位置
    static DirectX::SimpleMath::Vector2 MousePoint;
    static DirectX::SimpleMath::Vector2 LastMousePoint;
    // マウスの動きを1から-1で表現する
    static DirectX::SimpleMath::Vector2 NormalizedMouseMove;

    // マウスのホイール値
    static float MouseWheel;

    // 入力の有効
    static bool InputActive;

    // スクリーンの中心からどれだけ動いたか
    static DirectX::SimpleMath::Vector2 MouseSub;

    // スクリーンの中心位置
    static DirectX::SimpleMath::Vector2 ScreenCenter;

    // スクリーン上のカーソル位置
    static POINT Cursor;

    // 毎ループカーソルを中心にするか
    static bool IsSetCursorCenter;

    static HWND mhWnd;
public:
    // 初期化
    static void Init();
    // 更新
    static void Update();
    // リフレッシュ処理
    static void RefreshMouse();

    // キーの押下状態を取得する関数
    static KEYSTATE GetKeyState(int _Key);

    // マウスの現在値を取得
    static DirectX::SimpleMath::Vector2 GetMousePos()
    {
        return MousePoint;
    }

    // マウスが前回の位置からどれだけ動いたか
    // 毎ループセンターにセットしていない時に使う
    static DirectX::SimpleMath::Vector2 GetMouseMove();

    // 中心位置からどれだけ動いたかだけを取得
    static DirectX::SimpleMath::Vector2 GetCursorMove()
    {
        return MouseSub;
    }
    static DirectX::SimpleMath::Vector2 GetCursorPosition()
    {
        DirectX::SimpleMath::Vector2 tmp;
        tmp.x = (float)Cursor.x;
        tmp.y = (float)Cursor.y;
        return tmp;
    }

    // マウスホイールの回転の取得
    static float GetMouseWheel()
    {
        return MouseWheel;
    }

    // キーを押した瞬間をOSから取得してキー一時バッファに記録する
    static void SetKeyDownState(unsigned int _KeyCode);

    // キーを離した瞬間をOSから取得してキー一時バッファに記録する
    static void SetKeyUpState(unsigned int _KeyCode);

    // マウスポインターのセット
    static void SetMousePoint(unsigned int Param);
    static void SetMousePoint(float _x, float _y);
    static void SetCursorPosition(DirectX::SimpleMath::Vector2 Point);
    static void SetCursorPointCenter();

    // マウスホイールの値のセット
    static void SetMouseWheel(float _value);

    // 毎ループカーソルを中心にするかセット
    static void SetCursorCenterEnable()
    {
        IsSetCursorCenter = true;
    }
    static void SetCursorCenterDisable()
    {
        IsSetCursorCenter = false;
    }

    // キーの入力可否
    static void SetEnable();
    static void SetDisable();

    // ウィンドウハンドルをセット(Input::Init()よりも前に呼ぶ)
    static void SetHWND(HWND _handle)
    {
        mhWnd = _handle;
    }

    static bool GetIsCenter()
    {
        return IsSetCursorCenter;
    }
protected:

    // キー一時バッファを判断してキー状態を更新する
    static void UpdateKeyState();

};

#include <Xinput.h>

#define GAMEPAD_A XINPUT_GAMEPAD_A
#define GAMEPAD_B XINPUT_GAMEPAD_B
#define GAMEPAD_Y XINPUT_GAMEPAD_Y
#define GAMEPAD_X XINPUT_GAMEPAD_X
#define GAMEPAD_UP XINPUT_GAMEPAD_DPAD_UP
#define GAMEPAD_DOWN XINPUT_GAMEPAD_DPAD_DOWN
#define GAMEPAD_LEFT XINPUT_GAMEPAD_DPAD_LEFT
#define GAMEPAD_RIGHT XINPUT_GAMEPAD_DPAD_RIGHT
#define GAMEPAD_SHOULDER_L XINPUT_GAMEPAD_LEFT_SHOULDER
#define GAMEPAD_SHOULDER_R XINPUT_GAMEPAD_RIGHT_SHOULDER
#define GAMEPAD_STICK_L XINPUT_GAMEPAD_LEFT_THUMB
#define GAMEPAD_STICK_R XINPUT_GAMEPAD_RIGHT_THUMB
#define GAMEPAD_START XINPUT_GAMEPAD_START
#define GAMEPAD_BACK XINPUT_GAMEPAD_BACK

constexpr int StickMaxValue = 32768;

struct GamePadState
{
    KEYSTATE A, B, X, Y;                                   // ボタン
    KEYSTATE UP, DOWN, LEFT, RIGHT;         // 矢印
    KEYSTATE SHOULDER_L, SHOULDER_R;   // ショルダーボタン
    KEYSTATE STICK_L, STICK_R;                    // スティック
    KEYSTATE START;
};

class Controller_Input
{
private:
    static XINPUT_STATE GamePad[4];
    static XINPUT_STATE LastGamePad[4];

    // 入力状態
    static GamePadState myPad[4];

    static UINT TriggerThreshould;
    static SHORT LeftStickDead;
    static SHORT RightStickDead;

public:
    static void UpdateCurrentController();
    static KEYSTATE GetButton(unsigned int index, UINT _State);
    static KEYSTATE GetRightTriggerSimple(unsigned int index);
    static KEYSTATE GetLeftTriggerSimple(unsigned int index);
    static float GetRightTriggerValue(unsigned int index);
    static float GetLeftTriggerValue(unsigned int index);
    static DirectX::SimpleMath::Vector2 GetLeftStick(unsigned int index);
    static DirectX::SimpleMath::Vector2 GetRightStick(unsigned int index);
    static bool GetIsGamePadConnect(unsigned int index);
    static bool GetLeftStickMoveNow(unsigned int index);

    static void ControllerInit();
    static void ControllerUninit();
};