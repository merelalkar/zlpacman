#include "stdafx.h"
#include "pacman_game_screens.h"

#include "game_resources.h"
#include "game_events.h"
#include "standart_widgets.h"
#include "fader_layer.h"


using namespace Pegas;

class ChangeStateTask: public Process
{
public:
	ChangeStateTask(IPlatformContext* platform, GameStateID	newStateId)
	{
		m_platform = platform;
		m_newStateId = newStateId;
	}

	virtual void start(ProcessHandle myHandle, ProcessManagerPtr owner)
	{
		Process::start(myHandle, owner);

		m_platform->changeState(m_newStateId);
	}

	virtual void update(MILLISECONDS deltaTime) {}

private:
	IPlatformContext* m_platform;
	GameStateID	m_newStateId;
};

/*****************************************************************************************
	Main Menu 
******************************************************************************************/

const CURCOORD MainMenuLayer::k_logoWidth = 470;
const CURCOORD MainMenuLayer::k_logoHeight = 112;

enum MenuButtons
{
	k_menuButtonStart = 1,
	k_menuButtonOptions,
	k_menuButtonExit	
};

MainMenuLayer::MainMenuLayer():
	GUILayer(k_layerMainMenu, false)
{
	m_aboutToExit = false;
}


void MainMenuLayer::create(IPlatformContext* context)
{
	GUILayer::create(context);

	m_platform = context;

	TheEventMgr.addEventListener(this, Event_GUI_FadeinComplete::k_type);
	TheEventMgr.addEventListener(this, Event_GUI_ButtonClick::k_type);

	CURCOORD canvasWidth = GrafManager::getInstance().getCanvasWidth();
	CURCOORD canvasHeight = GrafManager::getInstance().getCanvasHeight();

	CURCOORD logoScaledWidth = canvasWidth - 10;
	CURCOORD logoScaledHeight = (CURCOORD)((k_logoHeight * logoScaledWidth * 1.0) / k_logoWidth); 

	m_logo._texture = k_texturePacmanLogo;
	m_logo._left = 5;
	m_logo._top = 5;
	m_logo._width = logoScaledWidth;
	m_logo._height = logoScaledHeight;

	CURCOORD buttonWidth, buttonHeight;
	GrafManager::getInstance().getTextExtent(_text("options"), k_fontMenuButton, buttonWidth, buttonHeight);
	buttonWidth+= 8;
	buttonHeight+= 8;

	float left = (canvasWidth - buttonWidth) * 0.5;
	float top = (canvasHeight - logoScaledHeight - (buttonHeight * 3) - 50) * 0.5;
	RGBCOLOR borderColor = 0;
	RGBCOLOR textColor = 0xffffffff;
	RGBCOLOR activeTextColor = 0xffffff33;

	ButtonWidget* startButton = new ButtonWidget(k_menuButtonStart);
	startButton->setPosition(left, top);
	startButton->setSize(buttonWidth, buttonHeight);
	startButton->setButtonStyle(k_buttonStateNormal, k_fontMenuButton, textColor, borderColor);
	startButton->setButtonStyle(k_buttonStatePressed, k_fontMenuButton, textColor, borderColor);
	startButton->setButtonStyle(k_buttonStateActive, k_fontMenuButton, activeTextColor, borderColor);
	startButton->setCaption(_text("start"));

	addWidget(WidgetPtr(startButton));

	top+= (buttonHeight + 20);

	ButtonWidget* optionsButton = new ButtonWidget(k_menuButtonOptions);
	optionsButton->setPosition(left, top);
	optionsButton->setSize(buttonWidth, buttonHeight);
	optionsButton->setButtonStyle(k_buttonStateNormal, k_fontMenuButton, textColor, borderColor);
	optionsButton->setButtonStyle(k_buttonStatePressed, k_fontMenuButton, textColor, borderColor);
	optionsButton->setButtonStyle(k_buttonStateActive, k_fontMenuButton, activeTextColor, borderColor);
	optionsButton->setCaption(_text("options"));

	addWidget(WidgetPtr(optionsButton));

	top+= (buttonHeight + 20);

	ButtonWidget* exitButton = new ButtonWidget(k_menuButtonExit);
	exitButton->setPosition(left, top);
	exitButton->setSize(buttonWidth, buttonHeight);
	exitButton->setButtonStyle(k_buttonStateNormal, k_fontMenuButton, textColor, borderColor);
	exitButton->setButtonStyle(k_buttonStatePressed, k_fontMenuButton, textColor, borderColor);
	exitButton->setButtonStyle(k_buttonStateActive, k_fontMenuButton, activeTextColor, borderColor);
	exitButton->setCaption(_text("exit"));

	addWidget(WidgetPtr(exitButton));	
}

void MainMenuLayer::destroy(IPlatformContext* context)
{
	GUILayer::destroy(context);

	TheEventMgr.removeEventListener(this);
}

void MainMenuLayer::onKeyDown(KeyCode key, KeyFlags flags)
{
	GUILayer::onKeyDown(key, flags);

	if(key == IKeyboardController::k_keyCodeTAB || key == IKeyboardController::k_keyCodeDOWN)
	{
		changeFocusNext();

	}else if(key == IKeyboardController::k_keyCodeUP)
	{
		changeFocusPrev();
	}
}

void MainMenuLayer::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_GUI_ButtonClick::k_type)
	{
		Event_GUI_ButtonClick* pEvent = evt->cast<Event_GUI_ButtonClick>();
		if(pEvent->m_button->getID() == k_menuButtonStart || pEvent->m_button->getID() == k_menuButtonExit)
		{
			EventPtr startFadein(new Event_GUI_StartFadein());
			TheEventMgr.pushEventToQueye(startFadein);

			m_aboutToExit = (pEvent->m_button->getID() == k_menuButtonExit);
		}
	}

	if(evt->getType() == Event_GUI_FadeinComplete::k_type)
	{
		if(m_aboutToExit)
		{
			m_platform->shutdownGame();
		}else
		{
			m_platform->changeState(k_stateEditor);			
		}
	}
}

void MainMenuLayer::render(IPlatformContext* context)
{
	GUILayer::render(context);

	GrafManager::getInstance().drawSprite(m_logo);
}

/*******************************************************************/
MainMenu::MainMenu():
	DefaultGameState(k_stateMainMenu)
{
	
}

void MainMenu::enter(IPlatformContext* context)
{
	DefaultGameState::enter(context);

	pushLayer(BaseScreenLayerPtr(new MainMenuLayer()));
	pushLayer(BaseScreenLayerPtr(new FaderLayer(k_layerFader)));

	EventPtr evt(new Event_GUI_StartFadeout(2));
	TheEventMgr.triggerEvent(evt);
}

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

	m_prevGridWidth = m_tileGrid.getWidth();
	m_prevGridHeight = m_tileGrid.getHeight();

	m_currentEditorMode = k_editorMode_None;
	m_gridPanStep = 1;
	m_gridSizingStep = 1;

	m_currentCellSize = m_tileGrid.getCellWidth();
	m_cellSizingStep = 0.05;
	m_tileGrid.setCellSize((CURCOORD)m_currentCellSize, (CURCOORD)m_currentCellSize);

	m_minGridWidth = m_maze._width;
	m_maxGridWidth = canvasWidth;
	m_minGridHeight = m_maze._height;
	m_maxGridHeight = canvasHeight;

	m_minRows = m_minColumns = 5;

	m_pilleID = m_tileGrid.addTileDesc(TileDesc(k_texturePillTile, false));
	m_obstacleID = m_tileGrid.addTileDesc(TileDesc(0, true));

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

	if(key == IKeyboardController::k_keyCode_1)
	{
		m_currentEditorMode = k_editorMode_PillsPlacement;
	}

	if(key == IKeyboardController::k_keyCode_2)
	{
		m_currentEditorMode = k_editorMode_ObstaclePlacement;
	}

	if(key == IKeyboardController::k_keyCode_S)
	{
		m_currentEditorMode = k_editorMode_GridSizing;
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

		if(m_currentEditorMode == k_editorMode_GridSizing)
		{
			CURCOORD left, top, width, height;
			m_tileGrid.getArea(left, top, width, height);

			if(key == IKeyboardController::k_keyCodeUP)
			{
				m_prevGridHeight-= m_gridSizingStep;

				if(m_prevGridHeight < m_minGridHeight) 
					m_prevGridHeight = m_minGridHeight;
			}
			if(key == IKeyboardController::k_keyCodeDOWN)
			{
				m_prevGridHeight+= m_gridSizingStep;

				/*if(m_prevGridHeight > m_maxGridHeight) 
					m_prevGridHeight = m_maxGridHeight;*/
			}
			if(key == IKeyboardController::k_keyCodeLEFT)
			{
				m_prevGridWidth-= m_gridSizingStep;

				if(m_prevGridWidth < m_minGridWidth) 
					m_prevGridWidth = m_minGridWidth;
			}
			if(key == IKeyboardController::k_keyCodeRIGHT)
			{
				m_prevGridWidth+= m_gridSizingStep;

				/*if(m_prevGridWidth > m_maxGridWidth) 
					m_prevGridWidth = m_maxGridWidth;*/
			}			

			m_tileGrid.setArea(left, top, m_prevGridWidth, m_prevGridHeight);
		}

		if(m_currentEditorMode == k_editorMode_GridCellSizing)
		{
			if(key == IKeyboardController::k_keyCodeUP)
			{
				m_currentCellSize+= m_cellSizingStep;				
			}
			if(key == IKeyboardController::k_keyCodeDOWN)
			{
				m_currentCellSize-= m_cellSizingStep;
				if(m_currentCellSize < 5) m_currentCellSize = 5;				
			}
			
			m_tileGrid.setCellSize((CURCOORD)m_currentCellSize, (CURCOORD)m_currentCellSize);
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
}

void EditorLayer::onKeyUp(KeyCode key, KeyFlags flags)
{
	GUILayer::onKeyUp(key, flags);

	if(key == IKeyboardController::k_keyCodeSPACE
		|| key == IKeyboardController::k_keyCodeCTRL
		|| key == IKeyboardController::k_keyCode_1
		|| key == IKeyboardController::k_keyCode_2
		|| key == IKeyboardController::k_keyCode_S
		|| key == IKeyboardController::k_keyCode_C)
	{
		m_currentEditorMode = k_editorMode_None;
	}
}

void EditorLayer::onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags)
{
	GUILayer::onMouseButtonDown(button, x, y, flags);

	if(button == k_mouseButtonLeft && m_currentEditorMode == k_editorMode_PillsPlacement)
	{
		TILEID currentID = m_tileGrid.getTilePoint((CURCOORD)x, (CURCOORD)y);
		currentID = currentID != m_pilleID ? m_pilleID : TileGrid::k_emptyCellTileId;
		m_tileGrid.setTilePoint((CURCOORD)x, (CURCOORD)y, currentID);

		int32 row, col;
		m_tileGrid.pointToCell((CURCOORD)x, (CURCOORD)y, row, col);
		OSUtils::getInstance().debugOutput("tile clicked [row = %d, column = %d]", row, col);
	}

	if(button == k_mouseButtonLeft && m_currentEditorMode == k_editorMode_ObstaclePlacement)
	{
		TILEID currentID = m_tileGrid.getTilePoint((CURCOORD)x, (CURCOORD)y);
		currentID = currentID != m_obstacleID ? m_obstacleID : TileGrid::k_emptyCellTileId;
		m_tileGrid.setTilePoint((CURCOORD)x, (CURCOORD)y, currentID);

		int32 row, col;
		m_tileGrid.pointToCell((CURCOORD)x, (CURCOORD)y, row, col);
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