#include "Utility.h"
#include	<filesystem>
#include "Debug.h"

namespace fs = std::filesystem;

std::string GetFileName(std::string FilePath)
{
    // u8エンコードでｐａｔｈをコンストラクト
    fs::path p = fs::u8path(FilePath.c_str());
    // パスから、ファイル名を除いたディレクトリ、ファイル名、
    // 拡張子を除いたファイル名、拡張子を抽出
//    std::cout << "ディレクトリ名 : " << p.parent_path() << std::endl;
//    std::cout << "ファイル名  : " << p.filename() << std::endl;
//    std::cout << "ファイル名（拡張子なし） : " << p.stem() << std::endl;
//    std::cout << "拡張子 : " << p.extension() << std::endl;

    return p.filename().string();
}

// ファイルパスからファイル名だけを取得する
std::wstring GetFileNameWide(std::string filepath) {

    // u8エンコードでｐａｔｈをコンストラクト
//    fs::path p = fs::u8path(filepath.c_str());
    fs::path p = fs::path(filepath.c_str());

    // システムＷＩＤＥ文字で取得
    const std::wstring wsfilename = p.filename().wstring();

    // ロケールを日本にする
    setlocale(LC_ALL, "Japanese");
    WNEWDEBULOG(wsfilename);

    return wsfilename;
}

// ファイルパスからファイル拡張子を取得する
std::string GetFileExt(std::string filepath) {

    // UTF-8
//    fs::path p = fs::u8path(filepath.c_str());
    fs::path p = fs::path(filepath.c_str());

    return p.extension().string();
}

// ファイルパス(UTF16)からファイル拡張子を取得する
std::wstring GetFileExtWide(std::string filepath) {

    //u8エンコードでｐａｔｈをコンストラクト
    fs::path p = fs::u8path(filepath.c_str());

    return p.extension().wstring();
}

// ファイルパスから親ディレクトリを取得する
std::wstring GetParentDirectoryWide(std::string filepath) {

    // u8エンコードでｐａｔｈをコンストラクト
    fs::path p = fs::u8path(filepath.c_str());

    // システムＷＩＤＥ文字(UTF-16)で取得
    std::wstring wsparentdirectory;

    // 親ディレクトリを持っているか？
    if (p.has_parent_path()) {
        wsparentdirectory = p.parent_path().wstring() + L"\\";
    }
    else {
        wsparentdirectory = L"\\";
    }

    return wsparentdirectory;
}

HRESULT CreateVertexBuffer(ID3D11Device* _device, unsigned int _stride, unsigned int _vertexnum, void* _data, ID3D11Buffer* _buffer)
{
    HRESULT hr;

    D3D11_BUFFER_DESC desc{};
    ZeroMemory(&desc, sizeof(desc));
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = _stride * _vertexnum;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA sd{};
    ZeroMemory(&sd, sizeof(sd));
    sd.pSysMem = _data;

    hr = _device->CreateBuffer(&desc, &sd, &_buffer);
    if (FAILED(hr))
    {
        NEWDEBUGLOG("頂点バッファ作成失敗");
    }
    return hr;
}

HRESULT CreateVertexBufferWrite(ID3D11Device* _device, unsigned int _stride, unsigned int _vertexnum, void* _data, ID3D11Buffer** _buffer)
{
    HRESULT hr;

    D3D11_BUFFER_DESC desc{};
    ZeroMemory(&desc, sizeof(desc));
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = _stride * _vertexnum;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA sd{};
    ZeroMemory(&sd, sizeof(sd));
    sd.pSysMem = _data;

    hr = _device->CreateBuffer(&desc, &sd, _buffer);
    if (FAILED(hr))
    {
        NEWDEBUGLOG("頂点バッファ作成失敗");
    }
    return hr;
}

HRESULT CreateConstantBuffer(ID3D11Device* _device, unsigned int _bytewidth, unsigned int _bytestride, ID3D11Buffer** _buffer)
{
    HRESULT hr;
    // 定数バッファ生成
    D3D11_BUFFER_DESC bufferDesc{};

    bufferDesc.ByteWidth = _bytewidth;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = _bytestride;

    hr = _device->CreateBuffer(&bufferDesc, NULL, _buffer);

    return hr;
}

HRESULT CreateConstantBufferWrite(ID3D11Device* _device, unsigned int _bytewidth, unsigned int _bytestride, ID3D11Buffer** _buffer)
{
    HRESULT hr;
    // 定数バッファ生成
    D3D11_BUFFER_DESC bufferDesc{};

    bufferDesc.ByteWidth = _bytewidth;
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = _bytestride;

    hr = _device->CreateBuffer(&bufferDesc, NULL, _buffer);

    return hr;
}
