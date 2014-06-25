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

	class GameScreen: public DefaultGameState
	{
	public:
		GameScreen();
		virtual void enter(IPlatformContext* context);
	};

	class GameApplication: public BaseGameApplication
	{
	public:
		GameApplication(HINSTANCE hInstance);
		virtual void init(HWND hWnd);
	};
}