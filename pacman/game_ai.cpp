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
	
	int32 backwardDirection = m_myCurrentDirection + 2;
	if(backwardDirection >= Character::k_moveTotalDirections)
	{
		backwardDirection -= Character::k_moveTotalDirections;
	}


	int32 choosenDirection = -1;
	float minHeuristic = MAX_REAL32;
	for(int32 i = 0; i < Character::k_moveTotalDirections; i++)
	{
		if(i == backwardDirection) continue;

		int32 row = m_myRow + rowOffset[i];
		int32 column = m_myColumn + columnOffset[i];
		
		if(isObstacle(row, column)) continue;

		float heuristic = getGoalHeuristic(row, column);
		if(heuristic < minHeuristic)
		{
			minHeuristic = heuristic;
			choosenDirection = i;
		}
	}

	return choosenDirection;
}

bool BaseAIState::isObstacle(int32 row, int32 column)
{
	return m_tileGrid->isObstacle(row, column);
}

void BaseAIState::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_CharacterMoved::k_type)
	{
		Event_CharacterMoved* pEvent = evt->cast<Event_CharacterMoved>();
		if(pEvent->_actorId == m_controlledActor || pEvent->_actorId == k_actorAll)
		{
			if(m_myRow != pEvent->_row || m_myColumn != pEvent->_column)
			{
				m_myRow = pEvent->_row;
				m_myColumn = pEvent->_column;
				m_characterMoved = true;
			}
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
	if(m_blockMutex > 0)
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

	BaseAIState::handleEvent(evt);
}

float BlinkyChaseState::getGoalHeuristic(int32 row, int32 column)
{
	Vector3 position, distance;
	m_tileGrid->cellCoords(row, column, position._x, position._y, true);
	distance = m_pacmanPosition - position;

	return distance.length();
}

/**********************************************************************************************************
	Pinky Chase State
***********************************************************************************************************/
PinkyChaseState::PinkyChaseState(TileGrid* tileGrid)
	:BaseAIState(tileGrid, k_actorPinky, Ghost::k_stateChasing)
{

}

void PinkyChaseState::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_CharacterMoved::k_type)
	{
		Event_CharacterMoved* pEvent = evt->cast<Event_CharacterMoved>();
		if(pEvent->_actorId == k_actorPacman)
		{
			m_pacmanRow = pEvent->_row;
			m_pacmanColumn = pEvent->_column;			
		}

		calculateGoalPosition();
	}

	if(evt->getType() == Event_DirectionChanged::k_type)
	{
		Event_DirectionChanged* pEvent = evt->cast<Event_DirectionChanged>();
		if(pEvent->_actorId == k_actorPacman)
		{
			m_pacmanDirection = pEvent->_newDirection;

			calculateGoalPosition();
		}
	}

	BaseAIState::handleEvent(evt);
}

float PinkyChaseState::getGoalHeuristic(int32 row, int32 column)
{
	Vector3 position, distance;
	m_tileGrid->cellCoords(row, column, position._x, position._y, true);
	distance = m_goalPosition - position;

	return distance.length();
}

void PinkyChaseState::calculateGoalPosition()
{
	assert(m_pacmanDirection >= 0 && m_pacmanDirection < Character::k_moveTotalDirections && "invalid param: m_pacmanDirection");

	int32 rowOffset[] = { 0, -1, 0, 1};
	int32 columnOffset[] = { -1, 0, 1, 0 };
	int32 row = m_pacmanRow + (rowOffset[m_pacmanDirection] * 4);
	int32 column = m_pacmanColumn + (columnOffset[m_pacmanDirection] * 4);

	m_tileGrid->cellCoords(row, column, m_goalPosition._x, m_goalPosition._y, true);
}

/*************************************************************************************************
	
**************************************************************************************************/
InkyChaseState::InkyChaseState(TileGrid* tileGrid)
	:BaseAIState(tileGrid, k_actorInky, Ghost::k_stateChasing)
{

}

void InkyChaseState::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_CharacterMoved::k_type)
	{
		Event_CharacterMoved* pEvent = evt->cast<Event_CharacterMoved>();
		if(pEvent->_actorId == k_actorPacman)
		{
			m_pacmanRow = pEvent->_row;
			m_pacmanColumn = pEvent->_column;			
		}

		if(pEvent->_actorId == k_actorBlinky)
		{
			m_blinkyPosition = pEvent->_position;			
		}

		calculateGoalPosition();
	}

	if(evt->getType() == Event_DirectionChanged::k_type)
	{
		Event_DirectionChanged* pEvent = evt->cast<Event_DirectionChanged>();
		if(pEvent->_actorId == k_actorPacman)
		{
			m_pacmanDirection = pEvent->_newDirection;

			calculateGoalPosition();
		}
	}

	BaseAIState::handleEvent(evt);
}

float InkyChaseState::getGoalHeuristic(int32 row, int32 column)
{
	Vector3 position, distance;
	m_tileGrid->cellCoords(row, column, position._x, position._y, true);
	distance = m_goalPosition - position;

	return distance.length();
}

void InkyChaseState::calculateGoalPosition()
{
	assert(m_pacmanDirection >= 0 && m_pacmanDirection < Character::k_moveTotalDirections && "invalid param: m_pacmanDirection");

	Vector3 lineCenter, line;

	int32 rowOffset[] = { 0, -1, 0, 1};
	int32 columnOffset[] = { -1, 0, 1, 0 };
	int32 row = m_pacmanRow + (rowOffset[m_pacmanDirection] * 2);
	int32 column = m_pacmanColumn + (columnOffset[m_pacmanDirection] * 2);

	m_tileGrid->cellCoords(row, column, lineCenter._x, lineCenter._y, true);
	line = lineCenter - m_blinkyPosition;
	m_goalPosition = m_blinkyPosition + (line * 2);
}

/*******************************************************************************************************
	
*******************************************************************************************************/
ClydeChaseState::ClydeChaseState(TileGrid* tileGrid, const Vector3 scatterPoint)
	:BaseAIState(tileGrid, k_actorClyde, Ghost::k_stateChasing)
{
	m_scatterPoint = scatterPoint;
	m_goalPosition = scatterPoint;
}

void ClydeChaseState::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_CharacterMoved::k_type)
	{
		Event_CharacterMoved* pEvent = evt->cast<Event_CharacterMoved>();
		if(pEvent->_actorId == k_actorPacman)
		{
			m_pacmanRow = pEvent->_row;
			m_pacmanColumn = pEvent->_column;
		}

		calculateGoalPosition();
	}

	BaseAIState::handleEvent(evt);
}

float ClydeChaseState::getGoalHeuristic(int32 row, int32 column)
{
	Vector3 position, distance;
	m_tileGrid->cellCoords(row, column, position._x, position._y, true);
	distance = m_goalPosition - position;

	return distance.length();
}
	
void ClydeChaseState::calculateGoalPosition()
{
	int32 dx = m_pacmanColumn - m_myColumn;
	int32 dy = m_pacmanRow - m_myRow;
	int32 distance = dx + dy;

	if(distance > 8)
	{
		m_tileGrid->cellCoords(m_pacmanRow, m_pacmanColumn, m_goalPosition._x, m_goalPosition._y); 
	}else
	{
		m_goalPosition = m_scatterPoint;
	}
}

/********************************************************************************************************

*********************************************************************************************************/
GoalDrivenState::GoalDrivenState(TileGrid* tileGrid, int32 controlledActor, int32 stateId):
	BaseAIState(tileGrid, controlledActor, stateId)
{

}

void GoalDrivenState::setGoalPoint(const Vector3& point)
{
	m_tileGrid->pointToCell(point._x, point._y, m_goalRow, m_goalColumn);  
}

void GoalDrivenState::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_CharacterMoved::k_type)
	{
		Event_CharacterMoved* pEvent = evt->cast<Event_CharacterMoved>();
		if(pEvent->_actorId == m_controlledActor 
			&& pEvent->_column == m_goalColumn
			&& pEvent->_row == m_goalRow)
		{
			suspend();

			onGoalAchieved();
		}
	}

	BaseAIState::handleEvent(evt);
}	

float GoalDrivenState::getGoalHeuristic(int32 row, int32 column)
{
	Vector3 position, goalPosition, distance;
	m_tileGrid->cellCoords(m_goalRow, m_goalColumn, goalPosition._x, goalPosition._y, true);
	m_tileGrid->cellCoords(row, column, position._x, position._y, true);
	distance = goalPosition - position;

	return distance.length();
}

void GoalDrivenState::onGoalAchieved()
{

}

/************************************************************************
	Runaway State
************************************************************************/
RunawayState::RunawayState(TileGrid* tileGrid, int32 controlledActor)
	:BaseAIState(tileGrid, controlledActor, Ghost::k_stateRunaway)
{

}

void RunawayState::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_CharacterMoved::k_type)
	{
		Event_CharacterMoved* pEvent = evt->cast<Event_CharacterMoved>();
		if(pEvent->_actorId == k_actorPacman)
		{
			m_pacmanPosition = pEvent->_position;
		}		
	}

	BaseAIState::handleEvent(evt);
}

float RunawayState::getGoalHeuristic(int32 row, int32 column)
{
	Vector3 position, distance;
	m_tileGrid->cellCoords(row, column, position._x, position._y, true);
	distance = m_pacmanPosition - position;

	return distance.length();
}

int32 RunawayState::chooseDirection()
{
	int32 rowOffset[] = { 0, -1, 0, 1};
	int32 columnOffset[] = { -1, 0, 1, 0 };
	
	int32 backwardDirection = m_myCurrentDirection + 2;
	if(backwardDirection >= Character::k_moveTotalDirections)
	{
		backwardDirection -= Character::k_moveTotalDirections;
	}

	int32 choosenDirection = -1;
	std::vector<int32> directions;	
	for(int32 i = 0; i < Character::k_moveTotalDirections; i++)
	{
		if(i == backwardDirection) continue;

		int32 row = m_myRow + rowOffset[i];
		int32 column = m_myColumn + columnOffset[i];
		
		if(isObstacle(row, column)) continue;

		directions.push_back(i);
	}

	if(directions.size() > 0)
	{
		std::random_shuffle(directions.begin(), directions.end());
		choosenDirection = directions.front();
	}

	return choosenDirection;
}

/**************************************************************************************************
**************************************************************************************************/
PrayState::PrayState(TileGrid* tileGrid, int32 controlledActor)
	:GoalDrivenState(tileGrid, controlledActor, Ghost::k_statePray)
{
	std::list<Vector3> nodes;
	tileGrid->getTiles(k_tilePreyGoalNode, nodes, true);
	assert(nodes.size() > 0 && "PrayState goal node not found");
	
	setGoalPoint(nodes.front());
}
	
void PrayState::onGoalAchieved()
{
	EventPtr evt = new Event_CharacterChangeState(m_controlledActor, Ghost::k_stateChasing);
	TheEventMgr.pushEventToQueye(evt);
}

bool PrayState::isObstacle(int32 row, int32 column)
{
	int32 collisionGroup;
	if(m_tileGrid->isObstacle(row, column, &collisionGroup))
	{
		return collisionGroup == k_collisionGroupDefault;
	}

	return false;
}
	



