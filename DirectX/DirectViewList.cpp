#include "DirectViewList.h"
#include "../Utility.h"

DirectViewList* DirectViewList::p_mInstance = nullptr;
std::unordered_map<std::string, RenderTarget*> DirectViewList::mRTVList{};
std::unordered_map<std::string, DepthStencil*> DirectViewList::mDSVList{};

void DirectViewList::Init()
{
	if (!p_mInstance)
	{
		p_mInstance = new DirectViewList();
	}
}

void DirectViewList::Uninit()
{
	for (auto rtv : mRTVList)
	{
		if (rtv.second != nullptr)
			rtv.second->Uninit();
		SAFE_NULL(rtv.second);
	}
	mRTVList.clear();

	for (auto dsv : mDSVList)
	{
		if (dsv.second != nullptr)
			dsv.second->Uninit();
		SAFE_NULL(dsv.second);
	}
	mDSVList.clear();

	if (p_mInstance)
	{
		delete p_mInstance;
	}
}

void DirectViewList::PushRTV(const char* _name, RenderTarget* _rtv)
{
	if (_rtv->GetView())
	{
		mRTVList[_name] = _rtv;
	}
}

void DirectViewList::PushDSV(const char* _name, DepthStencil* _dsv)
{
	if (_dsv->GetView())
	{
		mDSVList[_name] = _dsv;
	}
}

RenderTarget* DirectViewList::GetRTV(const char* _name)
{
	if (mRTVList.count(_name) > 0)
	{
		return mRTVList[_name];
	}
	return nullptr;
}

DepthStencil* DirectViewList::GetDSV(const char* _name)
{
	if (mDSVList.count(_name) > 0)
	{
		return mDSVList[_name];
	}
	return nullptr;
}

void DirectViewList::ClearRendetTargets()
{
	for (auto rtv : mRTVList)
	{
		rtv.second->Clear();
	}
}

void DirectViewList::ClearRendetTargets(const float* _color)
{
	for (auto rtv : mRTVList)
	{
		rtv.second->Clear(_color);
	}
}

void DirectViewList::ClearRendetTarget(std::string _name)
{
	if (mRTVList.count(_name) > 0)
	{
		mRTVList[_name]->Clear();
	}
}

void DirectViewList::ClearRendetTarget(std::string _name, const float* _color)
{
	if (mRTVList.count(_name) > 0)
	{
		mRTVList[_name]->Clear(_color);
	}
}

void DirectViewList::ClearDepthStencils()
{
	for (auto dsv : mDSVList)
	{
		dsv.second->Clear();
	}
}

void DirectViewList::ClearDepthStencil(std::string _name)
{
	if (mDSVList.count(_name) > 0)
	{
		mDSVList[_name]->Clear();
	}
}

void DirectViewList::ClearAll()
{
	for (auto rtv : mRTVList)
	{
		rtv.second->Clear();
	}
	for (auto dsv : mDSVList)
	{
		dsv.second->Clear();
	}
}
