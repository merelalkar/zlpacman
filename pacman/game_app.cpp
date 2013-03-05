#include "stdafx.h"
#include "game_app.h"
#include "game_resources.h"
#include "pacman_game_screens.h"

#include "resource.h"

using namespace Pegas;

const RESOURCEID k_textureCardBack = 1;
const RESOURCEID k_textureCardKing = 2;
const RESOURCEID k_textureFelt = 3;

const RESOURCEID k_stringYouWinThePrize = 1;
const RESOURCEID k_stringScores = 2;
const RESOURCEID k_stringTurns = 3;
const RESOURCEID k_stringGameWindowTitle = 4;

const RESOURCEID k_fontBig = 1;
const RESOURCEID k_fontSmall = 2;

const RESOURCEID k_soundPassNewSeries = 1;
const RESOURCEID k_soundReleaseCard = 2;
const RESOURCEID k_soundShowValidTurn = 3;
const RESOURCEID k_soundNoValidTurn = 4;
const RESOURCEID k_soundTakeCard = 5;
const RESOURCEID k_soundYouWin = 6;


void GameApplication::init(HWND hWnd)
{
	registerResources();
	m_grafManager.initialize(hWnd);

	m_lastTime = m_utils.getCurrentTime();

	addGameState(GameStatePtr(new MainMenu()));
	changeState(k_stateMainMenu);
}

bool GameApplication::run()
{
	if(!m_isActive || m_exitApplication)
	{
		return false;
	}

	MILLISECONDS deltaTime = m_utils.getCurrentTime() - m_lastTime;
	if(deltaTime > 0)
	{
		m_lastTime = m_utils.getCurrentTime();
	}

	m_eventManager.processEvents();
	m_processManager.updateProcesses(deltaTime);

	if(!m_statesStack.empty())
	{
		m_statesStack.top()->update(this, deltaTime, 0);
		m_statesStack.top()->render(this);
	}

	return m_exitApplication;
}

void GameApplication::cleanup()
{
	m_grafManager.destroy();
	m_stringManager.destroyAll();
	m_textureManager.destroyAll();
	m_fontManager.destroyAll();
	m_soundManager.destroyAll();
}

void GameApplication::resize(int width, int height)
{
	m_grafManager.setViewport(0, 0, width, height);
}

void GameApplication::registerResources()
{
	m_textureManager.registerResource(k_textureMaze, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_MAZE));
	m_textureManager.registerResource(k_texturePillTile, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_PILL_TILE));
	m_textureManager.registerResource(k_textureSuperPillTile, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_SUPER_PILL_TILE));
	m_textureManager.registerResource(k_texturePacmanLogo, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_PACMAN_LOGO));
		
	m_fontManager.registerResource(k_fontMain, MAKE_FONT_RESOURCE_CODE(_T("Verdana"), 12));
	m_fontManager.registerResource(k_fontMenuButton, MAKE_FONT_RESOURCE_CODE(_T("Snap ITC"), 36));

	/*m_soundManager.registerResource(k_soundPassNewSeries, MAKE_INT_RESOURCE_CODE(m_hInstance, IDR_WAVE1));
	m_soundManager.registerResource(k_soundReleaseCard, MAKE_INT_RESOURCE_CODE(m_hInstance, IDR_WAVE2));
	m_soundManager.registerResource(k_soundShowValidTurn, MAKE_INT_RESOURCE_CODE(m_hInstance, IDR_WAVE3));
	m_soundManager.registerResource(k_soundNoValidTurn, MAKE_INT_RESOURCE_CODE(m_hInstance, IDR_WAVE4));
	m_soundManager.registerResource(k_soundTakeCard, MAKE_INT_RESOURCE_CODE(m_hInstance, IDR_WAVE5));
	m_soundManager.registerResource(k_soundYouWin, MAKE_INT_RESOURCE_CODE(m_hInstance, IDR_WAVE6));
	m_soundManager.loadAll();*/

	/*
	m_stringManager.registerResource(k_stringYouWinThePrize, MAKE_INT_RESOURCE_CODE(m_hInstance, IDS_YOU_WIN_THE_PRIZE));
	m_stringManager.registerResource(k_stringScores, MAKE_INT_RESOURCE_CODE(m_hInstance, IDS_SCORES));
	m_stringManager.registerResource(k_stringTurns, MAKE_INT_RESOURCE_CODE(m_hInstance, IDS_TURNS));
	m_stringManager.registerResource(k_stringGameWindowTitle, MAKE_INT_RESOURCE_CODE(m_hInstance, IDS_APP_TITLE));
	m_stringManager.loadAll();*/
}

void GameApplication::processInput(UINT message, WPARAM wParam, LPARAM lParam)
{
	BYTE kbstate[256];
	GetKeyboardState(kbstate);

	int32 flags = 0;
	if(kbstate[VK_SHIFT] & 0x80) flags |= k_keyFlagShift;
	if(kbstate[VK_CONTROL] & 0x80) flags |=	k_keyFlagCtrl;
	if(kbstate[VK_MENU] & 0x80) flags |= k_keyFlagAlt;
	if(kbstate[VK_CAPITAL] & 0x1) flags |= k_keyFlagCapsLock;
	if(kbstate[VK_SCROLL] & 0x1) flags |= k_keyFlagScrollLock;
	if(kbstate[VK_NUMLOCK] & 0x1) flags |= k_keyFlagNumLock;

	switch(message)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
		
		if(message == WM_KEYDOWN)
		{
			flags |= (lParam & 0x40000000) ? k_keyFlagRepeat : 0;
		}	

		for(std::set<IKeyboardController*>::iterator it = m_keyboardControllers.begin(); it != m_keyboardControllers.end(); ++it)
		{
			if(message == WM_KEYDOWN)
			{
				(*it)->onKeyDown((KeyCode)wParam, flags);
			}
			if(message == WM_KEYUP)
			{
				(*it)->onKeyUp((KeyCode)wParam, flags);
			}

			if(message == WM_CHAR)
			{
				(*it)->onChar((tchar)wParam);
			}
		}
		break;
			
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:

		MouseButton button = 0;
		if(message == WM_RBUTTONDOWN || message == WM_RBUTTONUP || message == WM_RBUTTONDBLCLK) button = k_mouseButtonRight;
		if(message == WM_LBUTTONDOWN || message == WM_LBUTTONUP || message == WM_LBUTTONDBLCLK) button = k_mouseButtonLeft;
		if(message == WM_MBUTTONDOWN || message == WM_MBUTTONUP  || message == WM_MBUTTONDBLCLK) button = k_mouseButtonMiddle;
		if(message == WM_LBUTTONDBLCLK || message == WM_RBUTTONDBLCLK || message == WM_MBUTTONDBLCLK) flags |= k_keyFlagRepeat;

		float x = LOWORD(lParam);
		float y = HIWORD(lParam);

		for(std::set<IMouseController*>::iterator it = m_mouseControllers.begin(); it != m_mouseControllers.end(); ++it)
		{
			switch(message)
			{
				case WM_RBUTTONDOWN:
				case WM_LBUTTONDOWN:
				case WM_MBUTTONDOWN:
				case WM_LBUTTONDBLCLK:
				case WM_RBUTTONDBLCLK:
				case WM_MBUTTONDBLCLK:
					(*it)->onMouseButtonDown(button, x, y, flags);
					break;
				case WM_RBUTTONUP:
				case WM_LBUTTONUP:
				case WM_MBUTTONUP:
					(*it)->onMouseButtonUp(button, x, y, flags);
					break;
				case WM_MOUSEMOVE:
					(*it)->onMouseMove(x, y, flags);
					break;
				case WM_MOUSEWHEEL:
					(*it)->onMouseWheel(HIWORD(wParam) / 120, flags);
					break;
			}
		}//for(std::set<IMouseController*>::iterator it !=	m_mouseControllers.begin(); it != m_mouseControllers.end(); ++it)
		break;
	}//switch(message)
}

/***********************************************************************************************************************
	Platform Context implementation
***********************************************************************************************************************/
/*
	std::set<IKeyboardController*> m_keyboardControllers;
		std::set<IMouseController*>	m_mouseControllers;

		std::map<GameStateID, GameStatePtr> m_stateList;
		std::stack<GameStatePtr> m_statesStack;
*/
void GameApplication::addKeyboardController(IKeyboardController* controller)
{
	if(m_keyboardControllers.count(controller) == 0)
		m_keyboardControllers.insert(controller);
}

void GameApplication::removeKeyboardController(IKeyboardController* controller)
{
	m_keyboardControllers.erase(controller);
}

void GameApplication::addMouseController(IMouseController* controller)
{
	if(m_mouseControllers.count(controller) == 0)
		m_mouseControllers.insert(controller);
}

void GameApplication::removeMouseController(IMouseController* controller)
{
	m_mouseControllers.erase(controller);
}
		
ProcessHandle GameApplication::attachProcess(ProcessPtr process)
{
	return m_processManager.attachProcess(process);
}

void GameApplication::addGameState(GameStatePtr state)
{
	if(m_statesMap.count(state->getID()) == 0)
	{
		m_statesMap.insert(std::make_pair(state->getID(), state));		
	}
}

void GameApplication::removeState(GameStateID id)
{
	if(m_statesMap.count(id) != 0)
	{
		std::stack<GameStatePtr> temp;
		while(!m_statesStack.empty())
		{
			if(m_statesStack.top()->getID() != id)
				temp.push(m_statesStack.top());
			m_statesStack.pop();
		}
		m_statesStack = temp;
		m_statesMap.erase(id);				
	}
}

void GameApplication::changeState(GameStateID newStateId)
{
	if(m_statesMap.count(newStateId) == 0)
	{
		m_exitApplication = true;
		return;
	}

	if(!m_statesStack.empty())
	{
		m_statesStack.top()->leave(this);
		m_statesStack.pop();
	}

	m_statesStack.push(m_statesMap[newStateId]);
	m_statesStack.top()->enter(this);
}

void GameApplication::forwardToState(GameStateID newStateId)
{
	if(m_statesMap.count(newStateId) == 0)
	{
		m_exitApplication = true;
		return;
	}

	if(!m_statesStack.empty())
	{
		m_statesStack.top()->leave(this);				
	}
	m_statesStack.push(m_statesMap[newStateId]);
	m_statesStack.top()->enter(this);
}

void GameApplication::backwardToPreviousState()
{
	if(m_statesStack.size() < 2)
	{
		m_exitApplication = true;
		return;
	}

	m_statesStack.top()->leave(this);
	m_statesStack.pop();
	m_statesStack.top()->enter(this);
}

void GameApplication::shutdownGame()
{
	m_exitApplication = true;
}