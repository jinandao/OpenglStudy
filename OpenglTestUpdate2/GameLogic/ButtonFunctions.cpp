#include "ButtonFunctions.h"
#include <stdlib.h>
#include <iostream>

extern bool isChangedState;
extern enum GameState gamestate;

void ButtonFunctions::GameStartButton()
{
	isChangedState = true;
	gamestate = GameState::update;
}

void ButtonFunctions::GameEndButton()
{
	std::cout << "testendbutton" << std::endl;
	exit(0);
}

