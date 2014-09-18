#include "platform_android.h"
#include "base_game_application.h"

using namespace Pegas;

void BaseGameApplication::init()
{
	m_grafManager.initialize();

	m_lastTime = m_utils.getCurrentTime();

	TheEventMgr.addEventListener(this, Event_Game_ChangeState::k_type);
	TheEventMgr.addEventListener(this, Event_Game_ForwardToState::k_type);
	TheEventMgr.addEventListener(this, Event_Game_BackwardToPreviousState::k_type);
}

bool BaseGameApplication::run()
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

	if(deltaTime > 33) deltaTime = 33;

	m_eventManager.processEvents(0);
	m_processManager.updateProcesses(deltaTime);

	if(!m_statesStack.empty())
	{
		m_statesStack.top()->update(this, deltaTime, -1);
		m_statesStack.top()->render(this);
	}

	return m_exitApplication;
}

void BaseGameApplication::cleanup()
{
	TheEventMgr.removeEventListener(this);

	m_openedFiles.clear();

	m_grafManager.destroy();
	m_stringManager.destroyAll();
	m_textureManager.destroyAll();
	m_fontManager.destroyAll();
	m_soundManager.destroyAll();
}

void BaseGameApplication::resize(int width, int height)
{
	m_grafManager.setViewport(0, 0, width, height);
}

void BaseGameApplication::processInput()
{

}

/***********************************************************************************************************************
	Platform Context implementation
***********************************************************************************************************************/
void BaseGameApplication::addKeyboardController(IKeyboardController* controller)
{
	if(m_keyboardControllers.count(controller) == 0)
		m_keyboardControllers.insert(controller);
}

void BaseGameApplication::removeKeyboardController(IKeyboardController* controller)
{
	m_keyboardControllers.erase(controller);
}

void BaseGameApplication::addMouseController(IMouseController* controller)
{
	if(m_mouseControllers.count(controller) == 0)
		m_mouseControllers.insert(controller);
}

void BaseGameApplication::removeMouseController(IMouseController* controller)
{
	m_mouseControllers.erase(controller);
}

ProcessHandle BaseGameApplication::attachProcess(ProcessPtr process)
{
	return m_processManager.attachProcess(process);
}

void BaseGameApplication::terminateProcess(const ProcessHandle& handle)
{
	m_processManager.terminateProcess(handle);
}

void BaseGameApplication::addGameState(GameStatePtr state)
{
	if(m_statesMap.count(state->getID()) == 0)
	{
		//m_statesMap.insert(std::make_pair(state->getID(), state));
		m_statesMap[state->getID()] = state;
	}
}

void BaseGameApplication::removeState(GameStateID id)
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

void BaseGameApplication::changeState(GameStateID newStateId)
{
	EventPtr evt(new Event_Game_ChangeState(newStateId));
	TheEventMgr.pushEventToQueye(evt);
}

void BaseGameApplication::forwardToState(GameStateID newStateId)
{
	EventPtr evt(new Event_Game_ForwardToState(newStateId));
	TheEventMgr.pushEventToQueye(evt);
}

void BaseGameApplication::backwardToPreviousState()
{
	EventPtr evt(new Event_Game_BackwardToPreviousState());
	TheEventMgr.pushEventToQueye(evt);
}

void BaseGameApplication::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_Game_ChangeState::k_type)
	{
		Event_Game_ChangeState* pEvent = evt->cast<Event_Game_ChangeState>();
		_changeState(pEvent->_id);
	}

	if(evt->getType() == Event_Game_ForwardToState::k_type)
	{
		Event_Game_ForwardToState* pEvent = evt->cast<Event_Game_ForwardToState>();
		_forwardToState(pEvent->_id);
	}

	if(evt->getType() == Event_Game_BackwardToPreviousState::k_type)
	{
		_backwardToPreviousState();
	}
}

/************************************************************************************************
	actual changing state
**************************************************************************************************/
void BaseGameApplication::_changeState(GameStateID newStateId)
{
	if(m_statesMap.count(newStateId) == 0)
	{
		m_exitApplication = true;
		return;
	}

	if(!m_statesStack.empty())
	{
		if(m_statesStack.top()->getID() == newStateId)
		{
			return;
		}

		m_statesStack.top()->leave(this);
		m_statesStack.pop();
	}

	m_statesStack.push(m_statesMap[newStateId]);
	m_statesStack.top()->enter(this);
}

void BaseGameApplication::_forwardToState(GameStateID newStateId)
{
	if(m_statesMap.count(newStateId) == 0)
	{
		m_exitApplication = true;
		return;
	}

	if(!m_statesStack.empty())
	{
		if(m_statesStack.top()->getID() == newStateId)
		{
			return;
		}
		m_statesStack.top()->leave(this);
	}
	m_statesStack.push(m_statesMap[newStateId]);
	m_statesStack.top()->enter(this);
}

void BaseGameApplication::_backwardToPreviousState()
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

void BaseGameApplication::shutdownGame()
{
	m_exitApplication = true;
}

ISerializer* BaseGameApplication::createFile(const String& fileName)
{
	String path = fileName;
	if(path.find(_text(".map")) != String::npos)
	{
		path = _text("./maps/") + fileName;
	}

	FilePtr result = new File();
	if(result->create(path))
	{
		m_openedFiles.push_back(result);
		return result.get();
	}

	return 0;
}

ISerializer* BaseGameApplication::openFile(const String& fileName, uint32 mode)
{
	String path = fileName;
	if(path.find(_text(".map")) != String::npos)
	{
		path = _text("./maps/") + fileName;
	}

	FilePtr result = new File();
	if(result->open(path, mode))
	{
		m_openedFiles.push_back(result);
		return result.get();
	}

	return 0;
}

void BaseGameApplication::closeFile(ISerializer* file)
{
	for(std::list<FilePtr>::iterator it = m_openedFiles.begin(); it != m_openedFiles.end(); ++it)
	{
		if((*it) == file)
		{
			m_openedFiles.erase(it);
			break;
		}
	}
}


