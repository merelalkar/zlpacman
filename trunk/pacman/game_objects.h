#pragma once

#include "engine.h"
#include "tile_grid.h"

namespace Pegas
{
	class Character: public IEventListener
	{
	public:
		Character(TileGrid* tileGrid);

		Vector3 getPosition() const { return m_position; }
		float	getRadius() const { return m_radius; }

		virtual void handleEvent(EventPtr evt);
		virtual void update(float deltaTime);

	protected:
		TileGrid* m_tileGrid;
		Vector3	  m_position;
		Vector3	  m_velosity;
		float	  m_radius;

		enum Commands
		{
			k_commandTurnLeft = 0,
			k_commandTurnTop,
			k_commandTurnRight,
			k_commandTurnBottom
		};

		std::queue<int32> m_commands;
		int32 m_currentDirection;
	};

	class Pacman: public Character 
	{
	};

	class Ghost: public Character 
	{
	};
}