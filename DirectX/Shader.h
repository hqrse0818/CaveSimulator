#pragma once

#include <d3d11.h>
#include <vector>
#include "ShaderResourceView.h"
#include "../Format/Vertex.h"

// スプライト用汎用シェーダー
#define VS_SPRITE ("asset\\unlitTextureVS.cso")
#define PS_SPRITE ("asset\\unlitTexturePS.cso")

// モデル用汎用シェーダー
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
	// 0に入れないこと
	virtual void SetTexture(UINT _slot, ShaderResourceView* _pSRV) = 0;
	virtual void Bind() = 0;
	virtual HRESULT Load(const char*) = 0;

	virtual void Uninit();
};

// 頂点シェーダー
class VertexShader
	: public Shader
{
protected:
	ID3D11VertexShader* p_mVertexShader{};
	ID3D11InputLayout* p_mInputLayout{};
public:
	// VertexShaderの作成
	virtual HRESULT Load(const char* _FileName)override;
	HRESULT Load(const char* _FileName, Vertex_Type _type);

	// 0に入れないこと
	void SetTexture(UINT _slot, ShaderResourceView* _pSRV)override;
	void Bind()override;

	void Uninit()override;
};

// ピクセルシェーダー
class PixelShader
	: public Shader
{
private:
	ID3D11PixelShader* p_mPixelShader{};
public:
	HRESULT Load(const char* _FileName)override;
	// 0にいれないこと
	void SetTexture(UINT _slot, ShaderResourceView* _pSRV)override;
	void SetTexture(UINT _slot, ID3D11ShaderResourceView* _pSRV);
	void Bind()override;

	void Uninit()override;
};

// コンピュートシェーダー
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