#pragma once
#include <string>
#include "../EngineLayer/shadow_map_fbo.h"
#include "../EngineLayer/Lighting.h"
#include "../GameLayer/Pipeline.h"
#include "../EngineLayer/MathTools.h"
#include "../GameLayer/skybox.h"
#include <vector>

class UITechnique;
class SkyBoxTechnique;
class ShadowTechnique;
class LightingTechnique;
class ShadowMapFBO;
class Texture;
class Geometry;
class Camera;
class PSBillboardTechnique;
class PSUpdateTechnique;
class UIElement;
class Button;
class Player;
class Enemy;
class EnemyAI;
class BossAI;
class Barriage;
class Bullet;

class GameStateFunc
{
private:
	
	SkyBox* skyBox;	
	UITechnique* uiTechnique;
	SkyBoxTechnique* skyboxTechnique;
	ShadowTechnique* shadowTechnique;
	LightingTechnique* lightingTechnique;
	ShadowMapFBO shadowMapFBO;
	Texture* pTexture;
	Camera* pCamera;
	Geometry* pGeo;
	Geometry* pGeo2;
	Geometry* pGeotest;
	Camera* pShadowCamera;
	PSUpdateTechnique* ps;
	PSBillboardTechnique* pb;
	Vector3f quadPos;
	Vector3f quadRotate;
	Vector3f quadScale;
	DirectionalLight directionLight;
	SpotLight spotLight;
	PersProjInfo gPersProjInfo;
	Pipeline p;
	Player* _player;
	std::vector<std::pair<Enemy*, EnemyAI*>> enemies;
	Enemy* boss;
	BossAI* bossAI;
	Barriage* barriage;
	std::vector<Bullet*> bullets;

	UIElement* gameLabel;
	UIElement* gameWinLabel;
	UIElement* gameFailLabel;

private:
	void LightInit();
	void ShadowMapPass();
	void RenderPass();
	void ParticlesRender();
	void BarriageRender();
	void SkyBoxRender();

public:
	GameStateFunc();
	~GameStateFunc();

	void BeginUpdate();
	void PlayInitial();
	void PlayUpdate();
	void GameClear();
	void EndInitial();
	void WinEndUpdate();
	void FailEndUpdate();
	Player* player();
	const std::vector<std::pair<Enemy*, EnemyAI*>>& Enemies();
	Enemy* Boss();
	Barriage* BarriageToUse();
	std::vector<Bullet*>& Bullets();
};