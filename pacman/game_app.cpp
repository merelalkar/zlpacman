#include "stdafx.h"
#include "game_app.h"
#include "game_resources.h"

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
}

void GameApplication::run()
{
	if(!m_isActive)
		return;

	m_grafManager.render();
}

void GameApplication::cleanup()
{
	m_grafManager.destroy();
	m_stringManager.destroyAll();
	m_textureManager.destroyAll();
	m_fontManager.destroyAll();
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
	
	m_fontManager.registerResource(k_fontMain, MAKE_FONT_RESOURCE_CODE(_T("Verdana"), 12));

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