#include <glew.h>
#include <freeglut.h>
#include <iostream>
#include "EngineLayer/OpenglTools.h"
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
#include "GlobalVariablesManager.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

static class GlobalVariablesManager manager;

static void RenderSceneCB()
{
	if (manager.gamestate == GameState::begin)
	{
		manager.gameApp->BeginUpdate();
	}
	else if (manager.gamestate == GameState::update)
	{
		if (manager.isChangedState == true)
		{
			manager.isChangedState = false;
			manager.gameApp->PlayInitial();
		}
		else
		{
			manager.gameApp->PlayUpdate();
			if (manager.gameApp->Enemies().size() == 0&& manager.gameApp->Boss()->isDead==true)
			{
				manager.isChangedState = true;
				manager.gamestate = GameState::winend;
			}
			if (manager.gameApp->player()->isDead == true)
			{
				manager.isChangedState = true;
				manager.gamestate = GameState::failend;
			}
			if (manager.isChangedState == true)
			{
				manager.gameApp->GameClear();
			}
		}
	}
	else if(manager.gamestate==GameState::failend)
	{
		manager.gameApp->FailEndUpdate();
	}
	else
	{
		manager.gameApp->WinEndUpdate();
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
			if (manager.gamestate == GameState::begin)
			{
				manager.startButton->IsClicked(x1, y1);
			}
			if (manager.gamestate == GameState::winend|| manager.gamestate ==GameState::failend)
			{
				manager.restartButton->IsClicked(x1, y1);
				manager.endButton->IsClicked(x1, y1);
			}
		}
	}
}

static void KeyboardCB(unsigned char Key,int x,int y)
{	
	if (manager.gameApp->player() != nullptr)
	{
		manager.gameApp->player()->TakeAction(Key, manager.gameApp->Bullets(), manager.gameApp->BarriageToUse());
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
	
	GameStateFunc* gameApp = new GameStateFunc();

	manager.gameApp = gameApp;
	manager.isChangedState = false;
	manager.gamestate = GameState::begin;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	Button* startButton = new Button("Content/GameStartButton.jpg", 0.5, -0.6, 0.3, 0.15);
	startButton->Init();
	startButton->SetFunc(ButtonFunctions::GameStartButton);
	Button* restartButton = new Button("Content/GameRestartButton.jpg", 0.65, -0.6, 0.3, 0.15);
	restartButton->Init();
	restartButton->SetFunc(ButtonFunctions::GameStartButton);
	Button* endButton = new Button("Content/GameEndButton.jpg", 0.3, -0.6, 0.3, 0.15);
	endButton->Init();
	endButton->SetFunc(ButtonFunctions::GameEndButton);

	manager.startButton = startButton;
	manager.restartButton = restartButton;
	manager.endButton = endButton;

	gameApp->manager = &manager;
	ButtonFunctions::manager = &manager;

	glutMainLoop();

	SAFE_DELETE(manager.gameApp);
	SAFE_DELETE(manager.startButton);
	SAFE_DELETE(manager.restartButton);
	SAFE_DELETE(manager.endButton);
	
	return 0;
}

