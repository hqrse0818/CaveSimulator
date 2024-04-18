#include "Com_RenderCollider.h"
#include "../GameObject/GameObject.h"
#include "../Utility.h"

using namespace DirectX::SimpleMath;

Com_RenderCollider::Com_RenderCollider()
{
	SetVertex("VS_MODEL");
	SetPixel("NoHit");
}

void Com_RenderCollider::Uninit()
{
	Com_Visual::Uninit();
	SAFE_RELEASE(p_mVertexBuffer);
}

void Com_RenderCollider::Draw()
{
	Bind();
	// ���_�o�b�t�@�̐ݒ�
	UINT stride = sizeof(Vector3);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &p_mVertexBuffer, &stride, &offset);


	Matrix trans;
	trans = DirectX::SimpleMath::Matrix::CreateTranslation(p_mObject->p_mTransform->mPosition);
	Renderer::SetWorldMatrix(&trans);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::SetTopologyLineList();
}

void Com_RenderCollider::SetPixelShaderNoHit()
{
	SetPixel("NoHit");
}

void Com_RenderCollider::SetPixelShaderHit()
{
	SetPixel("Hit");
}
