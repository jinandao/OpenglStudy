#include "GameState.h"
#include <vector>
#include "../GameLayer/UIElement.h"
#include "../GameLayer/skybox.h"
#include "../GameLayer/Camera.h"
#include "../EngineLayer/Texture.h"
#include "../GameLayer/ParticleSystem.h"
#include "../GameLogic/Player.h"
#include "../GameLogic/Enemy.h"
#include "../GameLogic/EnemyAI.h"
#include "../GameLogic/BossAI.h"
#include "../GameLogic/BarrageShoot.h"
#include "../EngineLayer/uitechnique.h"
#include "../EngineLayer/shadowTechnique.h"
#include "../EngineLayer/LightingTechnique.h"
#include "../EngineLayer/skyboxtechnique.h"
#include "../GameLogic/Bullet.h"
#include "../EngineLayer/OpenglTools.h"
#include "../EngineLayer/PSUpdateTechnique.h"
#include "../EngineLayer/PSBillboardTechnique.h"
#include "../GameLayer/UIElement.h"
#include "../GameLogic/ButtonFunctions.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

extern std::vector<ParticleElement> particles;
extern Button* startButton;
extern Button* restartButton;
extern Button* endButton;

void GameStateFunc::BeginUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	uiTechnique->Enable();
	uiTechnique->SetSampler(0);
	gameLabel->Render();
	startButton->Render();
}

void GameStateFunc::PlayInitial()//资源初始化
{
	std::cout << "initial" << std::endl;
	ps = new PSUpdateTechnique();
	pb = new PSBillboardTechnique();

	LightInit();

	shadowMapFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	Vertex Verticestest[3] = { Vertex(Vector3f(-1.0f, -1.0f, 0.0f), Vector2f(0.0f, 0.0f),Vector3f(0.0f,1.0f,0.0f)),
							 Vertex(Vector3f(1.0f, -1.0f, 1.0f), Vector2f(0.0f, 1.0f),Vector3f(0.0f,1.0f,0.0f)),
							Vertex(Vector3f(0.0f, 1.0f, 0.0f), Vector2f(1.0f, 0.0f),Vector3f(0.0f,1.0f,0.0f)) };
	unsigned int Indicestest[3] = { 0,1,2 };
	std::vector<Vertex> vertstest(4);
	for (int i = 0; i < 3; i++)
	{
		vertstest[i] = Verticestest[i];
	}
	std::vector<unsigned int> indicestest(3);
	for (int i = 0; i < 3; i++)
	{
		indicestest[i] = Indicestest[i];
	}
	pGeotest = new Geometry(1, 1);
	pGeotest->AddMeshEntry(vertstest, indicestest, 2);
	std::string testpictest("test.png");
	pGeotest->AddTexure(testpictest);


	Vertex Vertices[4] = { Vertex(Vector3f(0.0f, 0.0f, 0.0f), Vector2f(0.0f, 0.0f),Vector3f(0.0f,1.0f,0.0f)),
							   Vertex(Vector3f(0.0f, 0.0f, 1.0f), Vector2f(0.0f, 1.0f),Vector3f(0.0f,1.0f,0.0f)),
							   Vertex(Vector3f(1.0f, 0.0f, 0.0f), Vector2f(1.0f, 0.0f),Vector3f(0.0f,1.0f,0.0f)),
							   Vertex(Vector3f(1.0f, 0.0f, 1.0f), Vector2f(1.0f, 1.0f),Vector3f(0.0f,1.0f,0.0f)) };
	unsigned int Indices[] = { 0, 3, 1,
							1, 3, 2,
							2, 3, 0,
							1, 2, 0 };
	std::vector<Vertex> verts(4);
	for (int i = 0; i < 4; i++)
	{
		verts[i] = Vertices[i];
	}
	std::vector<unsigned int> indices(12);
	for (int i = 0; i < 12; i++)
	{
		indices[i] = Indices[i];
	}

	pGeo = new Geometry(1, 1);
	pGeo->AddMeshEntry(verts, indices, 2);
	std::string testpic("test.png");
	pGeo->AddTexure(testpic);

	_player = new Player(Vector3f(0.1, 0.1, 0.1), Vector3f(0, 0, 0), Vector3f(25, 0, 25), 100, 1.5, 5);
	_player->SetMesh("Content/phoenix_ugv.md2");

	Enemy* enemy = new Enemy(Vector3f(0.1, 0.1, 0.1), Vector3f(0, 0, 0), Vector3f(10, 0, 10), 50, 1, 3);
	enemy->SetMesh("Content/phoenix_ugv.md2");
	EnemyAI* enemyAI = new EnemyAI(_player, enemy);
	Enemy* enemy2 = new Enemy(Vector3f(0.1, 0.1, 0.1), Vector3f(0, 0, 0), Vector3f(30, 0, 12), 50, 1, 3);
	enemy2->SetMesh(enemy->GetMesh());
	EnemyAI* enemyAI2 = new EnemyAI(_player, enemy2);
	boss = new Enemy(Vector3f(0.2, 0.1, 0.2), Vector3f(0, 20, 0), Vector3f(40, 0, 43), 50, 1, 3);
	boss->SetMesh(enemy->GetMesh());
	bossAI = new BossAI(_player, boss);

	enemies.emplace_back(std::make_pair(enemy, enemyAI));
	enemies.emplace_back(std::make_pair(enemy2, enemyAI2));

	gPersProjInfo.FOV = 60.0f;
	gPersProjInfo.Height = WINDOW_HEIGHT;
	gPersProjInfo.Width = WINDOW_WIDTH;
	gPersProjInfo.zNear = 1.0f;
	gPersProjInfo.zFar = 500.0f;

	skyBox = new SkyBox(pCamera, gPersProjInfo);
	skyBox->Init("", "Content/sp3right.jpg",
		"Content/sp3left.jpg",
		"Content/sp3top.jpg",
		"Content/sp3bot.jpg",
		"Content/sp3front.jpg",
		"Content/sp3back.jpg");

	//初始化透视投影矩阵
	p.SetPerspectiveProj(gPersProjInfo);

	barriage = new Barriage();
	barriage->LoadBarriage("Content/spider.obj");	
}

void GameStateFunc::PlayUpdate()
{
	ShadowMapPass();
	RenderPass();
}

void GameStateFunc::EndInitial()
{

}

void GameStateFunc::WinEndUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	uiTechnique->Enable();
	uiTechnique->SetSampler(0);

	gameWinLabel->Render();
	restartButton->Render();
	endButton->Render();
}

void GameStateFunc::FailEndUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	uiTechnique->Enable();
	uiTechnique->SetSampler(0);

	gameFailLabel->Render();
	restartButton->Render();
	endButton->Render();
}

Player* GameStateFunc::player()
{
	return _player;
}

const std::vector<std::pair<Enemy*, EnemyAI*>>& GameStateFunc::Enemies()
{
	return enemies;
}

Enemy* GameStateFunc::Boss()
{
	return boss;
}

Barriage* GameStateFunc::BarriageToUse()
{
	return barriage;
}

std::vector<Bullet*>& GameStateFunc::Bullets()
{
	return bullets;
}

void GameStateFunc::LightInit()
{
	directionLight.Color = COLOR_WHITE;
	directionLight.AmbientIntensity = 0.6;
	directionLight.DiffuseIntensity = 0.5;
	directionLight.Direction = Vector3f(0.5, 0.5, 0.5);

	spotLight.Color = COLOR_CYAN;
	spotLight.AmbientIntensity = 0.1;
	spotLight.DiffuseIntensity = 0.8;
	spotLight.Position = Vector3f(5, 20, 5);
	spotLight.Attenuation.Linear = 0.01f;
	spotLight.Direction = Vector3f(0.5f, -0.5f, 0);
	spotLight.Cutoff = 0.0f;
}

void GameStateFunc::ShadowMapPass()
{
	shadowTechnique->Enable();
	shadowMapFBO.BindForWriting();

	glClear(GL_DEPTH_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	p.SetCamera(*pShadowCamera);
	if (_player != nullptr && _player->isDead == false)
	{
		p.Scale(_player->GetScale());
		p.WorldPos(_player->GetPos());
		p.Rotate(_player->GetRotation());
		//glUniformMatrix4fv(gShadowWVP, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());		
		shadowTechnique->SetWVP(p.GetWVPTrans());
		_player->Render();
	}

	for (auto ite = enemies.begin(); ite != enemies.end();)
	{
		if (ite->first->isDead == false)
		{
			p.Scale(ite->first->GetScale());
			p.WorldPos(ite->first->GetPos());
			p.Rotate(ite->first->GetRotation());
			shadowTechnique->SetWVP(p.GetWVPTrans());
			ite->first->Render();
		}
		ite++;
	}

	for (auto ite = bullets.begin(); ite != bullets.end();)
	{
		if ((*ite)->isEnd == false)
		{
			p.Scale((*ite)->GetScale());
			p.WorldPos((*ite)->GetPos());
			p.Rotate((*ite)->GetRotation());
			shadowTechnique->SetWVP(p.GetWVPTrans());
			(*ite)->GetGeo()->Render();
		}
		ite++;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GameStateFunc::RenderPass()
{
	pCamera->OnRender();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	p.SetCamera(*pCamera);
	p.SetPerspectiveProj(gPersProjInfo);

	SkyBoxRender();
	/*shadowShaderProgram->Bind();
	p.Scale(quadScale);
	Vector3f test = quadPos + Vector3f(0, 17, 0);
	p.WorldPos(test);
	p.Rotate(quadRotate);
	glUniformMatrix4fv(gShadowWVP, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());
	shadowMapFBO.BindForReading(0);
	pGeo->Render();*/
	//shaderProgram->Bind();
	
	lightingTechnique->Enable();
	lightingTechnique->SetLight(directionLight, spotLight);
	lightingTechnique->SetSampler(0);
	lightingTechnique->SetShadowSampler(1);
	shadowMapFBO.BindForReading(1);

	p.SetObjectInfo(quadPos, quadScale, quadRotate);

	lightingTechnique->SetWorld(p.GetWorldTrans());
	lightingTechnique->SetWVP(p.GetWVPTrans());
	p.SetCamera(*pShadowCamera);

	lightingTechnique->SetLightWVP(p.GetWVPTrans());
	pTexture->Bind();
	//shadowMapFBO.BindForReading(0);	
	pGeo->Render();

	p.SetCamera(*pCamera);
	if (_player != nullptr && _player->isDead == false)
	{
		p.SetObjectInfo(_player->GetPos(), _player->GetScale(), _player->GetRotation());

		lightingTechnique->SetWorld(p.GetWorldTrans());
		lightingTechnique->SetWVP(p.GetWVPTrans());
		p.SetCamera(*pShadowCamera);

		lightingTechnique->SetLightWVP(p.GetWVPTrans());
		_player->Render();
		p.SetCamera(*pCamera);
	}
	else
	{
		SAFE_DELETE(_player);
	}

	for (auto ite = enemies.begin(); ite != enemies.end();)
	{
		if (ite->first->isDead == false)
		{
			p.SetObjectInfo(ite->first->GetPos(), ite->first->GetScale(), ite->first->GetRotation());
			lightingTechnique->SetWorld(p.GetWorldTrans());
			lightingTechnique->SetWVP(p.GetWVPTrans());
			p.SetCamera(*pShadowCamera);
			lightingTechnique->SetLightWVP(p.GetWVPTrans());
			ite->first->Render();
			ite->second->Update(bullets);
			ite++;
			p.SetCamera(*pCamera);
		}
		else
		{
			auto toDel1 = ite->first;
			auto toDel2 = ite->second;
			ite = enemies.erase(ite);
			SAFE_DELETE(toDel1);
			SAFE_DELETE(toDel2);
		}
	}

	if (boss->isDead == false)
	{
		p.SetObjectInfo(boss->GetPos(), boss->GetScale(), boss->GetRotation());

		lightingTechnique->SetWorld(p.GetWorldTrans());
		lightingTechnique->SetWVP(p.GetWVPTrans());
		p.SetCamera(*pShadowCamera);

		lightingTechnique->SetLightWVP(p.GetWVPTrans());
		boss->Render();
		bossAI->Update(bullets);
		p.SetCamera(*pCamera);
	}

	for (auto ite = bullets.begin(); ite != bullets.end();)
	{
		if ((*ite)->isEnd == false)
		{
			p.SetObjectInfo((*ite)->GetPos(), (*ite)->GetScale(), (*ite)->GetRotation());
			lightingTechnique->SetWorld(p.GetWorldTrans());
			lightingTechnique->SetWVP(p.GetWVPTrans());
			p.SetCamera(*pShadowCamera);
			lightingTechnique->SetLightWVP(p.GetWVPTrans());
			(*ite)->GetGeo()->Render();
			(*ite)->Move();
			(*ite)->CheckHurt(_player,enemies, boss);
			ite++;
			p.SetCamera(*pCamera);
		}
		else
		{
			auto toDel = *ite;
			ite = bullets.erase(ite);
			SAFE_DELETE(toDel);
		}
	}
	
	ParticlesRender();
	BarriageRender();
}

void GameStateFunc::ParticlesRender()
{
	for (auto ite = particles.begin(); ite != particles.end(); )
	{
		if (ite->times == 180)
		{
			ite->times -= 1;
			//std::cout << "test ps begin" << std::endl;
			ite->ps = new ParticleSystem();
			ite->ps->SetTechnique(ps, pb);
			ite->ps->InitParticleSystem(ite->pos);
			ite++;
		}
		else if (ite->times == 0)
		{			
			SAFE_DELETE(ite->ps);
			ite = particles.erase(ite);
		}
		else
		{
			ite->times -= 1;
			//std::cout << "test ps update" << std::endl;
			if (ite->ps != nullptr)
				ite->ps->Render(50, p.GetVPTrans(), pCamera->GetPos());
			ite++;
		}
	}
}

void GameStateFunc::BarriageRender()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	barriage->Update(p,enemies, boss);
	glDisable(GL_BLEND);
}

void GameStateFunc::SkyBoxRender()
{
	skyboxTechnique->Enable();
	Pipeline p;
	p.Scale(20.0f, 20.0f, 20.0f);
	p.Rotate(0.0f, 0.0f, 0.0f);
	p.WorldPos(pCamera->GetPos().x, pCamera->GetPos().y, pCamera->GetPos().z);
	p.SetCamera(pCamera->GetPos(), pCamera->GetTarget(), pCamera->GetUp());
	p.SetPerspectiveProj(gPersProjInfo);

	skyboxTechnique->SetSampler(0);
	skyboxTechnique->SetWVP(p.GetWVPTrans());
	skyBox->Render();
}

GameStateFunc::GameStateFunc()
{
	uiTechnique = new UITechnique();
	shadowTechnique = new ShadowTechnique();
	skyboxTechnique = new SkyBoxTechnique();
	lightingTechnique = new LightingTechnique();

	quadPos = Vector3f(0, 0, 0);
	quadRotate = Vector3f(0, 0, 0);
	quadScale = Vector3f(100, 0, 100);

	gameLabel = new UIElement("Content/GameLabel.jpg", -0.4, 0.3, 0.8, 0.4);
	gameLabel->Init();
	gameWinLabel = new UIElement("Content/VictoryEnd.jpg", -0.4, 0.3, 0.8, 0.4);
	gameWinLabel->Init();
	gameFailLabel = new UIElement("Content/FailureEnd.jpg", -0.4, 0.3, 0.8, 0.4);
	gameFailLabel->Init();

	pCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Vector3f(20, 30, -20), Vector3f(0, -0.3, 0.9), Vector3f(0, 1, 0));
	pShadowCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, spotLight.Position, spotLight.Direction, Vector3f(0.0, 1.0, 0.0));
	pTexture = new Texture("test.png");
}

GameStateFunc::~GameStateFunc()
{
	SAFE_DELETE(uiTechnique);
	SAFE_DELETE(shadowTechnique);
	SAFE_DELETE(skyboxTechnique);
	SAFE_DELETE(lightingTechnique);
	SAFE_DELETE(pTexture);
	SAFE_DELETE(pCamera);
	SAFE_DELETE(pShadowCamera);
	SAFE_DELETE(uiTechnique);
	SAFE_DELETE(gameLabel);
	SAFE_DELETE(gameWinLabel);
	SAFE_DELETE(gameFailLabel);
}

void GameStateFunc::GameClear()
{
	std::cout << "game clear" << std::endl;
	SAFE_DELETE(_player);
	for (auto ite = enemies.begin(); ite != enemies.end(); )
	{
		if (ite->first != nullptr)
		{
			auto toDel1 = ite->first;
			auto toDel2 = ite->second;
			SAFE_DELETE(toDel1);
			SAFE_DELETE(toDel2);
		}
		else
		{
			ite++;
		}
	}
	for (auto ite = particles.begin(); ite != particles.end(); )
	{
		SAFE_DELETE(ite->ps);
		ite = particles.erase(ite);
	}
	SAFE_DELETE(boss);
	SAFE_DELETE(bossAI);
	SAFE_DELETE(pGeo);
	SAFE_DELETE(ps);
	SAFE_DELETE(pb);
	SAFE_DELETE(skyBox);
}

