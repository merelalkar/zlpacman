#include "stdafx.h"
#include "game_objects.h"
#include "game_events.h"

using namespace Pegas;

/*************************************************************************************************************
	Character
**************************************************************************************************************/
void Character::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_ChangeDirection::k_type)
	{
		Event_ChangeDirection* eventObject = evt->cast<Event_ChangeDirection>();
		if(eventObject->_actorId != m_actorId || m_isBlocked)
		{
			//событие предназначено не нам или управление персонажем заблокировано
			return;
		}

		if(eventObject->_newDirection == m_currentDirection)
		{
			//персонаж уже движеться в этом направлении
			return;
		}

		float dotProduct = m_directions[m_currentDirection].dotProduct(m_directions[eventObject->_newDirection]);
		if(dotProduct < 0)
		{
			//разворот в противоположном направлении можно сделать сразу
			m_currentDirection = eventObject->_newDirection;

			EventPtr newEvent(new Event_DirectionChanged(m_actorId, m_currentDirection));
			TheEventMgr.pushEventToQueye(newEvent);
		}else
		{
			m_turnCommand = eventObject->_newDirection;			
		}
	}

	if(evt->getType() == Event_CancelChangingDirection::k_type)
	{
		Event_CancelChangingDirection* eventObject = evt->cast<Event_CancelChangingDirection>();
		if(eventObject->_actorId != m_actorId)
		{
			//событие предназначено не нам
			return;
		}
		m_turnCommand = -1;
	}

	if(evt->getType() == Event_EnableCharacterControl::k_type)
	{
		Event_EnableCharacterControl* eventObject = evt->cast<Event_EnableCharacterControl>();
		if(eventObject->_actorId != m_actorId)
		{
			//событие предназначено не нам
			return;
		}

		m_isBlocked = false;
		m_isMoving = true;

		EventPtr newEvent(new Event_CharacterMoveOn(m_actorId));
		TheEventMgr.pushEventToQueye(newEvent);
	}

	if(evt->getType() == Event_DisableCharacterControl::k_type)
	{
		Event_DisableCharacterControl* eventObject = evt->cast<Event_DisableCharacterControl>();
		if(eventObject->_actorId != m_actorId)
		{
			//событие предназначено не нам
			return;
		}

		m_isBlocked = true;
		m_isMoving = false;

		EventPtr newEvent(new Event_CharacterStopped(m_actorId));
		TheEventMgr.pushEventToQueye(newEvent);
	}
}

void Character::update(float deltaTime)
{
	int32 currentRow, currentColumn;
	
	if(m_isMoving)
	{
		m_tileGrid->pointToCell((CURCOORD)m_position._x, (CURCOORD)m_position._y, currentRow, currentColumn);

		//перемещение по лабиринту
		Vector3 newPos = m_position + (m_directions[m_currentDirection] * m_velocity * deltaTime);
		Vector3 facingPoint = newPos + m_directions[m_currentDirection] * m_radius;
		if(m_tileGrid->isObstaclePoint((CURCOORD)facingPoint._x, (CURCOORD)facingPoint._y))
		{
			//натолкнулись на препятсвие
			//корркетируем позицию персонажа чтобы он был в середине текущей клетки
			CURCOORD left, top;

			m_tileGrid->cellCoords(currentRow, currentColumn, left, top);
			m_position._x = left + (m_tileGrid->getCellWidth() * 1.0 / 2);
			m_position._y = top + (m_tileGrid->getCellHeight() * 1.0f / 2);

			m_isMoving = false;

			EventPtr newEvent(new Event_CharacterStopped(m_actorId));
			TheEventMgr.pushEventToQueye(newEvent);
		}else
		{
			m_position = newPos;
		}
	}

	//поворот
	if(m_turnCommand != -1 && (!m_isBlocked))
	{
		int32 nextRow, nextColumn;
		
		m_tileGrid->pointToCell((CURCOORD)m_position._x, (CURCOORD)m_position._y, currentRow, currentColumn);

		switch(m_turnCommand)
		{
		case k_moveLeft:
			nextRow = currentRow;
			nextColumn = currentColumn - 1;
			break;
		case k_moveTop:
			nextRow = currentRow - 1;
			nextColumn = currentColumn;
			break;
		case k_moveRight:
			nextRow = currentRow;
			nextColumn = currentColumn + 1;
			break;
		case k_moveBottom:
			nextRow = currentRow + 1;
			nextColumn = currentColumn ;
			break;
		};

		if(!m_tileGrid->isObstacle(nextRow, nextColumn))
		{
			bool entirelyInCell = true;
			int32	 row, col;

			for(int i = 0; i < 4; i++)
			{
				Vector3 borderPoint = m_position + (m_directions[i] * m_radius);
				m_tileGrid->pointToCell((CURCOORD)borderPoint._x, (CURCOORD)borderPoint._y, row, col);

				if(row != currentRow || col != currentColumn)
				{
					entirelyInCell = false;
					break;
				}
			}

			if(entirelyInCell)
			{
				m_currentDirection = m_turnCommand;
				m_turnCommand = -1;

				EventPtr newEvent(new Event_DirectionChanged(m_actorId, m_currentDirection));
				TheEventMgr.pushEventToQueye(newEvent);

				if(!m_isMoving)
				{
					m_isMoving = true;

					EventPtr newEvent2(new Event_CharacterMoveOn(m_actorId));
					TheEventMgr.pushEventToQueye(newEvent2);
				}
			}
		}
	}//if(m_turnCommand != -1)
}

/******************************************************************************************************************************
	Pacman
*******************************************************************************************************************************/
void Pacman::handleEvent(EventPtr evt)
{
	
	Character::handleEvent(evt);
}

void Pacman::update(float deltaTime)
{
	Character::update(deltaTime);

	if(m_isMoving)
	{
		int32 row, column;
		m_tileGrid->pointToCell((CURCOORD)m_position._x, (CURCOORD)m_position._y, row, column);
		if(row != m_prevRow && column != m_prevColumn)
		{
			m_prevRow = row;
			m_prevColumn = column;

			int32 collisionGroup = 0;
			m_tileGrid->isObstacle(row, column, &collisionGroup);

			if(collisionGroup == k_collisionGroupPill 
				|| collisionGroup == k_collisionGroupSuperPill 
				|| collisionGroup == k_collisionGroupBonus)
			{
				EventPtr newEvent(new Event_PacmanSwallowedPill(collisionGroup));
				TheEventMgr.pushEventToQueye(newEvent);
			}
		}//if(row != m_prevRow && column != m_prevColumn)
	}//if(m_isMoving)
}