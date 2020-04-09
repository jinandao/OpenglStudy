#include "GameState.h"
#include "../GameLayer/UIElement.h"
#include "../GameLayer/skybox.h"
#include "../EngineLayer/Lighting.h"
#include "../GameLayer/Camera.h"
#include "../EngineLayer/Texture.h"
#include "../GameLayer/ParticleSystem.h"
#include "../EngineLayer/shadow_map_fbo.h"
#include "../GameLogic/Player.h"
#include "../GameLogic/Enemy.h"
#include "../GameLogic/EnemyAI.h"
#include "../GameLogic/BossAI.h"
#include "../GameLayer/Pipeline.h"
#include "../GameLogic/BarrageShoot.h"
#include "../EngineLayer/uitechnique.h"
#include "../EngineLayer/shadowTechnique.h"
#include "../EngineLayer/LightingTechnique.h"
#include "../EngineLayer/skyboxtechnique.h"
#include "../GameLogic/Bullet.h"
#include "../EngineLayer/OpenglTools.h"
#include <vector>
#include "../EngineLayer/PSUpdateTechnique.h"
#include "../EngineLayer/PSBillboardTechnique.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

extern SkyBox* skybox;
extern UIElement* testUI;
extern Button* btn;
extern Button* btn2;
extern Button* btn3;
extern DirectionalLight directionLight;
extern SpotLight spotLight;

extern Geometry* pGeotest;
extern Camera* pCamera;
extern Texture* pTexture;
extern ParticleSystem* pstest;
extern ShadowMapFBO shadowMapFBO;
extern PersProjInfo gPersProjInfo;

extern Geometry* pGeo;
extern Player* player;

extern Enemy* enemy3;
extern BossAI* bossAI;
extern Barriage* barriage;

extern Pipeline p;
extern std::vector<std::pair<Enemy*, EnemyAI*>> enemies;
extern Camera *pShadowCamera;

extern UITechnique* uiTechnique;
extern SkyBoxTechnique* skyboxTechnique;
extern ShadowTechnique* shadowTechnique;
extern LightingTechnique* lightingTechnique;
extern std::vector<Bullet*> bullets;

extern std::vector<ParticleElement> particles;

extern UIElement* GameLabel;
extern UIElement* GameWinLabel;
extern UIElement* GameFailLabel;
extern Button* StartButton;
extern Button* RestartButton;
extern Button* EndButton;

Vector3f quadPos(0, 0, 0);
Vector3f quadRotate(0, 0, 0);
Vector3f quadScale(100, 0, 100);

PSUpdateTechnique* ps;
PSBillboardTechnique* pb;

void GameStateFunc::BeginUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	uiTechnique->Enable();
	uiTechnique->SetSampler(0);
	GameLabel->Render();
	StartButton->Render();
}

void GameStateFunc::PlayInitial()//资源初始化
{
	std::cout << "initial" << std::endl;
	ps = new PSUpdateTechnique();
	pb = new PSBillboardTechnique();

	LightInit();
	pCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Vector3f(20, 30, -20), Vector3f(0, -0.3, 0.9), Vector3f(0, 1, 0));
	//pCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);
	pShadowCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, spotLight.Position, spotLight.Direction, Vector3f(0.0, 1.0, 0.0));

	pTexture = new Texture("test.png");

	pstest = new ParticleSystem();
	pstest->SetTechnique(ps, pb);
	pstest->InitParticleSystem(Vector3f(30, 0, 40));

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

	//btn = new Button("Content/button1.jpg", 0.3, 0.2, 0.3, 0.15);
	////btn = new Button("Content/test2.jpg", -0.3, -0.4, 0.2, 0.1);
	//btn->Init();
	//btn->SetFunc(BtnFunc);
	//btn2 = new Button("Content/button2.jpg", -0.2, -0.4, 0.3, 0.15);
	//btn2->Init();
	//btn2->SetFunc(BtnFunc2);

	player = new Player(Vector3f(0.1, 0.1, 0.1), Vector3f(0, 0, 0), Vector3f(25, 0, 25), 100, 1.5, 5);
	player->SetMesh("Content/phoenix_ugv.md2");

	Enemy* enemy = new Enemy(Vector3f(0.1, 0.1, 0.1), Vector3f(0, 0, 0), Vector3f(10, 0, 10), 50, 1, 3);
	enemy->SetMesh("Content/phoenix_ugv.md2");
	EnemyAI* enemyAI = new EnemyAI(player, enemy);
	Enemy* enemy2 = new Enemy(Vector3f(0.1, 0.1, 0.1), Vector3f(0, 0, 0), Vector3f(30, 0, 12), 50, 1, 3);
	enemy2->SetMesh(enemy->GetMesh());
	EnemyAI* enemyAI2 = new EnemyAI(player, enemy2);
	enemy3 = new Enemy(Vector3f(0.2, 0.1, 0.2), Vector3f(0, 20, 0), Vector3f(40, 0, 43), 50, 1, 3);
	enemy3->SetMesh(enemy->GetMesh());
	bossAI = new BossAI(player, enemy3);

	/*enemy4 = new Enemy(Vector3f(0.2, 0.1, 0.2), Vector3f(0, 30, 0), Vector3f(45, 0, 19), 50, 1, 3);
	enemy4->SetMesh(enemy->GetMesh());
	bossAI = new BossAI(player, enemy4);*/

	enemies.emplace_back(std::make_pair(enemy, enemyAI));
	enemies.emplace_back(std::make_pair(enemy2, enemyAI2));

	gPersProjInfo.FOV = 60.0f;
	gPersProjInfo.Height = WINDOW_HEIGHT;
	gPersProjInfo.Width = WINDOW_WIDTH;
	gPersProjInfo.zNear = 1.0f;
	gPersProjInfo.zFar = 500.0f;

	skybox = new SkyBox(pCamera, gPersProjInfo);

	skybox->Init("", "Content/sp3right.jpg",
		"Content/sp3left.jpg",
		"Content/sp3top.jpg",
		"Content/sp3bot.jpg",
		"Content/sp3front.jpg",
		"Content/sp3back.jpg");

	//初始化透视投影矩阵
	p.SetPerspectiveProj(gPersProjInfo);

	barriage = new Barriage();
	barriage->LoadBarriage("Content/spider.obj");
	barriage->BulletInitial(player->GetPos(), player->GetRotation());
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

	GameWinLabel->Render();
	RestartButton->Render();
	EndButton->Render();
}

void GameStateFunc::FailEndUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	uiTechnique->Enable();
	uiTechnique->SetSampler(0);

	GameFailLabel->Render();
	RestartButton->Render();
	EndButton->Render();
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
	if (player != nullptr && player->isDead == false)
	{
		p.Scale(player->GetScale());
		p.WorldPos(player->GetPos());
		p.Rotate(player->GetRotation());
		//glUniformMatrix4fv(gShadowWVP, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());		
		shadowTechnique->SetWVP(p.GetWVPTrans());
		player->Render();
	}

	for (auto ite = enemies.begin(); ite != enemies.end();)
	{
		if (ite->first->isDead == false)
		{

			p.Scale(ite->first->GetScale());
			p.WorldPos(ite->first->GetPos());
			p.Rotate(ite->first->GetRotation());
			//glUniformMatrix4fv(gShadowWVP, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());
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
			//glUniformMatrix4fv(gShadowWVP, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());
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

	skyboxTechnique->Enable();
	Pipeline p;
	p.Scale(20.0f, 20.0f, 20.0f);
	p.Rotate(0.0f, 0.0f, 0.0f);
	p.WorldPos(pCamera->GetPos().x, pCamera->GetPos().y, pCamera->GetPos().z);
	p.SetCamera(pCamera->GetPos(), pCamera->GetTarget(), pCamera->GetUp());
	p.SetPerspectiveProj(gPersProjInfo);

	skyboxTechnique->SetSampler(0);
	skyboxTechnique->SetWVP(p.GetWVPTrans());
	skybox->Render();

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
	if (player != nullptr && player->isDead == false)
	{
		p.SetObjectInfo(player->GetPos(), player->GetScale(), player->GetRotation());

		lightingTechnique->SetWorld(p.GetWorldTrans());
		lightingTechnique->SetWVP(p.GetWVPTrans());
		p.SetCamera(*pShadowCamera);

		lightingTechnique->SetLightWVP(p.GetWVPTrans());
		player->Render();
		p.SetCamera(*pCamera);
	}
	else if (player != nullptr)
	{
		delete player;
		player = nullptr;
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
			Destroy(toDel1);
			delete toDel2;
			toDel2 = nullptr;
		}
	}

	if (enemy3->isDead == false)
	{
		p.SetObjectInfo(enemy3->GetPos(), enemy3->GetScale(), enemy3->GetRotation());

		lightingTechnique->SetWorld(p.GetWorldTrans());
		lightingTechnique->SetWVP(p.GetWVPTrans());
		p.SetCamera(*pShadowCamera);

		lightingTechnique->SetLightWVP(p.GetWVPTrans());
		enemy3->Render();
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
			(*ite)->CheckHurt();
			ite++;

			p.SetCamera(*pCamera);
		}
		else
		{
			auto toDel = *ite;
			ite = bullets.erase(ite);
			Destroy(toDel);
		}
	}

	//pstest->Render(50, p.GetVPTrans(), pCamera->GetPos());
	
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
			//std::cout << "test ps delete" << std::endl;
			delete ite->ps;
			ite->ps = nullptr;
			ite=particles.erase(ite);
		}
		else
		{
			ite->times -= 1;
			//std::cout << "test ps update" << std::endl;
			if(ite->ps!=nullptr)
				ite->ps->Render(50, p.GetVPTrans(), pCamera->GetPos());
			ite++;
		}				
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	barriage->Update(p);
	glDisable(GL_BLEND);
}

void GameStateFunc::GameClear()
{
	std::cout << "game clear" << std::endl;
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}
	for (auto ite = enemies.begin(); ite != enemies.end(); )
	{
		if (ite->first != nullptr)
		{
			auto toDel1 = ite->first;
			auto toDel2 = ite->second;
			ite = enemies.erase(ite);
			delete toDel1;
			toDel1 = nullptr;
			delete toDel2;
			toDel2 = nullptr;
		}
		else
		{
			ite++;
		}
	}
	for (auto ite = particles.begin(); ite != particles.end(); )
	{
		delete ite->ps;
		ite->ps = nullptr;
		ite = particles.erase(ite);
	}
	if (enemy3 != nullptr)
	{
		delete enemy3;
		enemy3 = nullptr;
	}
	if (pGeo != nullptr)
	{
		delete pGeo;
		pGeo = nullptr;
	}
	if (ps != nullptr)
	{
		delete ps;
		ps = nullptr;
	}
	if (pb != nullptr)
	{
		delete pb;
		pb = nullptr;
	}
}

