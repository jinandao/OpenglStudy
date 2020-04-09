#pragma once
#include <string>

class GameStateFunc
{
private:

	static void LightInit();
	static void ShadowMapPass();
	static void RenderPass();
	
public:
	
	static void BeginUpdate();
	static void PlayInitial();
	static void PlayUpdate();
	static void GameClear();
	static void EndInitial();
	static void WinEndUpdate();
	static void FailEndUpdate();
};