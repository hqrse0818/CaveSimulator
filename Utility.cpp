#include "Utility.h"
#include	<filesystem>
#include "Debug.h"

namespace fs = std::filesystem;

std::string GetFileName(std::string FilePath)
{
    // u8�G���R�[�h�ł����������R���X�g���N�g
    fs::path p = fs::u8path(FilePath.c_str());
    // �p�X����A�t�@�C�������������f�B���N�g���A�t�@�C�����A
    // �g���q���������t�@�C�����A�g���q�𒊏o
//    std::cout << "�f�B���N�g���� : " << p.parent_path() << std::endl;
//    std::cout << "�t�@�C����  : " << p.filename() << std::endl;
//    std::cout << "�t�@�C�����i�g���q�Ȃ��j : " << p.stem() << std::endl;
//    std::cout << "�g���q : " << p.extension() << std::endl;

    return p.filename().string();
}

// �t�@�C���p�X����t�@�C�����������擾����
std::wstring GetFileNameWide(std::string filepath) {

    // u8�G���R�[�h�ł����������R���X�g���N�g
//    fs::path p = fs::u8path(filepath.c_str());
    fs::path p = fs::path(filepath.c_str());

    // �V�X�e���v�h�c�d�����Ŏ擾
    const std::wstring wsfilename = p.filename().wstring();

    // ���P�[������{�ɂ���
    setlocale(LC_ALL, "Japanese");
    WNEWDEBULOG(wsfilename);

    return wsfilename;
}

// �t�@�C���p�X����t�@�C���g���q���擾����
std::string GetFileExt(std::string filepath) {

    // UTF-8
//    fs::path p = fs::u8path(filepath.c_str());
    fs::path p = fs::path(filepath.c_str());

    return p.extension().string();
}

// �t�@�C���p�X(UTF16)����t�@�C���g���q���擾����
std::wstring GetFileExtWide(std::string filepath) {

    //u8�G���R�[�h�ł����������R���X�g���N�g
    fs::path p = fs::u8path(filepath.c_str());

    return p.extension().wstring();
}

// �t�@�C���p�X����e�f�B���N�g�����擾����
std::wstring GetParentDirectoryWide(std::string filepath) {

    // u8�G���R�[�h�ł����������R���X�g���N�g
    fs::path p = fs::u8path(filepath.c_str());

    // �V�X�e���v�h�c�d����(UTF-16)�Ŏ擾
    std::wstring wsparentdirectory;

    // �e�f�B���N�g���������Ă��邩�H
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
        NEWDEBUGLOG("���_�o�b�t�@�쐬���s");
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
        NEWDEBUGLOG("���_�o�b�t�@�쐬���s");
    }
    return hr;
}

HRESULT CreateConstantBuffer(ID3D11Device* _device, unsigned int _bytewidth, unsigned int _bytestride, ID3D11Buffer** _buffer)
{
    HRESULT hr;
    // �萔�o�b�t�@����
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
    // �萔�o�b�t�@����
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
