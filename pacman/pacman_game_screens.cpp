#include "stdafx.h"
#include "pacman_game_screens.h"

#include "game_resources.h"
#include "game_events.h"
#include "standart_widgets.h"
#include "fader_layer.h"


using namespace Pegas;

/*****************************************************************************************
	Main Menu 
******************************************************************************************/

const CURCOORD MainMenuLayer::k_logoWidth = 470;
const CURCOORD MainMenuLayer::k_logoHeight = 112;

const CURCOORD k_mazeWidth = 470;
const CURCOORD k_mazeHeight = 519;

const CURCOORD k_topBarHeight = 15;
const CURCOORD k_bottomBarHeight = 10;

const CURCOORD k_sideMargin = 5;

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

