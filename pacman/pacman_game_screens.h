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

	class EditorLayer: public GUILayer, public IEventListener
	{
	public:
		EditorLayer(IPlatformContext* context);

		virtual void create(IPlatformContext* context);
		virtual void onKeyDown(KeyCode key, KeyFlags flags);
		virtual void onKeyUp(KeyCode key, KeyFlags flags);
		virtual void onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags);
		virtual void render(IPlatformContext* context);

		virtual void destroy(IPlatformContext* context);
		virtual void handleEvent(EventPtr evt);
	private:
		//������ ���������
		enum EditorMode
		{
			k_editorMode_None, //������� �������� 
			k_editorMode_GridPan, //����������� ����� �� ������ (SPACE + UP/DOWN/LEFT/RIGHT)
			k_editorMode_GridSizing, //��������� ������ � ������ ����� (S + UP/DOWN/LEFT/RIGHT)
			k_editorMode_GridCellSizing, //��������� ������� ������ ����� (C + UP/DOWN/LEFT/RIGHT)
			k_editorMode_GridCellsTweak, //��������� ���������� ����� �� ����������� � ��������� (CTRL + UP/DOWN/LEFT/RIGHT)
			k_editorMode_PillsPlacement,//����������� ������ (���) (1 + �������� ����� � ������ �����)
			k_editorMode_ObstaclePlacement //����������� ����������� (2 + �������� ����� � ������ �����)
		};

		EditorMode m_currentEditorMode;
		int32	 m_debugDrawFlags;

		TILEID m_pilleID;
		TILEID m_obstacleID;

		CURCOORD m_gridPanStep;
		CURCOORD m_gridSizingStep;

		float m_currentCellSize;
		float m_cellSizingStep;

		CURCOORD m_minGridWidth;
		CURCOORD m_maxGridWidth;
		CURCOORD m_minGridHeight;
		CURCOORD m_maxGridHeight;
				
		CURCOORD m_prevGridWidth;
		CURCOORD m_prevGridHeight;


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