#pragma once

enum GameState
{
	begin,
	update,
	winend,
	failend
};

class ButtonFunctions
{
public:
	static void GameStartButton();
	static void GameEndButton();
};