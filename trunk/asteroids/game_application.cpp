#include "stdafx.h"
#include "game_application.h"

#include "game_manager.h"

namespace Pegas
{
	/******************************************************************************************************************
		MainMenu
	*******************************************************************************************************************/
	MainMenu::MainMenu()
		:DefaultGameState(k_stateMainMenu)
	{

	}

	/************************************************************************************************************************
		GameScreen
	**************************************************************************************************************************/
	GameScreen::GameScreen()
		:DefaultGameState(k_stateGame)
	{

	}

	void GameScreen::enter(IPlatformContext* context)
	{
		DefaultGameState::enter(context);

		pushLayer(BaseScreenLayerPtr(new GameManager()));
	}

	/************************************************************************************************************************
		GameApplication
	*************************************************************************************************************************/
	GameApplication::GameApplication(HINSTANCE hInstance)
		:BaseGameApplication(hInstance)
	{

	}

	void GameApplication::init(HWND hWnd)
	{
		BaseGameApplication::init(hWnd);

		addGameState(GameStatePtr(new MainMenu()));
		addGameState(GameStatePtr(new GameScreen()));
		changeState(k_stateGame);
	}
}