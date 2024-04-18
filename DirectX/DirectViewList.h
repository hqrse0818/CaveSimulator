#pragma once
#include "ShaderResourceView.h"
#include <unordered_map>
#include <string>

// �O���[�o��RTV DSV �Ǘ��N���X

class DirectViewList
{
private:
	static DirectViewList* p_mInstance;
	static std::unordered_map<std::string, RenderTarget*> mRTVList;
	static std::unordered_map<std::string, DepthStencil*> mDSVList;
	DirectViewList() {};
public:
	static void Init();
	static void Uninit();

	// �v�b�V���֐�
	static void PushRTV(const char* _name, RenderTarget* _rtv);
	static void PushDSV(const char* _name, DepthStencil* _dsv);
	// �擾�֐�
	static RenderTarget* GetRTV(const char* _name);
	static DepthStencil* GetDSV(const char* _name);

	// ��ʃN���A�֐�
	static void ClearRendetTargets();
	static void ClearRendetTargets(const float* _color);
	static void ClearRendetTarget(std::string _name);
	static void ClearRendetTarget(std::string _name, const float* _color);

	static void ClearDepthStencils();
	static void ClearDepthStencil(std::string _name);

	static void ClearAll();
};

