#include "stdafx.h"
#include "game_ai.h"

#include "game_events.h"
#include "game_objects.h"

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
	m_blockMutex = 0;
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

	m_characterMoved = false;

	
	int32 choosenDirection = chooseDirection();
	if(choosenDirection != -1 && choosenDirection != m_myCurrentDirection)
	{
		EventPtr evt(new Event_ChangeDirection(m_controlledActor, choosenDirection));
		TheEventMgr.pushEventToQueye(evt);
	}
}

int32 BaseAIState::chooseDirection()
{
	int32 rowOffset[] = { 0, -1, 0, 1};
	int32 columnOffset[] = { -1, 0, 1, 0 };
	int32 backwardDirection = (m_myCurrentDirection + 2) % Character::k_moveTotalDirections;

	int32 choosenDirection = -1;
	float minHeuristic = MAX_REAL32;
	for(int32 i = 0; i < Character::k_moveTotalDirections; i++)
	{
		if(i == backwardDirection) continue;

		int32 row = m_myRow + rowOffset[i];
		int32 column = m_myColumn + columnOffset[i];
		
		if(m_tileGrid->isObstacle(row, column)) continue;

		float heuristic = getGoalHeuristic(row, column);
		if(heuristic < minHeuristic)
		{
			minHeuristic = heuristic;
			choosenDirection = i;
		}
	}

	return choosenDirection;
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
			m_characterMoved = true;
		}
	}

	if(evt->getType() == Event_DirectionChanged::k_type)
	{
		Event_DirectionChanged* pEvent = evt->cast<Event_DirectionChanged>();
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
			}

			if(pEvent->_newState != m_stateId && getStatus() == k_processStatusRunning)
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

	if(evt->getType() == Event_Game_Resume::k_type)
	{
		resume();
	}
}

void BaseAIState::suspend()
{
	Process::suspend();
	m_blockMutex++;
}

void BaseAIState::resume()
{
	m_blockMutex--;
	if(m_blockMutex == 0)
	{
		Process::resume();
	}
}

/************************************************************************
	BlinkyChaseState
************************************************************************/
BlinkyChaseState::BlinkyChaseState(TileGrid* tileGrid)
	:BaseAIState(tileGrid, k_actorBlinky, Ghost::k_stateChasing)
{

}

void BlinkyChaseState::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_CharacterMoved::k_type)
	{
		Event_CharacterMoved* pEvent = evt->cast<Event_CharacterMoved>();
		if(pEvent->_actorId == k_actorPacman)
		{
			m_pacmanPosition = pEvent->_position;
		}
	}

	if(evt->getType() == Event_DirectionChanged::k_type)
	{
		Event_DirectionChanged* pEvent = evt->cast<Event_DirectionChanged>();
		if(pEvent->_actorId == k_actorPacman)
		{
			m_pacmanCurrentDirection = pEvent->_newDirection;
		}
	}

	BaseAIState::handleEvent(evt);
}

float BlinkyChaseState::getGoalHeuristic(int32 row, int32 column)
{
	Vector3 position, distance;
	m_tileGrid->cellCoords(row, column, position._x, position._y, true);
	distance = m_pacmanPosition - position;

	return distance.length();
}



