#pragma once

#include "engine.h"
#include "tile_grid.h"

namespace Pegas
{
	enum Actors
	{
		k_actorAll = -1,
		k_actorPacman = 1,
		k_actorBlinky,
		k_actorPinky,
		k_actorInky,
		k_actorClyde
	};

	class Character: public IEventListener
	{
	public:
		Character(TileGrid* tileGrid, int actorId);

		Vector3 getPosition() const { return m_position; }
		float	getRadius() const { return m_radius; }

		virtual void handleEvent(EventPtr evt);
		virtual void update(float deltaTime);

	protected:
		int		  m_actorId;	
		TileGrid* m_tileGrid;
		Vector3	  m_position;
		float	  m_radius;

		enum eMovingDirections
		{
			k_moveLeft = 0,
			k_moveTop,
			k_moveRight,
			k_moveBottom
		};

		Vector3 m_directions[4];
		int		m_currentDirection;
		float	m_velocity;
		int		m_turnCommand;

		bool m_isBlocked;
		bool m_isMoving;
	};

	class Pacman: public Character 
	{
	};

	class Ghost: public Character 
	{
	};
}