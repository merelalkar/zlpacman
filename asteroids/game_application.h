#pragma once

namespace Pegas
{
	enum AsteroidsGameStates
	{
		k_stateMainMenu = 1,
		k_stateGame,
		k_stateEditor		
	};

	class MainMenu: public DefaultGameState
	{
	public:
		MainMenu();
	};

	class GameApplication: public BaseGameApplication
	{
	public:
		GameApplication(HINSTANCE hInstance);
		virtual void init(HWND hWnd);
	};
}