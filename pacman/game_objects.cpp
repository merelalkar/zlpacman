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

	m_currentDirection = k_moveLeft;
	m_velocity = 100.0f;
	m_turnCommand = -1;

	m_blockMutex = 0;
	m_isMoving = false;
	m_isVisible = true;
}

void Character::create(TileGrid* tileGrid, const Vector3& position)
{
	assert(tileGrid && "null pointer: tileGrid");

	m_tileGrid = tileGrid;
	m_radius = tileGrid->getCellWidth() * 0.5f;
	m_initialPosition = m_position = position;

	TheEventMgr.addEventListener(this, Event_ChangeDirection::k_type);
	TheEventMgr.addEventListener(this, Event_CancelChangingDirection::k_type);
	TheEventMgr.addEventListener(this, Event_EnableCharacterControl::k_type);
	TheEventMgr.addEventListener(this, Event_DisableCharacterControl::k_type);
	TheEventMgr.addEventListener(this, Event_ResetActors::k_type);
	TheEventMgr.addEventListener(this, Event_HideCharacter::k_type);
	TheEventMgr.addEventListener(this, Event_ShowCharacter::k_type);
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
		if(eventObject->_actorId != m_actorId || isBlocked())
		{
			//������� ������������� �� ��� ��� ���������� ���������� �������������
			return;
		}

		if(eventObject->_newDirection == m_currentDirection)
		{
			//�������� ��� ��������� � ���� �����������
			return;
		}

		float dotProduct = m_directions[m_currentDirection].dotProduct(m_directions[eventObject->_newDirection]);
		if(dotProduct < 0)
		{
			//�������� � ��������������� ����������� ����� ������� �����
			m_currentDirection = eventObject->_newDirection;

			EventPtr newEvent(new Event_DirectionChanged(m_actorId, m_currentDirection));
			TheEventMgr.pushEventToQueye(newEvent);

			if(!m_isMoving)
			{
				m_isMoving = true;

				EventPtr newEvent2(new Event_CharacterMoveOn(m_actorId));
				TheEventMgr.pushEventToQueye(newEvent2);
			}
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
			//������� ������������� �� ���
			return;
		}
		m_turnCommand = -1;
	}

	if(evt->getType() == Event_EnableCharacterControl::k_type)
	{
		Event_EnableCharacterControl* eventObject = evt->cast<Event_EnableCharacterControl>();
		if(eventObject->_actorId != m_actorId && eventObject->_actorId != k_actorAll)
		{
			//������� ������������� �� ���
			return;
		}

		decrementBlock();
		
		if(!isBlocked())
		{
			m_isMoving = true;
			
			EventPtr newEvent(new Event_CharacterMoveOn(m_actorId));
			TheEventMgr.pushEventToQueye(newEvent);
		}
	}

	if(evt->getType() == Event_DisableCharacterControl::k_type)
	{
		Event_DisableCharacterControl* eventObject = evt->cast<Event_DisableCharacterControl>();
		if(eventObject->_actorId != m_actorId && eventObject->_actorId != k_actorAll)
		{
			//������� ������������� �� ���
			return;
		}

		incrementBlock();

		if(isBlocked())
		{
			m_isMoving = false;

			EventPtr newEvent(new Event_CharacterStopped(m_actorId));
			TheEventMgr.pushEventToQueye(newEvent);
		}
	}

	if(evt->getType() == Event_ResetActors::k_type)
	{
		m_position = m_initialPosition;
		m_currentDirection = k_moveLeft;
		m_isVisible = true;
	}

	if(evt->getType() == Event_HideCharacter::k_type)
	{
		Event_HideCharacter* pEvent = evt->cast<Event_HideCharacter>();
		if(pEvent->_actorId == m_actorId || pEvent->_actorId == k_actorAll)
		{
			m_isVisible = false;
		}
	}

	if(evt->getType() == Event_ShowCharacter::k_type)
	{
		Event_ShowCharacter* pEvent = evt->cast<Event_ShowCharacter>();
		if(pEvent->_actorId == m_actorId || pEvent->_actorId == k_actorAll)
		{
			m_isVisible = true;
		}
	}
}

void Character::update(float deltaTime)
{
	int32 currentRow, currentColumn;
	
	if(m_isMoving)
	{
		m_tileGrid->pointToCell(m_position._x, m_position._y, currentRow, currentColumn);

		//����������� �� ���������
		Vector3 newPos = m_position + (m_directions[m_currentDirection] * m_velocity * deltaTime);
		Vector3 facingPoint = newPos + m_directions[m_currentDirection] * m_radius;
		if(m_tileGrid->isObstaclePoint((CURCOORD)facingPoint._x, (CURCOORD)facingPoint._y))
		{
			//������������ �� ����������
			//������������ ������� ��������� ����� �� ��� � �������� ������� ������
			m_tileGrid->cellCoords(currentRow, currentColumn, m_position._x, m_position._y, true);
			m_isMoving = false;

			EventPtr newEvent(new Event_CharacterStopped(m_actorId));
			TheEventMgr.pushEventToQueye(newEvent);
		}else
		{
			m_position = newPos;
		}

		EventPtr evt(new Event_CharacterMoved(m_actorId, m_position, currentRow, currentColumn));
		TheEventMgr.pushEventToQueye(evt);
	}

	//�������
	if(m_turnCommand != -1 && !isBlocked())
	{
		int32 nextRow, nextColumn;
		
		m_tileGrid->pointToCell(m_position._x, m_position._y, currentRow, currentColumn);

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
			nextColumn = currentColumn;
			break;
		};

		if(!m_tileGrid->isObstacle(nextRow, nextColumn))
		{
			Vector3 cellPosition;
			m_tileGrid->cellCoords(currentRow, currentColumn, cellPosition._x, cellPosition._y, true);
			
			Vector3 distanceVec = cellPosition - m_position;
			float distance = distanceVec.length();
			float epsilon = distance / m_radius; 

			if(epsilon < 0.75f)
			{
				m_position = cellPosition;
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
	
	if(state == k_animationRunning)
	{
		m_platform->attachProcess(animation);
		animation->suspend();
	}
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

	if(evt->getType() == Event_ResetActors::k_type)
	{
		m_currentAnimation = k_animationRunning;		
	}

	Character::handleEvent(evt);
}

void Pacman::update(float deltaTime)
{
	Character::update(deltaTime);

	if(m_isMoving)
	{
		int32 row, column;
		m_tileGrid->pointToCell(m_position._x, m_position._y, row, column);
		if(row != m_prevRow || column != m_prevColumn)
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
	if(!m_isVisible)
	{
		return;
	}

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
		sprite->_angle = 180.0f;
		break;
	case k_moveTop:
		sprite->_angle = -90.0f;
		break;
	case k_moveRight:
		sprite->_angle = 0.0f;
		break;
	case k_moveBottom:
		sprite->_angle = 90.0f;
		break;
	};

	GrafManager::getInstance().drawSprite(*sprite);
}

/****************************************************************************************************************
	Ghost class implementation
****************************************************************************************************************/
Ghost::Ghost(int actorId, IPlatformContext* platform)
	:Character(actorId), m_platform(platform)
{

}

void Ghost::create(TileGrid* tileGrid, const Vector3& position)
{
	Character::create(tileGrid, position);

	TheEventMgr.addEventListener(this, Event_DirectionChanged::k_type);
	TheEventMgr.addEventListener(this, Event_SuperForceOn::k_type);
	TheEventMgr.addEventListener(this, Event_SuperForceOff::k_type);
	TheEventMgr.addEventListener(this, Event_SuperForcePreOff::k_type);
	TheEventMgr.addEventListener(this, Event_CharacterMoved::k_type);
	TheEventMgr.addEventListener(this, Event_CharacterStateChanged::k_type);
}

void Ghost::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_DirectionChanged::k_type)
	{
		Event_DirectionChanged* pEvent = evt->cast<Event_DirectionChanged>();
		if(pEvent->_actorId == m_actorId 
			&& (m_currentState == k_stateChasing || m_currentState == k_statePray))
		{
			m_animations[m_currentAnimation]->suspend();
			m_currentAnimation = pEvent->_newDirection;
			if(m_currentState == k_statePray)
			{
				m_currentAnimation+= k_moveTotalDirections;
			}
			assert(m_currentAnimation >= 0 && m_currentAnimation < k_animationTotal && "invalid animation index"); 
			m_animations[m_currentAnimation]->resume();
		}//if(pEvent->_actorId == m_actorId)
	}//if(evt->getType() == Event_DirectionChanged::k_type)

	if(evt->getType() == Event_SuperForceOn::k_type)
	{
		if(m_currentState != k_statePray)
		{
			m_currentState = k_stateRunaway;
			m_animations[m_currentAnimation]->suspend();
			m_currentAnimation = k_animationRunaway;
			m_animations[m_currentAnimation]->resume();
		}
	}

	if(evt->getType() == Event_SuperForceOff::k_type)
	{
		if(m_currentState != k_statePray)
		{
			m_currentState = k_stateChasing;
			m_animations[m_currentAnimation]->suspend();
			m_currentAnimation = m_currentDirection;
			m_animations[m_currentAnimation]->resume();
		}
	}

	if(evt->getType() == Event_SuperForcePreOff::k_type)
	{
		if(m_currentState == k_stateRunaway)
		{
			m_animations[m_currentAnimation]->suspend();
			m_currentAnimation = k_animationBlink;
			m_animations[m_currentAnimation]->resume();
		}
	}

	if(evt->getType() == Event_CharacterMoved::k_type)
	{
		Event_CharacterMoved* pEvent = evt->cast<Event_CharacterMoved>();
		if(pEvent->_actorId == k_actorPacman)
		{
			m_pacmanRow = pEvent->_row;
			m_pacmanColumn = pEvent->_column;
		}
	}
}

void Ghost::update(float deltaTime)
{
	Character::update(deltaTime);

	if(m_currentState == k_stateChasing || m_currentState == k_stateRunaway)
	{
		int32 currentRow, currentColumn;
		m_tileGrid->pointToCell(m_position._x, m_position._y, currentRow, currentColumn);

		if(currentRow == m_prevRow && currentColumn == m_prevColumn)
		{
			return;
		}
		
		m_prevRow = currentRow;
		m_prevColumn = currentColumn;

		if(currentRow != m_pacmanRow || currentColumn != m_pacmanColumn)
		{
			return;
		}

		int32 actorID = m_currentState == k_stateChasing ? k_actorPacman : m_actorId;
		EventPtr evt(new Event_CharacterKilled(actorID, m_position));
		TheEventMgr.pushEventToQueye(evt);

		if(m_currentState == k_stateRunaway)
		{
			m_currentState = k_statePray;
			m_animations[m_currentAnimation]->suspend();
			m_currentAnimation = m_currentDirection + k_moveTotalDirections;
			m_animations[m_currentAnimation]->resume();
		}	
	}//if(m_currentState == k_stateChasing || m_currentState == k_stateRunaway)
}

void Ghost::draw()
{
	if(!m_isVisible)
	{
		return;
	}

	if(m_currentAnimation < 0 || m_currentAnimation >= k_animationTotal)
		return;

	if(m_animations[m_currentAnimation]->getStatus() == k_processStatusKilled)
		return;

	SpriteAnimation* animation = (SpriteAnimation*)m_animations[m_currentAnimation].get();
	SpriteParameters* sprite =  animation->getSprite();

	sprite->_left = (CURCOORD)(m_position._x - (sprite->_width * 0.5f));
	sprite->_top =	(CURCOORD)(m_position._y - (sprite->_height * 0.5f));
		
	GrafManager::getInstance().drawSprite(*sprite);
}

void Ghost::setAnimation(int state, ProcessPtr animation)
{
	assert(state >= 0 && state < k_animationTotal && "invalid animation key");

	m_animations[state] = animation;
	m_animations[state]->suspend();	
}