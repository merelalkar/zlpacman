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

	enum CollisionGroups
	{
		k_collisionGroupPill = 1,
		k_collisionGroupSuperPill,
		k_collisionGroupBonus,
		k_collisionGroupWall,
		k_collisionGroupTonnel
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
	public:
		virtual void handleEvent(EventPtr evt);
		virtual void update(float deltaTime);
		virtual void draw();

	private:
		int32 m_prevRow;
		int32 m_prevColumn;

		enum Animation
		{
			k_animationRunning = 0,
			k_animationDeath,
			k_animationTotal
		};

		IPlatformContext* m_platform;
		ProcessPtr	m_animations[k_animationTotal];
		int32		m_currentAnimation;
	};

	class Ghost: public Character 
	{
	public:
		virtual void handleEvent(EventPtr evt);
		virtual void update(float deltaTime);
	};
}