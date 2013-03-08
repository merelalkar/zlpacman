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
		virtual void onKeyUp(KeyCode key, KeyFlags flags);
		virtual void render(IPlatformContext* context);

		//virtual void destroy(IPlatformContext* context);
		//virtual void handleEvent(EventPtr evt);
	private:
		//режимы редактора
		enum EditorMode
		{
			k_editorMode_None, //никаких действий 
			k_editorMode_GridPan, //перемещение сетки по экрану (SPACE + UP/DOWN/LEFT/RIGHT)
			k_editorMode_GridSizing, //изменение ширины и высоты сетки (S + UP/DOWN/LEFT/RIGHT)
			k_editorMode_GridCellsTweak, //изменение количества ячеек по горизонтали и вертикали (CTRL + UP/DOWN/LEFT/RIGHT)
			k_editorMode_PillsPlacement,//расстановка пилюль (еды) (Р (англ "Pills") + кликнуть мышью в ячейку сетки)
			k_editorMode_ObstaclePlacement //расстановка препятствий (O ("Obstacles") + кликнуть мышью в ячейку сетки)
		};

		EditorMode m_currentEditorMode;
		int32	 m_debugDrawFlags;

		CURCOORD m_gridPanStep;
		CURCOORD m_gridSizingStep;

		CURCOORD m_minGridWidth;
		CURCOORD m_maxGridWidth;
		CURCOORD m_minGridHeight;
		CURCOORD m_maxGridHeight;

		int32 m_minRows;
		int32 m_minColumns;
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