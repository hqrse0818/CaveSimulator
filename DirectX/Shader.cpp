#include "Shader.h"
#include "renderer.h"
#include "../main.h"
#include <io.h>
#include <iostream>
#include "../Debug.h"
#include "../Utility.h"

using namespace std;

void Shader::CreateBuffer(D3D11_BUFFER_DESC _desc)
{
	// �o�b�t�@���ő�ʂȂ�V�������Ȃ�
	if (p_mBuffers.size() >= BufferMaxSize)
	{
		return;
	}

	// �o�b�t�@�̍쐬
	ID3D11Buffer* buffer{};
	Renderer::GetDevice()->CreateBuffer(&_desc, NULL, &buffer);

	p_mBuffers.emplace_back(buffer);
}

void Shader::CreateBuffer(void* _pData)
{
	// �^����ꂽ�\���̂���o�b�t�@�f�X�N�𐶐�
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(_pData);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	// �萔�o�b�t�@���쐬
	CreateBuffer(bufferDesc);
}

void Shader::CreateBuffer(unsigned int _DataSize)
{
	// �^����ꂽ�\���̂���o�b�t�@�f�X�N�𐶐�
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = _DataSize;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	// �萔�o�b�t�@���쐬
	CreateBuffer(bufferDesc);
}

void Shader::WriteBuffer(UINT _slot, void* _pData)
{
	if(_slot < p_mBuffers.size())
		Renderer::GetDeviceContext()->UpdateSubresource(p_mBuffers[_slot], 0, nullptr, _pData, 0, 0);
}

void Shader::Uninit()
{
	for (auto buffer : p_mBuffers)
	{
		buffer->Release();
	}
	p_mBuffers.clear();
}

HRESULT VertexShader::Load(const char* _FileName)
{
	FILE* file;
	long int filesize;

	// �����ŗ^����ꂽ�t�@�C�����o�C�i���ǂݍ��݃��[�h�ŊJ��
	fopen_s(&file, _FileName, "rb");
	assert(file);

	filesize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[filesize];
	fread(buffer, filesize, 1, file);
	fclose(file);

	// ���_�V�F�[�_�[�̊i�[�ƕۑ�
	HRESULT hr = Renderer::GetDevice()->CreateVertexShader(buffer, filesize, NULL, &p_mVertexShader);
	if (FAILED(hr))
	{
		NEWDEBUGLOG(_FileName << "�V�F�[�_�[�t�@�C���ǂݍ��݃G���[");
	}

	// ���̓��C�A�E�g���`
	// �ύX�ł���悤�ɂ���
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	4 * 3,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	4 * 6,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0,	4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"BONEINDEX", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},		// �A�j���[�V�����p�ɒǉ�
		{"BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},	// �A�j���[�V�����p�ɒǉ�
	};

	UINT numElements = ARRAYSIZE(layout);

	// ���̓��C�A�E�g�̍쐬�ƕۑ�
	Renderer::GetDevice()->CreateInputLayout(layout, numElements, buffer, filesize, &p_mInputLayout);

	delete[] buffer;

	return hr;
}

HRESULT VertexShader::Load(const char* _FileName, Vertex_Type _type)
{
	FILE* file;
	long int filesize;

	NEWDEBUGLOG(_FileName);

	// �����ŗ^����ꂽ�t�@�C�����o�C�i���ǂݍ��݃��[�h�ŊJ��
	fopen_s(&file, _FileName, "rb");
	assert(file);

	filesize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[filesize];
	fread(buffer, filesize, 1, file);
	fclose(file);

	// ���_�V�F�[�_�[�̊i�[�ƕۑ�
	HRESULT hr = Renderer::GetDevice()->CreateVertexShader(buffer, filesize, NULL, &p_mVertexShader);
	if (FAILED(hr))
	{
		NEWDEBUGLOG(_FileName << "�V�F�[�_�[�t�@�C���ǂݍ��݃G���[");
	}
	else
	{
		NEWDEBUGLOG("SUCCEEDED");
	}

	switch (_type)
	{
	case Sprite:
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,		0,	0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	4 * 2,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0,	4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		UINT elementNum = ARRAYSIZE(layout);
		
		hr = Renderer::GetDevice()->CreateInputLayout(layout, elementNum, buffer, filesize, &p_mInputLayout);
	}
	break;

	case StaticMesh:
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	4 *3,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	4 * 6,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0,	4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		UINT elementNum = ARRAYSIZE(layout);

		hr = Renderer::GetDevice()->CreateInputLayout(layout, elementNum, buffer, filesize, &p_mInputLayout);
	}
	break;

	case SkeletalMesh:
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	4 * 3,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	4 * 6,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0,	4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"BONEINDEX", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},		// �A�j���[�V�����p�ɒǉ�
			{"BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},	// �A�j���[�V�����p�ɒǉ�
		};

		UINT numElements = ARRAYSIZE(layout);

		// ���̓��C�A�E�g�̍쐬�ƕۑ�
		Renderer::GetDevice()->CreateInputLayout(layout, numElements, buffer, filesize, &p_mInputLayout);
	}
	break;
	case Geometory:
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,0},
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	4 * 3,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	4 * 6,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		UINT numElements = ARRAYSIZE(layout);

		Renderer::GetDevice()->CreateInputLayout(layout, numElements, buffer, filesize, &p_mInputLayout);
	}
	break;
	case StaticMeshPBR:
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	4 * 3,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	4 * 6,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0,	4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "MATERIALID", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 4 * 12, D3D11_INPUT_PER_VERTEX_DATA},
		};

		UINT numElements = ARRAYSIZE(layout);

		Renderer::GetDevice()->CreateInputLayout(layout, numElements, buffer, filesize, &p_mInputLayout);
	}
	break;

	case SkeletalMeshPBR:
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	4 * 3,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	4 * 6,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0,	4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "MATERIALID", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 4 *12, D3D11_INPUT_PER_VERTEX_DATA},
			{"BONEINDEX", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},		// �A�j���[�V�����p�ɒǉ�
			{"BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},	// �A�j���[�V�����p�ɒǉ�
			
		};

		UINT numElements = ARRAYSIZE(layout);

		// ���̓��C�A�E�g�̍쐬�ƕۑ�
		Renderer::GetDevice()->CreateInputLayout(layout, numElements, buffer, filesize, &p_mInputLayout);
	}
	break;

	case BillBoardPBR:
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	4 * 3,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	4 * 6,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0,	4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		UINT numElements = ARRAYSIZE(layout);

		// ���̓��C�A�E�g�̍쐬�ƕۑ�
		Renderer::GetDevice()->CreateInputLayout(layout, numElements, buffer, filesize, &p_mInputLayout);
	}
	break;
	default:
		break;
	}

	delete[] buffer;

	return  hr;
}

void VertexShader::SetTexture(UINT _slot, ShaderResourceView* _pSRV)
{
	ID3D11ShaderResourceView* pTex = _pSRV->GetSRV();
	Renderer::GetDeviceContext()->VSSetShaderResources(_slot, 1, &pTex);
}

void VertexShader::Bind()
{
	Renderer::GetDeviceContext()->VSSetShader(p_mVertexShader, nullptr, 0);
	Renderer::GetDeviceContext()->IASetInputLayout(p_mInputLayout);

	unsigned int index = CBufferStartIndex;
	for (auto buffer : p_mBuffers)
	{
		Renderer::GetDeviceContext()->VSSetConstantBuffers(index, 1, &buffer);
		index++;
	}
}

void VertexShader::Uninit()
{
	Shader::Uninit();
	SAFE_RELEASE(p_mInputLayout);
	SAFE_RELEASE(p_mVertexShader);
}

HRESULT PixelShader::Load(const char* _FileName)
{
	NEWDEBUGLOG(_FileName);
	FILE* file;
	long int filesize;

	fopen_s(&file, _FileName, "rb");
	assert(file);

	filesize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[filesize];
	fread(buffer, filesize, 1, file);
	fclose(file);

	HRESULT hr = Renderer::GetDevice()->CreatePixelShader(buffer, filesize, NULL, &p_mPixelShader);
	if (FAILED(hr))
	{
		NEWDEBUGLOG(_FileName << "�V�F�[�_�[�ǂݍ��݃G���[");
	}
	else
	{
		NEWDEBUGLOG("SUCCEEDED");
	}

	delete[] buffer;

	return hr;
}

void PixelShader::SetTexture(UINT _slot, ShaderResourceView* _pSRV)
{
	if ( _pSRV != nullptr)
	{
		ID3D11ShaderResourceView* pTex = _pSRV->GetSRV();
		if (pTex != nullptr)
		{
			Renderer::GetDeviceContext()->PSSetShaderResources(_slot, 1, &pTex);
		}
		else
		{
			Renderer::GetDeviceContext()->PSSetShaderResources(_slot, 1, nullptr);
		}
	}
}

void PixelShader::SetTexture(UINT _slot, ID3D11ShaderResourceView* _pSRV)
{
	if (_pSRV != nullptr)
	{
		Renderer::GetDeviceContext()->PSSetShaderResources(_slot, 1, &_pSRV);
	}
}

void PixelShader::Bind()
{
	Renderer::GetDeviceContext()->PSSetShader(p_mPixelShader, nullptr, 0);

	unsigned int index = CBufferStartIndex;
	for (auto buffer : p_mBuffers)
	{
		Renderer::GetDeviceContext()->PSSetConstantBuffers(index, 1, &buffer);
		index++;
	}
}

void PixelShader::Uninit()
{
	Shader::Uninit();
	SAFE_RELEASE(p_mPixelShader);
}

HRESULT ComputeShader::Load(const char* _FileName)
{
	FILE* file;
	long int filesize;

	fopen_s(&file, _FileName, "rb");
	assert(file);

	filesize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[filesize];
	fread(buffer, filesize, 1, file);
	fclose(file);

	HRESULT hr = Renderer::GetDevice()->CreateComputeShader(
		buffer, filesize, nullptr, &p_mComputeShader);
	

	return hr;
}

void ComputeShader::Bind()
{
}

void ComputeShader::Uninit()
{
	Shader::Uninit();
	SAFE_RELEASE(p_mComputeShader);
}