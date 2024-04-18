#include "TextureCreate.h"
#include <WICTextureLoader.h>
#include "../DirectX/renderer.h"
#include "utftosjisconv.h"
#include "../Utility.h"
#include <iostream>
#include "../Debug.h"
using namespace std;

TextureCreate* TextureCreate::p_mInstance = nullptr;
CRITICAL_SECTION TextureCreate::myCS{};

void TextureCreate::CreateTexture(const char* _FileName, ID3D11ShaderResourceView** _pTex)
{
    EnterCriticalSection(&myCS);
    if (_pTex)
    {
        ID3D11ShaderResourceView* te = *_pTex;
        SAFE_RELEASE(te);
    }

    std::wstring ws = sjis_to_wide_winapi(_FileName);

    ID3D11Device* device = Renderer::GetDevice();
    //CoInitialize(nullptr);
    HRESULT hr = DirectX::CreateWICTextureFromFile(
        Renderer::GetDevice(),
        ws.c_str(),
        nullptr,
        _pTex);

    if (FAILED(hr))
    {
        NEWDEBUGLOG("ÉGÉâÅ[");
    }

    assert(_pTex);
    LeaveCriticalSection(&myCS);
}

void TextureCreate::CreateTexture(const wchar_t* _FileName, ID3D11ShaderResourceView** _pTex)
{
    EnterCriticalSection(&myCS);
    if (_pTex)
    {
        ID3D11ShaderResourceView* te = *_pTex;
        SAFE_RELEASE(te);
    }

    DirectX::CreateWICTextureFromFile(
        Renderer::GetDevice(),
        _FileName,
        nullptr,
        _pTex);

    assert(_pTex);
    LeaveCriticalSection(&myCS);
}
