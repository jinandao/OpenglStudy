#pragma once

enum GameState
{
	begin,
	update,
	winend,
	failend
};

class GlobalVariablesManager;

class ButtonFunctions
{
public:
	static void GameStartButton();
	static void GameEndButton();

	static GlobalVariablesManager* manager;
};