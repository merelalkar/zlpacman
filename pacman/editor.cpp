#include "stdafx.h"
#include "editor.h"

#include "pacman_game_screens.h"
#include "game_resources.h"
#include "game_events.h"
#include "game_objects.h"
#include "standart_widgets.h"
#include "fader_layer.h"

using namespace Pegas;

/*****************************************************************************************************************
	Editor
*****************************************************************************************************************/
const CURCOORD EditorLayer::k_mazeWidth = 470;
const CURCOORD EditorLayer::k_mazeHeight = 519;

const CURCOORD EditorLayer::k_topBarHeight = 15;
const CURCOORD EditorLayer::k_bottomBarHeight = 10;

enum EditorButtons
{
	k_editorButtonSave = 1,
	k_editorButtonLoad
};

EditorLayer::EditorLayer(IPlatformContext* context): GUILayer(k_layerEditor, false)
{
	m_context = context;	
}

void EditorLayer::create(IPlatformContext* context)
{
	GUILayer::create(context);

	TheEventMgr.addEventListener(this, Event_GUI_ButtonClick::k_type);

	CURCOORD canvasWidth = GrafManager::getInstance().getCanvasWidth();
	CURCOORD canvasHeight = GrafManager::getInstance().getCanvasHeight();

	//maze sprite setup
	CURCOORD scaledMazeWidth, scaledMazeHeight;
	scaledMazeWidth = canvasWidth - 10;
	scaledMazeHeight = (CURCOORD)((k_mazeHeight * scaledMazeWidth * 1.0) / k_mazeWidth);

	m_maze._texture = k_textureMaze;
	m_maze._top = k_topBarHeight;
	m_maze._left = 5;
	m_maze._width = scaledMazeWidth;
	m_maze._height = scaledMazeHeight;

	//work zone line
	m_workZone_fromX = 0;
	m_workZone_fromY = scaledMazeHeight + k_topBarHeight + k_bottomBarHeight;
	m_workZone_toX = canvasWidth;
	m_workZone_toY = m_workZone_fromY;

	//tile grid setup
	m_debugDrawFlags = TileGrid::k_debugDrawGrid | TileGrid::k_debugDrawObstacles;

	m_tileGrid.create(30, 33);
	m_tileGrid.setArea(m_maze._left, m_maze._top, m_maze._width, m_maze._height);

	m_tileGrid.addTileDesc(TileDesc(k_texturePillTile, false));
	m_tileGrid.addTileDesc(TileDesc(0, true, k_collisionGroupDefault));
	m_tileGrid.addTileDesc(TileDesc(k_textureSuperPillTile, false));

	m_tileGrid.addTileDesc(TileDesc(k_texturePacmanEditorTile, false));
	m_tileGrid.addTileDesc(TileDesc(0, true, k_collisionGroupDoor));
	m_tileGrid.addTileDesc(TileDesc(k_textureTunnelEditorTile, false));

	m_tileGrid.addTileDesc(TileDesc(k_textureBlinkyEditorTile, false));
	m_tileGrid.addTileDesc(TileDesc(k_texturePinkyEditorTile, false));
	m_tileGrid.addTileDesc(TileDesc(k_textureInkyEditorTile, false));
	m_tileGrid.addTileDesc(TileDesc(k_textureClydeEditorTile, false));
	
	m_currentEditorMode = k_editorMode_None;
	m_gridPanStep = 0.2;
	m_cellSizingStep = 0.05;
	m_minRows = m_minColumns = 5;	

	//editor buttons;
	float buttonLeft = 5;
	float buttonTop = m_workZone_fromY + 5;
	float buttonWidth = 100;
	float buttonHeight = 30;

	ButtonWidget* saveButton = new ButtonWidget(k_editorButtonSave);
	saveButton->setPosition(buttonLeft, buttonTop);
	saveButton->setSize(buttonWidth, buttonHeight);
	saveButton->setCaption(_text("save"));

	addWidget(WidgetPtr(saveButton));

	buttonTop+= buttonHeight + 10;

	ButtonWidget* loadButton = new ButtonWidget(k_editorButtonLoad);
	loadButton->setPosition(buttonLeft, buttonTop);
	loadButton->setSize(buttonWidth, buttonHeight);
	loadButton->setCaption(_text("load"));

	addWidget(WidgetPtr(loadButton));
}

void EditorLayer::destroy(IPlatformContext* context)
{
	GUILayer::destroy(context);

	TheEventMgr.removeEventListener(this);
}

void EditorLayer::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_GUI_ButtonClick::k_type)
	{
		Event_GUI_ButtonClick* pEvent = evt->cast<Event_GUI_ButtonClick>();
		
		if(pEvent->m_button->getID() == k_editorButtonSave)
		{
			int32 canvasWidth = (int32)GrafManager::getInstance().getCanvasWidth();
			int32 canvasHeight = (int32)GrafManager::getInstance().getCanvasHeight();

			tchar fileName[256];
#ifdef		_UNICODE
			wsprintf(fileName, _text("maze_%d_%d.map"), canvasWidth, canvasHeight);  
#else
			sprintf(fileName, _text("maze_%d_%d.map"), canvasWidth, canvasHeight);
#endif
			
			ISerializer* fileStream = m_context->createFile(fileName);
			if(fileStream)
			{
				OSUtils::getInstance().debugOutput("saving...");

				m_tileGrid.save(*fileStream);
				m_context->closeFile(fileStream);

				OSUtils::getInstance().debugOutput("saving complete");
			}else
			{
				OSUtils::getInstance().debugOutput("can not create file");
			}
		}

		if(pEvent->m_button->getID() == k_editorButtonLoad)
		{
			int32 canvasWidth = (int32)GrafManager::getInstance().getCanvasWidth();
			int32 canvasHeight = (int32)GrafManager::getInstance().getCanvasHeight();

			tchar fileName[256];
#ifdef		_UNICODE
			wsprintf(fileName, _text("maze_%d_%d.map"), canvasWidth, canvasHeight);  
#else
			sprintf(fileName, _text("maze_%d_%d.map"), canvasWidth, canvasHeight);
#endif
			ISerializer* fileStream = m_context->openFile(fileName, FileOpenMode::k_readOnly);
			if(fileStream)
			{
				OSUtils::getInstance().debugOutput("loading...");

				m_tileGrid.load(*fileStream);
				m_context->closeFile(fileStream);

				OSUtils::getInstance().debugOutput("loading complete");
			}else
			{
				OSUtils::getInstance().debugOutput("can not open file");
			}
		}
	}
}

void EditorLayer::onKeyDown(KeyCode key, KeyFlags flags)
{
	GUILayer::onKeyDown(key, flags);

	if(key == IKeyboardController::k_keyCodeESCAPE)
	{
		ProcessPtr process(new Fadein());
		ProcessPtr process2(new ChangeStateTask(m_context, k_stateMainMenu));

		process->attachNext(process2);
		m_context->attachProcess(process);		
	}

	if(key == IKeyboardController::k_keyCodeSPACE)
	{
		m_currentEditorMode = k_editorMode_GridPan;
	}

	if(key == IKeyboardController::k_keyCodeCTRL)
	{
		m_currentEditorMode = k_editorMode_GridCellsTweak;
	}

	if(key == IKeyboardController::k_keyCode_C)
	{
		m_currentEditorMode = k_editorMode_GridCellSizing;
	}

	if(key == IKeyboardController::k_keyCodeUP
		|| key == IKeyboardController::k_keyCodeDOWN
		|| key == IKeyboardController::k_keyCodeLEFT
		|| key == IKeyboardController::k_keyCodeRIGHT)
	{
		if(m_currentEditorMode == k_editorMode_GridPan)
		{
			CURCOORD left, top, width, height;
			m_tileGrid.getArea(left, top, width, height);

			if(key == IKeyboardController::k_keyCodeUP)
			{
				top-= m_gridPanStep;
			}
			if(key == IKeyboardController::k_keyCodeDOWN)
			{
				top+= m_gridPanStep;
			}
			if(key == IKeyboardController::k_keyCodeLEFT)
			{
				left-= m_gridPanStep;
			}
			if(key == IKeyboardController::k_keyCodeRIGHT)
			{
				left+= m_gridPanStep;
			}			

			m_tileGrid.setArea(left, top, width, height);
		}

		if(m_currentEditorMode == k_editorMode_GridCellSizing)
		{
			CURCOORD currentCellSize = m_tileGrid.getCellWidth();

			if(key == IKeyboardController::k_keyCodeUP)
			{
				currentCellSize+= m_cellSizingStep;				
			}
			if(key == IKeyboardController::k_keyCodeDOWN)
			{
				currentCellSize-= m_cellSizingStep;
				if(currentCellSize < 5) currentCellSize = 5;				
			}
			
			m_tileGrid.setCellSize(currentCellSize, currentCellSize);
		}

		if(m_currentEditorMode == k_editorMode_GridCellsTweak)
		{
			int32 numRows = m_tileGrid.getNumRows();
			int32 numColumns = m_tileGrid.getNumColumns();

			if(key == IKeyboardController::k_keyCodeUP)
			{
				numRows++;
			}
			if(key == IKeyboardController::k_keyCodeDOWN)
			{
				numRows--;

				if(numRows < m_minRows) 
					numRows = m_minRows;
			}
			if(key == IKeyboardController::k_keyCodeLEFT)
			{
				numColumns--;

				if(numColumns < m_minColumns) 
					numColumns = m_minColumns;
			}
			if(key == IKeyboardController::k_keyCodeRIGHT)
			{
				numColumns++;				
			}

			m_tileGrid.create(numRows, numColumns);
		}
	}

	if(key == IKeyboardController::k_keyCode_Q && !(flags & k_keyFlagRepeat))
	{
		if(m_debugDrawFlags && TileGrid::k_debugDrawGrid)
			m_debugDrawFlags &= (~TileGrid::k_debugDrawGrid);
		else
			m_debugDrawFlags |= TileGrid::k_debugDrawGrid;
	}

	if(key == IKeyboardController::k_keyCode_A && !(flags & k_keyFlagRepeat))
	{
		if(m_debugDrawFlags && TileGrid::k_debugDrawObstacles)
			m_debugDrawFlags &= (~TileGrid::k_debugDrawObstacles);
		else
			m_debugDrawFlags |= TileGrid::k_debugDrawObstacles;
	}

	if(key == IKeyboardController::k_keyCode_1)
	{
		m_currentEditorMode = k_editorMode_TilesPlacement;
		m_currentTile = k_tilePill;
	}

	if(key == IKeyboardController::k_keyCode_2)
	{
		m_currentEditorMode = k_editorMode_TilesPlacement;
		m_currentTile = k_tileWall;
	}

	if(key == IKeyboardController::k_keyCode_3)
	{
		m_currentEditorMode = k_editorMode_TilesPlacement;
		m_currentTile = k_tileSuperPill;
	}

	if(key == IKeyboardController::k_keyCode_4)
	{
		m_currentEditorMode = k_editorMode_TilesPlacement;
		m_currentTile = k_tilePacman;
	}

	if(key == IKeyboardController::k_keyCode_5)
	{
		m_currentEditorMode = k_editorMode_TilesPlacement;
		m_currentTile = k_tileDoor;
	}

	if(key == IKeyboardController::k_keyCode_6)
	{
		m_currentEditorMode = k_editorMode_TilesPlacement;
		m_currentTile = k_tileTunnel;
	}

	if(key == IKeyboardController::k_keyCode_7)
	{
		m_currentEditorMode = k_editorMode_TilesPlacement;
		m_currentTile = k_tileBlinky;
	}

	if(key == IKeyboardController::k_keyCode_8)
	{
		m_currentEditorMode = k_editorMode_TilesPlacement;
		m_currentTile = k_tilePinky;
	}

	if(key == IKeyboardController::k_keyCode_9)
	{
		m_currentEditorMode = k_editorMode_TilesPlacement;
		m_currentTile = k_tileInky;
	}

	if(key == IKeyboardController::k_keyCode_0)
	{
		m_currentEditorMode = k_editorMode_TilesPlacement;
		m_currentTile = k_tileClyde;
	}	
}

void EditorLayer::onKeyUp(KeyCode key, KeyFlags flags)
{
	GUILayer::onKeyUp(key, flags);

	if(key != IKeyboardController::k_keyCodeUP
		&& key != IKeyboardController::k_keyCodeDOWN
		&& key != IKeyboardController::k_keyCodeLEFT
		&& key != IKeyboardController::k_keyCodeRIGHT)
	{
		m_currentEditorMode = k_editorMode_None;
	}
}

void EditorLayer::onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags)
{
	GUILayer::onMouseButtonDown(button, x, y, flags);

	if(button == k_mouseButtonLeft && m_currentEditorMode == k_editorMode_TilesPlacement)
	{
		TILEID prevID = m_tileGrid.getTilePoint(x, y);
		prevID = prevID != m_currentTile ? m_currentTile : TileGrid::k_emptyCellTileId;
		m_tileGrid.setTilePoint(x, y, prevID);

		int32 row, col;
		m_tileGrid.pointToCell(x, y, row, col);
		OSUtils::getInstance().debugOutput("tile clicked [row = %d, column = %d]", row, col);
	}	
}

void EditorLayer::render(IPlatformContext* context)
{
	GUILayer::render(context);

	GrafManager::getInstance().drawSprite(m_maze);
	GrafManager::getInstance().drawLine(m_workZone_fromX, m_workZone_fromY, m_workZone_toX, m_workZone_toY, 0xff0000ff);

	m_tileGrid.draw();
	m_tileGrid.debugDraw(m_debugDrawFlags);
}


/*******************************************************************/
Editor::Editor():
	DefaultGameState(k_stateEditor)
{
	
}

void Editor::enter(IPlatformContext* context)
{
	DefaultGameState::enter(context);

	pushLayer(BaseScreenLayerPtr(new EditorLayer(context)));
	pushLayer(BaseScreenLayerPtr(new FaderLayer(k_layerFader)));

	EventPtr evt(new Event_GUI_StartFadeout(1));
	TheEventMgr.triggerEvent(evt);
}