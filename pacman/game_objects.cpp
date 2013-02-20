#include "stdafx.h"
#include "game_objects.h"

using namespace Pegas;

/*
	TileGrid* m_tileGird;
		Vector3	  m_position;
		Vector3	  m_velosity;
		float	  m_radius;
*/

void Character::onCollisionEnter(IColissionHull* other)
{

}

void Character::handleEvent(EventPtr evt)
{

}

void Character::update(float deltaTime)
{
	Vector3 newPosition = m_position + (m_velosity * deltaTime);

	if(!m_tileGrid->isObstaclePoint((CURCOORD)newPosition._x, (CURCOORD)newPosition._y))
	{
		m_position = newPosition;
	}else
	{
		stop();
	}

	if(!m_commands.empty())
	{
		int32 command = m_commands.front();
		int32 row, column, nextRow, nextColumn;

		/*
			enum Commands
		{
			k_commandTurnLeft = 0,
			k_commandTurnRight,
			k_commandTurnTop,
			k_commandTurnBottom
		};
		*/
		
		m_tileGrid->pointToCell(m_position._x, m_position._y, row, column);
		switch(command)
		{
		case k_commandTurnLeft:
			nextRow = row;
			nextColumn = column - 1;
			break;
		case k_commandTurnRight:
			nextRow = row;
			nextColumn = column + 1;
			break;
		case k_commandTurnTop:
		case k_commandTurnBottom:
		};
	}
}