#include "stdafx.h"
#include "game_application.h"

namespace Pegas
{
	MainMenu::MainMenu()
		:DefaultGameState(k_stateMainMenu)
	{

	}

	GameApplication::GameApplication(HINSTANCE hInstance)
		:BaseGameApplication(hInstance)
	{

	}

	void GameApplication::init(HWND hWnd)
	{
		BaseGameApplication::init(hWnd);

		addGameState(GameStatePtr(new MainMenu()));
		changeState(k_stateMainMenu);
	}
}