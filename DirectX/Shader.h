#pragma once

#include <d3d11.h>
#include <vector>
#include "ShaderResourceView.h"
#include "../Format/Vertex.h"

// �X�v���C�g�p�ėp�V�F�[�_�[
#define VS_SPRITE ("asset\\unlitTextureVS.cso")
#define PS_SPRITE ("asset\\unlitTexturePS.cso")

// ���f���p�ėp�V�F�[�_�[
#define VS_MODEL ("asset\\vertexLightingVS.cso")
#define PS_MODEL ("asset\\vertexLightingPS.cso")

#define BufferMaxSize 9
#define CBufferStartIndex 6
#define SRVStartIndex 1

class Shader
{
protected:
	std::vector<ID3D11Buffer*> p_mBuffers;
public:
	void CreateBuffer(D3D11_BUFFER_DESC _desc);
	void CreateBuffer(void* _pData);
	void CreateBuffer(unsigned int _DataSize);
	void WriteBuffer(UINT _slot, void* _pData);
	// 0�ɓ���Ȃ�����
	virtual void SetTexture(UINT _slot, ShaderResourceView* _pSRV) = 0;
	virtual void Bind() = 0;
	virtual HRESULT Load(const char*) = 0;

	virtual void Uninit();
};

// ���_�V�F�[�_�[
class VertexShader
	: public Shader
{
protected:
	ID3D11VertexShader* p_mVertexShader{};
	ID3D11InputLayout* p_mInputLayout{};
public:
	// VertexShader�̍쐬
	virtual HRESULT Load(const char* _FileName)override;
	HRESULT Load(const char* _FileName, Vertex_Type _type);

	// 0�ɓ���Ȃ�����
	void SetTexture(UINT _slot, ShaderResourceView* _pSRV)override;
	void Bind()override;

	void Uninit()override;
};

// �s�N�Z���V�F�[�_�[
class PixelShader
	: public Shader
{
private:
	ID3D11PixelShader* p_mPixelShader{};
public:
	HRESULT Load(const char* _FileName)override;
	// 0�ɂ���Ȃ�����
	void SetTexture(UINT _slot, ShaderResourceView* _pSRV)override;
	void SetTexture(UINT _slot, ID3D11ShaderResourceView* _pSRV);
	void Bind()override;

	void Uninit()override;
};

// �R���s���[�g�V�F�[�_�[
class ComputeShader
	: public Shader
{
private:
	ID3D11ComputeShader* p_mComputeShader{};

public:
	HRESULT Load(const char* _FileName)override;
	void SetTexture(UINT _slot, ShaderResourceView* _pSRV)override {};
	void Bind()override;

	void Uninit();
};