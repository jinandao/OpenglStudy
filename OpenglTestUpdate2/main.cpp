#include <glew.h>
#include <freeglut.h>
#include <iostream>
#include "EngineLayer/OpenglTools.h"

//#include "EngineLayer/Texture.h"
#include "GameLayer/Camera.h"
#include "GameLayer/UIElement.h"
#include "GameLayer/ParticleSystem.h"
#include "GameLogic/Player.h"
#include "GameLogic/Enemy.h"
#include "GameLogic/EnemyAI.h"
#include "GameLogic/Bullet.h"
#include "GameLogic/BossAI.h"
#include "GameLogic/BarrageShoot.h"
#include "GameLogic/GameState.h"
#include "GameLogic/ButtonFunctions.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768


std::vector<ParticleElement> particles;

enum GameState gamestate;
bool isChangedState;

GameStateFunc* gameApp;

Button* startButton;
Button* restartButton;
Button* endButton;

static void RenderSceneCB()
{
	if (gamestate == GameState::begin)
	{
		gameApp->BeginUpdate();
	}
	else if (gamestate == GameState::update)
	{
		if (isChangedState == true)
		{
			isChangedState = false;
			gameApp->PlayInitial();
		}
		else
		{
			gameApp->PlayUpdate();
			if (gameApp->Enemies().size() == 0&& gameApp->Boss()->isDead==true)
			{
				isChangedState = true;
				gamestate = GameState::winend;
			}
			if (gameApp->player()->isDead == true)
			{
				isChangedState = true;
				gamestate = GameState::failend;
			}
			if (isChangedState == true)
			{
				gameApp->GameClear();
			}
		}
	}
	else if(gamestate==GameState::failend)
	{
		gameApp->FailEndUpdate();
	}
	else
	{
		gameApp->WinEndUpdate();
	}
	glutSwapBuffers();
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
				startButton->IsClicked(x1, y1);
			}
			if (gamestate == GameState::winend|| gamestate ==GameState::failend)
			{
				restartButton->IsClicked(x1, y1);
				endButton->IsClicked(x1, y1);
			}
		}
	}
}

static void KeyboardCB(unsigned char Key,int x,int y)
{	
	if (gameApp->player() != nullptr)
	{
		gameApp->player()->TakeAction(Key, gameApp->Bullets(),gameApp->BarriageToUse());
	}
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
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

	InitializeGlutCallbacks();

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		return 1;
	}

	gameApp = new GameStateFunc();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	gamestate = GameState::begin;
	isChangedState = false;	

	startButton = new Button("Content/GameStartButton.jpg", 0.5, -0.6, 0.3, 0.15);
	startButton->Init();
	startButton->SetFunc(ButtonFunctions::GameStartButton);
	restartButton = new Button("Content/GameRestartButton.jpg", 0.65, -0.6, 0.3, 0.15);
	restartButton->Init();
	restartButton->SetFunc(ButtonFunctions::GameStartButton);
	endButton = new Button("Content/GameEndButton.jpg", 0.3, -0.6, 0.3, 0.15);
	endButton->Init();
	endButton->SetFunc(ButtonFunctions::GameEndButton);

	glutMainLoop();

	delete gameApp;
	
	return 0;
}

