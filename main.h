#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <functional>
#include <locale.h>
#include <DirectXMath.h>
#include <WICTextureLoader.h>
#include <iostream>

#pragma warning(push)
#pragma warning(disable:4005)

#include <d3d11.h>

#pragma warning(pop)


// 外部ライブラリ
#pragma comment(lib,"directxtk.lib")
#pragma comment(lib,"directxtex.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment (lib,"winmm.lib")
#pragma comment (lib, "xinput.lib")
#pragma comment(lib, "dxgi.lib")			// リンク対象ライブラリにdxgxi.libを追加
#pragma comment(lib, "dxguid.lib")

constexpr uint32_t SCREEN_WIDTH = 1500;
constexpr uint32_t SCREEN_HEIGHT = (SCREEN_WIDTH / 16) * 9;

constexpr uint32_t MAX_SPRITE_WIDTH = 980;
constexpr uint32_t MAX_SPRITE_HEIGHT = (MAX_SPRITE_WIDTH / 16) * 9;

constexpr uint32_t ENVIRONMENT_RENDERMAP_WIDTH = 1600 * 0.5;
constexpr uint32_t ENVIRONMENT_RENDERMAP_HEIGHT = 900 * 0.5;
constexpr uint32_t ENVIRONMENT_SPRITE_WIDTH = 1600;
constexpr uint32_t ENVIRONMENT_SPRITE_HEIGHT = 900;
constexpr uint32_t ENVIRONMENT_CUBEMAP_WIDTH = 1200;
constexpr uint32_t ENVIRONMENT_CUBEMAP_HEIGHT = ENVIRONMENT_CUBEMAP_WIDTH/8 * 6;

#define SpriteScale (0.5)

#define SpriteWidth (MAX_SPRITE_WIDTH * SpriteScale)
#define SpriteHeight (MAX_SPRITE_HEIGHT * SpriteScale)