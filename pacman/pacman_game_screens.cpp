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

EditorLayer::EditorLayer(IPlatformContext* context): GUILayer(k_layerEditor, false)
{
	m_context = context;	
}

void EditorLayer::create(IPlatformContext* context)
{
	GUILayer::create(context);

	CURCOORD canvasWidth = GrafManager::getInstance().getCanvasWidth();
	CURCOORD canvasHeight = GrafManager::getInstance().getCanvasHeight();

	CURCOORD scaledMazeWidth, scaledMazeHeight;
	scaledMazeWidth = canvasWidth - 10;
	scaledMazeHeight = (CURCOORD)((k_mazeHeight * scaledMazeWidth * 1.0) / k_mazeWidth);

	m_maze._texture = k_textureMaze;
	m_maze._top = k_topBarHeight;
	m_maze._left = 5;
	m_maze._width = scaledMazeWidth;
	m_maze._height = scaledMazeHeight;

	m_workZone_fromX = 0;
	m_workZone_fromY = scaledMazeHeight + k_topBarHeight + k_bottomBarHeight;
	m_workZone_toX = canvasWidth;
	m_workZone_toY = m_workZone_fromY;
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
}

void EditorLayer::render(IPlatformContext* context)
{
	GUILayer::render(context);

	GrafManager::getInstance().drawSprite(m_maze);
	GrafManager::getInstance().drawLine(m_workZone_fromX, m_workZone_fromY, m_workZone_toX, m_workZone_toY, 0xff0000ff);
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