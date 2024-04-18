#include "Com_ScreenTexture.h"
#include "../DirectX/renderer.h"
#include "../DirectX/DirectViewList.h"

void Com_ScreenTexture::Draw()
{
	//ID3D11ShaderResourceView* psv = DirectViewList::GetRTV("")->GetSRV();

	//p_mSRV->SetSRV(Renderer::GetRTV(0)->GetSRV());

	Com_Sprite::Draw();
}
