#include "platform_windows.h"

using namespace Pegas;

void BaseGameApplication::init(HWND hWnd)
{
	m_grafManager.initialize(hWnd);

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
	}/*else
	{
		Sleep(1);
		return false;
	}*/

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

void BaseGameApplication::processInput(UINT message, WPARAM wParam, LPARAM lParam)
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