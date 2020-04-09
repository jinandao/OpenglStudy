#include <glew.h>
#include <freeglut.h>
#include <iostream>
#include "EngineLayer/OpenglTools.h"

#include "EngineLayer/Texture.h"
#include "GameLayer/Camera.h"
#include "EngineLayer/mesh.h"
#include "EngineLayer/Lighting.h"
#include "EngineLayer/MathTools.h"
#include "EngineLayer/shadow_map_fbo.h"
#include "GameLayer/Game.h"

#include "GameLayer/Pipeline.h"
#include "GameLayer/skybox.h"
#include "GameLayer/UIElement.h"
#include "GameLayer/Collider.h"
#include "GameLayer/ParticleSystem.h"

#include "GameLogic/Player.h"
#include "GameLogic/Enemy.h"
#include "GameLogic/EnemyAI.h"
#include "GameLogic/Bullet.h"
#include "GameLogic/BossAI.h"

#include "EngineLayer/uitechnique.h"
#include "EngineLayer/skyboxtechnique.h"
#include "EngineLayer/shadowTechnique.h"
#include "EngineLayer/LightingTechnique.h"

#include "GameLogic/BarrageShoot.h"
#include "GameLogic/GameState.h"
#include "GameLogic/ButtonFunctions.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

PersProjInfo gPersProjInfo;

ShadowMapFBO shadowMapFBO;

Texture* pTexture=nullptr;
Camera* pCamera = nullptr;

Geometry* pGeo = nullptr;
Geometry* pGeo2 = nullptr;
Geometry* pGeotest = nullptr;

Player* player = nullptr;

Camera* pShadowCamera = nullptr;

Matrix4f m_WVPtransformation;
Matrix4f m_VPtransformation;
//Matrix4f m_WPtransformation;
//Matrix4f m_WVtransformation;
Matrix4f m_Wtransformation;
//Matrix4f m_Vtransformation;
Matrix4f m_ProjTransformation;

DirectionalLight directionLight;
SpotLight spotLight;

GLuint gShadowWVP;
GLuint gShadowTex;

UITechnique* uiTechnique;
SkyBoxTechnique* skyboxTechnique;
ShadowTechnique* shadowTechnique;
LightingTechnique* lightingTechnique;

std::vector<Bullet*> bullets;
std::vector<std::pair<Enemy*,EnemyAI*>> enemies;



Pipeline p;
SkyBox* skybox;

UIElement* GameLabel;
UIElement* GameWinLabel;
UIElement* GameFailLabel;
Button* StartButton;
Button* RestartButton;
Button* EndButton;

ParticleSystem* pstest;

Barriage* barriage;

Enemy* enemy3;
BossAI* bossAI;

enum GameState gamestate;
std::vector<ParticleElement> particles;

bool isChangedState;

static void RenderSceneCB()
{
	if (gamestate == GameState::begin)
	{
		GameStateFunc::BeginUpdate();
	}
	else if (gamestate == GameState::update)
	{
		if (isChangedState == true)
		{
			isChangedState = false;
			GameStateFunc::PlayInitial();
		}
		else
		{
			GameStateFunc::PlayUpdate();			
			if (enemies.size() == 0&&enemy3->isDead==true)
			{
				isChangedState = true;
				gamestate = GameState::winend;
			}
			if (player->isDead == true)
			{
				isChangedState = true;
				gamestate = GameState::failend;
			}
			if (isChangedState == true)
			{
				GameStateFunc::GameClear();
			}
		}
	}
	else if(gamestate==GameState::failend)
	{
		GameStateFunc::FailEndUpdate();
	}
	else
	{
		GameStateFunc::WinEndUpdate();
	}
	glutSwapBuffers();
}

static void PassiveMouseCB(int x, int y)
{
	/*if (pCamera != nullptr)
	{
		pCamera->OnMouse(x, y);
	}*/
}

static void MouseFuncTest(int button,int state,int x,int y)
{
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_RIGHT_BUTTON)
		{
			float x1 = (x - WINDOW_WIDTH / 2.0) / (WINDOW_WIDTH*0.5);
			float y1 = -(y - WINDOW_HEIGHT / 2.0) / (WINDOW_HEIGHT*0.5);
		}
		if (button == GLUT_LEFT_BUTTON)
		{
			float x1 = (x - WINDOW_WIDTH / 2.0) / (WINDOW_WIDTH*0.5);
			float y1 = -(y - WINDOW_HEIGHT / 2.0) / (WINDOW_HEIGHT*0.5);

			if (gamestate == GameState::begin)
			{
				StartButton->IsClicked(x1, y1);
			}
			if (gamestate == GameState::winend|| gamestate ==GameState::failend)
			{
				std::cout << "testbutton" << std::endl;
				RestartButton->IsClicked(x1, y1);
				EndButton->IsClicked(x1, y1);
			}
		}
	}
}

static void KeyboardCB(unsigned char Key,int x,int y)
{	
	//std::cout << "Key:" << Key << std::endl;

	if (player != nullptr)
	{
		player->TakeAction(Key, bullets);
	}
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
	glutPassiveMotionFunc(PassiveMouseCB);
	glutMouseFunc(MouseFuncTest);
	glutKeyboardFunc(KeyboardCB);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial 01");

	//glutEnterGameMode();//?

	InitializeGlutCallbacks();

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		return 1;
	}

	//pCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Vector3f(0, -40, 50), Vector3f(0.5, -0.8, 0.2), Vector3f(0, 1, 0));

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	gamestate = GameState::begin;
	isChangedState = false;

	//CompileUIShader();
	uiTechnique = new UITechnique();
	shadowTechnique = new ShadowTechnique();
	skyboxTechnique = new SkyBoxTechnique();
	lightingTechnique = new LightingTechnique();

	/*btn = new Button("Content/button1.jpg", 0.3, 0.2, 0.3, 0.15);
	btn->Init();
	btn->SetFunc(ButtonFunctions::GameStartButton);
	btn2 = new Button("Content/button2.jpg", -0.2, -0.4, 0.3, 0.15);
	btn2->Init();
	btn2->SetFunc(ButtonFunctions::GameStartButton);

	btn3 = new Button("Content/button2.jpg",-0.2, -0.4, 0.3, 0.15);
	btn3->Init();
	btn3->SetFunc(ButtonFunctions::GameStartButton);*/
	//testUI = new UIElement("Content/test.png", -0.7, -0.2, 0.5, 0.3);
	//testUI->Init();
	GameLabel = new UIElement("Content/GameLabel.jpg", -0.4, 0.3, 0.8, 0.4);
	GameLabel->Init();
	GameWinLabel = new UIElement("Content/VictoryEnd.jpg", -0.4, 0.3, 0.8, 0.4);
	GameWinLabel->Init();
	GameFailLabel = new UIElement("Content/FailureEnd.jpg", -0.4, 0.3, 0.8, 0.4);
	GameFailLabel->Init();
	StartButton=new Button("Content/GameStartButton.jpg",0.5,-0.6,0.3,0.15);
	StartButton->Init();
	StartButton->SetFunc(ButtonFunctions::GameStartButton);
	RestartButton=new Button("Content/GameRestartButton.jpg", 0.65, -0.6, 0.3, 0.15);
	RestartButton->Init();
	RestartButton->SetFunc(ButtonFunctions::GameStartButton);
	EndButton = new Button("Content/GameEndButton.jpg", 0.3, -0.6, 0.3, 0.15);
	EndButton->Init();
	EndButton->SetFunc(ButtonFunctions::GameEndButton);


	glutMainLoop();

	return 0;
}

