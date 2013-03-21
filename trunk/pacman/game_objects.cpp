#include "stdafx.h"
#include "game_objects.h"
#include "game_events.h"

//Sprite animation class
#include "platform_windows.h"

using namespace Pegas;
/*************************************************************************************************************
	Character
**************************************************************************************************************/
Character::Character(int actorId)
{
	m_actorId = actorId;
	m_tileGrid = 0;
	m_radius = 1.0f;

	m_directions[k_moveLeft] = Vector3(-1.0f, 0.0f, 0.0f);
	m_directions[k_moveTop] = Vector3(0.0f, -1.0f, 0.0f);
	m_directions[k_moveRight] = Vector3(1.0f, 0.0f, 0.0f);
	m_directions[k_moveBottom] = Vector3(0.0f, 1.0f, 0.0f);

	m_currentDirection = k_moveRight;
	m_velocity = 1.0f;
	m_turnCommand = -1.0f;

	m_isBlocked = false;
	m_isMoving = false;	
}

void Character::create(TileGrid* tileGrid, const Vector3& position)
{
	assert(tileGrid && "null pointer: tileGrid");

	m_tileGrid = tileGrid;
	m_radius = tileGrid->getCellWidth() * 0.5;
	m_position = position;

	TheEventMgr.addEventListener(this, Event_ChangeDirection::k_type);
	TheEventMgr.addEventListener(this, Event_CancelChangingDirection::k_type);
	TheEventMgr.addEventListener(this, Event_EnableCharacterControl::k_type);
	TheEventMgr.addEventListener(this, Event_DisableCharacterControl::k_type);
}

void Character::destroy()
{
	TheEventMgr.removeEventListener(this);
}

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
Pacman::Pacman(int actorId, IPlatformContext* platform):
	Character(actorId)
{
	m_platform = platform;
	m_prevRow = 0;
	m_prevColumn = 0;
	m_currentAnimation = 0;
}

void Pacman::create(TileGrid* tileGrid, const Vector3& position)
{
	Character::create(tileGrid, position);

	TheEventMgr.addEventListener(this, Event_CharacterStopped::k_type);
	TheEventMgr.addEventListener(this, Event_CharacterMoveOn::k_type);
	TheEventMgr.addEventListener(this, Event_PacmanDeath::k_type);
}

void Pacman::setAnimation(int state, ProcessPtr animation)
{
	assert(state >= 0 && state <k_animationTotal && "invalid animation key");

	m_animations[state] = animation;
}

void Pacman::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_CharacterStopped::k_type)
	{
		Event_CharacterStopped* pEvent = evt->cast<Event_CharacterStopped>();
		if(pEvent->_actorId == m_actorId)
		{
			m_animations[k_animationRunning]->suspend();
		}
	}

	if(evt->getType() == Event_CharacterMoveOn::k_type)
	{
		Event_CharacterMoveOn* pEvent = evt->cast<Event_CharacterMoveOn>();
		if(pEvent->_actorId == m_actorId)
		{
			m_currentAnimation = k_animationRunning;
			m_animations[k_animationRunning]->resume();
		}
	}

	if(evt->getType() == Event_PacmanDeath::k_type)
	{
		m_currentAnimation = k_animationDeath;
		m_platform->attachProcess(m_animations[k_animationDeath]);
	}

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

			TILEID tile = m_tileGrid->getTile(row, column);

			if(tile == k_tilePill || tile == k_tileSuperPill || tile == k_tileBonus)
			{
				EventPtr newEvent(new Event_PacmanSwallowedPill(tile, row, column));
				TheEventMgr.pushEventToQueye(newEvent);
			}
		}//if(row != m_prevRow && column != m_prevColumn)
	}//if(m_isMoving)
}

void Pacman::draw()
{
	if(m_currentAnimation < 0 || m_currentAnimation >= k_animationTotal)
		return;

	if(m_animations[m_currentAnimation]->getStatus() == k_processStatusKilled)
		return;

	SpriteAnimation* animation = (SpriteAnimation*)m_animations[m_currentAnimation].get();
	SpriteParameters* sprite =  animation->getSprite();

	sprite->_left = (CURCOORD)(m_position._x - (sprite->_width * 0.5f));
	sprite->_top =	(CURCOORD)(m_position._y - (sprite->_height * 0.5f));
		
	switch(m_currentDirection)
	{
	case k_moveLeft:
		sprite->_angle = 0.0f;
		break;
	case k_moveTop:
		sprite->_angle = -90.0f;
		break;
	case k_moveRight:
		sprite->_angle = 180.0f;
		break;
	case k_moveBottom:
		sprite->_angle = 90.0f;
		break;
	};

	GrafManager::getInstance().drawSprite(*sprite);
}