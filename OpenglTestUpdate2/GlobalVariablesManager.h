#pragma once
#include "GameLogic/ButtonFunctions.h"
#include "GameLayer/UIElement.h"

class GameStateFunc;

class GlobalVariablesManager
{
public:
	enum GameState gamestate;
	bool isChangedState;
	GameStateFunc* gameApp;
	Button* startButton;
	Button* restartButton;
	Button* endButton;
};