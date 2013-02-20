#pragma once

#include "engine.h"
#include "tile_grid.h"
#include "collision_checker.h"

namespace Pegas
{
	enum PacmanCollisionGroups
	{
		k_collisionGroupPacman = 1,
		k_collisionGroupGhost
	};

	class Character: public IColissionHull, public IEventListener
	{
	public:
		Character(TileGrid* tileGrid);

		virtual Vector3 getPosition() { return m_position; }
		virtual float	getRadius() { return m_radius; }

		virtual void	onCollisionEnter(IColissionHull* other);
		virtual void	handleEvent(EventPtr evt);

		virtual void update(float deltaTime);

	protected:
		TileGrid* m_tileGrid;
		Vector3	  m_position;
		Vector3	  m_velosity;
		float	  m_radius;

		enum Commands
		{
			k_commandTurnLeft = 0,
			k_commandTurnRight,
			k_commandTurnTop,
			k_commandTurnBottom
		};

		std::queue<int32> m_commands;
	};

	class Pacman: public Character 
	{
	public:
		virtual int32 getCollisionGroup() { return k_collisionGroupPacman; }
	};

	class Ghost: public Character 
	{
	public:
		virtual int32 getCollisionGroup() { return k_collisionGroupGhost; }
	};
}