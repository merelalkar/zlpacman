#pragma once
#include "default_game_state.h"
#include "gui_layer.h"
#include "tile_grid.h"

namespace Pegas
{
	/********************************************************************************************************
		Editor
	*********************************************************************************************************/
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
			k_editorMode_GridCellSizing, //��������� ������� ������ ����� (C + UP/DOWN/LEFT/RIGHT)
			k_editorMode_GridCellsTweak, //��������� ���������� ����� �� ����������� � ��������� (CTRL + UP/DOWN/LEFT/RIGHT)
			k_editorMode_TilesPlacement//����������� ������ �� ����� (<Num> + �������� ����� � ������ �����)			
		};

		EditorMode m_currentEditorMode;
		int32	 m_debugDrawFlags;
		TILEID m_currentTile;

		CURCOORD m_gridPanStep;
		CURCOORD m_cellSizingStep;
		
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