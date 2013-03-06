#pragma once
#include "default_game_state.h"
#include "gui_layer.h"
#include "tile_grid.h"

namespace Pegas
{
	enum PacmanLayers
	{
		k_layerMainMenu = 1,
		k_layerOptions,
		k_layerPause,
		k_layerFader,
		k_layerEditor
	};

	enum PacmanGameStates
	{
		k_stateMainMenu = 1,
		k_stateGame,
		k_stateEditor		
	};

	/*******************************************************************************************************
		Main Menu
	********************************************************************************************************/
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

	/********************************************************************************************************
		Editor
	*********************************************************************************************************/
	class TileGrid;

	class EditorLayer: public GUILayer
	{
	public:
		EditorLayer(IPlatformContext* context);

		virtual void create(IPlatformContext* context);
		virtual void onKeyDown(KeyCode key, KeyFlags flags);
		virtual void render(IPlatformContext* context);

		//virtual void destroy(IPlatformContext* context);
		//virtual void handleEvent(EventPtr evt);
	private:
		IPlatformContext* m_context;	
		
		TileGrid m_tileGrid;
		SpriteParameters m_maze;

		CURCOORD m_workZone_fromX;
		CURCOORD m_workZone_fromY;
		CURCOORD m_workZone_toX;
		CURCOORD m_workZone_toY;

		static const CURCOORD k_mazeWidth;
		static const CURCOORD k_mazeHeight;
		static const CURCOORD k_topBarHeight;
		static const CURCOORD k_bottomBarHeight;
	};

	class Editor: public DefaultGameState
	{
	public:
		Editor();
		virtual void enter(IPlatformContext* context);
	};
}