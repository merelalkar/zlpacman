#pragma once
#include "default_game_state.h"
#include "gui_layer.h"

namespace Pegas
{
	enum PacmanLayers
	{
		k_layerMainMenu = 1,
		k_layerOptions,
		k_layerPause,
		k_layerFader
	};

	enum PacmanGameStates
	{
		k_stateMainMenu = 1,
		k_stateGame,
		k_stateEditor		
	};

	class MainMenuLayer: public GUILayer, public IEventListener 
	{
	public:
		MainMenuLayer();

		virtual void create(IPlatformContext* context);
		virtual void destroy(IPlatformContext* context);
		virtual void onKeyDown(KeyCode key, KeyFlags flags);
		virtual void handleEvent(EventPtr evt);
		virtual void render(IPlatformContext* context);
	private:
		bool m_aboutToExit;
		IPlatformContext* m_platform;

		SpriteParameters m_logo;
		static const CURCOORD k_logoWidth;
		static const CURCOORD k_logoHeight;		
	};

	class MainMenu: public DefaultGameState
	{
	public:
		MainMenu();
		virtual void enter(IPlatformContext* context);
	};

	class Options: public GUILayer {};

	class GlobalPause: public GUILayer {};
}