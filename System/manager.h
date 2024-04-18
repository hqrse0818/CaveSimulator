#pragma once
#include	<cinttypes>
#include "../Scene/Scene.h"

// ëOï˚éQè∆
class Application;

class Manager
{
private:
	static Scene* p_mCurrentScene;
	static Scene* p_mNextScene;
	static bool bChangeScene;
public:
	static void Init(Application* ap);
	static void Uninit();
	static void Update(uint64_t d);
	static void Draw(uint64_t d);

	static class Scene* GetScene()
	{
		return p_mCurrentScene;
	}

	template <typename T>
	static void SetScene()
	{
		if (p_mCurrentScene)
		{
			p_mCurrentScene->UninitBase();
			delete p_mCurrentScene;
		}
		p_mCurrentScene = new T();
		p_mCurrentScene->InitBase();
		p_mCurrentScene->StartBase();
	}

	template <typename T>
	static void SetNextScene()
	{
		if (p_mNextScene)
		{
			delete p_mNextScene;
			p_mNextScene = nullptr;
		}
		p_mNextScene = new T();
		bChangeScene = true;
	}
};