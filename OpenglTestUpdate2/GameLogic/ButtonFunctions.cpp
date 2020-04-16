#include "ButtonFunctions.h"
#include <stdlib.h>
#include <iostream>
#include "../GlobalVariablesManager.h"

GlobalVariablesManager* ButtonFunctions::manager = nullptr;

void ButtonFunctions::GameStartButton()
{
	if (manager != nullptr)
	{
		manager->isChangedState = true;
		manager->gamestate = GameState::update;
	}
}

void ButtonFunctions::GameEndButton()
{
	exit(0);
}

