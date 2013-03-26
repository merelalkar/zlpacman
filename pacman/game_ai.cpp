#include "stdafx.h"
#include "game_ai.h"

#include "game_events.h"

using namespace Pegas;

/**********************************************************************************
	
***********************************************************************************/
BaseAIState::BaseAIState(TileGrid* tileGrid, int32 controlledActor, int32 stateId)
{
	m_tileGrid = tileGrid;
		
	m_controlledActor = controlledActor;
	m_stateId = stateId;

	m_myRow = -1;
	m_myColumn = -1;
	m_myCurrentDirection = -1;
	m_characterMoved = false;
}

void BaseAIState::start(ProcessHandle myHandle, ProcessManagerPtr owner)
{
	Process::start(myHandle, owner);

	TheEventMgr.addEventListener(this, Event_CharacterMoved::k_type);
	TheEventMgr.addEventListener(this, Event_DirectionChanged::k_type);
	TheEventMgr.addEventListener(this, Event_CharacterStateChanged::k_type);
	TheEventMgr.addEventListener(this, Event_Game_ChangeState::k_type);
	TheEventMgr.addEventListener(this, Event_Game_Pause::k_type);
	TheEventMgr.addEventListener(this, Event_Game_Resume::k_type);
}

void BaseAIState::terminate()
{
	TheEventMgr.removeEventListener(this);

	Process::terminate();
}

void BaseAIState::update(MILLISECONDS deltaTime)
{
	if(!m_characterMoved)
		return;


}

void BaseAIState::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_CharacterMoved::k_type)
	{
		Event_CharacterMoved* pEvent = evt->cast<Event_CharacterMoved>();
		if(pEvent->_actorId == m_controlledActor || pEvent->_actorId == k_actorAll)
		{
			m_myRow = pEvent->_row;
			m_myColumn = pEvent->_column;
		}
	}

	if(evt->getType() == Event_DirectionChanged::k_type)
	{
		Event_DirectionChanged* pEvent = evt->cast<Event_DirectionChanged*>();
		if(pEvent->_actorId == m_controlledActor || pEvent->_actorId == k_actorAll)
		{
			m_myCurrentDirection = pEvent->_newDirection;
		}
	}

	if(evt->getType() == Event_CharacterStateChanged::k_type)
	{
		Event_CharacterStateChanged* pEvent = evt->cast<Event_CharacterStateChanged>();
		if(pEvent->_actorId == m_controlledActor || pEvent->_actorId == k_actorAll)
		{
			if(pEvent->_newState == m_stateId)
			{
				resume();
			}else
			{
				suspend();
			}
		}
	}

	if(evt->getType() == Event_Game_ChangeState::k_type)
	{
		terminate();
	}

	if(evt->getType() == Event_Game_Pause::k_type)
	{
		suspend();
	}
}


